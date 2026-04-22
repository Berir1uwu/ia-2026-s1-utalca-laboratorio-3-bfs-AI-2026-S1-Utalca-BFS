#include "Map.h"
#include "Search.h"
#include "ColorMap.h"
#include <cassert>
#include <iostream>

int main(int argc, char *argv[]){

    //Verify that the amount of arguments is correct
    //6 arguments: program name, map file, x1, y1, x2, y2
    if (argc < 6){
        std::cerr << "Faltan argumentos" << std::endl;
        return 1;
    }

    Map map(argv[1]);
    ColorMap colorMap(map);
    colorMap.print();
    
    int x1 = std::atoi(argv[2]);
    int y1 = std::atoi(argv[3]);
    int x2 = std::atoi(argv[4]);
    int y2 = std::atoi(argv[5]);


   
auto pathBFS = Search::BFS(map, {x1, y1}, {x2, y2});
colorMap.print(pathBFS);

auto pathGREEDY = Search::Greedy(map, {x1, y1}, {x2, y2});
colorMap.print(pathGREEDY);

auto pathAStar = Search::AStar(map, {x1, y1}, {x2, y2});
colorMap.print(pathAStar);

auto pathWAStar = Search::WAStar(map, {x1, y1}, {x2, y2});
colorMap.print(pathWAStar);

    if (!pathAStar.empty()) {
    colorMap.print(pathAStar); 
    std::cout << "Dist: " << pathAStar.size() - 1 << std::endl; 
} else {
    std::cout << "No se encontro un camino con A*." << std::endl;
}/*
    if (!path.empty())
    {
        std::cout << "Dist: " << path.size() - 1 << std::endl;
    } else 
    {
        std::cout << "No se encontro un camino." << std::endl;
    }*/
    
    return 0;
}