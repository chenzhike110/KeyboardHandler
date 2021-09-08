#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QUdpSocket>
#include <string>
#include <linux/input.h>

class KeyBoard
{
public:
    KeyBoard(std::string, int);
    void send();
    void StartReading();
private:
    QUdpSocket* sendSocket;
    struct input_event inputEvent;
    int c_iHd;
    quint8 _command[10];
    QByteArray tx;
    int _port;
    int function_holding[6];
};

#endif // KEYBOARD_H
