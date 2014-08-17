//
//  GameScene.cpp
//  nyangame
//
//  Created by Masaki Kase on 2014/08/15.
//
//

#include "GameScene.h"
#include "BlockSprite.h"
#include "PlaySE.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    GameScene* layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

// init
bool GameScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved,this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    
    auto dip = Director::getInstance()->getEventDispatcher();
    dip->addEventListenerWithSceneGraphPriority(listener, this);
    
    initForVariables();
    
    showBackground();

    showBlock();
    
    // preload sound
    SimpleAudioEngine::getInstance()->preloadEffect(MP3_REMOVE_BLOCK);
    
    return true;
}

// show background image
void GameScene::showBackground()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    // create background image
    m_background = Sprite::create(PNG_BACKGROUND);
    m_background->setPosition(Vec2(winSize.width/2, winSize.height/2));
    addChild(m_background, kZOrder::kZOrderBackground, kTag::kTagBackground);
}

// initialize for block
void GameScene::initForVariables()
{
    // set random
    srand((unsigned)time(nullptr));
    
    // get block size
    BlockSprite* pBlock = BlockSprite::createWithBlockType(kBlockRed);
    m_blockSize = pBlock->getContentSize().height;
    
    blockTypes.push_back(kBlockRed);
    blockTypes.push_back(kBlockBlue);
    blockTypes.push_back(kBlockYellow);
    blockTypes.push_back(kBlockGreen);
    blockTypes.push_back(kBlockGray);
    
    _animating = false;
}

// show block images
void GameScene::showBlock()
{
    for (int x=0; x<MAX_BLOCK_X; x++) {
        for (int y=0; y<MAX_BLOCK_Y; y++) {
            // get block type
            kBlock blockType = (kBlock)(rand() % kBlockCount);
            
            int tag = getTag(x, y);
            m_blockTags[blockType].push_back(tag);
            
            // create block
            BlockSprite* pBlock = BlockSprite::createWithBlockType(blockType);
            pBlock->setPosition(getPosition(x, y));
            m_background->addChild(pBlock, kZOrderBlock, tag);
        }
    }
}

Vec2 GameScene::getPosition(int posIndexX, int posIndexY)
{
    float offsetX = m_background->getContentSize().width * 0.168;
    float offsetY = m_background->getContentSize().height * 0.029;
    
    return Vec2((posIndexX + 0.5) * m_blockSize + offsetX, (posIndexY + 0.5) * m_blockSize + offsetY);
}

int GameScene::getTag(int posIndexX, int posIndexY)
{
    return kTagBaseBlock + posIndexX * 100 + posIndexY;
}

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
    // if block is animating game don't recieve tap
    return !_animating;
}

void GameScene::onTouchEnded(Touch *touch, Event *event)
{
    Vec2 touchPoint = m_background->convertToNodeSpace(touch->getLocation());
    
    int tag = 0;
    kBlock blockType;
    getTouchBlockTag(touchPoint, tag, blockType);
    CCLOG("touch block: %i, %i", tag, blockType);
    
    if (tag != 0) {
        list<int> sameColorBlockTags = getSameColorBlockTags(tag, blockType);
        
        if (sameColorBlockTags.size() > 1) {
            _animating = true;
            
            removeBlock(sameColorBlockTags, blockType);
            movingBlocksAnimation1(sameColorBlockTags);
        }
    }
}

void GameScene::getTouchBlockTag(Vec2 touchPoint, int &tag, kBlock &blockType)
{
    for (int x=0; x<MAX_BLOCK_X; x++) {
        for (int y=0; y<MAX_BLOCK_Y; y++) {
            int currentTag = getTag(x, y);
            Node *node = m_background->getChildByTag(currentTag);
            if (node && node->boundingBox().containsPoint(touchPoint))
            {
                tag = currentTag;
                blockType = ((BlockSprite*)node)->getBlockType();
                return;
            }
        }
    }
}

bool GameScene::hasSameColorBlock(list<int> blockTagList, int searchBlockTag)
{
    list<int>::iterator it;
    for (it = blockTagList.begin(); it!=blockTagList.end(); ++it) {
        if (*it == searchBlockTag)
        {
            return true;
        }
    }
    return false;
}

list<int> GameScene::getSameColorBlockTags(int baseTag, kBlock blockType)
{
    list<int> sameColorBlockTags;
    sameColorBlockTags.push_back(baseTag);
    
    // push block tag to list in loop
    // list is change in loop
    list<int>::iterator it = sameColorBlockTags.begin();
    while (it != sameColorBlockTags.end()) {
        // around block's tag
        int tags[] = {
            *it + 100, // right block tag
            *it - 100, // left block tag
            *it + 1,   // up block tag
            *it - 1    // down block tag
        };
        
        for (int i=0; i<sizeof(tags)/sizeof(tags[0]); i++) {
            if (!hasSameColorBlock(sameColorBlockTags, tags[i])) {
                if (hasSameColorBlock(m_blockTags[blockType], tags[i])) {
                    sameColorBlockTags.push_back(tags[i]);
                }
            }
        }
        it++;
    }
    
    return sameColorBlockTags;
}

void GameScene::removeBlock(list<int> blockTags, kBlock blockType)
{
    bool first = true;
    
    list<int>::iterator it = blockTags.begin();
    while (it != blockTags.end()) {
        m_blockTags[blockType].remove(*it);
        
        Node *block = m_background->getChildByTag(*it);
        if (block)
        {
            //block->removeFromParentAndCleanup(true);
            // scaleTo
            ScaleTo* scale = ScaleTo::create(REMOVING_TIME, 0);
            // remove block
            CallFuncN* func = CallFuncN::create([this](Node* block){
                this->removingBlock(block);
            });
            
            FiniteTimeAction* sequense = Sequence::create(scale, func, nullptr);
            
            FiniteTimeAction* action;
            if (first) {
                PlaySE* playSE = PlaySE::create(MP3_REMOVE_BLOCK);
                action = Spawn::create(sequense, playSE, nullptr);
                first = false;
            }
            else
            {
                action = sequense;
            }
            
            block->runAction(action);
        }
        
        it++;
    }
    
    SimpleAudioEngine::getInstance()->playEffect(MP3_REMOVE_BLOCK);
}

void GameScene::removingBlock(Node *block)
{
    block->removeFromParentAndCleanup(true);
}

GameScene::PositionIndex GameScene::getPositionIndex(int tag)
{
    int pos1_x = (tag - kTagBaseBlock) / 100;
    int pos1_y = (tag - kTagBaseBlock) % 100;
    
    return PositionIndex(pos1_x, pos1_y);
}

void GameScene::setNewPosition1(int tag, PositionIndex posIndex)
{
    BlockSprite *blockSprite = (BlockSprite*)m_background->getChildByTag(tag);
    int nextPosY = blockSprite->getNextPosY();
    if (nextPosY == -1)
    {
        nextPosY = posIndex.y;
    }
    
    blockSprite->setNextPos(posIndex.x, --nextPosY);
}

void GameScene::searchNewPosition1(list<int> blocks)
{
    list<int>::iterator it1 = blocks.begin();
    while (it1 != blocks.end()) {
        PositionIndex posIndex1 = getPositionIndex(*it1);
        
        vector<kBlock>::iterator it2 = blockTypes.begin();
        while (it2 != blockTypes.end()) {
            list<int>::iterator it3 = m_blockTags[*it2].begin();
            while (it3 != m_blockTags[*it2].end()) {
                PositionIndex posIndex2 = getPositionIndex(*it3);
                
                if (posIndex1.x == posIndex2.x && posIndex1.y < posIndex2.y) {
                    setNewPosition1(*it3, posIndex2);
                }
                
                it3++;
            }
            
            it2++;
        }
        
        it1++;
    }
}

void GameScene::moveBlock()
{
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end()) {
        list<int>::iterator it2 = m_blockTags[*it1].begin();
        while (it2 != m_blockTags[*it1].end()) {
            BlockSprite *blockSprite = (BlockSprite*)m_background->getChildByTag(*it2);
            int nextPosX = blockSprite->getNextPosX();
            int nextPosY = blockSprite->getNextPosY();
            
            if (nextPosX != -1 || nextPosY != -1) {
                int newTag = getTag(nextPosX, nextPosY);
                blockSprite->initNextPos();
                blockSprite->setTag(newTag);
                
                *it2 = newTag;
                
                MoveTo *move = MoveTo::create(MOVING_TIME, getPosition(nextPosX, nextPosY));
                blockSprite->runAction(move);
            }
            
            it2++;
        }
        
        it1++;
    }
}

void GameScene::movingBlocksAnimation1(list<int> blocks)
{
    searchNewPosition1(blocks);
    
    moveBlock();
    
    scheduleOnce(schedule_selector(GameScene::movingBlocksAnimation2), MOVING_TIME);
}

void GameScene::movedBlocks(float dt)
{
    _animating = false;
}

void GameScene::setNewPosition2(int tag, PositionIndex posIndex)
{
    BlockSprite *blockSprite = (BlockSprite*)m_background->getChildByTag(tag);
    int nextPosX = blockSprite->getNextPosX();
    if (nextPosX == -1) {
        nextPosX = posIndex.x;
    }
    
    blockSprite->setNextPos(--nextPosX, posIndex.y);
}

map<int, bool> GameScene::getExistsBlockColumn()
{
    map<int, bool> xBlock;
    for (int i=0; i<MAX_BLOCK_X; i++) {
        xBlock[i] = false;
    }
    
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end()) {
        list<int>::iterator it2 = m_blockTags[*it1].begin();
        while (it2 != m_blockTags[*it1].end()) {
            xBlock[getPositionIndex(*it2).x] = true;
            it2++;
        }
        it1++;
    }
    
    return xBlock;
}

void GameScene::searchNewPosition2()
{
    map<int, bool> xBlocks = getExistsBlockColumn();
    
    bool first = true;
    for (int i=MAX_BLOCK_X-1; i>=0; i--) {
        if (xBlocks[i]) {
            first = false;
            continue;
        }
        else
        {
            if (first) {
                continue;
            }
            else
            {
                vector<kBlock>::iterator it1 = blockTypes.begin();
                while (it1 != blockTypes.end()) {
                    list<int>::iterator it2 = m_blockTags[*it1].begin();
                    while (it2 != m_blockTags[*it1].end()) {
                        PositionIndex posIndex = getPositionIndex(*it2);
                        
                        if (i < posIndex.x) {
                            setNewPosition2(*it2, posIndex);
                        }
                        
                        it2++;
                    }
                    
                    it1++;
                }
            }
        }
    }
}

void GameScene::movingBlocksAnimation2(float dt)
{
    searchNewPosition2();
    
    moveBlock();
    
    scheduleOnce(schedule_selector(GameScene::movedBlocks), MOVING_TIME);
}
