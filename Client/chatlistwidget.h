#ifndef CHATLISTWIDGET_H
#define CHATLISTWIDGET_H
#include <QListWidget>
#include <QListWidgetItem>
#include "frienditem.h"
#include "friendiconlabel.h"

enum ItemType
{
    ItemType_Message = 0,
    ItemType_Friend,
    ItemType_Group,
};

class ChatListWidget : public QListWidget
{
    Q_OBJECT
public:

    explicit ChatListWidget(ItemType type = ItemType_Message,QWidget *parent = nullptr);
    ItemType GetType(){return m_type;}
    void SetType(ItemType type){m_type = type;}


private:
    ItemType m_type;
};

#endif // CHATLISTWIDGET_H
