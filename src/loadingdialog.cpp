#include "../include/loadingdialog.h"
#include "../ui/ui_loadingdialog.h"

LoadingDialog::LoadingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoadingDialog)
{
    ui->setupUi(this);

    // Set window settings
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // Set loading gif
    QMovie* movie = new QMovie(":/assets/images/loading.gif");
    movie->setScaledSize(QSize(64, 64));
    ui->loadingLabel->setMovie(movie);
    ui->loadingLabel->setAlignment(Qt::AlignCenter);
    movie->start();
}

LoadingDialog::~LoadingDialog()
{
    delete ui;
}
