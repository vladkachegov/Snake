#include "zonemap.h"

std::array<Node,4> ZoneMap::DIRS = {Node(-1,0),  //left
                                    Node(1,0),  //right
                                    Node(0,1),  // top
                                    Node(0,-1)}; //bottom

ZoneMap::ZoneMap()
{

}

std::vector<std::vector<Node> > ZoneMap::get_grid() const
{
    return grid;
}

std::vector<Node> ZoneMap::get_obstacles() const
{
    return obstacles;
}
