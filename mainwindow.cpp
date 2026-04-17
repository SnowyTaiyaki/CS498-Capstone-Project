#include "mainwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QProcess>
#include <QComboBox>

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

    // Create the dropdown box
    comboBox = new QComboBox(this);
    comboBox->addItem("Select Test Algorithm");
    comboBox->addItem("Algorithm 1");
    comboBox->addItem("Algorithm 2");
    comboBox->addItem("Algorithm 3");
    comboBox->addItem("Algorithm 4");

    // Add the buttons to the layout
    mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(uploadButton);
    mainLayout->addWidget(videoButton);
    mainLayout->addWidget(comboBox);
    mainLayout->addWidget(processButton);
    mainLayout->addWidget(resultsButton);

    // Set the central widget
    setCentralWidget(centralWidget);

    // Make the buttons functional
    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::openImage);         // Open the image when clicked
    connect(videoButton, &QPushButton::clicked, this, &MainWindow::openVideo);          // Turn on video when clicked
    connect(processButton, &QPushButton::clicked, this, &MainWindow::processImage);     // Test with machine learning when clicked
    connect(resultsButton, &QPushButton::clicked, this, &MainWindow::openResults);      // View results when clicked
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

// Display the user's camera feed
void MainWindow::openVideo()
{
    // // Create a new process for video
    // QProcess *process = new QProcess(this);

    // // Give the path to python installation
    // QString program = "C:/Users/Julie/AppData/Local/Programs/Python/Python311/python.exe";

    // // Give the path to video detection file
    // QString script = "C:/Users/Julie/Desktop/SUNY Poly/QT/ImageGUI/videodetect.py";

    // // Give args to video detection file
    // QStringList arguments;
    // arguments << script;

    // // Start the process detached
    // process->startDetached(program, arguments);

    // // Throw error if video cannot be started
    // if (!process->waitForStarted())
    // {
    //     QMessageBox::warning(this, "Error", "Failed to start video detection.");
    //     return;
    // }

    QMessageBox::information(this, "", "This is a placeholder for the video button.");
}

// Start the machine learning testing of the image
void MainWindow::processImage()
{

    // Give the user an error if there is no image uploaded
    // if (currentImagePath.isEmpty())
    // {
    //     QMessageBox::warning(this, "Error", "Please upload an image first.");
    //     return;
    // }

    // // Save the output into an image file
    // QString outputPath = "output.jpg";

    // // Create a QProcess variable to call the Python function
    // QProcess process;

    // QString program = "python";
    // QStringList arguments;

    // // Use the path to the detectfaces.py file
    // arguments << "C:/Users/Julie/Desktop/SUNY Poly/QT/ImageGUI/detectfaces.py" << currentImagePath << outputPath;

    // // Start the Python program
    // process.start(program, arguments);

    // // If the process does not finish, give the user an error
    // if (!process.waitForFinished())
    // {
    //     QMessageBox::warning(this, "Error", "Failed to run Python script.");
    //     return;
    // }

    // // Get information about the file being read
    // QByteArray output = process.readAllStandardOutput();
    // QByteArray error = process.readAllStandardError();

    // // Display that information in the debug window
    // qDebug() << "Python output:" << output;
    // qDebug() << "Python error:" << error;

    // // Load processed image
    // QPixmap resultPixmap(outputPath);

    // // Make sure the image exists
    // if (!resultPixmap.isNull())
    // {
    //     imageLabel->setPixmap(resultPixmap.scaled(
    //         imageLabel->width(),
    //         imageLabel->height(),
    //         Qt::KeepAspectRatio,
    //         Qt::SmoothTransformation
    //         ));
    // }

    // // If the image doesn't exist, give the user an error
    // else
    // {
    //     QMessageBox::warning(this, "Error", "Failed to load processed image.");
    // }

    QMessageBox::information(this, "", "This is a placeholder for the process button.");
}

// Display the machine learning results
void MainWindow::openResults()
{
    QMessageBox::information(this, "", "This is a placeholder for the results button.");
}
