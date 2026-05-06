from deepface import DeepFace

output = DeepFace.analyze("test-images/images.jpeg", actions=['age', 'gender', 'race', 'emotion'])

print(output)
