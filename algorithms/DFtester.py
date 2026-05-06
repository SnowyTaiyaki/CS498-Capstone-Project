from deepface import DeepFace

print("HELLO")

output = DeepFace.analyze("/home/foster/CS498-Capstone-Project/tdb/control/images.jpeg", actions=['age', 'gender', 'race', 'emotion'])

print(output)
