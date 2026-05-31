#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include <qpushbutton.h>
#include "homepage.h"
#include "database.h"
#include "course.h"

namespace Ui {
class AllCoursesPage;
}

class Database;  // forward declaration

class AllCoursesPage : public QWidget
{
    Q_OBJECT

public:
    explicit AllCoursesPage(Database* database = nullptr, QWidget* parent = nullptr);

    void refreshCourses();

    void updateCareerRank();

    void updateStats();

    ~AllCoursesPage();

signals:
    void goBackHome();

private slots:
    void on_backButton_clicked();

private:
    Ui::AllCoursesPage* ui;
    Database* database;
};
