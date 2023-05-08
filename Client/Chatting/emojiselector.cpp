#include "emojiselector.h"
#include "smoothscrolllistwidget.h"


EmojiSelector::EmojiSelector(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint | Qt::Popup),
    m_layout(new QVBoxLayout(this)),
    m_stackedWidget(new QStackedWidget(this))
{
    setFixedSize(400,300);
    //m_stackedWidget->setStyleSheet("border:none");
    this->setLayout(m_layout);
    m_layout->setSpacing(0);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->setSpacing(0);
    m_layout->addWidget(m_stackedWidget);
    m_layout->addLayout(headerLayout);

    QStringList categories = {"表情 ", "装饰标志", "食物", "交通"};
    for (int i = 0; i < categories.size(); ++i) {
        QPushButton *categoryButton = new QPushButton(categories[i], this);
        headerLayout->addWidget(categoryButton);

        connect(categoryButton, &QPushButton::clicked, this, [this, i]() {
            switchPage(i);
        });
    }
    addEmojiPage(0x1F600, 0x1F64F);
    addEmojiPage(0x2702, 0x27B0);
    addEmojiPage(0x1F344, 0x1F37F);
    addEmojiPage(0x1F680, 0x1F6C0);
}

void EmojiSelector::addEmojiPage(char32_t start, char32_t end)
{
    SmoothScrollListWidget *emojiList = new SmoothScrollListWidget(this);
    emojiList->setFlow(QListView::LeftToRight);
    emojiList->setWrapping(true);
    emojiList->setSpacing(0);
    emojiList->setResizeMode(QListView::Adjust);
    emojiList->setGridSize(QSize(40, 40));
    emojiList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    for (char32_t i = start; i <= end; ++i) {
        QString emoji = QString::fromUcs4(&i, 1);
        QPushButton *emojiButton = new QPushButton(emoji, this);
        emojiButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        emojiButton->setStyleSheet("QPushButton {"
                                   "border: none;"
                                   "min-width: 40px;"
                                   "max-width: 40px;"
                                   "min-height: 40px;"
                                   "max-height: 40px;"
                                   "font-size: 24px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "background-color: rgba(200, 200, 200, 100);"
                                   "font-size: 30px;"
                                   "}");

        connect(emojiButton, &QPushButton::clicked, this, [this, emoji]() {
            emit emojiSelected(emoji);
        });

        QListWidgetItem *item = new QListWidgetItem(emojiList);
        item->setSizeHint(emojiButton->sizeHint());
        emojiList->addItem(item);
        emojiList->setItemWidget(item, emojiButton);
    }

    m_stackedWidget->addWidget(emojiList);
}

void EmojiSelector::switchPage(int index)
{
    m_stackedWidget->setCurrentIndex(index);
    qDebug()<<"index:"<<index;
}
