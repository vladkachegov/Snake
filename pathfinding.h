#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <array>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <zonemap.h>
#include <node.h>
#include <queue>
#include <QDebug>

class Pathfinding
{
public:
    Pathfinding();
    static std::vector<Node> find_path(ZoneMap &map,
                                       const Node &from,
                                       const Node &to, bool &is_founded);
    static std::vector<Node> get_path_vector(const std::unordered_map<Node,Node> &visit_log,const Node &from, const Node &to);
};

#endif // PATHFINDING_H
