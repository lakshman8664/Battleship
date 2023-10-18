#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
    
  private:
    int m_rows, m_cols;
    struct ship
    {
        int m_length;
        char m_symbol;
        std::string m_name;
    };
    vector<ship> battleships;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
: m_rows(nRows), m_cols(nCols)
{
    //if the passed in rows and columns exceed MAXROWS & MAXCOLS, set them = to the maxes
    if (m_rows > MAXROWS)
        m_rows = MAXROWS;
    if (m_cols > MAXCOLS)
        m_cols = MAXCOLS;
}

int GameImpl::rows() const
{
    return m_rows;  // return # rows
}

int GameImpl::cols() const
{
    return m_cols;  // return # of cols
}
bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols(); //check if point is valid position on board
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols())); //return random point on board
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    //check that length is valid
    if (length < 0 || length > rows() || length > cols())
    {
        return false;
    }
    
    //check that symbol hasn't been used
    
    
    for (int i = 1; i < battleships.size(); i++)
    {
        if (battleships[i].m_symbol == symbol)
            return false; //loop through vector and compare symbols to symbol that's being passed in to make new ship
    }
    if (symbol == '.' || symbol == 'o' || symbol == 'X' || symbol == '#')
        return false;
            
    ship newShip; //create new ship object
    newShip.m_symbol = symbol;
    newShip.m_length = length;
    newShip.m_name = name;
    battleships.push_back(newShip); //pushes a new ship object onto the vector and returns true
    
    return true;
}

int GameImpl::nShips() const
{
    return battleships.size();
}

int GameImpl::shipLength(int shipId) const
{
    return battleships.at(shipId).m_length;
}

char GameImpl::shipSymbol(int shipId) const
{
    return battleships.at(shipId).m_symbol;
}

string GameImpl::shipName(int shipId) const
{
    return battleships.at(shipId).m_name;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    //if both players cannot place their ships, return nullptr
    if (p1->placeShips(b1) == false || p2->placeShips(b2) == false)
        return nullptr;
    //GAME NOW STARTS
    
    //keep looping until one board doesn't have ships anymore
    while (b1.allShipsDestroyed() == false || b2.allShipsDestroyed() == false)
    {
        //if first player is human, display second player's board without revealing where ships are, and if it's a CPU player, display whole board
        cout << p1->name() << "'s turn. Board for " << p2->name() << ":" << endl;
        
        if (p1->isHuman())
            b2.display(true);
        else
            b2.display(false);
        
        //now, make first player attack
        bool shotHit = false;
        bool shipDestroyed = false;
        int shipId = -1;
        Point p = p1->recommendAttack(); //point that first player attacks
        
        //print message based on who attacked and if they hit/missed
        if (!b2.attack(p, shotHit, shipDestroyed, shipId))
        {
            //Shuman the Human wasted a shot at ___
            cout << p1->name() << " wasted a shot at (" << p.r << "," << p.c << ") ." << endl;
        }
        else
        {
            if (!shotHit)
            {
                //Shuman the Human attacked ___ and missed, resulting in:
                cout << p1->name() << " attacked (" << p.r << "," << p.c << ") and missed, resulting in: " << endl;
            }
            else if (shotHit == true && shipDestroyed == false)
            {
                //Shuman the Human attacked ___ and hit something, resulting in:
                cout << p1->name() << " attacked (" << p.r << "," << p.c << ") and hit something, resulting in: " << endl;
            }
            else if (shotHit == true && shipDestroyed == true)
            {
                //Mediocre Mimi attacked ____ and destroyed the ____, resulting in:
                cout << p1->name() << " attacked (" << p.r << "," << p.c << ") and destroyed the " << shipName(shipId) << ", resulting in: " << endl;
            }
        }
        
        p1->recordAttackResult(p, true, shotHit, shipDestroyed, shipId);
        
        //after attack, display board once again
        if (p1->isHuman())
            b2.display(true);
        else
            b2.display(false);
        
        //if board 2 has no ships left, player 1 has won, returning a pointer to player 1
        if (b2.allShipsDestroyed())
        {
            cout << p1->name() << " wins!" << endl;
            return p1;
        }
        //If the third parameter is true, then after displaying the result of each shot, the function displays a "Press enter to continue: " prompt and waits for the user to press enter before continuing
        if (shouldPause)
        {
            waitForEnter();
        }

        //now, it's player 2's turn
        
        cout << p2->name() << "'s turn. Board for " << p1->name() << ":" << endl;
        
        if (p2->isHuman())
            b1.display(true);
        else
            b1.display(false);
        
        //make second player attack
        shotHit = false;
        shipDestroyed = false;
        shipId = -1;
        Point q = p2->recommendAttack(); //point that first player attacks
        
        //print message based on who attacked and if they hit/missed
        if (!b1.attack(q, shotHit, shipDestroyed, shipId))
        {
            //Shuman the Human wasted a shot at ___
            cout << p2->name() << " wasted a shot at ( " << q.r << "," << q.c << ") ." << endl;
        }
        else
        {
            if (!shotHit)
            {
                //Shuman the Human attacked ___ and missed, resulting in:
                cout << p2->name() << " attacked (" << q.r << "," << q.c << ") and missed, resulting in: " << endl;
            }
            else if (shotHit == true && shipDestroyed == false)
            {
                //Shuman the Human attacked ___ and hit something, resulting in:
                cout << p2->name() << " attacked (" << q.r << "," << q.c << ") and hit something, resulting in: " << endl;
            }
            else if (shotHit == true && shipDestroyed == true)
            {
                cout << p2->name() << " attacked (" << q.r << "," << q.c << ") and destroyed the " << shipName(shipId) << ", resulting in: " << endl;
            }
        }
        
        p2->recordAttackResult(q, true, shotHit, shipDestroyed, shipId);
        
        //after attack, display board once again
        if (p2->isHuman())
            b1.display(true);
        else
            b1.display(false);
        
        //if board 1 has no ships left, player 2 has won, returning a pointer to player 2
        if (b1.allShipsDestroyed())
        {
            cout << p2->name() << " wins!" << endl;
            return p2;
        }
        
        //If the third parameter is true, then after displaying the result of each shot, the function displays a "Press enter to continue: " prompt and waits for the user to press enter before continuing
        if (shouldPause)
        {
            waitForEnter();
        }
    }
    return nullptr; 
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

