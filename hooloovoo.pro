
QT += core gui opengl xml phonon

TARGET = Hooloovoo
TEMPLATE = app

LIBS += -lbox2d

OBJECTS_DIR = buildFiles/obj
MOC_DIR = buildFiles/moc
RCC_DIR = buildFiles/rcc
UI_HEADERS_DIR = buildFiles/ui_h

INCLUDEPATH += ./src/ \
                ./src/editwidgets/ \
                ./src/components/


#RC_FILE += resources/appicon.rc

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
    src/causeeffectlabelwidget.h \
    src/picturepreviewwidget.h \
    src/commandcomponent.h \
    src/componentalteration.h \
    src/levelmanager.h \
    src/gameobjectmodel.h \
    src/property.h \
    src/propertydelegate.h \
    src/editwidgets/editwidget.h \
    src/editwidgets/stringedit.h \
    src/propertytreeview.h \
    src/editwidgets/doubleedit.h \
    src/editwidgets/coloredit.h \
    src/editwidgets/integeredit.h \
    src/editwidgets/booleanedit.h \
    src/editwidgets/stringlistedit.h \
    src/editwidgets/enumedit.h \
    src/gameobjecteditwidget.h \
    src/gamefilemodel.h \
    src/gameobjectfilewidget.h \
    src/editwidgets/alterationedit.h \
    src/aboutdialog.h \
    src/components/timercomponent.h \
    src/components/spawncomponent.h \
    src/components/physicscontrollercomponent.h \
    src/components/physicscomponent.h \
    src/components/inputcomponent.h \
    src/components/contactlistener.h \
    src/components/contactcomponent.h \
    src/components/componentfactory.h \
    src/components/component.h \
    src/components/commandcomponent.h \
    src/components/audiocomponent.h \
    src/components/animationcomponent.h

SOURCES += \
    src/gameobject.cpp \
    src/gamecore.cpp \
    src/main.cpp \
    src/graphicsscene.cpp \
    src/graphicsview.cpp \
    src/gamefiledialog.cpp \
    src/gameobjecteditdialog.cpp \
    src/propertyeditwidget.cpp \
    src/physicsmanager.cpp \
    src/componentlistwidget.cpp \
    src/componenteditwidget.cpp \
    src/inputreceiver.cpp \
    src/editwidgets/pointedit.cpp \
    src/editwidgets/triggeredit.cpp \
    src/editwidgets/triggerlineedit.cpp \
    src/audiopreviewwidget.cpp \
    src/leveldatadialog.cpp \
    src/filemanager.cpp \
    src/filepreviewwidget.cpp \
    src/mainwindow.cpp \
    src/causeeffectmanager.cpp \
    src/causeandeffecteditwidget.cpp \
    src/causeeffectlabelwidget.cpp \
    src/picturepreviewwidget.cpp \
    src/componentalteration.cpp \
    src/levelmanager.cpp \
    src/gameobjectmodel.cpp \
    src/property.cpp \
    src/propertydelegate.cpp \
    src/editwidgets/editwidget.cpp \
    src/editwidgets/stringedit.cpp \
    src/propertytreeview.cpp \
    src/editwidgets/doubleedit.cpp \
    src/editwidgets/coloredit.cpp \
    src/editwidgets/integeredit.cpp \
    src/editwidgets/booleanedit.cpp \
    src/editwidgets/stringlistedit.cpp \
    src/editwidgets/enumedit.cpp \
    src/gameobjecteditwidget.cpp \
    src/gamefilemodel.cpp \
    src/gameobjectfilewidget.cpp \
    src/editwidgets/alterationedit.cpp \
    src/aboutdialog.cpp \
    src/components/timercomponent.cpp \
    src/components/spawncomponent.cpp \
    src/components/physicscontrollercomponent.cpp \
    src/components/physicscomponent.cpp \
    src/components/inputcomponent.cpp \
    src/components/contactlistener.cpp \
    src/components/contactcomponent.cpp \
    src/components/componentfactory.cpp \
    src/components/component.cpp \
    src/components/commandcomponent.cpp \
    src/components/audiocomponent.cpp \
    src/components/animationcomponent.cpp

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
    src/causeeffectlabelwidget.ui \
    src/picturepreviewwidget.ui \
    src/levelmanager.ui \
    src/editwidgets/stringedit.ui \
    src/editwidgets/doubleedit.ui \
    src/editwidgets/coloredit.ui \
    src/editwidgets/integeredit.ui \
    src/editwidgets/booleanedit.ui \
    src/editwidgets/stringlistedit.ui \
    src/editwidgets/enumedit.ui \
    src/gameobjecteditwidget.ui \
    src/gameobjectfilewidget.ui \
    src/editwidgets/alterationedit.ui \
    src/aboutdialog.ui

OTHER_FILES += \

RESOURCES += \
    resources/Icons.qrc \
#    resources/appicon.rc




































































































































































