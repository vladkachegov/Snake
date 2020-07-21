#ifndef SNAKE_H
#define SNAKE_H
#include <vector>
#include "node.h"
#include <array>
#include <algorithm>
#include <memory>

class Snake
{    
public:
    enum SMD // Snake Movement Direction
    {
        TAIL,
        HEAD
    };
    Snake();
    void change_direction();
    std::vector<Node> get_snake_pos() const;
    void set_snake_pos(const std::vector<Node> &value);
    bool move(const Node &new_pos);
    bool collides_with_other_snake(std::shared_ptr<Snake> snake2);
    std::vector<Node> potential_positions(const std::array<Node,4> &dirs);
    //operators
    bool operator==(const Snake &candidate);
    bool operator!=(const Snake &candidate);
    int get_id() const;
    SMD dir = HEAD;


private:
    std::vector<Node> snake_pos;

    int id;
    static int ID;

};

#endif // SNAKE_H
