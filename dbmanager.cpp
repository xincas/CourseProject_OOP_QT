#include "dbmanager.h"

DBManager::DBManager(QObject *parent) : QObject(parent)
{

}

DBManager::DBManager(QList<Faculty *> *f, QList<Group *> *g, QList<Teacher *> *t, QList<Subject *> *sub, QList<Student *> *stud, QList<Grade*>* gr, UtilFaculty* util, QObject *parent) :
    QObject(parent), faculties_(f), groups_(g), teachers_(t), subjects_(sub), students_(stud), grades_(gr), util_(util)
{

}

bool DBManager::connectToDB()
{
    db_ = QSqlDatabase::addDatabase("QMYSQL");
    db_.setHostName("localhost");
    db_.setUserName("root");
    db_.setPassword("lox123");
    db_.setDatabaseName("cp_oop");

    if (!db_.open()) {
        qDebug() << db_.lastError().text();
        return false;
    } else {
        qDebug() << "Connected succsessful!";
        return true;
    }
}

bool DBManager::initEntities()
{
    initFaculties();
    initGroups();
    initStudents();
    initSubjects();
    initTeachers();
    initGrades();

    initSubsOfGroups();
    initSubsOfTeachers();

    return true;
}

bool DBManager::selectFacultyById(int id, Faculty *faculty)
{

}

bool DBManager::insertFaculty(Faculty *faculty)
{
    if (!faculty)
        return false;

    if(faculty->id() == -1) {
        QSqlQuery query;
        query.prepare("insert into faculties(name)"
                      "values (?)");
        query.addBindValue(faculty->getName());

        if (!query.exec()) {
            last_error_ = query.lastError().text();
            return false;
        }

        query.exec("select last_insert_id() from faculties");
        query.next();
        faculty->setId(query.value(0).toInt());

        faculty->setState(SqlMeta::Selected);
        return true;
    }
    else
    {
        QSqlQuery query;
        query.prepare("insert into faculties(id, name)"
                      "values (?, ?)");
        query.addBindValue(faculty->id());
        query.addBindValue(faculty->getName());

        if (!query.exec()) {
            last_error_ = query.lastError().text();
            return false;
        }

        faculty->setState(SqlMeta::Selected);
        return true;
    }
}

bool DBManager::updateFaculty(Faculty *faculty)
{
    if (!faculty)
        return false;

    QSqlQuery query;
    query.prepare("update faculties set"
                                    "name = ?"
                  "where id = ?");
    query.addBindValue(faculty->getName());
    query.addBindValue(faculty->id());

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    faculty->setState(SqlMeta::Selected);
    return true;
}

bool DBManager::deleteFaculty(int id)
{
    if (id < 0)
        return false;

    Faculty* d_f = util_->facultyById(id);

    for(auto gr: d_f->getGroups()) {
        if (!deleteGroup(gr->id()))
            return false;
    }

    QSqlQuery query;
    query.prepare("delete from faculties where id = ?");
    query.addBindValue(id);
    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    d_f->setState(SqlMeta::Deleted);
    return true;
}

bool DBManager::selectGroupById(int id, Group *group)
{

}

bool DBManager::insertGroup(Group *group)
{
    if (!group && !group->getFaculty())
        return false;

    QSqlQuery query;
    query.prepare("insert into groupss(number, id_faculty)"
                  "values (?, ?)");
    query.addBindValue(group->getName());
    query.addBindValue(group->getFaculty()->id());

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    query.exec("select last_insert_id() from groupss");
    query.next();
    group->setId(query.value(0).toInt());

    group->setState(SqlMeta::Selected);
    return true;
}

bool DBManager::updateGroup(Group *group)
{
    if (!group && !group->getFaculty())
        return false;

    QSqlQuery query;
    query.prepare("update groupss set"
                                    "number = ?"
                                    "id_faculty = ?"
                  "where id = ?");
    query.addBindValue(group->getName());
    query.addBindValue(group->getFaculty()->id());
    query.addBindValue(group->id());

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    group->setState(SqlMeta::Selected);
    return true;
}

bool DBManager::deleteGroup(int id)
{
    if (id < 0)
        return false;

    Group* d_gr = util_->groupById(id);

    for(auto st: d_gr->getStuds()) {
        if (!deleteStudent(st->id()))
            return false;
    }
    for(auto sub: d_gr->getSubs()) {
        if (!deleteSubsGroups(sub->id(), d_gr->id()))
            return false;
    }

    QSqlQuery query;
    query.prepare("delete from groupss where id = ?");
    query.addBindValue(id);
    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    d_gr->setState(SqlMeta::Deleted);
    return true;
}

bool DBManager::insertTeacher(Teacher *teacher)
{
    if (!teacher)
        return false;

    QSqlQuery query;
    query.prepare("insert into teachers(f_name, s_name, pat, email)"
                  "values (?, ?, ?, ?)");
    query.addBindValue(teacher->getFirstName());
    query.addBindValue(teacher->getSecondName());
    query.addBindValue(teacher->getPatName());
    query.addBindValue(teacher->getEmail());

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    query.exec("select last_insert_id() from teachers");
    query.next();
    teacher->setId(query.value(0).toInt());

    if(teacher->getSubs().count() > 0) {
        for (auto subject: teacher->getSubs()) {
            insertSubsTeachers(subject->id(), teacher->id());
        }
    }

    teacher->setState(SqlMeta::Selected);
    return true;
}

bool DBManager::updateTeacher(Teacher *teacher)
{

}

bool DBManager::deleteTeacher(int id)
{
    if (id < 0)
        return false;

    Teacher* teacher = util_->teacherById(id);

    for (auto grade: *grades_) {
        if (grade->getTeacher() == teacher)
            if (!deleteGrade(grade->id()))
                return false;
    }

    for (auto subject: *subjects_) {
        if (subject->getTeachers().contains(teacher))
            if (!deleteSubsTeachers(subject->id(), id))
                return false;
    }

    QSqlQuery query;
    query.prepare("delete from teachers where id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    teacher->setState(SqlMeta::Deleted);
    return true;
}

bool DBManager::insertSubject(Subject *subject)
{
    if (!subject)
        return false;

    QSqlQuery query;
    query.prepare("insert into subjects(name, hours, check_type)"
                  "values (?, ?, ?)");
    query.addBindValue(subject->getName());
    query.addBindValue(subject->getHours());
    query.addBindValue(util_->checkTypeToStrSQL(subject->getCheckType()));

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    query.exec("select last_insert_id() from subjects");
    query.next();
    subject->setId(query.value(0).toInt());

    if(subject->getTeachers().count() > 0) {
        for (auto teacher: subject->getTeachers()) {
            insertSubsTeachers(subject->id(), teacher->id());
        }
    }

    subject->setState(SqlMeta::Selected);
    return true;
}

bool DBManager::updateSubject(Subject *subject)
{

}

bool DBManager::deleteSubject(int id)
{
    if (id < 0)
        return false;

    Subject* subject = util_->subjectById(id);

    for (auto grade: *grades_) {
        if (grade->getSub() == subject)
            if (!deleteGrade(grade->id()))
                return false;
    }

    for (auto group: *groups_) {
        if (group->getSubs().contains(subject))
            if (!deleteSubsGroups(id, group->id()))
                return false;
    }

    for (auto teacher: *teachers_) {
        if (teacher->getSubs().contains(subject))
            if (!deleteSubsTeachers(id, teacher->id()))
                return false;
    }

    QSqlQuery query;
    query.prepare("delete from subjects where id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    subject->setState(SqlMeta::Deleted);
    return true;
}

bool DBManager::insertSubsGroups(int id_sub, int id_gr)
{
    QSqlQuery query;
    query.prepare("insert into subs_groups(id_sub, id_group)"
                  "values (?, ?)");
    query.addBindValue(id_sub);
    query.addBindValue(id_gr);

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

bool DBManager::updateSubsGroups(int id_sub, int id_gr)
{

}

bool DBManager::deleteSubsGroups(int id_sub, int id_gr)
{
    if (id_sub < 0 || id_gr < 0)
        return false;

    QSqlQuery query;
    query.prepare("delete from subs_groups where id_sub = ? and id_group = ?");
    query.addBindValue(id_sub);
    query.addBindValue(id_gr);

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    return true;
}

bool DBManager::insertSubsTeachers(int id_sub, int id_tech)
{
    QSqlQuery query;
    query.prepare("insert into subs_teachers(id_sub, id_teacher)"
                  "values (?, ?)");
    query.addBindValue(id_sub);
    query.addBindValue(id_tech);

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

bool DBManager::updateSubsTeachers(int id_sub, int id_tech)
{

}

bool DBManager::deleteSubsTeachers(int id_sub, int id_tech)
{
    QSqlQuery query;
    query.prepare("delete from subs_teachers where id_sub = ? and id_teacher = ?");
    query.addBindValue(id_sub);
    query.addBindValue(id_tech);

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }
    return true;
}

bool DBManager::selectStudentById(int id, Student *stud)
{
    QSqlQuery query;
    query.prepare("select t.* from students t where id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    while(query.next()) {
        int id_stud = query.value(0).toInt();
        QString s_name = query.value(1).toString();
        QString f_name = query.value(2).toString();
        QString pat = query.value(3).toString();
        QDate date_of_birth = query.value(4).toDate();
        int id_group = query.value(5).toInt();
        QString phone = query.value(6).toString();
        QString email = query.value(7).toString();

        Group* gr = util_->groupById(id_group);

        if (stud && gr) {
            stud->setId(id_stud);
            stud->setSecondName(s_name);
            stud->setFirstName(f_name);
            stud->setPatName(pat);
            stud->setDOB(date_of_birth);
            stud->setGroup(gr);
            stud->setPhone(phone);
            stud->setEmail(email);

            gr->setState(SqlMeta::Selected);
            return true;
        }
    }
    return false;
}

bool DBManager::insertStudent(Student *student)
{
    if (!student && !student->getGroup())
        return false;

    QSqlQuery query;
    query.prepare("insert into students(s_name, f_name, pat, date_of_birth, id_group, phone, email)"
                  "values (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(student->getSecondName());
    query.addBindValue(student->getFirstName());
    query.addBindValue(student->getPatName());
    query.addBindValue(student->getDOB());
    query.addBindValue(student->getGroup()->id());
    query.addBindValue(student->getPhone());
    query.addBindValue(student->getEmail());

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    query.exec("select last_insert_id() from students");
    query.next();
    student->setId(query.value(0).toInt());

    student->setState(SqlMeta::Selected);
    return true;
}

bool DBManager::updateStudent(Student *student)
{
    if (!student && !student->getGroup())
        return false;

    QSqlQuery query;
    query.prepare("update students set"
                                    "s_name = ?"
                                    "f_name = ?"
                                    "pat = ?"
                                    "date_of_birth = ?"
                                    "id_group = ?"
                                    "phone = ?"
                                    "email = ?"
                  "where id = ?");
    query.addBindValue(student->getSecondName());
    query.addBindValue(student->getFirstName());
    query.addBindValue(student->getPatName());
    query.addBindValue(student->getDOB());
    query.addBindValue(student->getGroup()->id());
    query.addBindValue(student->getPhone());
    query.addBindValue(student->getEmail());
    query.addBindValue(student->id());

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    student->setState(SqlMeta::Selected);
    return true;
}

bool DBManager::deleteStudent(int id)
{
    if (id < 0)
        return false;

    Student* d_st = util_->studentById(id);

    for(auto gr: d_st->getGrades()) {
        if (!deleteGrade(gr->id()))
            return false;
    }

    QSqlQuery query;
    query.prepare("delete from students where id = ?");
    query.addBindValue(id);
    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    d_st->setState(SqlMeta::Deleted);
    return true;
}

bool DBManager::selectGradeById(int id, Grade* gr)
{
    QSqlQuery query;
    query.prepare("select t.* from grades t where id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    while(query.next()) {
        int id_grade = query.value(0).toInt();
        QDate date_receive = query.value(2).toDate();
        int mark = query.value(1).toInt();
        int id_sub = query.value(3).toInt();
        int id_stud = query.value(4).toInt();
        int id_teacher = query.value(5).toInt();

        if (gr) {
            gr->setId(id_grade);
            gr->setDateReceive(date_receive);
            gr->setMark(mark);
            gr->setTeacher(util_->teacherById(id_teacher));
            gr->setStud(util_->studentById(id_stud));
            gr->setSub(util_->subjectById(id_sub));

            gr->setState(SqlMeta::Selected);

            return true;
        }

        qDebug() << id_grade << date_receive << mark;
    }
    return false;
}

bool DBManager::insertGrade(Grade *grade)
{
    if (!grade && !grade->getStud() && !grade->getSub() && !grade->getTeacher())
        return false;

    QSqlQuery query;
    query.prepare("insert into grades(date_receive, mark, id_sub, id_stud, id_teacher)"
                  "values (:date, :mark, :id_sub, :id_stud, :id_teacher)");
    query.bindValue(":date", grade->getDateReceive());
    query.bindValue(":mark", grade->getMark());
    query.bindValue(":id_sub", grade->getSub()->id());
    query.bindValue(":id_stud", grade->getStud()->id());
    query.bindValue(":id_teacher", grade->getTeacher()->id());

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    query.exec("select last_insert_id() from grades");
    query.next();
    grade->setId(query.value(0).toInt());

    grade->setState(SqlMeta::Selected);
    return true;
}

bool DBManager::updateGrade(Grade *grade)
{
    if (!grade && !grade->getStud() && !grade->getSub() && !grade->getTeacher())
        return false;

    QSqlQuery query;
    query.prepare("update grades set"
                                    "date_receive = ?"
                                    "mark = ?"
                                    "id_sub = ?"
                                    "id_stud = ?"
                                    "id_teacher = ?"
                  "where id = ?");
    query.addBindValue(grade->getDateReceive().toString());
    query.addBindValue(grade->getMark());
    query.addBindValue(grade->getSub()->id());
    query.addBindValue(grade->getStud()->id());
    query.addBindValue(grade->getTeacher()->id());
    query.addBindValue(grade->id());

    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    grade->setState(SqlMeta::Selected);
    return true;
}

bool DBManager::deleteGrade(int id)
{
    if (id < 0)
        return false;

    QSqlQuery query;
    query.prepare("delete from grades where id = ?");
    query.addBindValue(id);
    if (!query.exec()) {
        last_error_ = query.lastError().text();
        return false;
    }

    util_->gradeById(id)->setState(SqlMeta::Deleted);
    return true;
}

QString DBManager::lastError()
{
    return last_error_;
}

bool DBManager::initFaculties()
{
    QSqlQuery query("SELECT t.* FROM cp_oop.faculties t LIMIT 501");

    while (query.next()) {
        int id_fac = query.value(0).toInt();
        QString name = query.value(1).toString();

        Faculty* fac = new Faculty(id_fac, name, this);
        faculties_->append(fac);
    }

#ifdef QT_DEBUG
    for(auto fac: *faculties_){
        qDebug() << fac->id() << " " << fac->getName();
    }
#endif

    return true;
}

bool DBManager::initGroups()
{
    QSqlQuery query("SELECT t.* FROM cp_oop.groupss t LIMIT 501");

    while(query.next()) {
        int id_group = query.value(0).toInt();
        QString name = query.value(1).toString();
        int id_fac = query.value(2).toInt();

        Faculty* fc = util_->facultyById(id_fac);
        if (!fc)
            continue;
        Group* gr = new Group(id_group, name, fc);
        fc->addNewGroup(gr);
        groups_->append(gr);
    }

#ifdef QT_DEBUG
    for(auto gr: *groups_){
        qDebug() << gr->id() << " " << gr->getName() << gr->getFaculty()->getName();
    }
#endif

    return true;
}

bool DBManager::initTeachers()
{
    QSqlQuery query("SELECT t.* FROM cp_oop.teachers t LIMIT 501");

    while(query.next()) {
        int id_sub = query.value(0).toInt();
        QString f_name = query.value(1).toString();
        QString s_name = query.value(2).toString();
        QString pat = query.value(3).toString();
        QString email = query.value(4).toString();

        Teacher* th = new Teacher(id_sub, s_name, f_name, pat, email);
        teachers_->append(th);
    }

#ifdef QT_DEBUG
    for(auto th: *teachers_){
        qDebug() << th->id() << " " << th->getFullName() << "" << th->getEmail();
    }
#endif

    return true;
}

bool DBManager::initSubjects()
{
    QSqlQuery query("SELECT t.* FROM cp_oop.subjects t LIMIT 501");

    while(query.next()) {
        int id_sub = query.value(0).toInt();
        QString name = query.value(1).toString();
        int hours = query.value(2).toInt();
        Subject::CheckType ch_tp = UtilFaculty::checkTypeFromStr(query.value(3).toString());

        Subject* sub = new Subject(id_sub, name, hours, ch_tp);
        subjects_->append(sub);
    }

#ifdef QT_DEBUG
    for(auto sub: *subjects_){
        qDebug() << sub->id() << " " << sub->getName() << "" << sub->getHours();
    }
#endif

    return true;
}

bool DBManager::initStudents()
{
    QSqlQuery query("SELECT t.* FROM cp_oop.students t LIMIT 501");

    while(query.next()) {
        int id_st = query.value(0).toInt();
        QString f_name = query.value(2).toString();
        QString s_name = query.value(1).toString();
        QString pat = query.value(3).toString();
        QDate dob = query.value(4).toDate();
        int id_gr = query.value(5).toInt();
        QString phone = query.value(6).toString();
        QString email = query.value(7).toString();

        auto gr = util_->groupById(id_gr);
        Student* st = new Student(id_st, s_name, f_name, pat, gr, dob, phone, email);
        gr->addNewStud(st);
        students_->append(st);
    }

#ifdef QT_DEBUG
    for(auto st: *students_){
        qDebug() << st->id() << " " << st->getFullName() << " " << st->getGroup()->getName() <<  " " << st->getDOB() << " " << st->getEmail() << " " << st->getPhone();
    }
#endif

    return true;
}

bool DBManager::initGrades()
{
    QSqlQuery query("SELECT t.* FROM cp_oop.grades t LIMIT 501");

    while(query.next()) {
        int id_grade = query.value(0).toInt();
        QDate date_receive = query.value(1).toDate();
        int mark = query.value(2).toInt();
        int id_sub = query.value(3).toInt();
        int id_stud = query.value(4).toInt();
        int id_teacher = query.value(5).toInt();


        auto st = util_->studentById(id_stud);
        auto sub = util_->subjectById(id_sub);
        auto teacher = util_->teacherById(id_teacher);
        Grade* grade = new Grade(id_grade, date_receive, mark, sub, st, teacher);
        st->addGrade(id_grade, grade);
        grades_->append(grade);
    }

#ifdef QT_DEBUG
    for(auto gr: *grades_){
        qDebug() << gr->id() << " " << gr->getDateReceive() << " " << gr->getStud()->getFullName()
                 <<  " " << gr->getSub()->getName() << " " << gr->getTeacher()->getFullName();
    }
#endif

    return true;
}

bool DBManager::initSubsOfGroups()
{
    QSqlQuery query("SELECT t.* FROM cp_oop.subs_groups t LIMIT 501");

    while(query.next()) {
        int id_sub = query.value(0).toInt();
        int id_gr = query.value(1).toInt();

        auto gr = util_->groupById(id_gr);
        auto sub = util_->subjectById(id_sub);

        if (gr && sub){
            gr->addNewSubject(sub);
        }
    }

#ifdef QT_DEBUG
    for(auto gr: *groups_){
        qDebug() << gr->id() << " " << gr->getName() << "subjects: [";
        QList<Subject*> subs = gr->getSubs();
        for(auto s: subs) {
            qDebug() << s->id() << " " << s->getName();
        }
        qDebug() << "]";
    }
#endif

    return true;
}

bool DBManager::initSubsOfTeachers()
{
    QSqlQuery query("SELECT t.* FROM cp_oop.subs_teachers t LIMIT 501");

    while(query.next()) {
        int id_sub = query.value(0).toInt();
        int id_teacher = query.value(1).toInt();

        auto teacher = util_->teacherById(id_teacher);
        auto sub = util_->subjectById(id_sub);

        if (teacher && sub){
            teacher->addNewSubject(id_sub, sub);
            sub->addNewTeacher(id_teacher, teacher);
        }
    }

#ifdef QT_DEBUG
    for(auto tch: *teachers_){
        qDebug() << tch->id() << " " << tch->getFullName() << "subjects: [";
        QList<Subject*> subs = tch->getSubs();
        for(auto s: subs) {
            qDebug() << s->id() << " " << s->getName();
        }
        qDebug() << "]";
    }

    for(auto s: *subjects_) {
        qDebug() << s->id() << " " << s->getName() << "teachers: [";
        QList<Teacher*> tchs = s->getTeachers();
        for(auto t: tchs) {
            qDebug() << t->id() << " " << t->getFullName();
        }
        qDebug() << "]";
    }
#endif

    return true;
}

