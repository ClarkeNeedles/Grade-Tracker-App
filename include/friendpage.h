#ifndef FRIENDPAGE_H
#define FRIENDPAGE_H

#include <QDialog>
#include "database.h"

namespace Ui {
class FriendPage;
}

class FriendPage : public QDialog
{
    Q_OBJECT

signals:
    void closeFriendPage();

public:
    explicit FriendPage(Database* database, QWidget *parent = nullptr);
    ~FriendPage();

    void populateFriendPage(const QString& friendName);

private slots:
    void on_backButton_clicked();

private:
    Ui::FriendPage *ui;

    Database* database;
};

#endif // FRIENDPAGE_H
