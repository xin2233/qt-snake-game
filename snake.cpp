#include "snake.h"
#include "ui_snake.h"
#include <iostream>

snake::snake(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::snake)
{
    ui->setupUi(this);
    // move the window to the center of desktop
//    QDesktopWidget *desktop = QApplication::desktop();
//    int x = (desktop->width() - this->width()) / 2;
//    int y = (desktop->height() - this->width()) / 2;

    //获取设备屏幕大小
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    //获取设备像素比
//    double devicePixelRatio = QGuiApplication::primaryScreen()->devicePixelRatio();
//    width
    int x = (screenRect.width() - this->width()) / 2;
//    height
    int y = (screenRect.height() - this->height()) /2;
    move(x,y);
    // button event send to game status slot to change status
    connect(ui->startbutton,SIGNAL(clicked()),this, SLOT(game_start()));
    connect(ui->pausebutton,SIGNAL(clicked()),this, SLOT(game_pause_n_continuous()));
    // timer initialization
    time_interval = 50;
    timer = new QTimer(this);
    // initilize flag
    start_flag = false;
    pause_flag = false;
    end_flag = false;
}

snake::~snake()
{
    qDebug() << tr("snake::~snake()");
    delete ui;
}
// paint event
void snake::paintEvent(QPaintEvent *){
    this->setWindowTitle("Snake Game");
    // paint interface
    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    painter.drawRect(0,0,500,500);
    painter.setBrush(Qt::black);
    painter.drawRect(10,10,480,480);
    // paint grids
//    for(int i = 0; i < 50; i++){
//        painter.drawLine(10*i+10,10,10*i+10,490);
//        painter.drawLine(10,10*i+10,490,10*i+10);
//    }
    // paint snake and food
    painter.setBrush(Qt::white);
    if(start_flag||pause_flag){
        painter.drawRects(&snake_body[0],snake_body.size());
        painter.drawRect(init_food);
    }

    QFont gameover("Courier",50);
    painter.setFont(gameover);
    if(!end_flag&&!start_flag&&!pause_flag){
        painter.drawText(10,10,480,480,Qt::AlignHCenter|Qt::AlignVCenter,"Snake Game");
    }
    if(end_flag){
        painter.drawText(10,10,480,480,Qt::AlignHCenter|Qt::AlignVCenter,"Game Over");
    }
}
// key button event
void snake::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_W: if(direction!="down")   direction = "up"; break;
    case Qt::Key_S: if(direction!="up")     direction = "down"; break;
    case Qt::Key_A: if(direction!="right")  direction = "left"; break;
    case Qt::Key_D: if(direction!="left")   direction = "right"; break;
    default:;
    }
}
// initialize snake
void snake::init_snake(){
    direction = "right";
    snake_body.resize(3);
    snake_length = snake_body.count();
    for(int i = 0; i < snake_length; i++){
        QRect rect(50,10*i+30,10,10);
        snake_body[i] = rect;
    }
}
// create food
QRect snake::create_food(){
    int x = QRandomGenerator::global()->generate()%47;
    int y = QRandomGenerator::global()->generate()%47;
    QRect food(x*10+10,y*10+10,10,10);
    if(snake_body.contains(food)){
        create_food();
    }
    return food;
}
// start a new game
void snake::game_start(){

    start_flag = true;
    pause_flag = false;
    end_flag = false;

    score = 0;
    init_snake();
    init_food = create_food();

    timer->stop();
    timer = new QTimer(this);
    timer->start(time_interval);
    connect(timer, SIGNAL(timeout()),this, SLOT(snake_move()));

}
// pause or continuous
void snake::game_pause_n_continuous(){
    start_flag = not start_flag ;
    pause_flag = not pause_flag ;
    update();
}
// update the movement of snake
void snake::snake_move(){

    if(start_flag){

        snake_head = snake_body.first();
        snake_tail = snake_body.last();

        snake_length = snake_body.count();
        for(int i = 0; i < snake_length-1; i++){
        snake_body[snake_body.size()-i-1] = snake_body[snake_body.size()-i-2];
    }

        if(direction == "up"){
        snake_head.setTop(snake_head.top()-10);
        snake_head.setBottom(snake_head.bottom()-10);
    }
        else if(direction == "down"){
        snake_head.setTop(snake_head.top()+10);
        snake_head.setBottom(snake_head.bottom()+10);
    }
        else if(direction == "left"){
        snake_head.setLeft(snake_head.left()-10);
        snake_head.setRight(snake_head.right()-10);
    }
        else if(direction == "right"){
        snake_head.setLeft(snake_head.left()+10);
        snake_head.setRight(snake_head.right()+10);
    }
        collision();
        snake_body[0] = snake_head;
    }

    update();

}
// physical collision
void snake::collision(){
    //collided with food
    if(snake_head == init_food){
        score += 10;
        ui->sc->display(score);
        init_food = create_food();
        snake_body.push_back(snake_tail);
    }

    //collided with boarder
    if(snake_head.x()<10 || snake_head.x()>480 || snake_head.y() <10 || snake_head.y()>480){
        timer->stop();
        start_flag = false;
        end_flag = true;
    }

    //collided with itself
    if(snake_body.contains(snake_head)){
        timer->stop();
        start_flag = false;
        end_flag = true;
    }

}
