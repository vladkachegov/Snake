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
public slots:
    /** Update snake representation with given id*/
    void update_snakes(int id);
private slots:
    /** Start timer*/
    void on_start_moving_clicked();
    /** Create new maze and snakes*/
    void on_maze_button_clicked();
private:
    // gui members
    /** Draw single new snake in scene*/
    void draw_new_snake(const int &id = 0);
    /** Draw new map in scene*/
    void draw_map();
    /** Remove deleted snake rects from scene*/
    void remove_snake_rects(const int &id);
    /** Draw maze + both snakes (used on init.)*/
    void draw_objects();
    /** Tells controller to generate new maze + snakes*/
    void generate_maze();
   /** Tells controller to generate new snakes in the same maze*/
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
    int timer_tick = 1000;
    // model members
    SnakeController sc;
    std::vector<std::pair<std::vector<QGraphicsRectItem*>,int >> rects;


};
#endif // MAINWINDOW_H
