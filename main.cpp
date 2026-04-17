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
    int x1 = std::atoi(argc[2]);
    int y1 = std::atoi(argc[3]);
    int x2 = std::atoi(argc[4]);
    int y2 = std::atoi(argc[5]);

    if (x1 < 0 || x1 >= map.h || y1 < 0 || y1 >= map.w ||
    x2 < 0 || x2 >= map.h || y2 < 0 || y2 >= map.w) {
    std::cerr << "Error: Coordenadas fuera de los limites del mapa." << std::endl;
    return 1;
}
    //Load map with class Map
    Map map(argv[1]);
    ColorMap colorMap(map);
    colorMap.print();

    auto path = Search::BFS(map,{atoi(argv[2]),atoi(argv[3])},{atoi(argv[4]),atoi(argv[5])}); 
    colorMap.print(path);
    
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