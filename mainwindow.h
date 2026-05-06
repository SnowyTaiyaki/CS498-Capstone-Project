#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QImageCapture>

namespace Ui {class MainWindow;}

// Main application window
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    // Constructor
    MainWindow(QWidget *parent = nullptr);

     // Destructor
    ~MainWindow();

private slots:
    void openImage();               // Allows user to select an image and then displays the selected image
    void openVideo();               // Allows the user to use video instead of image
    void processImage();            // Allows the user to test with machine learning
    void openResults();             // Allows the user to open the results window
    void stopCamera();              // Stop the camera
    void onImageCaptured(int id, const QImage &image);         // Allow image to be captured from camera

private:
    Ui::MainWindow *ui;

    QWidget     *centralWidget;     // Holds UI components

    QVBoxLayout *mainLayout;        // Vertical layout

    QComboBox *comboBox;            // Drop down box

    QPushButton *uploadButton;      // Allows the user to upload an image
    QPushButton *videoButton;       // Allow to user to turn on video
    QPushButton *processButton;     // Allow the user to begin testing with machine learning
    QPushButton *resultsButton;     // Allow the user to view the results after the image has been tested
    QPushButton *captureButton;      // Allow the user to take a photo from video feed

    QLabel      *imageLabel;        // Label used to display the image

    QPixmap originalPixmap;         // Unscaled original image

    QString currentImagePath;       // Current image path

    QCamera *camera = nullptr;                          // Camera
    QMediaCaptureSession *captureSession = nullptr;     // Video session
    QVideoWidget *videoWidget = nullptr;                // Video widget
    QImageCapture *imageCapture = nullptr;              // Allow image capture from video

protected:
    // Allow the image to be scaled
     void resizeEvent(QResizeEvent *event) override;
};

#endif // MAINWINDOW_H
