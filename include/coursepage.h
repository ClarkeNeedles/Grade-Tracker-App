#ifndef COURSEPAGE_H
#define COURSEPAGE_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QPixmap>
#include <QListWidgetItem>
#include <QMessageBox>
#include "database.h"
#include "loghoursdialog.h"
#include "timerdialog.h"
#include "errordialog.h"
#include "inputgradedialog.h"

namespace Ui {
class CoursePage;
}

class CoursePage : public QWidget
{
    Q_OBJECT

signals:
    void closeCoursePage();

public:
    explicit CoursePage(Database* database,
                        QWidget *parent = nullptr);
    ~CoursePage();

    void populateCoursePage(const QString& courseName);

private slots:
    void on_backButton_clicked();
    void on_logHoursButton_clicked();
    void on_timerButton_clicked();
    void on_inputGradeButton_clicked();
    void on_editCourseButton_clicked();
    void on_completeButton_clicked();
    void on_assignmentsList_itemDoubleClicked(QListWidgetItem* item);

private:
    Ui::CoursePage *ui;
    Database* database;
    Course* currentCourse;

    void updateGradeLabel();
    void updateStudyHours();
    void updatePieChart();
    void updateAssignmentsList();
    void updateRankDisplay();
};

#endif // COURSEPAGE_H
