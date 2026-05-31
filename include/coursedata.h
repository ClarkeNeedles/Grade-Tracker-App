#ifndef COURSEDATA_H
#define COURSEDATA_H

#include "course.h"

class CourseData
{
private:
    std::vector<Course> courses;

public:
    CourseData();

    void clear();

    void addCourse(const QString& courseName);

    void addCourse(const QString& courseName,
           double grade,
           double studyHours,
           RankType rank,
           bool enrolled);

    void addAssignment(const QString& name,
                      double weight,
                      double grade);

    const std::vector<Course>& getCourses() const { return this->courses; }

    Course* findCourse(const QString& courseName);

    bool isEmpty() const { return this->courses.empty(); }
};

#endif // COURSEDATA_H
