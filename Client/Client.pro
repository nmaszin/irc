QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES +=  main.cpp\
            ChatPart.cpp \
            Client.cpp \
            ConnectWindow.cpp \
            InputParse.cpp \
            Networking.cpp \
            OutputParse.cpp

HEADERS  += Client.h \
            ChatPart.h \
            ConnectWindow.h \
            InputParse.h \
            Networking.h \
            OutputParse.h

FORMS    += Client.ui \
    ConnectWindow.ui
