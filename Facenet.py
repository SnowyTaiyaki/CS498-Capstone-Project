import cv2
import torch
from facenet_pytorch import MTCNN, InceptionResnetV1

# 1. Initialize Models
# MTCNN detects the face; InceptionResnetV1 is the FaceNet "brain"
device = torch.device('cuda:0' if torch.cuda.is_available() else 'cpu')
mtcnn = MTCNN(keep_all=True, device=device)
resnet = InceptionResnetV1(pretrained='vggface2').eval().to(device)

video = cv2.VideoCapture(0)

while True:
    ret, frame = video.read()
    if not ret: break

    # 2. Detect Faces
    # returns bounding boxes and the faces as tensors
    boxes, _ = mtcnn.detect(frame)
    faces = mtcnn(frame)

    if boxes is not None:
        for i, box in enumerate(boxes):
            # Draw the box on the screen
            cv2.rectangle(frame, (int(box[0]), int(box[1])), (int(box[2]), int(box[3])), (0, 255, 0), 2)
            
            # 3. Get FaceNet Embedding (The unique number string)
            if faces is not None:
                face_embedding = resnet(faces[i].unsqueeze(0).to(device))
                # Now 'face_embedding' is a 512-dimension vector representing that person!
                print(f"Face {i} recognized. Embedding shape: {face_embedding.shape}")

    cv2.imshow('FaceNet Test', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

video.release()
cv2.destroyAllWindows()