#ifndef NODE_H
#define NODE_H
#include <cstdlib>
#include <functional>
/** Node, part of grid map*/
class Node
{
public:
    Node();
    Node(int _x,int _y):x(_x),y(_y)
    {

    }

    bool operator==(const Node &node)
    {
        return ((x==node.x)&&(y==node.y));
    }

    bool is_equal_to(const Node &node) const
    {
        return ((x==node.x)&&(y==node.y));
    }
    bool operator!=(const Node &node)
    {
        return ((x!=node.x)||(y!=node.y));
    }
    int x = 0;
    int y = 0;
};



namespace std {

template <>
struct hash<Node>
{
    std::size_t operator()(const Node& node) const noexcept {
        return std::hash<int>()(node.x ^ (node.y << 4));
    }
};

}


#endif // NODE_H
