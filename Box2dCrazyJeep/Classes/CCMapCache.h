//
//  CCMapCache.h
//  Box2DIpadTest
//
//  Created by 兴天科技 on 13-9-10.
//
//

#ifndef __Box2DIpadTest__CCMapCache__
#define __Box2DIpadTest__CCMapCache__

#include "cocos2d.h"
#include "Box2D.h"
#include "PublicDefine.h"
using namespace cocos2d;
using namespace std;
class CCMapItem : public cocos2d::CCObject{
public:
    int          m_index = 0;
    CCTMXTiledMap *m_pMap;
    CCArray *m_pPointArray;
    vector<b2Body *> m_bodyArray;
public:
    CCMapItem();
    ~CCMapItem();
    
    void removePoint(CCArray *allPoint);
};

class CCRoleCar : public cocos2d::CCObject{
public:
    b2World* m_world;
    CCLayer *m_scene;
    b2Body *m_body;
    b2Body *m_wheel1;
    b2Body *m_wheel2;
public:
    CCRoleCar();
    ~CCRoleCar();
    
    void removePoint(CCArray *allPoint);
};

class CCMapCache : public cocos2d::CCObject{
public:
    cocos2d::CCArray       *m_pMapArray;
    CCArray       *m_pAllArray;
    CCMapItem *m_currentMapItem;
    
    b2World* m_world;
    CCLayer *m_scene;
    bool     m_isLoad[SCREEN_ACCOUNT];
    CCRoleCar *m_roleCar;
public:
    CCMapCache();
    ~CCMapCache();
    void initData();
    CCRoleCar * readRole();
    void readData(CCMapItem *mapItem,float startX);
    CCRoleCar * reSetMap();
    
    
    
    b2Body* createCarWheel(b2Vec2 &v2,float radius);
    void makeMapWithIndex(int index);
    void removeMapWithIndex(int index);
    void makeBox2DEdgeObject(b2World* world,const b2Vec2* vertices, int32 count);
    b2Body * makeBox2DPolygonObject(b2World* world,b2Vec2 &center,const b2Vec2* vertices, int32 count);
};

#endif /* defined(__Box2DIpadTest__CCMapCache__) */
