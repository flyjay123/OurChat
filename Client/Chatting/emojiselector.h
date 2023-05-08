#ifndef EMOJISELECTOR_H
#define EMOJISELECTOR_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QSignalMapper>
#include <QFocusEvent>

class EmojiSelector : public QDialog
{
    Q_OBJECT

public:
    explicit EmojiSelector(QWidget *parent = nullptr);

signals:
    void emojiSelected(const QString &emoji);



private:
    QGridLayout *m_layout;
    QSignalMapper *m_signalMapper;
};

#endif // EMOJISELECTOR_H
