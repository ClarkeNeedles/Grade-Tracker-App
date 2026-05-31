#include "../include/timerdialog.h"
#include "../ui/ui_timerdialog.h"

TimerDialog::TimerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TimerDialog)
    , timer(new QTimer(this))
{
    ui->setupUi(this);

    setWindowTitle("Timer");

    connect(timer, &QTimer::timeout, this, &TimerDialog::updateDisplay);
    connect(ui->log, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->close, &QPushButton::clicked, this, &QDialog::reject);
}

TimerDialog::~TimerDialog()
{
    delete ui;
}

void TimerDialog::on_start_clicked()
{
    timer->start(1000);
}

void TimerDialog::on_stop_clicked()
{
    timer->stop();
}

void TimerDialog::updateDisplay()
{
    // Update time
    elapsedSeconds++;

    // Calculate minutes and seconds
    int hours   = elapsedSeconds / 3600;
    int minutes = (elapsedSeconds % 3600) / 60;
    int seconds = elapsedSeconds % 60;

    // Format display
    ui->clock->display(
        QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'))
        );
}

double TimerDialog::getStudyHours() const
{
    return ((double)elapsedSeconds) / 3600.0;
}


