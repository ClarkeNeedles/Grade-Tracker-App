#include "../include/coursecreatedialog.h"
#include "../ui/ui_coursecreatedialog.h"

CourseCreateDialog::CourseCreateDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::CourseCreateDialog)
{
    ui->setupUi(this);

    setWindowTitle("Create Course");
}

CourseCreateDialog::~CourseCreateDialog()
{
    delete ui;
}

QString CourseCreateDialog::getCourseName() const
{
    return ui->courseNameEdit->text();
}
