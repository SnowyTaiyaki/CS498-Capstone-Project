import sys
import cv2
import inspireface as isf

 # The detectFace function will takes an image uploaded by a user, runs face detection software on it, and saves a new image with the results as output
def detectFace(
    image_path: str,
    output_path: str = "output.jpg",
    confidence_threshold: float = 0.5
):
    """
    Detect faces in an image and save a new image with a rotated bounding box for each face and facial landmarks.
    """

    # Create session with required features enabled
    session = isf.InspireFaceSession(
        opt=isf.HF_ENABLE_NONE,
        detect_mode=isf.HF_DETECT_MODE_ALWAYS_DETECT
    )

    # Set detection confidence threshold
    session.set_detection_confidence_threshold(confidence_threshold)

    # Read image
    image = cv2.imread(image_path)
    if image is None:
        raise ValueError(f"Invalid image path: {image_path}")

    # Perform face detection
    faces = session.face_detection(image)
    print(f"Detected {len(faces)} faces")

    # Draw detection results on image
    draw = image.copy()

    for face in faces:
        x1, y1, x2, y2 = face.location

        # Coordinates for bounding box
        center = ((x1 + x2) / 2, (y1 + y2) / 2)
        size = (x2 - x1, y2 - y1)
        angle = face.roll

        # Draw rotated box
        rect = (center, size, angle)
        box = cv2.boxPoints(rect).astype(int)
        cv2.drawContours(draw, [box], 0, (100, 180, 29), 2)

        # Draw landmarks
        landmarks = session.get_face_dense_landmark(face)
        for x, y in landmarks.astype(int):
            cv2.circle(draw, (x, y), 1, (220, 100, 0), 2)

    # Save output image to the specified output path
    success = cv2.imwrite(output_path, draw)

    # Throw error if the image could not be saved
    if not success:
        raise RuntimeError("Failed to save output image")

    print(f"Saved result to {output_path}")

# Where the script will run
if __name__ == "__main__":

    # Make sure there is an input path
    if len(sys.argv) < 2:
        print("Usage: python face_detect.py <input_image> [output_image]")
        sys.exit(1)

    # Input image path
    input_path = sys.argv[1]

    # Output path if provided
    output_path = "output.jpg"
    if len(sys.argv) >= 3:
        output_path = sys.argv[2]

    try:
        # Run facial recognition
        detectFace(input_path, output_path)

        # Exit
        sys.exit(0)

    # Print errors during processing
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)
