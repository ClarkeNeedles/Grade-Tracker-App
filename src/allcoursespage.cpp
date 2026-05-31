#include "../include/allcoursespage.h"
#include "../ui/ui_allcoursespage.h"

/**
 * All course page constructor
 *
 * @param database The system database containing user information
 * @param parent The QWidget parent of the courses page
*/
AllCoursesPage::AllCoursesPage(Database* database, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::AllCoursesPage)
    , database(database)
{
    ui->setupUi(this);
}

/**
 * All course page destructor
*/
AllCoursesPage::~AllCoursesPage()
{
    delete ui;
}

/**
 * Return button moves user to home page
*/
void AllCoursesPage::on_backButton_clicked()
{
    emit goBackHome();
}

/**
 * Refresh button updates course information from database
*/
void AllCoursesPage::refreshCourses()
{
    // Clear course list to make space for updated information
    ui->coursesList->clear();

    // Check that database is not null
    if(!database)
        return;

    // Use getter for database courses as they are private
    const std::vector<Course>& allCourses = database->getCourses();

    // Add all course names to the ui
    for (const Course& course : allCourses)
        ui->coursesList->addItem(course.getName());

    // Call updater functions for display of rank and stats
    updateCareerRank();
    updateStats();
}

/**
 * Function to update overall rank based on course information
*/
void AllCoursesPage::updateCareerRank()
{
    // Ensure database is not null
    if(!database)
        return;
    // Use course getter to access courses
    const std::vector<Course>& allCourses = database->getCourses();

    // Set basic rank for no courses.
    if(allCourses.empty())
    {
        ui->careerRankLabel->setText("Career Rank: Beginner");
        ui->averageGradeLabel->setText("Average Grade: --%");
        ui->rankIconLabel->clear();
        return;
    }

    // Calculate the weighted average of all courses
    double totalScore = 0.0;
    int countedCourses = 0;
    for(const Course& course : allCourses)
    {
        double grade = course.getGrade();
        double hours = course.getStudyHours();
        double courseScore = (grade * 0.7) + (hours * 0.3);
        totalScore += courseScore;
        countedCourses++;
    }

    // Get the average rank across all courses
    double avgScore = totalScore / countedCourses;
    // Determine rank
    RankType careerRank;
    if(avgScore >= 80)
        careerRank = RankType::SCHOLAR;
    else if(avgScore >= 70)
        careerRank = RankType::PRODIGY;
    else if(avgScore >= 55)
        careerRank = RankType::RISINGSTAR;
    else if(avgScore >= 45)
        careerRank = RankType::LEARNER;
    else
        careerRank = RankType::BEGINNER;

    // Update rank text and icon
    QString rankText;
    QString imagePath;
    switch (careerRank)
    {
    case RankType::BEGINNER:
        rankText = "Career Rank: Beginner";
        imagePath = ":/ranks/../images/beginner.png";
        break;
    case RankType::LEARNER:
        rankText = "Career Rank: Learner";
        imagePath = ":/ranks/../images/learner.png";
        break;
    case RankType::RISINGSTAR:
        rankText = "Career Rank: Rising Star";
        imagePath = ":/ranks/../images/risingstar.png";
        break;
    case RankType::PRODIGY:
        rankText = "Career Rank: Prodigy";
        imagePath = ":/ranks/../images/prodigy.png";
        break;
    case RankType::SCHOLAR:
        rankText = "Career Rank: Scholar";
        imagePath = ":/ranks/../images/scholar.png";
        break;
    }

    // Set all relevant ui elements
    ui->careerRankLabel->setText(rankText);
    ui->averageGradeLabel->setText(QString("Average Grade: %1%").arg(avgScore, 0, 'f', 1));
    ui->rankIconLabel->setPixmap(QPixmap(imagePath));
    ui->rankIconLabel->setScaledContents(true);
}

/**
 * Update course related stats based on course information
*/
void AllCoursesPage::updateStats()
{
    // Database null check
    if(!database)
        return;

    // Use of course vector getter
    const std::vector<Course>& allCourses = database->getCourses();
    // Standard output for no courses
    if(allCourses.empty())
    {
        ui->efficientCourseLabel->setText("Most Efficient Course: --");
        ui->highestGradeLabel->setText("Highest Grade Achieved: --");
        ui->lowestGradeLabel->setText("Lowest Grade Achieved: --");
        ui->mostHoursLabel->setText("Course with Most Hours: --");
        ui->longestSessionLabel->setText("Longest Study Session: --");
        return;
    }

    // Create pointers for courses of interest and create other useful variables
    const Course* mostEfficient = nullptr;
    const Course* highestGrade = nullptr;
    const Course* lowestGrade = nullptr;
    const Course* mostHours = nullptr;
    double bestEfficiencyScore = -1;
    // Loop through courses
    for(const Course& course : allCourses)
    {
        double grade = course.getGrade();
        double hours = course.getStudyHours();

        // Determine highest grade, lowest grade, and most study hours in course
        if(!highestGrade || grade > highestGrade->getGrade())
            highestGrade = &course;

        if(!lowestGrade || grade < lowestGrade->getGrade())
            lowestGrade = &course;

        if(!mostHours || hours > mostHours->getStudyHours())
            mostHours = &course;

        // Find course with most grade per amount of studying
        // Efficiency score = grade / (hours + 1)  (+1 avoids div by 0)
        double efficiency = grade / (hours + 1);
        if(efficiency > bestEfficiencyScore)
        {
            bestEfficiencyScore = efficiency;
            mostEfficient = &course;
        }
    }

    // Update labels
    ui->highestGradeLabel->setText(
        QString("Highest Grade Achieved: %1 (%2%)")
            .arg(highestGrade ? highestGrade->getName() : "--")
            .arg(highestGrade ? highestGrade->getGrade() : 0));
    ui->lowestGradeLabel->setText(
        QString("Lowest Grade Achieved: %1 (%2%)")
            .arg(lowestGrade ? lowestGrade->getName() : "--")
            .arg(lowestGrade ? lowestGrade->getGrade() : 0));
    ui->mostHoursLabel->setText(
        QString("Course with Most Hours: %1 (%2h)")
            .arg(mostHours ? mostHours->getName() : "--")
            .arg(mostHours ? mostHours->getStudyHours() : 0));
    ui->efficientCourseLabel->setText(
        QString("Most Efficient Course: %1 (%2%)")
            .arg(mostEfficient ? mostEfficient->getName() : "--")
            .arg(bestEfficiencyScore));
}
