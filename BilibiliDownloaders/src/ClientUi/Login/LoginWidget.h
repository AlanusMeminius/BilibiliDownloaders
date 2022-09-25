#pragma once

#include <QWidget>

namespace Ui { class LoginWidget; };


class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

private:
    Ui::LoginWidget* ui;
};
