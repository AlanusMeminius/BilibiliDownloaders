#pragma once

#include <QWidget>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class CompletedListItemWidget; };
QT_END_NAMESPACE

class CompletedListItemWidget : public QWidget
{
    Q_OBJECT

public:
    CompletedListItemWidget(QWidget *parent = nullptr);
    ~CompletedListItemWidget();

    void SetUi();
    void SignalsAndSlots();

private:
    Ui::CompletedListItemWidget*ui;
};


class CompletedListWidget : public QListWidget
{
    Q_OBJECT
public:
    CompletedListWidget(QWidget* parent = nullptr);
    ~CompletedListWidget();

};

