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


   

   // auto path = Search::BFS(map,{atoi(argv[2]),atoi(argv[3])},{atoi(argv[4]),atoi(argv[5])}); 
    //colorMap.print(path);
    auto path = Search::greedyBFS(map, {x1, y1}, {x2, y2});
    colorMap.print(path);

    auto pathAStar = Search::AStar(map, {x1, y1}, {x2, y2}); 
    colorMap.print(pathAStar); // Punto 6 del checklist
    //Calculate path distance
    //Print path distance
    if (!path.empty())
    {
        std::cout << "Dist: " << path.size() - 1 << std::endl;
    } else 
    {
        std::cout << "No se encontro un camino." << std::endl;
    }
    
    return 0;
}