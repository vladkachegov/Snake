#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pathfinding.h"
#include <QGraphicsView>
#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    connect(&timer,&QTimer::timeout,&sc,&SnakeController::move_snakes);
    connect(&sc,&SnakeController::snakes_moved,[=]()
    {
        int snake_number = 0;
        for (auto snake : sc.get_snakes())
        {
            int node_number = 0;
            QVector<QGraphicsRectItem*> prev_snake_pos = rects.at(snake_number);
            for (auto pos : snake->get_snake_pos())
            {
                auto rect = prev_snake_pos.at(node_number);
                auto p = rect->pos();
                auto p1 = (QPointF(pos.x*node_size, pos.y*node_size));
                rect->setPos(p1);
                ++node_number;
            }
            ++snake_number;
        }
    });
    // gui setup

    free_brush.setColor(Qt::cyan);
    free_brush.setStyle(Qt::BrushStyle::SolidPattern);


    obstacle_brush.setColor(Qt::black);
    obstacle_brush.setStyle(Qt::BrushStyle::SolidPattern);


    snake1_brush.setColor(Qt::GlobalColor::yellow);
    snake1_brush.setStyle(Qt::BrushStyle::SolidPattern);


    snake2_brush.setColor(Qt::GlobalColor::red);
    snake2_brush.setStyle(Qt::BrushStyle::SolidPattern);

    head_brush.setColor(Qt::GlobalColor::blue);
    head_brush.setStyle(Qt::BrushStyle::SolidPattern);

    tail_brush.setColor(Qt::GlobalColor::darkGreen);
    tail_brush.setStyle(Qt::BrushStyle::SolidPattern);

    node_size = 8;  // single node size

    ui->setupUi(this);
    auto view = ui->view;
    scene = new QGraphicsScene;

    generate_snakes();

    // draw maze
    foreach (auto node_vec, sc.get_map().get_grid()) {
        foreach (auto node, node_vec) {
            bool isObstacle = sc.get_map().is_obstacle(node,ZoneMap::OBSTACLE_ONLY);
            QBrush brush = isObstacle ? obstacle_brush : free_brush;
            auto rect = scene->addRect(node.x*node_size,node.y*node_size,
                                       node_size,node_size,
                                       Qt::PenStyle::NoPen,
                                       brush);

        }
    }

    auto snakes = sc.get_snakes();
    // draw snakes
    for (auto snake : snakes)
    {
        int id = snake->get_id();
        QVector<QGraphicsRectItem *> snake_traj;
        for (auto node : snake->get_snake_pos())
        {

            QGraphicsRectItem *ra = new QGraphicsRectItem(0,0,node_size,node_size);
            ra->setPos(node.x*node_size,node.y*node_size);
            QBrush brush = id == 1 ? snake1_brush : snake2_brush;
            ra->setPen(Qt::PenStyle::NoPen);
            ra->setBrush(brush);
            scene->addItem(ra);
            snake_traj.push_back(ra);
        }
        rects.push_back(snake_traj);
    }
    //highlight heads and tails
    for (auto pos : rects)
    {
        pos.first()->setBrush(tail_brush);
        pos.last()->setBrush(head_brush);
    }

//    // controller setup
//    generate_snakes(); // generate map snakes and obstacles
    view->setScene(scene);
}

MainWindow::~MainWindow()
{

    delete ui;
}


void MainWindow::on_start_moving_clicked()
{
    timer.start(1); // snake moves every second
}

void MainWindow::generate_snakes()
{
    sc.prepare_controller();
    sc.set_map(ZoneMap(100,100));
    sc.generate_snakes();
}

void MainWindow::on_maze_button_clicked()
{


}
