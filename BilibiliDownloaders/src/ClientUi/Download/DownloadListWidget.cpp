#include "DownloadListWidget.h"
#include "ui_DownloadListWidget.h"

DownloadListItemWidget::DownloadListItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DownloadListItemWidget())
{
    ui->setupUi(this);
    SetUi();

    SignalsAndSlots();
}

DownloadListItemWidget::~DownloadListItemWidget()
{
    delete ui;
}

void DownloadListItemWidget::SetUi()
{
}

void DownloadListItemWidget::SignalsAndSlots()
{
}

DownloadListWidget::DownloadListWidget(QWidget* parent)
{
}

DownloadListWidget::~DownloadListWidget()
{
}
