#include "snakecontroller.h"

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
    map.clear_snakes();
    Node s_tail;
    int emergency_iterator = 0;
    std::vector<Node> generated_pos;
    while (true)
    {
        for (int ii = 0; ii<snake_count*2;++ii)
        {
            // generating snake pos
            int x = rand() % 100;     // snake tail
            int y = rand() % 100;     // snake tail
            s_tail = Node(x,y);      // snake tail
            generated_pos.push_back(s_tail);
        }
        //        generated_pos.push_back(Node(10,10));
        //        generated_pos.push_back(Node(10,50));
        //        generated_pos.push_back(Node(90,90));
        //        generated_pos.push_back(Node(45,45));
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

    //    // debug only BEGIN

    //    generated_pos.clear();
    //    generated_pos.push_back(Node(0,40));
    //    generated_pos.push_back(Node(60,40));
    //    generated_pos.push_back(Node(30,10));
    //    generated_pos.push_back(Node(30,80));
    //    // debug only END
    for (int ii = 0;ii<(int)generated_pos.size();ii+=2)
    {
        auto snake = std::make_shared<Snake>();
        snake->set_snake_pos(Pathfinding::find_path(map,
                                                    generated_pos.at(ii),
                                                    generated_pos.at(ii+1)));
        snakes.push_back(snake);
        map.add_snake(snake);
    }
    return true;
}


void SnakeController::move_snakes()
{
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
            snake->change_direction();
        }
    }
    emit snakes_moved();
}
/** Validation of generated tails and heads.
 * If at least 2 are equal => BAD
    If any is an obstacle => BAD
    Else ==> GOOD
*/
bool SnakeController::validate_generated_pos(const std::vector<Node> &tails_and_heads)
{
    // check for equality
    for (auto elem_it  = tails_and_heads.begin();elem_it < tails_and_heads.end() - 1;++elem_it)
    {
        // check [first:last) to be an obstacle
        if (map.is_obstacle(*elem_it))
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

std::vector<std::shared_ptr<Snake> > SnakeController::get_snakes() const
{
    return snakes;
}

void SnakeController::prepare_controller()
{
    snakes.clear();
}

void SnakeController::refresh_map()
{
    map = ZoneMap(100,100);
}

SnakeController::~SnakeController()
{
    qDebug() << "SNAKE CONTROLLER IS DEAD!";
}
