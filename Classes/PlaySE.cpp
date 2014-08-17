//
//  PlaySE.cpp
//  nyangame
//
//  Created by Masaki Kase on 2014/08/17.
//
//

#include "PlaySE.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

PlaySE::PlaySE(string sound)
{
    m_sound = sound;
}

PlaySE::~PlaySE()
{
    
}

PlaySE* PlaySE::create(string sound)
{
    PlaySE *pRet = new PlaySE(sound);
    if (pRet)
    {
        pRet->autorelease();
    }
    
    return pRet;
}

void PlaySE::update(float time)
{
    CC_UNUSED_PARAM(time);
    SimpleAudioEngine::getInstance()->playEffect(m_sound.c_str());
}

PlaySE* PlaySE::reverse() const
{
    return PlaySE::create(m_sound);
}

PlaySE* PlaySE::clone() const
{
    return PlaySE::create(m_sound);
}
