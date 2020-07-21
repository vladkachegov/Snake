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
        for (auto snake : sc.get_snakes())
        {
            int node_number = 0;
            std::vector<QGraphicsRectItem*> prev_snake_pos;
            auto it = std::find_if(rects.begin(),rects.end(),[=](const std::pair<std::vector<QGraphicsRectItem*>,int> pair)
            {
                return (pair.second == snake->get_id());
            });
            prev_snake_pos = rects.at(it - rects.begin()).first;
            for (auto pos : snake->get_snake_pos())
            {
                auto rect = prev_snake_pos.at(node_number);
                auto p = rect->pos();
                auto p1 = (QPointF(pos.x*node_size, pos.y*node_size));
                rect->setPos(p1);
                ++node_number;
            }
        }
    });
    connect(&sc,&SnakeController::snakes_collided,&timer,&QTimer::stop);
    connect(&sc,&SnakeController::snake_collided,[=](const int &id)
    {
        sc.replace_snake(id);
        remove_snake_rects(id);

    });
    // gui setup

    free_brush.setColor(Qt::cyan);
    free_brush.setStyle(Qt::BrushStyle::SolidPattern);


    obstacle_brush.setColor(Qt::darkGray);
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

    generate_maze();

    // draw maze
    draw_objects();

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
    timer.start(500); // snake moves every msecond
}

void MainWindow::generate_maze()
{
    sc.prepare_controller();
    sc.set_map(ZoneMap(100,100));
    sc.generate_snakes();
}
void MainWindow::generate_snakes()
{
    sc.prepare_controller();
    sc.generate_snakes();
}

void MainWindow::on_maze_button_clicked()
{
    scene->clear();
    rects.clear();

    generate_maze();
    timer.stop();

    // draw maze
    draw_objects();


}

void MainWindow::on_snakes_button_clicked()
{
    scene->clear();
    rects.clear();

    generate_snakes();
    timer.stop();

    // draw maze
    draw_objects();

}

void MainWindow::remove_snake_rects(const int &id)
{
    auto it = std::find_if(rects.begin(),rects.end(),[=](const std::pair<std::vector<QGraphicsRectItem*>,int> pair)
    {
        return (pair.second == id);
    });
    int index = it - rects.begin();
    for (auto node : rects.at(index).first)
    {
        delete node;
    }
    rects.erase(rects.begin() + index);
}

void MainWindow::draw_objects()
{
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
        std::vector<QGraphicsRectItem *> snake_traj;
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

        rects.push_back({snake_traj,id});
    }
    //highlight heads and tails
    for (auto pos : rects)
    {
        pos.first.front()->setBrush(tail_brush);
        pos.first.back()->setBrush(head_brush);
    }
}
