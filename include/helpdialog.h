#pragma once
#include <QDialog>

namespace Ui { class HelpDialog; }
class HelpDialog : public QDialog
{
    Q_OBJECT
public:
    explicit HelpDialog(QWidget *parent = nullptr);
    ~HelpDialog();

private slots:
    void on_closeButton_clicked();

private:
    Ui::HelpDialog *ui;
};

