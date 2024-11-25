#ifndef ITEM
#define ITEM

#include <iostream>

using namespace std;

struct Item{
    public:
        Item();
        void createItem(string itemNameParam, int rowParam, int columnParam);
        string getName();
        int getItemRow();
        int getItemCol();

    private:
        int row;
        int column;
        string itemName;

};
#endif