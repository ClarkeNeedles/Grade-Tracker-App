#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QListWidgetItem>
#include "database.h"
#include "rank_type.h"
#include "coursecreatedialog.h"
#include "helpdialog.h"
#include "errordialog.h"
#include "addfrienddialog.h"
#include "loghoursdialog.h"
#include "timerdialog.h"

namespace Ui {
class HomePage;
}

class HomePage : public QWidget
{
    Q_OBJECT

signals:
    void logout();
    void login();
    void goToCourses();
    void goToCoursePage(const QString& courseName);
    void goToFriendPage(const QString& friendName);
    void refreshStart();
    void refreshStop();

public:
    explicit HomePage(
        Database* database,
        QWidget* parent = nullptr);
    ~HomePage();

private:
    void populateHomePage();

private slots:
    void on_logoutButton_clicked();

    void on_addCourseButton_clicked();

    void on_helpButton_clicked();

    void on_coursesList_itemDoubleClicked(QListWidgetItem* item);

    void on_friendsList_itemDoubleClicked(QListWidgetItem *item);

    void on_addFriendButton_clicked();

    void on_logHoursButton_clicked();

    void on_studyTimerButton_clicked();

    void on_coursesButton_clicked();

    void on_refreshButton_clicked();

private:
    Ui::HomePage *ui;

    Database* database;
};

#endif // HOMEPAGE_H
