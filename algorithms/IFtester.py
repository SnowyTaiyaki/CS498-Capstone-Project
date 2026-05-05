


import cv2
import inspireface as isf
import sys


def detectFace(
    image_path: str,
    confidence_threshold: float = 0.5,
    show: bool = True,
    save_path: str | None = None
):
    """
    Detect faces in an image and draw bounding boxes + landmarks.

    Args:
        image_path (str): Path to input image.
        confidence_threshold (float): Detection confidence threshold.
        show (bool): Whether to display the result.
        save_path (str | None): If provided, saves the output image.

    Returns:
        draw (ndarray): Image with drawings.
        faces (list): Detected face objects.
    """
    ret = isf.reload()
    assert ret, "Failure to launch. Ensure resource path is correct."

    # Create session
    session = isf.InspireFaceSession(
        isf.HF_ENABLE_NONE,
        detect_mode=isf.HF_DETECT_MODE_ALWAYS_DETECT
    )

    session.set_detection_confidence_threshold(confidence_threshold)

    # Read image
    image = cv2.imread(image_path)
    if image is None:
        raise ValueError(f"Invalid image path: {image_path}")

    # Detect faces
    faces = session.face_detection(image)
    print(f"Detected {len(faces)} faces")

    # Draw results
    draw = image.copy()

    for face in faces:
        x1, y1, x2, y2 = face.location

        # Rotated bounding box
        center = ((x1 + x2) / 2, (y1 + y2) / 2)
        size = (x2 - x1, y2 - y1)
        angle = face.roll

        rect = (center, size, angle)
        box = cv2.boxPoints(rect).astype(int)
        cv2.drawContours(draw, [box], 0, (100, 180, 29), 2)

        # Landmarks
        landmarks = session.get_face_dense_landmark(face)
        for x, y in landmarks.astype(int):
            cv2.circle(draw, (x, y), 1, (220, 100, 0), 2)

    # Save if needed
    if save_path:
        cv2.imwrite(save_path, draw)

    # Show if needed
    if show:
        cv2.imshow("Face Detection", draw)
        cv2.waitKey(0)
        cv2.destroyAllWindows()

    return draw, faces

if __name__ == "__main__":

    if len(sys.argv) > 1:
        print("########" + sys.argv[1])
        detectFace(sys.argv[1])
    else:
        detectFace("test-images/images.jpeg")
