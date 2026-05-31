#include "../include/addfrienddialog.h"
#include "../ui/ui_addfrienddialog.h"

/**
 * Add friend dialog box constructor
 *
 * @param parent The QWidget parent of the dialog
*/
AddFriendDialog::AddFriendDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AddFriendDialog)
{
    ui->setupUi(this);
    setWindowTitle("Add Friend");
}

/**
 * Add friend dialog box destructor
*/
AddFriendDialog::~AddFriendDialog()
{
    delete ui;
}

/**
 * Friend name getter function
 *
 * @return QString Containing friend username
*/
QString AddFriendDialog::getFriendName() const
{
    return ui->friendNameEdit->text();
}
