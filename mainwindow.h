#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "dbmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_but_deleteFaculty_clicked();

    void on_cBox_faculties_currentIndexChanged(int index);

    void on_but_addFaculty_clicked();

    void on_but_addGroup_clicked();

    void on_but_deleteGroup_clicked();

    void on_but_addStud_clicked();

    void on_but_deleteStud_clicked();

    void on_tbView_students_doubleClicked(const QModelIndex &index);

    void on_cBox_subject_tab4_currentIndexChanged(int index);

    void on_but_addGrade_clicked();

    void on_but_deleteGrade_clicked();

    void on_but_addSubject_clicked();

    void on_but_deleteSubject_clicked();

    void on_but_addTeacher_clicked();

    void on_but_deleteTeacher_clicked();

    void on_cBox_group_tab5_currentIndexChanged(int index);

    void on_cBox_subs_tab5_2_currentIndexChanged(int index);

    void on_cBox_teacher_tab5_currentIndexChanged(int index);

    void on_but_attachSubToGroup_clicked();

    void on_but_deattachSubToGroup_clicked();

    void on_but_attachSubAndTeacher_clicked();

    void on_but_deattachTeacherFromSub_clicked();

    void on_but_deattachSubFromTeacher_clicked();

private:
    Ui::MainWindow *ui;
    void initModels();

    DBManager* db;
    UtilFaculty* util;

    QList<Faculty*> faculties;
    FacultyListModel* fac_model;
    FacultyGroupModel* fac_gr_model;

    QList<Group*> groups;
    GroupListModel* group_model;

    QList<Teacher*> teachers;
    TeacherListModel* teach_model;

    QList<Subject*> subjects;
    SubjectListModel* sub_model;
    QList<QString> check_types;

    QList<Student*> students;
    StudentFilterModel* stud_filter_model;
    StudTableModel* stud_model;

    QList<Grade*> grades;

    //tab4 models
    Student* curr_stud;
    GradeTableModel* grade_model;
    SubjectListModel* subs_t4_model;
    TeacherListModel* teachers_t4_model;

    //tab5 models
    SubjectListModel* subs_t5_model_1;
    SubjectListModel* subs_t5_model_2;
    TeacherListModel* teachers_t5_model;

    void tab4_init();
};
#endif // MAINWINDOW_H
