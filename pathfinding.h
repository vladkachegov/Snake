#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <array>
#include <vector>
#include <algorithm>
#include <unordered_map>
struct Node
{
    Node(){}
    Node(int _x,int _y):x(_x),y(_y)
    {

    }
    int x = 0;
    int y = 0;

    bool operator==(const Node &node)
    {
        return ((x==node.x)&(y==node.y));
    }
    bool operator!=(const Node &node)
    {
        return ((x!=node.x)||(y!=node.y));
    }

};
bool operator==(const Node &node1,const Node &node2);

// hash function for unordered map
namespace std {
template<> struct hash<Node> {
  std::size_t operator()(const Node& id) const noexcept {
    return std::hash<int>()(id.x ^ (id.y << 4));
  }
};
}
struct ZoneMap
{
    // Generate map on initialization
    ZoneMap(){};
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
//        for (int ii=0;ii<number_of_obstacles;)
//        {
//            int x = rand() % 100;
//            int y = rand() % 100;
//            Node obstacle_node(x,y);
//            auto find_res = std::find(obstacles.begin(),obstacles.end(),obstacle_node);
//            bool already_exists = !(find_res == obstacles.end());
//            if (!already_exists)
//            {
//                obstacles.push_back(obstacle_node);
//                ++ii;
//            }
//        }
    }

    void set_obstacles(const std::vector<Node> &new_obstacles)
    {
        obstacles = new_obstacles;
    }
    bool is_obstacle(const Node &node)
    {
        auto find_res = std::find(obstacles.begin(),obstacles.end(),node);
        bool isObstacle = !(find_res == obstacles.end());
        return isObstacle;
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

    int width = 100;
    int height = 100;
    int number_of_obstacles = 300;
    std::vector<std::vector<Node>> grid;
    std::vector<Node> obstacles;
    static std::array<Node,4> DIRS;


};


class Pathfinding
{
public:
    Pathfinding();
    std::vector<Node> find_path(ZoneMap &map, const Node &from, const Node &to);
    std::vector<Node> get_path_vector(const std::unordered_map<Node,Node> &visit_log,const Node &from, const Node &to);
private:
    std::vector<Node> path;
};

#endif // PATHFINDING_H
