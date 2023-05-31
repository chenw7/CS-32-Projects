//
//  Player.hpp
//  Proj1New
//
//  Created by Simba Chen on 1/14/23.
//

#ifndef Player_h
#define Player_h

#include <stdio.h>
#include <string>
class Arena;

class Player
{
  public:
      // Constructor
    Player(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    std::string dropPoisonedCarrot();
    std::string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};


#endif /* Player_h */
