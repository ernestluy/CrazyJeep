//
//  CCBackgroundCache.h
//  Box2DIpadTest
//
//  Created by 兴天科技 on 13-9-23.
//
//

#ifndef __Box2DIpadTest__CCBackgroundCache__
#define __Box2DIpadTest__CCBackgroundCache__
#include "cocos2d.h"
#include "Box2D.h"
#include "PublicDefine.h"


using namespace cocos2d;
using namespace std;
class CCBackgroundCache : public cocos2d::CCObject{
public:
    CCArray *m_backgroundArray;
    CCSprite *m_sb1Sprite;
    CCSprite *m_sb2Sprite;
    float    m_bgWidth;
    float    m_bgHeight;
    CCLayer *m_scene;
public:
    CCBackgroundCache();
    ~CCBackgroundCache();
    void setBackgroundScene(CCLayer *layer);
    void setBackgroundPosition(float p);
};
#endif /* defined(__Box2DIpadTest__CCBackgroundCache__) */
