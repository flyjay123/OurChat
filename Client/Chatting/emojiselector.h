#ifndef EMOJISELECTOR_H
#define EMOJISELECTOR_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QSignalMapper>
#include <QFocusEvent>
#include <QStackedWidget>
#include <QButtonGroup>


class EmojiSelector : public QDialog
{
    Q_OBJECT

public:
    explicit EmojiSelector(QWidget *parent = nullptr);

signals:
    void emojiSelected(const QString &emoji);

private:
    void addEmojiPage(char32_t start, char32_t end);
    void switchPage(int index);


private:
    QVBoxLayout *m_layout;
    QSignalMapper *m_signalMapper;
    QStackedWidget *m_stackedWidget;
    QButtonGroup *m_buttonGroup;
};

#endif // EMOJISELECTOR_H
