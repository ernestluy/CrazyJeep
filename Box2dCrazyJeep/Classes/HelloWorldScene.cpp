//
//  HelloWorldScene.cpp
//  Box2dCrazyJeep
//
//  Created by 兴天科技 on 13-9-24.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Utility.h"
#include "PublicDefine.h"
#include "CCDataCenter.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define PTM_RATIO 32
float MY_POSITIO_X = 200;
float MY_POSITION_Y = 200;
enum {
    kTagParentNode = 1,
};

PhysicsSprite::PhysicsSprite()
: m_pBody(NULL)
{

}

void PhysicsSprite::setPhysicsBody(b2Body * body)
{
    m_pBody = body;
}

// this method will only get called if the sprite is batched.
// return YES if the physics values (angles, position ) changed
// If you return NO, then nodeToParentTransform won't be called.
bool PhysicsSprite::isDirty(void)
{
    return true;
}

// returns the transform matrix according the Chipmunk Body values
CCAffineTransform PhysicsSprite::nodeToParentTransform(void)
{
    b2Vec2 pos  = m_pBody->GetPosition();

    float x = pos.x * PTM_RATIO;
    float y = pos.y * PTM_RATIO;

    if ( isIgnoreAnchorPointForPosition() ) {
        x += m_obAnchorPointInPoints.x;
        y += m_obAnchorPointInPoints.y;
    }

    // Make matrix
    float radians = m_pBody->GetAngle();
    float c = cosf(radians);
    float s = sinf(radians);

    if( ! m_obAnchorPointInPoints.equals(CCPointZero) ){
        x += c*-m_obAnchorPointInPoints.x + -s*-m_obAnchorPointInPoints.y;
        y += s*-m_obAnchorPointInPoints.x + c*-m_obAnchorPointInPoints.y;
    }

    // Rot, Translate Matrix
    m_sTransform = CCAffineTransformMake( c,  s,
        -s,    c,
        x,    y );

    return m_sTransform;
}

HelloWorld::HelloWorld()
{
    setTouchEnabled( true );
    setAccelerometerEnabled( true );

    //CCSize s = CCDirector::sharedDirector()->getWinSize();
    // init physics
    this->initPhysics();

    CCSpriteBatchNode *parent = CCSpriteBatchNode::create("blocks.png", 100);
    m_pSpriteTexture = parent->getTexture();

    addChild(parent, 0, kTagParentNode);


    
    m_pPathArray = CCPointArray::create(100);
    m_pPathArray->retain();
    
    m_pAllArray = CCArray::create();
    m_pAllArray->retain();
    m_pMapArray = CCArray::create();
    m_pMapArray->retain();
    
    m_mapCache = new CCMapCache();
    m_mapCache->m_scene = this;
    m_mapCache->m_world = world;
    m_pAllArray = m_mapCache->m_pAllArray;
    
    m_bgCache = new CCBackgroundCache();
    
    m_mapCache->initData();
    
    /*------------------------------------------------------------
     debugDraw = new GLESDebugDraw(PTM_RATIO);   //这里新建一个 debug渲染模块
     world->SetDebugDraw(debugDraw);    //设置
     uint32 flags = 0;
     flags += b2Draw::e_shapeBit ;
     //b2Draw::e_centerOfMassBit;   //获取需要显示debugdraw的块
     //b2Draw::e_aabbBit;  //AABB块
     //b2Draw::e_centerOfMassBit; 物体质心
     //b2Draw::e_jointBit;  //关节
     //b2Draw::e_shapeBit;   形状
     debugDraw->SetFlags(flags);   //需要显示那些东西
     **/
    //------------------------------------------------------------
    //world->SetContactListener((b2ContactListener*)this);
    b2Listener = new MyContactListener();
    world->SetContactListener(b2Listener);
    
    //audio  engine.mp3
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("engine.mp3");
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("engine.caf");
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("engine.caf", true);
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    //    schedule_selector
    
    
    schedule(schedule_selector(HelloWorld::flushScreen), 0.5);
    
    schedule(schedule_selector(HelloWorld::flushStatue), 0.1);
    
    schedule(schedule_selector(HelloWorld::counttime), 1.0);
    scheduleUpdate();
}

HelloWorld::~HelloWorld()
{
    delete world;
    world = NULL;
    
    delete m_pArray;
    m_pArray = NULL;
    
    delete m_pAllArray;
    m_pAllArray = NULL;
    
    delete m_pMapArray;
    m_pMapArray  = NULL;
    
    delete  m_mapCache;
    m_mapCache = NULL;
    
    delete b2Listener;
    b2Listener  = NULL;
    
    delete m_bgCache;
    
    //delete m_debugDraw;
}

void HelloWorld::flushScreen(){
    
    b2Vec2 pos = m_c1->GetPosition();
    int tx = pos.x * PTM_RATIO;
    //CCLog("flushScreen x:%d",tx);
    int index = (int)tx/(int)SCREEN_ITEM_WIDTH;
    int im = (int)tx%(int)SCREEN_ITEM_WIDTH;
    m_screenIndex = index;
    if (im>SCREEN_ONE *2) {//缓存前进方向的线路
        m_mapCache->makeMapWithIndex(m_screenIndex+1);
        if (m_screenIndex>0) {
            m_mapCache->removeMapWithIndex(m_screenIndex-1);
        }
    }else if (im<SCREEN_ONE){
        if (m_screenIndex>0) {
            m_mapCache->makeMapWithIndex(m_screenIndex-1);
            m_mapCache->removeMapWithIndex(m_screenIndex+1);
        }
    }
    //b2Vec2 pos2 = m_trackBody->GetPosition();
    //CCLog("x:%f,y:%f",pos2.x,pos2.y);
    
    //m_pPathArray->addControlPoint(ccp(pos2.x*PTM_RATIO, pos2.y*PTM_RATIO));
    //if(m_isTouch)CCLog("f:%f,c:%f",m_force,m_climbing);
    if (m_isTouch) {//增加判断，如果速度超过20就不需要额外的推力
        b2Vec2 vel = m_c1->GetLinearVelocity();
        //CCLog("speed:%f",vel.x);
        if (vel.x > MAX_SPEED) {
            m_isNeedAddForce = true;
        }else{
            m_isNeedAddForce = false;
        }
    }
    if (m_force<0) {
        if (m_force>(-PTM_FORCE)) {
            m_force -= INTERVAL_ADD_FORCE;
        }
        
        if (m_climbing<CLIMBING_FORCE) {
            m_climbing += INTERVAL_ADD_FORCE;
        }
    }else{
        if (m_force<PTM_FORCE) {
            m_force += INTERVAL_ADD_FORCE;
        }
        if (m_climbing>(-CLIMBING_FORCE)) {
            m_climbing -= INTERVAL_ADD_FORCE;
        }
    }
    
    
}


//施加防止翻滚的冲量
void HelloWorld::flushStatue(){
    if (m_isStart) {
        m_timeCount+=0.2;
        CCString *timeStr = CCString::createWithFormat("%f",m_timeCount);
        m_timeLabel->setString(timeStr->getCString());
    }
    
    b2Vec2 pos1 = m_c1->GetPosition();
    b2Vec2 pos2 = m_c2->GetPosition();
    b2Vec2 bodyPos = m_body->GetPosition();
    float diffY = pos2.y- pos1.y;
    float diffX = pos2.x- pos1.x;
    float r = diffY/diffX;
    
    if (CCDataCenter::sharedInstance()->m_isGround >= 2 && bodyPos.x<pos1.x){//&& pos1.y<pos2.y && bodyPos.y<pos1.y
        m_isNeedForce = true;
        m_climbingForce.Set(m_climbing, m_climbing*r);
    }else{
        m_isNeedForce = false;
    }
    if (!m_isTouch) {
        m_isNeedForce = false;
    }
    if (CCDataCenter::sharedInstance()->m_isGround == 0) {//!CCDataCenter::sharedInstance()->m_isCrash
        float lforce = 3;
        r = abs(r);
        if (pos1.y>pos2.y  && r>0.7) {
            //CCLog("lforce ");
            m_c2->ApplyLinearImpulse( b2Vec2(0,lforce), m_c2->GetWorldCenter());
            m_c1->ApplyLinearImpulse( b2Vec2(0,-lforce), m_c1->GetWorldCenter());
        }else{
            //circalBody->ApplyLinearImpulse( b2Vec2(0,lforce), circalBody->GetWorldCenter());
        }
    }
    
}

void HelloWorld::initPhysics()
{

    //CCSize s = CCDirector::sharedDirector()->getWinSize();

    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    world = new b2World(gravity);

    // Do we want to let bodies sleep?
    world->SetAllowSleeping(true);

    world->SetContinuousPhysics(true);

//     m_debugDraw = new GLESDebugDraw( PTM_RATIO );
//     world->SetDebugDraw(m_debugDraw);
    /*
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    //        flags += b2Draw::e_jointBit;
    //        flags += b2Draw::e_aabbBit;
    //        flags += b2Draw::e_pairBit;
    //        flags += b2Draw::e_centerOfMassBit;
    //m_debugDraw->SetFlags(flags);


    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2EdgeShape groundBox;

    // bottom

    groundBox.Set(b2Vec2(0,0), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);

    // top
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);

    // left
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));
    groundBody->CreateFixture(&groundBox,0);

    // right
    groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
     */
}
void HelloWorld::applyForce(){
    if (!m_isStart) {
        return;
    }
    m_c1->ApplyTorque(m_force);
    m_c2->ApplyTorque(m_force);
    if (m_isNeedForce) {//m_isNeedForce
        m_c1->ApplyForce( m_climbingForce, m_c1->GetWorldCenter());
        m_c2->ApplyForce( m_climbingForce, m_c2->GetWorldCenter());
    }
    //m_wheel2->GetLinearVelocity()
}

void HelloWorld::initLayout(){
    /*
     //背景层
     CCLayer* bgLayer = new CCLayer();
     bgLayer->autorelease();
     m_bgCache->setBackgroundScene(bgLayer);
     m_scene->addChild(bgLayer);
     */
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    //-------------reset按钮
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(HelloWorld::resetScene) );
    pCloseItem->setPosition( ccp(size.width - 20, 20) );
    
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    //this->addChild(pMenu, 10);
    
    
    CCLayer* layer = new CCLayer();
    m_layout = layer;
    layer->autorelease();
    layer->addChild(pMenu, 10);
    m_scene->addChild(layer);
    
    m_timeLabel = CCLabelTTF::create("TIME", "Marker Felt", 32);
    m_layout->addChild(m_timeLabel, 0);
    m_timeLabel->setColor(ccc3(0,0,255));
    m_timeLabel->setPosition(ccp( 100, size.height-50));
    
    m_countdDownLabel = CCLabelTTF::create("countdDownLabel", "Marker Felt", 32);
    m_countdDownLabel->retain();
    m_layout->addChild(m_countdDownLabel);
    m_countdDownLabel->setColor(ccc3(0,0,255));
    m_countdDownLabel->setPosition(ccp( 280, size.height-50));
    
    
}

void HelloWorld::startGame(){
    m_countDownTime = COUNTDOW_TIME_S;
    CCString *cdStr = CCString::createWithFormat("%d",m_countDownTime);
    m_countdDownLabel->setString(cdStr->getCString());
    if (m_countdDownLabel->getParent() == NULL) {
        m_layout->addChild(m_countdDownLabel);
    }
    
    
    m_timeCount = 0;
    CCString *tStr = CCString::createWithFormat("%f",m_timeCount);
    m_timeLabel->setString(tStr->getCString());
    m_isStart = false;
    
}
void HelloWorld::counttime(){
    if (m_isStart) {
        return;
    }
    CCLog("counttime:%d",m_countDownTime);
    CCString *cdStr = CCString::createWithFormat("%d",m_countDownTime);
    m_countdDownLabel->setString(cdStr->getCString());
    m_countDownTime --;
    
    if (m_countDownTime < 0) {
        m_isStart = true;
        if (m_countdDownLabel->getParent()) {
            m_countdDownLabel->removeFromParent();
        }
        
    }
    
}
void HelloWorld::resetScene(){

    m_roleCar = m_mapCache->reSetMap();
    m_c1 = m_roleCar->m_wheel1;
    m_c2 = m_roleCar->m_wheel2;
    m_body = m_roleCar->m_body;
    m_mapCache->makeMapWithIndex(0);
    m_screenIndex = 0;
    
    
    startGame();
}
void HelloWorld::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    kmGLPushMatrix();

    world->DrawDebugData();
    
    //add by 61
    kmGLTranslatef(0, 0, 0);//kmGLTranslatef(50, 50, 0);//设置位置,（x，y，z）
    //ccDrawCardinalSpline(m_pArray, 1, 100);//根据点与张力来画线（参数1：点数组，参数2：张力值，参数三：精度,指的是线段的精细程度,越大分段越多,越精细）//m_pArray, 7, 100
    
   
    glLineWidth( 2.0f );
    ccDrawColor4B(255,0,0,255);
    
    if (m_pAllArray && m_pAllArray->count() > 0)
    {
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(m_pAllArray, pObj)
        {
            CCPointArray *tempPointArray = (CCPointArray *)pObj;
            ccDrawCardinalSpline(tempPointArray, 1, 100);
        }
    }

    kmGLPopMatrix();
}

void HelloWorld::addNewSpriteAtPosition(CCPoint p)
{
    if (1) {
        return;
    }
    CCLOG("Add sprite %0.2f x %02.f",p.x,p.y);
    CCNode* parent = getChildByTag(kTagParentNode);
    
    //We have a 64x64 sprite sheet with 4 different 32x32 images.  The following code is
    //just randomly picking one of the images
    int idx = (CCRANDOM_0_1() > .5 ? 0:1);
    int idy = (CCRANDOM_0_1() > .5 ? 0:1);
    PhysicsSprite *sprite = new PhysicsSprite();
    sprite->initWithTexture(m_pSpriteTexture, CCRectMake(32 * idx,32 * idy,32,32));
    sprite->autorelease();
    
    parent->addChild(sprite);
    
    sprite->setPosition( CCPointMake( p.x, p.y) );
    
    // Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
    
    b2Body *body = world->CreateBody(&bodyDef);
    
    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(.5f, .5f);//These are mid points for our 1m box
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;    
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);
    
    sprite->setPhysicsBody(body);
}


void HelloWorld::update(float dt)
{
    
    if (m_isTouch) {
        this->applyForce();
    }
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    int velocityIterations = 8;
    int positionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    world->Step(dt, velocityIterations, positionIterations);
    
    //Iterate over the bodies in the physics world
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            //Synchronize the AtlasSprites position and rotation with the corresponding body
            CCSprite* myActor = (CCSprite*)b->GetUserData();
            myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
        }    
    }
    
    b2Vec2 pos = m_c1->GetPosition();
	
	CCPoint newPos = ccp(-1 * pos.x * PTM_RATIO + MY_POSITIO_X,MY_POSITION_Y - pos.y * PTM_RATIO);//this->getPosition().y  * PTM_RATIO
	
    this->setPosition(newPos);
    //m_bgCache->setBackgroundPosition(newPos.x/5);
    if (m_isTouch) {
        if (m_force>0) {
            int dd  = 200;
            if (MY_POSITIO_X>dd) {
                MY_POSITIO_X -=INTERVAL_SCREEN_MOVE;
            }
        }else{
            int dd  = 500;
            if (MY_POSITIO_X<dd) {
                MY_POSITIO_X +=INTERVAL_SCREEN_MOVE;
            }
            
        }
    }
}
#pragma mark -
void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
    
    CCSetIterator it;
    CCTouch* touch;
    m_isTouch = true;
    for( it = pTouches->begin(); it != pTouches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
    }
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    CCSize ss = CCDirector::sharedDirector()->getWinSize();
    int d = ss.width/2;
    if (location.x>d) {
        m_force = -INTERVAL_ADD_FORCE;
        m_climbing = INTERVAL_ADD_FORCE;
    }else{
        m_force = INTERVAL_ADD_FORCE;
        m_climbing = -INTERVAL_ADD_FORCE;
    }
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    //this->applyForce();
    
}
void HelloWorld::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
    
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    m_isTouch = false;
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    if (1) {
        return;
    }
    //Add a new body/atlas sprite at the touched location
    CCSetIterator it;
    CCTouch* touch;
    
    for( it = touches->begin(); it != touches->end(); it++) 
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint location = touch->getLocationInView();
        
        location = CCDirector::sharedDirector()->convertToGL(location);
        
        addNewSpriteAtPosition( location );
    }
}
#pragma mark -
CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new HelloWorld();
    HelloWorld *hh = (HelloWorld*)layer;
    hh->m_scene = scene;
    hh->initLayout();
    hh->resetScene();
    scene->addChild(layer);
    layer->release();
    return scene;
}
