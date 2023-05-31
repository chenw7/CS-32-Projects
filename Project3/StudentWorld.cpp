#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_bank = 0;
}

int StudentWorld::init()
{
    Board bd;
    string board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = bd.loadBoard(board_file);
    
    if (result == Board::load_fail_file_not_found){
        cerr << "Could not find board01.txt data file\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_fail_bad_format){
        cerr << "Your board was improperly formatted\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_success){
        cerr << "Successfully loaded board\n";
        for(int x = 0; x < 16; x++){
            for(int y = 0; y < 16; y++){
                Board::GridEntry ge = bd.getContentsOf(x, y);
                switch (ge) {
                    case Board::empty:
                        cout << "Location " << x << " " << y << " is empty\n";
                        break;
                    case Board::boo:
                        cout << "Location " << x << " " << y << " has a Boo and a blue coin square\n";
                        actors.push_back(new Boo(this, IID_BOO, SPRITE_WIDTH*x, SPRITE_WIDTH*y));
                        actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH*x, SPRITE_HEIGHT*y, true));
                        break;
                    case Board::bowser:
                        cout << "Location " << x << " " << y << " has a Bowser and a blue coin square\n";
                        actors.push_back(new Bowser(this, IID_BOWSER, SPRITE_WIDTH*x, SPRITE_HEIGHT*y));
                        actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH*x, SPRITE_HEIGHT*y, true));
                        break;
                    case Board::player:
                        cout << "Location " << x << " " << y << " has Peach & Yoshi and a blue coin square\n";
                        peach = new Avatar(this, IID_PEACH, SPRITE_WIDTH*x, SPRITE_HEIGHT*y, 1);
                        yoshi = new Avatar(this, IID_YOSHI, SPRITE_WIDTH*x, SPRITE_HEIGHT*y, 2);
                        actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH*x, SPRITE_HEIGHT*y, true));
                        break;
                    case Board::red_coin_square:
                        cout << "Location " << x << " " << y << " has a red coin square\n";
                        actors.push_back(new CoinSquare(this, IID_RED_COIN_SQUARE, SPRITE_WIDTH*x, SPRITE_HEIGHT*y, false));
                        break;
                    case Board::blue_coin_square:
                        cout << "Location " << x << " " << y << " has a blue coin square\n";
                        actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH*x, SPRITE_HEIGHT*y, true));
                        break;
                    case Board::up_dir_square:
                        cout << "Location " << x << " " << y << " has a up direction square\n";
                        actors.push_back(new DirectionSquare(this, IID_DIR_SQUARE, SPRITE_WIDTH*x, SPRITE_HEIGHT*y, 90));
                        break;
                    case Board::down_dir_square:
                        cout << "Location " << x << " " << y << " has a down direction square\n";
                        actors.push_back(new DirectionSquare(this, IID_DIR_SQUARE, SPRITE_WIDTH*x, SPRITE_HEIGHT*y, 270));
                        break;
                    case Board::left_dir_square:
                        cout << "Location " << x << " " << y << " has a left direction square\n";
                        actors.push_back(new DirectionSquare(this, IID_DIR_SQUARE, SPRITE_WIDTH*x, SPRITE_HEIGHT*y, 180));
                        break;
                    case Board::right_dir_square:
                        cout << "Location " << x << " " << y << " has a right direction square\n";
                        actors.push_back(new DirectionSquare(this, IID_DIR_SQUARE, SPRITE_WIDTH*x, SPRITE_HEIGHT*y, 0));
                        break;
                    case Board::event_square:
                        cout << "Location " << x << " " << y << " has an event square\n";
                        actors.push_back(new EventSquare(this, IID_EVENT_SQUARE, SPRITE_WIDTH*x, SPRITE_HEIGHT*y));
                        break;
                    case Board::bank_square:
                        cout << "Location " << x << " " << y << " has a bank square\n";
                        actors.push_back(new BankSquare(this, IID_BANK_SQUARE, SPRITE_WIDTH*x, SPRITE_HEIGHT*y));
                        break;
                    case Board::star_square:
                        cout << "Location " << x << " " << y << " has a star square\n";
                        actors.push_back(new StarSquare(this, IID_STAR_SQUARE, SPRITE_WIDTH*x, SPRITE_HEIGHT*y));
                        break;
                }
            }
        }
    }
	startCountdownTimer(100);  // this placeholder causes timeout after 5 seconds
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    peach->doSomething();
    yoshi->doSomething();
    
    vector<Actor*>::iterator p = actors.begin();
    while(p != actors.end()){
        if((*p)->getState()){
            (*p)->doSomething();
            p++;
        }
        else if((*p)->isVortex() && !(*p)->getState()){
            delete *p;
            p = actors.erase(p);
        }
        else if(!(*p)->isWalkingActor() && !(*p)->getState()){
            delete *p;
            p = actors.erase(p);
        }
        else{
            p++;
        }
    }
    
    if (timeRemaining() <= 0){
        playSound(SOUND_GAME_FINISHED);
        int winner = randInt(1, 2);
        if(peach->getStars() != yoshi->getStars()){
            if(peach->getStars() > yoshi->getStars())
                winner = 1;
            else
                winner = 2;
        }
        
        else if(peach->getCoins() != yoshi->getCoins()){
            if(peach->getCoins() > yoshi->getCoins())
                winner = 1;
            else
                winner = 2;
        }
        
        if(winner == 1){
            setFinalScore(peach->getStars(), peach->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        setFinalScore(yoshi->getStars(), yoshi->getCoins());
        return GWSTATUS_YOSHI_WON;
    }
    
    std::ostringstream oss;
    oss.fill('0');
    oss << "P1 Roll: " << (peach->getTicks()+7)/8 << " ";
    oss << "Stars: " << peach->getStars() << " ";
    oss << "$$: " << peach->getCoins() << " ";
    if(peach->hasVortex())
        oss << "VOR ";
    oss << "| Time: " << timeRemaining() << " ";
    oss << "| Bank: " << bankAmount() << " ";
    oss << "| P2 Roll: " << (yoshi->getTicks()+7)/8 << " ";
    oss << "Stars: " << yoshi->getStars() << " ";
    oss << "$$: " << yoshi->getCoins() << " ";
    if(yoshi->hasVortex())
        oss << "VOR";
    
    setGameStatText(oss.str());
    
	return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::isValidPos(double x, double y)
{
    for(auto p = actors.begin(); p != actors.end(); p++){
        if(!(*p)->isWalkingActor() && (*p)->getX() <= x && (*p)->getX() + SPRITE_WIDTH > x &&
           (*p)->getY() <= y && (*p)->getY() + SPRITE_HEIGHT > y)
            return true;
    }
    return false;
}

bool StudentWorld::canTeleport(double x, double y)
{
    for(auto p = actors.begin(); p != actors.end(); p++){
        if(!(*p)->isWalkingActor() && (*p)->getX() == x && (*p)->getY() == y)
            return true;
    }
    return false;
}

void StudentWorld::setDirection(int playerNum, int direction)
{
    if(playerNum == 1){
        peach->setMDirection(direction);
    }
    if(playerNum == 2){
        yoshi->setMDirection(direction);
    }
}

void StudentWorld::setSDirection(int playerNum, int sdirection){
    if(playerNum == 1){
        peach->setDirection(sdirection);
    }
    if(playerNum == 2){
        yoshi->setDirection(sdirection);
    }
}

void StudentWorld::addCoin(int playerNum, int amount){
    if(playerNum == 1){
        peach->addCoins(amount);
    }
    if(playerNum == 2){
        yoshi->addCoins(amount);
    }
}

int StudentWorld::getCoin(int playerNum){
    if(playerNum == 1)
        return peach->getCoins();
    return yoshi->getCoins();
}

int StudentWorld::getStar(int playerNum){
    if(playerNum == 1)
        return peach->getStars();
    return yoshi->getStars();
}

void StudentWorld::addStar(int playerNum, int amount){
    if(playerNum == 1)
        peach->addStars(amount);
    else
        yoshi->addStars(amount);
}

bool StudentWorld::landed(Actor& actor, int playerNum)
{
    if(playerNum == 1){
        if(actor.getX() == peach->getX() && actor.getY() == peach->getY() && peach->getState()){
            return true;
        }
    }
    if(playerNum == 2){
        if(actor.getX() == yoshi->getX() && actor.getY() == yoshi->getY() && yoshi->getState()){
            return true;
        }
    }
    return false;
}

bool StudentWorld::passed(Actor& actor, int playerNum)
{
    if(playerNum == 1){
        if(actor.getX() == peach->getX() && actor.getY() == peach->getY()){
            return true;
        }
    }
    if(playerNum == 2){
        if(actor.getX() == yoshi->getX() && actor.getY() == yoshi->getY()){
            return true;
        }
    }
    return false;
}

void StudentWorld::teleportToRandom(int playerNum) //NEW VALID RANDOM DIRECTION AFTER LANDING
{
    Avatar* player;
    if(playerNum == 1)
        player = peach;
    else
        player = yoshi;
    
    int x = randInt(0, 256);
    int y = randInt(0, 256);
    while(!canTeleport(x, y)){
        x = randInt(0, 256);
        y = randInt(0, 256);
    }
    
    player->moveTo(x, y);
    
    player->getRandomValidDirection(player, player->getX(), player->getY());
}

void StudentWorld::giveVortex(int playerNum)
{
    if(playerNum == 1){
        peach->giveVortex();
    }
    if(playerNum == 2){
        yoshi->giveVortex();
    }
    playSound(SOUND_GIVE_VORTEX);
}

void StudentWorld::swapEverything(Square* s, int playerNum)
{
    if(playerNum == 1){  //Setting both to true so it doesn't activate again
        s->setStatus(true, 2);
    }
    if(playerNum == 2){
        s->setStatus(true, 1);
    }
    bool state = peach->getState();
    int ticks = peach->getTicks();
    int x = peach->getX();
    int y = peach->getY();
    int mDirection = peach->getMDirection();
    int sDirection = peach->getDirection();
    
    peach->moveTo(yoshi->getX(), yoshi->getY());
    yoshi->moveTo(x, y);
    peach->setTicks(yoshi->getTicks());
    yoshi->setTicks(ticks);
    peach->setMDirection(yoshi->getMDirection());
    yoshi->setMDirection(mDirection);
    peach->setDirection(yoshi->getDirection());
    yoshi->setDirection(sDirection);
    peach->changeState(yoshi->getState());
    yoshi->changeState(state);
}

void StudentWorld::swapCoins()
{
    int pCoins = peach->getCoins();
    int yCoins = yoshi->getCoins();
    peach->addCoins(yCoins-pCoins);
    yoshi->addCoins(pCoins-yCoins);
}

void StudentWorld::swapStars()
{
    int pStars = peach->getStars();
    int yStars = yoshi->getStars();
    peach->addStars(yStars-pStars);
    yoshi->addStars(pStars-yStars);
}

void StudentWorld::newVortex(double x, double y, int direction)
{
    actors.push_back(new Vortex(this, IID_VORTEX, x, y, direction));
}

bool StudentWorld::collided(Actor& a)
{
    for(auto b: actors){
        if(b->impactable() && abs(a.getX() - b->getX()) < SPRITE_WIDTH && abs(a.getY() - b->getY()) < SPRITE_HEIGHT){
            b->gotHit();
            playSound(SOUND_HIT_BY_VORTEX);
            return true;
        }
    }
    return false;
}

void StudentWorld::newDropping(double x, double y)
{
    actors.push_back(new DroppingSquare(this, IID_DROPPING_SQUARE, x, y));
}

void StudentWorld::deactivateSquare(double x, double y)
{
    for(auto p = actors.begin(); p != actors.end(); p++){
        if(!(*p)->isWalkingActor() && (*p)->getX() == x && (*p)->getY() == y){
            (*p)->changeState(false);
            break;
        }
    }
}

void StudentWorld::changeForceStatus(int playerNum, bool state)
{
    if(playerNum == 1)
        peach->changeForced(state);
    else
        yoshi->changeForced(state);
}

bool StudentWorld::isChoosingAtFork(int playerNum)
{
    if(playerNum == 1)
        return peach->isChoosingDirection();
    return yoshi->isChoosingDirection();
}

void StudentWorld::cleanUp()
{
    delete peach;
    delete yoshi;
    vector<Actor*>::iterator p = actors.begin();
    while(!actors.empty()){
        delete *p;
        p = actors.erase(p);
    }
}
