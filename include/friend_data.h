#ifndef FRIEND_DATA_H
#define FRIEND_DATA_H

#include <QString>
#include "rank_type.h"

struct Friend
{
    QString username;
    RankType rank;
    int studyHours;

    Friend(const QString& username, RankType rank, int studyHours) :
        username(username), rank(rank), studyHours(studyHours) {}
};

class FriendData
{
private:
    std::vector<Friend> friends;

public:
    FriendData();

    void clear();

    void addFriend(const QString& username,
                   RankType rank,
                   int studyHours);

    const std::vector<Friend>& getFriends() const { return this->friends; }

    Friend* findFriend(const QString& friendName);
};

#endif // FRIEND_DATA_H
