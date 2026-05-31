#include "../include/friendpage.h"
#include "../ui/ui_friendpage.h"

FriendPage::FriendPage(Database* database,
                       QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FriendPage)
    , database(database)
{
    ui->setupUi(this);
}

FriendPage::~FriendPage()
{
    delete ui;
}

void FriendPage::populateFriendPage(const QString& friendName)
{
    Friend* f = database->getFriend(friendName);

    if (!f)
        return;

    ui->usernameLabel->setText("Username: " + f->username);
    ui->studyHoursLabel->setText("Total Study Hours: " + QString::number(f->studyHours));

    // Set rank image
    QPixmap pixmap;
    switch(f->rank)
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
}

void FriendPage::on_backButton_clicked()
{
    emit closeFriendPage();
}

