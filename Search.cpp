#include "Search.h"
#include <queue>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <limits>
#include <deque>

namespace std {
    template<> struct hash<std::pair<int,int>> {
        std::size_t operator()( const std::pair<int,int> & p) const noexcept {
            hash<int> hasher;
            return hasher(p.first) ^ (hasher(p.second)<<1);
        }
    };
}

// Estructura Node para algoritmos con prioridad
struct Node {
    std::pair<int, int> pos;
    float g;  
    float h; 
    float f; 

    bool operator>(const Node& other) const {
        return f > other.f; 
    }
};

float Search::Heuristic(std::pair<int, int> start, std::pair<int, int> goal) {
    return std::abs(start.first - goal.first) + std::abs(start.second - goal.second);
}

std::vector<std::pair<int,int>> Search::reconstruct(const std::unordered_map<std::pair<int,int>,std::pair<int,int>> &pathCache, const std::pair<int,int> &goal) {
    std::deque<std::pair<int,int>> nodes;
    auto node = goal;
    while(pathCache.find(node) != pathCache.end()){
        nodes.push_front(node);
        node = pathCache.at(node);
    }
    nodes.push_front(node);
    return std::vector<std::pair<int,int>>(nodes.begin(), nodes.end());
}

// --- 1. BFS ---
std::vector<std::pair<int,int>> Search::BFS(const Map& map, std::pair<int,int> start, std::pair<int,int> goal) {
    std::cout<<"===========================\nRunning BFS...\n";
    auto startTime = std::chrono::high_resolution_clock::now();
    int visitedCount = 0;
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

    std::vector<std::vector<bool>> visited(map.h, std::vector<bool>(map.w, false));
    std::queue<std::pair<int,int>> OPEN;
    std::unordered_map<std::pair<int,int>,std::pair<int,int>> pathCache;

    OPEN.push(start);
    visited[start.first][start.second] = true;

    while(!OPEN.empty()){
        std::pair<int, int> pos = OPEN.front();
        OPEN.pop();
        visitedCount++;

        if(pos == goal){
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration<double, std::milli>(endTime - startTime).count();
            std::cout << "BFS FOUND!\nTIME: " << duration << " ms\nVISITED: " << visitedCount 
                      << "\nOPEN: " << OPEN.size() << "\nDIRECTIONS: 4\n";
            return reconstruct(pathCache, pos);
        }

        for(auto dir : dirs){
            std::pair<int, int> neighbor = {pos.first + dir.first, pos.second + dir.second};
            if(neighbor.first >= 0 && neighbor.first < map.h && neighbor.second >= 0 && neighbor.second < map.w && 
               map._map[neighbor.first][neighbor.second] != '1' && !visited[neighbor.first][neighbor.second]) {
                visited[neighbor.first][neighbor.second] = true;
                OPEN.push(neighbor);
                pathCache[neighbor] = pos;
            }
        }
    }
    return {};
}

// --- 2. GREEDY BFS ---
std::vector<std::pair<int,int>> Search::Greedy(const Map& map, std::pair<int,int> start, std::pair<int,int> goal) {
    std::cout<<"===========================\nRunning Greedy...\n";
    auto startTime = std::chrono::high_resolution_clock::now();
    int visitedCount = 0;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> OPEN; 
    std::unordered_map<std::pair<int,int>,std::pair<int,int>> pathCache;
    std::vector<std::vector<bool>> visited(map.h, std::vector<bool>(map.w, false));

    float hStart = Heuristic(start, goal);
    OPEN.push({start, 0, hStart, hStart}); // Inicialización completa
    visited[start.first][start.second] = true;
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};   

    while(!OPEN.empty()){
        Node current = OPEN.top();
        OPEN.pop();
        visitedCount++;

        if(current.pos == goal){
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration<double, std::milli>(endTime - startTime).count();
            std::cout << "GREEDY FOUND!\nTIME: " << duration << " ms\nVISITED: " << visitedCount 
                      << "\nOPEN: " << OPEN.size() << "\nDIRECTIONS: 4\n";
            return reconstruct(pathCache, goal);
        }

        for(auto dir : dirs){
            std::pair<int, int> neighbor = {current.pos.first + dir.first, current.pos.second + dir.second};
            if(neighbor.first >= 0 && neighbor.first < map.h && neighbor.second >= 0 && neighbor.second < map.w && 
               map._map[neighbor.first][neighbor.second] != '1' && !visited[neighbor.first][neighbor.second]) {
                visited[neighbor.first][neighbor.second] = true; 
                pathCache[neighbor] = current.pos;
                float hN = Heuristic(neighbor, goal);
                OPEN.push({neighbor, 0, hN, hN});
            }
        }
    }
    return {};
}

// --- 3. A* ---
std::vector<std::pair<int,int>> Search::AStar(const Map& map, std::pair<int,int> start, std::pair<int,int> goal) {
    std::cout << "===========================\nRunning A*...\n";
    auto startTime = std::chrono::high_resolution_clock::now();
    int visitedCount = 0;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> OPEN;
    std::unordered_map<std::pair<int,int>, std::pair<int,int>> pathCache;
    std::unordered_map<std::pair<int,int>, float> gCost; 

    float hStart = Heuristic(start, goal);
    OPEN.push({start, 0, hStart, hStart});
    gCost[start] = 0;
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

    while(!OPEN.empty()){
        Node current = OPEN.top();
        OPEN.pop();
        visitedCount++;

        if(current.pos == goal){
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration<double, std::milli>(endTime - startTime).count();
            std::cout << "A* FOUND!\nTIME: " << duration << " ms\nVISITED: " << visitedCount 
                      << "\nOPEN: " << OPEN.size() << "\nDIRECTIONS: 4\n";
            return reconstruct(pathCache, goal);
        }

        for(auto dir : dirs){
            std::pair<int, int> neighbor = {current.pos.first + dir.first, current.pos.second + dir.second};
            if(neighbor.first >= 0 && neighbor.first < map.h && neighbor.second >= 0 && neighbor.second < map.w && 
               map._map[neighbor.first][neighbor.second] != '1') {
                float tentative_gCost = gCost[current.pos] + 1;
                if(gCost.find(neighbor) == gCost.end() || tentative_gCost < gCost[neighbor]) {
                    gCost[neighbor] = tentative_gCost;
                    float hN = Heuristic(neighbor, goal);
                    pathCache[neighbor] = current.pos;
                    OPEN.push({neighbor, tentative_gCost, hN, tentative_gCost + hN});
                }
            }
        }
    }
    return {};
}

// --- 4. WEIGHTED A* ---
std::vector<std::pair<int,int>> Search::WAStar(const Map& map, std::pair<int,int> start, std::pair<int,int> goal) {
    std::cout << "===========================\nRunning WA*...\n";
    float weight = 2.0f; 
    auto startTime = std::chrono::high_resolution_clock::now();
    int visitedCount = 0; // Declarado correctamente

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> OPEN;
    std::unordered_map<std::pair<int,int>, std::pair<int,int>> pathCache;
    std::unordered_map<std::pair<int,int>, float> gCost; 

    gCost[start] = 0;
    float hStart = Heuristic(start, goal);
    OPEN.push({start, 0, hStart, weight * hStart});
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

    while(!OPEN.empty()){
        Node current = OPEN.top();
        OPEN.pop();
        visitedCount++;

        if(current.pos == goal){
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration<double, std::milli>(endTime - startTime).count();
            std::cout << "WA* FOUND!\nTIME: " << duration << " ms\nVISITED: " << visitedCount 
                      << "\nOPEN: " << OPEN.size() << "\nDIRECTIONS: 4\n";
            return reconstruct(pathCache, goal);
        }

        for(auto dir : dirs){
            std::pair<int, int> neighbor = {current.pos.first + dir.first, current.pos.second + dir.second};
            if(neighbor.first >= 0 && neighbor.first < map.h && neighbor.second >= 0 && neighbor.second < map.w && 
               map._map[neighbor.first][neighbor.second] != '1') {
                float tentative_gCost = gCost[current.pos] + 1;
                if(gCost.find(neighbor) == gCost.end() || tentative_gCost < gCost[neighbor]) {
                    gCost[neighbor] = tentative_gCost;
                    float hN = Heuristic(neighbor, goal);
                    pathCache[neighbor] = current.pos;
                    OPEN.push({neighbor, tentative_gCost, hN, tentative_gCost + (weight * hN)});
                }
            }
        }
    }
    std::cout << "NOT FOUND!!!\n";
    return {};
}