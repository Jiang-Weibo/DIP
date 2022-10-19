#include "captureimage.h"
#include "ui_captureimage.h"

captureImage::captureImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::captureImage)
{
    ui->setupUi(this);

    setWindowTitle("capture Image");
    m_camera = new QCamera(this);
    m_cameraViewfinder = new QCameraViewfinder();
    m_cameraImageCapture = new QCameraImageCapture(m_camera);
    m_cameraImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    m_camera->setCaptureMode(QCamera::CaptureStillImage);
    m_camera->setViewfinder(m_cameraViewfinder);
    ui->ImageView->addWidget(m_cameraViewfinder);
    connect(m_cameraImageCapture,&QCameraImageCapture::imageCaptured,this,&captureImage::showImage);

}

captureImage::~captureImage()
{
    m_camera->stop();
    delete m_camera;
    delete m_cameraViewfinder;
    delete m_cameraImageCapture;
    delete ui;
}

const QPixmap* captureImage::getPixmap()
{
    return ui->imageLabel->pixmap();
}

void captureImage::showImage(int index, QImage captureImage)
{
    QPixmap pixmap = QPixmap::fromImage(captureImage);
    ui->imageLabel->setScaledContents(true);
    ui->imageLabel->setPixmap(pixmap);
    emit signal(pixmap);
}

void captureImage::on_confirmBtn_clicked()
{
    m_cameraImageCapture->capture();
    //emit signal();
}

void captureImage::on_openCaptureBtn_clicked()
{
     m_camera->start();
}

void captureImage::on_closeCaptureBtn_clicked()
{
    m_camera->stop();
    close();
}

void captureImage::on_saveImageBtn_clicked()
{
    if(!ui->imageLabel->pixmap()){
        QMessageBox::information(this,"warning","no image is capture!");
        return;
    }
    QString name = QFileDialog::getSaveFileName(this,
                                                tr("Open Config"),
                                                "",
                                                tr("Config Files (*.png)"));
    if(name.isEmpty()){
        QMessageBox::information(this,"warning","please input a name.");
        return;
    }
    if(!ui->imageLabel->pixmap()->save(name)){
        QMessageBox::information(this,"error","Fail to save image.");
    }else{
        QMessageBox::information(this,"info","Success to save image.");
    }
}

void captureImage::on_closeBtn_clicked()
{
    m_camera->stop();
    close();
}
