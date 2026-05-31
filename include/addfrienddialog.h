#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QDialog>

namespace Ui {
class AddFriendDialog;
}

class AddFriendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriendDialog(QWidget *parent = nullptr);
    ~AddFriendDialog();

    QString getFriendName() const;

private:
    Ui::AddFriendDialog *ui;
};

#endif // ADDFRIEND_H
