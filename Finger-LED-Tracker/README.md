Finger LED Tracker

Controls 5 individual LEDs in real time using hand tracking from a webcam.
Each LED corresponds to one finger — raise a finger, its LED lights up instantly.

## Photos


## How It Works
1. Python captures webcam feed and runs MediaPipe hand landmark detection
2. Finger states (up/down) are encoded as a 5-bit string e.g. `10110`
3. String is sent over USB serial to the Arduino
4. Arduino maps each bit to an LED pin

## Hardware
- Arduino Uno
- 5 green LEDs
- 5x 220Ω resistors
- Breadboard + jumper wires
- Webcam

## Wiring
| Finger | Arduino Pin |
|--------|------------|
| Thumb  | 8          |
| Index  | 9          |
| Middle | 10         |
| Ring   | 11         |
| Pinky  | 12         |

## Software
- Python 3.11
- opencv-python
- mediapipe 0.10.30+
- pyserial

## Setup
1. Upload `finger_led_tracker.ino` to the Arduino
2. Install dependencies: `pip install opencv-python mediapipe pyserial`
3. Update `COM5` in `hand_tracker.py` to your Arduino's port
4. Run `python hand_tracker.py`

## Challenges
- No prior Python experience — had to learn syntax, debugging, and how
  to run scripts from the terminal from scratch while building this
- MediaPipe's newer versions dropped the `solutions` API requiring a
  rewrite to use the Tasks API
- Python 3.13 incompatibility with MediaPipe — had to install Python 3.11
  specifically to get it working
- Learning to read error messages and fix dependency issues independently

## Future Plans
- Swap LEDs for servos to control a physical robot hand
- Add continuous bend angle tracking for smoother servo control
