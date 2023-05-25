#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "tcpclient.h"

class InputDialog : public QDialog {
    Q_OBJECT

public:
    explicit InputDialog(TcpClient* t, QWidget *parent = nullptr);

private slots:
    void onConfirmButtonClicked();

private:
    QLineEdit *m_lineEdit1;
    QLineEdit *m_lineEdit2;
    TcpClient* m_t;
};

#endif // INPUTDIALOG_H
