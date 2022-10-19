#ifndef CROPTOOL_H
#define CROPTOOL_H

#include <QWidget>
#include <QPixmap>
#include <QDebug>
#include <QPoint>
#include <QMouseEvent>
#include <QPainter>
namespace Ui {
class CropTool;
}

class CropTool : public QWidget
{
    Q_OBJECT

public:
    explicit CropTool(QPixmap pixmap,QWidget *parent = nullptr);
    ~CropTool();
private slots:
    void on_cancelBtn_clicked();

    void on_confirmBtn_clicked();

    void receiveLabelSlots(QPixmap a);
signals:
    void confimSignal(QPixmap a);
private:
    Ui::CropTool *ui;
    QPixmap m_pixmap;
    QPixmap m_tempPixmap;
};

#endif // CROPTOOL_H
