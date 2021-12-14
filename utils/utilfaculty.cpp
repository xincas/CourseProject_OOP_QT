#include "utilfaculty.h"

UtilFaculty::UtilFaculty(QList<Faculty*>* f, QList<Group*>* g, QList<Teacher*>* t,
                         QList<Subject*>* sub, QList<Student*>* stud, QList<Grade*>* gr):
    faculties_(f), groups_(g), teachers_(t), subjects_(sub), students_(stud), grades_(gr)
{

}

Faculty *UtilFaculty::facultyById(int id)
{
    for(auto& f : *faculties_) {
        if (f->id() == id)
            return f;
    }
    return nullptr;
}

Group *UtilFaculty::groupById(int id)
{
    for(auto& g : *groups_) {
        if (g->id() == id)
            return g;
    }
    return nullptr;
}

Student *UtilFaculty::studentById(int id)
{
    for(auto& s : *students_) {
        if (s->id() == id)
            return s;
    }
    return nullptr;
}

Subject *UtilFaculty::subjectById(int id)
{
    for(auto& s : *subjects_) {
        if (s->id() == id)
            return s;
    }
    return nullptr;
}

Teacher *UtilFaculty::teacherById(int id)
{
    for(auto& s : *teachers_) {
        if (s->id() == id)
            return s;
    }
    return nullptr;
}

Grade *UtilFaculty::gradeById(int id)
{
    for(auto& s : *grades_) {
        if (s->id() == id)
            return s;
    }
    return nullptr;
}

Node *UtilFaculty::parserForFacultyAndGrops(const QList<Faculty *> &fac)
{
    Node* root = new Node(Node::None, 0);

    for(auto& f: fac){
        Node* n_f = new Node(Node::Faculty, f);
        n_f->parent = root;
        root->children.append(n_f);
        for(auto& gr: f->getGroups()){
            Node* n_g = new Node(Node::Group, gr);
            n_g->parent = n_f;
            n_f->children.append(n_g);
        }
    }

    return root;
}

Subject::CheckType UtilFaculty::checkTypeFromStr(QString str)
{
    if (str == "КП") {
        return Subject::CheckType::CourseProject;
    } else if (str == "Э") {
        return Subject::CheckType::Exam;
    } else {
        return Subject::CheckType::Credit;
    }
}

QString UtilFaculty::checkTypeToStr(Subject::CheckType check)
{
    switch (check) {
    case Subject::CourseProject:
        return "Курсовой проект";
    case Subject::Exam:
        return "Экзамен";
    default:
        return "Зачет";
    }
}

QString UtilFaculty::checkTypeToStrSQL(Subject::CheckType check)
{
    switch (check) {
    case Subject::CourseProject:
        return "КП";
    case Subject::Exam:
        return "Э";
    default:
        return "З";
    }
}

void UtilFaculty::deleteFaculty(Faculty *f)
{
    for(auto gr: f->getGroups()) {
        deleteGroup(gr);
    }
    faculties_->removeOne(f);
    delete f; f = 0;
}

void UtilFaculty::deleteGroup(Group *f)
{
    for(auto st: f->getStuds()) {
        deleteStudent(st);
    }
    groups_->removeOne(f);
    delete f; f = 0;
}

void UtilFaculty::deleteStudent(Student *f)
{
    for(auto gr: f->getGrades()) {
        deleteGrade(gr);
    }
    students_->removeOne(f);
    delete f; f = 0;
}

void UtilFaculty::deleteSubject(Subject *f)
{
    subjects_->removeOne(f);
    delete f; f = 0;
}

void UtilFaculty::deleteTeacher(Teacher *f)
{
    teachers_->removeOne(f);
    delete f; f = 0;
}

void UtilFaculty::deleteGrade(Grade *f)
{
    grades_->removeOne(f);
    delete f; f = 0;
}
