#include "VideoListWidget.h"
#include "ui_VideoListWidget.h"

VideoListItemWidget::VideoListItemWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::VideoListItemWidget)
{
    ui->setupUi(this);
}

VideoListItemWidget::~VideoListItemWidget()
{
    delete ui;
}


VideoListWidget::VideoListWidget(QWidget *parent)
    : QListWidget(parent)
{

}

VideoListWidget::~VideoListWidget()
{

}
