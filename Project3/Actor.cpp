#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Avatar::doSomething()
{
    if(getState()){ //PLAYER IS WAITING
        int die_roll = randInt(1, 10);
        int newX, newY;
        getPositionInThisDirection(getMDirection(), SPRITE_WIDTH, newX, newY);
        switch(getWorld()->getAction(playerNum)){
            case ACTION_NONE:
                break;
            case ACTION_ROLL:
                setTicks(die_roll*8);
                changeState(false);
                break;
            case ACTION_FIRE:
                if(hasVortex()){
                    getWorld()->newVortex(newX, newY, getMDirection());
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                }
                removeVortex();
                break;
        }
    }
    
    if(!getState()){ //PLAYER IS WALKING
        
        if(getWorld()->canTeleport(getX(), getY()) && !m_forcedDirection){
            int originalDirection = getMDirection();
            int newDirection = 0;
            if(getNumValidDirections(getX(), getY()) > 2){
                changeChoosingStatus(true);
                switch(getWorld()->getAction(playerNum)){
                    case ACTION_UP:
                        newDirection = 90;
                        break;
                    case ACTION_DOWN:
                        newDirection = 270;
                        break;
                    case ACTION_LEFT:
                        newDirection = 180;
                        break;
                    case ACTION_RIGHT:
                        newDirection = 0;
                        break;
                    default:
                        return;
                }
                
                if(originalDirection != (newDirection+180)%360) { //Validity Check
                    changeChoosingStatus(false);
                    setMDirection(newDirection);
                    if(getMDirection() == 180)
                        setDirection(180);
                    else
                        setDirection(0);
                }
                else
                    return;
            }
        }
        switchDirection();
        m_forcedDirection = false;
        
        if(getTicks() > 0){
            moveAtAngle(getMDirection(), 2);
            setTicks(getTicks()-1);
        }
        
        if(getTicks() == 0)
            changeState(true);
    }
}

void Avatar::addCoins(int amount){
    if(getCoins() + amount >= 0)
        coins += amount;
}

void MovingAvatar::switchDirection()
{
    if(getMDirection() == right){
        if(!getWorld()->isValidPos(getX()+SPRITE_WIDTH+1, getY())){
            if(getWorld()->isValidPos(getX(), getY()+SPRITE_HEIGHT+1)){
                setMDirection(up);
                setDirection(right);
                return;
            }
            setMDirection(down);
            setDirection(right);
        }
    }
    
    else if(getMDirection() == up){
        if(!getWorld()->isValidPos(getX(), getY()+SPRITE_HEIGHT+1)){
            if(getWorld()->isValidPos(getX()-1, getY())){
                setMDirection(left);
                setDirection(left);
                return;
            }
            setMDirection(right);
            setDirection(right);
        }
    }
    
    else if(getMDirection() == left){
        if(!getWorld()->isValidPos(getX()-1, getY())){
            if(getWorld()->isValidPos(getX(), getY()+SPRITE_HEIGHT+1)){
                setMDirection(up);
                setDirection(right);
                return;
            }
            setMDirection(down);
            setDirection(right);
        }
    }
    
    else if(getMDirection() == down){
        if(!getWorld()->isValidPos(getX(), getY()-1)){
            if(getWorld()->isValidPos(getX()-1, getY())){
                setMDirection(left);
                setDirection(left);
                return;
            }
            setMDirection(right);
            setDirection(right);
        }
    }
}

void MovingAvatar::getRandomValidDirection(MovingAvatar* a, double x, double y){
    std::vector<int> validDirections;
    if(getWorld()->isValidPos(x+SPRITE_WIDTH+1, y)){
        validDirections.push_back(0);
    }
    if(getWorld()->isValidPos(x, y+SPRITE_HEIGHT+1)){
        validDirections.push_back(90);
    }
    if(getWorld()->isValidPos(x-1, y)){
        validDirections.push_back(180);
    }
    if(getWorld()->isValidPos(x, y-1)){
        validDirections.push_back(270);
    }
    
    a->setMDirection(validDirections[randInt(0, validDirections.size()-1)]);
    
    if(a->getMDirection() == 180)
        a->setDirection(180);
    else
        a->setDirection(0);
}

int MovingAvatar::getNumValidDirections(double x, double y)
{
    int count = 0;
    
    if(getWorld()->isValidPos(x+SPRITE_WIDTH+1, y)){
        count++;
    }
    if(getWorld()->isValidPos(x, y+SPRITE_HEIGHT+1)){
        count++;
    }
    if(getWorld()->isValidPos(x-1, y)){
        count++;
    }
    if(getWorld()->isValidPos(x, y-1)){
        count++;
    }
    return count;
}

void Vortex::doSomething(){
    if(!getState())
        return;
    moveAtAngle(m_direction, 2);
    if(getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT)
        changeState(false);
    if(getWorld()->collided(*this))
        changeState(false);
}

void Baddies::gotHit()
{
    int x = randInt(0, 256);
    int y = randInt(0, 256);
    while(!getWorld()->canTeleport(x, y)){
        x = randInt(0, 256);
        y = randInt(0, 256);
    }
    moveTo(x, y);
    setMDirection(0);
    setDirection(0);
    resetCount();
}

void Bowser::doSomething()
{
    if(isPaused()){ //If Bowser is in paused state
        for(int i = 1; i < 3; i++){
            if(getWorld()->landed(*this, i) && !wasActivated(i)){ //If Bowser and a Player Avatar are on the same square, and the Avatar is in a waiting to roll state
                int action = randInt(1, 2);
                if(action == 1){
                    //Bowser will cause the Avatar to lose all their coins
                    getWorld()->addCoin(i, getWorld()->getCoin(i)*-1);
                    getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
                }
                setStatus(true, i);
            }
            if(!getWorld()->landed(*this, i) && wasActivated(i))
                setStatus(false, i);
        }
        decCount();
        
        if(getCount() == 0){
            int toMove = randInt(1, 10);
            setTicks(toMove*8);
            getRandomValidDirection(this, getX(), getY());
            changePaused(false);
        }
    }
    
    else if (!isPaused()){
        if(getWorld()->canTeleport(getX(), getY())){
            if(getNumValidDirections(getX(), getY()) > 2)
                getRandomValidDirection(this, getX(), getY());
        }
        //If Bauser is at a turning point, prefer up over down (if both ok)
        //prefer right over left (if both ok)
        //update moving and sprite direction
        
        if(getTicks() > 0){
            switchDirection();
            moveAtAngle(getMDirection(), 2);
            setTicks(getTicks()-1);
        }
        if(getTicks() == 0){
            changePaused(true);
            resetCount();
            int action = randInt(1, 4);
            if(action == 4){
                getWorld()->deactivateSquare(getX(), getY());
                getWorld()->newDropping(getX(), getY());
                getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
            }
        }
    }
}

void Boo::doSomething()
{
    if(isPaused()){
        for(int i = 1; i < 3; i++){
            if(getWorld()->landed(*this, i) && !wasActivated(i)){
                int action = randInt(1, 2);
                if(action == 1){
                    getWorld()->swapStars();
                }
                else{
                    getWorld()->swapCoins();
                }
                getWorld()->playSound(SOUND_BOO_ACTIVATE);
                setStatus(true, i);
            }
            if(!getWorld()->landed(*this, i) && wasActivated(i))
                setStatus(false, i);
        }
        decCount();
        
        if(getCount() == 0){
            int toMove = randInt(1, 3);
            setTicks(toMove*8);
            getRandomValidDirection(this, getX(), getY());
            changePaused(false);
        }
    }
    
    else if(!isPaused()){
        if(getWorld()->canTeleport(getX(), getY())){
            if(getNumValidDirections(getX(), getY()) > 2)
                getRandomValidDirection(this, getX(), getY());
        }
        //If Boo is at turning point, prefer up over down (if both ok)
        //prefer right over left (if both ok)
        if(getTicks() > 0){
            switchDirection();
            moveAtAngle(getMDirection(), 2);
            setTicks(getTicks()-1);
        }
        if(getTicks() == 0){
            changePaused(true);
            resetCount();
        }
    }
}

void CoinSquare::doSomething()
{
    if(!getState()){
        return;
    }
    
    for(int i = 1; i < 3; i++){
        if(getWorld()->landed(*this, i) && !wasActivated(i)){
            if(givesMoney){
                getWorld()->addCoin(i, 3);
                getWorld()->playSound(SOUND_GIVE_COIN);
            }
            else{
                getWorld()->addCoin(i, -3);
                getWorld()->playSound(SOUND_TAKE_COIN);
            }
            
            setStatus(true, i);
        }
        
        if(!getWorld()->landed(*this, i) && wasActivated(i))
            setStatus(false, i);
    }
}

void StarSquare::doSomething()
{
    for(int i = 1; i < 3; i++){
        if((getWorld()->landed(*this, i) || getWorld()->passed(*this, i)) && !wasActivated(i)){
            if(getWorld()->getCoin(i) >= 20){
                getWorld()->addCoin(i, -20);
                getWorld()->addStar(i, 1);
                getWorld()->playSound(SOUND_GIVE_STAR);
            }
            
            setStatus(true, i);
        }
        
        if(!getWorld()->landed(*this, i) && wasActivated(i))
            setStatus(false, i);
    }
}

void DirectionSquare::doSomething()
{
    for(int i = 1; i < 3; i++){
        if(getWorld()->passed(*this, i) && !wasActivated(i)){
            getWorld()->setDirection(i, m_direction);
            getWorld()->changeForceStatus(i, true);
            if(m_direction == 180){
                getWorld()->setSDirection(i, 180);
            }
            else{
                getWorld()->setSDirection(i, 0);
            }
        }
        setStatus(true, i);
        if(!getWorld()->passed(*this, i) && wasActivated(i))
            setStatus(false, i);
    }
}

void BankSquare::doSomething()
{
    for(int i = 1; i < 3; i++){
        if(getWorld()->isChoosingAtFork(i))
            break;
        
        if(getWorld()->landed(*this, i) && !wasActivated(i)){
            getWorld()->addCoin(i, getWorld()->bankAmount());
            getWorld()->addBank(getWorld()->bankAmount()*-1);
            getWorld()->playSound(SOUND_WITHDRAW_BANK);
        }
        
        else if(getWorld()->passed(*this, i) && !wasActivated(i)){
            if(getWorld()->getCoin(i) >= 5){
                getWorld()->addCoin(i, -5);
                getWorld()->addBank(5);
            }
            else{
                getWorld()->addBank(getWorld()->getCoin(i));
                getWorld()->addCoin(i, getWorld()->getCoin(i)*-1);
            }
            getWorld()->playSound(SOUND_DEPOSIT_BANK);
            setStatus(true, i);
        }
        
        if(!getWorld()->landed(*this, i) && wasActivated(i))
            setStatus(false, i);
        
    }
}

void EventSquare::doSomething()
{
    for(int i = 1; i < 3; i++){
        if(getWorld()->landed(*this, i) && !wasActivated(i)){
            setStatus(true, i);
            int action = randInt(1, 3);
            if(action == 1){
                //TELEPORT PLAYER TO SQUARE ON BOARD
                getWorld()->teleportToRandom(i);
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            }
            else if(action == 2){
                //SWAP POSITION, MOVEMENT STATES, AND ACTIVATION STATUS FOR PLAYERS
                getWorld()->swapEverything(this, i);
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            }
            else{
                //GIVE PLAYER VORTEX
                getWorld()->giveVortex(i);
            }
        }
        
        if(!getWorld()->landed(*this, i) && wasActivated(i))
            setStatus(false, i);
    }
}

void DroppingSquare::doSomething()
{
    for(int i = 1; i < 3; i++){
        if(getWorld()->landed(*this, i) && !wasActivated(i)){
            setStatus(true, i);
            int action = randInt(1, 2);
            if(action == 1){
                if(getWorld()->getCoin(i) >= 10)
                    getWorld()->addCoin(i, -10);
                else
                    getWorld()->addCoin(i, getWorld()->getCoin(i)*-1);
            }
            if(action == 2){
                if(getWorld()->getStar(i) >= 1)
                    getWorld()->addStar(i, -1);
            }
            getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
            if(!getWorld()->landed(*this, i) && wasActivated(i))
                setStatus(false, i);
        }
    }
}
