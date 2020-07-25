#include "snake.h"

int Snake::ID = 1;

Snake::Snake()
{
    id = ID;
    ++ID;
}

void Snake::reset_global_id()
{
    ID = 1;
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

void Snake::collides_with_other_snake(std::shared_ptr<Snake> snake2)
{
    for (auto node : snake2->get_snake_pos())
    {
        auto it = std::find(snake_pos.begin(),snake_pos.end(),node);
        if (it != snake_pos.end())
        {
            status = SnakeStatus::COLLIDED;
            snake2->set_collided();
            break;
        }
    }
}

void Snake::collides_with_itself()
{
    for (auto iter = snake_pos.begin();iter < snake_pos.end()-1;++iter)
    {
        auto it = std::find(iter+1,snake_pos.end(),*iter);
        if (it != snake_pos.end())
        {
            status = SnakeStatus::COLLIDED;
        }
    }
}

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

void Snake::set_collided()
{
    status = SnakeStatus::COLLIDED;
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

Node Snake::get_prev_node()
{
    switch (dir)
    {
    case (SMD::HEAD):
    {
        auto it = snake_pos.end();
        Node prev_node = *(it -2 ); // get 1 element BEFORE HEAD
        return (prev_node);
    }
    case (SMD::TAIL):
    {
        auto it = snake_pos.begin();
        Node prev_node = *(it+1); // get 1 element AFTER tail
        return (prev_node);
    }

    }
}

Snake::~Snake()
{
//    --ID;
    qDebug()<< "Snake is dead!";

}

void Snake::set_id(int value)
{
    id = value;
}

Snake::SnakeStatus Snake::get_status() const
{
    return status;
}

