#ifndef STORE
#define STORE

#include "item.h"
#include <iostream>
#include <vector>


using namespace std;

class Store{
    public:
        Store();
        void createStore(string filename);
        void printStore();
        void addItemToStore(string newItem, int xCoord, int yCoord);
        void removeItemFromStore(string itemRemoved);
        void addItemFileToStore(string filename);
        void printItemsInStore();
        vector<pair<int, int>> findPath(string currLocation, string goalLocation);
        pair<int, int> getRowCol();
        vector<vector<char>> getLayout();
        string getStoreName();
        pair<int, int> findItemLocation(string itemName);

    private:
        bool checkValidMove(int currentRow, int currentCol);
        int rows;
        int columns;
        string storeName;
        vector<vector<char>> storeLayout;
        vector<Item> itemsInStore;
};



#endif