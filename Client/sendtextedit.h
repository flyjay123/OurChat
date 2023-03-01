#ifndef SENDTEXTEDIT_H
#define SENDTEXTEDIT_H

#include <QTextEdit>
#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QMainWindow>

class SendTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    SendTextEdit(QWidget *parent = nullptr) : QTextEdit(parent){}

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
   void keyPressEnter();
};

#endif // SENDTEXTEDIT_H
