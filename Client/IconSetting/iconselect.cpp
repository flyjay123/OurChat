#include "iconselect.h"
#include "ui_iconselect.h"
#include <QListWidgetItem>
#include <QDebug>

IconSelect::IconSelect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IconSelect)
{
    ui->setupUi(this);
    for(int i = 1; i < 21; i++) {
        iconUrl.push_back(":/Icons/src/QQIcon/icon" + QString::number(i) + ".jpg");
    }
    ui->listWidget->setIconSize(QSize(100, 100));
    for(int i = 1; i < 21; i++) {
        QListWidgetItem *item = new QListWidgetItem(QIcon(iconUrl[i - 1]), "", ui->listWidget);
        item->setSizeHint(QSize(100, 100));
        item->setFlags(item->flags() & ~Qt::ItemIsDragEnabled);
    }

}

IconSelect::~IconSelect()
{
    delete ui;
}

void IconSelect::on_listWidget_itemSelectionChanged()
{
    qDebug() << "click" << ui->listWidget->currentIndex().row();
    curIcon = ui->listWidget->currentIndex().row();
}

void IconSelect::on_pushButton_clicked()
{
    QString url;
    QString text = ui->lineEdit->text();
    if(curIcon == -1 && text.isEmpty())
        close();
    else if(!text.isEmpty()) {
        url = text;
        emit SetIcon(url);
        close();
    }
    else {
        url = iconUrl[curIcon];
        emit SetIcon(url);
        close();
    }
}

void IconSelect::on_lineEdit_selectionChanged()
{
    curIcon = -1;
}
