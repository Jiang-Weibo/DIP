#include "widget.h"
#include "ui_widget.h"

std::vector<Widget::cor> Widget::regionVec;
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("~BeautyImage~Version1.0~");
    shutDownPages();

    //regionY[0]=-1;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::OpenPages()
{
    int count = ui->tabWidget->count();
    for(int i=1;i<count;i++){
        ui->tabWidget->setTabEnabled(i,true);
    }
}

void Widget::updateMatAndPixmap(cv::Mat a, QPixmap b)
{
    a.copyTo(mainMat);
    mainPixmap = QPixmap(b);
}

void Widget::updateMatAndPixmap(cv::Mat a)
{
    a.copyTo(mainMat);
    mainPixmap = CVS::cvMatToQPixmap(mainMat);
}

void Widget::updateMatAndPixmap(QPixmap b)
{
    mainMat = CVS::QPixmapToCvMat(b);
    mainPixmap = CVS::cvMatToQPixmap(mainMat);
}

void Widget::AllocateLabels()
{
}

void Widget::UpdateAllLabels(QPixmap map)
{
}

bool Widget::cvBGR2HSI(cv::Mat& original)
{
    if(original.type()!=16){
        QMessageBox::warning(this,"format error","input mat is not CV_8UC3");
        return false;
    }
    int row = original.rows;
    int col = original.cols*original.channels();
    for(int i=0;i<row;i++){
        uchar* tempRow = original.ptr<uchar>(i);
        for(int j=0;j<col;j+=3){
            uchar B = tempRow[j];
            uchar G = tempRow[j+1];
            uchar R = tempRow[j+2];
            uchar I = (uchar)((B+G+R)/3.0f);
            uchar S = 1.-3.0f/(B+R+G)*std::min(B,std::min(G,R));
            float theta = acos(((R-G+R-B)/2.0f)/sqrt((R-G)*(R-G)+(R-G)*(G-B)));
            uchar H = G>=B ? (uchar)theta : 360-(uchar)theta;
            tempRow[j]=H;
            tempRow[j+1]=S;
            tempRow[j+2]=I;
        }
    }
    //QMessageBox::warning(this,"error","frequency statistic error");
    return true;
}

void Widget::dealCapture(QPixmap map)
{
    mainMat = CVS::QPixmapToCvMat(map);
    OpenPages();
}

void Widget::on_basicFunctionBtn_clicked()
{
    if(!m_mainLabel.pixmap()){
        QMessageBox::information(this,"warning","no image is opened!");
        return;
    }
    ui->tabWidget->setCurrentIndex(1);
}

void Widget::shutDownPages()
{
    int count = ui->tabWidget->count();
    for(int i=1;i<count;i++){
        ui->tabWidget->setTabEnabled(i,false);
    }
}

void Widget::on_tabWidget_currentChanged(int index)
{
    if(index==0)
        return;
    switch (index) {
    case 1: mainPixmap = CVS::cvMatToQPixmap(mainMat); ui->showLabel_1->setPixmap(mainPixmap); ui->showLabel_1->setScaledContents(true);
        break;
    case 2: mainPixmap = CVS::cvMatToQPixmap(mainMat); ui->showLabel_2->setPixmap(mainPixmap); ui->showLabel_2->setScaledContents(true);
        break;
    case 3: mainPixmap = CVS::cvMatToQPixmap(mainMat); ui->showLabel_3->setPixmap(mainPixmap); ui->showLabel_3->setScaledContents(true);
        break;
    case 4: mainPixmap = CVS::cvMatToQPixmap(mainMat); ui->showLabel_4->setPixmap(mainPixmap); ui->showLabel_4->setScaledContents(true);
        break;
    case 5: mainPixmap = CVS::cvMatToQPixmap(mainMat); ui->showLabel_5->setPixmap(mainPixmap); ui->showLabel_5->setScaledContents(true);
        break;
    }
}

void Widget::on_openFileBtn_clicked()
{
    QString path;
    path = QFileDialog::getOpenFileName(this,
                                        "please choose an image file",
                                        "./",
                                        "Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)");

    if (!path.isEmpty())
    {
        mainMat = cv::imread(path.toStdString());
        mainMat.copyTo(finalMat);
        //qDebug()<<"mainMat type is "<<mainMat.type();
        //qDebug()<<"mainMat channel is "<<mainMat.channels();
    }else{
        return;
    }
    mainPixmap = CVS::cvMatToQPixmap(mainMat);
    finalPixmap = CVS::cvMatToQPixmap(finalMat);
    m_mainLabel.setPixmap(mainPixmap);
    if(!m_mainLabel.pixmap()){
        QMessageBox::information(this,"error","mainLabel is null");
        return;
    }else{
        OpenPages();
        AllocateLabels();
        UpdateAllLabels(mainPixmap);

    }

}

void Widget::on_openCameraBtn_clicked()
{
    if(captureWindow)
        captureWindow->show();
    else{
        captureWindow = new captureImage();// 防止关闭
        connect(captureWindow,&captureImage::signal,this,&Widget::dealCapture);
        captureWindow->show();
    }

}

void Widget::on_rotateSlider_valueChanged(int value)
{
    //qDebug()<<"value is "<<value;
    ui->rotateLabel->setText("旋转"+QString::number(value,10));
    QTransform transform;
    transform.rotate(value);
    QPixmap newMap;
    newMap = mainPixmap.transformed(transform);
    ui->showLabel_1->setPixmap(newMap);
}

void Widget::on_horFlipBtn_clicked()
{
    ui->rotateSlider->setValue(-ui->rotateSlider->value());
    QImage img = ui->showLabel_1->pixmap()->toImage();
    img = img.mirrored(true,false);
    ui->showLabel_1->setPixmap(QPixmap::fromImage(img));
}

void Widget::on_verFlipBtn_clicked()
{
    ui->rotateSlider->setValue(-ui->rotateSlider->value());
    QImage img = ui->showLabel_1->pixmap()->toImage();
    img = img.mirrored(false,true);
    ui->showLabel_1->setPixmap(QPixmap::fromImage(img));
}

void Widget::on_translateSliderX_valueChanged(int value)
{
    ui->transLabelX->setText("移动x"+QString::number(value));
    ui->showLabel_1->setGeometry(320+value,20,ui->showLabel_1->width(),ui->showLabel_1->height());
}

void Widget::on_translateSliderY_valueChanged(int value)
{
    ui->transLabelY->setText("移动y"+QString::number(value));
    ui->showLabel_1->setGeometry(310,20+value,ui->showLabel_1->width(),ui->showLabel_1->height());
}

void Widget::mousePressEvent(QMouseEvent *e){
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_S))
    {
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        tr("Open Config"),
                                                        "./",
                                                        tr("*.jpg;;*.bmp;;*.png;;*.pgm;;*.pbm"));

        if (!fileName.isNull() && !mainMat.empty())
        {
            //fileName是文件名
            imwrite(fileName.toStdString(),mainMat);
        }
        else
        {
            //点的是取消
        }
    }
}

void Widget::on_cropBtn_clicked()
{
    if(cropTool==nullptr){
        cropTool = new CropTool(*(ui->showLabel_1->pixmap()));
        connect(cropTool,&CropTool::confimSignal,this,&Widget::receiveCropSignal);
        cropTool->show();
        cropTool = nullptr;
    }
}

void Widget::receiveCropSignal(QPixmap map)
{
    ui->showLabel_1->setPixmap(QPixmap(map));
}

void Widget::on_contrastSlider_valueChanged(int value)
{
    cv::Mat tempMat;
    mainMat.copyTo(tempMat);
    //对比度与亮度的改变
    int height = tempMat.rows;//求出tempMat的高
    int width = tempMat.cols;//求出tempMat的宽
    float alpha = value/100.0f;//调整对比度
    //循环操作，遍历每一列，每一行的元素
    for(int row = 0;row < height; row++)
    {
        for(int col = 0;col < width;col++)
        {
            if(tempMat.channels() == 3)//判断是否为3通道图片
            {
                //将遍历得到的原图像素值，返回给变量b,g,r
                float b = tempMat.at<cv::Vec3b>(row,col)[0];//nlue
                float g = tempMat.at<cv::Vec3b>(row,col)[1];//green
                float r= tempMat.at<cv::Vec3b>(row,col)[2];//red
                //开始操作像素，对变量b,g,r做改变后再返回到新的图片。
                tempMat.at<cv::Vec3b>(row,col)[0] = cv::saturate_cast<uchar>(b*alpha);
                tempMat.at<cv::Vec3b>(row,col)[1] = cv::saturate_cast<uchar>(g*alpha);
                tempMat.at<cv::Vec3b>(row,col)[2] = cv::saturate_cast<uchar>(r*alpha);
            }
            else if(tempMat.channels() == 1)//判断是否为单通道的图片
            {
                float v = tempMat.at<uchar>(row,col);
                tempMat.at<uchar>(row,col) = cv::saturate_cast<uchar>(v*alpha);
            }
        }
    }
    ui->showLabel_2->setPixmap(CVS::cvMatToQPixmap(tempMat));
}

void Widget::on_luminanceSlider_valueChanged(int value)
{
    cv::Mat tempMat;
    mainMat.copyTo(tempMat);
    //亮度的改变
    int height = tempMat.rows;//求出tempMat的高
    int width = tempMat.cols;//求出tempMat的宽
    float beta = value;//调整亮度加50
    //循环操作，遍历每一列，每一行的元素
    for(int row = 0;row < height; row++)
    {
        for(int col = 0;col < width;col++)
        {
            if(tempMat.channels() == 3)//判断是否为3通道图片
            {
                //将遍历得到的原图像素值，返回给变量b,g,r
                float b = tempMat.at<cv::Vec3b>(row,col)[0];//nlue
                float g = tempMat.at<cv::Vec3b>(row,col)[1];//green
                float r= tempMat.at<cv::Vec3b>(row,col)[2];//red
                //开始操作像素，对变量b,g,r做改变后再返回到新的图片。
                tempMat.at<cv::Vec3b>(row,col)[0] = cv::saturate_cast<uchar>(b+beta);
                tempMat.at<cv::Vec3b>(row,col)[1] = cv::saturate_cast<uchar>(g+beta);
                tempMat.at<cv::Vec3b>(row,col)[2] = cv::saturate_cast<uchar>(r+beta);
            }
            else if(tempMat.channels() == 1)//判断是否为单通道的图片
            {
                float v = tempMat.at<uchar>(row,col);
                tempMat.at<uchar>(row,col) = cv::saturate_cast<uchar>(v+beta);
            }
        }
    }
    ui->showLabel_2->setPixmap(CVS::cvMatToQPixmap(tempMat));
}

void Widget::on_saturabilitySlider_valueChanged(int value)
{
    cv::Mat img;
    mainMat.copyTo(img);
    cv::Mat new_img;
    img.copyTo(new_img);
    float increment = value/ 100.f;
    for (int col = 0; col < img.cols; col++)
    {
        for (int row = 0; row < img.rows; row++)
        {
            // R,G,B 分别对应数组中下标的 2,1,0
            uchar r = img.at<cv::Vec3b> (row, col)[2];
            uchar g = img.at<cv::Vec3b> (row, col)[1];
            uchar b = img.at<cv::Vec3b> (row, col)[0];

            float maxn = std::max (r, std::max (g, b));
            float minn = std::min (r, std::min (g, b));

            float delta, value;
            delta = (maxn - minn) / 255;
            value = (maxn + minn) / 255;

            float new_r, new_g, new_b;

            if (delta < 1e-1)		 // 差为 0 不做操作，保存原像素点
            {
                new_img.at<cv::Vec3b> (row, col)[0] = (uchar)b;
                new_img.at<cv::Vec3b> (row, col)[1] = (uchar)g;
                new_img.at<cv::Vec3b> (row, col)[2] = (uchar)r;
                continue;
            }

            float light, sat, alpha;
            light = value / 2;

            if (light < 0.5f)
                sat = delta / value;
            else
                sat = delta / (2 - value);

            if (increment >= 0)
            {
                if ((increment + sat) >= 1)
                    alpha = sat;
                else
                {
                    alpha = 1 - increment;
                }
                alpha = 1 / alpha - 1;
                new_r = r + (r - light * 255) * alpha;
                new_g = g + (g - light * 255) * alpha;
                new_b = b + (b - light * 255) * alpha;
            }
            else
            {
                alpha = increment;
                new_r = light * 255 + (r - light * 255) * (1 + alpha);
                new_g = light * 255 + (g - light * 255) * (1 + alpha);
                new_b = light * 255 + (b - light * 255) * (1 + alpha);
            }
            new_img.at<cv::Vec3b> (row, col)[0] = (uchar)new_b;
            new_img.at<cv::Vec3b> (row, col)[1] = (uchar)new_g;
            new_img.at<cv::Vec3b> (row, col)[2] = (uchar)new_r;
        }
    }
    ui->showLabel_2->setPixmap(CVS::cvMatToQPixmap(new_img));
}

void Widget::on_hueSlider_valueChanged(int value)
{
    cv::Mat tempMat;
    mainMat.copyTo(tempMat);
    //tempMat.convertTo(tempMat,)
    if(tempMat.type()!=16){
        qDebug()<<"pic is not CV_8U3";
        return;
    }
    cv::cvtColor(tempMat,tempMat,cv::COLOR_BGR2HLS);
    for(int i=0;i<tempMat.rows;i++){
        for(int j=0;j<tempMat.cols;j++){
            tempMat.at<cv::Vec3b>(i,j)[0] +=value;
        }
    }
    cv::cvtColor(tempMat,tempMat,cv::COLOR_HLS2BGR);
    ui->showLabel_2->setPixmap(CVS::cvMatToQPixmap(tempMat));
}

void Widget::on_addNoiseBtn_clicked()
{
    qDebug()<<ui->selectNoiseTypeBox->currentText()<<"被选择";
}

void Widget::on_restoreImageBtn_clicked()
{
    ui->showLabel_3->setPixmap(mainPixmap);
    ui->gaussMeanSlider->setValue(0);
    ui->gaussDeviationSlider->setValue(0);
    ui->uniformInfSlider->setValue(0);
    ui->uniformSupSlider->setValue(0);
}

void Widget::on_gaussMeanSlider_valueChanged(int value)
{
    if(ui->selectNoiseTypeBox->currentText()=="高斯噪声"){
        int deviation = ui->gaussDeviationSlider->value();
        cv::Mat imgMat;
        mainMat.copyTo(imgMat);
        cv::Mat noiseMat = cv::Mat::zeros(imgMat.rows,imgMat.cols,imgMat.type());
        cv::RNG rng;
        rng.fill(noiseMat,cv::RNG::NORMAL,value,deviation);
        imgMat = imgMat + noiseMat;
        ui->showLabel_3->setPixmap(CVS::cvMatToQPixmap(imgMat));
    }
}

void Widget::on_gaussDeviationSlider_valueChanged(int value)
{
    if(ui->selectNoiseTypeBox->currentText()=="高斯噪声"){
        int mean = ui->gaussMeanSlider->value();
        cv::Mat imgMat;
        mainMat.copyTo(imgMat);
        cv::Mat noiseMat = cv::Mat::zeros(imgMat.rows,imgMat.cols,imgMat.type());
        cv::RNG rng;
        rng.fill(noiseMat,cv::RNG::NORMAL,mean,value);
        imgMat = imgMat + noiseMat;
        ui->showLabel_3->setPixmap(CVS::cvMatToQPixmap(imgMat));
        //qDebug()<<"gauss over@!";
    }
}

void Widget::on_uniformSupSlider_valueChanged(int value)
{
    if(ui->selectNoiseTypeBox->currentText()=="均匀噪声"){
        int inf = ui->uniformInfSlider->value();
        cv::Mat imgMat;
        mainMat.copyTo(imgMat);
        cv::Mat noiseMat = cv::Mat::zeros(imgMat.rows,imgMat.cols,imgMat.type());
        cv::RNG rng;
        rng.fill(noiseMat,cv::RNG::UNIFORM,inf,value);
        imgMat = imgMat + noiseMat;
        ui->showLabel_3->setPixmap(CVS::cvMatToQPixmap(imgMat));
        //qDebug()<<"gauss over@!";
    }
}

void Widget::on_uniformInfSlider_valueChanged(int value)
{
    if(ui->selectNoiseTypeBox->currentText()=="均匀噪声"){
        int sup = ui->uniformSupSlider->value();
        cv::Mat imgMat;
        mainMat.copyTo(imgMat);
        cv::Mat noiseMat = cv::Mat::zeros(imgMat.rows,imgMat.cols,imgMat.type());
        cv::RNG rng;
        rng.fill(noiseMat,cv::RNG::UNIFORM,value,sup);
        imgMat = imgMat + noiseMat;
        ui->showLabel_3->setPixmap(CVS::cvMatToQPixmap(imgMat));
        //qDebug()<<"gauss over@!";
    }
}

void Widget::on_impulseNoiseSlider_valueChanged(int value)
{
    if(ui->selectNoiseTypeBox->currentText()=="椒盐噪声"){
        cv::Mat showMat;
        mainMat.copyTo(showMat);
        int n = (int)(value*showMat.rows*showMat.cols*showMat.channels()/100.f);
        cv::Mat resultImage = showMat.clone();
        int SaltOrSpecy = 255;
        for (int k = 0; k < n; k++)
        {
            if(n%2)
                SaltOrSpecy = 255;
            else
                SaltOrSpecy = 0;
            //随机取值行列
            int i = rand() % resultImage.cols;
            int j = rand() % resultImage.rows;
            //图像通道判定
            if (resultImage.channels() == 1)
                resultImage.at<uchar>(j, i) = (uchar)SaltOrSpecy;
            else
            {
                resultImage.at<cv::Vec3b>(j, i)[0] = (uchar)SaltOrSpecy;
                resultImage.at<cv::Vec3b>(j, i)[1] = (uchar)SaltOrSpecy;
                resultImage.at<cv::Vec3b>(j, i)[2] = (uchar)SaltOrSpecy;
            }
        }
        ui->showLabel_3->setPixmap(CVS::cvMatToQPixmap(resultImage));
    }

}

void Widget::on_eliminationNoiseBtn_clicked()
{
    QString type = ui->selectEliminationMethodBox->currentText();
    int index = ui->selectEliminationMethodBox->currentIndex();
    if(ui->showLabel_3->pixmap()->isNull()) return;
    switch (index) {
    case 0:{
        cv::Mat dst = CVS::QPixmapToCvMat( *ui->showLabel_3->pixmap());
        cv::GaussianBlur(dst,dst,cv::Size(ui->gaussSlider->value(),ui->gaussSlider->value()),0,0);
        ui->showLabel_3->setPixmap(CVS::cvMatToQPixmap(dst));
        break;
    }
    case 1:{
        cv::Mat dst = CVS::QPixmapToCvMat( *ui->showLabel_3->pixmap());
        cv::medianBlur(CVS::QPixmapToCvMat(*ui->showLabel_3->pixmap()),dst,ui->medianSlider->value());
        ui->showLabel_3->setPixmap(CVS::cvMatToQPixmap(dst));
        break;
    }
    case 2:{
        cv::Mat dst = CVS::QPixmapToCvMat( *ui->showLabel_3->pixmap());
        cv::blur(CVS::QPixmapToCvMat(*ui->showLabel_3->pixmap()),dst,cv::Size(ui->meanSlider->value(),ui->meanSlider->value()));
        ui->showLabel_3->setPixmap(CVS::cvMatToQPixmap(dst));
        break;
    }
    case 3:{
        cv::Mat dst = CVS::QPixmapToCvMat( *ui->showLabel_3->pixmap());
        cv::bilateralFilter(CVS::QPixmapToCvMat(*ui->showLabel_3->pixmap()),dst,ui->bilateralSlider->value(),ui->bilateralSlider_2->value(),114);
        ui->showLabel_3->setPixmap(CVS::cvMatToQPixmap(dst));
        break;
    }
    }
}

void Widget::on_ShapenBtn_clicked()
{
    cv::Mat mat;
    mainMat.copyTo(mat);
    if(ui->SharpenComboBox->currentIndex()==0){
        //Laplace Operator
        cv::Mat af;
        cv::Laplacian(mat,af,mat.type(),3);
        for (int i=0;i<mat.rows;i++) {
            uchar* t2 = af.ptr<uchar>(i);
            uchar* tempRow = mat.ptr<uchar>(i);
            for (int j=0;j<mat.cols*mat.channels();j+=3){
                t2[j+2]=t2[j+1]=0;
                if(t2[j]==255) tempRow[j]=t2[j];
            }
        }
    }else if(ui->SharpenComboBox->currentIndex()==1){
        //Sobel oeprator
        cv::Mat m;
        mat.copyTo(m);
        cv::Mat xBord;
        cv::Mat yBord;
        cv::Sobel(m,xBord,CV_32F,1,0);
        cv::Sobel(m,yBord,CV_32F,1,0);
        cv::convertScaleAbs(xBord,xBord);
        cv::convertScaleAbs(yBord,yBord);
        cv::add(xBord, yBord, m, cv::Mat(), CV_16S);
        cv::convertScaleAbs(m, m);
        for (int i=0;i<mat.rows;i++) {
            uchar* t2 = m.ptr<uchar>(i);
            uchar* tempRow = mat.ptr<uchar>(i);
            for (int j=0;j<mat.cols*mat.channels();j+=3){
                t2[j+2]=t2[j+1]=0;
                if(t2[j]==255) tempRow[j]=t2[j];
            }
        }
        //imshow("sobelAf",mat);
    }
    ui->showLabel_4->setPixmap(CVS::cvMatToQPixmap(mat));
}

void Widget::on_RegionGrowBtn_clicked()
{
    Widget::regionVec.clear();
    imshow("regionGrow",CVS::QPixmapToCvMat(*ui->showLabel_4->pixmap()));
    cv::setMouseCallback("regionGrow",&Widget::cvCallBackFunc,nullptr);
    //qDebug()<<"callback over!";
}

void Widget::cvCallBackFunc(int event, int x, int y, int flags, void *param)
{
    if(event==cv::EVENT_LBUTTONDOWN){
        Widget::regionVec.push_back(Widget::cor(x,y));
    }
}

void Widget::on_regionGrowBrginBtn_clicked()
{
    if(regionVec.empty()) return;

    struct corrds{
        corrds(int _x,int _y):x(_x),y(_y){
        }
        int x,y;
    };
    cv::Mat originalMat;
    int threshold = QInputDialog::getInt(this,"threshold","noLabel",5);
    if(threshold==0){
        threshold = 5;// Default value is  5
    }
    mainMat.copyTo(originalMat);
    cv::cvtColor(originalMat,originalMat,cv::COLOR_BGR2GRAY);
    int **state = new int*[originalMat.rows];
    for(int i=0;i<originalMat.rows;i++){
        state[i] = new int[originalMat.cols];
    }
    for(int i=0;i<originalMat.rows;i++){
        for(int j=0;j<originalMat.cols;j++){
            state[i][j] = 0;
        }
    }
    //Edge case
    for(int i=0;i<originalMat.rows;i++){
        state[i][0]=state[i][originalMat.cols-1]=257;
    }
    for(int j=0;j<originalMat.cols;j++){
        state[0][j]=state[originalMat.rows-1][j]=257;
    }
    std::vector<corrds> pixels;
    int index = 1;//region index
    while(!Widget::regionVec.empty()){
        Widget::cor t = Widget::regionVec.back();
        Widget::regionVec.pop_back();
        //qDebug()<<"k = "<<k<<" color = "<<mainMat.at<uchar>(regionX[k],regionY[k]);
        for (int i=t.y-5;i<t.y+5;i++) {
            for(int j=t.x-5;j<t.x+5;j++){
                if(state[i][j]==0)// Haven't assign region
                {
                    state[i][j] = index;
                    if(state[i-1][j-1]==0 && abs(originalMat.at<uchar>(i-1,j-1)-originalMat.at<uchar>(i,j))<threshold){
                        state[i-1][j-1] = index;
                        pixels.push_back(corrds(i-1,j-1));
                    }
                    if(state[i-1][j]==0 && abs(originalMat.at<uchar>(i-1,j)-originalMat.at<uchar>(i,j))<threshold){
                        state[i-1][j] = index;
                        pixels.push_back(corrds(i-1,j));
                    }
                    if(state[i-1][j+1]==0 && abs(originalMat.at<uchar>(i-1,j+1)-originalMat.at<uchar>(i,j))<threshold){
                        state[i-1][j+1] = index;
                        pixels.push_back(corrds(i-1,j+1));
                    }
                    if(state[i][j-1]==0 && abs(originalMat.at<uchar>(i,j-1)-originalMat.at<uchar>(i,j))<threshold){
                        state[i][j-1] = index;
                        pixels.push_back(corrds(i,j-1));
                    }
                    if(state[i][j]==0 && abs(originalMat.at<uchar>(i,j)-originalMat.at<uchar>(i,j))<threshold){
                        state[i][j] = index;
                        pixels.push_back(corrds(i,j));
                    }
                    if(state[i][j+1]==0 && abs(originalMat.at<uchar>(i,j+1)-originalMat.at<uchar>(i,j))<threshold){
                        state[i][j+1] = index;
                        pixels.push_back(corrds(i,j+1));
                    }
                    if(state[i+1][j-1]==0 && abs(originalMat.at<uchar>(i+1,j-1)-originalMat.at<uchar>(i,j))<threshold){
                        state[i+1][j-1] = index;
                        pixels.push_back(corrds(i+1,j-1));
                    }
                    if(state[i+1][j]==0 && abs(originalMat.at<uchar>(i+1,j)-originalMat.at<uchar>(i,j))<threshold){
                        state[i+1][j] = index;
                        pixels.push_back(corrds(i+1,j));
                    }
                    if(state[i+1][j+1]==0 && abs(originalMat.at<uchar>(i+1,j+1)-originalMat.at<uchar>(i,j))<threshold){
                        state[i+1][j+1] = index;
                        pixels.push_back(corrds(i+1,j+1));
                    }
                    index++;
                    while(pixels.empty()==false){
                        corrds temp = pixels.back();
                        int i = temp.x;
                        int j = temp.y;
                        int index = state[i][j];
                        pixels.pop_back();
                        if(state[i-1][j-1]==0 && abs(originalMat.at<uchar>(i-1,j-1)-originalMat.at<uchar>(i,j))<threshold){
                            state[i-1][j-1] = index;
                            pixels.push_back(corrds(i-1,j-1));
                        }
                        if(state[i-1][j]==0 && abs(originalMat.at<uchar>(i-1,j)-originalMat.at<uchar>(i,j))<threshold){
                            state[i-1][j] = index;
                            pixels.push_back(corrds(i-1,j));
                        }
                        if(state[i-1][j+1]==0 && abs(originalMat.at<uchar>(i-1,j+1)-originalMat.at<uchar>(i,j))<threshold){
                            state[i-1][j+1] = index;
                            pixels.push_back(corrds(i-1,j+1));
                        }
                        if(state[i][j-1]==0 && abs(originalMat.at<uchar>(i,j-1)-originalMat.at<uchar>(i,j))<threshold){
                            state[i][j-1] = index;
                            pixels.push_back(corrds(i,j-1));
                        }
                        if(state[i][j]==0 && abs(originalMat.at<uchar>(i,j)-originalMat.at<uchar>(i,j))<threshold){
                            state[i][j] = index;
                            pixels.push_back(corrds(i,j));
                        }
                        if(state[i][j+1]==0 && abs(originalMat.at<uchar>(i,j+1)-originalMat.at<uchar>(i,j))<threshold){
                            state[i][j+1] = index;
                            pixels.push_back(corrds(i,j+1));
                        }
                        if(state[i+1][j-1]==0 && abs(originalMat.at<uchar>(i+1,j-1)-originalMat.at<uchar>(i,j))<threshold){
                            state[i+1][j-1] = index;
                            pixels.push_back(corrds(i+1,j-1));
                        }
                        if(state[i+1][j]==0 && abs(originalMat.at<uchar>(i+1,j)-originalMat.at<uchar>(i,j))<threshold){
                            state[i+1][j] = index;
                            pixels.push_back(corrds(i+1,j));
                        }
                        if(state[i+1][j+1]==0 && abs(originalMat.at<uchar>(i+1,j+1)-originalMat.at<uchar>(i,j))<threshold){
                            state[i+1][j+1] = index;
                            pixels.push_back(corrds(i+1,j+1));
                        }
                    }
                }
            }
        }
    }
    cv::Mat m;
    mainMat.copyTo(m);
    for(int i=1;i<originalMat.rows;i++){
        uchar* tempRow = m.ptr<uchar>(i);
        for(int j=1,k=3;j<originalMat.cols;j++,k+=3){
            if(state[i][j]!=257 && state[i][j]!=0){
                //originalMat.at<uchar>(i,j)=state[i][j];
                tempRow[k]=0;
            }
        }
    }
    imshow("afterRegionGrowing",m);
    mainMat.copyTo(originalMat);
    if(QMessageBox::information(this,"confirm","save this pic?",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes){
        if(divideTool==nullptr){
            divideTool = new DivideTool(state,originalMat);
            connect(divideTool,&DivideTool::signal,this,&Widget::dealDivideSlot);
            divideTool->show();
            divideTool = nullptr;
        }
    }
}

void Widget::dealDivideSlot(QPixmap pix)
{
    //this->divideImage = pix;
    ui->showLabel_4->setPixmap(pix);
}

void Widget::on_maoboliBtn_clicked()
{
    Mat mat;
    mainMat.copyTo(mat);
    for (int i=0;i<mat.rows-5;i++) {
        for(int j=0;j<mat.cols-5;j++){
            int r = rand()%5;
            mat.at<Vec3b>(i,j)[0] = mainMat.at<Vec3b>(i+r,j+r)[0];
            mat.at<Vec3b>(i,j)[1] = mainMat.at<Vec3b>(i+r,j+r)[1];
            mat.at<Vec3b>(i,j)[2] = mainMat.at<Vec3b>(i+r,j+r)[2];
        }
    }
    ui->showLabel_5->setPixmap(CVS::cvMatToQPixmap(mat));
}

void Widget::on_fudiaoBtn_clicked()
{
    Mat m,mat;
    cvtColor(mainMat,m,COLOR_BGR2GRAY);
    cvtColor(mainMat,mat,COLOR_BGR2GRAY);
    for (int i=0;i<m.rows-1;i++) {
        for(int j=0;j<m.cols-1;j++){
            int p1 = mat.ptr<uchar>(i)[j];
            int p2 = mat.ptr<uchar>(i+1)[j+1];
            int p = p1-p2+128;
            if(p>255) p=255;
            else if(p<0) p=0;
            m.ptr<uchar>(i)[j]=p;
        }
    }
    cvtColor(m,m,COLOR_GRAY2BGR);
    ui->showLabel_5->setPixmap(CVS::cvMatToQPixmap(m));
}

void Widget::on_enhanceFunctionBtn_clicked()
{
    if(!m_mainLabel.pixmap()){
        QMessageBox::information(this,"warning","no image is opened!");
        return;
    }
    ui->tabWidget->setCurrentIndex(2);
}

void Widget::on_noiseFunctionBtn_clicked()
{
    if(!m_mainLabel.pixmap()){
        QMessageBox::information(this,"warning","no image is opened!");
        return;
    }
    ui->tabWidget->setCurrentIndex(3);
}

void Widget::on_pushButton_4_clicked()
{
    if(!m_mainLabel.pixmap()){
        QMessageBox::information(this,"warning","no image is opened!");
        return;
    }
    ui->tabWidget->setCurrentIndex(4);
}

void Widget::on_pushButton_6_clicked()
{
    if(!m_mainLabel.pixmap()){
        QMessageBox::information(this,"warning","no image is opened!");
        return;
    }
    ui->tabWidget->setCurrentIndex(5);
}

void Widget::on_confirmBtn1_clicked()
{
    this->mainMat = CVS::QPixmapToCvMat( *ui->showLabel_1->pixmap());
}

void Widget::on_confirmBtn1_2_clicked()
{
    this->mainMat = CVS::QPixmapToCvMat( *ui->showLabel_2->pixmap());
}

void Widget::on_confirmBtn1_3_clicked()
{
    this->mainMat = CVS::QPixmapToCvMat( *ui->showLabel_3->pixmap());
}

void Widget::on_confirmBtn1_4_clicked()
{
    this->mainMat = CVS::QPixmapToCvMat( *ui->showLabel_4->pixmap());
}

void Widget::on_confirmBtn1_5_clicked()
{
    this->mainMat = CVS::QPixmapToCvMat( *ui->showLabel_5->pixmap());
}

void Widget::on_restoreImageBtn_2_clicked()
{
    ui->showLabel_4->setPixmap(mainPixmap);
}

void Widget::on_restoreImageBtn_5_clicked()
{
    ui->showLabel_1->setPixmap(mainPixmap);
    ui->rotateSlider->setValue(0);
    ui->translateSliderX->setValue(0);
    ui->translateSliderY->setValue(0);
}

void Widget::on_restoreImageBtn_4_clicked()
{
    ui->showLabel_2->setPixmap(mainPixmap);
    ui->contrastSlider->setValue(100);
    ui->luminanceSlider->setValue(0);
    ui->saturabilitySlider->setValue(0);
    ui->hueSlider->setValue(0);
}

void Widget::on_restoreImageBtn_3_clicked()
{
    ui->showLabel_5->setPixmap(mainPixmap);
}

void Widget::on_gaussSlider_valueChanged(int value)
{

}
