#ifndef FRIENTITEM_H
#define FRIENTITEM_H

#include <QWidget>

namespace Ui {
class FriendItem;
}

class FriendItem : public QWidget
{
    Q_OBJECT

public:
    explicit FriendItem(QWidget *parent = nullptr);
    ~FriendItem();

    int account(){return m_account;}
    void setAccount(int a){m_account = a;}
    void setName(const QString &name);
    void setSignature(const QString &signature);
    void setAvatar(const QPixmap &avatar);

    QString getName();

private:
    Ui::FriendItem *ui;
    int m_account;
};

#endif // FRIENTITEM_H
