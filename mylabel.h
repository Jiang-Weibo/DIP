#ifndef MYLABEL_H
#define MYLABEL_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QPixmap>
class myLabel : public QLabel
{
    Q_OBJECT
signals:
    void signal(QPixmap a);
public:
    myLabel(QWidget* parent = 0);
private:
    QPoint p1,p2;
    QPixmap cropMap;
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
};

#endif // MYLABEL_H
