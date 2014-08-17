//
//  BlockSprite.h
//  nyangame
//
//  Created by Masaki Kase on 2014/08/16.
//
//

#ifndef __nyangame__BlockSprite__
#define __nyangame__BlockSprite__

#include "cocos2d.h"
#include "Config.h"

USING_NS_CC;

class BlockSprite : public Sprite
{
protected:
    const char* getBlockImageFileName(kBlock blockType);
    
public:
    CC_SYNTHESIZE_READONLY(kBlock, m_blockType, BlockType);
    CC_SYNTHESIZE_READONLY(int, _nextPosX, NextPosX);
    CC_SYNTHESIZE_READONLY(int, _nextPosY, NextPosY);
    
    BlockSprite();
    virtual ~BlockSprite();
    virtual bool initWithBlockType(kBlock blockType);
    static BlockSprite* createWithBlockType(kBlock blockType);
    
    // for move block
    void initNextPos();
    void setNextPos(int nextPosX, int nextPosY);
};

#endif /* defined(__nyangame__BlockSprite__) */
