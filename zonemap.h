#ifndef ZONEMAP_H
#define ZONEMAP_H
#include <vector>
#include "node.h"
#include <algorithm>
#include <snake.h>
#include <array>
#include <memory>
class ZoneMap
{
public:
    ZoneMap();
    ZoneMap(int _width, int _height):width(_width),height(_height)
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
    void set_obstacles(const std::vector<Node> &new_obstacles)
    {
        obstacles = new_obstacles;
    }

    /** Check the node to be an obstacle.
     * If snake arg passed - check for collisions with other snakes too
        returns true is the node is an obstacle*/
    bool is_obstacle(const Node &node)
    {
        auto obstacle_it = std::find(obstacles.begin(),obstacles.end(),node);
        bool isObstacle = !(obstacle_it == obstacles.end());                    // check obstacles
        bool collidesWithOtherSnake = collides_with_snakes(node);
        if (isObstacle || collidesWithOtherSnake)
        {
            return true;
        }else
        {
            return false;
        }
    }
    /** Check the node to be an other snake. */
    bool collides_with_snakes(const Node &node)
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
    bool is_out_of_bounds(const Node &node)
    {
        return !((node.x>=0 && node.x < width)&&(node.y>=0 && node.y < height));
    }
    std::vector<Node> get_neighbours(const Node &node)
    {
        std::vector<Node> result;
        for (auto dir : DIRS)
        {
            int x = node.x + dir.x;
            int y = node.y + dir.y;
            Node neighbour(x,y);
            bool obstacle = is_obstacle(neighbour); // is obstacle
            bool OOB = is_out_of_bounds(neighbour); // is out of bounds
            if (!obstacle && !OOB)
            {
                result.push_back(neighbour);
            }
        }
        return result;
    }

    void add_snake(std::shared_ptr<Snake> new_snake)
    {
        snakes_on_map.push_back(new_snake);
    }

    std::vector<std::vector<Node> > get_grid() const;

    std::vector<Node> get_obstacles() const;

private:
    int width = 100;
    int height = 100;
    int number_of_obstacles = 300;
    std::vector<std::vector<Node>> grid;
    std::vector<Node> obstacles;
    std::vector<std::shared_ptr<Snake>> snakes_on_map;
    static std::array<Node,4> DIRS;
};

#endif // ZONEMAP_H







