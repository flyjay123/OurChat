#include "logging.h"
#include <QBitmap>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include "stringtool.h"
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

#if 0
    QWidget *container;
    container=new QWidget(this);
    container->setGeometry(5,5,width,height);
    container->setStyleSheet("QWidget{border-radius:4px;background:rgba(255,255,255,1);}");

    this->setWindowFlags(Qt::FramelessWindowHint);      //去掉标题栏无边框
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
    container->setGraphicsEffect(shadow);
#else
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
#endif


    //memset(&m_userInfo,'\0',sizeof(m_userInfo));
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



void Logging::on_pushbtn_regist_clicked()
{

}

void Logging::on_pushButton_login_clicked()
{

    json msg;
    msg.insert("cmd","login");
    msg.insert("account",ui->lineEdit_account->text());
    msg.insert("password",ui->lineEdit_password->text());

    t.SendMsg(msg);

    if(t.socket->waitForReadyRead(3000))
    {
        t.onReadyRead();
        json msg = t.GetMessage();
        if(msg.value("cmd").toString() == "yes")
        {
            qDebug() << "登录成功" << endl;
        }
        else
        {
            //string err = e["errMsg"].get<string>();
            //qDebug() << StringTool::Str2QStr(err) << endl;
        }
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
