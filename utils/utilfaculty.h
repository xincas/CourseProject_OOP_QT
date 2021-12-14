#ifndef UTILFACULTY_H
#define UTILFACULTY_H

#include "models/treeModels/facultygroupmodel.h"
#include "models/listModels/facultylistmodel.h"
#include "models/listModels/grouplistmodel.h"
#include "models/listModels/teacherlistmodel.h"
#include "models/listModels/subjectlistmodel.h"
#include "models/tableModels/studentfiltermodel.h"
#include "models/tableModels/studtablemodel.h"
#include "models/tableModels/gradetablemodel.h"

class UtilFaculty
{
public:
    UtilFaculty(QList<Faculty*>* f, QList<Group*>* g, QList<Teacher*>* t,
                QList<Subject*>* sub, QList<Student*>* stud, QList<Grade*>* gr);

    Faculty* facultyById(int id);
    Group* groupById(int id);
    Student* studentById(int id);
    Subject* subjectById(int id);
    Teacher* teacherById(int id);
    Grade* gradeById(int id);

    static Node* parserForFacultyAndGrops(const QList<Faculty*>& fac);
    static Subject::CheckType checkTypeFromStr(QString str);
    static QString checkTypeToStr(Subject::CheckType check);
    static QString checkTypeToStrSQL(Subject::CheckType check);

    void deleteFaculty(Faculty* f);
    void deleteGroup(Group* f);
    void deleteStudent(Student* f);
    void deleteSubject(Subject* f);
    void deleteTeacher(Teacher* f);
    void deleteGrade(Grade* f);

    template<typename T>
    static bool indexIsValid(int index, QList<T> conteiner)
    {
        if (index < 0)
            return false;
        if (index >= conteiner.count())
            return false;
        return true;
    }

private:
    QList<Faculty*>* faculties_;
    QList<Group*>* groups_;
    QList<Teacher*>* teachers_;
    QList<Subject*>* subjects_;
    QList<Student*>* students_;
    QList<Grade*>* grades_;
};

#endif // UTILFACULTY_H
