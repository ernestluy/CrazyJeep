//
//  PublicDefine.h
//  Box2DIpadTest
//
//  Created by 兴天科技 on 13-9-10.
//
//

#ifndef Box2DIpadTest_PublicDefine_h
#define Box2DIpadTest_PublicDefine_h
enum TKTag{
    kTagBullet = 1,
    kTagFloor,
    kTagCircal,
    kTagIre,
};





#define PTM_RATIO 32
#define PTM_FORCE 25//60     //车轮的扭力
#define CLIMBING_FORCE 20//10 //爬坡的外力

#define INTERVAL_ADD_FORCE  5
#define MAX_SPEED  18

#define COUNTDOW_TIME_S  6

#define  START_X   11
#define ANGULAR_DAMPING_L 0.3
#define MY_DENSITY 0.3//3车的密度
#define WHEEL_DENSITY 0.15//车轮的密度

#define INTERVAL_SCREEN_MOVE 2//屏幕的宽度

#define SCREEN_ONE  480.0//屏幕的宽度

#define SCREEN_ITEM_WIDTH   (SCREEN_ONE*3.0)//每张地形的宽度
#define SCREEN_ACCOUNT      8               //地形的张数



#define MY_CCP(tb2Vec2) ccp((tb2Vec2).x * PTM_RATIO ,(tb2Vec2).y *PTM_RATIO)
#endif
