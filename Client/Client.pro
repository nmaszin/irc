QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


CONFIG += c++17

SOURCES +=  main.cpp\
            Client.cpp \
            ChannelState.cpp \
            ChatPart.cpp \
            ConnectDialog.cpp \
            InputMessage.cpp \
            IrcSocket.cpp \
            ServerState.cpp \
            Networking.cpp

HEADERS  += Client.h \
            ChannelState.h \
            ChatPart.h \
            ConnectDialog.h \
            InputMessage.h \
            IrcSocket.h \
            ServerState.h \
            Networking.h

FORMS    += Client.ui \
            ConnectDialog.ui

RESOURCES += \
              app.qrc
