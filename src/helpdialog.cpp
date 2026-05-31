#include "../include/helpdialog.h"
#include "../ui/ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    setWindowTitle("Help / About");
}

HelpDialog::~HelpDialog()
{
    delete ui;
}


void HelpDialog::on_closeButton_clicked()
{
    this->close();
}

