#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    util = new UtilFaculty(&faculties, &groups, &teachers, &subjects, &students, &grades);
    db = new DBManager(&faculties, &groups, &teachers, &subjects, &students, &grades, util);

    if (db->connectToDB()) {
        db->initEntities();
    }

    curr_stud = 0;
    fac_gr_model = 0;
    fac_model = 0;
    grade_model = 0;
    group_model = 0;
    stud_filter_model = 0;
    stud_model = 0;
    subs_t4_model = 0;
    teachers_t4_model = 0;
    teach_model = 0;
    sub_model = 0;
    teachers_t5_model = 0;
    subs_t5_model_1 = 0;
    subs_t5_model_2 = 0;

    initModels();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initModels()
{
    stud_model = new StudTableModel(&students);
    stud_filter_model = new StudentFilterModel();
    stud_filter_model->setSourceModel(stud_model);
    connect(ui->chBox_filterByGroup, &QCheckBox::toggled, stud_filter_model, &StudentFilterModel::updateFilterEnable);
    connect(ui->lEdit_find, &QLineEdit::textChanged, stud_filter_model, &StudentFilterModel::updateFilterByName);
    connect(ui->cBox_groups, &QComboBox::currentTextChanged, stud_filter_model, &StudentFilterModel::updateFilterByGroup);

    fac_model = new FacultyListModel(&faculties);

    fac_gr_model = new FacultyGroupModel();
    Node* root = UtilFaculty::parserForFacultyAndGrops(faculties);
    fac_gr_model->setRootNode(root);

    group_model = new GroupListModel(&groups);

    teach_model = new TeacherListModel(teachers);

    sub_model = new SubjectListModel(subjects);

    //tab1
    ui->trView_tb1->setModel(fac_gr_model);
    ui->trView_tb1->expandAll();
    ui->cBox_faculties->setModel(fac_model);

    //tab2
    ui->tbView_students->setModel(stud_filter_model);
    ui->cBox_groups->setModel(group_model);
    ui->cBox_groupsW->setModel(group_model);

    //tab3
    check_types << "Зачет" << "Экзамен" << "Курсовой проект";
    ui->cBox_check_type->addItems(check_types);
    ui->lView_teachers->setModel(teach_model);
    ui->lView_subjects->setModel(sub_model);

    //tab4
    tab4_init();

    //tab5
    ui->cBox_group_tab5->setModel(group_model);
    ui->cBox_subs_tab5_1->setModel(sub_model);
    ui->cBox_teacher_tab5->setModel(teach_model);
    ui->cBox_subs_tab5_2->setModel(sub_model);
}


void MainWindow::on_but_deleteFaculty_clicked()
{
    QModelIndex index = ui->trView_tb1->currentIndex();
    qDebug() << "[TAB1][but_deleteFaculty] Current count of faculties: " << faculties.count();
    qDebug() << "[TAB1][but_deleteFaculty] Trying to delete the faculty index(" << index.row() << ";" << index.column() << ")";

    auto replay = QMessageBox::question(this, tr("Подтверждение действия."), tr("Удаление кафедры также приведет к удалению всех её групп\nПродолжить?"));

    if(replay == QMessageBox::Yes){
        if(index.isValid() && !index.parent().isValid()) {
            Faculty* d_f = faculties.at(index.row());
            if (db->deleteFaculty(d_f->id())) {


                util->deleteFaculty(d_f);

                //updating model class
                Node* root = UtilFaculty::parserForFacultyAndGrops(faculties);
                fac_gr_model->setRootNode(root);
                ui->trView_tb1->expandAll();
                qDebug() << "[TAB1][but_deleteFaculty] Faculty deleted";
                qDebug() << "[TAB1][but_deleteFaculty] TreeModel updated";

                fac_model->refreshData(&faculties);
                stud_model->refreshData(&students);
            }
            else {
                QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
            }
        }
    }
    qDebug() << "[TAB1][but_deleteFaculty] Count of faculties after deleting: " << faculties.count();
    qDebug() << "[TAB1] [but_deleteFaculty] FacultyListModel updated";
}


void MainWindow::on_but_addFaculty_clicked()
{
    int id_fac = ui->lEdit_facultyId->text().toInt();
    QString fac_name = ui->lEdit_facultyName->text();

    ui->lEdit_facultyId->clear();
    ui->lEdit_facultyName->clear();

    auto already_exist = false;

    for(auto& fac : faculties){
        if (fac->id() == id_fac) {
            already_exist = true;
            break;
        }
    }

    if (already_exist) {
        QMessageBox::warning(this, "Некорректный ввод", "Кафедра с таким номером уже существует!");
        return;
    }

    Faculty* n_fac = new Faculty(-1, fac_name);
    n_fac->setState(SqlMeta::SqlState::Added);

    qDebug() << "[TAB1][but_addFaculty] Trying to add facaulty: " << n_fac->id() << " " << n_fac->getName();

    //sql query and checks for issues
    //db.insert()
    if (db->insertFaculty(n_fac)) {
        faculties.append(n_fac);
        qDebug() << "[TAB1][but_addFaculty] Facaulty: |" << n_fac->id() << " " << n_fac->getName() << " | added";

        //updating model class
        Node* root = UtilFaculty::parserForFacultyAndGrops(faculties);
        fac_gr_model->setRootNode(root);
        ui->trView_tb1->expandAll();
        qDebug() << "[TAB1][but_addFaculty] TreeModel updated";

        qDebug() << "[TAB1] [but_addFaculty] FacultyListModel updated";
        fac_model->refreshData(&faculties);
        stud_model->refreshData(&students);
    }
    else {
        QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
        delete n_fac;
    }
}


void MainWindow::on_but_addGroup_clicked()
{
    QString name_gr = ui->lEdit_group_name->text();
    int index_fac = ui->cBox_faculties->currentIndex();
    Faculty* fac = 0;

    ui->lEdit_group_name->clear();
    ui->cBox_faculties->setCurrentIndex(0);

    if (UtilFaculty::indexIsValid(index_fac, faculties)){
        fac = faculties.at(index_fac);
        qDebug() << "[TAB1][but_addGroup] Trying to add group to faculty:" << fac->id() << " " << fac->getName();
    }



    if (fac){
        Group* n_gr = new Group(-1, name_gr, fac);

        qDebug() << "[TAB1][but_addGroup] Trying to add group: " << n_gr->id() << " " << n_gr->getName();

        n_gr->setState(SqlMeta::SqlState::Added);

        //sql query and checks for issues
        //db.insert()
        if (db->insertGroup(n_gr)) {
            fac->addNewGroup(n_gr);
            groups.append(n_gr);
            qDebug() << "[TAB1] [but_addGroup] Facaulty: |" << n_gr->id() << " " << n_gr->getName() << " | added";

            //updating model class
            Node* root = UtilFaculty::parserForFacultyAndGrops(faculties);
            fac_gr_model->setRootNode(root);
            ui->trView_tb1->expandAll();
            qDebug() << "[TAB1] [but_addGroup] TreeModel updated";

            qDebug() << "[TAB1] [but_addGroup] GroupsListModel updated";
            group_model->refreshData(&groups);
            stud_model->refreshData(&students);
        }
        else {
            QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
            delete n_gr;
        }
    }
}


void MainWindow::on_but_deleteGroup_clicked()
{
    QModelIndex index = ui->trView_tb1->currentIndex();
    qDebug() << "[TAB1][but_deleteGroup] Current count of groups: " << groups.count();
    qDebug() << "[TAB1][but_deleteGroup] Trying to delete the group index(" << index.row() << ";" << index.column() << ")";

    auto replay = QMessageBox::question(this, tr("Подтверждение действия."), tr("Удаление группы также приведет к удалению всех её студентов\nПродолжить?"));

    if (replay == QMessageBox::Yes) {
        if(index.isValid() && index.parent().isValid()) {
            int index_fac = index.parent().row();
            Faculty* fac = faculties.at(index_fac);
            Group* gr = fac->getGroups().at(index.row());

            //sql query and checks for issues
            //db.insert()
            if (db->deleteGroup(gr->id())) {
                fac->deleteGroup(gr->id());
                //delete all students of group
                //delete Group* !!!!!!!!!!!!!!!!

                util->deleteGroup(gr);

                qDebug() << "[TAB1][but_deleteGroup] Group deleted";
                //updating model class
                qDebug() << "[TAB1] [but_addGroup] TreeModel updated";
                Node* root = UtilFaculty::parserForFacultyAndGrops(faculties);
                fac_gr_model->setRootNode(root);
                ui->trView_tb1->expandAll();

                qDebug() << "[TAB1] [but_addGroup] GroupsListModel updated";
                group_model->refreshData(&groups);
                stud_model->refreshData(&students);
            }
            else {
                QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
            }
        }
    }
    qDebug() << "[TAB1][but_deleteGroup] Count of groups after deleting: " << groups.count();
}


void MainWindow::on_cBox_faculties_currentIndexChanged(int index)
{
    qDebug() << "[TAB1][cBox_faculties] index: " << index;
}


void MainWindow::on_but_addStud_clicked()
{
    int index_gr = ui->cBox_groupsW->currentIndex();
    QString full_name = ui->lEdit_fullName->text();
    QDate dob = ui->dEdit_dob->date();
    QString phone = ui->lEdit_phone->text();
    QString email = ui->lEdit_email->text();

    ui->cBox_groupsW->setCurrentIndex(0);
    ui->lEdit_fullName->clear();
    ui->dEdit_dob->clear();
    ui->lEdit_phone->clear();
    ui->lEdit_email->clear();

    Group* gr = 0;

    //If full_name not consists of f_name, s_name, patronym, will be throw invalid index exception
    auto s_f_t_names = full_name.split(" ");

    if (s_f_t_names.count() != 3) {
        QMessageBox::warning(this, tr("Некорректный ввод"), tr("Некорректные ФИО.\nФИО должно быть указано через пробелы."));
        return;
    }

    if(UtilFaculty::indexIsValid(index_gr, groups)) {
        gr = groups.at(index_gr);
        qDebug() << "[TAB1][but_addGroup] Trying to add student to group:" << gr->id() << " " << gr->getName();
    }

    if (gr) {
        Student* n_stud = new Student(-1, s_f_t_names.at(0), s_f_t_names.at(1), s_f_t_names.at(2), gr, dob, phone, email);
        qDebug() << "[TAB1][but_addStud] Trying to add student: " << n_stud->id() << " " << n_stud->getFullName();
        n_stud->setState(SqlMeta::SqlState::Added);

        if (db->insertStudent(n_stud)) {
            gr->addNewStud(n_stud);
            students.append(n_stud);

            qDebug() << "[TAB1][but_addStud] Student: |" << n_stud->id() << " " << n_stud->getFullName() << " | added";
            qDebug() << "[TAB1][but_addGroup] StudentTableModel updated";
            stud_model->refreshData(&students);
            return;
        }
        else {
            QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
            delete n_stud;
        }
    }
    qDebug() << "[TAB1][but_addStud] Failed to add student";
}


void MainWindow::on_but_deleteStud_clicked()
{
    QModelIndex index = ui->tbView_students->currentIndex();
    qDebug() << "[TAB2][but_deleteStud] Current count of students: " << groups.count();
    qDebug() << "[TAB2][but_deleteStud] Trying to delete the students index(" << index.row() << ";" << index.column() << ")";

    if(index.isValid()) {
        int id_stud = stud_filter_model->index(index.row(), 0).data().value<int>();
        Student* d_stud = util->studentById(id_stud);
        Group* gr = d_stud->getGroup();

        auto replay = QMessageBox::question(this, tr("Подтверждение действия."), tr("Удаление студента также приведет к удалению всех его оценок\nПродолжить?"));

        if (replay == QMessageBox::Yes){
            //sql query and checks for issues
            if (db->deleteStudent(d_stud->id())) {
                //delete all grades of student
                //delete Student* !!!!!!!!!!!!!!!!
                gr->deleteStud(d_stud->id());
                util->deleteStudent(d_stud);

                qDebug() << "[TAB2][but_deleteStud] Student deleted";
                qDebug() << "[TAB1][but_addGroup] StudentTableModel updated";
                stud_model->refreshData(&students);

                curr_stud = 0;
                tab4_init();
            }
            else {
                QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
                for(auto grade: d_stud->getGrades()){
                    if(grade->state() == SqlMeta::Deleted) {
                        db->insertGrade(grade);
                    }
                }
            }
        }
    }
    qDebug() << "[TAB2][but_deleteStud] Count of students after deleting: " << groups.count();
}


void MainWindow::tab4_init()
{
    if (curr_stud) {
        ui->lEdit_fullName_tab4->setText(curr_stud->getFullName());
        ui->lEdit_group_tab4->setText(curr_stud->getGroup()->getName());
        ui->dEdit_dob_tab4->setDate(curr_stud->getDOB());
        ui->lEdit_phone_tab4->setText(curr_stud->getPhone());
        ui->lEdit_email_tab4->setText(curr_stud->getEmail());

        if (grade_model && subs_t4_model && teachers_t4_model){
            delete grade_model; grade_model = 0;
            delete subs_t4_model; subs_t4_model = 0;
            delete teachers_t4_model; teachers_t4_model = 0;
        }

        grade_model = new GradeTableModel(curr_stud->getGrades());
        subs_t4_model = new SubjectListModel(curr_stud->getGroup()->getSubs());

        ui->tbView_grades->setModel(grade_model);
        ui->cBox_subject_tab4->setModel(subs_t4_model);
    } else {
        ui->lEdit_fullName_tab4->clear();
        ui->lEdit_group_tab4->clear();
        ui->dEdit_dob_tab4->clear();
        ui->lEdit_phone_tab4->clear();
        ui->lEdit_email_tab4->clear();

        if (grade_model && subs_t4_model && teachers_t4_model){
            delete grade_model; grade_model = 0;
            delete subs_t4_model; subs_t4_model = 0;
            delete teachers_t4_model; teachers_t4_model = 0;
        }

    }
}


void MainWindow::on_tbView_students_doubleClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        int id_stud = stud_filter_model->index(index.row(), 0).data().value<int>();
        curr_stud = util->studentById(id_stud);

        tab4_init();

        ui->tabWidget->setCurrentIndex(3);
    }
}


void MainWindow::on_cBox_subject_tab4_currentIndexChanged(int index)
{
    if (teachers_t4_model) {
        delete teachers_t4_model; teachers_t4_model = 0;
    }

    teachers_t4_model = new TeacherListModel(subs_t4_model->getSource().at(index)->getTeachers());
    ui->cBox_teacher_tab4->setModel(teachers_t4_model);
}


void MainWindow::on_but_addGrade_clicked()
{
    if (curr_stud) {
        int mark = ui->sBox_mark_tab4->value();
        QDate date = QDate::currentDate();
        int i_sub = ui->cBox_subject_tab4->currentIndex();
        int i_teacher = ui->cBox_teacher_tab4->currentIndex();

        ui->sBox_mark_tab4->clear();
        ui->cBox_subject_tab4->setCurrentIndex(0);
        ui->cBox_teacher_tab4->setCurrentIndex(0);

        SubjectListModel* cur_sub_model = static_cast<SubjectListModel*>(ui->cBox_subject_tab4->model());
        TeacherListModel* cur_teacher_model = static_cast<TeacherListModel*>(ui->cBox_teacher_tab4->model());

        if (!UtilFaculty::indexIsValid(i_sub, cur_sub_model->getSource()) || !UtilFaculty::indexIsValid(i_teacher, cur_teacher_model->getSource())) {
            QMessageBox::warning(this, tr("Неккоретный ввод"), tr("Настроите план."));
            return;
        }

        Subject* sub = cur_sub_model->getSource().at(i_sub);
        Teacher* teach = cur_teacher_model->getSource().at(i_teacher);

        qDebug() << "[TAB4][but_addGrade] Trying to add grade to student:" << curr_stud->id() << " " << curr_stud->getFullName();

        Grade* n_gr = new Grade(-1, date, mark, sub, curr_stud, teach);

        qDebug() << "[TAB1][but_addGrade] Trying to add grade: " << n_gr->getMark() << " " << n_gr->getSub()->getName();

        n_gr->setState(SqlMeta::SqlState::Added);

        //sql query and checks for issues
        if (db->insertGrade(n_gr)) {
            curr_stud->addGrade(n_gr->id(), n_gr);
            grades.append(n_gr);
            qDebug() << "[TAB4][but_addGrade] Grade: |" << n_gr->getMark() << " " << n_gr->getSub()->getName() << " | added";


            qDebug() << "[TAB4][but_addGrade] GradeTableModel updated";
            grade_model->refreshData(curr_stud->getGrades());
        }
        else {
            qDebug() << "[TAB4][but_addGrade] Failed to add grade";
            qDebug() << "[TAB4][but_addGrade] Deleting new pointer";
            QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
            delete n_gr;
        }
    }
}


void MainWindow::on_but_deleteGrade_clicked()
{
    if(curr_stud) {
        QModelIndex index = ui->tbView_grades->currentIndex();

        if(index.isValid()){
            Grade* d_gr = grade_model->getSource().at(index.row());
            qDebug() << "[TAB4][but_deleteGrade] Trying to delete grade:" << d_gr->id();

            if (db->deleteGrade(d_gr->id())){
                curr_stud->deleteGrade(d_gr->id());
                util->deleteGrade(d_gr);

                qDebug() << "[TAB4][but_deleteGrade] Succsessful deleted grade";
                qDebug() << "[TAB4][but_deleteGrade] GradeTableModel updated";
                grade_model->refreshData(curr_stud->getGrades());
                return;
            }
        }
    }
    qDebug() << "[TAB4][but_deleteGrade] Failed to delete grade";
}


void MainWindow::on_but_addSubject_clicked()
{
    QString name = ui->lEdit_name_tab3->text();
    int hours = ui->sBox_hours_tab3->value();
    Subject::CheckType check = (Subject::CheckType)ui->cBox_check_type->currentIndex();

    ui->lEdit_name_tab3->clear();
    ui->sBox_hours_tab3->clear();
    ui->cBox_check_type->setCurrentIndex(0);

    Subject* subject = new Subject(-1, name, hours, check);
    subject->setState(SqlMeta::Added);

    if(db->insertSubject(subject)) {
        subjects.append(subject);

        sub_model->refreshData(subjects);
    }
    else {
        QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
        delete subject;
    }
}


void MainWindow::on_but_deleteSubject_clicked()
{
    QModelIndex index = ui->lView_subjects->currentIndex();
    qDebug() << "[TAB3][but_deleteSubject] Current count of subjects: " << subjects.count();
    qDebug() << "[TAB3][but_deleteSubject] Trying to delete the subject index(" << index.row() << ";" << index.column() << ")";

    auto replay = QMessageBox::question(this, tr("Подтверждение действия."), tr("Удаление дисциплины также приведет к удалению всех оценок по данной дисциплине\nПродолжить?"));

    if (replay == QMessageBox::Yes) {
        if (index.isValid()) {
            int i = index.row();

            if (UtilFaculty::indexIsValid(i, subjects)) {
                Subject* subject = subjects.at(i);

                if (db->deleteSubject(subject->id())) {
                    for (auto grade : grades) {
                        if (grade->state() == SqlMeta::Deleted) {
                            Student* stud = grade->getStud();
                            stud->deleteGrade(grade->id());

                            util->deleteGrade(grade);
                        }
                    }

                    for (auto group : groups) {
                        group->deleteSubject(subject->id());
                    }

                    for (auto teacher : teachers) {
                        teacher->deleteSubject(subject->id());
                    }

                    util->deleteSubject(subject);

                    group_model->refreshData(&groups);
                    sub_model->refreshData(subjects);
                    teach_model->refreshData(teachers);
                    tab4_init();
                }
                else
                {
                    QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
                    //restore deleted grades if there are exists
                    //TODO
                }
            }
        }
    }
}


void MainWindow::on_but_addTeacher_clicked()
{
    QString full_name = ui->lEdit_fullName_tab3->text();
    QString email = ui->lEdit_email_tab3->text();

    ui->lEdit_fullName_tab3->clear();
    ui->lEdit_email_tab3->clear();

    auto s_f_p_names = full_name.split(" ");

    if (s_f_p_names.count() != 3) {
        QMessageBox::warning(this, tr("Некорректный ввод"), tr("Некорректные ФИО.\nФИО должно быть указано через пробелы."));
        return;
    }

    Teacher* teacher = new Teacher(-1, s_f_p_names.at(0), s_f_p_names.at(1), s_f_p_names.at(2), email);
    teacher->setState(SqlMeta::Added);

    if(db->insertTeacher(teacher)) {
        teachers.append(teacher);

        teach_model->refreshData(teachers);
    }
    else {
        QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
        delete teacher;
    }
}


void MainWindow::on_but_deleteTeacher_clicked()
{
    QModelIndex index = ui->lView_teachers->currentIndex();
    qDebug() << "[TAB3][deleteTeacher] Current count of teachers: " << teachers.count();
    qDebug() << "[TAB3][deleteTeacher] Trying to delete the teacher index(" << index.row() << ";" << index.column() << ")";


    auto replay = QMessageBox::question(this, tr("Подтверждение действия"), tr("Удаление преподавателя также приведет к удалению всех оценок, которые за ним закреплены\nПродолжить?"));

    if (replay == QMessageBox::Yes) {
        if (index.isValid()) {
            int i = index.row();

            if (UtilFaculty::indexIsValid(i, teachers)) {
                Teacher* teacher = teachers.at(i);

                if (db->deleteTeacher(teacher->id())) {
                    for (auto grade : grades) {
                        if (grade->state() == SqlMeta::Deleted) {
                            Student* stud = grade->getStud();
                            stud->deleteGrade(grade->id());

                            util->deleteGrade(grade);
                        }
                    }

                    for (auto subject : subjects) {
                        subject->deleteTeacher(teacher->id());
                    }

                    util->deleteTeacher(teacher);

                    group_model->refreshData(&groups);
                    sub_model->refreshData(subjects);
                    teach_model->refreshData(teachers);
                    tab4_init();
                }
                else
                {
                    QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
                    //restore deleted grades if there are exists
                    //TODO
                }
            }
        }
    }
}


void MainWindow::on_cBox_group_tab5_currentIndexChanged(int index)
{
    if (subs_t5_model_1) {
        delete subs_t5_model_1; subs_t5_model_1 = 0;
    }

    subs_t5_model_1 = new SubjectListModel(groups.at(index)->getSubs());
    ui->lView_subs_of_group->setModel(subs_t5_model_1);
}


void MainWindow::on_cBox_subs_tab5_2_currentIndexChanged(int index)
{
    if (teachers_t5_model) {
        delete teachers_t5_model; teachers_t5_model = 0;
    }

    teachers_t5_model = new TeacherListModel(subjects.at(index)->getTeachers());
    ui->lView_teachers_of_sub->setModel(teachers_t5_model);
}


void MainWindow::on_cBox_teacher_tab5_currentIndexChanged(int index)
{
    if (subs_t5_model_2) {
        delete subs_t5_model_2; subs_t5_model_2 = 0;
    }

    subs_t5_model_2 = new SubjectListModel(teachers.at(index)->getSubs());
    ui->lView_subs_of_teacher->setModel(subs_t5_model_2);
}


void MainWindow::on_but_attachSubToGroup_clicked()
{
    int index_group = ui->cBox_group_tab5->currentIndex();
    int index_sub = ui->cBox_subs_tab5_1->currentIndex();

    if (UtilFaculty::indexIsValid(index_group, groups) && UtilFaculty::indexIsValid(index_sub, subjects)) {
        Group* group = groups.at(index_group);
        Subject* sub = subjects.at(index_sub);

        if (group->getSubs().contains(sub)) {
            QMessageBox::warning(this, tr("Невозможная операция"), QString("Дисциплина уже находится в плане группы %1!").arg(group->getName()));
            return;
        }

        if (db->insertSubsGroups(sub->id(), group->id())) {
            group->addNewSubject(sub);

            subs_t5_model_1->refreshData(group->getSubs());
        }
        else {
            QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
        }
    }
}


void MainWindow::on_but_deattachSubToGroup_clicked()
{
    int index_group = ui->cBox_group_tab5->currentIndex();
    int index_sub = ui->lView_subs_of_group->currentIndex().row();

    Group* group = groups.at(index_group);

    if (UtilFaculty::indexIsValid(index_sub, group->getSubs())) {
        Subject* sub = group->getSubs().at(index_sub);

        if (db->deleteSubsGroups(sub->id(), group->id())) {
            group->deleteSubject(sub->id());

            subs_t5_model_1->refreshData(group->getSubs());
        }
        else {
            QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
        }
    }
    else {
        QMessageBox::information(this, tr("Некорректное действие"), tr("Не выбрана дисциплина"));
        return;
    }
}


void MainWindow::on_but_attachSubAndTeacher_clicked()
{
    int index_sub = ui->cBox_subs_tab5_2->currentIndex();
    int index_teacher = ui->cBox_teacher_tab5->currentIndex();

    if (UtilFaculty::indexIsValid(index_teacher, teachers) && UtilFaculty::indexIsValid(index_sub, subjects)) {
        Teacher* teacher = teachers.at(index_teacher);
        Subject* sub = subjects.at(index_sub);

        if (teacher->getSubs().contains(sub) || sub->getTeachers().contains(teacher)) {
            QMessageBox::warning(this, tr("Невозможная операция"), tr("Преподаватель и дисциплина уже привязаны!"));
            return;
        }

        if (db->insertSubsTeachers(sub->id(), teacher->id())) {
            sub->addNewTeacher(teacher->id(), teacher);
            teacher->addNewSubject(sub->id(), sub);

            teachers_t5_model->refreshData(sub->getTeachers());
            subs_t5_model_2->refreshData(teacher->getSubs());
        }
        else {
            QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
        }
    }
}


void MainWindow::on_but_deattachTeacherFromSub_clicked()
{
    int index_sub = ui->cBox_subs_tab5_2->currentIndex();
    int index_teacher = ui->lView_teachers_of_sub->currentIndex().row();

    Subject* sub = subjects.at(index_sub);

    if (UtilFaculty::indexIsValid(index_teacher, sub->getTeachers())) {
        Teacher* teacher = sub->getTeachers().at(index_teacher);

        if (db->deleteSubsTeachers(sub->id(), teacher->id())) {
            sub->deleteTeacher(teacher->id());
            teacher->deleteSubject(sub->id());

            teachers_t5_model->refreshData(sub->getTeachers());
            subs_t5_model_2->refreshData(teacher->getSubs());
        }
        else {
            QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
        }
    }
    else {
        QMessageBox::information(this, tr("Некорректное действие"), tr("Не выбран преподаватель"));
        return;
    }
}


void MainWindow::on_but_deattachSubFromTeacher_clicked()
{
    int index_sub = ui->lView_subs_of_teacher->currentIndex().row();
    int index_teacher = ui->cBox_teacher_tab5->currentIndex();

    Teacher* teacher = teachers.at(index_teacher);

    if (UtilFaculty::indexIsValid(index_sub, teacher->getSubs())) {
        Subject* sub = teacher->getSubs().at(index_sub);

        if (db->deleteSubsTeachers(sub->id(), teacher->id())) {
            sub->deleteTeacher(teacher->id());
            teacher->deleteSubject(sub->id());

            teachers_t5_model->refreshData(sub->getTeachers());
            subs_t5_model_2->refreshData(teacher->getSubs());
        }
        else {
            QMessageBox::warning(this, tr("Ошибка базы данных"), db->lastError());
        }
    }
    else {
        QMessageBox::information(this, tr("Некорректное действие"), tr("Не выбрана дисциплина"));
        return;
    }
}

