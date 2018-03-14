#include "HelloWorldScene.h"
#pragma execution_character_set("utf-8")
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	float winw = visibleSize.width;  // 获取屏幕宽度
	float winh = visibleSize.height; // 获取屏幕高度

	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	//从贴图中以像素单位切割，创建关键帧
	frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height/2));
	addChild(player, 3);

	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用hp条设置progressBar
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(origin.x+14*pT->getContentSize().width,origin.y + visibleSize.height - 2*pT->getContentSize().height));
	addChild(pT,1);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0,0);

	// 静态动画
	idle.reserve(1);
	idle.pushBack(frame0);

	// 攻击动画
	attack.reserve(17);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113*i,0,113,113)));
		attack.pushBack(frame);
	}

	// 可以仿照攻击动画
	// 死亡动画(帧数：22帧，高：90，宽：79）
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
	dead.reserve(22);
	for (int i = 0; i < 22; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
		dead.pushBack(frame);
	}

	// 运动动画(帧数：8帧，高：101，宽：68）
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
	run.reserve(8);
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
		run.pushBack(frame);
	}
	run.pushBack(frame0);

	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 36;
	auto menuLabel1 = Label::createWithTTF(ttfConfig, "W");
	auto menuLabel2 = Label::createWithTTF(ttfConfig, "S");
	auto menuLabel3 = Label::createWithTTF(ttfConfig, "A");
	auto menuLabel4 = Label::createWithTTF(ttfConfig, "D");
	auto menuLabel5 = Label::createWithTTF(ttfConfig, "X");
	auto menuLabel6 = Label::createWithTTF(ttfConfig, "Y");

	auto item1 = MenuItemLabel::create(menuLabel1, CC_CALLBACK_1(HelloWorld::moveEvent_w, this));
	auto item2 = MenuItemLabel::create(menuLabel2, CC_CALLBACK_1(HelloWorld::moveEvent_s, this));
	auto item3 = MenuItemLabel::create(menuLabel3, CC_CALLBACK_1(HelloWorld::moveEvent_a, this));
	auto item4 = MenuItemLabel::create(menuLabel4, CC_CALLBACK_1(HelloWorld::moveEvent_d, this));
	auto item5 = MenuItemLabel::create(menuLabel5, CC_CALLBACK_1(HelloWorld::deadEvent, this));
	auto item6 = MenuItemLabel::create(menuLabel6, CC_CALLBACK_1(HelloWorld::attackEvent, this));

	item1->setPosition(Vec2(70, 70));
	item2->setPosition(Vec2(70, 30));
	item3->setPosition(Vec2(30, 30));
	item4->setPosition(Vec2(110, 30));
	item5->setPosition(Vec2(winw - 30, 70));
	item6->setPosition(Vec2(winw - 70, 30));

	auto menu = Menu::create(item1, item2, item3, item4, item5, item6, NULL);
	menu->setPosition(Vec2(0, 0));
	addChild(menu, 0);

	time = Label::createWithTTF(ttfConfig, "180");
	time->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - time->getContentSize().height));
	schedule(schedule_selector(HelloWorld::update), 1.0f, kRepeatForever, 0);
	dtime = 180;
	this->addChild(time, 1);

	return true;
}

void HelloWorld::moveEvent_w(Ref* pSender) {
	if (player->getSpriteFrame() == frame0 && pT->getPercentage()) {
		auto animation = Animation::createWithSpriteFrames(run, 0.05f, 1);
		auto animate = Animate::create(animation);
		float winh = visibleSize.height; // 获取屏幕高度
		float playh = player->getContentSize().height;
		int nowy = player->getPosition().y;
		if (winh - nowy - playh / 2 >= 50) {
			player->runAction(Spawn::create(animate, MoveBy::create(0.45f, Vec2(0, 50)), nullptr));
		} else {
			player->runAction(Spawn::create(animate, MoveBy::create(0.45f, Vec2(0, winh - nowy - playh / 2)), nullptr));
		}
	}
}

void HelloWorld::attackEvent(Ref* pSender) {
	if (player->getSpriteFrame() == frame0 && pT->getPercentage()) {
		auto animation = Animation::createWithSpriteFrames(attack, 0.1f);
		animation->setRestoreOriginalFrame(true);
		auto animate = Animate::create(animation);
		player->runAction(Sequence::create(animate, NULL));
		pT->setPercentage(pT->getPercentage() + 15.0);
	}
}

void HelloWorld::deadEvent(Ref* pSender) {
	if (player->getSpriteFrame() == frame0 && pT->getPercentage()) {
		auto animation = Animation::createWithSpriteFrames(dead, 0.1f);
		animation->setRestoreOriginalFrame(true);
		auto animate = Animate::create(animation);
		player->runAction(Sequence::create(animate, NULL));
		pT->setPercentage(pT->getPercentage() - 15.0);
	}
}

void HelloWorld::moveEvent_a(Ref* pSender) {
	if (player->getSpriteFrame() == frame0 && pT->getPercentage()) {
		auto animation = Animation::createWithSpriteFrames(run, 0.05f, 1);
		auto animate = Animate::create(animation);
		//float winw = visibleSize.width;  // 获取屏幕宽度
		float playw = player->getContentSize().width;
		int nowx = player->getPosition().x;
		if (nowx - playw / 2 - 50 >= 0) {
			player->runAction(Spawn::create(animate, MoveBy::create(0.45f, Vec2(-50, 0)), nullptr));
		}
		else {
			player->runAction(Spawn::create(animate, MoveBy::create(0.45f, Vec2(-(nowx - playw / 2), 0)), nullptr));
		}
	}
}

void HelloWorld::moveEvent_s(Ref* pSender) {
	if (player->getSpriteFrame() == frame0 && pT->getPercentage()) {
		auto animation = Animation::createWithSpriteFrames(run, 0.05f, 1);
		auto animate = Animate::create(animation);
		float playh = player->getContentSize().height;
		int nowy = player->getPosition().y;
		if (nowy - playh / 2 - 50 >= 0) {
			player->runAction(Spawn::create(animate, MoveBy::create(0.45f, Vec2(0, -50)), nullptr));
		}
		else {
			player->runAction(Spawn::create(animate, MoveBy::create(0.45f, Vec2(0, -(nowy - playh / 2))), nullptr));
		}
	}
}

void HelloWorld::moveEvent_d(Ref* pSender) {
	if (player->getSpriteFrame() == frame0 && pT->getPercentage()) {
		auto animation = Animation::createWithSpriteFrames(run, 0.05f, 1);
		auto animate = Animate::create(animation);
		float winw = visibleSize.width;  // 获取屏幕宽度
		float playw = player->getContentSize().width;
		int nowx = player->getPosition().x;
		if (winw - nowx - playw / 2 >= 50) {
			player->runAction(Spawn::create(animate, MoveBy::create(0.45f, Vec2(50, 0)), nullptr));
		}
		else {
			player->runAction(Spawn::create(animate, MoveBy::create(0.45f, Vec2(winw - nowx - playw / 2, 0)), nullptr));
		}
	}
}

void HelloWorld::update(float dt) {
	dtime --;
	if (dtime < 0) 
		dtime = 0;
	time->setString(std::to_string(dtime));
}