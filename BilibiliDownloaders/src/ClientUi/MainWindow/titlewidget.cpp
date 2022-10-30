#include "TitleWidget.h"

#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <qt_windows.h>


TitleWidget::TitleWidget(QWidget* parent)
    : QWidget(parent)
    , m_labelIcon(new QLabel(this))
    , m_labelTitle(new QLabel(this))
    , m_btnMin(new QPushButton(this))
    , m_btnMax(new QPushButton(this))
    , m_btnClose(new QPushButton(this))
{
    SetUi();
    setAttribute(Qt::WA_StyledBackground);

    SignalsAndSlots();

}

void TitleWidget::SetUi()
{
    m_btnMax->setToolTip(tr("Maximize"));
    m_btnMax->setIcon(style()->standardPixmap(QStyle::SP_TitleBarMaxButton));
    m_btnMin->setToolTip("Minimize");
    m_btnMin->setIcon(style()->standardPixmap(QStyle::SP_TitleBarMinButton));
    m_btnClose->setToolTip("Close");
    m_btnClose->setIcon(style()->standardPixmap(QStyle::SP_TitleBarCloseButton));

    window()->installEventFilter(this);
}

void TitleWidget::SignalsAndSlots()
{
    connect(m_btnMin, &QPushButton::clicked, this, [&]() {
        window()->showMinimized();
    });

    connect(m_btnMax, &QPushButton::clicked, this, [&]() {
        window()->isMaximized() ? window()->showNormal() : window()->showMaximized();
    });

    connect(m_btnClose, &QPushButton::clicked, this, [&]() {
        window()->close();
    });
}

void TitleWidget::mousePressEvent(QMouseEvent* event)
{
    if (ReleaseCapture())
    {
        if (window()->isTopLevel())
        {
            SendMessage(HWND(window()->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
    }

    event->ignore();
}

void TitleWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    emit m_btnMax->clicked();
}

void TitleWidget::paintEvent(QPaintEvent*)
{
    // 为了使用qss
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

bool TitleWidget::eventFilter(QObject* obj, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::WindowTitleChange:
    {
        QWidget* widget = qobject_cast<QWidget*>(obj);
        if (widget)
        {
            m_labelTitle->setText(widget->windowTitle());
            return true;
        }
    }
    case QEvent::WindowIconChange:
    {
        QWidget* widget = qobject_cast<QWidget*>(obj);
        if (widget)
        {
            m_labelIcon->setPixmap(widget->windowIcon().pixmap(m_labelIcon->size()));
            return true;
        }
    }
    default:
        break;
    }

    return QWidget::eventFilter(obj, event);
}

void TitleWidget::resizeEvent(QResizeEvent* event)
{
    m_btnMin->setGeometry(QRect(width() - 8 - 34 * 3, 0, 33, 28));
    m_btnMax->setGeometry(QRect(width() - 8 - 34 * 2, 0, 33, 28));
    m_btnClose->setGeometry(QRect(width() - 8 - 34 * 1, 0, 33, 28));

    return QWidget::resizeEvent(event);
}
