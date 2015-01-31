//
//  HelloWorldScene.h
//  Box2dCrazyJeep
//
//  Created by 兴天科技 on 13-9-24.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"
#include "CCMapCache.h"
#include "GLES-Render.h"
#include "MyContactListener.h"
#include "CCBackgroundCache.h"

class PhysicsSprite : public cocos2d::CCSprite
{
public:
    PhysicsSprite();
    void setPhysicsBody(b2Body * body);
    virtual bool isDirty(void);
    virtual cocos2d::CCAffineTransform nodeToParentTransform(void);
private:
    b2Body* m_pBody;    // strong ref
};

class HelloWorld : public cocos2d::CCLayer {
public:
    b2Body* m_c1;
    b2Body* m_c2;
    b2Body* m_body;
    b2Body *m_wheel2;
    GLESDebugDraw *debugDraw;    //
    bool    m_isTouch;
    float   m_force;
    float   m_climbing;
    
    cocos2d::CCTMXTiledMap *m_sceneMap;
    cocos2d::CCPointArray  *m_pArray;
    cocos2d::CCArray       *m_pAllArray;
    
    cocos2d::CCArray       *m_pMapArray;
    cocos2d::CCDictionary  *m_pDicCache;
    
    
    CCMapCache  *m_mapCache;
    CCRoleCar   *m_roleCar;
    
    CCBackgroundCache *m_bgCache;
    
    int    m_screenIndex;
    b2Body *m_trackBody;
    cocos2d::CCPointArray       *m_pPathArray;
    
    MyContactListener *b2Listener;
    CCScene *m_scene;
    CCLayer *m_layout;
    CCLabelTTF *m_timeLabel;
    CCLabelTTF *m_countdDownLabel;
    bool   m_isNeedForce;
    bool   m_isNeedAddForce;
    bool   m_isStart;
    int    m_countDownTime;
    float  m_timeCount;
    b2Vec2 m_climbingForce;
public:
    ~HelloWorld();
    HelloWorld();
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    void initLayout();
    void resetScene();
    void initPhysics();
    // adds a new sprite at a given coordinate
    void addNewSpriteAtPosition(cocos2d::CCPoint p);
    
    virtual void draw();
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void update(float dt);
    
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void FooTest();
    void createCircalShape();
    void createBigCircle();
    void createACar();
    void applyForce();
    
    void drawCollisionTiles(cocos2d::CCTMXTiledMap *sceneMap,float startX);
    void drawBackMap();
    void makeBox2dObjAt(cocos2d::CCPoint p,cocos2d::CCPoint size,bool b,float rotation,float  friction,float density,float restitution,int boxId);
    void makeBox2DEdgeObject(const b2Vec2* vertices, int32 count);
    void createEdge();
    
    void readMaps();
    void flushScreen();
    void flushStatue();
    
    void startGame();
    void counttime();
private:
    b2World* world;
    cocos2d::CCTexture2D* m_pSpriteTexture; // weak ref
};

#endif // __HELLO_WORLD_H__
