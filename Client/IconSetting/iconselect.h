#ifndef ICONSELECT_H
#define ICONSELECT_H

#include <QWidget>

namespace Ui {
class IconSelect;
}

class IconSelect : public QWidget
{
    Q_OBJECT

public:
    explicit IconSelect(QWidget *parent = nullptr);
    ~IconSelect();

signals:
    void  SetIcon(QString url);

private slots:
    void on_listWidget_itemSelectionChanged();

    void on_pushButton_clicked();

    void on_lineEdit_selectionChanged();

private:
    Ui::IconSelect *ui;
    int curIcon = -1;
    QString curIconUrl;
    QVector <QString> iconUrl;
};

#endif // ICONSELECT_H
