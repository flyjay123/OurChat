#include "inputdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

InputDialog::InputDialog(TcpClient *t,QWidget *parent) : QDialog(parent) {
    m_t = t;
    this->setWindowTitle("输入对话框");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label1 = new QLabel("IP:", this);
    layout->addWidget(label1);
    m_lineEdit1 = new QLineEdit(this);
    layout->addWidget(m_lineEdit1);

    QLabel *label2 = new QLabel("Port:", this);
    layout->addWidget(label2);
    m_lineEdit2 = new QLineEdit(this);
    layout->addWidget(m_lineEdit2);

    QPushButton *confirmButton = new QPushButton("确定", this);
    connect(confirmButton, &QPushButton::clicked, this, &InputDialog::onConfirmButtonClicked);
    layout->addWidget(confirmButton);

    QPushButton *cancelButton = new QPushButton("取消", this);
    connect(cancelButton, &QPushButton::clicked, this, &InputDialog::reject);
    layout->addWidget(cancelButton);
}

void InputDialog::onConfirmButtonClicked() {
    m_t->SetServer(m_lineEdit1->text(),m_lineEdit2->text().toInt());
    m_t->ConnectToServer();

    accept();
}
