QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES +=  main.cpp\
            ChatPart.cpp \
            Client.cpp \
            ConnectDialog.cpp \
            InputParse.cpp \
            Networking.cpp \
            OutputParse.cpp

HEADERS  += Client.h \
            ChatPart.h \
            ConnectDialog.h \
            InputParse.h \
            Networking.h \
            OutputParse.h

FORMS    += Client.ui \
    ConnectDialog.ui

RESOURCES += \
              app.qrc
