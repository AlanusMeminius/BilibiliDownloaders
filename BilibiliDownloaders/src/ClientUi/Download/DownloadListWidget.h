#pragma once

#include <QWidget>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class DownloadListItemWidget; };
QT_END_NAMESPACE

class DownloadListItemWidget : public QWidget
{
    Q_OBJECT

public:
    DownloadListItemWidget(QWidget *parent = nullptr);
    ~DownloadListItemWidget();

    void SetUi();
    void SignalsAndSlots();
private:
    Ui::DownloadListItemWidget*ui;
};

class DownloadListWidget : public QListWidget
{
    Q_OBJECT
public:
    DownloadListWidget(QWidget* parent = nullptr);
    ~DownloadListWidget();

};
