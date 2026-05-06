#include "mainwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QProcess>
#include <QComboBox>
#include <QLineEdit>
#include <QCoreApplication>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QImageCapture>
#include <QTimer>

// Initializes main window and UI components
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create a central widget for all other widgets to reside
    centralWidget = new QWidget(this);

    // Use a vertical layout
    mainLayout = new QVBoxLayout(centralWidget);

    // Tell the user an image has not been uploaded yet
    imageLabel = new QLabel(this);
    imageLabel->setText("No image loaded.");

    videoWidget = new QVideoWidget(this);
    videoWidget->setMinimumSize(200, 200);
    videoWidget->hide();

    // Center the text
    imageLabel->setAlignment(Qt::AlignCenter);

    // Allow scaling of the image
    imageLabel->setScaledContents(false);
    imageLabel->setMinimumSize(200, 200);

    // Create the buttons
    uploadButton = new QPushButton("Upload Image", this);
    videoButton = new QPushButton("Use Video Feed", this);
    processButton = new QPushButton("Process Image", this);
    resultsButton = new QPushButton("View Results", this);
    captureButton = new QPushButton("Take Picture", this);

    // Create image capture variables
    imageCapture = nullptr;
    camera = nullptr;
    captureSession = nullptr;

    // Create the dropdown box (combobox)
    comboBox = new QComboBox(this);

    // Center the text in the combobox
    comboBox->setEditable(true);
    comboBox->lineEdit()->setReadOnly(true);
    comboBox->lineEdit()->setAlignment(Qt::AlignCenter);

    // Populate the combobox
    comboBox->addItem("Select Test Algorithm");
    comboBox->addItem("DeepFace");
    comboBox->addItem("FaceNet");
    comboBox->addItem("InspireFace");

    // Add the buttons and combobox to the layout
    mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(videoWidget);
    mainLayout->addWidget(uploadButton);
    mainLayout->addWidget(videoButton);
    mainLayout->addWidget(captureButton);
    mainLayout->addWidget(comboBox);
    mainLayout->addWidget(processButton);
    mainLayout->addWidget(resultsButton);

    // Space out the buttons and combobox
    mainLayout->setSpacing(5);

    // Set the central widget
    setCentralWidget(centralWidget);

    // Make the buttons functional
    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::openImage);         // Upload an image when clicked
    connect(videoButton, &QPushButton::clicked, this, &MainWindow::openVideo);          // Turn on video when clicked
    connect(processButton, &QPushButton::clicked, this, &MainWindow::processImage);     // Test with selected machine learning algorithm when clicked
    connect(resultsButton, &QPushButton::clicked, this, &MainWindow::openResults);      // View test results when clicked

    connect(captureButton, &QPushButton::clicked, this, [=]()
            {
                if (!imageCapture)
                {
                    qDebug() << "ImageCapture not initialized";
                    return;
                }

                if (!imageCapture->isReadyForCapture())
                {
                    qDebug() << "Camera not ready";
                    return;
                }

                imageCapture->capture();
            });
}

// Destructor
MainWindow::~MainWindow() {}

// Opens explorer to let the user upload an image, then proceeds to display the selected image
void MainWindow::openImage()
{
    // Filter explorer to only look for image-based file types
    QString filename = QFileDialog::getOpenFileName
        (
            this,                                           // Parent widget
            tr("Open Image"),                               // Dialog box title
            "",                                             // Initial file location
            tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)")   // Filter file types
            );

    // If the user uploaded a file, display the image using a pixmap and label
    if (!filename.isEmpty())
    {
        currentImagePath = filename;

        QPixmap pixmap(filename);

        if (!pixmap.isNull())
        {
            originalPixmap = pixmap; // Store the original image for scaling

            // Scale image to fit label
            imageLabel->setPixmap(originalPixmap.scaled
            (
                imageLabel->width(),
                imageLabel->height(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            ));

            // Clear text
            imageLabel->setText("");
        }

        else
        {
            // Debug if image fails to be uploaded and tell the user the image failed to load
            qDebug() << "Failed to load image:" << filename;
            imageLabel->setText("Failed to load image");
        }
    }
}

// Allow the image to actually scale with the window
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    // Use the original image and scale that
    if (!originalPixmap.isNull())
    {
        // Keep the image uniform
        imageLabel->setPixmap(originalPixmap.scaled
        (
            imageLabel->width(),
            imageLabel->height(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        ));
    }
}

// Display camera feed
void MainWindow::openVideo()
{
    imageLabel->hide();
    videoWidget->show();

    if (!camera)
    {
        // Initialize camera
        camera = new QCamera(this);

        // Initialize image capture
        imageCapture = new QImageCapture(this);

        // Connect camera and video to video session
        captureSession = new QMediaCaptureSession(this);

        // Attach camera and image capture to video session
        captureSession->setCamera(camera);
        captureSession->setImageCapture(imageCapture);

        // Connect the captured image to handler
        connect(imageCapture, &QImageCapture::imageCaptured,
                this, &MainWindow::onImageCaptured);
    }

    //Connect capture session to video widget
    captureSession->setVideoOutput(videoWidget);

    // Start camera feed
    camera->start();
}

// Handles image captured from camera
void MainWindow::onImageCaptured(int, const QImage &image)
{
    // Turn captured photo into a pixmap for display
    QPixmap pixmap = QPixmap::fromImage(image);

    // Make sure the image exists
    if (!pixmap.isNull())
    {
        // Store original image for scaling
        originalPixmap = pixmap;

        // Display image in main UI window
        imageLabel->setPixmap(originalPixmap.scaled(
            imageLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            ));

        // Save image to disk for use later
        QString savePath = QCoreApplication::applicationDirPath() + "/captured.jpg";
        image.save(savePath);

        // Update image path to use the photo taken
        currentImagePath = savePath;

        // Display path of image
        qDebug() << "Saved captured image to:" << savePath;
    }

    // Stop camera if running
    if (camera)
        camera->stop();

    // Disconnect video output
    if (captureSession)
        captureSession->setVideoOutput(nullptr);

    // Switch back to image display from camera feed
    QTimer::singleShot(0, this, [=]()
                       {
                           videoWidget->hide();
                           imageLabel->show();
                       });
}

// Destroys the camera
void MainWindow::stopCamera()
{
    // Detach video
    if (captureSession)
        captureSession->setVideoOutput(nullptr);

    // Stop camera
    if (camera)
        camera->stop();

    // Hide video widget
    videoWidget->hide();

    // Delete video widget
    videoWidget->deleteLater();

    // Recreate video widget
    videoWidget = new QVideoWidget(this);

    // Insert the new video widget
    mainLayout->insertWidget(1, videoWidget);
}

// Start the machine learning testing of the image
void MainWindow::processImage()
{
    // Initialize process
    QProcess process;

    // Start the process
    process.start();

    // Make sure the process has time to start
    process.waitForFinished(5000);

    QString selectedAlgorithm = comboBox->currentText();

    switch (comboBox->currentIndex())
    {

        case 1:
        {
            // Process using Algorithm 1 (DeepFace)
            QProcess process;

            // Get executable directory
            QDir dir(QCoreApplication::applicationDirPath());

            // Move to the directory the algorithm is in
            dir.cdUp();
            dir.cdUp();

            // Point to Python script
            QString scriptPath = dir.filePath("DFtester.py");
            scriptPath = QDir::cleanPath(scriptPath);

            // Display script location
            qDebug() << "Running Python script:";
            qDebug() << scriptPath;

            // Chcek if the script exists
            if (!QFile::exists(scriptPath))
            {
                qDebug() << "ERROR: Script not found!";
                break;
            }

            // Setup process
            process.setProgram("python");
            process.setArguments(QStringList()
                                 << scriptPath
                                 << currentImagePath
                                );

            // Get the working directory
            process.setWorkingDirectory(QFileInfo(scriptPath).absolutePath());

            // Start the actual process
            process.start();

            // Wait for the process to finish, if it doesn't, produce an error.
            if (!process.waitForFinished(5000))
            {
                qDebug() << "Python process failed:" << process.errorString();
            }

            // Display script contents
            qDebug() << "" << process.readAllStandardOutput();

            break;
        }

        case 2:
        {
            // Process using Algorithm 2 (FaceNet)
            QProcess process;

            // Get executable directory
            QDir dir(QCoreApplication::applicationDirPath());

            // Move to the directory the algorithm is in
            dir.cdUp();
            dir.cdUp();

            // Point to Python script
            QString scriptPath = dir.filePath("FNtester.py");
            scriptPath = QDir::cleanPath(scriptPath);

            // Display script location
            qDebug() << "Running Python script:";
            qDebug() << scriptPath;

            // Chcek if the script exists
            if (!QFile::exists(scriptPath))
            {
                qDebug() << "ERROR: Script not found!";
                break;
            }

            // Setup process
            process.setProgram("python");
            process.setArguments(QStringList()
                                 << scriptPath
                                 << currentImagePath
                                 );

            // Get the working directory
            process.setWorkingDirectory(QFileInfo(scriptPath).absolutePath());

            // Start the actual process
            process.start();

            // Wait for the process to finish, if it doesn't, produce an error.
            if (!process.waitForFinished(5000))
            {
                qDebug() << "Python process failed:" << process.errorString();
            }

            // Display script contents
            qDebug() << "" << process.readAllStandardOutput();
            break;
        }
        case 3:
        {
            // Process using Algorithm 3 (InspireFace)
            QProcess process;

            // Get executable directory
            QDir dir(QCoreApplication::applicationDirPath());

            // Move to the directory the algorithm is in
            dir.cdUp();
            dir.cdUp();

            // Point to Python script
            QString scriptPath = dir.filePath("IFtester.py");
            scriptPath = QDir::cleanPath(scriptPath);

            // Display script location
            qDebug() << "Running Python script:";
            qDebug() << scriptPath;

            // Chcek if the script exists
            if (!QFile::exists(scriptPath))
            {
                qDebug() << "ERROR: Script not found!";
                break;
            }

            // Setup process
            process.setProgram("python");
            process.setArguments(QStringList()
                                 << scriptPath
                                 << currentImagePath
                                 );

            // Get the working directory
            process.setWorkingDirectory(QFileInfo(scriptPath).absolutePath());

            // Start the actual process
            process.start();

            // Wait for the process to finish, if it doesn't, produce an error.
            if (!process.waitForFinished(5000))
            {
                qDebug() << "Python process failed:" << process.errorString();
            }

            // Display script contents
            qDebug() << "" << process.readAllStandardOutput();

            break;
        }
        default:
            // If no algorithm was selected, ask the user to select one.
            qDebug() << "Algorithm not selected.";
            QMessageBox::warning(this, "Error", "Please select an algorithm.");
            return;
    }
}

// Display the machine learning results
void MainWindow::openResults()
{
    QMessageBox::information(this, "", "This is a placeholder for the results button.");
}
