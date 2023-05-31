#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld() {};
    
    //Helper Functions
    bool isValidPos(double x, double y);
    bool canTeleport(double x, double y);
    void setDirection(int playerNum, int direction);
    void setSDirection(int playerNum, int sdirection);
    void addCoin(int playerNum, int amount);
    int getCoin(int playerNum);
    int getStar(int playerNum);
    void addStar(int playerNum, int amount);
    bool landed(Actor& actor, int playerNum);
    bool passed(Actor& actor, int playerNum);
    void addBank(int amount) {m_bank += amount;}
    int bankAmount() const {return m_bank;}
    void teleportToRandom(int playerNum);
    void giveVortex(int playerNum);
    void swapEverything(Square* s, int playerNum);
    void swapCoins();
    void swapStars();
    void newVortex(double x, double y, int direction);
    bool collided(Actor& a);
    void newDropping(double x, double y);
    void deactivateSquare(double x, double y);
    void changeForceStatus(int playerNum, bool state);
    bool isChoosingAtFork(int playerNum);
    
private:
    std::vector<Actor*> actors;
    Avatar* peach;
    Avatar* yoshi;
    int m_bank; //IMPLEMENT LATER
};

#endif // STUDENTWORLD_H_
