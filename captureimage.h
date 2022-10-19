#ifndef CAPTUREIMAGE_H
#define CAPTUREIMAGE_H

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
namespace Ui {
class captureImage;
}

class captureImage : public QWidget
{
    Q_OBJECT

public:
    explicit captureImage(QWidget *parent = nullptr);
    ~captureImage();
    const QPixmap* getPixmap();
signals:
    void signal(QPixmap map);
public slots:
    void showImage(int index,QImage captureImage);
private slots:
    void on_confirmBtn_clicked();
    void on_openCaptureBtn_clicked();

    void on_closeCaptureBtn_clicked();

    void on_saveImageBtn_clicked();

    void on_closeBtn_clicked();

private:
    Ui::captureImage *ui;
    QCamera* m_camera = nullptr;
    QCameraViewfinder* m_cameraViewfinder = nullptr;
    QCameraImageCapture* m_cameraImageCapture = nullptr;
};

#endif // CAPTUREIMAGE_H
