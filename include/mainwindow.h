#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"
#include "loginpage.h"
#include "homepage.h"
#include "coursepage.h"
#include "loadingdialog.h"
#include "errordialog.h"
#include "allcoursespage.h"
#include "friendpage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    Database* database;

    // Pages
    LoginPage* login;
    HomePage* home;
    CoursePage* course;
    AllCoursesPage* allCourses;
    FriendPage* fpage;

    // Loading screen
    LoadingDialog* loading = nullptr;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void showLoading();
};

#endif // MAINWINDOW_H
