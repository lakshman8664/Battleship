#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
    const Game& m_game;
    char m_board[MAXROWS][MAXCOLS]; //private variable for Board class
};

BoardImpl::BoardImpl(const Game& g)
: m_game(g)
{
    clear(); //initializes board to empty array
}

void BoardImpl::clear()
{
    
    //create empty board
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            m_board[r][c] = '.';
        }
    }
}

void BoardImpl::block()
{
    int numBlocked = 0;
    int numCells = .5 * (m_game.rows() * m_game.cols());
      // Block cells with 50% probability

    while (numBlocked < numCells) //keep looping while # of blocked cells is less than half of cells
    {
        for (int r = 0; r < m_game.rows(); r++)
        {
            for (int c = 0; c < m_game.cols(); c++)
            {
                if (m_board[r][c] != '#') //if it isn't already blocked, repeat randInt
                {
                    if (randInt(2) == 0)
                    {
                        numBlocked++; //increment this every time we place a #
                        m_board[r][c] = '#';
                    }
                }
                if (numBlocked == numCells) //if numBlocked has reached the desired number, break out of all loops
                    break;
            }
            if (numBlocked == numCells)
                break;
        }
    }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (m_board[r][c] == '#')
            {
                m_board[r][c] = '.';
            }
        }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    //check that initial point is valid
    if (!(m_game.isValid(topOrLeft)))
        return false;
    if (m_board[topOrLeft.r][topOrLeft.c] == 'X' || m_board[topOrLeft.r][topOrLeft.c] == 'o' || m_board[topOrLeft.r][topOrLeft.c] == '#')
        return false;
    //check if shipID is valid
    if (shipId < 0 || shipId > (m_game.nShips() - 1))
        return false;
    
    //check that shipID has already been used on this Board
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (m_board[r][c] == m_game.shipSymbol(shipId))
            {
                return false;
            }
        }
    }
    if (dir == HORIZONTAL)
    {
        //check that ship length doesn't go off right side of board
        if ((topOrLeft.c + m_game.shipLength(shipId)) > m_game.cols())
        {
            return false;
        }
        
        //loop through columns untl ship length is reached and check that ship is able to be placed
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            //check that point is a free spot at each column
            if (m_board[topOrLeft.r][topOrLeft.c + i] != '.')
            {
                return false;
            }
            //m_board[topOrLeft.r][topOrLeft.c + i] = m_game.shipSymbol(shipId); //accesses ship symbol and assigns it to free spot
        }
       // cout << "good\n";
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            //check that point is a free spot at each column
            m_board[topOrLeft.r][topOrLeft.c + i] = m_game.shipSymbol(shipId); //accesses ship symbol and assigns it to free spot
        }
    }
    
    if (dir == VERTICAL)
    {
        //check that ship length doesn't go off bottom of board
        if ((topOrLeft.r + m_game.shipLength(shipId)) > m_game.rows())
        {
            return false;
        }
        
        //loop through rows until ship length is reached and check that ship is able to be placed
        for (int k = 0; k < m_game.shipLength(shipId); k++)
        {
            //check that point is a free spot at each row
            if (m_board[topOrLeft.r + k][topOrLeft.c] != '.')
            {
                return false;
            }
        }
        for (int k = 0; k < m_game.shipLength(shipId); k++)
        {
           
            m_board[topOrLeft.r + k][topOrLeft.c] = m_game.shipSymbol(shipId); //accesses ship symbol and assigns it to free spot
        }
    }
    return true; //if ship has been successfully placed
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    //check that initial point is valid
    if (!(m_game.isValid(topOrLeft)))
        return false;
    
    //check if shipID is valid
    if (shipId < 0 || shipId > (m_game.nShips() - 1))
        return false;
    
    if (dir == HORIZONTAL)
    {
        //loop through columns untl ship length is reached and check that ship is able to be unplaced
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            //check that point is a free spot at each column
            if (m_board[topOrLeft.r][topOrLeft.c + i] != m_game.shipSymbol(shipId))
            {
                return false;
            }
        }
        //if ship is able to be unplaced, loop through once again and unplace ship
        for (int i2 = 0; i2 < m_game.shipLength(shipId); i2++)
        {
            m_board[topOrLeft.r][topOrLeft.c + i2] = '.';
        }
    }
    
    if (dir == VERTICAL)
    {
        //loop through rows until ship length is reached and check that ship is able to be unplaced
        for (int k = 0; k < m_game.shipLength(shipId); k++)
        {
            //check that topOrLeft is a free spot at each row
            if (m_board[topOrLeft.r + k][topOrLeft.c] != m_game.shipSymbol(shipId))
            {
                //revert to original board
                return false;
            }
        }
        //unplace ship
        for (int k2 = 0; k2 < m_game.shipLength(shipId); k2++)
        {
            m_board[topOrLeft.r + k2][topOrLeft.c] = '.';
        }
    }
    return true; //if ship has been successfully unplaced
}

void BoardImpl::display(bool shotsOnly) const
{
    for (int r = 0; r < m_game.rows()+1; r++)
    {
        for (int c = 0; c < m_game.cols()+2; c++)
        {
            if (r == 0 && c == 0)
                cout << ' '; //in row 0, col 0, there's a space
            else if (r > 0 && c == 0)
                cout << r-1; //in the first column, the row # is displayed
            else if (c == 1)
                cout << ' '; //in column 1, only spaces
            else if (r == 0 && c > 1)
                cout << c - 2; //in first row past first 2 spaces, display col #
    
            //once outside part of array is established, display actual grid
            if (r > 0 && c > 1)
            {
                if (!shotsOnly)
                {
                    cout << m_board[r-1][c-2];
                }
                if (shotsOnly)
                {
                    if (m_board[r-1][c-2] != '.' && m_board[r-1][c-2] != 'X' && m_board[r-1][c-2] != 'o' && m_board[r-1][c-2] != '#')
                        cout << '.';
                    else
                        cout << m_board[r-1][c-2];
                }
            }
            
        }
        cout << endl;
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    shotHit = false;
    shipDestroyed = false;
    
    //CHECKS FOR IF ATTACK IS VALID OR NOT
    
    //if point is invalid, return false
    if (!m_game.isValid(p))
        return false;
    
    //if attack has been made here, return false
    if (m_board[p.r][p.c] == 'o' || m_board[p.r][p.c] == 'X')
        return false;
    
    //ATTACK IS VALID IF CODE UNDER THIS RUNS
    
    //if attack is on a period, shot has missed
    if (m_board[p.r][p.c] == '.')
    {
        shotHit = false;
        m_board[p.r][p.c] = 'o';
    }
    else //point p is an undamaged segment of a ship, so set to "hit!"
    //if it's not a period, it has to be a shipSymbol
    {
        char tempSymbol = m_board[p.r][p.c]; //copies shipSymbol into temp variable
        m_board[p.r][p.c] = 'X'; //now considered a damaged segment
        shotHit = true;
        
        shipDestroyed = true; //initialize shipDestroyed to true unless we find same shipSymbol
        for (int r = 0; r < m_game.rows(); r++)
        {
            for (int c = 0; c < m_game.cols(); c++)
            {
                //if ship Symbol still exists on the board, ship is not destroyed yet
                if (m_board[r][c] == tempSymbol)
                {
                    shipDestroyed = false;
                }
            }
        }
        if (shipDestroyed == true)
            //loop through shipSymbols and if it equals tempSymbol, we have found the correct ship type and assign index to shipID
            for(int i = 0; i < m_game.nShips(); i++)
            {
                if(m_game.shipSymbol(i) == tempSymbol)
                    shipId = i;
            }
    }
    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    //loops through rows and columns, if there is any character on the board besides the "hit", "missed", and unattacked spots, there are still ships on the board so return false
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (m_board[r][c] != '.' && m_board[r][c] != 'X' && m_board[r][c] != 'o')
            {
                return false;
            }
        }
    }
    return true;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
