#include "store.h"
#include "item.h"

#include <queue>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

const int rowMoveArray[] = {1, -1, 0, 0};
const int colMoveArray[] = {0, 0, 1, -1};


Store::Store(){
    rows = 0;
    columns = 0;
    storeName = "N/A";
}

void Store::createStore(string filename){
    // init file reading
    ifstream fin(filename);

    if (fin.fail()){
        cerr << "Failed to open file." << endl;
        return;
    }

    string currentLine;
    // get first line with number of rows and columns and name of store
    getline(fin, currentLine);
    istringstream xyInput(currentLine);
    xyInput >> rows >> columns >> storeName;

    // # # #  1,2
    // # 0 #
    // # # #
    
    // create 2d vector for the storeLayout
    for (int i = 0; i < rows; i++){
        vector<char> temp(columns);
        storeLayout.push_back(temp);
    } 

    string rowsAndColumns;
    int index = 0;
    // read through rest of file lines to make the 2d vector storeLayout
    while (getline(fin, rowsAndColumns)){
        istringstream rowsAndColumnsInput(rowsAndColumns);

        for (int i = 0; i < columns; i++){
            char addition;
            rowsAndColumnsInput >> addition;
            storeLayout.at(index).at(i) = addition;
        }

        index++;
        if (index == rows){
            break;
        }
    }
}

void Store::removeItemFromStore(string itemName){
    int index = -1;

    // loop through vector of items and if the item is in the vector set index to the index of item
    for (size_t i = 0; i < itemsInStore.size(); i++){
        if (itemsInStore.at(i).getName() == itemName){
            index = i;
        }
    }

    // if index is still init value the item was not found
    if (index == -1){
        cout << "Could not find the item " << itemName << " in the store." << endl;
    }
    // else erase the index from the vector of items
    else{
        cout << "Erasing the item " << itemName << " from the store." << endl;
        itemsInStore.erase(itemsInStore.begin() + index);
    }
}

void Store::addItemToStore(string itemName, int xCoord, int yCoord){
    Item newItem;
    newItem.createItem(itemName, xCoord, yCoord);
    itemsInStore.push_back(newItem);
    cout << endl;
    cout << endl;
    cout << "---------------------------------" << endl;
    cout << "Item successfully added to store." << endl;
    cout << "Item name: " << newItem.getName() << endl;;
    cout << "---------------------------------" << endl;
    cout << endl;
    cout << endl;
}

void Store::printStore(){
    cout << endl;
    cout << endl;
    cout << "---------------------------------------" << endl;
    cout << "Printing Store." << endl;
    cout << "Store Name: " << storeName << endl;
    cout << "Rows: " << rows << "     Columns: " << columns << endl;
    cout << "Total Area: " << rows * columns << endl;
    cout << "Items in Store: " << itemsInStore.size() << endl;
    cout << endl;
    cout << "Store Layout:" << endl;

    // loops through 2d vector "storeLayout" and prints it
    for (int i = 0; i < rows; i++){
        for (int z = 0; z < columns; z++){
            cout << storeLayout.at(i).at(z);
        }
        cout << endl;
    }

    cout << endl;
    cout << "---------------------------------------" << endl;
    cout << endl;
    cout << endl;
}

void Store::printItemsInStore(){
    cout << endl;
    cout << endl;
    cout << "---------------------------------------" << endl;
    // if vector of items is empty meaning no items in store
    if (itemsInStore.size() <= 0){
        cout << "There are no items in this store." << endl;
    }else{
        cout << "Printing Store Items to File: items.txt";

        // inits and opens a output file
        ofstream outputFile;
        outputFile.open("TextFiles/items.txt");
        // set name, number of items in store, and the format of items
        outputFile << "Store Name: " << storeName << "\n";
        outputFile << "Items in Store: " << itemsInStore.size() << "\n";
        outputFile << "Item Format: Item Name, X coordinate in store, Y coordinate in store \n";
        // loop through vector of items and get their name, x coordinate, and y coordinate
        for (size_t i = 0; i < itemsInStore.size(); i++){
            outputFile << itemsInStore.at(i).getName() << "," 
            
            << itemsInStore.at(i).getItemRow() << ","
            << itemsInStore.at(i).getItemCol() << "\n";
        }
        outputFile.close();
        cout << endl;
        cout << "Printing complete." << endl;
        
    }
    cout << "---------------------------------------" << endl;
    cout << endl;
    cout << endl;
}

void Store::addItemFileToStore(string filename){
    // start file reading
    ifstream fin(filename);

    if(fin.fail()){
        cerr << "Failed to open file." << endl;
        return;
    }

    // skips first line of file
    string currentLine;
    getline(fin, currentLine);

    string getItems;
    
    // goes through rest of lines
    while (getline(fin, getItems)){
        Item addedItem;
        istringstream item(getItems);
        string itemName;
        int xCoord, yCoord;
        // set values from file
        item >> itemName >> xCoord >> yCoord;
        // check for an _ in the item name and if there is change it with a space ex: baked_beans -> baked beans
        for (size_t i = 0; i < itemName.length(); i++){
            if (itemName[i] == '_'){
                string newName;
                newName += itemName.substr(0,i);
                newName += " ";
                newName += itemName.substr(i+1, newName.length());
                itemName = newName;
                break;
            }
        }
        // create a new item with file data and add it to vector of items
        addedItem.createItem(itemName, xCoord, yCoord);
        itemsInStore.push_back(addedItem);
    }
}

vector<pair<int, int>> Store::findPath(string currLocation, string goalLocation){
    // the rows and columns are swapped because im a dumbass
    // in the gui i used x and y positions like a graph
    // but in the find path i used rows and columns
    // still finds path great :D
    
    // init values for starting and ending location
    Item startItem, endItem;
    bool checkStart, checkEnd;

    // loop through items to find starting and ending location
    for (size_t i = 0; i < itemsInStore.size(); i++){
        if (itemsInStore.at(i).getName() == currLocation){
            startItem = itemsInStore.at(i);
            checkStart = true;
        }
        if (itemsInStore.at(i).getName() == goalLocation){
            endItem = itemsInStore.at(i);
            checkEnd = true;
        }
    }

    // make a pair of the end row and column for later
    pair<int, int> end = {endItem.getItemCol(), endItem.getItemRow()};

    // if any location have the init values then item does not exist
    if (checkStart == false || checkEnd == false){
        cout << "Could not find Item!" << endl;
        // return empty vector
        return {};
    }

    /*
        make a vector of pairs which all init to {-1,-1}

        this vector holds pairs that can hold the path.  At one index of this vector
        it holds a pair to the index of storeLayout where it came from.  So basically
        during the BFS (breadth first search) it adds what move it did to cameFrom so 
        when the program finds the end item it can go through cameFrom and get the moves
        that were made
    */ 
    vector<vector<pair<int,int>>> cameFrom(rows, vector<pair<int,int>>(columns, {-1,-1}));

    // make a visited vector for the breadth first search
    vector<vector<bool>> visited;
    for (size_t i = 0; i < rows; i++){
        vector<bool> newVect(columns);
        visited.push_back(newVect);
    }
    
    // if the spot of the storeLayout is a 0 meaning it is a walkable isle then set visited to false
    // and set everything else meaning the # to true so that during BFS it does not look at those spots
    for (size_t i = 0; i < rows; i++){
        for (size_t z = 0; z < columns; z++){
            if (storeLayout.at(i).at(z) == '0'){
                visited.at(i).at(z) = false;
            } else {
                visited.at(i).at(z) = true;
            }
        }
    }

    
    queue<pair<int, int>> searchQueue; // init a queue
    visited.at(startItem.getItemCol()).at(startItem.getItemRow()) = true; // set starting location to visited
    searchQueue.push({startItem.getItemCol(), startItem.getItemRow()}); // add starting location to top of queue
    vector<pair<int, int>> path; // init a vector to hold the shortest path

    
    while (!searchQueue.empty()){
        // get the front of the queue
        pair<int, int> position = searchQueue.front();
        searchQueue.pop();
        int currentRow = position.first;
        int currentCol = position.second;

        // if the current row and column are the end row and column
        if (currentRow == endItem.getItemCol() && currentCol == endItem.getItemRow()){
            /*
                This for loop begins with a pair which is end.  end is a pair of integers which are the 
                row and column for the item we are searching for a path too.  So the loop starts at the
                end and goes through cameFrom but only when cameFrom pairs are not -1
            */
            for (pair<int,int> current = end; current.first != -1 && current.second != -1; current = cameFrom.at(current.first).at(current.second)){
                path.push_back(current);
            }
        }

        // loop through all 4 directions (up, down, right, left) and see if it is a valid move and if so do it
        for (int i = 0; i < 4; i++){
            // adds 1, -1, or 0 to current location
            int moveRow = currentRow + rowMoveArray[i];
            int moveCol = currentCol + colMoveArray[i];

            // check bounds and make sure spot has not been visited already
            if (checkValidMove(moveRow, moveCol) == true && visited.at(moveRow).at(moveCol) == false){
                searchQueue.push({moveRow, moveCol}); // add move to queue
                visited.at(moveRow).at(moveCol) = true; // set spot to visited
                cameFrom.at(moveRow).at(moveCol) = {currentRow, currentCol}; // add move to cameFrom
            }
        }
    }
    // return the path.  It should always find a path because above it checks to make sure both items exist
    // but if it doesnt then nothing will be added to path so it will be empty anyway
    return path;

}


bool Store::checkValidMove(int currentRow, int currentCol){
    // check that the row is not out of bounds of the store layout
    if (currentRow >= 0 && currentRow < rows){
        if (currentCol >= 0 && currentCol < columns){
            // return true if the row and column given are in the layout
            return true;
        }
    }
    // return false if not
    return false;
}

pair<int, int> Store::findItemLocation(string itemName){
    pair<int, int> location = {-1, -1};
    for (Item item: itemsInStore){
        if (item.getName() == itemName){
            location.first = item.getItemRow();
            location.second = item.getItemCol();
        }
    }
    return location;
}

pair<int, int> Store::getRowCol(){
    pair<int, int> returnPair;
    returnPair.first = rows;
    returnPair.second = columns;
    return returnPair;
}

vector<vector<char>> Store::getLayout(){
    return storeLayout;
}

string Store::getStoreName(){
    return storeName;
}
