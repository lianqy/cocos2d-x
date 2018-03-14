#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;
class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	virtual void moveEvent_w(Ref* pSender);
	virtual void moveEvent_a(Ref* pSender);
	virtual void moveEvent_s(Ref* pSender);
	virtual void moveEvent_d(Ref* pSender);
	virtual void attackEvent(Ref* pSender);
	virtual void deadEvent(Ref* pSender);

	bool attackMonster();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	void hitByMonster(float dt);
	void creatMonster(float dt);
private:
	cocos2d::Sprite* player;
	cocos2d::Vector<SpriteFrame*> attack;
	cocos2d::Vector<SpriteFrame*> dead;
	cocos2d::Vector<SpriteFrame*> run;
	cocos2d::Vector<SpriteFrame*> idle;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Label* time;
	cocos2d::SpriteFrame* frame0;
	int killNum;
	cocos2d::ProgressTimer* pT;
};

#endif // __HELLOWORLD_SCENE_H__
