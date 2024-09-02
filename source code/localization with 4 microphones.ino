// Project Title   : Localization and tracking of moving targets by microphones.
// Purpose         : Implementation of a sound localization system using four microphones to determine the direction of a sound source. 
//                   The code reads analog values from each microphone, averages the readings over a period, and compares the results 
//                   to identify the microphone closest to the sound source. The system activates an LED corresponding to the nearest 
//                   microphone, indicating the direction of the sound. The code also includes functionality to handle button input, 
//                   control a buzzer, and manage the system's state. The setup is designed to be adaptable, allowing for real-time 
//                   sound source detection and localization in a simple and efficient manner.
//
// Language        : Arduino & Matlab
// Author          : Hemant Ramphul
// Github          : https://github.com/hemantramphul/Localization-and-tracking-of-moving-targets-by-microphones/
// Date            : 30 July 2024

int sensorPin1 = A0;        // Microphone 1 is connected to analog pin A0
int sensorPin2 = A1;        // Microphone 2 is connected to analog pin A1
int sensorPin3 = A2;        // Microphone 3 is connected to analog pin A2
int sensorPin4 = A3;        // Microphone 4 is connected to analog pin A3

int LEDON = 2;              // LED indicating the system is on, connected to digital pin 2
int LED1 = 9;               // LED corresponding to Microphone 1, connected to digital pin 9
int LED2 = 12;              // LED corresponding to Microphone 2, connected to digital pin 12
int LED3 = 11;              // LED corresponding to Microphone 3, connected to digital pin 11
int LED4 = 10;              // LED corresponding to Microphone 4, connected to digital pin 10

int buzzerPin = 8;          // Buzzer connected to digital pin 4
int buttonPin = 13;         // Button connected to digital pin 7
int buttonState;            // Variable to store the state of the button

int micValue = 0;           // Placeholder variable, not used in this code
int period = 100;           // Number of samples to take for averaging microphone readings
const int threshold = 150;  // Threshold to check microphone sum
const float speedOfSound = 0.0343; // Speed of sound in cm/us (343 m/s)

float micPositions[4][2] = {  // Positions of the microphones in cm (assuming square setup)
  {0, 0},    // Mic 1 (bottom-left corner)
  {0, 17},   // Mic 2 (top-left corner)
  {17, 17},  // Mic 3 (top-right corner)
  {17, 0}    // Mic 4 (bottom-right corner)
};

// Define an enum for better readability and management of LED states
enum MicLedState {
  NONE = 0,  // No LED
  MIC1 = 1,  // LED1 (Microphone 1)
  MIC2 = 2,  // LED2 (Microphone 2)
  MIC3 = 3,  // LED3 (Microphone 3)
  MIC4 = 4   // LED4 (Microphone 4)
};

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud rate for debugging

  // Set microphone pins as input
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(sensorPin4, INPUT);

  // Set LED pins as output
  pinMode(LEDON, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  // Set buzzer and button pins
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Enable internal pull-up resistor for the button
}

MicLedState determineLed(int mic1, int mic2, int mic3, int mic4) {
  // Use combined comparison and threshold check
  if ((mic1 > mic2) && (mic1 > mic3) && (mic1 > mic4) && (mic1 > threshold)) {
    return MIC1; // Microphone 1 has the highest value and exceeds the threshold
  }
  else if ((mic2 > mic1) && (mic2 > mic3) && (mic2 > mic4) && (mic2 > threshold)) {
    return MIC2; // Microphone 2 has the highest value and exceeds the threshold
  }
  else if ((mic3 > mic1) && (mic3 > mic2) && (mic3 > mic4) && (mic3 > threshold)) {
    return MIC3; // Microphone 3 has the highest value and exceeds the threshold
  }
  else if ((mic4 > mic1) && (mic4 > mic2) && (mic4 > mic3) && (mic4 > threshold)) {
    return MIC4; // Microphone 4 has the highest value and exceeds the threshold
  }
  else {
    return NONE; // No microphone meets the criteria
  }
}

void handleLeds(MicLedState led) {
  // Activate the appropriate LED
  digitalWrite(LED1, led == MIC1);
  digitalWrite(LED2, led == MIC2);
  digitalWrite(LED3, led == MIC3);
  digitalWrite(LED4, led == MIC4);

  if (led != NONE) {
    delay(100); // Keep the LED on for 100ms
  }

  // Turn off all LEDs
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
}

void loop() {
  digitalWrite(LEDON, HIGH); // Keep the system-on LED turned on

  buttonState = digitalRead(buttonPin); // Read the state of the button
  digitalWrite(buzzerPin, buttonState); // Activate the buzzer based on the button state

  if (buttonState == LOW) {
    // Variables to store the time of arrival, initialized to -1 to indicate no detection
    unsigned long time1 = -1, time2 = -1, time3 = -1, time4 = -1;
    bool mic1Detected = false, mic2Detected = false, mic3Detected = false, mic4Detected = false;

    unsigned long startTime = micros(); // Record start time

    // Variables to store the sum of readings from each microphone
    unsigned long sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;

    // Take multiple readings to average out the microphone data
    for (int i = 0; i < period; i++) {
      int read1 = analogRead(sensorPin1);
      int read2 = analogRead(sensorPin2);
      int read3 = analogRead(sensorPin3);
      int read4 = analogRead(sensorPin4);

      sum1 += read1; // Add reading from Microphone 1
      sum2 += read2; // Add reading from Microphone 2
      sum3 += read3; // Add reading from Microphone 3
      sum4 += read4; // Add reading from Microphone 4

      // Record the time of the first detection above the threshold
      if (read1 > threshold && !mic1Detected) {
          time1 = micros() - startTime;
          mic1Detected = true;
      }
      if (read2 > threshold && !mic2Detected) {
          time2 = micros() - startTime;
          mic2Detected = true;
      }
      if (read3 > threshold && !mic3Detected) {
          time3 = micros() - startTime;
          mic3Detected = true;
      }
      if (read4 > threshold && !mic4Detected) {
          time4 = micros() - startTime;
          mic4Detected = true;
      }
    }

    // Calculate the average reading for each microphone
    sum1 /= period;
    sum2 /= period;
    sum3 /= period;
    sum4 /= period;

    calculateAndPrintDistances(time1, time2, time3, time4, sum1, sum2, sum3, sum4);

    // Determine which LED should be activated based on the microphone readings
    MicLedState led = determineLed(sum1, sum2, sum3, sum4);
    handleLeds(led); // Activate the corresponding LED
  }
  delay(100); // Delay before the next loop iteration
}

// Function to calculate the estimated distance to the sound source
float calculateDistance(unsigned long time1, unsigned long time2, unsigned long time3, unsigned long time4) {
  unsigned long minTime = min(time1, min(time2, min(time3, time4)));
  float distance = minTime * speedOfSound; // Distance in cm based on TDOA and speed of sound
  return distance;
}

float calculateIndividualDistance(unsigned long time) {
    return time * speedOfSound; // Distance based on time and speed of sound
}

void calculateAndPrintDistances(unsigned long time1, unsigned long time2, unsigned long time3, unsigned long time4,
                                unsigned long sum1, unsigned long sum2, unsigned long sum3, unsigned long sum4) {
    // Calculate distances based on time
    float dist1 = calculateIndividualDistance(time1);
    float dist2 = calculateIndividualDistance(time2);
    float dist3 = calculateIndividualDistance(time3);
    float dist4 = calculateIndividualDistance(time4);

    // Print distances if they exceed the threshold
    if (sum1 > threshold) {
        Serial.print("Distance to sound source from Mic 1: ");
        Serial.print(dist1);
        Serial.println(" cm");
    }
    if (sum2 > threshold) {
        Serial.print("Distance to sound source from Mic 2: ");
        Serial.print(dist2);
        Serial.println(" cm");
    }
    if (sum3 > threshold) {
        Serial.print("Distance to sound source from Mic 3: ");
        Serial.print(dist3);
        Serial.println(" cm");
    }
    if (sum4 > threshold) {
        Serial.print("Distance to sound source from Mic 4: ");
        Serial.print(dist4);
        Serial.println(" cm");
    }

    // Determine the microphone with the highest intensity sum (i.e., closest sound source)
    int closestMic = 1;
    unsigned long maxSum = sum1;
    if (sum2 > maxSum) {
        closestMic = 2;
        maxSum = sum2;
    }
    if (sum3 > maxSum) {
        closestMic = 3;
        maxSum = sum3;
    }
    if (sum4 > maxSum) {
        closestMic = 4;
        maxSum = sum4;
    }

    // Print the closest microphone and its distance
    Serial.print("The sound source is nearest to Microphone ");
    Serial.print(closestMic);
    Serial.print(" with a sum value of ");
    Serial.println(maxSum);

    // Print the estimated distance to the closest microphone
    float closestDistance;
    switch (closestMic) {
        case 1:
            closestDistance = dist1;
            break;
        case 2:
            closestDistance = dist2;
            break;
        case 3:
            closestDistance = dist3;
            break;
        case 4:
            closestDistance = dist4;
            break;
    }

    Serial.print("Estimated distance to the sound source: ");
    Serial.print(closestDistance);
    Serial.println(" cm");
}
