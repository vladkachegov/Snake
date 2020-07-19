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
            for (auto pos : snake->get_snake_pos())
            {
                auto rect = rects.at(snake_number).at(node_number);
                auto rp = rect->pos();
                //                rect->setPos(rect->mapToScene(pos.x,pos.y));
                ++node_number;
            }
            ++snake_number;
        }
    });
    // gui setup
    QBrush free_brush;
    free_brush.setColor(Qt::cyan);
    free_brush.setStyle(Qt::BrushStyle::SolidPattern);

    QBrush obstacle_brush;
    obstacle_brush.setColor(Qt::black);
    obstacle_brush.setStyle(Qt::BrushStyle::SolidPattern);

    QBrush snake1_brush;
    snake1_brush.setColor(Qt::GlobalColor::yellow);
    snake1_brush.setStyle(Qt::BrushStyle::SolidPattern);

    QBrush snake2_brush;
    snake2_brush.setColor(Qt::GlobalColor::red);
    snake2_brush.setStyle(Qt::BrushStyle::SolidPattern);

    node_size = 8;  // single node size

    ui->setupUi(this);
    auto view = ui->view;
    auto scene = new QGraphicsScene;

    // controller setup
    generate_snakes();
    auto snakes = sc.get_snakes();
    // draw grid
    foreach (auto node_vec, sc.get_map().get_grid()) {
        foreach (auto node, node_vec) {
            bool isObstacle = sc.get_map().is_obstacle(node);
            QBrush brush = isObstacle ? obstacle_brush : free_brush;
            auto rect = scene->addRect(node.x*node_size,node.y*node_size,
                                       node_size,node_size,
                                       Qt::PenStyle::NoPen,
                                       brush);

        }
    }
    for (auto snake : snakes)
    {
        int id = snake->get_id();
        QVector<QGraphicsRectItem *> snake_traj;
        for (auto node : snake->get_snake_pos())
        {

            QBrush brush = id == 1 ? snake1_brush : snake2_brush;
            QGraphicsRectItem *rect = scene->addRect(node.x*node_size,node.y*node_size,
                                       node_size,node_size,
                                       Qt::PenStyle::NoPen,
                                       brush);
            snake_traj.push_back(rect);
        }
        for (auto a : snake_traj)
        {
            qDebug() << a->x();
        }
        rects.push_back(snake_traj);
    }



    view->setScene(scene);


}

MainWindow::~MainWindow()
{

    delete ui;
}


void MainWindow::on_start_moving_clicked()
{
    timer.start(1000); // snake moves every second
}

void MainWindow::generate_snakes()
{
    sc.prepare_controller();
    sc.set_map(ZoneMap(100,100));
    sc.generate_snakes();
}
