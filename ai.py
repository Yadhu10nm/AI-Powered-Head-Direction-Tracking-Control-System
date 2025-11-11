import cv2
import mediapipe as mp
import serial
import time

arduino = serial.Serial('COM3', 9600)
time.sleep(2)


mp_face_mesh = mp.solutions.face_mesh
cap = cv2.VideoCapture(0)

prev_dir = None

with mp_face_mesh.FaceMesh(
        max_num_faces=1,
        refine_landmarks=True,
        min_detection_confidence=0.5,
        min_tracking_confidence=0.5
) as face_mesh:
    while cap.isOpened():
        success, image = cap.read()
        if not success:
            continue

        # Flip image and convert to RGB
        image = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)
        image.flags.writeable = False
        results = face_mesh.process(image)
        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        if results.multi_face_landmarks:
            for face_landmarks in results.multi_face_landmarks:
                h, w, _ = image.shape

                # Key landmarks
                nose_tip = face_landmarks.landmark[1]
                left_cheek = face_landmarks.landmark[234]
                right_cheek = face_landmarks.landmark[454]

                # Convert to pixel coords
                nose_x = int(nose_tip.x * w)
                left_x = int(left_cheek.x * w)
                right_x = int(right_cheek.x * w)

                # Calculate midline (center of face)
                face_center_x = (left_x + right_x) // 2

                # Draw key points
                # cv2.circle(image, (nose_x, int(nose_tip.y * h)), 5, (0, 255, 0), -1)
                # cv2.circle(image, (left_x, int(left_cheek.y * h)), 5, (255, 0, 0), -1)
                # cv2.circle(image, (right_x, int(right_cheek.y * h)), 5, (0, 0, 255), -1)

                # Compare nose position relative to face center
                offset = nose_x - face_center_x

                if offset > 20:
                    direction = "Right"

                    if prev_dir != direction:
                        arduino.write("R".encode())

                elif offset < -20:
                    direction = "Left"
                    if prev_dir != direction:
                       arduino.write("L".encode())

                else:
                    direction = "Center"
                    if prev_dir != direction:
                         arduino.write("C".encode())

                # Display direction on frame
                cv2.putText(image, f"Direction: {direction}", (30, 50),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)
                # print(direction)

                prev_dir = direction

        cv2.imshow("Head Direction Tracker", image)
        if cv2.waitKey(5) & 0xFF == 27:
            break

cap.release()
cv2.destroyAllWindows()
