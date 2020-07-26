#include "snakecontroller.h"
#include <QThread>
SnakeController::SnakeController(QObject *parent) : QObject(parent)
{

}

ZoneMap SnakeController::get_map() const
{
    return map;
}

void SnakeController::set_map(const ZoneMap &value)
{
    map = value;
}

bool SnakeController::generate_snakes()
{
    bool success = false;
    map.clear_snakes();
    while (!success)
    {
        Node snake_limb;
        int emergency_iterator = 0;
        std::vector<Node> generated_pos;

        while (true)
        {
            for (int ii = 0; ii<snake_count*2;++ii)
            {
                // generating snake pos
                int x = rand() % 100;     // snake tail
                int y = rand() % 100;     // snake tail
                snake_limb = Node(x,y);      // snake tail
                generated_pos.push_back(snake_limb);
            }
            bool pos_ok = validate_generated_pos(generated_pos);
            if (pos_ok)
            {
                break;
            }else
            {
                generated_pos.clear();
            }
            if (emergency_iterator == 100)
            {
                return false;
            }
            ++emergency_iterator;
            qDebug() << emergency_iterator;
        }
        for (int ii = 0;ii<(int)generated_pos.size();ii+=2)
        {
            auto snake = std::make_shared<Snake>();
            bool path_found = false;
            snake->set_snake_pos(Pathfinding::find_path(map,
                                                        generated_pos.at(ii),
                                                        generated_pos.at(ii+1),
                                                        path_found));
            if (path_found)
            {
                snakes.push_back(snake);
                map.add_snake(snake);
                success = path_found;
            }else
            {
                success = path_found;
                break;
            }
        }
        if (!success)
        {
            snakes.clear();
            map.clear_snakes();
        }
    }
    return success;
}

void SnakeController::generate_snake(const int &id)
{
    bool success = false;
    while (!success)
    {
        Node node;
        std::vector<Node> generated_pos;
        while (true)
        {
            for (int ii = 0; ii<2;++ii)
            {
                // generating snake pos
                int x = rand() % 100;     // snake tail
                int y = rand() % 100;     // snake tail
                node = Node(x,y);      // snake tail
                generated_pos.push_back(node);
            }
            bool pos_ok = validate_generated_pos(generated_pos);
            if (pos_ok)
            {
                break;
            }else
            {
                generated_pos.clear();
            }
        }
        auto snake = std::make_shared<Snake>();
        snake->set_id(id);
        bool path_found = false;
        snake->set_snake_pos(Pathfinding::find_path(map,
                                                    generated_pos.at(0),
                                                    generated_pos.at(1),
                                                    path_found));
        if (path_found)
        {
            snakes.push_back(snake);
            map.add_snake(snake);
            success = path_found;
        }
    }
}

void SnakeController::replace_snake(const int &id)
{
    remove_snake(id);           // delete old snake from everywhere
    generate_snake(id);         // create new one in new place
    emit snake_replaced(id);
}

void SnakeController::remove_snake(const int &id)
{
    int snake_index = 0;

    auto it = std::find_if(snakes.begin(),snakes.end(),[=](const std::shared_ptr<Snake> snake)
    {
        return (snake->get_id() == id);
    });
    snake_index = (it - snakes.begin()); // index is always the same in ANY vector!!!
    snakes.erase(snakes.begin() + snake_index);
    map.remove_snake(id);
}


void SnakeController::move_snakes()
{
    prepare_for_next_move();
    for (auto it = snakes.begin();it<snakes.end();)
    {
        auto snake = *it;
        std::vector<Node> dirs;
        if (snake->dir == Snake::HEAD)
        {
            dirs = map.get_neighbours(snake->get_snake_pos().back(),snake);
        }else
        {
            dirs = map.get_neighbours(snake->get_snake_pos().front(),snake);
        }

        if (!dirs.empty())
        {
            int d = 0;
            if (dirs.size() != 1)
            {
                d = rand() % (dirs.size());
            }
            auto dir = dirs.at(d);
            snake->move(dir);
            ++it;
        }else
        {
            snake->change_direction(); // change direction and skip this step
        }
    }    
    collision_check();
    emit snakes_moved();
}

void SnakeController::prepare_for_next_move()
{
    std::vector<int> collided_snakes;
    for (auto s : snakes)
    {
        if (s->get_status() == Snake::SnakeStatus::COLLIDED)
        {
            collided_snakes.push_back(s->get_id());
        }
    }
    for (auto id : collided_snakes)
    {
        replace_snake(id);
    }
}

void SnakeController::add_to_collided(const int &id)
{

}

void SnakeController::collision_check()
{
    // check for collision between multiple snakes
    for (auto it = snakes.begin();it < (snakes.end() - 1);++it)
    {
        for (auto it2 = it+1;it2 < snakes.end(); ++it2)
        {
            (*it)->collides_with_other_snake((*it2));
        }
    }
    // check for collision in single snake (if such coll. exists => send snake id to view)
    for (auto snake : snakes)
    {
        if(snake->get_status() != Snake::SnakeStatus::COLLIDED)
        {
            snake->collides_with_itself();
        }
    }
    // at this moment all the snakes were checked and have appropriate
}

bool SnakeController::validate_generated_pos(const std::vector<Node> &tails_and_heads)
{
    // check for equality
    for (auto elem_it  = tails_and_heads.begin();elem_it < tails_and_heads.end() - 1;++elem_it)
    {
        // check [first:last) to be an obstacle
        if (map.is_obstacle(*elem_it) || map.collides_with_snakes(*elem_it))
        {
            return false;
        }
        for (auto eq_it = elem_it+1;eq_it < tails_and_heads.end();++eq_it)
        {
            if ((*elem_it).is_equal_to(*eq_it))
            {
                return false;
            }
        }
    }
    // check {last} to be an obstacle
    return !map.is_obstacle(tails_and_heads.back());
}

bool SnakeController::validate_gp(const Node &node)
{
    if (map.is_obstacle(node))
    {
        return false;
    }else
    {
        for (auto snake : snakes)
        {
            auto it = std::find(snake->get_snake_pos().begin(),
                                snake->get_snake_pos().end(),
                                node);
            if (it!=snake->get_snake_pos().end())
            {
                return false;
            }
        }
        return true;
    }
}




std::vector<std::shared_ptr<Snake> > SnakeController::get_snakes() const
{
    return snakes;
}



void SnakeController::prepare_controller()
{
    snakes.clear();
    map.clear_snakes();
    Snake::reset_global_id();

}

void SnakeController::refresh_map()
{
    map = ZoneMap(100,100);
}

SnakeController::~SnakeController()
{
    qDebug() << "SNAKE CONTROLLER IS DEAD!";
}
