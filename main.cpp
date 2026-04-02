#include "Map.h"
#include "Search.h"
#include "ColorMap.h"
#include <cassert>
#include <iostream>

int main(int argc, char *argv[]){

    //Verify that the amount of arguments is correct
    //6 arguments: program name, map file, x1, y1, x2, y2

    //Load map with class Map
    Map map(argv[1]);
    ColorMap colorMap(map);
    colorMap.print();

    auto path = Search::BFS(map,{atoi(argv[2]),atoi(argv[3])},{atoi(argv[4]),atoi(argv[5])}); 
    colorMap.print(path);
    
    //Calculate path distance
    //Print path distance
    
    return 0;
}