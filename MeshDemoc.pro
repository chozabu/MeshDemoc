!include("../Common/retroshare_plugin.pri"): error("Could not include file ../Common/retroshare_plugin.pri")

CONFIG += qt uic qrc resources
QT     += network xml script
INCLUDEPATH += ../../retroshare-gui/src/retroshare-gui/
INCLUDEPATH += ../../retroshare-gui/src/temp/ui/

greaterThan(QT_MAJOR_VERSION, 4) {
	# Qt 5
	QT     += widgets
}
DEFINES *= NO_SQLCIPHER
SOURCES = topjcdialog.cpp \
    gui/MeshDemocCreatePostDialog.cpp \
    gui/MeshDemocDialog.cpp \
    gui/MeshDemocGroupDialog.cpp \
    gui/MeshDemocItem.cpp \
    gui/MeshDemocListWidget.cpp \
    gui/MeshDemocUserNotify.cpp \
    services/p3meshDemoc.cc \
    serialiser/rsmeshDemocitems.cc \
    MeshDemocPlugin.cpp
HEADERS = topjcdialog.h \
    gui/MeshDemocCreatePostDialog.h \
    gui/MeshDemocDialog.h \
    gui/MeshDemocGroupDialog.h \
    gui/MeshDemocItem.h \
    gui/MeshDemocListWidget.h \
    gui/MeshDemocUserNotify.h \
    interface/rsmeshDemoc.h \
    interface/rsmeshDemocVEG.h \
    services/p3meshDemoc.h \
    serialiser/rsmeshDemocitems.h \
    MeshDemocPlugin.h
FORMS   = \
    topjcdialog.ui \
    gui/MeshDemocCreatePostDialog.ui \
    gui/MeshDemocItem.ui \
    gui/MeshDemocListWidget.ui

TARGET = MeshDemoc

RESOURCES = MeshDemoc_images.qrc lang/MeshDemoc_lang.qrc \
    gui/MeshDemoc_images.qrc

TRANSLATIONS +=  \
                        lang/MeshDemoc_cs.ts \
                        lang/MeshDemoc_da.ts \
                        lang/MeshDemoc_de.ts \
                        lang/MeshDemoc_el.ts \
                        lang/MeshDemoc_en.ts \
                        lang/MeshDemoc_es.ts \
                        lang/MeshDemoc_fi.ts \
                        lang/MeshDemoc_fr.ts \
                        lang/MeshDemoc_hu.ts \
                        lang/MeshDemoc_it.ts \
                        lang/MeshDemoc_ja_JP.ts \
                        lang/MeshDemoc_ko.ts \
                        lang/MeshDemoc_nl.ts \
                        lang/MeshDemoc_pl.ts \
                        lang/MeshDemoc_ru.ts \
                        lang/MeshDemoc_sv.ts \
                        lang/MeshDemoc_tr.ts \
                        lang/MeshDemoc_zh_CN.ts

OTHER_FILES += \
    testit.sh
