#pragma once

#include <QLineEdit>
#include "ui_SearchLineEdit.h"

namespace Ui { class SearchLineEdit; };


class SearchLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    SearchLineEdit(QWidget *parent = nullptr);
    ~SearchLineEdit();
    void SetEditFinishedSearch(bool enabled);

protected:
    void resizeEvent(QResizeEvent* event) override;

signals:
    void searchCliecked();

private:
    void SetUi();
    void SignalsAndSlots();

private:
    Ui::SearchLineEdit *ui;
};
