#include "item.h"

#include <iostream>

using namespace std;
Item::Item(){
    row = -1;
    column = -1;
    itemName = "N/A";
}

void Item::createItem(string itemNameParam, int rowParam, int columnParam){
    row = rowParam;
    column = columnParam;
    itemName = itemNameParam;
    
}

string Item::getName(){
    return itemName;
}

int Item::getItemRow(){
    return row;
}

int Item::getItemCol(){
    return column;
}