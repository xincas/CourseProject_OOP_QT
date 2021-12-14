QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dbmanager.cpp \
    entities/faculty.cpp \
    entities/grade.cpp \
    entities/teacher.cpp \
    models/listModels/grouplistmodel.cpp \
    models/listModels/subjectlistmodel.cpp \
    models/listModels/teacherlistmodel.cpp \
    models/tableModels/gradetablemodel.cpp \
    models/tableModels/studentfiltermodel.cpp \
    models/treeModels/facultygroupmodel.cpp \
    models/listModels/facultylistmodel.cpp \
    entities/group.cpp \
    main.cpp \
    mainwindow.cpp \
    entities/person.cpp \
    entities/student.cpp \
    entities/subject.cpp \
    models/treeModels/node.cpp \
    models/tableModels/studtablemodel.cpp \
    utils/utilfaculty.cpp

HEADERS += \
    dbmanager.h \
    entities/faculty.h \
    entities/grade.h \
    entities/teacher.h \
    models/listModels/grouplistmodel.h \
    models/listModels/subjectlistmodel.h \
    models/listModels/teacherlistmodel.h \
    models/tableModels/gradetablemodel.h \
    models/tableModels/studentfiltermodel.h \
    models/treeModels/facultygroupmodel.h \
    models/listModels/facultylistmodel.h \
    entities/group.h \
    mainwindow.h \
    entities/person.h \
    models/treeModels/node.h \
    models/tableModels/studtablemodel.h \
    utils/sqlmeta.h \
    entities/student.h \
    entities/subject.h \
    utils/utilfaculty.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/'../../../../../../Program Files/MySQL/MySQL Server 8.0/lib/' -llibmysql

INCLUDEPATH += $$PWD/'../../../../../../Program Files/MySQL/MySQL Server 8.0/include'
DEPENDPATH += $$PWD/'../../../../../../Program Files/MySQL/MySQL Server 8.0/include'

RESOURCES += \
    ../dark-theme/style.qrc
