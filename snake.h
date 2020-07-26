#ifndef SNAKE_H
#define SNAKE_H
#include <vector>
#include "node.h"
#include <array>
#include <algorithm>
#include <memory>
#include <QDebug>

class Snake
{    
public:
    /** Snake movement direction struct*/
    enum SMD
    {
        TAIL,
        HEAD
    };
    /** Snake status enum
        OK => good to go
        COLLIDED => IS DEAD
    */
    enum SnakeStatus
    {
        OK,         // snake is good to go
        COLLIDED,   // snake has collided
        FRESH       // snake was just created and skips turn
    };

    Snake();
    /** Reset global snake id in case of new maze*/
    static void reset_global_id();
    /** Change movement direction to opposite*/
    void change_direction();
    /** Get current snake pos (tail to head)*/
    std::vector<Node> get_snake_pos() const;
    /** Set current snake pos (tail to head)*/
    void set_snake_pos(const std::vector<Node> &value);
    /** Move snake to new pos*/
    bool move(const Node &new_pos);
    /** Check if snake has collided with other snake and change their status*/
    void collides_with_other_snake(std::shared_ptr<Snake> snake2);
    /** Check if snake has collided with itself and change status*/
    void collides_with_itself();
    /** Get ALL positions snake could take on this step*/
    std::vector<Node> potential_positions(const std::array<Node,4> &dirs);
    /** Mark current snake as collided*/
    void set_collided();
    /** Mark current snake as good to go*/
    void set_ok();
    //operators
    bool operator==(const Snake &candidate);
    bool operator!=(const Snake &candidate);
    int get_id() const;
    /** Get position 1 BEFORE current movement direction*/
    Node get_prev_node();
    SMD dir = SMD::HEAD;
    ~Snake();
    void set_id(int value);
    SnakeStatus get_status() const;

private:
    std::vector<Node> snake_pos;
    SnakeStatus status = SnakeStatus::FRESH;
    int id;
    static int ID;

};

#endif // SNAKE_H
