#include "dividetool.h"
#include "ui_dividetool.h"

DivideTool::DivideTool(int** mask,Mat mat,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DivideTool)
{
    ui->setupUi(this);
    connect(this,&DivideTool::signal,this,&DivideTool::slot);
    this->setAttribute(Qt::WA_DeleteOnClose);
    //Keep in mind that mask[][] is 1 channel type
    this->mask = mask;
    mat.copyTo(this->mat);
    mat.copyTo(tempMat);
    map = CVS::cvMatToQPixmap(this->mat);

    this->map = QPixmap(map);
    ui->showLabel->setPixmap(this->map);
    ui->showLabel->setScaledContents(true);
}

DivideTool::~DivideTool()
{
    delete ui;
}

void DivideTool::on_radioButton_clicked()
{
    if(ui->radioButton->isChecked()) {
        Mat t;
        mat.copyTo(t);
        cvtColor(t,t,COLOR_BGR2BGRA);
        for (int i = 1; i < t.rows; i++) {
            for (int j = 1; j < t.cols; j++) {
                if(mask[i][j]!=257 && mask[i][j]!=0) {
                    t.at<Vec4b>(i, j)[3]=255;
                }else{
                    t.at<Vec4b>(i, j)[3]=0;//0 is transparent
                    t.at<Vec4b>(i,j)[0]=255;
                    t.at<Vec4b>(i,j)[1]=255;
                    t.at<Vec4b>(i,j)[2]=255;
                }
            }
        }
        cvtColor(t,t,COLOR_BGRA2BGR);
        t.copyTo(tempMat);
        ui->showLabel->setPixmap(CVS::cvMatToQPixmap(tempMat));
        this->sendPix = QPixmap(CVS::cvMatToQPixmap(tempMat));
    }else{
        ui->showLabel->setPixmap(CVS::cvMatToQPixmap(mat));
        this->sendPix = QPixmap(CVS::cvMatToQPixmap(mat));
    }
}

void DivideTool::on_toumingSlider_valueChanged(int value)
{
    Mat t;
    tempMat.copyTo(t);
    cvtColor(t,t,COLOR_BGR2BGRA);
    QImage img = CVS::cvMatToQImage(t);

    for (int i = 1; i < t.rows; i++) {
        for (int j = 1; j < t.cols; j++) {
            if(mask[i][j]!=257 && mask[i][j]!=0){
                img.setPixelColor(j,i,QColor(t.at<Vec4b>(i, j)[2],t.at<Vec4b>(i, j)[1],t.at<Vec4b>(i, j)[0],value));
            }else{
                img.setPixelColor(j,i,QColor(t.at<Vec4b>(i, j)[2],t.at<Vec4b>(i, j)[1],t.at<Vec4b>(i, j)[0],255));
            }
        }
    }
    ui->showLabel->setPixmap(QPixmap::fromImage(img));
    //this->img = QImage(img);
}


void DivideTool::on_confirmBtn_clicked()
{
    emit signal(this->sendPix);
    close();
}

void DivideTool::slot()
{

}
