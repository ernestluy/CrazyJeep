//
//  MyContactListener.cpp
//  Box2DSongshu
//
//  Created by 兴天科技 on 13-8-2.
//
//

#include "MyContactListener.h"
#include "PublicDefine.h"
#include "cocos2d.h"
#include "CCDataCenter.h"
void MyContactListener::BeginContact(b2Contact* contact){
    CCDataCenter::sharedInstance()->m_isGround ++;
    /*
    cocos2d::CCSprite *spriteA = (cocos2d::CCSprite *)contact->GetFixtureA()->GetBody()->GetUserData();
    cocos2d::CCSprite *spriteB = (cocos2d::CCSprite *)contact->GetFixtureB()->GetBody()->GetUserData();

    //CCLog("Begin:%d",CCDataCenter::sharedInstance()->m_isGround);
    if (spriteA) {
        if (spriteA->getTag() == kTagCircal) {
            CCDataCenter::sharedInstance()->m_isGround ++;
        }
    }
    if (spriteB) {
        if (spriteB->getTag() == kTagCircal) {
            CCDataCenter::sharedInstance()->m_isGround ++;
        }
    }
    */

}


void MyContactListener::EndContact(b2Contact* contact){
    CCDataCenter::sharedInstance()->m_isGround --;
    
    /*
    cocos2d::CCSprite *spriteA = (cocos2d::CCSprite *)contact->GetFixtureA()->GetBody()->GetUserData();
    cocos2d::CCSprite *spriteB = (cocos2d::CCSprite *)contact->GetFixtureB()->GetBody()->GetUserData();
    if (spriteA) {
        if (spriteA->getTag() == kTagCircal) {
            CCDataCenter::sharedInstance()->m_isGround --;
        }
    }
    if (spriteB) {
        if (spriteB->getTag() == kTagCircal) {
            CCDataCenter::sharedInstance()->m_isGround --;
        }
    }
     */
    //CCLog("End:%d",CCDataCenter::sharedInstance()->m_isGround);
}