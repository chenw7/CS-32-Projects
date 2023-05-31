#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, double startX, double startY, int startDirection, int depth, int sprite, bool impactable = false)
    :GraphObject(imageID, startX, startY, sprite, depth), m_world(world), m_state(true), m_walking(true), m_impactable(impactable)
    {};
    
    virtual void doSomething() = 0;
    
    //VIRTUAL FUNCTIONS
    virtual bool isWalkingActor(){return m_walking;}
    virtual bool isVortex() {return false;}
    virtual void gotHit() {}
    virtual bool isPaused() {return false;}
    virtual void changePaused() {};
    virtual ~Actor() {};
    
    //NON-VIRTUAL FUNCTIONS
    StudentWorld* getWorld() {return m_world;}
    void changeState(bool state){m_state = state;}
    bool getState() const {return m_state;}
    bool impactable() const {return m_impactable;}
    void makeImpactable() {m_impactable = true;}
    
private:
    bool m_walking;
    bool m_state;
    //FOR AVATAR,       WAITING(T), WALKING(F)
    //FOR COIN SQUARES, ACTIVE(T), INACTIVE(F)
    //FOR BADDIES,      PAUSED(T), NOT PAUSED(F)
    //FOR VORTEX,       ACTIVE(T), INACTIVE(F)
    StudentWorld* m_world;
    bool m_impactable;
};

class MovingAvatar: public Actor
{
public:
    MovingAvatar(StudentWorld* world, int imageID, double startX, double startY)
    :Actor(world, imageID, startX, startY, 0, 0, 0), movingDirection(0), ticks_to_move(0)
    {};
    
    virtual void doSomething() = 0;
    int getMDirection() const {return movingDirection;}
    void setMDirection(int newD) {movingDirection = newD;}
    int getTicks() const {return ticks_to_move;}
    void setTicks(int ticks) {ticks_to_move = ticks;}
    bool canMove(int direction, int distance); //IMPLEMENT
    void switchDirection(); //CHANGES DIRECTION IF NECESSARY
    void getRandomValidDirection(MovingAvatar* a, double x, double y);
    int getNumValidDirections(double x, double y);

private:
    int movingDirection;
    int ticks_to_move;
};

class Avatar: public MovingAvatar
{
public:
    Avatar(StudentWorld* world, int imageID, double startX, double startY, int pNum)
    :MovingAvatar(world, imageID, startX, startY), playerNum(pNum), coins(0), stars(0), m_hasVortex(false), m_forcedDirection(false), walkingAtFork(false)
    {};
    virtual void doSomething();
    int getCoins() const {return coins;}
    void addCoins(int amount);
    int getStars() const {return stars;}
    void addStars(int amount) {stars += amount;}
    bool hasVortex() const {return m_hasVortex;}
    void giveVortex() {m_hasVortex = true;}
    void removeVortex() {m_hasVortex = false;}
    void changeForced(bool status) {m_forcedDirection = status;}
    bool isChoosingDirection() const {return walkingAtFork;}
    void changeChoosingStatus(bool state) {walkingAtFork = state;}
    
private:
    int playerNum;
    int coins;
    int stars;
    bool m_hasVortex;
    bool m_forcedDirection;
    bool walkingAtFork;
};

class Baddies: public MovingAvatar
{
public:
    Baddies(StudentWorld* world, int imageID, double startX, double startY)
    :MovingAvatar(world, imageID, startX, startY), pauseCounter(180), m_paused(true)
    {
        makeImpactable();
        activated[0] = false;
        activated[1] = false;
    };
    
    virtual void doSomething() {} //NEED IMPLEMENTATION
    virtual void gotHit();
    int getCount() const {return pauseCounter;}
    void decCount() {pauseCounter--;}
    void resetCount() {pauseCounter = 180;}
    bool isPaused() {return m_paused;}
    void changePaused(bool state) {m_paused = state;}
    bool wasActivated(int playerNum) const {return activated[playerNum-1];}
    void setStatus(bool state, int playerNum) {activated[playerNum-1] = state;}
    
private:
    bool m_paused;
    int pauseCounter;
    bool activated[2];
};

class Bowser: public Baddies
{
public:
    Bowser(StudentWorld* world, int imageID, double startX, double startY)
    :Baddies(world, imageID, startX, startY)
    {}
    
    virtual void doSomething();
};

class Boo: public Baddies
{
public:
    Boo(StudentWorld* world, int imageID, double startX, double startY)
    :Baddies(world, imageID, startX, startY)
    {}
    
    virtual void doSomething();
};

class Vortex: public MovingAvatar
{
public:
    Vortex(StudentWorld* world, int imageID, double startX, double startY, int direction)
    :MovingAvatar(world, imageID, startX, startY), m_direction(direction)
    {};
    
    virtual void doSomething();
    virtual bool isVortex() {return true;}
    
private:
    int m_direction;
};

class Square: public Actor
{
public:
    Square(StudentWorld* world, int imageID, double startX, double startY, int sprite)
    :Actor(world, imageID, startX, startY, 0, 1, sprite)
    {
        activated[0] = false;
        activated[1] = false;
    };
    
    virtual void doSomething() = 0;
    virtual bool isWalkingActor() {return false;}
    bool wasActivated(int playerNum) const {return activated[playerNum-1];}
    void setStatus(bool state, int playerNum) {activated[playerNum-1] = state;}

private:
    bool activated[2];
};

class CoinSquare: public Square
{
public:
    CoinSquare(StudentWorld* world, int imageID, double startX, double startY, bool gives)
    :Square(world, imageID, startX, startY, 0), givesMoney(gives)
    {};
    
    virtual void doSomething();
    
private:
    bool givesMoney;
};

class StarSquare: public Square
{
public:
    StarSquare(StudentWorld* world, int imageID, double startX, double startY)
    :Square(world, imageID, startX, startY, 0)
    {};
    
    virtual void doSomething();
};

class DirectionSquare: public Square
{
public:
    DirectionSquare(StudentWorld* world, int imageID, double startX, double startY, int SD)
    :Square(world, imageID, startX, startY, SD), m_direction(SD)
    {};
    
    virtual void doSomething();
private:
    int m_direction;
};

class BankSquare: public Square
{
public:
    BankSquare(StudentWorld* world, int imageID, double startX, double startY)
    :Square(world, imageID, startX, startY, 0)
    {};
    
    virtual void doSomething();

};

class EventSquare: public Square
{
public:
    EventSquare(StudentWorld* world, int imageID, double startX, double startY)
    :Square(world, imageID, startX, startY, 0)
    {};
    
    virtual void doSomething();
};

class DroppingSquare: public Square
{
public:
    DroppingSquare(StudentWorld* world, int imageID, double startX, double startY)
    :Square(world, imageID, startX, startY, 0)
    {};
    
    virtual void doSomething();
};

#endif // ACTOR_H_
