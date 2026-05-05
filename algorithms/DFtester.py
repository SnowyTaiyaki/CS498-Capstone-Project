

from deepface import DeepFace

fish = DeepFace.analyze("test-images/images.jpeg", actions=['age', 'gender', 'race', 'emotion'])

print(fish)
