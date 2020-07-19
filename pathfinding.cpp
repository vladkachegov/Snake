#include "pathfinding.h"

Pathfinding::Pathfinding()
{

}

std::vector<Node> Pathfinding::find_path(ZoneMap &map,
                                         const Node &from,
                                         const Node &to)
{
    using namespace std;
    unordered_map<Node,Node> visit_log;
    queue<Node> queue;
    visit_log[from] = from;
    queue.push(from);
    while (!queue.empty())
    {
        Node elem = queue.front();
        queue.pop();
        for (Node n : map.get_neighbours(elem))
        {
            if (visit_log.find(n) == visit_log.end())
            {
                visit_log[n] = elem;
                queue.push(n);
                if (n == to)
                {
                    return get_path_vector(visit_log,from,to);
                }
            }
        }
    }
}

std::vector<Node> Pathfinding::get_path_vector(const std::unordered_map<Node, Node> &visit_log,
                                  const Node &from, const Node &to)
{
    using namespace std;
    vector<Node> path;
    Node node = to;
    path.push_back(to);
    while (node!=from)
    {
        node = visit_log.at(node);
        path.push_back(node);
    }
    std::reverse(path.begin(),path.end());
    return path;
}

bool operator==(const Node &node1, const Node &node2)
{
    return ((node1.x==node2.x)&(node1.y==node2.y));
}


