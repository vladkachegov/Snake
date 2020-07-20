#ifndef SNAKECONTROLLER_H
#define SNAKECONTROLLER_H

#include <memory>
#include <QDebug>
#include <QObject>

#include "pathfinding.h"
#include "snake.h"

class SnakeController : public QObject
{
    Q_OBJECT
public:
    explicit SnakeController(QObject *parent = nullptr);

    ZoneMap get_map() const;
    void set_map(const ZoneMap &value);
    bool generate_snakes();
    std::vector<std::shared_ptr<Snake> > get_snakes() const;
    void prepare_controller();
    void refresh_map();
    ~SnakeController();
signals:
    void snakes_moved();
    void snake_stucked(int id);
    void snakes_collided();
public slots:
    void move_snakes();

private:
    bool validate_generated_pos(const std::vector<Node> &tails_and_heads);
    ZoneMap map;
    std::vector<std::shared_ptr<Snake>> snakes;
    int snake_count = 2;
//    Snake snake3;


};

#endif // SNAKECONTROLLER_H
