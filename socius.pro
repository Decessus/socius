#-------------------------------------------------
#
# Project created by QtCreator 2011-02-28T01:28:37
#
#-------------------------------------------------

QT       += core gui xml network webkit

TARGET = socius
TEMPLATE = app


SOURCES += main.cpp\
        socius.cpp \
    socialsource.cpp \
    scripting/irc_scripting.cpp \
    irc/irc.cpp \
    twitter/twitter.cpp \
    irc/session.cpp \
    irc/commands.cpp

HEADERS  += socius.h \
    updateEvent.h \
    error.h \
    socialsource.h \
    scripting/irc_scripting.h \
    irc/irc.h \
    twitter/twitter.h \
    irc/session.h \
    irc/utilities.h \
    irc/commands.h

FORMS    += socius.ui \
    irc.ui

CONFIG += libircclient-qt oauth

OTHER_FILES += \
    README.mkdn
