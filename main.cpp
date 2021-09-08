#include <QCoreApplication>
#include "KeyBoard.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    KeyBoard handler(argv[1], std::atoi(argv[2]));
    return a.exec();
}
