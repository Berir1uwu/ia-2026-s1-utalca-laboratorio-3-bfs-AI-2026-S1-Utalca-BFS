#include "Search.h"
#include <queue>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <limits>
#include <deque>

namespace std
{
    template<> struct hash<std::pair<int,int>>
    {
        std::size_t operator()( const std::pair<int,int> & p) const noexcept
        {
            hash<int> hasher;
            return hasher(p.first) ^ (hasher(p.second)<<1);
        }
    };
}

// 1. IMPLEMENTACIÓN DE LA HEURÍSTICA (Punto 3 del checklist)
float Search::Heuristic(std::pair<int, int> start, std::pair<int, int> goal) {
    return std::abs(start.first - goal.first) + std::abs(start.second - goal.second);
}

std::vector<std::pair<int,int>> Search::reconstruct(const std::unordered_map<std::pair<int,int>,std::pair<int,int>> &pathCache, const std::pair<int,int> &goal){
    std::deque<std::pair<int,int>> nodes;
    auto node = goal;

    while(pathCache.find(node) != pathCache.end()){
        nodes.push_front(node);
        node = pathCache.at(node);
    }
    nodes.push_front(node);

    std::vector<std::pair<int,int>> vec(nodes.begin(), nodes.end());
    return vec;
}

std::vector<std::pair<int,int>> Search::BFS(const Map& map, std::pair<int,int> start, std::pair<int,int> goal){
    std::cout<<"===========================\nRunning BFS...\n";
    auto startTime = std::chrono::high_resolution_clock::now();
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

    std::vector<std::vector<bool>> visited(map.h, std::vector<bool>(map.w, false));
    std::queue<std::pair<int,int>> OPEN;
    std::unordered_map<std::pair<int,int>,std::pair<int,int>> pathCache;

    OPEN.push(start);
    visited[start.first][start.second] = true;

    while(!OPEN.empty()){
        std::pair<int, int> pos = OPEN.front();
        OPEN.pop();

        if(pos == goal){
            auto endTime = std::chrono::high_resolution_clock::now();
            int count = 0;
            for(int i=0; i<map.h; i++)
                for(int j=0; j<map.w; j++)
                    if(visited[i][j]) count++;

            std::cout<<"VISITED: "<<count<<std::endl;
            std::cout<<"OPEN: "<<OPEN.size()<<std::endl;
            std::cout<<"FOUND in "<<(endTime-startTime).count()/1000000.0<<"ms\n";
            return reconstruct(pathCache, pos);
        }

        for(auto dir : dirs){
            std::pair<int, int> neighbor = {pos.first + dir.first, pos.second + dir.second};
            
            if(neighbor.first >= 0 && neighbor.first < map.h && neighbor.second >= 0 && neighbor.second < map.w && 
               map._map[neighbor.first][neighbor.second] != '1' && !visited[neighbor.first][neighbor.second])
            {
                visited[neighbor.first][neighbor.second] = true;
                OPEN.push(neighbor);
                pathCache[neighbor] = pos;
            }
        }
    }
    std::cout<<"NOT FOUND!!!!\n";
    return {start, goal};
};
struct Node {
    std::pair<int, int> pos;
    float g; // Coste real desde el inicio hasta este nodo (Punto 3 del checklist) 
    float h; // Heurística (estimación hasta el final)
    float f; // f = g + h (Coste total estimado) 

    // El comparador debe utilizar f = g + h (Punto 5 del checklist) 
    bool operator>(const Node& other) const {
        return f > other.f; 
    }
};

std::vector<std::pair<int,int>> Search::greedyBFS(const Map& map, std::pair<int,int> start, std::pair<int,int> goal){
    std::cout<<"===========================\nRunning Greedy...\n";
    auto startTime = std::chrono::high_resolution_clock::now();

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> OPEN; 
    std::unordered_map<std::pair<int,int>,std::pair<int,int>> pathCache;
    std::vector<std::vector<bool>> visited(map.h, std::vector<bool>(map.w, false));

    float h_val = Heuristic(start, goal);
OPEN.push({start, 0, h_val, h_val});
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};   

    while(!OPEN.empty()){
        Node current = OPEN.top();
        OPEN.pop();

        if(current.pos == goal){
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration<double, std::milli>(endTime - startTime).count();
            std::cout << "FOUND in Greedy!\n";
            std::cout << "Time: " << duration << " ms\n"; 
            return reconstruct(pathCache, goal);
        }

        for(auto dir : dirs){
            std::pair<int, int> neighbor = {current.pos.first + dir.first, current.pos.second + dir.second};


            if(neighbor.first >= 0 && neighbor.first < map.h && 
               neighbor.second >= 0 && neighbor.second < map.w && 
               map._map[neighbor.first][neighbor.second] != 1 && 
               !visited[neighbor.first][neighbor.second]) 
            {
                visited[neighbor.first][neighbor.second] = true; 
                pathCache[neighbor] = current.pos;
                OPEN.push({neighbor, Heuristic(neighbor, goal)});
            }
        }
    }
    std::cout<<"NOT FOUND!!!!\n";
    return {start, goal};
};
    std::vector<std::pair<int,int>> Search::AStar(const Map& map, std::pair<int,int> start, std::pair<int,int> goal){
std::cout << "===========================\nRunning A*...\n";
    auto startTime = std::chrono::high_resolution_clock::now();

    // Inicialización
    OPEN.push({start, Heuristic(start, goal)});
    gCost[start] = 0;

    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

    while(!OPEN.empty()){
        Node current = OPEN.top();
        OPEN.pop();

        if(current.pos == goal){
            auto endTime = std::chrono::high_resolution_clock::now();
            std::cout << "A* FOUND the path!\n";
            return reconstruct(pathCache, goal);
        }

        for(auto dir : dirs){
            std::pair<int, int> neighbor = {current.pos.first + dir.first, current.pos.second + dir.second};

            if(neighbor.first >= 0 && neighbor.first < map.h && 
               neighbor.second >= 0 && neighbor.second < map.w && 
               map._map[neighbor.first][neighbor.second] != 1) 
           
                float tentative_gCost = gCost[current.pos] + 1;
                if(gCost.find(neighbor) == gCost.end() || tentative_gCost < gCost[neighbor]) {
                    gCost[neighbor] = tentative_gCost;
                    float fCost = tentative_gCost + Heuristic(neighbor, goal); // f = g + h
                    
                    pathCache[neighbor] = current.pos;
                    OPEN.push({neighbor, fCost});
                }
            }
        }
    }
    return {};
    
};