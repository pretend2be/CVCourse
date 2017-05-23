#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    QString left_image_path, right_image_path;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
