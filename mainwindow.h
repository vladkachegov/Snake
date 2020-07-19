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

private:
    int node_size = 8;
    Ui::MainWindow *ui;
    SnakeController sc;
    QVector<QVector<QGraphicsRectItem*>> rects;
    void generate_snakes();
    QTimer timer;
};
#endif // MAINWINDOW_H
