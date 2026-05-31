#include "../include/database.h"

Database::Database(QObject* parent) : QObject(parent)
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    apiKey = env.value("SUPABASE_API_KEY").toUtf8();
    baseUrl = env.value("SUPABASE_BASE_URL");
    authUrl = env.value("SUPABASE_AUTH_URL");
}

Database::~Database()
{

}

void Database::clear()
{
    this->courseData.clear();
    this->friendData.clear();
}

void Database::createUser(const QString& username,
                          const QString& email,
                          const QString& password)
{
    // Set up body
    QJsonObject data;
    data["username"] = username;
    QJsonObject credentials;
    credentials["email"] = email;
    credentials["password"] = password;
    credentials["data"] =  data;
    QByteArray jsonData = QJsonDocument(credentials).toJson();

    // Request type
    QJsonDocument doc = databaseQuery("/signup", &jsonData);

    if (doc.isNull() || !doc.isObject())
    {
        qDebug() << "Failed to create user.";
        emit loginFailed();
        return;
    }

    qDebug() << "Successfully created user.";

    // Although there is no data to fetch
    // This function essentially refreshes the data from last user
    fetchUserData();
}

void Database::loginUser(const QString& username,
                         const QString& email,
                         const QString& password)
{
    // Set up data
    QJsonObject data;
    data["username"] = username;
    QJsonObject credentials;
    credentials["email"] = email;
    credentials["password"] = password;
    credentials["data"] =  data;

    QByteArray jsonData = QJsonDocument(credentials).toJson();

    // Request type
    QJsonDocument doc = databaseQuery("/token?grant_type=password", &jsonData);

    if (!doc.isNull() && doc.isObject())
    {
        QJsonObject obj = doc.object();

        // Get user specific database variables from response
        this->userData.setAccessToken(obj["access_token"].toString());
        this->userData.setRefreshToken(obj["refresh_token"].toString());
        this->userData.setUserId(obj["user"].toObject()["id"].toString());

        fetchUserData();
    }
    else
    {
        qDebug() << "User not found.";
        emit loginFailed();
        return;
    }
}

bool Database::createCourse(const QString& courseName)
{
    QJsonObject json;
    json["profile_id"] = this->userData.getUserId();
    json["class_name"] = courseName;
    QByteArray jsonData = QJsonDocument(json).toJson();

    QJsonDocument doc = databaseQuery("/courses", &jsonData);

    // No data was returned
    if (!doc.isArray() || doc.array().isEmpty())
    {
        qDebug() << "Failed to create course";
        return false;
    }

    // Add to local database
    this->courseData.addCourse(courseName);

    qDebug() << "Course created successfully";
    return true;
}

Course* Database::getCourse(const QString& courseName)
{
    return this->courseData.findCourse(courseName);
}


bool Database::addFriend(const QString& userName)
{
    // Search to ensure that the user exists
    QJsonDocument doc = databaseQuery("/profiles?select=id,username,rank,study_hours&username=eq." + userName);

    // No data was returned
    if (!doc.isArray() || doc.array().isEmpty()) {
        qDebug() << "Friend not found.";
        return false;
    }

    QJsonObject obj = doc.array().first().toObject();
    QString friendId = obj.value("id").toString();
    RankType rank = static_cast<RankType>(obj.value("rank").toInt());
    int studyHours = obj.value("study_hours").toInt();
    QString userId = this->userData.getUserId();

    // Create the friendship in local and remote database
    QJsonObject body;
    body["profile_id"] = userId;
    body["friend_id"] = friendId;
    QByteArray jsonData = QJsonDocument(body).toJson();

    doc = databaseQuery("/friends", &jsonData);

    // No data was returned
    if (!doc.isArray() || doc.array().isEmpty())
    {
        qDebug() << "Failed to create friendship";
        return false;
    }

    // Add friend to local database
    this->friendData.addFriend(userName, rank, studyHours);

    qDebug() << "Friend successfully added!";
    return true;
}

Friend* Database::getFriend(const QString& friendName)
{
    return this->friendData.findFriend(friendName);
}

bool Database::updateStudyHours(double hours)
{
    // Set up body
    double newTotal = this->getStudyHours() + hours;
    QJsonObject body;
    body["study_hours"] = newTotal;
    QByteArray jsonData = QJsonDocument(body).toJson();

    QJsonDocument doc = databaseQuery("/profiles?id=eq." + this->userData.getUserId(), &jsonData);

    // No data was returned
    if (!doc.isArray() || doc.array().isEmpty())
    {
        qDebug() << "Failed to update study hours";
        return false;
    }

    this->userData.setStudyHours(newTotal);
    emit refreshStudyHours();
    qDebug() << "Study hours updated successfully!";

    return true;
}

bool Database::updateCourseHours(Course* course, double hours)
{
    // Set up body
    double newTotal = course->getStudyHours() + hours;
    QString encodedName = QUrl::toPercentEncoding(course->getName());
    QJsonObject body;
    body["study_hours"] = newTotal;
    QByteArray jsonData = QJsonDocument(body).toJson();

    QJsonDocument doc = databaseQuery("/courses?profile_id=eq."
                                      + this->userData.getUserId() +
                                      "&class_name=eq." + encodedName,
                                      &jsonData);


    // No data was returned
    if (!doc.isArray() || doc.array().isEmpty())
    {
        qDebug() << "Failed to update course study hours";
        return false;
    }

    updateStudyHours(hours);
    emit refreshStudyHours();
    qDebug() << "Study hours updated!";

    return true;
}

bool Database::addAssignment(Course* course, const QString& name, double weight, double grade)
{
    QString encodedName = QUrl::toPercentEncoding(course->getName());

    QJsonDocument doc = databaseQuery("/courses?profile_id=eq." + this->userData.getUserId() +
                                      "&class_name=eq." + encodedName + "&select=id");

    // No data was returned
    if (!doc.isArray() || doc.array().isEmpty())
    {
        qDebug() << "No matching course found";
        return false;
    }

    QJsonArray arr = doc.array();
    QString courseId = arr[0].toObject()["id"].toString();

    QJsonObject body;
    body["user_class_id"] = courseId;
    body["assignment_name"] = name;
    body["grade"] = grade;
    body["weight"] = weight;
    QByteArray jsonData = QJsonDocument(body).toJson();

    doc = databaseQuery("/assignments", &jsonData);

    // No data was returned
    if (!doc.isArray() || doc.array().isEmpty())
    {
        qDebug() << "Failed to create assignment";
        return false;
    }

    qDebug() << "Assignment created successfully!";

    return true;
}

const std::vector<Course>& Database::getCourses() const
{
    return this->courseData.getCourses();
}

RankType Database::getCareerRank() const
{
    if (this->courseData.isEmpty())
        return RankType::BEGINNER;

    double totalScore = 0.0;
    int countedCourses = 0;

    auto courses = courseData.getCourses();
    for (const Course& course : courses)
    {
        double grade = course.getGrade();          // assuming 0–100
        double studyHours = course.getStudyHours(); // total hours logged

        // compute individual course score
        double courseScore = (grade * 0.7) + (studyHours * 0.3);

        totalScore += courseScore;
        countedCourses++;
    }

    if (countedCourses == 0)
        return RankType::BEGINNER;

    double avgScore = totalScore / countedCourses;

    if (avgScore >= 80)
        return RankType::SCHOLAR;
    else if (avgScore >= 70)
        return RankType::PRODIGY;
    else if (avgScore >= 55)
        return RankType::RISINGSTAR;
    else if (avgScore >= 45)
        return RankType::LEARNER;
    else if (avgScore >= 35)
        return RankType::BEGINNER;
    else
        return RankType::BEGINNER;
}

const std::vector<Friend>& Database::getFriends() const
{
    return this->friendData.getFriends();
}


QString Database::getUsername() const
{
    return this->userData.getUsername();
}

RankType Database::getRank() const
{
    return this->userData.getRank();
}

double Database::getStudyHours() const
{
    return this->userData.getStudyHours();
}

void Database::refreshData()
{
    fetchUserData();
}

void Database::updateRank()
{
    this->userData.setRank(getCareerRank());

    emit refreshRank();
}

QJsonDocument Database::databaseQuery(const QString& query, QByteArray* jsonData)
{
    // Set up the url
    QUrl url(baseUrl + query);

    bool tokenRequest{};
    if (query.contains("/token") || query.contains("/signup"))
    {
        url = authUrl + query;
        tokenRequest = true;
    }

    // headers
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("apikey", this->apiKey);
    request.setRawHeader("Prefer", "return=representation");

    // Only set auth header if not a token request
    if (!tokenRequest)
        request.setRawHeader("Authorization", QString("Bearer %1").arg(this->userData.getAccessToken()).toUtf8());

    // Post or get request based on if jsonData is provided
    QNetworkReply* reply;
    if (jsonData)
    {
        QJsonObject obj = QJsonDocument::fromJson(*jsonData).object();
        if (obj.contains("study_hours"))
        {
            reply = manager.sendCustomRequest(request, "PATCH", *jsonData);
        }
        else
        {
            reply = manager.post(request, *jsonData);
        }
    }
    else
    {
        reply = manager.get(request);
    }

    // Loop until finished request
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error fetching:" << reply->errorString();
        reply->deleteLater();
        return QJsonDocument();
    }

    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);
    reply->deleteLater();

    return doc;
}

void Database::fetchUserData()
{
    // Select all data we want from the profiles table
    QJsonDocument doc = databaseQuery("/profiles?select=username,rank,study_hours&id=eq."
             + this->userData.getUserId());

    // No data was returned
    if (!doc.isArray() || doc.array().isEmpty())
    {
        qDebug() << "User not found.";
        emit loginFailed();
        return;
    }

    QJsonObject obj = doc.array().first().toObject();

    this->userData.setUsername(obj["username"].toString());
    this->userData.setRank(static_cast<RankType>(obj["rank"].toInt()));
    this->userData.setStudyHours(obj["study_hours"].toDouble());

    // Get the course related data
    getCourseData();

    // Get the friend related data
    getFriendData();

    // Refresh the home page
    emit populateHomePage();
}

void Database::getCourseData()
{
    // Select all data we want from courses table
    QUrl url("https://gmhrsbcdzksllhvrncgn.supabase.co/rest/v1/courses?profile_id=eq."
             + this->userData.getUserId());
    QNetworkRequest request(url);

    // headers
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("apikey", this->apiKey);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(this->userData.getAccessToken()).toUtf8());

    QNetworkReply* reply = manager.get(request);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    reply = manager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        emit loginFailed();
        qDebug() << "Error fetching courses:" << reply->errorString();
        return;
    }

    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);
    reply->deleteLater();

    // No data was returned
    if (!doc.isArray() || doc.array().isEmpty())
    {
        qDebug() << "No courses found.";
        emit populateHomePage();
        return;
    }

    // Add all of the courses
    QJsonArray courses = doc.array();
    for (const QJsonValue &val : courses)
    {
        QJsonObject obj = val.toObject();

        this->courseData.addCourse(
            obj["class_name"].toString(),
            obj["grade"].toDouble(),
            obj["study_hours"].toDouble(),
            static_cast<RankType>(obj["rank"].toInt()),
            obj["enrolled"].toBool());

        // Get all of the course assignments
        QString courseId = obj["id"].toString();

        QUrl url("https://gmhrsbcdzksllhvrncgn.supabase.co/rest/v1/assignments?select=*&user_class_id=eq." + courseId);
        QNetworkRequest request(url);

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader("apikey", this->apiKey);
        request.setRawHeader("Authorization", QString("Bearer %1").arg(this->userData.getAccessToken()).toUtf8());

        QNetworkReply* reply = manager.get(request);

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() != QNetworkReply::NoError)
        {
            qDebug() << "Assignment fetch failed for course" << courseId << ":" << reply->errorString();
            reply->deleteLater();
            return;
        }

        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);

        if (!jsonDoc.isArray())
        {
            qDebug() << "Invalid assignment response:" << response;
            reply->deleteLater();
            return;
        }

        QJsonArray assignments = jsonDoc.array();

        // Loop through assignments for this course
        for (const QJsonValue& val : assignments)
        {
            QJsonObject obj = val.toObject();

            this->courseData.addAssignment(
                obj["assignment_name"].toString(),
                obj["weight"].toDouble(),
                obj["grade"].toDouble()
                );
        }

        reply->deleteLater();

    }

    reply->deleteLater();
}

void Database::getFriendData()
{
    // Select all data we want from friends table
    QUrl url("https://gmhrsbcdzksllhvrncgn.supabase.co/rest/v1/friends?profile_id=eq."
             + this->userData.getUserId());

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("apikey", this->apiKey);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(this->userData.getAccessToken()).toUtf8());

    QNetworkReply* reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);

    // Add all of the friends
    QJsonArray friends = doc.array();
    for (const QJsonValue &val : friends)
    {
        QJsonObject obj = val.toObject();

        request.setUrl("https://gmhrsbcdzksllhvrncgn.supabase.co/rest/v1/profiles?select=username,rank,study_hours&id=eq."
                       + obj["friend_id"].toString());

        reply = manager.get(request);

        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() != QNetworkReply::NoError)
        {
            qDebug() << "Error fetching user:" << reply->errorString();
            return;
        }

        response = reply->readAll();
        doc = QJsonDocument::fromJson(response);
        reply->deleteLater();

        QJsonObject friendObj = doc.array().first().toObject();

            this->friendData.addFriend(
            friendObj["username"].toString(),
            static_cast<RankType>(friendObj["rank"].toInt()),
            friendObj["study_hours"].toInt());
    }

    reply->deleteLater();
}
