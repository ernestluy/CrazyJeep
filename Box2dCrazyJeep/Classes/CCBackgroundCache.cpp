//
//  CCBackgroundCache.cpp
//  Box2DIpadTest
//
//  Created by 兴天科技 on 13-9-23.
//
//

#include "CCBackgroundCache.h"
#define BG_RESET_X  (-10)
CCBackgroundCache::CCBackgroundCache(){
    /*
    m_sb1Sprite = CCSprite::create("sb1.png");
    m_sb1Sprite->setAnchorPoint(ccp(0, 0));
    m_sb1Sprite->setPosition(ccp(0, 0));
    m_sb1Sprite->retain();
    
    m_sb2Sprite = CCSprite::create("sb2.png");
    m_sb2Sprite->setAnchorPoint(ccp(0, 0));
    m_sb2Sprite->setPosition(ccp(m_sb1Sprite->getContentSize().width, 0));
    m_sb2Sprite->retain();
    
    m_bgWidth = m_sb1Sprite->getContentSize().width;
    m_bgHeight = m_sb1Sprite->getContentSize().height;
    */
}

CCBackgroundCache::~CCBackgroundCache(){
    m_sb1Sprite->release();
    m_sb2Sprite->release();
}


void CCBackgroundCache::setBackgroundScene(CCLayer *layer){
    m_scene = layer;
    m_scene->addChild(m_sb1Sprite);
    //m_scene->addChild(m_sb2Sprite);
}

void CCBackgroundCache::setBackgroundPosition(float p){
    //CCSize s = CCDirector::sharedDirector()->getWinSize();
    int mX = (int)p%(int)(m_bgWidth);
    if (mX>=0) {
        return;
    }
    int tindex = p/m_bgWidth;
    if (tindex%2 == 0) {
        m_sb1Sprite->setPosition(ccp(mX, 0));
        m_sb2Sprite->setPosition(ccp(m_bgWidth+mX, 0));
    }else{
        m_sb2Sprite->setPosition(ccp(mX, 0));
        m_sb1Sprite->setPosition(ccp(m_bgWidth+mX, 0));
    }
    
    
}