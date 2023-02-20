#include "logging.h"
#include <QBitmap>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include "stringtool.h"
#include "client.h"
#include<QPropertyAnimation>
#include <QThread>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QTimer>
#include <QShortcut>


using namespace std;
Logging::Logging(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Logging)
{
    ui->setupUi(this);
    Init();
}

Logging::~Logging()
{
    delete ui;
}

void Logging::Init()
{
    this->setWindowTitle("WeChat 登录");
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint); // 最小化按钮
    setWindowFlags(windowFlags() | Qt::WindowContextHelpButtonHint); // 帮助按钮

    int width = this->width()-10;
    int height = this->height()-10;
    ui->centerWidget->setGeometry(5,5,width,height);
    ui->centerWidget->setStyleSheet("QWidget{border-radius:4px;background:rgba(255,255,255,1);}");  //设置圆角

    this->setWindowFlags(Qt::FramelessWindowHint);          //去掉标题栏无边框
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    //实例阴影shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor(QColor(39,40,43,100));
    //设置阴影圆角
    shadow->setBlurRadius(10);
    //给嵌套QWidget设置阴影
    ui->centerWidget->setGraphicsEffect(shadow);

    //memset(&m_userInfo,'\0',sizeof(m_userInfo));

    QShortcut *key=new QShortcut(QKeySequence(Qt::Key_Return),this);//创建一个快捷键"Key_Return"键
    connect(key,&QShortcut::activated,this,&Logging::on_pushButton_login_clicked);//连接到指定槽函数
}

void Logging::mousePressEvent(QMouseEvent *event)
{
    mouseWindowTopLeft = event->pos();
}

void Logging::mouseMoveEvent(QMouseEvent *event)
{
    //窗口移动
    if (event->buttons() & Qt::LeftButton)
    {
        mouseDeskTopLeft = event->globalPos();
        windowDeskTopLeft = mouseDeskTopLeft - mouseWindowTopLeft;  //矢量计算
        this->move(windowDeskTopLeft);     //移动到目的地
    }
}

void Logging::on_pushBtn_hide_clicked()
{
    QWidget* pWindow = this->window();
    if(pWindow->isTopLevel())
        pWindow->showMinimized();
}

void Logging::on_pushBtn_close_clicked()
{
    json msg;
    msg.insert("cmd","logout");
    t.SendMsg(msg);
    this->close();
}

void Logging::on_pushBtn_close_2_clicked()
{
    json msg;
    msg.insert("cmd","logout");
    t.SendMsg(msg);
    this->close();
}

void Logging::on_pushBtn_hide_2_clicked()
{
    QWidget* pWindow = this->window();
    if(pWindow->isTopLevel())
        pWindow->showMinimized();
}

void Logging::on_pushButton_return_clicked()
{
    PageSwitch(0);
}

void Logging::on_pushbtn_regist_clicked()
{
    PageSwitch(1);
}
void Logging::PageSwitch(int pageIndex)
{
    int curIndex = ui->stackedWidget->currentIndex();
    {
        if (pageIndex < 0 || pageIndex >= ui->stackedWidget->count()) {
            return;
        }
        QWidget *currentPage = ui->stackedWidget->currentWidget();
        QWidget *targetPage = ui->stackedWidget->widget(pageIndex);
        targetPage->setGeometry(0,0,currentPage->width(),currentPage->height());

        if (!currentPage || !targetPage || currentPage == targetPage) {
            return;
        }

        int currentPageX = currentPage->x();
        int targetPageX = targetPage->x();
        int currentPageWidth = currentPage->width();
        // 创建当前页面向左滑出的动画
        QPropertyAnimation *currentPageAnimation = new QPropertyAnimation(currentPage, "geometry");
        currentPageAnimation->setDuration(600);
        currentPageAnimation->setEasingCurve(QEasingCurve::InOutQuad);
        // 创建目标页面从右至左滑进的动画
        QPropertyAnimation *targetPageAnimation = new QPropertyAnimation(targetPage, "geometry");
        targetPageAnimation->setDuration(600);
        targetPageAnimation->setEasingCurve(QEasingCurve::InOutQuad);
if(pageIndex == 1)
{
    currentPageAnimation->setStartValue(QRect(currentPageX, currentPage->y(), currentPageWidth, currentPage->height()));
    currentPageAnimation->setEndValue(QRect(currentPageX - currentPageWidth, currentPage->y(), currentPageWidth, currentPage->height()));

    targetPageAnimation->setStartValue(QRect(targetPageX + currentPageWidth, targetPage->y(), targetPage->width(), targetPage->height()));
    targetPageAnimation->setEndValue(QRect(targetPageX, targetPage->y(), targetPage->width(), targetPage->height()));
}
else
{
    currentPageAnimation->setStartValue(QRect(currentPageX, currentPage->y(), currentPageWidth, currentPage->height()));
    currentPageAnimation->setEndValue(QRect(currentPageX + currentPageWidth, currentPage->y(), currentPageWidth, currentPage->height()));

    targetPageAnimation->setStartValue(QRect(targetPageX - currentPageWidth, targetPage->y(), targetPage->width(), targetPage->height()));
    targetPageAnimation->setEndValue(QRect(targetPageX, targetPage->y(), targetPage->width(), targetPage->height()));
}
        // 创建动画组，添加动画，并启动动画组
        QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup();
        animationGroup->addAnimation(currentPageAnimation);
        animationGroup->addAnimation(targetPageAnimation);
        ui->stackedWidget->widget(pageIndex)->setVisible(true);
        ui->stackedWidget->widget(pageIndex)->update();
        ui->stackedWidget->widget(pageIndex)->show();
        animationGroup->start(QAbstractAnimation::DeleteWhenStopped);

        animationGroup->setProperty(
                "widget", QVariant::fromValue(ui->stackedWidget->widget(curIndex)));

        connect(animationGroup, &QParallelAnimationGroup::finished, [=]() {
            // 切换页面
            ui->stackedWidget->widget(pageIndex)->hide();
            ui->stackedWidget->setCurrentIndex(pageIndex);
                });
    }
}

void Logging::on_pushButton_login_clicked()
{
     if(!t.m_isConnected)
     {
         qDebug() << "未连接" << endl;
         if(t.ConnetToServer() == -1)
         {
             qDebug() << "连接失败" << endl;
             return;
         }
     }
    json msg;
    msg.insert("cmd","login");
    msg.insert("account",ui->lineEdit_account->text());
    msg.insert("password",ui->lineEdit_password->text());

    t.SendMsg(msg);


    //t.socket->readyRead();
    if(t.socket->waitForReadyRead(3000))
    {
        json msg = t.GetMessage();
        if(msg.isEmpty()) return ;
        if(msg.value("cmd").toString() == "yes")
        {
            qDebug() << "登录成功" << endl;
            Client* client = new Client();
            client->show();
            this->close();
        }
        else
            qDebug() << msg.value("err").toString() << endl;
    }
    else
        qDebug() << "与服务器连接失败" << endl;
}

void Logging::on_pushButton_regist_clicked()
{
    if(!t.m_isConnected)
    {
        if(t.ConnetToServer() == -1)
            return;
    }
    json msg;
    msg.insert("cmd","regist");
    msg.insert("account",ui->lineEdit_account_2->text());
    msg.insert("password",ui->lineEdit_password_2->text());
    msg.insert("name",ui->lineEdit_name->text());
    t.SendMsg(msg);

    if(t.socket->waitForReadyRead(3000))
    {
       // t.onReadyRead();
        json msg = t.GetMessage();
        if(msg.value("cmd").toString() == "yes")
        {
            qDebug() << "注册成功" << endl;
            ui->stackedWidget->setCurrentIndex(0);
        }
        else
        {
            qDebug() << msg.value("err").toString() << endl;
        }
    }
}

void Logging::on_pushButton_seePassword_clicked()
{
    if(ui->lineEdit_password->echoMode() == QLineEdit::Password)
    {
          ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
    }
    else
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}




