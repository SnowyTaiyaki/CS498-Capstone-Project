#include "mainwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>

// Initializes main window and UI components
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create a central widget for all other widgets to reside
    centralWidget = new QWidget(this);

    // Use a vertical layout
    mainLayout = new QVBoxLayout(centralWidget);

    // Tell the user an image hasn't been uploaded yet
    imageLabel = new QLabel(this);
    imageLabel -> setText("No image loaded");

    // Center the text
    imageLabel -> setAlignment(Qt::AlignCenter);

    // Allow the image to scale with the label
    imageLabel -> setScaledContents(true);
    imageLabel -> setMinimumSize(200, 200);

    // Create a button that lets the user upload an image
    uploadButton = new QPushButton("Upload Image", this);

    // Add widgets to the layout
    mainLayout -> addWidget(imageLabel);
    mainLayout -> addWidget(uploadButton);

    // Set the central widget
    setCentralWidget(centralWidget);

    // Connect the button to allow the image to open
    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::openImage);
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

    // If user uploaded a file, display the image using a pixmap and label
    if (!filename.isEmpty())
    {
        QPixmap pixmap(filename);

        if (!pixmap.isNull())
        {
            // Allow the image to be scaled (KeepAspectRatio isn't working -- will look into it more)
            imageLabel -> setPixmap(pixmap.scaled(imageLabel -> size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

            // Clear the text that tells the user they haven't uploaded an image yet
            imageLabel -> setText("");
        }

        else
        {
            // Debug if image fails to be uploaded and tell the user the image failed to load
            qDebug() << "Failed to load image:" << filename;
            imageLabel -> setText("Failed to load image");
        }
    }
}
