#include "client.h"
#include "logging.h"
#include "tcpclient.h"
#include <QApplication>
bool friendWidgetOn = false;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Logging w;
    w.show();
    return a.exec();
}
