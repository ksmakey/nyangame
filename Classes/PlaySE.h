//
//  PlaySE.h
//  nyangame
//
//  Created by Masaki Kase on 2014/08/17.
//
//

#ifndef __nyangame__PlaySE__
#define __nyangame__PlaySE__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class PlaySE : public ActionInstant
{
public:
    PlaySE(string sound);
    virtual ~PlaySE();
    
    virtual void update(float time);
//    virtual bool isDone();
//    virtual void step(float dt);
    virtual PlaySE* reverse() const;
    virtual PlaySE* clone() const;
    
public:
    static PlaySE* create(string sound);
    
protected:
    string m_sound;
};

#endif /* defined(__nyangame__PlaySE__) */
