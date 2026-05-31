#include "../include/inputgradedialog.h"
#include "../ui/ui_inputgradedialog.h"

InputGradeDialog::InputGradeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InputGradeDialog)
{
    ui->setupUi(this);
}

InputGradeDialog::~InputGradeDialog()
{
    delete ui;
}

QString InputGradeDialog::getAssignmentName() const
{
    return ui->assignmentNameEdit->text();
}

double InputGradeDialog::getWeight() const
{
    return ui->weightSpin->value();
}

double InputGradeDialog::getGrade() const
{
    return ui->gradeSpin->value();
}
