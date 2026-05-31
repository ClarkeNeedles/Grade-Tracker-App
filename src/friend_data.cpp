#include "../include/friend_data.h"

FriendData::FriendData() {}

void FriendData::clear()
{
    this->friends.clear();
}

void FriendData::addFriend(const QString& username,
               RankType rank,
               int studyHours)
{
    this->friends.push_back(Friend(username, rank, studyHours));
}

Friend* FriendData::findFriend(const QString& friendName)
{
    // Search for friend in vector
    auto f = std::find_if(this->friends.begin(), this->friends.end(),
                               [friendName](const Friend& f) {
                                   return f.username == friendName;
                               });

    if (f != friends.end())
    {
        // Return pointer to friend in vector
        return &(*f);
    }

    return nullptr;
}
