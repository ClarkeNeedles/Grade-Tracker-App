#include "../include/loginpage.h"
#include "../ui/ui_loginpage.h"

LoginPage::LoginPage(Database* database, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LoginPage)
    , database(database)
{
    ui->setupUi(this);
}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::on_createButton_clicked()
{
    // Get values from login screen
    QString username = ui->usernameEdit->text();
    QString email    = ui->emailEdit->text();
    QString password = ui->passwordEdit->text();

    // Error message to display
    QString errorMessage;

    if(username.isEmpty())
    {
        errorMessage = "Username cannot be empty.";
    }
    else if(email.isEmpty())
    {
        errorMessage = "Email cannot be empty.";
    }
    else if(password.isEmpty())
    {
        errorMessage = "Password cannot be empty.";
    }
    else if(password.length() < 6)
    {
        errorMessage = "Password must be at least 6 characters.";
    }
    else
    {
        QRegularExpression regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        if(!regex.match(email).hasMatch()) {
            errorMessage = "Email format is invalid.";
        }
    }

    // If no errors present, create account
    if(errorMessage.isEmpty())
    {
        database->createUser(username, email, password);
    }
    else
    {
        ErrorDialog* error = new ErrorDialog(errorMessage, this);
        error->show();
    }
}

void LoginPage::on_loginButton_clicked()
{
    emit loginStarted();

    database->loginUser(ui->usernameEdit->text(),
                       ui->emailEdit->text(),
                       ui->passwordEdit->text());
}
