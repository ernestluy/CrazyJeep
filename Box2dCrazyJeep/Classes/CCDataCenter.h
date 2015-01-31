//
//  CCDataCenter.h
//  Box2DIpadTest
//
//  Created by 兴天科技 on 13-9-13.
//
//

#ifndef __Box2DIpadTest__CCDataCenter__
#define __Box2DIpadTest__CCDataCenter__
#include "cocos2d.h"
#include "Box2D.h"
#include "PublicDefine.h"
using namespace cocos2d;
using namespace std;
class CCDataCenter {//: public cocos2d::CCObject
public:
    bool    m_isCrash;
    int     m_isGround;
public:
    CCDataCenter();
    static CCDataCenter * sharedInstance();
};
#endif /* defined(__Box2DIpadTest__CCDataCenter__) */
