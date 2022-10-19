#ifndef DIVIDETOOL_H
#define DIVIDETOOL_H

#include <QWidget>
#include <QDebug>
#include <QPixmap>
#include"convertMat2QPixmap.h"
#include"opencv.hpp"
using namespace std;
using namespace cv;
namespace Ui {
class DivideTool;
}

class DivideTool : public QWidget
{
    Q_OBJECT

public:
    explicit DivideTool(int** mask,cv::Mat mat,QWidget *parent = nullptr);
    ~DivideTool();

private slots:
    void on_radioButton_clicked();

    void on_toumingSlider_valueChanged(int value);

    void on_confirmBtn_clicked();

private:
    Ui::DivideTool *ui;
    int** mask;
    Mat mat;
    Mat tempMat;
    QPixmap map;
    QPixmap tempMap;
    QPixmap sendPix;
signals:
    void signal(QPixmap pix);
public slots:
    void slot();
};

#endif // DIVIDETOOL_H
