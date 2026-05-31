#include "../include/mainwindow.h"
#include "../ui/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , database(new Database(this))
{
    ui->setupUi(this);
    login = new LoginPage(database, this);
    home = new HomePage(database, this);
    course = new CoursePage(database, this);
    allCourses = new AllCoursesPage(database, this);
    fpage = new FriendPage(database, this);

    setWindowTitle("App");

    // Add pages
    ui->stackedWidget->addWidget(login);
    ui->stackedWidget->addWidget(home);
    ui->stackedWidget->addWidget(course);
    ui->stackedWidget->addWidget(allCourses);
    ui->stackedWidget->addWidget(fpage);

    // Set up listeners
    if (login)
    {
        connect(login, &LoginPage::loginStarted, this, &MainWindow::showLoading);
    }

    if (home)
    {
        connect(home, &HomePage::login, this, [this]() {
            ui->stackedWidget->setCurrentWidget(home);

            // Hide loading screen
            if (loading)
            {
                loading->close();
                loading->deleteLater();
                loading = nullptr;
            }
        });

        connect(home, &HomePage::logout, this, [this]() {
            // Clear the database
            this->database->clear();

            ui->stackedWidget->setCurrentWidget(login);
        });

        connect(home, &HomePage::refreshStart, this, [this]() {
            showLoading();
        });

        connect(home, &HomePage::refreshStop, this, [this]() {
            // Hide loading screen
            if (loading)
            {
                loading->close();
                loading->deleteLater();
                loading = nullptr;
            }
        });
    }

    if (database)
    {
        connect(database, &Database::loginFailed, this, [this]() {
            if (loading)
            {
                loading->close();
                loading->deleteLater();
                loading = nullptr;
            }

            ErrorDialog* error = new ErrorDialog("Login failed.", this);
            error->show();
        });
    }

    if (course)
    {
        connect(home, &HomePage::goToCoursePage, this, [this](const QString& courseName) {
            course->populateCoursePage(courseName);
            ui->stackedWidget->setCurrentWidget(course);
        });

        connect(course, &CoursePage::closeCoursePage, this, [this]() {
            ui->stackedWidget->setCurrentWidget(home);
        });
    }

    if (allCourses)
    {
        connect(home, &HomePage::goToCourses, this, [this]() {
            allCourses->refreshCourses();
            ui->stackedWidget->setCurrentWidget(allCourses);
        });

        connect(allCourses, &AllCoursesPage::goBackHome, this, [this]() {
            ui->stackedWidget->setCurrentWidget(home);
        });
    }

    if (fpage)
    {
        connect(home, &HomePage::goToFriendPage, this, [this](const QString& friendName) {
            fpage->populateFriendPage(friendName);
            ui->stackedWidget->setCurrentWidget(fpage);
        });

        connect(fpage, &FriendPage::closeFriendPage, this, [this]() {
            ui->stackedWidget->setCurrentWidget(home);
        });
    }

    // Set the startup page
    ui->stackedWidget->setCurrentWidget(login);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showLoading()
{
    // Show loading screen
    loading = new LoadingDialog(this);
    loading->setModal(true);
    loading->show();
}
