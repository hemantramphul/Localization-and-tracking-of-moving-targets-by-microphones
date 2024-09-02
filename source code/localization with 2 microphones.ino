#define SPEED_OF_SOUND_CM 34300.0 // Speed of sound in air in cm/s
#define DISTANCE_BETWEEN_MIC 18.0 // Distance between microphones in centimeters

// Define pins for LEDs and sensors
int leftLedPin = 12;
int rightLedPin = 5;

int leftSensorPin = 7;
int rightSensorPin = 4;

// Variables to store sensor states
bool leftVal = false;
bool rightVal = leftVal;

// Variables to store the time when the sound is detected by each sensor
unsigned long timeLeft;
unsigned long timeRight;
unsigned long timeDifference;

void setup() {
  // Set the LED pins as outputs
  pinMode(leftLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);

  // Set the sensor pins as inputs
  pinMode(leftSensorPin, INPUT);
  pinMode(rightSensorPin, INPUT);

  // Initialize serial communication at 9600 baud
  Serial.begin(9600);
}

// Function to calculate the distance of the sound source based on the time difference
float calculateDistance(unsigned long timeDiff) {
  // Convert the time difference to distance in cm using the formula:
  // distance = (time difference in seconds * speed of sound in cm/s) / 2
  float distance = (timeDiff * 0.000001 * SPEED_OF_SOUND_CM) / 2;
  return distance;
}

void loop() {
  // Read the sensor values
  leftVal = digitalRead(leftSensorPin);
  rightVal = digitalRead(rightSensorPin);

  // Case 1: Neither sensor detects the sound
  if (leftVal == LOW && rightVal == LOW) {
    digitalWrite(leftLedPin, LOW);
    digitalWrite(rightLedPin, LOW);
    // Serial.println("None"); // Optional debug print
  } 
  // Case 2: Right sensor detects the sound first
  else if (leftVal == LOW && rightVal == HIGH) {
    timeRight = micros(); // Record the time when right sensor detects the sound
    while (digitalRead(leftSensorPin) == LOW) {} // Wait until the left sensor detects the sound
    timeLeft = micros(); // Record the time when left sensor detects the sound
    timeDifference = timeLeft - timeRight; // Calculate the time difference

    digitalWrite(leftLedPin, LOW); // Turn off the left LED
    digitalWrite(rightLedPin, HIGH); // Turn on the right LED
    Serial.println("Right"); // Print "Right" to indicate the sound was detected first on the right

    // Calculate the distance to the sound source
    float distance = calculateDistance(timeDifference);
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(500); // Delay for half a second before the next reading
  } 
  // Case 3: Left sensor detects the sound first
  else if (leftVal == HIGH && rightVal == LOW) {
    timeLeft = micros(); // Record the time when left sensor detects the sound
    while (digitalRead(rightSensorPin) == LOW) {} // Wait until the right sensor detects the sound
    timeRight = micros(); // Record the time when right sensor detects the sound
    timeDifference = timeRight - timeLeft; // Calculate the time difference

    digitalWrite(leftLedPin, HIGH); // Turn on the left LED
    digitalWrite(rightLedPin, LOW); // Turn off the right LED
    Serial.println("Left"); // Print "Left" to indicate the sound was detected first on the left

    // Calculate the distance to the sound source
    float distance = calculateDistance(timeDifference);
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(500); // Delay for half a second before the next reading
  } 
  // Case 4: Both sensors detect the sound simultaneously
  else {
    digitalWrite(leftLedPin, HIGH); // Turn on both LEDs
    digitalWrite(rightLedPin, HIGH);
    Serial.println("Both"); // Print "Both" to indicate simultaneous detection
  }
}
