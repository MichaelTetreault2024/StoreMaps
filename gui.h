#ifndef GUI
#define GUI

#include "store.h"
#include "item.h"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

class makeGUI{
    public:
        void draw(Store store);
        void createShape(sf::Uint8 rValue, sf::Uint8 gValue, sf::Uint8 bValue, int width, int height, int xPos, int yPos, vector<sf::RectangleShape> &shapes);
        void createText(string text, int size, sf::Color color, int xPos, int yPos, sf::Font &font, vector<sf::Text> &texts);
};

#endif