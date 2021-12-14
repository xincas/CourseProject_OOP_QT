#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDriver>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "utils/utilfaculty.h"

class DBManager : public QObject
{
    Q_OBJECT
public:
    explicit DBManager(QObject *parent = nullptr);
    DBManager(QList<Faculty*>* f, QList<Group*>* g, QList<Teacher*>* t,
              QList<Subject*>* sub, QList<Student*>* stud, QList<Grade*>* gr, UtilFaculty* util, QObject *parent = nullptr);

    bool connectToDB();
    bool initEntities();

    bool selectFacultyById(int id, Faculty* faculty);
    bool insertFaculty(Faculty* faculty);
    bool updateFaculty(Faculty* faculty);
    bool deleteFaculty(int id);

    bool selectGroupById(int id, Group* group);
    bool insertGroup(Group* group);
    bool updateGroup(Group* group);
    bool deleteGroup(int id);

    bool selectTeacherById(int id, Teacher* teacher);
    bool insertTeacher(Teacher* teacher);
    bool updateTeacher(Teacher* teacher);
    bool deleteTeacher(int id);

    bool selectSubjectById(int id, Subject* subject);
    bool insertSubject(Subject* subject);
    bool updateSubject(Subject* subject);
    bool deleteSubject(int id);

    //bool selectSubsGroupsById(int id, Group* stud);
    bool insertSubsGroups(int id_sub, int id_gr);
    bool updateSubsGroups(int id_sub, int id_gr);
    bool deleteSubsGroups(int id_sub, int id_gr);

    //bool selectSubsTeachersById(int id, Group* stud);
    bool insertSubsTeachers(int id_sub, int id_tech);
    bool updateSubsTeachers(int id_sub, int id_tech);
    bool deleteSubsTeachers(int id_sub, int id_tech);

    bool selectStudentById(int id, Student* stud);
    bool insertStudent(Student* student);
    bool updateStudent(Student* student);
    bool deleteStudent(int id);

    bool selectGradeById(int id, Grade* gr);
    bool insertGrade(Grade* grade);
    bool updateGrade(Grade* grade);
    bool deleteGrade(int id);

    QString lastError();

private:
    bool initFaculties();
    bool initGroups();
    bool initTeachers();
    bool initSubjects();
    bool initStudents();
    bool initGrades();
    bool initSubsOfGroups();
    bool initSubsOfTeachers();

private:
    QSqlDatabase db_;

    QList<Faculty*>* faculties_;
    QList<Group*>* groups_;
    QList<Teacher*>* teachers_;
    QList<Subject*>* subjects_;
    QList<Student*>* students_;
    QList<Grade*>* grades_;

    UtilFaculty* util_;

    QString last_error_;
};

#endif // DBMANAGER_H
