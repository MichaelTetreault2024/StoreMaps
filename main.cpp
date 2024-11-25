#include "store.h"
#include "gui.h"

#include <iostream>

using namespace std;

int main () {

    Store myStore; // create store object
    myStore.createStore("TextFiles/schnucks.txt"); // load in store layout (isles and shelves)
    myStore.addItemFileToStore("TextFiles/itemInput.txt"); // load in items to store

    makeGUI myGUI; // create gui object
    myGUI.draw(myStore); // draw the store

    return 0;
}