#include <chrono>
#include <random>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include "TextureManager.h"


class Board{
public:

    std::map<std::string, sf::Texture> textures;

    struct Tile{
        bool mine = false;
        bool flagged = false;
        bool revealed = false;
        int mineNeighbors = 0;
        int row;
        int col;
        sf::Sprite sprite;


        Tile(int row, int col){
            this->row = row;
            this->col = col;
        }


        void setTexture(){
            if(!revealed){
                //display tile_hiddenSprite
                sprite.setTexture(TextureManager::getTexture("tile_hidden"));
            }else if(revealed && mine == true){
                sprite.setTexture(TextureManager::getTexture("mine"));
            }else if(revealed && mineNeighbors == 0){
                //display tile_revealedSprite
                sprite.setTexture(TextureManager::getTexture("tile_revealed"));
            }else{
                //display specific number sprite
                sprite.setTexture(TextureManager::getTexture("number_" + to_string(mineNeighbors)));
            }

            //set position of sprite
            sprite.setPosition((col * 32), (row * 32));
        }

        void setFlagTexture(){
            sprite.setTexture(TextureManager::getTexture("flag"));
            sprite.setPosition((col * 32), (row * 32));
        }

        void setMineTexture(){
            sprite.setTexture(TextureManager::getTexture("mine"));
            sprite.setPosition((col * 32), (row * 32));
        }

    };

    int numMines = 0;
    int numFlags = 0;
    int minutes = 0;
    int seconds = 0;
    int rows;
    int columns;
    std::vector<std::vector<Tile>> board;
    sf::Sprite mdOneSprite;
    sf::Sprite mdTwoSprite;
    sf::Sprite mdThreeSprite;
    sf::Sprite negativeSprite;
    sf::Sprite tm1Sprite;
    sf::Sprite tm2Sprite;
    sf::Sprite ts1Sprite;
    sf::Sprite ts2Sprite;



    Board(int numMines, int rows, int columns);
    void placeMines();
    void reset_board();
    void drawBoard(sf::RenderWindow& window);
    void drawMinesRemaining(sf::RenderWindow& window);
    void drawFlags(sf::RenderWindow& window);
    void drawMines(sf::RenderWindow& window);
    void drawTimer(sf::RenderWindow& window);
    void findMineNeighbors();
    void revealTiles(int row, int col);
    bool checkWin();

};



