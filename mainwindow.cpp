#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "image.h"
#include "depth.h"
#include "qimagemaker.h"
#include <QCoreApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QtConcurrent/QtConcurrent>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mydisp = new QLabel(this);
    mydisp->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ComputeDisparityMap(){
    int min_disparity = ui->MinDisparity->text().toInt();
    int max_disparity = ui->MaxDisparity->text().toInt();
    int window_size = ui->WindowSize->text().toInt();

    QImage left_image(left_image_path);
    QImage right_image(right_image_path);
    mycv::Image left(left_image);
    mycv::Image right(right_image);
    auto disparity = mycv::getDisparityMap(left, right, window_size, min_disparity, max_disparity);
    disparity = disparity.getNormalized();
    QImage qimage_disp = mycv::toQImage(disparity);

    QPixmap MYpm = QPixmap::fromImage(qimage_disp);
    MYpm = MYpm.scaled(300, 300, Qt::KeepAspectRatio, Qt::FastTransformation);
    mydisp->setPixmap(MYpm);

    QString str = base_path;
    str += "\/selfmade_disparity_map.png";
    int i = 1;
    while(QFileInfo::exists(str)){
        str = base_path + "\/selfmade_disparity_map(" + QString::number(i) + ").png";
        i++;
    }
    qimage_disp.save(str);
    status->setText("Done, disparity maps saved to " + base_path);
}

void MainWindow::on_ComputeButton_clicked()
{
    int min_disparity = ui->MinDisparity->text().toInt();
    int max_disparity = ui->MaxDisparity->text().toInt();
    int window_size = ui->WindowSize->text().toInt();

    if(min_disparity > max_disparity) {
        QMessageBox::critical(this, "Wrong parameters", "Minimum disparity greater than maximum disparity");
        return;
    }
    if(window_size % 2 != 1 || window_size < 1) {
        QMessageBox::critical(this, "Wrong parameters", "Wrong window size (must be positive odd number)");
        return;
    }
    if(max_disparity % 16 != 0) {
        QMessageBox::critical(this, "Wrong parameters", "Wrong maximum disparity (must be positive number divisible by 16)");
        return;
    }

    if(left_image_path == 0 || right_image_path == 0) {
        QMessageBox::critical(this, "No images", "Images are not loaded");
        return;
    }
    status = new QLabel(this);
    status->setText("Computing...");
    ui->statusBar->addWidget(status);
    cv::Mat img1, img2, g1, g2;
    cv::Mat disp, disp8;
    img1 = cv::imread(left_image_path.toStdString().c_str());
    img2 = cv::imread(right_image_path.toStdString().c_str());
    cv::cvtColor(img1, g1, CV_BGR2GRAY);
    cv::cvtColor(img2, g2, CV_BGR2GRAY);

    cv::Ptr<cv::StereoBM> sbm = cv::StereoBM::create(0,0);
    sbm->setMinDisparity(min_disparity);
    sbm->setBlockSize(window_size);
    sbm->setNumDisparities(max_disparity);
    sbm->setDisp12MaxDiff(5);
    sbm->setUniquenessRatio(5);
    sbm->setSpeckleWindowSize(100);
    sbm->setSpeckleRange(10);
    sbm->compute(g1, g2, disp);
    cv::normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);

    QImage img((uchar*) disp8.data, disp8.cols, disp8.rows, disp8.step, QImage::Format_Grayscale8);
    QLabel* OCVlabel = new QLabel(this);
    QPixmap OCVpm = QPixmap::fromImage(img);
    OCVpm = OCVpm.scaled(300, 300, Qt::KeepAspectRatio, Qt::FastTransformation);
    OCVlabel->setGeometry(10, 360, OCVpm.width(), OCVpm.height());
    mydisp->setGeometry(330, 360, OCVpm.width(), OCVpm.height());
    OCVlabel->setPixmap(OCVpm);
    OCVlabel->show();

    QFuture<void> future = QtConcurrent::run(this, &MainWindow::ComputeDisparityMap);

    QString str;
    str = base_path;
    str += "\/ocv_disparity_map.png";
    int i = 1;
    while(QFileInfo::exists(str)){
        str = base_path + "\/ocv_disparity_map(" + QString::number(i) + ").png";
        i++;
    }
    img.save(str);
    mydisp->show();
}

void MainWindow::on_LeftLoadButton_clicked()
{
    QString filter("Images (*.png *.bmp *.jpg *.jpeg *.tiff *.gif *.xmp)");
    left_image_path = QFileDialog::getOpenFileName(0, "Load Left Image", QDir::currentPath(), filter);
    QImage left_image(left_image_path);
    QLabel* label = new QLabel(this);
    QPixmap pm = QPixmap::fromImage(left_image);
    pm = pm.scaled(300, 300, Qt::KeepAspectRatio, Qt::FastTransformation);
    label->setGeometry(10, 30, pm.width(), pm.height());
    label->setPixmap(pm);
    label->show();
}

void MainWindow::on_RightLoadButton_clicked()
{
    QString filter("Images (*.png *.bmp *.jpg *.jpeg *.tiff *.gif *.xmp)");
    right_image_path = QFileDialog::getOpenFileName(0, "Load Right Image", QDir::currentPath(), filter);
    QImage right_image(right_image_path);
    QLabel* label = new QLabel(this);
    QPixmap pm = QPixmap::fromImage(right_image);
    pm = pm.scaled(300, 300, Qt::KeepAspectRatio, Qt::FastTransformation);
    label->setGeometry(330, 30, pm.width(), pm.height());
    label->setPixmap(pm);
    label->show();
}

void MainWindow::on_SaveDirectory_clicked()
{
    base_path = QFileDialog::getExistingDirectory(this, tr("Set save directory"), base_path);
}
