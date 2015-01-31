//
//  MyContactListener.h
//  Box2DSongshu
//
//  Created by 兴天科技 on 13-8-2.
//
//

#ifndef __Box2DSongshu__MyContactListener__
#define __Box2DSongshu__MyContactListener__

#include <iostream>
#include "Box2D.h"
#include "cocos2d.h"
class MyContactListener : public b2ContactListener{

public:
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
};
#endif /* defined(__Box2DSongshu__MyContactListener__) */
