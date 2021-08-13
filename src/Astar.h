#pragma once

#include <queue>
#include <functional>
#include "Map.h"
#include <map>
class GridAstar
{


public:
    typedef std::pair<int,int> Point ;

    struct Node
    {
        double g;
        Point pos;
        double f;

        bool operator ==(const Node &b) const
        {
            return  pos == b.pos;
        }
        bool operator <(const Node& b) const
        {
            return f > b.f;
        }
        double val = 0;
        Node* predecessor = nullptr;
    };


    GridAstar(GridMap& _map, Point _start, Point _end):
        map(_map)
    {
        allNodes.reserve(map.grid_width* map.grid_height);
        //Create nodes from map
        for(size_t x = 0; x < map.grid_width; x++)
        {
            for(size_t y = 0; y < map.grid_height;y++)
            {
                allNodes.push_back({0, {x,y}, 0, 255-map(x,y)});
            }
        }
        //Find start and end position
        for(auto & elem: allNodes)
        {
            if(_start == elem.pos)
                start_node = &elem;
            if(_end == elem.pos)
                end_node = &elem;
        }
    }
    std::optional<Node> start()
    {
        //Add start node to open list
        openlist.push_back(start_node);

        while(true)
        {
            std::sort(openlist.begin(),openlist.end(), [](const Node*  a, const Node*  b) {return a->f > b->f;}); //Needs explicit comparision because we have a list of pointers

            const auto current_node = openlist.back();
            openlist.pop_back();

            if(*current_node == *end_node)
            {
                return *current_node;
            }

            closedlist.push_back(current_node);

            expandNode(current_node);

            if(openlist.empty())
                return std::nullopt;
        }
    }
    inline double cost(const Node* current,const Node* successor) const
    {
        return 5*std::abs(current->pos.first - successor->pos.first) + std::abs(current->pos.second - successor->pos.second);
    }


    inline void expandNode(Node* _node)
    {
        //Get possible successors

        std::vector<Node*> successors;
        successors.reserve(8);
        int x = _node->pos.first;
        int y = _node->pos.second;

        const int limit = 255;

        Node* n = fromPos({x+1,y+1});
        if(n && n->val < limit)
            successors.push_back(n);
        n = fromPos({x+1,y+0});
        if(n&& n->val < limit)
            successors.push_back(n);
        n = fromPos({x+1,y-1});
        if(n&& n->val < limit)
            successors.push_back(n);
        n = fromPos({x+0,y+1});
        if(n&& n->val < limit)
            successors.push_back(n);
        n = fromPos({x+0,y-1});
        if(n&& n->val < limit)
            successors.push_back(n);
         n = fromPos({x-1,y+1});
        if(n&& n->val < limit)
            successors.push_back(n);
        n = fromPos({x-1,y+0});
        if(n&& n->val < limit)
            successors.push_back(n);
        n = fromPos({x-1,y-1});
        if(n&& n->val < limit)
            successors.push_back(n);




        for(auto & successor : successors)
        {
            if(inClosedList(successor))
            {
                continue;
            }

            const double tentative_g = _node->g + successor->val;

            auto in_open_list = inOpenList(successor);

            if(in_open_list && tentative_g >= successor->g)
            {
                continue;
            }

            successor->predecessor = _node;

            successor->g = tentative_g;

            const double f = tentative_g +cost(_node, successor);
            successor->f = f;

            if(!in_open_list)
            {
                openlist.push_back(successor);
            }

        }

    }

    bool inOpenList(const Node* _node)
    {
        return std::find(openlist.begin(), openlist.end(), _node) != openlist.end();
    }

    bool inClosedList(const Node* _node)
    {
        return std::find(closedlist.begin(),closedlist.end(), _node) != closedlist.end();
    }
    Node* fromPos(const Point& _pos)
    {
        for(auto& node: allNodes)
        {
            if(node.pos == _pos)
                return &node;
        }
        return nullptr;
    }


private:
    std::vector<Node> allNodes;
    std::vector<Node*>  openlist;
    std::vector<Node*> closedlist;
    Node* start_node = nullptr;
    Node* end_node = nullptr;
    GridMap& map;




};

