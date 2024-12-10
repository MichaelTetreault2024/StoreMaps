#include "gui.h"
#include "store.h"
#include "item.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <vector>
using namespace std;
using namespace sf;


void makeGUI::draw(Store store)
{   
    // create the window
    RenderWindow window(VideoMode(1000, 1000), store.getStoreName());
    
    // creates all the shapes using the function createShape
    // passes in rgb values, width, height, x position on screen, y position on screen, and the vector shapes passed by reference
    vector<RectangleShape> shapes;
    createShape(128, 128, 128, 700, 700, 0, 0, shapes); // map background
    createShape(90, 90, 90, 1000, 50, 0, 0, shapes); // map title background
    createShape(180, 180, 180, 700, 290, 0, 710, shapes); // map legend background
    createShape(90, 90, 90, 700, 50, 0, 710, shapes); // title legend background
    createShape(180, 180, 180, 290, 980, 710, 50, shapes); // actions background
    createShape(90, 90, 90, 10, 1000, 990, 0, shapes); // right side screen border
    createShape(90, 90, 90, 700, 10, 0, 700, shapes); // map bottom border
    createShape(90, 90, 90, 10, 1000, 700, 0, shapes); // map right border
    createShape(90, 90, 90, 10, 1000, 0, 0, shapes); // left border
    createShape(90, 90, 90, 1000, 10, 0, 990, shapes); // legend bottom border
    createShape(128, 128, 128, 30, 30, 40, 800, shapes); // shelf legend key
    createShape(30, 144, 255, 30, 30, 40, 850, shapes); // path legend key
    createShape(173, 216, 230, 30, 30, 40, 900, shapes); // isle legend key
    createShape(30, 144, 255, 30, 30, 400, 800, shapes); // current location legend key blue box
    createShape(161, 40, 48, 30, 30, 400, 900, shapes); // goal legend key
    createShape(255, 255, 255, 250, 40, 725, 130, shapes); // input text for current location box
    createShape(255, 255, 255, 250, 40, 725, 260, shapes); // input text for goal location
    createShape(255, 255, 255, 250, 40, 725, 390, shapes); // clear / reset white box
    createShape(255, 255, 255, 250, 40, 725, 520, shapes); // close white box

    // loads in the font from a file
    Font font; 
    cout << "Current working directory: " << filesystem::current_path() << endl;
    //font.loadFromFile("Courier_New.ttf");
    
    
    // creates all the text on the screen using the function createText
    // passes in the string to display, character size, color, x position on screen, y position on screen, then font and texts vector by reference
    vector<Text> texts;
    createText("Map of " + store.getStoreName(), 30, Color::Black, 260, 5, font, texts); // map title text
    createText("Map Key", 30, Color::Black, 285, 708, font, texts); // legend title text
    createText("Shelf / Wall", 20, Color::Black, 90, 802, font, texts); // sheld legend key text
    createText("Isle", 20, Color::Black, 90, 902, font, texts); // isle legend key text
    createText("Path to Item", 20, Color::Black, 90, 850, font, texts); // path legend key text
    createText("Item Location", 20, Color::Black, 460, 902, font, texts); // goal legend key text
    createText("Actions", 30, Color::Black, 810, 5, font, texts); // actions title text
    createText("Input Closest Item", 18, Color::Black, 725, 90, font, texts); // input closest item text
    createText("Input Goal Item", 18, Color::Black, 725, 220, font, texts); // input goal item text
    createText("Current Location", 20, Color::Black, 460, 803, font, texts); // current location text in map legend
    createText("Clear / Reset", 25, Color::Black, 750, 395, font, texts); // clear / reset text
    createText("Close", 25, Color::Black, 820, 525, font, texts); // close text
    createText("If you input an item and it does", 13, Color::Black, 718, 670, font, texts); // close text
    createText("not appear on map then it doesnt", 13, Color::Black, 718, 700, font, texts); // close text
    createText("exist in the store. If two items", 13, Color::Black, 718, 730, font, texts); // close text
    createText("are inputted and display but no", 13, Color::Black, 718, 760, font, texts); // close text
    createText("path appears then no path exists.", 13, Color::Black, 718, 790, font, texts); // close text
    
    // gets the store layout from the store object passed in
    vector<vector<char>> layout = store.getLayout();

    // gets approximate width and height for squares made to display the map
    // these are calculated by taking the width or height of the window and dividing it by how many squares are need or
    // in other words divided by the length or height of the layout 2d vector
    double width = 690 / layout.size();
    double height = 650 / layout.at(0).size();

    // used to see if user has clicked on either box so that the program can start seeiing if they enter text  
    bool checkInputCurrLocation = false;
    bool checkInputGoalLocation = false;

    // used to see if user inputted text for a goal item and pressed enter 
    // then program sets this to true and an if statement inside window loop check if true 
    // and if so draws the goal item location onto the map
    bool drawGoal = false;
    bool drawCurr = false;

    // just check if user has pressed enter after inputting an item they are closest too
    // then if both this and drawGoal are true the program tries to find the shortest path
    // between the two and draws it to the map
    bool locationInputted = false;

    // strings to hold characters the user inputs after clicking on one of the text boxes
    string inputCurrLocationString;
    string inputGoalLocationString;

    // inits x and y positions for the goal item and currrent location
    // I have these here because they get used in different branches
    int xPositionGoal; 
    int yPositionGoal; 
    int xPositionCurr;
    int yPositionCurr;

    // boolean to check if the path has been made
    bool pathMade = false;

    // boolean to check if user has pressed close button
    bool closePressed = false;

    // init the path vector
    vector<pair<int,int>> path;
    
    // window loop to keep the window open
    while (window.isOpen())
    {
        // creates event object
        Event event;
        // check for event like closing the window
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed){
                window.close();
            }

            // check to see if user has clicked in either of text boxes
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left){
                if (Mouse::getPosition(window).x < 975 && Mouse::getPosition(window).x > 725){
                    if (Mouse::getPosition(window).y < 170 && Mouse::getPosition(window).y > 130){
                        checkInputCurrLocation = true;
                        drawCurr = false;
                    }
                    else if (Mouse::getPosition(window).y < 300 && Mouse::getPosition(window).y > 260){
                        drawGoal = false;
                        checkInputGoalLocation = true;
                    }
                    else if (Mouse::getPosition(window).y < 430 && Mouse::getPosition(window).y > 390){
                        drawGoal = false;
                        drawCurr = false;
                        path.clear();
                        inputCurrLocationString.clear();
                        inputGoalLocationString.clear();
                        checkInputCurrLocation = false;
                        checkInputGoalLocation = false;
                    }
                    else if (Mouse::getPosition(window).y < 560 && Mouse::getPosition(window).y > 520){
                        closePressed = true;
                    }
                    
                }
            }

            // if user has clicked on the closest item text box and if other is false to make sure you cant type in both of them
            if (checkInputCurrLocation && !checkInputGoalLocation){
                // if they enter any letter add it to string
                if (event.type == Event::TextEntered){
                    if (event.text.unicode < 128){
                        inputCurrLocationString += static_cast<char>(event.text.unicode);
                    }
                }
                // if they enter backspace pop last character on string but only if string is not empty
                else if (event.key.code == Keyboard::BackSpace){
                    if (!inputCurrLocationString.empty()){
                        inputCurrLocationString.pop_back();
                    }
                }
                // if user press enter signaling they are done typing
                else if (event.key.code == Keyboard::Return){
                    // set booleans to let program know what to do
                    checkInputCurrLocation = false;
                    locationInputted = true;
                    drawCurr = true;
                    xPositionCurr = 10;
                    yPositionCurr = 50;
                    // init and find location of the item using the string name that the user inputs
                    pair<int, int> currLocation;
                    currLocation = store.findItemLocation(inputCurrLocationString);
                    if (currLocation.first == -1){
                        drawCurr = false;
                        locationInputted = false;
                    }
                    // since the location comes as vector indexes the program must multiply it by the width and height
                    // of each square and add that onto the position of the first square
                    xPositionCurr += currLocation.first * width;
                    yPositionCurr += currLocation.second * height;
                }
            }

            // if the user has clicked on box to enter the goal item and if other is false to make sure you cant type in both of them
            if (checkInputGoalLocation && !checkInputCurrLocation){
                // if they enter any letter add it to string
                if (event.type == Event::TextEntered){
                    if (event.text.unicode < 128){
                        inputGoalLocationString += static_cast<char>(event.text.unicode);
                    }
                }
                // if they enter backspace pop last character on string but only if string is not empty
                else if (event.key.code == Keyboard::BackSpace){
                    if (!inputGoalLocationString.empty()){
                        inputGoalLocationString.pop_back();
                    }
                }
                // if user press enter signaling they are done typing
                else if (event.key.code == Keyboard::Return){
                    xPositionGoal = 10;
                    yPositionGoal = 50;
                    checkInputGoalLocation = false;
                    drawGoal = true;
                    // find the item location using ffunction in store
                    pair<int, int> goalLocation, currLocation;
                    goalLocation = store.findItemLocation(inputGoalLocationString);
                    if (goalLocation.first == -1){
                        drawGoal = false;
                    }
                    // since the location comes as vector indexes the program must multiply it by the width and height
                    // of each square and add that onto the position of the first square
                    xPositionGoal += goalLocation.first * width;
                    yPositionGoal += goalLocation.second * height;
                }
            }
        }

        if (closePressed){
            break;
        }

        // if user has pressed enter and inputted text into closest item and goal item
        if (drawGoal == true && locationInputted == true){
            // find the shortest path to the item
            path = store.findPath(inputCurrLocationString, inputGoalLocationString);
            // erase the first and last value in the path because those are the current location and end location
            // I already draw those and do not want to draw over them
            path.erase(path.begin() + 0);
            path.erase(path.begin() + path.size() - 1);
            // set this to true to let the program know it can try and draw the path now
            pathMade = true;
            // set this to false so that the loop only does it once and doesnt do a BFS every time
            locationInputted = false;
            
        } 
        
        // clear window then draw all the shapes
        window.clear();
        
        // loops through shapes and texts to draw it all to window
        for (RectangleShape s: shapes) {window.draw(s);}
        for (Text t: texts) {window.draw(t);}

        // Draws the inputted text for closest item and goal item
        vector<Text> loopTexts;
        createText(inputCurrLocationString, 25, Color::Black, 730, 132, font, loopTexts);
        createText(inputGoalLocationString, 25, Color::Black, 730, 262, font, loopTexts);
        for (Text t: loopTexts){window.draw(t);}

        // after all thhe layout shapes are drawn draw the map over it
        // init the starting x and y positions of the squares
        int xPosition = 10, yPosition = 50;
        
        // loop through layout 2d vector
        for (size_t i = 0; i < layout.size(); i++){
            for (size_t z = 0; z < layout.at(0).size(); z++){
                RectangleShape test(Vector2f(0,0)); // create shape
                // set color based on character in vector
                if (layout.at(i).at(z) == '#'){
                    test.setFillColor(Color{128, 128, 128});
                } else {
                    test.setFillColor(Color{173, 216, 230});
                }
                // set size and position
                test.setSize(Vector2f(width,height));
                test.setPosition(Vector2f(xPosition,yPosition));
                window.draw(test); // draw the shape
                xPosition += width; // move over to right to draw next shape
            }
            // after doing one row move down to do next row
            yPosition += height;
            xPosition = 10; // set the row back at the left end of the window
        }

        // if the user has inputted a goal item draw it
        if (drawGoal){
            RectangleShape goalLocation(Vector2f(0,0));
            goalLocation.setFillColor(Color{161, 40, 48});
            goalLocation.setSize(Vector2f(width, height));
            // this position is calculated ealier i aint gonna explain it again, more comments above
            goalLocation.setPosition(Vector2f(xPositionGoal, yPositionGoal));
            window.draw(goalLocation);
        }

        if (drawCurr){
            RectangleShape currrLocationBack(Vector2f(0,0));
            currrLocationBack.setFillColor(Color{30, 144, 255});
            currrLocationBack.setSize(Vector2f(width, height));
            // this position is calculated ealier i aint gonna explain it again, more comments above
            currrLocationBack.setPosition(Vector2f(xPositionCurr, yPositionCurr));
            window.draw(currrLocationBack);
        }

        // if the path has been drawn
        if (pathMade){
            // get path length to use later
            int pathLength = path.size();
            
            // if no path was found
            if (pathLength <= 0){
                // do something to prompt user to try again
                
            } else{
                // init x and y postions
                int xPos = 10, yPos = 50;
                // init a vector of shapes to draw easier
                vector<RectangleShape> pathShapes;
                // loop through and draw shapes
                // it gets the second then first becasue of the row col / x y problem
                for (size_t i = 0; i < pathLength; i++){
                    createShape(30, 144, 255, width, height, xPos + path.at(i).second * width, yPos + path.at(i).first * height, pathShapes);
                }
                // loop through and draw the path
                for (RectangleShape r: pathShapes) {window.draw(r);}
                
            }
        }
        // display the window
        window.display();
    }
}

// helper function to make shapes on screen
void makeGUI::createShape(Uint8 rValue, Uint8 gValue, Uint8 bValue, int width, int height, int xPos, int yPos, vector<RectangleShape> &shapeVector){
    RectangleShape newShape(Vector2f(0,0));
    newShape.setFillColor(Color{rValue, gValue, bValue});
    newShape.setSize(Vector2f(width, height));
    newShape.setPosition(Vector2f(xPos, yPos));
    shapeVector.push_back(newShape);
}

// helper function to make text on screen
void makeGUI::createText(string text, int size, Color color, int xPos, int yPos, Font &font, vector<Text> &textsVector){
    Text newText;
    newText.setFont(font);
    newText.setString(text);
    newText.setCharacterSize(size);
    newText.setFillColor(color);
    newText.setPosition(Vector2f(xPos, yPos));
    textsVector.push_back(newText);
}
