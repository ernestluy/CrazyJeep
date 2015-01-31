//
//  CCDataCenter.cpp
//  Box2DIpadTest
//
//  Created by 兴天科技 on 13-9-13.
//
//

#include "CCDataCenter.h"

CCDataCenter * shareDataCenterInstance = NULL;
CCDataCenter * CCDataCenter::sharedInstance(){
    if (!shareDataCenterInstance) {
        shareDataCenterInstance = new CCDataCenter();
    }
    
    return shareDataCenterInstance;
}

CCDataCenter::CCDataCenter(){
    m_isCrash = false;
    m_isGround= 0;
}