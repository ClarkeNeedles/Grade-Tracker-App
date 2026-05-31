#ifndef DATABASE_H
#define DATABASE_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QEventLoop>
#include <QProcessEnvironment>
#include "userdata.h"
#include "coursedata.h"
#include "friend_data.h"
#include "rank_type.h"
#include "course.h"

class Database : public QObject
{
    Q_OBJECT

signals:
    void populateHomePage();
    void populateFriendPage(Friend* f);
    void loginFailed();
    void refreshStudyHours();
    void refreshRank();

private:
    const QByteArray apiKey;
    const QString baseUrl;
    const QString authUrl;
    QNetworkAccessManager manager;

    // Data
    UserData userData;
    CourseData courseData;
    FriendData friendData;

public:
    explicit Database(QObject* parent = nullptr);
    ~Database();

    void clear();

    void createUser(const QString& username,
                    const QString& email,
                    const QString& password);

    void loginUser(const QString& username,
                   const QString& email,
                   const QString& password);

    bool createCourse(const QString& courseName);
    Course* getCourse(const QString& courseName);

    bool addFriend(const QString& userName);
    Friend* getFriend(const QString& friendName);

    bool updateStudyHours(double hours);
    bool updateCourseHours(Course* course, double hours);

    bool addAssignment(Course* course, const QString& name, double weight, double grade);

    const std::vector<Course>& getCourses() const;
    RankType getCareerRank() const;
    const std::vector<Friend>& getFriends() const;
    QString getUsername() const;
    RankType getRank() const;
    double getStudyHours() const;

    void refreshData();
    void updateRank();

private:
    QJsonDocument databaseQuery(const QString& query, QByteArray* body = nullptr);
    void fetchUserData();
    void getCourseData();
    void getFriendData();
};

#endif // DATABASE_H
