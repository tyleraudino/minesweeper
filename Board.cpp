#include "Board.h"



Board::Board(int numMines,  int rows, int columns) {
    //initialize board and tiles
    std::vector<std::vector<Tile>> board;
    for(int i = 0; i < rows; i++){
        board.push_back(std::vector<Tile>());
        for (int j = 0; j < columns; j++) {
            board[i].push_back(Tile(i, j));
        }

    }

    this->board = board;
    this->numMines = numMines;
    this->rows = rows;
    this->columns = columns;

    mdOneSprite.setTexture(TextureManager::getTexture("digits"));
    mdTwoSprite.setTexture(TextureManager::getTexture("digits"));
    mdThreeSprite.setTexture(TextureManager::getTexture("digits"));
    negativeSprite.setTexture(TextureManager::getTexture("digits"));
    tm1Sprite.setTexture(TextureManager::getTexture("digits"));
    tm2Sprite.setTexture(TextureManager::getTexture("digits"));
    ts1Sprite.setTexture(TextureManager::getTexture("digits"));
    ts2Sprite.setTexture(TextureManager::getTexture("digits"));

    negativeSprite.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
    mdOneSprite.setPosition(33, 32*(rows + 0.5) + 16);
    mdTwoSprite.setPosition(33 + 21, 32*(rows + 0.5) + 16);
    mdThreeSprite.setPosition(33 + (2*21), 32*(rows + 0.5) + 16);
    negativeSprite.setPosition(12, 32*(rows + 0.5) + 16);
    tm1Sprite.setPosition((columns * 32) - 97, 32*(rows + 0.5) + 16);
    tm2Sprite.setPosition((columns * 32) - 97 + 21, 32*(rows + 0.5) + 16);
    ts1Sprite.setPosition((columns * 32) - 54, 32*(rows + 0.5) + 16);
    ts2Sprite.setPosition((columns * 32) - 54 + 21, 32*(rows + 0.5) + 16);
    placeMines();


}

void Board::placeMines(){
    int minesplaced = 0;
    int row;
    int col;
    while(minesplaced < numMines){
        row = (rand() % rows);
        col = (rand() % columns);

        //check there's no mile already
        if (board[row][col].mine != true){
            board[row][col].mine = true;
            minesplaced += 1;
        }
    }

    //print to check
    /*for(int i = 0; i < rows; i++){
        std::cout << std::endl;
        for (int j = 0; j < columns; j++) {
            std::cout << int(this->board[i][j].mine);
        }

    }*/

    findMineNeighbors();
}

void Board::reset_board(){
    //check this
    numFlags = 0;
    minutes = 0;
    seconds = 0;
    //reset mines
    for(int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++) {
            board[i][j].mine = false;
            board[i][j].revealed = false;
            board[i][j].flagged = false;
            board[i][j].mineNeighbors = 0;
        }
    }

    //get new mines
    placeMines();

}

void Board::drawBoard(sf::RenderWindow& window){
    for(int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++) {
            board[i][j].setTexture();
            window.draw(board[i][j].sprite);
        }
    }
}

void Board::drawFlags(sf::RenderWindow& window){
    for(int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++) {
            if(board[i][j].flagged){
                board[i][j].setFlagTexture();
                window.draw(board[i][j].sprite);
            }
        }
    }
}

void Board::drawMines(sf::RenderWindow& window){
    for(int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++) {
            if(board[i][j].mine){
                board[i][j].setMineTexture();
                window.draw(board[i][j].sprite);
            }
        }
    }
}

void Board::drawMinesRemaining(sf::RenderWindow& window) {int mineDisplay;
    unsigned int digitOne = 0;
    unsigned int digitTwo = 0;
    unsigned int digitThree = 0;
    mineDisplay = numMines - numFlags;
    digitOne = (unsigned int) (mineDisplay/100) % 10;
    digitTwo = (unsigned int) (mineDisplay/10) % 10;
    digitThree = (unsigned int) mineDisplay % 10;

    if (mineDisplay < 0){
        //if negative
        digitOne = (unsigned int) ((-1 * mineDisplay)/100) % 10;
        digitTwo = (unsigned int) ((-1 * mineDisplay)/10) % 10;
        digitThree = (unsigned int) (-1 * mineDisplay) % 10;
        window.draw(negativeSprite);
    }

    mdOneSprite.setTextureRect(sf::IntRect(digitOne * 21, 0, 21, 32));
    mdTwoSprite.setTextureRect(sf::IntRect(digitTwo * 21, 0, 21, 32));
    mdThreeSprite.setTextureRect(sf::IntRect(digitThree * 21, 0, 21, 32));
    window.draw(mdOneSprite);
    window.draw(mdTwoSprite);
    window.draw(mdThreeSprite);
}

void Board::drawTimer(sf::RenderWindow &window) {
    int minutes0 = minutes / 10 % 10; //minutes index 0
    int minutes1 = minutes % 10; // minutes index 1
    int seconds0 = seconds / 10 % 10; // seconds index 0
    int seconds1 = seconds % 10; // seconds index 1

    tm1Sprite.setTextureRect(sf::IntRect(minutes0 * 21, 0, 21, 32));
    tm2Sprite.setTextureRect(sf::IntRect(minutes1* 21, 0, 21, 32));
    ts1Sprite.setTextureRect(sf::IntRect(seconds0 * 21, 0, 21, 32));
    ts2Sprite.setTextureRect(sf::IntRect(seconds1 * 21, 0, 21, 32));
    window.draw(tm1Sprite);
    window.draw(tm2Sprite);
    window.draw(ts1Sprite);
    window.draw(ts2Sprite);

}

void Board::findMineNeighbors() {

    for(int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++) {
            int numMines = 0;
            if (board[i][j].mine){
                continue;
            }

            for (int row = i - 1; (row < i + 2) && (row < rows); row++){
                for(int col = j - 1; (col < j + 2) && (col < columns); col++){
                    //make sure not checking tiles that don't exist
                    if(row < 0){
                        row = 0;
                    }else if (row == rows){
                        row = rows - 1;
                    }

                    if(col < 0){
                        col = 0;
                    }else if(col == columns){
                        col = columns - 1;
                    }
                    //check if mines
                    if (board[row][col].mine){
                            numMines += 1;
                    }
                }
            }
            board[i][j].mineNeighbors = numMines;
            }
        }
    }

void Board::revealTiles(int row, int col){
    for (int i = row - 1; (i < row + 2) && (i < rows); i++){
        for(int j = col - 1; (j < col + 2) && (j < columns); j++){
            //make sure not checking tiles that don't exist
            if(i < 0){
                i = 0;
            }else if (i == rows){
                i = rows - 1;
            }

            if(j < 0){
                j = 0;
            }else if(j == columns){
                j = columns - 1;
            }
            //check if number
            if (!board[i][j].revealed && !board[i][j].mine && !board[i][j].flagged){
                board[i][j].revealed = true;
                //recursion and check this one's neighbors
                if (board[i][j].mineNeighbors == 0){
                    revealTiles(i, j);
                }
            }
        }
    }
}

bool Board::checkWin() {
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            // check that all non-mine tiles are revealed, if one is, end loop and return false;
            if (!board[i][j].mine && !board[i][j].revealed){
                return false;
            }

        }
    }

    return true;
}