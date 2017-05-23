#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QStandardPaths>
#include "image.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_ComputeButton_clicked();

    void on_LeftLoadButton_clicked();

    void on_RightLoadButton_clicked();

    void on_SaveDirectory_clicked();

private:
    void ComputeDisparityMap();
    QString left_image_path, right_image_path;
    QString base_path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QLabel* status;
    QLabel* mydisp;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
