#ifndef SNAKE_H
#define SNAKE_H

#include <QMainWindow>
#include <QScreen>
#include <QPainter>
#include <QButtonGroup>
#include <QTimer>
#include <QKeyEvent>
#include <QVector>
#include <QtDebug>
#include <QRandomGenerator>
#include <QLCDNumber>

QT_BEGIN_NAMESPACE
namespace Ui { class snake; }
QT_END_NAMESPACE

class snake : public QMainWindow
{
    Q_OBJECT

public:
    snake(QWidget *parent = nullptr); // construct function
    ~snake(); // deconstruct function

public:
    // event
    void paintEvent(QPaintEvent*);
    void keyPressEvent(QKeyEvent*);
    // physical collision covering collision with food, wall and itself
    void collision();
    // initialize the location of snake and food on the map
    void init_snake();
    QRect create_food();

public slots:
    // button slot (signal: button event)
    void game_start();
    void game_pause_n_continuous();
    // snake move slot (signal: timer)
    void snake_move();

private:
    Ui::snake *ui;
    // timer
    QTimer *timer;
    int time_interval;
    // snake
    QVector<QRect> snake_body;
    QRect snake_head;
    QRect snake_tail;
    int snake_length;
    QString direction;
    // game item
    int score;
    QRect init_food;
    // game status
    bool start_flag;
    bool pause_flag;
    bool end_flag;
};
#endif // SNAKE_H
