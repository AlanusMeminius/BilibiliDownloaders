#pragma once

#include <QWidget>
#include <QObject>

class QLabel;
class QPushButton;

class TitleWidget : public QWidget 
{
    Q_OBJECT
public:
    TitleWidget(QWidget* parent = nullptr);


    void SetUi();
    void SignalsAndSlots();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;
    void resizeEvent(QResizeEvent* event);

public:
    QLabel* m_labelIcon;
    QLabel* m_labelTitle;
    QPushButton* m_btnMin;
    QPushButton* m_btnMax;
    QPushButton* m_btnClose;
};