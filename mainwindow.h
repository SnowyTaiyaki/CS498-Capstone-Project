#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

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
    // Allows user to select an image and then displays the selected image
    void openImage();

private:
    QWidget *centralWidget;         // Holds UI components
    QVBoxLayout *mainLayout;        // Vertical layout
    QPushButton *uploadButton;      // Allows the user to upload an image
    QLabel *imageLabel;             // Label used to display the image
};

#endif // MAINWINDOW_H
