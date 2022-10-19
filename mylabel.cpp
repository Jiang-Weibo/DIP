#include "mylabel.h"

myLabel::myLabel(QWidget* parent):
    QLabel(parent)
{

}
void myLabel::mousePressEvent(QMouseEvent *event){
    p2 = p1 = event->pos();

}

void myLabel::mouseMoveEvent(QMouseEvent *event){
    //qDebug()<<"cropTool mouse MoveEvent is called!";
    p2 = event->pos();
    update();
}

void myLabel::paintEvent(QPaintEvent *event){
    QLabel::paintEvent(event);
    QPainter painter(this);
    painter.setPen(QPen(Qt::red,2));
    painter.drawRect(QRect(p1,p2));
    cropMap = this->pixmap()->copy(QRect(p1,p2));
    emit signal(cropMap);
}
