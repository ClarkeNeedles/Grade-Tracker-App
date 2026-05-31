#include "../include/userdata.h"

UserData::UserData() {}

// Get functions
QString UserData::getUserId() const
{
    return this->userId;
}

QString UserData::getAccessToken() const
{
    return this->accessToken;
}

QString UserData::getRefreshToken() const
{
    return this->refreshToken;
}

QString UserData::getUsername() const
{
    return this->username;
}

RankType UserData::getRank() const
{
    return this->rank;
}

double UserData::getStudyHours() const
{
    return this->studyHours;
}

// Set functions
void UserData::setUserId(const QString& userId)
{
    this->userId = userId;
}

void UserData::setAccessToken(const QString& accessToken)
{
    this->accessToken = accessToken;
}

void UserData::setRefreshToken(const QString& refreshToken)
{
    this->refreshToken = refreshToken;
}

void UserData::setUsername(const QString& username)
{
    this->username = username;
}

void UserData::setRank(RankType rank)
{
    this->rank = rank;
}

void UserData::setStudyHours(double studyHours)
{
    this->studyHours = studyHours;
}
