#include "snakecontroller.h"
#include <QThread>
SnakeController::SnakeController(QObject *parent) : QObject(parent)
{
    connect(&save_thread, &QThread::started, &sa, &SqlAdapter::run);
    connect(&sa, &SqlAdapter::finished, &save_thread, &QThread::quit);
    sa.moveToThread(&save_thread);
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
    for (int id = 1; id <= snake_count; ++id)
    {
        generate_snake(id);
    }
}

void SnakeController::generate_snake(const int &id)
{
    bool success = false;
    while (!success)
    {
        std::pair<Node,Node> limbs;
        while (true)
        {
            for (int ii = 0; ii<2;++ii)
            {
                // generating snake pos
                int x = rand() % 100;
                int y = rand() % 100;
                Node node(x,y);      // snake limb
                if (ii == 0)
                {
                    limbs.first = node; // save as tail
                }else
                {
                    limbs.second = node; // save as head
                }
            }
            bool pos_ok = vgp(limbs);
            if (pos_ok)
            {
                break;
            }
        }
        bool path_found = false;
        auto snake_pos = Pathfinding::find_path(map, limbs, path_found);
        if (path_found)
        {
            auto snake = std::make_shared<Snake>();
            snake->set_id(id);
            snake->set_snake_pos(snake_pos);
            snakes.push_back(snake);
            map.add_snake(snake);
            success = path_found;
        }
        qDebug() << "lfp";
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
    auto it = std::find_if(snakes.begin(),snakes.end(),[=](const std::shared_ptr<Snake> snake)
    {
        return (snake->get_id() == id);
    });
    int snake_index = (it - snakes.begin()); // index is always the same in ANY vector!!!
    snakes.erase(snakes.begin() + snake_index);
    map.remove_snake(id);
}


void SnakeController::move_snakes()
{
    qDebug() << "moved!";
    prepare_for_next_move(); // replace collided snakes
    for (auto it = snakes.begin();it<snakes.end();)
    {
        // new snakes skip turn
        if ((*it)->get_status() == Snake::SnakeStatus::FRESH)
        {
            (*it)->set_ok();
            ++it;
        }else
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
    }
    collision_check(); // check snakes for collision
    emit snakes_moved();
}

void SnakeController::save_to_sqlite()
{
    sa.prepare_entities(map,snakes);
    save_thread.start();
    qDebug() << "saving";
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
bool SnakeController::vgp(const std::pair<Node,Node> &tail_and_head)
{
    if (tail_and_head.first.is_equal_to(tail_and_head.second))
    {
        return false;
    }else
    {
        bool tail_is_obstacle = map.is_obstacle(tail_and_head.first);
        bool head_is_obstacle = map.is_obstacle(tail_and_head.second);
        bool tail_is_snake = map.collides_with_snakes(tail_and_head.first);
        bool head_is_snake = map.collides_with_snakes(tail_and_head.second);
        bool tail_is_bad = (tail_is_obstacle || tail_is_snake);
        bool head_is_bad = (head_is_obstacle || head_is_snake);

        bool tail_is_stuck = (map.get_neighbours(tail_and_head.first).size()==1);
        bool head_is_stuck = (map.get_neighbours(tail_and_head.second).size()==1);
        bool ok = (!tail_is_bad &&
                   !head_is_bad &&
                   (!tail_is_stuck && !head_is_stuck));
        return ok;
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
