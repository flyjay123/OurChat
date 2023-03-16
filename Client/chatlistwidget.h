#ifndef CHATLISTWIDGET_H
#define CHATLISTWIDGET_H
#include <QListWidget>
#include <QListWidgetItem>
#include "frienditem.h"
#include "friendiconlabel.h"


class ChatListWidget : public QListWidget
{
    Q_OBJECT
public:

    explicit ChatListWidget(QWidget *parent = nullptr);
};

#endif // CHATLISTWIDGET_H
