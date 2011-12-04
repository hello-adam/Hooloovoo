
QT += core gui opengl xml phonon

TARGET = Hooloovoo
TEMPLATE = app

LIBS += -lbox2d

OBJECTS_DIR = buildFiles/obj
MOC_DIR = buildFiles/moc
RCC_DIR = buildFiles/rcc
UI_HEADERS_DIR = buildFiles/ui_h

HEADERS += \
    src/gameobject.h \
    src/gamecore.h \
    src/component.h \
    src/componentfactory.h \
    src/graphicsscene.h \
    src/graphicsview.h \
    src/gamefiledialog.h \
    src/gameobjecteditdialog.h \
    src/propertyeditwidget.h \
    src/physicscomponent.h \
    src/physicsmanager.h \
    src/physicscontrollercomponent.h \
    src/componentlistwidget.h \
    src/spawncomponent.h \
    src/animationcomponent.h \
    src/inputcomponent.h \
    src/componenteditwidget.h \
    src/contactlistener.h \
    src/contactcomponent.h \
    src/inputreceiver.h \
    src/editwidgets/pointedit.h \
    src/timercomponent.h \
    src/editwidgets/triggeredit.h \
    src/editwidgets/triggerlineedit.h \
    src/audiocomponent.h \
    src/audiopreviewwidget.h \
    src/leveldatadialog.h \
    src/filemanager.h \
    src/filepreviewwidget.h \
    src/mainwindow.h \
    src/causeeffectmanager.h \
    src/causeandeffecteditwidget.h \
    src/causeeffectlabelwidget.h

SOURCES += \
    src/gameobject.cpp \
    src/gamecore.cpp \
    src/component.cpp \
    src/main.cpp \
    src/componentfactory.cpp \
    src/graphicsscene.cpp \
    src/graphicsview.cpp \
    src/gamefiledialog.cpp \
    src/gameobjecteditdialog.cpp \
    src/propertyeditwidget.cpp \
    src/physicscomponent.cpp \
    src/physicsmanager.cpp \
    src/physicscontrollercomponent.cpp \
    src/componentlistwidget.cpp \
    src/spawncomponent.cpp \
    src/animationcomponent.cpp \
    src/inputcomponent.cpp \
    src/componenteditwidget.cpp \
    src/contactlistener.cpp \
    src/contactcomponent.cpp \
    src/inputreceiver.cpp \
    src/editwidgets/pointedit.cpp \
    src/timercomponent.cpp \
    src/editwidgets/triggeredit.cpp \
    src/editwidgets/triggerlineedit.cpp \
    src/audiocomponent.cpp \
    src/audiopreviewwidget.cpp \
    src/leveldatadialog.cpp \
    src/filemanager.cpp \
    src/filepreviewwidget.cpp \
    src/mainwindow.cpp \
    src/causeeffectmanager.cpp \
    src/causeandeffecteditwidget.cpp \
    src/causeeffectlabelwidget.cpp

FORMS += \
    src/gamefiledialog.ui \
    src/gameobjecteditdialog.ui \
    src/propertyeditwidget.ui \
    src/componenteditwidget.ui \
    src/editwidgets/pointedit.ui \
    src/editwidgets/triggerlineedit.ui \
    src/editwidgets/triggeredit.ui \
    src/audiopreviewwidget.ui \
    src/leveldatadialog.ui \
    src/mainwindow.ui \
    src/causeandeffecteditwidget.ui \
    src/causeeffectlabelwidget.ui

OTHER_FILES +=




























































































