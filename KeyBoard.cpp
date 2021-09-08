#include "KeyBoard.h"
#include <QDataStream>
#include <unistd.h>

#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <thread>

namespace  {
    std::thread * readThread;
    enum BUFFON{
        UP = 103,
        DOWN = 108,
        LEFT = 105,
        RIGHT = 106,
        Q = 16,
        W = 17,
        A = 30,
        S = 31,
        D = 32,
        F = 33
    };
}

KeyBoard::KeyBoard(std::string eventName, int port) : tx(10, 0), _port(port+60000){
    sendSocket = new QUdpSocket();
    qDebug() << "start reading" << QString::fromStdString(eventName);
    eventName = "/dev/input/"+eventName;
    c_iHd = open(eventName.c_str(), O_RDONLY | O_NONBLOCK);
    if (c_iHd == -1) {
        qDebug() << "[ERROR] Read Input Error!";
        exit(-1);
    }
    fcntl(c_iHd, F_SETFL, 0);
    readThread = new std::thread([ = ] {StartReading();});
    readThread->detach();
}

void KeyBoard::send() {
    for(int i=0;i <10; i++){
        tx[i] = _command[i];
    }
    sendSocket->writeDatagram(tx.data(), sizeof (_command), QHostAddress("127.0.0.1"), _port);
}

void KeyBoard::StartReading() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::microseconds(2));
        read(c_iHd, &inputEvent, sizeof (struct input_event));
        if (inputEvent.type != EV_KEY) continue;
        switch (inputEvent.code) {
        case UP:
            _command[0] = inputEvent.value * 127;
            break;
        case DOWN:
            _command[1] = inputEvent.value * 127;
            break;
        case LEFT:
            _command[2] = inputEvent.value * 127;
            break;
        case RIGHT:
            _command[3] = inputEvent.value * 127;
            break;
        case Q:
            if (inputEvent.type == 0) _command[4] = function_holding[0];
            else if (inputEvent.type == 1 || inputEvent.type == 2) function_holding[0] = std::min(function_holding[0]+1, 255);
            break;
        case W:
            if (inputEvent.type == 0) _command[5] = function_holding[1];
            else if (inputEvent.type == 1 || inputEvent.type == 2) function_holding[1] = std::min(function_holding[1]+1, 255);
            break;
        case A:
            if (inputEvent.type == 0) _command[6] = function_holding[2];
            else if (inputEvent.type == 1 || inputEvent.type == 2) function_holding[2] = std::min(function_holding[2]+1, 255);
            break;
        case S:
            if (inputEvent.type == 0) _command[7] = function_holding[3];
            else if (inputEvent.type == 1 || inputEvent.type == 2) function_holding[3] = std::min(function_holding[3]+1, 255);
            break;
        case D:
            if (inputEvent.type == 0) _command[8] = function_holding[4];
            else if (inputEvent.type == 1 || inputEvent.type == 2) function_holding[4] = std::min(function_holding[4]+1, 255);
            break;
        case F:
            if (inputEvent.type == 0) _command[9] = function_holding[5];
            else if (inputEvent.type == 1 || inputEvent.type == 2) function_holding[5] = std::min(function_holding[5]+1, 255);
            break;
        default:
            qDebug() << inputEvent.type << " " << inputEvent.code << " " << inputEvent.value;
        }
        send();
    }
}
