//
//  CCMapCache.cpp
//  Box2DIpadTest
//
//  Created by 兴天科技 on 13-9-10.
//
//

#include "CCMapCache.h"
#include "PublicDefine.h"
#include "Utility.h"
using namespace cocos2d;
CCMapCache::CCMapCache(){
    m_pMapArray = new CCArray();
    m_pAllArray = new CCArray();
    memset(m_isLoad, 0, sizeof(m_isLoad));
    m_roleCar  = NULL;
}
CCMapCache::~CCMapCache(){
    delete m_pMapArray;
    m_pMapArray = NULL;
    delete m_pAllArray;
    
    m_roleCar->release();
}
void CCMapCache::initData(){
    for (int i = 0; i<SCREEN_ACCOUNT; i++) {
        CCString *tempStr = CCString::createWithFormat("map_%d.tmx",i+1);
        CCTMXTiledMap *tempMap = CCTMXTiledMap::create(tempStr->getCString());
        tempMap->setAnchorPoint(ccp(0,0));
        tempMap->retain();
        CCMapItem *item = new CCMapItem();
        item->autorelease();
        item->m_index = i;
        item->m_pMap = tempMap;
        m_pMapArray->addObject(item);
        this->readData(item, SCREEN_ITEM_WIDTH *i);
    }
}
void CCMapCache::readData(CCMapItem *mapItem,float startX) {

    
    float x, y;
    
    //polygon
    CCTMXObjectGroup *polygonObjects = mapItem->m_pMap ->objectGroupNamed("line");//[tileMapNode objectGroupNamed:@"Collision"];
    CCArray *polygonArray = polygonObjects->getObjects();
    //polygonArray->data
    
    
    
    
    if (polygonArray && polygonArray->count() > 0)
    {
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(polygonArray, pObj)
        {
            CCDictionary* spawnPoint = (CCDictionary*)pObj;
            CCString *xStr = (CCString *)spawnPoint->objectForKey("x");
            x = xStr->floatValue() + startX;
            xStr = (CCString *)spawnPoint->objectForKey("y");
            y = xStr->floatValue();
            CCLog("x:%f,y:%f",x,y);
            
            CCString* points = (CCString*)spawnPoint->objectForKey("polylinePoints");
            CCLog("   polyline:%s",points->getCString());
            std::vector<std::string> vect;
            std::vector<std::string> vectPoint;
            split(points->m_sString, " ", vect);
            CCPointArray *tempPointArray = CCPointArray::create(vect.size());
            
            for (int h = 0; h<vect.size(); h++) {
                float x2,y2;
                vectPoint.clear();
                split(vect[h], ",", vectPoint);
                if (vectPoint.size()==2) {
                    CCString *tempStr = CCString::create(vectPoint[0]);
                    x2 = tempStr->floatValue();
                    x2 = x+x2;
                    tempStr = CCString::create(vectPoint[1]);
                    y2 = tempStr->floatValue();
                    y2 = y - y2;
                    tempPointArray->addControlPoint(ccp(x2, y2));
                    //CCLog("   x:%f,y:%f",x2,y2);
                    //vecs[h] = b2Vec2(x2/PTM_RATIO,y2/PTM_RATIO);
                    /*
                    x2 = x2/PTM_RATIO;
                    y2 = y2/PTM_RATIO;
                    tempPointArray ->addControlPoint(ccp(x2, y2));
                     */
                }
                
            }
            mapItem->m_pPointArray->addObject(tempPointArray);
        }
    }
}

CCRoleCar * CCMapCache::reSetMap(){
    if (m_roleCar) {
        m_roleCar->release();
        m_roleCar  = NULL;
    }
    
    for (int i = 0; i<SCREEN_ACCOUNT; i++) {
        if (m_isLoad[i]) {
            m_isLoad[i] = false;
            this->removeMapWithIndex(i+1);
        }
    }
    
    this->makeMapWithIndex(0);
    return this->readRole();
}

CCRoleCar * CCMapCache::readRole(){
    CCRoleCar *role = new CCRoleCar();
    role->m_world = m_world;
    role->m_scene = m_scene;
    role->autorelease();
    m_roleCar = role;
    m_roleCar->retain();
    CCTMXTiledMap *tempMap = CCTMXTiledMap::create("play_role.tmx");
    //polygon
    float sx,sy;
    CCTMXObjectGroup *polygonObjects = tempMap->objectGroupNamed("object");//[tileMapNode
    
    
    CCDictionary *dict = (CCDictionary*)polygonObjects->objectNamed("car_body");
    CCString *xStr = (CCString *)dict->objectForKey("x");
    sx = xStr->floatValue();
    xStr = (CCString *)dict->objectForKey("y");
    sy = xStr->floatValue();
    CCLog("x:%f,y:%f",sx,sy);
    
    CCArray* points = (CCArray*)dict->objectForKey("points");
    b2Vec2 *vecs = (b2Vec2 *)malloc(sizeof(b2Vec2)*points->count());
    memset(vecs, 0, sizeof(b2Vec2)*points->count());
    float minX = 0,minY = 0,maxX = 0,maxY = 0;
    for (int i=0; i< points->count(); ++i) {
        CCDictionary* dict2 = (CCDictionary*)points->objectAtIndex(i);
        const char* key = "x";
        int x = ((CCString*)dict2->objectForKey("x"))->intValue();
        x = sx +x;
        key = "y";
        int y = ((CCString*)dict2->objectForKey(key))->intValue();
        y = sy -y;
        
        x = x/PTM_RATIO;
        y = y/PTM_RATIO;
        CCLog("x=%d, y=%d",x,y);
        vecs[i] = b2Vec2(x,y);
        
        if (i == 0) {
            minX = x;minY = y;maxX = x;maxY = y;
        }
        if (minX>x) {
            minX = x;
        }
        if (maxX <x) {
            maxX = x;
        }
        if (minY>y) {
            minY = y;
        }
        if (maxY < y) {
            maxY = y;
        }
    }
    for (int i = 0; i<points->count(); i++) {
        vecs[i].x = vecs[i].x - minX;
        vecs[i].y = vecs[i].y - minY;
        CCLog("x=%f, y=%f",vecs[i].x,vecs[i].y);
    }
    b2Vec2 bv2 ;
    bv2.Set(minX, minY);
    b2Body *bb = this->makeBox2DPolygonObject(m_world,bv2, vecs, points->count());
    free(vecs);
    
    //car_wheel_1
    CCDictionary *dictWheel = (CCDictionary*)polygonObjects->objectNamed("car_wheel_1");
    xStr = (CCString *)dictWheel->objectForKey("x");
    sx = xStr->floatValue();
    xStr = (CCString *)dictWheel->objectForKey("y");
    sy = xStr->floatValue();
    CCLog("x:%f,y:%f",sx,sy);
    xStr = (CCString *)dictWheel->objectForKey("width");
    float ra = xStr->floatValue()/2;
    b2Vec2 v2 ;
    v2.Set((sx+ra)/PTM_RATIO, (sy+ra)/PTM_RATIO );
    b2Body *w1 = this->createCarWheel(v2, ra/PTM_RATIO);
    
    
    ////car_wheel_2
    dictWheel = (CCDictionary*)polygonObjects->objectNamed("car_wheel_2");
    xStr = (CCString *)dictWheel->objectForKey("x");
    sx = xStr->floatValue();
    xStr = (CCString *)dictWheel->objectForKey("y");
    sy = xStr->floatValue();
    CCLog("x:%f,y:%f",sx,sy);
    xStr = (CCString *)dictWheel->objectForKey("width");
    ra = xStr->floatValue()/2;
    v2.Set((sx+ra)/PTM_RATIO, (sy+ra)/PTM_RATIO );
    b2Body *w2 = this->createCarWheel(v2, ra/PTM_RATIO);
    role->m_body = bb;
    role->m_wheel1 = w1;
    role->m_wheel2 = w2;
    
    //定义关节
    b2RevoluteJointDef revoluteJoint;
    revoluteJoint.Initialize( w1, bb, w1->GetWorldCenter());
    m_world->CreateJoint(&revoluteJoint);//.CreateJoint(revoluteJoint);
    
    revoluteJoint.Initialize( w2, bb, w2->GetWorldCenter());
    m_world->CreateJoint(&revoluteJoint);//.CreateJoint(revoluteJoint);
    return role;
}

b2Body* CCMapCache::createCarWheel(b2Vec2 &v2,float radius){

    CCSprite *tempBox = CCSprite::create("car_wheel.png");
    tempBox->setPosition(ccp(v2.x*PTM_RATIO, v2.y*PTM_RATIO));
    m_scene->addChild(tempBox);
    tempBox->setTag(kTagCircal);
    
    //body definition
    b2BodyDef myBodyDef;
    myBodyDef.type = b2_dynamicBody;
    myBodyDef.userData = tempBox;
    myBodyDef.position.Set(v2.x,v2.y);
    b2Body* circalBody = m_world->CreateBody(&myBodyDef);
    circalBody->SetLinearDamping(ANGULAR_DAMPING_L);
    circalBody->SetAngularDamping(ANGULAR_DAMPING_L);
    
    //shape definition
    b2CircleShape circleShape;
    circleShape.m_radius = radius;
    
    //fixture definition
    b2FixtureDef myFixtureDef;
    myFixtureDef.shape = &circleShape;
    myFixtureDef.density = MY_DENSITY;
    myFixtureDef.friction = 2;
    myFixtureDef.restitution = .2f;
    circalBody->CreateFixture(&myFixtureDef);
    return circalBody;
}
void CCMapCache::makeMapWithIndex(int index){
    if (index>= SCREEN_ACCOUNT) return;
    if (m_isLoad[index]) 
        return;
    
    CCLog("loading : index:%d",index);
    m_isLoad[index] = true;
    CCMapItem *tempItem = (CCMapItem *)m_pMapArray->objectAtIndex(index);
    m_currentMapItem = tempItem;
    m_pAllArray->addObjectsFromArray(tempItem->m_pPointArray);
    if (tempItem->m_pPointArray && tempItem->m_pPointArray->count() > 0)
    {
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(tempItem->m_pPointArray, pObj)
        {
            CCPointArray *tempPointArray = (CCPointArray *)pObj;
            b2Vec2 *vecs = (b2Vec2 *)malloc(sizeof(b2Vec2)*tempPointArray->count());
            memset(vecs, 0, sizeof(b2Vec2)*tempPointArray->count());
            
            for (int i = 0; i<tempPointArray->count(); i++) {
                CCPoint p = tempPointArray->getControlPointAtIndex(i);
                vecs[i] = b2Vec2(p.x/PTM_RATIO,p.y/PTM_RATIO);
            }
            this->makeBox2DEdgeObject(m_world, vecs, tempPointArray->count());
            free(vecs);
        }
        
    }

}
void CCMapCache::removeMapWithIndex(int index){
    if (index>=m_pMapArray->count()) {
        return;
    }
    if (!m_isLoad[index])
        return;
    
    CCLog("remove : index:%d",index);
    m_isLoad[index] = false;
    CCMapItem *tempItem = (CCMapItem *)m_pMapArray->objectAtIndex(index);
    //清楚线路的绘制
    m_pAllArray->removeObjectsInArray(tempItem->m_pPointArray);
    
    //清楚线路的刚体
    for (int i = 0; i<tempItem->m_bodyArray.size(); i++) {
        b2Body *tempBody = tempItem->m_bodyArray[i];
        m_world->DestroyBody(tempBody);
    }
    tempItem->m_bodyArray.clear();
}

void CCMapCache::makeBox2DEdgeObject(b2World* world,const b2Vec2* vertices, int32 count){
    //－－－－－－－－－
    b2BodyDef screenBoundsDef;
    screenBoundsDef.type = b2_staticBody;
    screenBoundsDef.position.Set(0.0f, 0.0f);
    b2Body *screenBounds = world->CreateBody(&screenBoundsDef);
    //screenBounds->SetUserData((void*)kTagFloor);
    
    
    m_currentMapItem->m_bodyArray.push_back(screenBounds);
    
    b2ChainShape worldShape;
    worldShape.CreateChain(vertices, count);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &worldShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    //fixtureDef.filter.categoryBits = 1;
    //fixtureDef.filter.maskBits = 1;
    screenBounds->CreateFixture(&fixtureDef);
}
b2Body * CCMapCache::makeBox2DPolygonObject(b2World* world,b2Vec2 &center,const b2Vec2* vertices, int32 count){
    CCSprite * block = CCSprite::create("car_body.png");//PolyShape_6
    block->setAnchorPoint(ccp(0, 0));
    float x = 500;
    float y = 500;
    //CCPoint point = ccp(x, y);
    block->setPosition(ccp(x, y));
    m_scene->addChild(block);
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.userData = block;
    //b2Vec2 vv  = b2Vec2( point.x/PTM_RATIO - 0.5,point.y/PTM_RATIO - 0.5);
    bodyDef.position.Set(center.x, center.y);
    
    b2Body *body = m_world->CreateBody(&bodyDef);
    
    // Define another box shape for our dynamic body.
    b2PolygonShape polygonShape;
    //polygonShape.SetAsBox(2.5, 2.5);
    polygonShape.Set(vertices, count);
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = MY_DENSITY;
    fixtureDef.friction = 0.1f;
    fixtureDef.restitution = 0.01f;
    //fixtureDef.filter.categoryBits = 1;
    //fixtureDef.filter.maskBits = 1;
    body->CreateFixture(&fixtureDef);
    return body;
    /*
    b2BodyDef screenBoundsDef;
    screenBoundsDef.type = b2_dynamicBody;
    screenBoundsDef.position.Set(point.x/PTM_RATIO,point.y/PTM_RATIO);
    
    block->setAnchorPoint(ccp(0,0));
    screenBoundsDef.userData = block;
    b2Body *screenBounds = world->CreateBody(&screenBoundsDef);
    
    //b2ChainShape worldShape;
    //worldShape.CreateChain(vertices, count);
    
    b2PolygonShape worldShape;
    //worldShape.SetAsBox(2.5, 2.5);
    worldShape.Set(vertices, count);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &worldShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.2f;
    screenBounds->CreateFixture(&fixtureDef);
     */
}
#pragma mark - CCRoleCar

CCRoleCar::CCRoleCar(){
    
}
CCRoleCar::~CCRoleCar(){
    CCSprite *sprite = (CCSprite *)m_body->GetUserData();
    m_scene->removeChild(sprite);
    sprite = (CCSprite *)m_wheel1->GetUserData();
    m_scene->removeChild(sprite);
    sprite = (CCSprite *)m_wheel2->GetUserData();
    m_scene->removeChild(sprite);
    
    m_world->DestroyBody(m_body);
    m_world->DestroyBody(m_wheel1);
    m_world->DestroyBody(m_wheel2);
}
#pragma mark - CCMapItem
CCMapItem::CCMapItem(){
    m_pPointArray = new CCArray();
}
CCMapItem::~CCMapItem(){
    m_pMap->release();
    delete m_pPointArray;
    m_pPointArray = NULL;

}

void CCMapItem::removePoint(CCArray *allPoint){
    allPoint->removeObjectsInArray(m_pPointArray);
}