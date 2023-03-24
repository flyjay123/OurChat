#ifndef SYSTEMMESSAGE_H
#define SYSTEMMESSAGE_H

#include <QWidget>

namespace Ui {
class SystemMessage;
}

class SystemMessage : public QWidget
{
    Q_OBJECT

public:
    explicit SystemMessage(QWidget *parent = nullptr);
    ~SystemMessage();

private:
    Ui::SystemMessage *ui;
};

#endif // SYSTEMMESSAGE_H
