#ifndef SNAKE_H
#define SNAKE_H
#include <vector>
#include "node.h"
#include <array>
#include <algorithm>
enum class SMD // Snake Movement Direction
{
    TAIL,
    HEAD
};

class Snake
{    
public:
    Snake();
    std::vector<Node> get_snake_pos() const;
    void set_snake_pos(const std::vector<Node> &value);
    bool move(const Node &new_pos);
    std::vector<Node> potential_positions(const std::array<Node,4> &dirs);
    //operators
    bool operator==(const Snake &candidate);
    bool operator!=(const Snake &candidate);
    int get_id() const;



private:
    std::vector<Node> snake_pos;
    SMD dir = SMD::HEAD;
    int id;
    static int ID;

};

#endif // SNAKE_H
