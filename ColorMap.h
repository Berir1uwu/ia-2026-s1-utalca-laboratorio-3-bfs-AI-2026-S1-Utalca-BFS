#pragma once
#include "Map.h"
#include <array>




class ColorMap:public Map{//clase que herede de Map y reimplemente los métodos print para imprimir con colores
    std::array<int,5> colors;
public:
    ColorMap(const Map& rhs);
    void print() const;                 //prints map
    void print(std::vector<std::pair<int,int>>) const; //prints map and path 


};