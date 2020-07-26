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
    enum OBSTACLE_CHECK_MODE
    {
        OBSTACLE_ONLY,
        OBSTACLE_PLUS_SNAKES
    };
    ZoneMap();
    ZoneMap(int _width, int _height);
    void set_obstacles(const std::vector<Node> &new_obstacles);
    /** Check the node to be an obstacle.*/
    bool is_obstacle(const Node &node);
    /** Check the node to be an other snake. */
    bool collides_with_snakes(const Node &node);
    bool is_out_of_bounds(const Node &node);
    std::vector<Node> get_neighbours(const Node &node, std::shared_ptr<Snake> snake = nullptr);
    void add_snake(std::shared_ptr<Snake> new_snake);
    void remove_snake(const int &id);
    void add_snakes(const std::vector<std::shared_ptr<Snake>> &new_snakes);
    std::vector<std::vector<Node> > get_grid() const;
    std::vector<Node> get_obstacles() const;
    void clear_snakes();

private:
    int width = 100;
    int height = 100;
    int number_of_obstacles = 3000;
    std::vector<std::vector<Node>> grid;
    std::vector<Node> obstacles;
    std::vector<std::shared_ptr<Snake>> snakes_on_map;
    static std::array<Node,4> DIRS;
};

#endif // ZONEMAP_H








