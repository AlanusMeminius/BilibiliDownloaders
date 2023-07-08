#pragma once

#include <QDialog>
#include "ui_CDownloadDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CDownloadDialog; };
QT_END_NAMESPACE

class CDownloadDialog : public QDialog
{
    Q_OBJECT

public:
    CDownloadDialog(QWidget* parent = nullptr);
    ~CDownloadDialog();


    void SetUi();
    void SignalsAndSlots();

private:
    Ui::CDownloadDialog* ui;
};
