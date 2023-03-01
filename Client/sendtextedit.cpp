#include <QDebug>
#include <QKeyEvent>
#include <QEvent>
#include "sendtextedit.h"

void SendTextEdit::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if(event->modifiers() == Qt::ControlModifier)
        {
            append("");
        }
        else
            emit keyPressEnter();
    }
    else
     {
         QTextEdit::keyPressEvent(event);
     }

}


