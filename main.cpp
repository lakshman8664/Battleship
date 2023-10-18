#include "Game.h"
#include "Player.h"
#include "Board.h"
#include <iostream>
#include <string>

using namespace std;

bool addStandardShips(Game& g)
{
    return g.addShip(5, 'A', "aircraft carrier")  &&
           g.addShip(4, 'B', "battleship")  &&
           g.addShip(3, 'D', "destroyer")  &&
           g.addShip(3, 'S', "submarine")  &&
           g.addShip(2, 'P', "patrol boat");
}

//int main()
//{
//    Point p(9, 1);
//    Point q(3,3);
//
//
    Game g(10, 10);
////    cout << "Rows:" << g.rows() << endl;
////    cout << "Cols:" << g.cols() << endl;
////    cout << "IsValid:" << g.isValid(p) << endl;
////    cout << "Random Point:" << g.randomPoint().r << g.randomPoint().c << endl;
//    addStandardShips(g);
//    cout << "# of ships: " << g.nShips() << endl;
//
//    cout << "length of ship 2: " << g.shipLength(2) << endl;
//
//    cout << "symbol of ship 2: " << g.shipSymbol(2) << endl;
//
//    cout << "name of ship 2: " << g.shipName(2) << endl;
//
//    Board newBoard(g);
//
//    newBoard.display(false);
//
//    newBoard.block();
//
//    newBoard.display(false);
//
//    newBoard.unblock();
//
//    newBoard.placeShip(p, 2, HORIZONTAL);
//    newBoard.placeShip(q, 0, VERTICAL);
//    newBoard.unplaceShip(destroyer, 2, HORIZONTAL);
//    newBoard.unplaceShip(aircraftCarrier, 0, VERTICAL);
//    bool shotHit = false;
//    bool shipDestroyed = false;
//    int shipId = 0;
//    newBoard.attack(q, shotHit, shipDestroyed, shipId);
//    newBoard.display(false);
//    Player* p1 = createPlayer("mediocre", "Lakshman", g);
//    Player* p2 = createPlayer("awful", "AwfulPlayer", g);
//    g.play(p1, p2, true);
//    delete p1;
//    delete p2;
//}

int main()
{
    const int NTRIALS = 10;

    cout << "Select one of these choices for an example of the game:" << endl;
    cout << "  1.  A mini-game between two mediocre players" << endl;
    cout << "  2.  A mediocre player against a human player" << endl;
    cout << "  3.  A " << NTRIALS
         << "-game match between a mediocre and an awful player, with no pauses"
         << endl;
    cout << "Enter your choice: ";
    string line;
    getline(cin,line);
    if (line.empty())
    {
        cout << "You did not enter a choice" << endl;
    }
    else if (line[0] == '1')
    {
        Game g(2, 3);
        g.addShip(2, 'R', "rowboat");
        Player* p1 = createPlayer("mediocre", "Player2", g);
        Player* p2 = createPlayer("mediocre", "Player1", g);
        cout << "This mini-game has one ship, a 2-segment rowboat." << endl;
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '2')
    {
        Game g(10, 10);
        addStandardShips(g);
        Player* p2 = createPlayer("mediocre", "Player2", g);
        Player* p1 = createPlayer("human", "Player1", g);
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '3')
    {
        int nMediocreWins = 0;

        for (int k = 1; k <= NTRIALS; k++)
        {
            cout << "============================= Game " << k
                 << " =============================" << endl;
            Game g(10, 10);
            
            addStandardShips(g);
            Player* p1 = createPlayer("good", "Goood George", g);
            Player* p2 = createPlayer("mediocre", "Mediocre Mimi", g);
            Player* winner = (k % 2 == 1 ?
                                g.play(p1, p2, false) : g.play(p2, p1, false));
            if (winner == p2)
                nMediocreWins++;
            delete p1;
            delete p2;
        }
        cout << "The mediocre player won " << nMediocreWins << " out of "
             << NTRIALS << " games." << endl;
          // We'd expect a mediocre player to win most of the games against
          // an awful player.  Similarly, a good player should outperform
          // a mediocre player.
    }
    else
    {
       cout << "That's not one of the choices." << endl;
    }
}
