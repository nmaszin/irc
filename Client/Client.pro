QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


CONFIG += c++17

SOURCES +=  main.cpp\
            ChannelState.cpp \
            ChatPart.cpp \
            Client.cpp \
            ConnectDialog.cpp \
            InputParse.cpp \
            IrcSocket.cpp \
            Networking.cpp \
            OutputIrcMessage.cpp \
            OutputParse.cpp \
            ServerState.cpp

HEADERS  += Client.h \
            ChannelState.h \
            ChatPart.h \
            ConnectDialog.h \
            InputParse.h \
            IrcSocket.h \
            Networking.h \
            OutputIrcMessage.h \
            OutputParse.h \
            ServerState.h \
            utils.h

FORMS    += Client.ui \
    ConnectDialog.ui

RESOURCES += \
              app.qrc
