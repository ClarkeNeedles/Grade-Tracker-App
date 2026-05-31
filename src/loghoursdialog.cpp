#include "../include/loghoursdialog.h"
#include "../ui/ui_loghoursdialog.h"

LogHoursDialog::LogHoursDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::LogHoursDialog)
{
    ui->setupUi(this);

    setWindowTitle("Log Study Hours");

    ui->hoursSpin->setMinimum(0);
    ui->hoursSpin->setMaximum(9999);
    ui->hoursSpin->setDecimals(2);
    ui->hoursSpin->setSingleStep(1);
    ui->hoursSpin->setValue(0);
}

LogHoursDialog::~LogHoursDialog()
{
    delete ui;
}

double LogHoursDialog::getStudyHours() const
{
    return ui->hoursSpin->value();
}
