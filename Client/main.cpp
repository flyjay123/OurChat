#include "client.h"
#include "logging.h"
#include "tcpclient.h"
#include <QApplication>

TcpClient t;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    t.ConnetToServer();
    Logging w;
    w.show();
    return a.exec();
}
