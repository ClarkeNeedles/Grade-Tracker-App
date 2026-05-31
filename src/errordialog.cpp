#include "../include/errordialog.h"
#include "../ui/ui_errordialog.h"

ErrorDialog::ErrorDialog(const QString& message, QWidget *parent)
    : QDialog(parent), ui(new Ui::ErrorDialog)
{
    ui->setupUi(this);

    ui->errorMessage->setText(message);

    setWindowTitle("Error");
}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}

void ErrorDialog::on_pushButton_clicked()
{
    this->close();
}

