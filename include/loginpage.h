#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include "database.h"
#include "errordialog.h"

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget
{
    Q_OBJECT

signals:
    void loginStarted();

public:
    explicit LoginPage(
        Database* database,
        QWidget* parent = nullptr);
    ~LoginPage();

private slots:
    void on_createButton_clicked();

    void on_loginButton_clicked();

private:
    Ui::LoginPage *ui;

    Database* database;
};

#endif // LOGINPAGE_H
