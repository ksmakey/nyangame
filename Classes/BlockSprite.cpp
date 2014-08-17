//
//  BlockSprite.cpp
//  nyangame
//
//  Created by Masaki Kase on 2014/08/16.
//
//

#include "BlockSprite.h"

// Constructor
BlockSprite::BlockSprite()
{
    initNextPos();
}

// Desctructor
BlockSprite::~BlockSprite()
{
    
}

// create BlockSprite
BlockSprite* BlockSprite::createWithBlockType(kBlock blockType)
{
    BlockSprite *pRet = new BlockSprite();
    if (pRet && pRet->initWithBlockType(blockType))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

// initialize BlockSprite
bool BlockSprite::initWithBlockType(kBlock blockType)
{
    if (!Sprite::initWithFile(getBlockImageFileName(blockType))) {
        return false;
    }
    
    m_blockType = blockType;
    return true;
}

// get Target Image's Filename
const char* BlockSprite::getBlockImageFileName(kBlock blockType)
{
    switch (blockType) {
        case kBlockRed:
            return "red.png";
        case kBlockBlue:
            return "blue.png";
        case kBlockYellow:
            return "yellow.png";
        case kBlockGreen:
            return "green.png";
        case kBlockGray:
            return "gray.png";
        default:
            CCASSERT(false, "invalid blockType");
            return "";
    }
}

void BlockSprite::initNextPos()
{
    _nextPosX = -1;
    _nextPosY = -1;
}

void BlockSprite::setNextPos(int nextPosX, int nextPosY)
{
    _nextPosX = nextPosX;
    _nextPosY = nextPosY;
}
