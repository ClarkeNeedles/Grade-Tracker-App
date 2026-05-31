#ifndef USERDATA_H
#define USERDATA_H

#include <QString>
#include "rank_type.h"

class UserData
{
private:
    QString userId;
    QString accessToken;
    QString refreshToken;
    QString username;
    RankType rank{};
    double studyHours{};

public:
    UserData();

    // Get functions
    QString getUserId() const;
    QString getAccessToken() const;
    QString getRefreshToken() const;
    QString getUsername() const;
    RankType getRank() const;
    double getStudyHours() const;

    // Set functions
    void setUserId(const QString& userId);
    void setAccessToken(const QString& accessToken);
    void setRefreshToken(const QString& refreshToken);
    void setUsername(const QString& username);
    void setRank(RankType rank);
    void setStudyHours(double studyHours);
};

#endif // USERDATA_H
