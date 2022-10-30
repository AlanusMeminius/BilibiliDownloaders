#pragma once

#include <QDialog>
#include "ui_CDownloadDialog.h"

class CDownloadDialog : public QDialog
{
    Q_OBJECT

public:
    CDownloadDialog(QWidget *parent = nullptr);
    ~CDownloadDialog();


    void SetUi();
    void SignalsAndSlots();

private:
    Ui::CDownloadDialogClass ui;
};
