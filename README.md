# CS498-Capstone-Project
Collaborative project using Machine Learning (ML) to process facial recognition using an uploaded image, or ideally, a live video feed.

# Features
The application includes 5 major options for use. The 'Upload Image' button allows for a file upload. The 'Use Video Feed' button attempts to connect to a webcam for the purpose
of live video stream input. The 'Select Test Algorithm' drop down allows for the user to choose a facial recognition algorithm to choose from. The 'Process Image' button
executes the chosen algorithm. 'View Results' displays the results of the algorithm.

# Libraries / Sources
UI Development: <br>
https://www.qt.io/development <br>
Scope of use: QT Educational End User License, not for commericial, military, or defense use or implimentation
<br>

## Modules/Algorithms:
https://www.insightface.ai/ <br>
https://github.com/HyperInspire/InspireFace <br>
https://github.com/timesler/facenet-pytorch <br>
https://github.com/serengil/deepface <br>
https://opencv.org/ <br>
https://cmake.org/ <br>

#Project Notes
The python algorithms DFtester and IFtester can both be run on their own, and show results. DF (DeepFace) produces a terminal output with the anaylsis function as demonstrated. IF (InspireFace) produces an image overlay with all the landmarking noted.
Two separate python environments should be set up: 1 for DF and IF and one for FN (FaceNet). This is due to FN requiring a numpy version lower than 2.0 which goes against python 3.12 standards and so an older version of python is needed. That being said, the software may still run into general issues even with the fully separate environment.

modules to install: opencv-python, deepface, inspireface, torch (pytorch), facenet-pytorch, tf-keras, numpy

<br>



