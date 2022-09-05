#pragma once
#pragma execution_character_set("utf-8")


#include <QWidget>
#include <QListWidget>
namespace Ui { class VideoListItemWidget; };

class VideoListItemWidget : public QWidget
{
    Q_OBJECT

public:
    VideoListItemWidget(QWidget* parent = nullptr);
    ~VideoListItemWidget();

private:
    Ui::VideoListItemWidget* ui;
};

class VideoListWidget : public QListWidget
{
    Q_OBJECT

public:
    VideoListWidget(QWidget *parent = nullptr);
    ~VideoListWidget();

};
