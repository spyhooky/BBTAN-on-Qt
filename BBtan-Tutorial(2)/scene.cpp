#include "scene.h"

Scene::Scene(QObject *parent)
{
    screenMode = 0;
}

void Scene::Init(int x_start)
{
    // start
    btn_start = new Btn();
    QPixmap start;
    start.load("../img/btn_start.jpg");
    start = start.scaled(start.width()*3/2,start.height()*3/2,Qt::KeepAspectRatio);
    btn_w = (start.width()*3)/2 - 60;
    btn_h = (start.height()*3)/2 - 30;
    btn_start->setPixmap(start);
    btn_start->setPos(200,390);
    addItem(btn_start);
    // change
    btn_change_ball = new Btn();
    QPixmap cb;
    cb.load("../img/btn_change_ball.jpg");
    cb = cb.scaled((cb.width()*3)/2,(cb.height()*3)/2,Qt::KeepAspectRatio);
    btn_change_ball->setPixmap(cb);
    btn_change_ball->setPos(35,390);
    addItem(btn_change_ball);

    //  setting the boundary , only lowerBound has a little different
    leftBound = x_start;
    rightBound = x_start+370;
    upperBound = 50;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(screenMode == 0)
    {
        // Beginning page
        if(event->scenePos().x() > btn_start->pos().x() && event->scenePos().x() <= btn_start->pos().x()+btn_w)
        {
            // Now x is in range , judge y
            if(event->scenePos().y() > btn_start->pos().y() && event->scenePos().y() <= btn_start->pos().y()+btn_h)
            {
                // x , y both in button area
                cout<< "Start Game"<< endl;
                // Clear original background
                bgChange();
                // Goto setting the game initial
                gameInit();
                screenMode = 1;
            }
        }
        // or click on change ball
        if(event->scenePos().x() > btn_change_ball->pos().x() && event->scenePos().x() <= btn_change_ball->pos().x()+btn_w)
        {
            // Now x is in range , judge y
            if(event->scenePos().y() > btn_change_ball->pos().y() && event->scenePos().y() <= btn_change_ball->pos().y()+btn_h)
            {
                // x , y both in button area
                cout<< "change get"<< endl;
                bgChange();
                screenMode = 2; // mode to change ball
            }
        }
    }
    else if(screenMode == 1)
    {
        // Starting Page
        if(event->scenePos().x() > btn_back->pos().x() && event->scenePos().x() <= btn_back->pos().x()+btn_small_w)
        {
            // Now x is in range , judge y
            if(event->scenePos().y() > btn_back->pos().y() && event->scenePos().y() <= btn_back->pos().y()+btn_small_h)
            {
                // x , y both in back button area
                bgRecover();
                screenMode = 0;
            }
        }
        // Starting Page setting - shooting point
    }
    else if(screenMode == 2)
    {
        // Changing ball Page
        if(event->scenePos().x() > btn_back->pos().x() && event->scenePos().x() <= btn_back->pos().x()+btn_small_w)
        {
            // Now x is in range , judge y
            if(event->scenePos().y() > btn_back->pos().y() && event->scenePos().y() <= btn_back->pos().y()+btn_small_h)
            {
                // x , y both in back button area
                bgRecover();
                screenMode = 0;
            }
        }
        // Else for changing ball's implementation

    }

    /* For debugging */
    //cout << "btn start x: " << btn_start->pos().x()<< " btn start y: " << btn_start->pos().y() << endl;
    //cout << "Now event x: " << event->scenePos().x()<< " event y: " << event->scenePos().y() << endl;
}

void Scene::bgChange()
{
    /* first , change background picture */
    QImage bg;
    bg.load("../img/start.jpg");
    bg = bg.scaled(370,630);
    this->setBackgroundBrush(bg);
    /* second , remove btn - start and change ball */
    this->removeItem(btn_start);
    this->removeItem(btn_change_ball);

    // Back buttom setting
    btn_back = new Btn();
    QPixmap bk;
    bk.load("../img/back.jpg");
    btn_small_w = bk.width()*2/3;
    btn_small_h = bk.height()*2/3;
    bk = bk.scaled(btn_small_w , btn_small_h ,Qt::KeepAspectRatio);
    btn_back->setPixmap(bk);
    btn_back->setPos(0,5);
    addItem(btn_back);
}

void Scene::bgRecover()
{
    // remove item here
    removeItem(btn_back);
    removeItem(man);
    // add back initial page
    addItem(btn_start);
    addItem(btn_change_ball);
    QImage bg;
    bg.load("../img/open.jpg");
    bg = bg.scaled(370,630);
    this->setBackgroundBrush(bg);
}

void Scene::gameInit()
{
    // Setting the character on it
    man = new Character();
    QPixmap ch;
    ch.load("../img/character.png");
    ch = ch.scaled( ch.width()*2/3, ch.height()*2/3 ,Qt::KeepAspectRatio);
    man->setPixmap(ch);
    man->setPos(300,450);
    // save the initial position
    ch_x_pos = 300;
    // save the initial lowerBoundary
    lowerBound = 450+50; // 50 is the bias (every picture's have it's original boundary)
    this->addItem(man);

    // Setting the ball on it
    ball = new Ball(upperBound,lowerBound,leftBound,rightBound);
    QPixmap ba;
    ba.load("../img/ball.png");
    ba = ba.scaled(ba.width()/5 , ba.height()/5,Qt::KeepAspectRatio);
    ball->setPixmap(ba);
    ball->setPos(300,lowerBound);
    this->addItem(ball);

    // Set up timer to control each item
    timer = new QTimer(this);
    connect(timer , SIGNAL(timeout()) , this , SLOT(advance()));
    timer->start(10);
}
