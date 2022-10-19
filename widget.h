#ifndef WIDGET_H
#define WIDGET_H

#include"opencv.hpp"
#include"captureimage.h"
#include"croptool.h"
#include"dividetool.h"
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QPainter>
#include <QInputDialog>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    struct cor{
        cor(int _x,int _y):x(_x),y(_y){}
        int x;
        int y;
    };
private:
    QLabel m_mainLabel;
    captureImage* captureWindow = nullptr;
    cv::Mat mainMat;
    QPixmap mainPixmap;
    cv::Mat finalMat;
    QPixmap finalPixmap;
    CropTool *cropTool = nullptr;
    QLabel *showLabel_1 = nullptr, *showLabel_2 = nullptr;
    DivideTool *divideTool = nullptr;
    QImage divideImage;
    static std::vector<cor> regionVec;

private:
    void OpenPages();
    void SetPicture(QPixmap& pixmap);
    void updateMatAndPixmap(cv::Mat a,QPixmap b);
    void updateMatAndPixmap(cv::Mat a);
    void updateMatAndPixmap(QPixmap b);
    void AllocateLabels();
    void UpdateAllLabels(QPixmap map);
    bool cvBGR2HSI(cv::Mat& original);
private slots:
    void dealCapture(QPixmap map);
    void on_basicFunctionBtn_clicked();
    void shutDownPages();
    void on_tabWidget_currentChanged(int index);

    void on_openFileBtn_clicked();

    void on_openCameraBtn_clicked();

    void on_rotateSlider_valueChanged(int value);

    void on_horFlipBtn_clicked();

    void on_verFlipBtn_clicked();

    void on_translateSliderX_valueChanged(int value);

    void on_translateSliderY_valueChanged(int value);
    void on_cropBtn_clicked();
    void receiveCropSignal(QPixmap map);
    void on_contrastSlider_valueChanged(int value);

    void on_luminanceSlider_valueChanged(int value);

    void on_saturabilitySlider_valueChanged(int value);

    void on_hueSlider_valueChanged(int value);

    void on_addNoiseBtn_clicked();

    void on_restoreImageBtn_clicked();

    void on_gaussMeanSlider_valueChanged(int value);

    void on_gaussDeviationSlider_valueChanged(int value);

    void on_uniformSupSlider_valueChanged(int value);

    void on_uniformInfSlider_valueChanged(int value);

    void on_impulseNoiseSlider_valueChanged(int value);

    void on_eliminationNoiseBtn_clicked();

    void on_ShapenBtn_clicked();

    void on_RegionGrowBtn_clicked();

    static void cvCallBackFunc(int event,int x,int y,int flags,void *param);

    void on_regionGrowBrginBtn_clicked();

    void dealDivideSlot(QPixmap pix);

    void on_maoboliBtn_clicked();

    void on_fudiaoBtn_clicked();

    void on_enhanceFunctionBtn_clicked();

    void on_noiseFunctionBtn_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_confirmBtn1_clicked();

    void on_confirmBtn1_2_clicked();

    void on_confirmBtn1_3_clicked();

    void on_confirmBtn1_4_clicked();

    void on_confirmBtn1_5_clicked();

    void on_restoreImageBtn_2_clicked();

    void on_restoreImageBtn_5_clicked();

    void on_restoreImageBtn_4_clicked();

    void on_restoreImageBtn_3_clicked();

    void on_gaussSlider_valueChanged(int value);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
