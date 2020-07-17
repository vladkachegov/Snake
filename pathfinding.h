#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <array>
#include <vector>
struct Node
{
    Node(int _x,int _y):x(_x),y(_y)
    {

    }
    bool isAvailable = true;
    int x = 0;
    int y = 0;
    //    std::array<Node,4> neighbours;
};
struct ZoneMap
{
    // Generate map on initialization
    ZoneMap(){};
    ZoneMap(int _width, int _height):width(_width),height(_height)
    {

    }
    int width = 100;
    int height = 100;
    std::vector<std::vector<Node>> grid;
    static std::array<Node,4> DIRS;
    bool out_of_bounds();
    bool is_obstacle();
};
std::array<Node,4> ZoneMap::DIRS = {Node(-1,0),  //left
                                    Node(1,0),  //right
                                    Node(0,1),  // top
                                    Node(0,-1)}; //bottom

class Pathfinding
{
public:
    Pathfinding();
private:

};

#endif // PATHFINDING_H
