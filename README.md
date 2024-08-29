# Localization and tracking of moving targets by microphones.

The aim of this thesis is to develop a sound localization and tracking system using a four-microphone array to determine the direction and approximate distance of a sound source. Sound localization involves identifying the position of a sound-emitting object, which has diverse applications in fields such as robotics, surveillance, and automated systems. In this project, four microphones are strategically placed at the corners of a 17cm x 17cm square to capture sound waves. The Time Difference of Arrival (TDOA) method is employed to calculate the time it takes for sound waves to reach each microphone, thereby estimating the direction and distance of the sound source. The system uses the calculated time differences and sound intensities to activate an LED corresponding to the nearest microphone, providing a visual indication of the sound's direction. 
A threshold-based approach ensures that only significant sound events are detected, reducing the influence of background noise. The estimated distance to the sound source is calculated using the speed of sound and the time differences, providing real-time feedback. The system has been tested in various controlled environments, demonstrating its ability to consistently identify the direction of the sound source and provide reasonable distance estimates. 
This research outlines the design and implementation of the sound localization system and discusses its performance, highlighting its potential applications in enhancing the capabilities of autonomous systems. Future work could involve refining the distance estimation technique and exploring the system's adaptability to different environmental conditions and sound types.

<small>Keywords: acoustics; sound source localization; robotics; artificial intelligence; microphone arrays</small>