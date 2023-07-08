#include "CompletedListWidget.h"
#include "ui_CompletedListWidget.h"

CompletedListItemWidget::CompletedListItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CompletedListItemWidget())
{
    ui->setupUi(this);
    SetUi();

    SignalsAndSlots();
}

CompletedListItemWidget::~CompletedListItemWidget()
{
    delete ui;
}

void CompletedListItemWidget::SetUi()
{
}

void CompletedListItemWidget::SignalsAndSlots()
{
}

CompletedListWidget::CompletedListWidget(QWidget* parent)
{
}

CompletedListWidget::~CompletedListWidget()
{
}
