import cv2
import mediapipe as mp
from mediapipe.tasks import python
from mediapipe.tasks.python import vision
import serial
import time
import urllib.request
import os

# Download the hand landmarker model if not already present
model_path = 'hand_landmarker.task'
if not os.path.exists(model_path):
    print("Downloading hand landmarker model...")
    urllib.request.urlretrieve(
        'https://storage.googleapis.com/mediapipe-models/hand_landmarker/hand_landmarker/float16/1/hand_landmarker.task',
        model_path
    )
    print("Downloaded!")

# Update this to your Arduino's port
ser = serial.Serial('COM5', 9600, timeout=1)
time.sleep(2)

# Setup hand landmarker
base_options = python.BaseOptions(model_asset_path=model_path)
options = vision.HandLandmarkerOptions(
    base_options=base_options,
    num_hands=1
)
detector = vision.HandLandmarker.create_from_options(options)

cap = cv2.VideoCapture(0)
last_cmd = ""

def get_finger_states(landmarks):
    fingers = []
    # Thumb
    fingers.append(landmarks[4].x < landmarks[3].x)
    # Other four fingers: tip y above pip joint y = extended
    for tip_id in [8, 12, 16, 20]:
        fingers.append(landmarks[tip_id].y < landmarks[tip_id - 2].y)
    return fingers

while True:
    ret, frame = cap.read()
    frame = cv2.flip(frame, 1)

    # Convert to MediaPipe image
    rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=rgb)

    result = detector.detect(mp_image)

    if result.hand_landmarks:
        landmarks = result.hand_landmarks[0]

        # Draw landmarks manually
        h, w, _ = frame.shape
        for lm in landmarks:
            cx, cy = int(lm.x * w), int(lm.y * h)
            cv2.circle(frame, (cx, cy), 5, (0, 255, 0), -1)

        states = get_finger_states(landmarks)
        cmd = ''.join(['1' if f else '0' for f in states])

        names = ['Thumb', 'Index', 'Middle', 'Ring', 'Pinky']
        for i, (name, state) in enumerate(zip(names, states)):
            color = (0, 255, 0) if state else (0, 0, 255)
            cv2.putText(frame, f"{name}: {'UP' if state else 'DOWN'}",
                        (10, 40 + i * 30), cv2.FONT_HERSHEY_SIMPLEX,
                        0.7, color, 2)

        if cmd != last_cmd:
            ser.write((cmd + '\n').encode())
            last_cmd = cmd
    else:
        if last_cmd != "00000":
            ser.write(b"00000\n")
            last_cmd = "00000"

    cv2.imshow("Finger LED Tracker", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
ser.close()