#pragma once
#pragma execution_character_set("utf-8")


#include <QWidget>
#include <QListWidget>
namespace Ui { class VideoTabItemWidget; };

class VideoTabItemWidget : public QWidget
{
    Q_OBJECT

public:
    VideoTabItemWidget(QWidget* parent = nullptr);
    ~VideoTabItemWidget();

private:
    void SetUi();
    void SignalsAndSlots();

private:
    Ui::VideoTabItemWidget* ui;
};

class VideoTabWidget : public QListWidget
{
    Q_OBJECT

public:
    VideoTabWidget(QWidget *parent = nullptr);
    ~VideoTabWidget();

};
