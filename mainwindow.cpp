#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pathfinding.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QBrush free_brush;
    free_brush.setColor(Qt::cyan);
    free_brush.setStyle(Qt::BrushStyle::SolidPattern);
    QBrush obstacle_brush;
    obstacle_brush.setColor(Qt::black);
    obstacle_brush.setStyle(Qt::BrushStyle::SolidPattern);
    ui->setupUi(this);    
    ZoneMap map(100,100);
    auto view = ui->view;
    auto scene = new QGraphicsScene;
    int node_size = 8;
    foreach (auto node_vec, map.grid) {
        foreach (auto node, node_vec) {
            bool isObstacle = map.is_obstacle(node);
            QBrush brush = isObstacle ? obstacle_brush : free_brush;
            auto rect = scene->addRect(node.x*node_size,node.y*node_size,
                                       node_size,node_size,
                                       Qt::PenStyle::NoPen,
                                       brush);

        }
    }


    view->setScene(scene);

}

MainWindow::~MainWindow()
{
    delete ui;
}

