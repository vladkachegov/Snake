#include "zonemap.h"

std::array<Node,4> ZoneMap::DIRS = {Node(-1,0),  //left
                                    Node(1,0),  //right
                                    Node(0,1),  // top
                                    Node(0,-1)}; //bottom

ZoneMap::ZoneMap()
{

}

ZoneMap::ZoneMap(int _width, int _height):width(_width),height(_height)
{
    for (int row = 0;row<height;++row)
    {
        std::vector<Node> new_row;
        for (int column = 0; column < width; ++column)
        {
            Node node(column,row);
            new_row.push_back(node);
        }
        grid.push_back(new_row);
    }
    for (int ii=0;ii<number_of_obstacles;)
    {
        int x = rand() % 100;
        int y = rand() % 100;
        Node obstacle_node(x,y);
        auto find_res = std::find(obstacles.begin(),obstacles.end(),obstacle_node);
        bool already_exists = !(find_res == obstacles.end());
        if (!already_exists)
        {
            obstacles.push_back(obstacle_node);
            ++ii;
        }
    }
}

void ZoneMap::set_obstacles(const std::vector<Node> &new_obstacles)
{
    obstacles = new_obstacles;
}

bool ZoneMap::is_obstacle(const Node &node)
{
    auto obstacle_it = std::find(obstacles.begin(),obstacles.end(),node);
    bool isObstacle = !(obstacle_it == obstacles.end());
    if (isObstacle)
    {
        return true;
    }else
    {
        return false;
    }
}

bool ZoneMap::collides_with_snakes(const Node &node)
{
    for (auto s : snakes_on_map)
    {
        for (auto n : s->get_snake_pos())
        {
            if (node.is_equal_to(n))
            {
                return true;
            }
        }

    }
    return false;
}

bool ZoneMap::is_out_of_bounds(const Node &node)
{
    return !((node.x>=0 && node.x < width)&&(node.y>=0 && node.y < height));
}

std::vector<Node> ZoneMap::get_neighbours(const Node &node, std::shared_ptr<Snake> snake)
{
    std::vector<Node> result;
    for (auto dir : DIRS)
    {
        int x = node.x + dir.x;
        int y = node.y + dir.y;
        Node neighbour(x,y); // potential neighbour
        bool collides_with_snake = false;
        if (snake != nullptr)
        {
            // check for snake move
            auto snake_pos = snake->get_snake_pos();
            auto iter = std::find(snake_pos.begin(),snake_pos.end(),neighbour);
            // we can go to new pos only if it is NOT PREVIOUS POSITION!
            bool not_found = (iter == snake_pos.end()
                              ||
                              ( (iter!=snake_pos.end()) && ((*iter)!=snake->get_prev_node()) )
                              ); // check new pos to be anything but previous position
            collides_with_snake = !not_found;
        }else
        {   // check for pathfinding
            collides_with_snake = collides_with_snakes(node);
        }
        bool obstacle = is_obstacle(neighbour); // is obstacle
        bool OOB = is_out_of_bounds(neighbour); // is out of bounds
        if (!obstacle && !OOB && !collides_with_snake)
        {
            result.push_back(neighbour);
        }
    }
    return result;
}

void ZoneMap::add_snake(std::shared_ptr<Snake> new_snake)
{
    snakes_on_map.push_back(new_snake);
}

void ZoneMap::remove_snake(const int &id)
{
    auto it = std::find_if(snakes_on_map.begin(),snakes_on_map.end(),[=](const std::shared_ptr<Snake> snake)
    {
        return (snake->get_id() == id);
    });
    int snake_index = (it - snakes_on_map.begin()); // index is always the same in ANY vector!!!
    snakes_on_map.erase(snakes_on_map.begin() + snake_index);
}

void ZoneMap::add_snakes(const std::vector<std::shared_ptr<Snake> > &new_snakes)
{
    snakes_on_map = new_snakes;
}

std::vector<std::vector<Node> > ZoneMap::get_grid() const
{
    return grid;
}

std::vector<Node> ZoneMap::get_obstacles() const
{
    return obstacles;
}

void ZoneMap::clear_snakes()
{
    snakes_on_map.clear();
}
