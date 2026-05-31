#ifndef TIMERDIALOG_H
#define TIMERDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class TimerDialog;
}

class TimerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimerDialog(QWidget *parent = nullptr);
    ~TimerDialog();
    double getStudyHours() const;

private slots:
    void on_start_clicked();

    void on_stop_clicked();

    void updateDisplay();

private:
    Ui::TimerDialog *ui;
    QTimer *timer;
    int elapsedSeconds = 0;
};

#endif // TIMER_H
