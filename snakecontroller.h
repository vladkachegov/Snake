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
    /** Generate 2 snakes. Snake::ID must be reset before func. call*/
    bool generate_snakes();
    /** Replace collided snake with the new one and keep the same snake id. */
    void replace_snake(const int &id);
    std::vector<std::shared_ptr<Snake> > get_snakes() const;
    /** Reset controller to original state and keep map */
    void prepare_controller();
    void refresh_map();
    ~SnakeController();
signals:
    /** Notify view when all snakes were moved */
    void snakes_moved();
public slots:
    /** Move snakes as view demands it*/
    void move_snakes();

private:
    /** Mark snake as "COLLIDED" to delete later*/
    void add_to_collided(const int &id);
    /** Generate snake with same id, as her dead parent*/
    bool generate_snake(const int &id);
    /** Remove snake from controller and from map). */
    void remove_snake(const int &id);
    /** Check snakes for any kind of collision */
    void collision_check();
    /** Validation of generated tails and heads.
     * If at least 2 are equal => BAD
        If any is an obstacle => BAD
        Else ==> GOOD
    */
    bool validate_generated_pos(const std::vector<Node> &tails_and_heads);
    ZoneMap map;
    std::vector<std::shared_ptr<Snake>> snakes;
    int snake_count = 1;    
    std::vector<int> collided_snakes;
    bool validate_gp(const Node &node);
};

#endif // SNAKECONTROLLER_H
