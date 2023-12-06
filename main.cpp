//Tyler Audino
//COP3503C
//Finished 12/3/23
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <map>
#include "Board.h"



void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }

std::vector<std::vector<std::string>> savetoLB(int minutes, int seconds, std::string name, std::string filePath, std::vector<std::vector<std::string>> leaderboardVec){

    name.pop_back();
    name += "*";
    for (int i = 0; i < 5 ; i++){
        int lbMinutes = stoi(leaderboardVec[i][0].substr(0, 2));
        int lbSeconds = stoi(leaderboardVec[i][0].substr(3, 2));
        if ((minutes < lbMinutes) | (minutes <= lbMinutes && seconds < lbSeconds)){
            std::string secString;
            std::string minString;
            if ((seconds / 10 % 10) == 0){
                secString = "0" + to_string(seconds);
            }else{
                secString = to_string(seconds);
            }
            if ((minutes / 10 % 10) == 0){
                minString = "0" + to_string(minutes);
            }else{
                minString = to_string(minutes);
            }

            std::string recordTime = minString + ":" + secString;
            std::vector<string> newRecord{recordTime, name};
            //auto pos = std::find(leaderboardVec.begin(),leaderboardVec.end(), leaderboardVec[i]);
            leaderboardVec.insert(leaderboardVec.begin() + i, newRecord);

            ofstream file(filePath);
            for (int i = 0; i < 5; i++){
                file << leaderboardVec[i][0] << "," << leaderboardVec[i][1] << endl;
            }
            return leaderboardVec;
        }
    }

    return leaderboardVec;
}

std::string getLBstring(std::vector<std::vector<std::string>> leaderboardVec){
    std::string lba[5][3] = {"1.", "", "", "2.", "","", "3.",
                             "", "", "4.", "", "", "5.", "", ""};
    for (int i = 0; i < 5 ; i++){
        lba[i][1] = leaderboardVec[i][0];
        lba[i][2] = leaderboardVec[i][1];
    }

    std::string lbtext = "";
    for (int i = 0; i < 5; i++){
        lbtext += lba[i][0] + "\t" + lba[i][1] + "\t" + lba[i][2] + "\n\n";
    }

    return lbtext;
}

std::vector<std::vector<std::string>> asterickRemove(std::vector<std::vector<std::string>> leaderboardVec){
    for (int i = 0; i < 5; i++){
        if (leaderboardVec[i][1].find('*') != std::string::npos){
            leaderboardVec[i][1].pop_back();
        }
    }
    return leaderboardVec;
}

int main() {
    //get config info
    std::ifstream config;
    config.open("files/config.cfg");
    std::vector<int> configVec;
    std::string line;
    while(std::getline(config, line)){
        std::istringstream stream(line);
        std::string buffer;
        getline(stream, buffer);
        configVec.push_back(stoi(buffer));
    }

    bool gameOpen = false;
    bool gameWon = false;
    bool gameLost = false;
    bool paused = false;
    bool pausedPrev = false;
    bool leaderboardOpen = false;
    bool debugMode = false;
    int clickRow;
    int clickCol;
    const int Columns = configVec[0];
    const int Rows = configVec[1];
    const int Mines = configVec[2];
    const int width = Columns * 32;
    const int height = (Rows * 32) + 100;
    const int widthL = Columns * 16;
    const int heightL = (Rows * 16) + 50;

    sf::RenderWindow welcome(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
    sf::Text welcomeText;
    sf::Font font;
    font.loadFromFile("files/font.ttf");
    
    welcomeText.setFont(font);
    welcomeText.setString("WELCOME TO MINESWEEPER!");
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcomeText, width/2, (height/2) - 150);
    
    sf::Text enterNameText;
    enterNameText.setFont(font);
    enterNameText.setString("Enter your name:");
    enterNameText.setCharacterSize(20);
    enterNameText.setFillColor(sf::Color::White);
    enterNameText.setStyle(sf::Text::Bold);
    setText(enterNameText, width/2, (height/2) - 75);

    std::string name[10];
    for (int i = 0; i < 10; i++){
        name[i] = "";
    }

    std::string displayName = "";

    int currentChar = 0;

    sf::Text playerName;
    playerName.setFont(font);
    playerName.setString("|");
    playerName.setCharacterSize(18);
    playerName.setFillColor(sf::Color::Yellow);
    playerName.setStyle(sf::Text::Bold);
    setText(playerName, width/2, (height/2) - 45);

// welcome window loop
    while(welcome.isOpen()) {
        sf::Event event;
        while(welcome.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                welcome.close();
            }

            //get user name from text
            if(event.type == sf::Event::TextEntered){

                if (event.text.unicode == '\b' && currentChar > 0){
                    currentChar -= 1;
                    name[currentChar] = "";
                    displayName = "";
                    for (int i = 0; i < currentChar; i++){
                        displayName += name[i];
                    }
                    displayName += "|";
                    playerName.setString(displayName);
                    setText(playerName, width/2, (height/2) - 45);

                }else if (event.text.unicode == '\b'){
                    //nothing happens when nothing to backspace
                }else if (std::isalpha(event.text.unicode) && currentChar < 10){
                     if (currentChar == 0){
                         name[currentChar] = std::toupper(static_cast<char>(event.text.unicode));
                     }else{
                         name[currentChar] = std::tolower(static_cast<char>(event.text.unicode));
                     }
                     currentChar += 1;
                     displayName = "";
                     for (int i = 0; i < currentChar; i++){
                         displayName += name[i];
                     }
                     if (currentChar != 10){
                         displayName += "|";
                     }
                     playerName.setString(displayName);
                     setText(playerName, width/2, (height/2) - 45);
                }
            }

            if(event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && currentChar != 0){
                //close welcome window move to game window loop
                welcome.close();
                gameOpen = true;
            }
        }

        welcome.clear(sf::Color::Blue);
        welcome.draw(welcomeText);
        welcome.draw(enterNameText);
        welcome.draw(playerName);
        welcome.display();
    }

    //don't open game loop if window closed and no name entered
    if (!gameOpen){
        return 0;
    }

    //render game window
    sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);


    //load textures that aren't tiles or digits
    sf::Sprite debugSprite;
    debugSprite.setTexture(TextureManager::getTexture("debug"));
    debugSprite.setPosition((Columns*32)-304,(Rows + 0.5) * 32);

    sf::Sprite faceSprite;
    faceSprite.setTexture(TextureManager::getTexture("face_happy"));
    faceSprite.setPosition(((Columns/2.0)*32) - 32, (Rows + 0.5)*32);

    sf::Sprite leaderboardSprite;
    leaderboardSprite.setTexture(TextureManager::getTexture("leaderboard"));
    leaderboardSprite.setPosition((Columns*32) - 176, (Rows + 0.5)*32);

    sf::Sprite ppSprite;
    ppSprite.setTexture(TextureManager::getTexture("pause"));
    ppSprite.setPosition((Columns*32) - 240, (Rows + 0.5)*32);


    //create text for leaderboard
    //read leaderboard file
    std::ifstream leaderboardFile;
    leaderboardFile.open("files/leaderboard.txt");
    std::vector<std::vector<std::string>> leaderboardVec;
    int counter = 0;
    while(std::getline(leaderboardFile, line)){
        std::istringstream stream(line);
        std::string bufferTime;
        std::string bufferName;
        getline(stream, bufferTime, ',');
        getline(stream, bufferName, ',');
        leaderboardVec.push_back(std::vector<std::string>());
        leaderboardVec[counter].push_back(bufferTime);
        leaderboardVec[counter].push_back(bufferName);
        counter += 1;
    }

    leaderboardFile.close();


    sf::Text leaderboardText;
    leaderboardText.setFont(font);
    leaderboardText.setString("LEADERBOARD");
    leaderboardText.setCharacterSize(20);
    leaderboardText.setFillColor(sf::Color::White);
    leaderboardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(leaderboardText, widthL/2, (heightL/2) - 120);

    sf::Text leaderboardNames;
    leaderboardNames.setFont(font);
    leaderboardNames.setString(getLBstring(leaderboardVec));
    leaderboardNames.setCharacterSize(18);
    leaderboardNames.setFillColor(sf::Color::White);
    leaderboardNames.setStyle(sf::Text::Bold);
    setText(leaderboardNames, widthL/2, (heightL/2) + 20);


    //generate inital board
    chrono::high_resolution_clock::time_point start_time = chrono::high_resolution_clock::now();//starting the timer when the program begins to run!
    Board board(Mines, Rows, Columns);
    //board to show when paused or lb open
    Board empty(0, Rows, Columns);
    //to overlay flags
    Board flags(0, Rows, Columns);
    Board mines(0, Rows, Columns);
    empty.revealTiles(0, 0);


    chrono::high_resolution_clock::time_point pauseTime = chrono::high_resolution_clock::now();
    auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count();


    //create game loop

    while(gameWindow.isOpen()) {
        sf::Event event;


        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
            }
            //normal GamePlay
            if (!gameLost && !gameWon) {
                 if (event.type == sf::Event::MouseButtonPressed) {
                     sf::Vector2<float> clickPos = gameWindow.mapPixelToCoords(sf::Mouse::getPosition(gameWindow));
                     if (event.mouseButton.button == sf::Mouse::Right && !paused) {
                         //right click places flags
                         clickRow = (int) clickPos.y / 32;
                         clickCol = (int) clickPos.x / 32;

                         if((clickCol > board.columns) | (clickRow > board.rows)){
                             //prevents crashing if click not on tile
                         }else {
                             //place or remove flag

                             if (!board.board[clickRow][clickCol].flagged && !board.board[clickRow][clickCol].revealed){
                                 board.board[clickRow][clickCol].flagged = true;
                                 flags.board[clickRow][clickCol].flagged = true;
                                 board.numFlags += 1;
                             }else if(board.board[clickRow][clickCol].flagged){
                                 board.board[clickRow][clickCol].flagged = false;
                                 flags.board[clickRow][clickCol].flagged = false;
                                 board.numFlags -= 1;
                             }

                         }

                     } else if (event.mouseButton.button == sf::Mouse::Left) {
                         //leaderboard
                         if (leaderboardSprite.getGlobalBounds().contains(clickPos)) {
                             leaderboardOpen = true;
                             if(paused){
                                 pausedPrev = true;
                             }else{
                                 paused = true;
                                 pauseTime = chrono::high_resolution_clock::now();
                             }
                         } else if (debugSprite.getGlobalBounds().contains(clickPos) && !paused) {
                             debugMode = !debugMode;
                             for(int i = 0; i < board.rows; i++){
                                 for (int j = 0; j < board.columns; j++) {
                                     if(board.board[i][j].mine){
                                         //overlay mine
                                         mines.board[i][j].mine = true;
                                     }
                                 }
                             }
                         } else if (faceSprite.getGlobalBounds().contains(clickPos)) {
                             //face (restart game)
                             //reset to happy if game ended
                             faceSprite.setTexture(TextureManager::getTexture("face_happy"));
                             gameWon = false;
                             gameLost = false;
                             paused = false;
                             ppSprite.setTexture(TextureManager::getTexture("pause"));
                             board.reset_board();
                             flags.reset_board();
                             mines.reset_board();
                             start_time = chrono::high_resolution_clock::now();
                             pauseTime = chrono::high_resolution_clock::now();
                             elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count();
                         } else if (ppSprite.getGlobalBounds().contains(clickPos)) {
                             paused = !paused;
                             if (paused) {
                                 //PAUSE GAME
                                 ppSprite.setTexture(TextureManager::getTexture("play"));
                                 pauseTime = chrono::high_resolution_clock::now();
                             } else {
                                 //unpaused
                                 chrono::high_resolution_clock::time_point unPausedTime = chrono::high_resolution_clock::now();
                                 elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count(); //Addition is necessary for when hitting the pause button more than once
                                 ppSprite.setTexture(TextureManager::getTexture("pause"));
                             }

                         } else if(!paused){
                             //when tile left clicked
                             //determine which tile
                             clickRow = (int) clickPos.y / 32;
                             clickCol = (int) clickPos.x / 32;

                             if ((clickCol >= board.columns) | (clickRow >= board.rows)) {
                                 //prevents crashing if click not on tile
                             } else if (board.board[clickRow][clickCol].mine && !board.board[clickRow][clickCol].flagged) {
                                 //determine if mine WHEN NOT FLAGGED
                                 gameLost = true;
                                 paused = true;
                                 flags.reset_board();
                                 mines.reset_board();
                                 board.board[clickRow][clickCol].revealed = true;
                                 //reveal all
                                 for (int i = 0; i < board.rows; i++) {
                                     for (int j = 0; j < board.columns; j++) {
                                         if (board.board[i][j].mine == true) {
                                             board.board[i][j].revealed = true;
                                         }

                                     }
                                 }

                                 faceSprite.setTexture(TextureManager::getTexture("face_lose"));
                             } else if (board.board[clickRow][clickCol].flagged) {
                                 //nothing because flagged needs to be unflagged
                             } else if(board.board[clickRow][clickCol].revealed){
                                 //if number tile, check if enough flags to remove others
                                 //can lose game if a mine is incorrectly revealed
                                 gameLost = board.revealNumTiles(clickRow, clickCol);
                                 gameWon = board.checkWin();

                                 if(gameLost){
                                     paused = true;
                                     flags.reset_board();
                                     mines.reset_board();
                                     for (int i = 0; i < board.rows; i++) {
                                         for (int j = 0; j < board.columns; j++) {
                                             if (board.board[i][j].mine == true) {
                                                 board.board[i][j].revealed = true;
                                             }

                                         }
                                     }
                                 }else if (gameWon){
                                     faceSprite.setTexture(TextureManager::getTexture("face_win"));
                                     paused = true;
                                     leaderboardVec = savetoLB(board.minutes, board.seconds, displayName,
                                                               "files/leaderboard.txt", leaderboardVec);
                                     leaderboardNames.setString(getLBstring(leaderboardVec));
                                     leaderboardOpen = true;
                                     debugMode = false;
                                     //put flags on mines
                                     for (int i = 0; i < board.rows; i++) {
                                         for (int j = 0; j < board.columns; j++) {
                                             if (board.board[i][j].mine == true) {
                                                 board.board[i][j].revealed = false; // for debug mode
                                                 flags.board[i][j].flagged = true;

                                             }
                                             board.numFlags = Mines;

                                         }
                                     }
                                 }
                             }else{
                                 //reveal if not mine
                                 board.board[clickRow][clickCol].revealed = true;
                                 if (board.board[clickRow][clickCol].mineNeighbors == 0) {
                                     board.revealTiles(clickRow, clickCol);
                                 }

                                 //determine if win after revealing
                                 gameWon = board.checkWin();
                                 if (gameWon) {
                                     faceSprite.setTexture(TextureManager::getTexture("face_win"));
                                     paused = true;
                                     leaderboardVec = savetoLB(board.minutes, board.seconds, displayName,
                                                               "files/leaderboard.txt", leaderboardVec);
                                     leaderboardNames.setString(getLBstring(leaderboardVec));
                                     leaderboardOpen = true;
                                     debugMode = false;
                                     //put flags on mines
                                     for (int i = 0; i < board.rows; i++) {
                                         for (int j = 0; j < board.columns; j++) {
                                             if (board.board[i][j].mine == true) {
                                                 board.board[i][j].revealed = false; // for debug mode
                                                 flags.board[i][j].flagged = true;

                                             }
                                             board.numFlags = Mines;

                                         }
                                     }
                                 }
                             }
                         }
                     }
                 }

            }else if (gameWon | gameLost){
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2<float> clickPos = gameWindow.mapPixelToCoords(sf::Mouse::getPosition(gameWindow));
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (leaderboardSprite.getGlobalBounds().contains(clickPos)) {
                            //leaderboard
                            leaderboardOpen = true;
                        } else if (faceSprite.getGlobalBounds().contains(clickPos)) {
                            //face (restart game)
                            //reset to happy if game ended
                            faceSprite.setTexture(TextureManager::getTexture("face_happy"));
                            gameWon = false;
                            gameLost = false;
                            paused = false;
                            board.reset_board();
                            flags.reset_board();
                            mines.reset_board();
                            leaderboardVec = asterickRemove(leaderboardVec);
                            leaderboardNames.setString(getLBstring(leaderboardVec));
                            start_time = chrono::high_resolution_clock::now();
                            pauseTime = chrono::high_resolution_clock::now();
                            elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count();

                        }
                    }
                }
            }
        }




        //this finds the time elapsed, so the current time - the time the window opened.
        auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
        int total_time = game_duration.count(); // necessary to subtract elapsed time later because "game_duration.count()" is const

        if(!paused) {
            //enters if the game is NOT paused. This is the condition that keeps the timer from incrementing when paused.
            //cout << "not paused\n";
            total_time =  total_time - elapsed_paused_time; //
            board.minutes = total_time / 60;
            board.seconds = total_time % 60;
        }



        // display window
        gameWindow.clear(sf::Color::White);
        gameWindow.draw(faceSprite);
        gameWindow.draw(debugSprite);
        gameWindow.draw(leaderboardSprite);
        gameWindow.draw(ppSprite);
        empty.drawBoard(gameWindow);
        if((!paused && !leaderboardOpen) | gameWon | gameLost){
            board.drawBoard(gameWindow);
            if(debugMode){
                mines.drawMines(gameWindow);
            }
            flags.drawFlags(gameWindow);
        }
        board.drawMinesRemaining(gameWindow);
        board.drawTimer(gameWindow);
        gameWindow.display();

        if(leaderboardOpen){
            sf::RenderWindow leaderboard(sf::VideoMode(widthL, heightL), "Minesweeper", sf::Style::Close);
            while(leaderboard.isOpen()) {
                sf::Event event;
                while (leaderboard.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        leaderboard.close();
                        leaderboardOpen = false;
                        if (!gameWon && !gameLost && !pausedPrev){
                            paused = false;
                            chrono::high_resolution_clock::time_point unPausedTime = chrono::high_resolution_clock::now();
                            elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count(); //Addition is necessary for when hitting the pause button more than once

                        }else{
                            pausedPrev = false;
                        }

                    }
                }
                leaderboard.clear(sf::Color::Blue);
                leaderboard.draw(leaderboardText);
                leaderboard.draw(leaderboardNames);
                leaderboard.display();
            }
        }



    }

    //reset and write leaderboard in case close without winning after restarting
    leaderboardVec = asterickRemove(leaderboardVec);
    leaderboardNames.setString(getLBstring(leaderboardVec));
    ofstream file("files/leaderboard.txt");
    for (int i = 0; i < 5; i++){
        file << leaderboardVec[i][0] << "," << leaderboardVec[i][1] << endl;
    }
    file.close();
    return 0;
}
