#ifndef COURSECREATEDIALOG_H
#define COURSECREATEDIALOG_H

#include <QDialog>

namespace Ui {
class CourseCreateDialog;
}

class CourseCreateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CourseCreateDialog(QWidget* parent = nullptr);
    ~CourseCreateDialog();

    QString getCourseName() const;

private:
    Ui::CourseCreateDialog* ui;
};

#endif // COURSECREATEDIALOG_H
