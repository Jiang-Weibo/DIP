#include "croptool.h"
#include "ui_croptool.h"

CropTool::CropTool(QPixmap pixmap,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CropTool)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->m_pixmap = QPixmap(pixmap);
    ui->label->setPixmap(m_pixmap);
    ui->label->setScaledContents(true);
    connect(ui->label,&myLabel::signal,this,&CropTool::receiveLabelSlots);

}

CropTool::~CropTool()
{
    delete ui;
}

void CropTool::on_cancelBtn_clicked()
{
    close();
}

void CropTool::on_confirmBtn_clicked()
{
    m_pixmap = QPixmap(m_tempPixmap);
    emit confimSignal(m_pixmap);
    close();
}

void CropTool::receiveLabelSlots(QPixmap a)
{
    m_tempPixmap = QPixmap(a);
}

