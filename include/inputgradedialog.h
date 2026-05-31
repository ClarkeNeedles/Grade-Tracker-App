#ifndef INPUTGRADEDIALOG_H
#define INPUTGRADEDIALOG_H

#include <QDialog>

namespace Ui {
class InputGradeDialog;
}

class InputGradeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputGradeDialog(QWidget *parent = nullptr);
    ~InputGradeDialog();

    QString getAssignmentName() const;
    double getWeight() const;
    double getGrade() const;

private:
    Ui::InputGradeDialog *ui;
};

#endif // INPUTGRADEDIALOG_H
