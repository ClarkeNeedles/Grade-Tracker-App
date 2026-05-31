#include "../include/coursepage.h"
#include "../ui/ui_coursepage.h"

CoursePage::CoursePage(Database* database, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CoursePage)
    , database(database)
    , currentCourse(nullptr)
{
    ui->setupUi(this);

    // Initial empty chart
    auto *chart = new QChart();
    chart->setTitle("Grade Distribution");
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setTheme(QChart::ChartThemeLight);
    chart->setFont(QFont("Segoe UI", 8));
    chart->legend()->setFont(QFont("Segoe UI", 8));
    chart->legend()->setMarkerShape(QLegend::MarkerShapeCircle);

    ui->graphicsViewChart->setChart(chart);
}

CoursePage::~CoursePage()
{
    delete ui;
}

void CoursePage::populateCoursePage(const QString& courseName)
{
    currentCourse = database->getCourse(courseName);

    if (!currentCourse)
        return;

    ui->courseNameLabel->setText(currentCourse->getName());

    updateGradeLabel();
    updateStudyHours();
    updateRankDisplay();
    updatePieChart();
    updateAssignmentsList();
}

void CoursePage::updateGradeLabel()
{
    ui->currentGradeLabel->setText("Current Grade: " +
                                   QString::number(currentCourse->getGrade(), 'f', 2) + "%");
}

void CoursePage::updateStudyHours()
{
    ui->studyHoursLabel->setText("Course Study Hours: " +
                                 QString::number(currentCourse->getStudyHours(), 'f', 2));
}

void CoursePage::updatePieChart()
{
    if (!currentCourse) return;

    // Get assignments
    const auto& assignments = currentCourse->getAssignmentsList();

    // Create new series
    auto *series = new QPieSeries();

    if (assignments.empty())
    {
        // Show placeholder if no assignments
        series->append("No grades yet", 100);
        series->slices().at(0)->setBrush(QColor(200, 200, 200));
    }
    else
    {
        // Add each assignment as a slice
        for (const auto& assignment : assignments)
        {
            QString label = QString("%1: %2%").arg(assignment.name).arg(assignment.weight, 0, 'f', 2);
            series->append(label, assignment.weight);
        }

        // Color slices based on performance
        for (int i = 0; i < series->slices().size(); ++i)
        {
            auto slice = series->slices().at(i);
            double grade = assignments[i].grade;

            // Color based on grade
            if (grade >= 80)
                slice->setBrush(QColor(76, 175, 80)); // Green
            else if (grade >= 70)
                slice->setBrush(QColor(139, 195, 74)); // Light green
            else if (grade >= 60)
                slice->setBrush(QColor(255, 193, 7)); // Yellow
            else if (grade >= 50)
                slice->setBrush(QColor(255, 152, 0)); // Orange
            else
                slice->setBrush(QColor(244, 67, 54)); // Red
        }
    }

    // Update chart
    auto *chart = ui->graphicsViewChart->chart();
    chart->removeAllSeries();
    chart->addSeries(series);
    chart->setTitle("Grade Distribution");
}

void CoursePage::updateAssignmentsList()
{
    if (!currentCourse) return;

    ui->assignmentsList->clear();

    const auto& assignments = currentCourse->getAssignmentsList();
    for (const auto& assignment : assignments)
    {
        QString itemText = QString("%1: %2% (Weight: %3%)")
        .arg(assignment.name)
            .arg(assignment.grade, 0, 'f', 1)
            .arg(assignment.weight, 0, 'f', 1);
        ui->assignmentsList->addItem(itemText);
    }
}

void CoursePage::updateRankDisplay()
{
    if (!currentCourse)
        return;

    QPixmap pixmap;
    switch(currentCourse->getRank())
    {
    case RankType::BEGINNER:
        pixmap = QPixmap(":/ranks/../images/beginner.png");
        break;
    case RankType::LEARNER:
        pixmap = QPixmap(":/ranks/../images/learner.png");
        break;
    case RankType::RISINGSTAR:
        pixmap = QPixmap(":/ranks/../images/risingstar.png");
        break;
    case RankType::PRODIGY:
        pixmap = QPixmap(":/ranks/../images/prodigy.png");
        break;
    case RankType::SCHOLAR:
        pixmap = QPixmap(":/ranks/../images/scholar.png");
        break;
    }

    ui->rankLabel->setPixmap(pixmap);
    ui->rankLabel->setScaledContents(true);
}

void CoursePage::on_backButton_clicked()
{
    emit closeCoursePage();
}

void CoursePage::on_logHoursButton_clicked()
{
    if (!currentCourse) return;

    LogHoursDialog popup(this);
    popup.setModal(true);

    if (popup.exec() == QDialog::Accepted)
    {
        double hours = popup.getStudyHours();

        if (!database->updateCourseHours(this->currentCourse, static_cast<double>(hours)))
            return;

        currentCourse->addStudyHours(hours);
        updateStudyHours();
        updateRankDisplay();

        this->database->updateRank();
    }
}

void CoursePage::on_timerButton_clicked()
{
    if (!currentCourse) return;

    TimerDialog popup(this);
    popup.setModal(true);

    if (popup.exec() == QDialog::Accepted)
    {
        double hours = popup.getStudyHours();

        if (!database->updateCourseHours(this->currentCourse, static_cast<double>(hours)))
            return;

        currentCourse->addStudyHours(hours);
        updateStudyHours();
        updateRankDisplay();

        this->database->updateRank();
    }
}

void CoursePage::on_inputGradeButton_clicked()
{
    if (!currentCourse)
        return;

    InputGradeDialog popup(this);
    popup.setModal(true);

    if (popup.exec() == QDialog::Accepted)
    {
        QString name = popup.getAssignmentName();
        double weight = popup.getWeight();
        double grade = popup.getGrade();

        QString result = currentCourse->addAssignment(name, weight, grade);
        bool bResult = database->addAssignment(this->currentCourse, name, weight, grade);

        if ((result == "Grade added successfully") && bResult)
        {
            updateGradeLabel();
            updatePieChart();
            updateRankDisplay();
            updateAssignmentsList();
            this->database->updateRank();

            QMessageBox::information(this, "Success", "Grade added successfully!");
        }
        else
        {
            ErrorDialog* error = new ErrorDialog(result, this);
            error->show();
        }
    }
}

void CoursePage::on_editCourseButton_clicked()
{
    if (!currentCourse)
        return;

    const auto& assignments = currentCourse->getAssignmentsList();

    if (assignments.empty())
    {
        ErrorDialog* error = new ErrorDialog("No assignments to edit.", this);
        error->show();
        return;
    }

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Edit Course");
    msgBox.setText("Double-click an assignment in the list to delete it.");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void CoursePage::on_completeButton_clicked()
{
    if (!currentCourse)
        return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Complete Course",
                                  "Mark this course as complete?\nYou will no longer be able to add grades or hours.",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        currentCourse->setEnrolled(false);
        QMessageBox::information(this, "Success", "Course marked as complete!");
        emit closeCoursePage();
    }
}

void CoursePage::on_assignmentsList_itemDoubleClicked(QListWidgetItem* item)
{
    if (!currentCourse)
        return;

    // Extract assignment name from the item text
    QString itemText = item->text();
    QString assignmentName = itemText.split(":")[0];

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Assignment",
                                  "Are you sure you want to delete " + assignmentName + "?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        QString result = currentCourse->remAssignment(assignmentName);

        if (result == "Grade deleted successfully")
        {
            updateAssignmentsList();
            updatePieChart();

            QMessageBox::information(this, "Success", "Assignment deleted successfully!");
        }
        else
        {
            ErrorDialog* error = new ErrorDialog(result, this);
            error->show();
        }
    }
}

