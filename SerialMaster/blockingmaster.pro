QT += widgets serialport
requires(qtConfig(combobox))

TARGET = blockingmaster
TEMPLATE = app

HEADERS += \
    dialog.h \
    masterthread.h \
    settingsdialog.h

SOURCES += \
    main.cpp \
    dialog.cpp \
    masterthread.cpp \
    settingsdialog.cpp

FORMS += \
    settingsdialog.ui
