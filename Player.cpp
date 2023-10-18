#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual ~AwfulPlayer();
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

AwfulPlayer::~AwfulPlayer()
{
    
}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}





//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class HumanPlayer : public Player
{
public:
    HumanPlayer(string nm, const Game& g);
    virtual ~HumanPlayer();
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    const Game& m_game;
};

HumanPlayer::HumanPlayer(string nm, const Game& g)
:Player(nm, g), m_game(g)
{
}

HumanPlayer::~HumanPlayer()
{}

bool HumanPlayer::isHuman() const
{
    return true;
}

bool HumanPlayer::placeShips(Board &b)
{
    Direction dir;
    cout << name() << " must place " << m_game.nShips() << " ships." << endl;
    b.display(false); //display screen with everything, not just shotsOnly
    
    //check that game board is big enough for ships to be placed
    int bigShip = 0;
    for (int i = 1; i < m_game.nShips(); i++)
    {
        if (m_game.shipLength(i) > m_game.shipLength(i-1))
            int bigShip = m_game.shipLength(i); //length of longest ship
    }
    
    //if board configs cannot place the largest ship, return false
    if (m_game.rows() < bigShip || m_game.cols() < bigShip)
        return false;
        
    
    //loop through user input of each ship that needs to be added
    for (int i = 0; i < m_game.nShips(); i++)
    {
        //first have user input direction
        char dirInput = '?'; //initialize to some random char not 'h' or 'v'
        
        while (dirInput != 'h' && dirInput !='v')
        {
            cout << "Enter h or v for direction of " << m_game.shipName(i) << " (length " << m_game.shipLength(i) << "): " << endl;
            
            cin >> dirInput; //user cannot input a data type direction, so input character (dirInput) and then assign to dir
            cin.ignore(10000, '\n');
            if (dirInput == 'h')
            {
                dir = HORIZONTAL;
            }
            else if (dirInput == 'v')
            {
                dir = VERTICAL;
            }
            else
                cout << "Direction must be h or v." << endl;
        }
        
        //now, have user input the top/left point of ship being placed
        int r = -1;
        int c = -1;
        
        while (true)
        {
            cout << "Enter row and column of leftmost cell (e.g., 3 5): " << endl;
            getLineWithTwoIntegers(r, c);
            
            //if input is not valid, loop again
            if (b.placeShip(Point(r, c), i, dir) == false)
                cout << "The ship can not be placed there." << endl;
            else
                break;
        }
        
        //at this point, ship is able to be placed
        b.display(false); //want to see undamaged ship that has just been placed
    }

    return true;
}

Point HumanPlayer::recommendAttack()
{
    int r = -1;
    int c = -1;
    cout << "Enter the row and column to attack (e.g., 3 5): " << endl;
    
    //if entries are invalid, keep looping until user inputs valid entries
    while (getLineWithTwoIntegers(r, c) == false)
    {
//        if (getLineWithTwoIntegers(r, c) == false || isdigit(r) == false || isdigit(c) == false)
            cout << "You must enter 2 integers." << endl;
    }
    
    return Point(r, c);
}


void HumanPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // HumanPlayer completely ignores the result of any attack
}

void HumanPlayer::recordAttackByOpponent(Point /* p */)
{
      // HumanPlayer completely ignores what the opponent does
}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class MediocrePlayer : public Player
{
public:
    MediocrePlayer(string nm, const Game& g);
    virtual ~MediocrePlayer();
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    bool placeHelper(int shipId, Board &b);
    virtual Point recommendAttack();
    bool validAttack(Point p);
    void potentialPointsFinder(Point p);
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    
private:
    Point m_lastPointAttacked;
    const Game& m_game;
    int state;
    vector<Point> pointsAttacked; //keeps track of points that have been attacked
    vector<Point> potentialPoints; //keeps track of potential points to attack
};
// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

MediocrePlayer::MediocrePlayer(string nm, const Game& g)
: Player(nm, g), m_game(g), m_lastPointAttacked(0, 0), state(1)
{
    
}

MediocrePlayer::~MediocrePlayer()
{
    
}
bool MediocrePlayer::isHuman() const
{
    return false;
}

bool MediocrePlayer::placeShips(Board &b)
{
    
    
    for (int tries = 0; tries < 50; tries++)
    {
        b.block(); //for mediocore, we need to block before placing
        if(placeHelper(0, b))
        {
            b.unblock();
            return true;
        }
        b.unblock();
    }
    return false;
}

//RECURSION FUNCTION (takes a board and shipID and sees if it can be placed) * starts with shipID 0 *
bool MediocrePlayer::placeHelper(int shipTrack, Board &b)
{
    //Base case:
        //if last ship can be placed, place the ship and return true

    //if not last ship...
    //b.display(0);
    if(shipTrack == game().nShips())
    {
        return true;
    }
    
    //loop through whole board checking if ship can be placed
    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
        {
            if(b.placeShip(Point(r,c), shipTrack, HORIZONTAL)) //if it can be placed
            {
                if (placeHelper(shipTrack+1, b)) //if next ship can be placed, place the ship and return the function with next ship as parameter
                {
                    return true; //loop through board again and check if next ship can be placed)
                }
                else //if not, unplace the ship
                {
                    b.unplaceShip(Point(r,c), shipTrack, HORIZONTAL);
                }
            }
            
            if(b.placeShip(Point(r,c), shipTrack, VERTICAL))
            {
                if (placeHelper(shipTrack+1, b)) //if next ship can be placed, place the ship and return the function with next ship as parameter
                {
                    return true;
                }
                else //if not, unplace the CURRENT ship
                {
                    b.unplaceShip(Point(r,c), shipTrack, VERTICAL);
                }
            }
        }
    }
    return false; //if it can't be placed ANYWHERE, return false
}

void MediocrePlayer::recordAttackByOpponent(Point /* p */)
{
      // MediocrePlayer completely ignores what the opponent does
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    
    //STATE 1:
    if (state == 1)
    {
        if (shotHit)
        {
            if(!shipDestroyed)
            {
                state = 2; //The attack hits a ship, but does not destroy it. In this case, the player switches to state 2
                if (!potentialPoints.empty()) //if the potential points has anything, empty it
                {
                    potentialPoints.clear();
                }
                potentialPointsFinder(p); //fill up vector with all potential points around initial "hit"
            }
        }
    }
    //STATE 2:
    if (state == 2)
    {
        if (p.r == 999 && p.c == 999) //whole cross searched and ship not destroyed, returns (999,999)
            state = 1;
        
        if(shotHit)
        {
            if( shipDestroyed == true)
            {
                state = 1; //ship is destroyed, or whole grid was searched, so switch back so state 1
            }
        }
    }
    
}

Point MediocrePlayer::recommendAttack()
{
    //State 1: return random Point on board
    if (state == 1)
    {
        int r = -1;
        int c = -1;
        //generate random point, if it's in vector, go back and loop through until it's not
        do
        {
            r = randInt(game().rows());
            c = randInt(game().cols());
        }while(!validAttack(Point(r,c)));
        m_lastPointAttacked = Point(r,c);
        pointsAttacked.push_back(m_lastPointAttacked); //track points that have been attacked with vector
        return Point(r,c);
    }

    //State 2:
    else
    {
        //picks random point on cross
        int pos = 0;
        while (pos == 0) {
            pos = randInt(potentialPoints.size());
        }
        Point p = potentialPoints.at(pos);
        potentialPoints.erase(potentialPoints.begin() + pos); //erase point from potential list
        pointsAttacked.push_back(p);
        return p;
    }
    
    return Point(999, 999);
}

void MediocrePlayer::potentialPointsFinder(Point p)
{
    int r = p.r;
    int c = p.c;
    
    potentialPoints.push_back(p);
    
    for(int up = 1; up < 5; up++)
    {
        if (validAttack(Point(r - up, c)))
            potentialPoints.push_back(Point(r - up, c));
    }
    for(int down = 1; down < 5; down++)
    {
        if (validAttack(Point(r + down, c)))
            potentialPoints.push_back(Point(r + down, c));
    }
    for(int right = 1; right < 5; right++)
    {
        if (validAttack(Point(r, c + right)))
            potentialPoints.push_back(Point(r, c + right));
    }
    for (int left = 1; left < 5; left++)
    {
        if (validAttack(Point(r, c - left)))
            potentialPoints.push_back(Point(r, c - left));
    }
}

bool MediocrePlayer::validAttack(Point p) //helper function to check if potential attack point is valid and hasn't been attacked already
{
    for (int i = 0; i < pointsAttacked.size(); i++)
    {
        if (p.r == pointsAttacked[i].r && p.c == pointsAttacked[i].c)
            return false; //point has been attacked already
    }
    
    if (!game().isValid(p))
        return false;
    
    return true;
}

//*********************************************************************
//  GoodPlayer
//*********************************************************************


class GoodPlayer : public Player
{
public:
    GoodPlayer(string nm, const Game& g);
    virtual ~GoodPlayer();
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    bool placeHelper(int shipId, Board &b);
    virtual Point recommendAttack();
    bool validAttack(Point p);
    void potentialPointsFinder(Point p);
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);

private:
    Point m_lastPointAttacked;
    const Game& m_game;
    int state;
    stack<Point> pointsToAttack; //keeps track of points around a "hit"
    vector<Point> attackHistory; //keeps track of every point attacked
};

GoodPlayer::GoodPlayer(string nm, const Game& g)
: Player(nm, g), m_game(g), m_lastPointAttacked(0, 0), state(1)
{

}

GoodPlayer::~GoodPlayer() {}

bool GoodPlayer::isHuman() const
{
    return false;
}

void GoodPlayer::recordAttackByOpponent(Point /* p */)
{
}

bool GoodPlayer::placeShips(Board &b)
{
    
    for (int tries = 0; tries < 50; tries++)
    {
        b.block(); //for mediocore, we need to block before placing
        if(placeHelper(0, b))
        {
            b.unblock();
            return true;
        }
        b.unblock();
    }
    return false;
}

//RECURSION FUNCTION (takes a board and shipID and sees if it can be placed) * starts with shipID 0 *
bool GoodPlayer::placeHelper(int shipTrack, Board &b)
{
    //Base case:
        //if last ship can be placed, place the ship and return true

    //if not last ship...
    //b.display(0);
    if(shipTrack == game().nShips())
    {
        return true;
    }
    
    //loop through whole board checking if ship can be placed
    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
        {
            if(b.placeShip(Point(r,c), shipTrack, HORIZONTAL)) //if it can be placed
            {
                if (placeHelper(shipTrack+1, b)) //if next ship can be placed, place the ship and return the function with next ship as parameter
                {
                    return true; //loop through board again and check if next ship can be placed)
                }
                else //if not, unplace the ship
                {
                    b.unplaceShip(Point(r,c), shipTrack, HORIZONTAL);
                }
            }
            
            if(b.placeShip(Point(r,c), shipTrack, VERTICAL))
            {
                if (placeHelper(shipTrack+1, b)) //if next ship can be placed, place the ship and return the function with next ship as parameter
                {
                    return true;
                }
                else //if not, unplace the CURRENT ship
                {
                    b.unplaceShip(Point(r,c), shipTrack, VERTICAL);
                }
            }
        }
    }
    return false; //if it can't be placed ANYWHERE, return false
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    //STATE 1:
    if (state == 1)
    {
        if (shotHit)
        {
            if(!shipDestroyed)
            {
                state = 2; //The attack hits a ship, but does not destroy it. In this case, the player switches to state 2
                potentialPointsFinder(p); //fill up vector with all potential points around initial "hit"
            }
        }
    }
    //STATE 2:
    if (state == 2)
    {
        if (p.r == 999 && p.c == 999) //whole cross searched and ship not destroyed, returns (999,999)
            state = 1;
        
        if(shotHit)
        {
            if( shipDestroyed == true)
            {
                state = 1; //ship is destroyed, or whole grid was searched, so switch back so state 1
            }
        }
    }
}

Point GoodPlayer::recommendAttack()
{
    //State 1: return random Point on board
    if (state == 1)
    {
        int r = -1;
        int c = -1;
        //generate random point, if it's in vector, go back and loop through until it's not
        do
        {
            r = randInt(game().rows());
            c = randInt(game().cols());
        }while(!validAttack(Point(r,c)));
        m_lastPointAttacked = Point(r,c);
        attackHistory.push_back(m_lastPointAttacked); //
        return Point(r,c);
    }

    //State 2:
    else
    {
        if (!pointsToAttack.empty())
        {
            Point top = pointsToAttack.top();
            pointsToAttack.pop(); //pops top value
            return top;
        }
    }
    
    return Point(999, 999);
}

void GoodPlayer::potentialPointsFinder(Point p)
{
    int r = p.r;
    int c = p.c;
    
    // If cell above p is valid add it to the stack
    if (validAttack(Point(p.r-1, p.c)))
    {
       pointsToAttack.push(Point(p.r-1, p.c));
    }
    // If cell below p is valid add it to the stack
    if (validAttack(Point(p.r+1, p.c)))
    {
       pointsToAttack.push(Point(p.r+1, p.c));
    }
    // If cell to the left of p is valid add it to the stack
    if (validAttack(Point(p.r, p.c-1)))
    {
        pointsToAttack.push(Point(p.r, p.c-1));
    }
    // If cell to the right of p is valid add it to the stack
    if (validAttack(Point(p.r, p.c+1)))
    {
        pointsToAttack.push(Point(p.r, p.c+1));
    }
}

bool GoodPlayer::validAttack(Point p) //helper function to check if potential attack point is valid and hasn't been attacked already
{
    for (int i = 0; i < attackHistory.size(); i++)
    {
        if (p.r == attackHistory[i].r && p.c == attackHistory[i].c)
            return false; //point has been attacked already
    }
    
    if (!game().isValid(p))
        return false;
    
    return true;
}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };

    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  && type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
