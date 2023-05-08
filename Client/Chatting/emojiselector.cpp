#include "emojiselector.h"

EmojiSelector::EmojiSelector(QWidget *parent) :
    QDialog(parent,Qt::FramelessWindowHint | Qt::Popup),
    m_layout(new QGridLayout(this)),
    m_signalMapper(new QSignalMapper(this))
{
    this->setLayout(m_layout);
    m_layout->setSpacing(0);

    int row = 0;
    int col = 0;

    for (char32_t i = 0x1F601; i <= 0x1F64F; ++i) {
        QString emoji = QString::fromUcs4(&i, 1);
        QPushButton *emojiButton = new QPushButton(emoji, this);
        emojiButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        emojiButton->setStyleSheet("QPushButton {"
                                   "border: none;"
                                   "width: 40px;"
                                   "height: 40px;"
                                   "font-size: 24px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "background-color: rgba(200, 200, 200, 100);"
                                   "font-size: 30px;"
                                   "}");



        m_layout->addWidget(emojiButton, row, col);

        connect(emojiButton, &QPushButton::clicked, this, [this, emoji]() {
            emit emojiSelected(emoji);
        });

        col++;
        if (col >= 8) {
            col = 0;
            row++;
        }
    }
}

