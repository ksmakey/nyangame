#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Config.h"

USING_NS_CC;
using namespace std;

#define MAX_BLOCK_X 8
#define MAX_BLOCK_Y 8
#define REMOVING_TIME 0.1f
#define MOVING_TIME 0.2f
#define PNG_BACKGROUND "background.png"
#define MP3_REMOVE_BLOCK "removeBlock.mp3"

class GameScene : public cocos2d::Layer
{
protected:
    enum kTag
    {
        kTagBackground = 1,
        kTagBaseBlock = 10000
    };
    
    enum kZOrder
    {
        kZOrderBackground,
        kZOrderBlock
    };
    
    // like a Class
    struct PositionIndex
    {
        PositionIndex(int x1, int y1)
        {
            x = x1;
            y = y1;
        }
        
        int x;
        int y;
    };
    
    // backgroundImage
    Sprite* m_background;
    void showBackground();
    
    // block image height size
    float m_blockSize;
    // block tags
    map<kBlock, list<int>> m_blockTags;
    // initialize for block
    void initForVariables();
    void showBlock();
    // get block image's postion
    Vec2 getPosition(int posIndexX, int posIndexY);
    int getTag(int posIndexX, int posIndexY);
    
    // get touched block tag
    void getTouchBlockTag(Vec2 touchPoint, int &tag, kBlock &blockType);
    list<int> getSameColorBlockTags(int baseTag, kBlock blockType);
    void removeBlock(list<int> blockTags, kBlock blockType);
    // check is block included by list
    bool hasSameColorBlock(list<int> blockTagList, int searchBlockTag);
    
    void removingBlock(Node *block);
    
    vector<kBlock> blockTypes;
    PositionIndex getPositionIndex(int tag);
    // set new position index
    void setNewPosition1(int tag, PositionIndex posIndex);
    void searchNewPosition1(list<int> blocks);
    void moveBlock();
    void movingBlocksAnimation1(list<int> blocks);
    
    bool _animating;
    void movedBlocks(float dt);
    
    map<int, bool> getExistsBlockColumn();
    void searchNewPosition2();
    void setNewPosition2(int tag, PositionIndex posIndex);
    void movingBlocksAnimation2(float dt);
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
    // Touch Event
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchEnded(Touch *touch, Event *event);
};

#endif // __HELLOWORLD_SCENE_H__