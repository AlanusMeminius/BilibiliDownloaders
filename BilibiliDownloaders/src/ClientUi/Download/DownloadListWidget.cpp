#include "DownloadListWidget.h"
#include "ui_DownloadListWidget.h"

DownloadListItemWidget::DownloadListItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DownloadListItemWidget())
{
    ui->setupUi(this);
}

DownloadListItemWidget::~DownloadListItemWidget()
{
    delete ui;
}

DownloadListWidget::DownloadListWidget(QWidget* parent)
{
}

DownloadListWidget::~DownloadListWidget()
{
}
