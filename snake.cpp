#include "snake.h"

int Snake::ID = 1;

Snake::Snake()
{
    id = ID;
    ++ID;
}

void Snake::change_direction()
{
    dir = dir == HEAD ? TAIL : HEAD; // change movement direction to opposite
}

std::vector<Node> Snake::get_snake_pos() const
{
    return snake_pos;
}

void Snake::set_snake_pos(const std::vector<Node> &value)
{    
    snake_pos = value;
}

bool Snake::move(const Node &new_pos)
{
    switch (dir)
    {
    case (SMD::TAIL):
    {
        snake_pos.erase(snake_pos.end() - 1);                   // remove first element. now snake is fine, but needs new tail!
        auto tail = new_pos;                      // prev. tail pos
        snake_pos.insert(snake_pos.begin(),tail);           // add tail to snake pos
        return true;
    }
    case (SMD::HEAD):
    {
        auto head = new_pos;                     // prev. head pos
        snake_pos.erase(snake_pos.begin());                 // remove last element. now snake is fine, but needs new head!
        snake_pos.push_back(head);                          // add head to snake pos
        return true;
    }
    }
}
/** Get potential positions of the snake*/
std::vector<Node> Snake::potential_positions(const std::array<Node, 4> &dirs)
{
    std::vector<Node> pot_pos;
    Node leading_node;
    switch(dir)
    {
    case(SMD::HEAD):
    {
        leading_node = snake_pos.back();
        break;
    }
    case(SMD::TAIL):
    {
        leading_node = snake_pos.front();
        break;
    }
    }

    for (auto dir_node : dirs)
    {
        Node candidate(leading_node.x + dir_node.x,
                       leading_node.y + dir_node.y);
        auto it = std::find(snake_pos.begin(),snake_pos.end(),candidate);
        if (it == snake_pos.end())
        {
            pot_pos.push_back(candidate);
        }
    }
    return pot_pos;
}

bool Snake::operator==(const Snake &candidate)
{
    return (this->id == candidate.id);
}

bool Snake::operator!=(const Snake &candidate)
{
    return (this->id != candidate.id);
}

int Snake::get_id() const
{
    return id;
}

