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
            auto it = std::find_if(rects.begin(),rects.end(),[=](const std::pair<std::vector<QGraphicsRectItem*>,int> pair)
            {
                return (pair.second == snake->get_id());
            });

            int index = it - rects.begin();
            std::vector<QGraphicsRectItem*> prev_snake_pos;
            prev_snake_pos = rects.at(index).first;
            int node_number = 0;
            for (auto pos : snake->get_snake_pos())
            {
                auto rect = prev_snake_pos.at(node_number);                
                auto p1 = (QPointF(pos.x*node_size, pos.y*node_size));
                rect->setPos(p1);
                ++node_number;
            }
        }
    });
    connect(&sc,&SnakeController::snakes_collided,
            this,&MainWindow::generate_snakes);
    connect(&sc,&SnakeController::snake_collided,[=](const int &id)
    {
        collided_snakes_id.push_back(id);
    });
    connect(&sc,&SnakeController::snake_new_pos_validated,
            this,
            &MainWindow::clear_collided_snakes);
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
    timer.start(timer_tick); // snake moves every msecond
}

void MainWindow::generate_maze()
{
    sc.prepare_controller();
    sc.set_map(ZoneMap(100,100));
    sc.generate_snakes();
}

void MainWindow::generate_snakes()
{
//    timer.stop();
    for (auto snake : sc.get_snakes())
    {
        remove_snake_rects(snake->get_id());
    }
    sc.prepare_controller();
    sc.generate_snakes();
    for (auto snake : sc.get_snakes())
    {
        draw_new_snake(snake->get_id());
    }
}

void MainWindow::on_maze_button_clicked()
{
    timer.stop();
    scene->clear();
    rects.clear();
    generate_maze();
    // draw maze
    draw_objects();
    timer.start(timer_tick);


}

void MainWindow::clear_collided_snakes()
{
    if (!collided_snakes_id.empty())
    {
        for (auto snake_id : collided_snakes_id)
        {
            remove_snake_rects(snake_id);   // delete prev. rects
            sc.replace_snake(snake_id);     // generate new snake with same id
            draw_new_snake(snake_id);       // draw new rects
        }
        collided_snakes_id.clear();
    }
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

void MainWindow::draw_new_snake(const int &id)
{
    auto snakes= sc.get_snakes();
    auto snake_it = std::find_if(snakes.begin(),
                                 snakes.end(),
                                 [&](const std::shared_ptr<Snake> &snake)
    {
        return (snake->get_id() == id);
    });
    std::vector<QGraphicsRectItem *> snake_traj;
    for (auto node : (*snake_it)->get_snake_pos())
    {
        QGraphicsRectItem *ra = new QGraphicsRectItem(0,0,node_size,node_size);
        ra->setPos(node.x*node_size,node.y*node_size);
        QBrush brush = id == 1 ? snake1_brush : snake2_brush;
        ra->setPen(Qt::PenStyle::NoPen);
        ra->setBrush(brush);
        scene->addItem(ra);
        snake_traj.push_back(ra);
    }
    snake_traj.front()->setBrush(tail_brush);
    snake_traj.back()->setBrush(head_brush);
    rects.push_back({snake_traj,id});
}

void MainWindow::draw_map()
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
}

void MainWindow::draw_objects()
{
    draw_map();
    for (auto snake : sc.get_snakes())
    {
        draw_new_snake(snake->get_id());
    }
}

void MainWindow::on_export_button_clicked()
{

}
