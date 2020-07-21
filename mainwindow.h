#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "snakecontroller.h"
#include <QGraphicsRectItem>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_start_moving_clicked();

    void on_maze_button_clicked();

    void on_snakes_button_clicked();

private:
    // gui members
    void remove_snake_rects(const int &id);;
    void draw_objects();
    void generate_maze();
    void generate_snakes();

    Ui::MainWindow *ui;
    QGraphicsScene* scene;

    QBrush free_brush;
    QBrush obstacle_brush;
    QBrush snake1_brush;
    QBrush snake2_brush;
    QBrush head_brush;
    QBrush tail_brush;
    int node_size = 8;

    QTimer timer;
    // model members

    SnakeController sc;
    std::vector<std::pair<std::vector<QGraphicsRectItem*>,int >> rects;

};
#endif // MAINWINDOW_H
