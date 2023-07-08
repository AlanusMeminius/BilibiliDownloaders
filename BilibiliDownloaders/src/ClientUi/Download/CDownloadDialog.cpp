#include "CDownloadDialog.h"
#include "ui_CDownloadDialog.h"

CDownloadDialog::CDownloadDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CDownloadDialog)
{
    ui->setupUi(this);
    SetUi();

    setWindowTitle(tr("Download Center"));

    SignalsAndSlots();
}

CDownloadDialog::~CDownloadDialog()
{
}

void CDownloadDialog::SetUi()
{
}

void CDownloadDialog::SignalsAndSlots()
{
}
