#ifndef LOGSTUDYHOURS_H
#define LOGSTUDYHOURS_H

#include <QDialog>

namespace Ui {
class LogHoursDialog;
}

class LogHoursDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogHoursDialog(QWidget* parent = nullptr);
    ~LogHoursDialog();

    double getStudyHours() const;

private:
    Ui::LogHoursDialog* ui;
};

#endif // LOGSTUDYHOURS_H
