#include "CompletedListWidget.h"
#include "ui_CompletedListWidget.h"

CompletedListItemWidget::CompletedListItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CompletedListItemWidget())
{
    ui->setupUi(this);
}

CompletedListItemWidget::~CompletedListItemWidget()
{
    delete ui;
}

CompletedListWidget::CompletedListWidget(QWidget* parent)
{
}

CompletedListWidget::~CompletedListWidget()
{
}
