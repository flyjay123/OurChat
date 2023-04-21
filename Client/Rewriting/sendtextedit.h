#ifndef SENDTEXTEDIT_H
#define SENDTEXTEDIT_H

#include <QTextEdit>
#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QMainWindow>
#include <QKeyEvent>
#include <QEvent>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QBuffer>

class SendTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    SendTextEdit(QWidget *parent = nullptr) : QTextEdit(parent){}

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
   void keyPressEnter();
};

#endif // SENDTEXTEDIT_H
