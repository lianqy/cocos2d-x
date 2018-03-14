#include "GameSence.h"

USING_NS_CC;

Scene* GameSence::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameSence::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameSence::init()
{

	if (!Layer::init())
	{
		return false;
	}

	srand((unsigned)time(NULL));

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//设置背景
	float winw = visibleSize.width;  // 获取屏幕宽度
	float winh = visibleSize.height; // 获取屏幕高度
	auto bg = Sprite::create("level-background-0.jpg");
	bg->setPosition(Vec2(winw / 2 + origin.x, winh / 2 + origin.y));
	float spx = bg->getTextureRect().getMaxX();
	bg->setScaleX(winw / spx);      //  背景缩放
	bg->setScaleY(winw / spx);
	this->addChild(bg, 0);

	//增加StoneLayer
	stoneLayer = Layer::create();
	stoneLayer->ignoreAnchorPointForPosition(false);
	stoneLayer->setAnchorPoint(Vec2::ZERO);
	stoneLayer->setPosition(Vec2::ZERO);

	//添加石头
	stone = Sprite::create("stone.png");
	stone->setPosition(Vec2(560, 480));
	stoneLayer->addChild(stone);

	//增加MouseLayer
	mouseLayer = Layer::create();
	mouseLayer->ignoreAnchorPointForPosition(false);
	mouseLayer->setAnchorPoint(Vec2(0, 0));
	mouseLayer->setPosition(Vec2(0, winh / 2));

	//添加SHOOT
	auto label = Label::createWithTTF("Shoot", "fonts/arial.ttf", 70);
	auto shootBtn = MenuItemLabel::create(label, CC_CALLBACK_1(GameSence::shootMenuCallback, this));
	auto shoot = Menu::create(shootBtn, NULL);
	shoot->setPosition(Vec2(800, 190));
	mouseLayer->addChild(shoot);

	mouse = Sprite::createWithSpriteFrameName("gem-mouse-0.png");
	Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	mouse->runAction(RepeatForever::create(mouseAnimate));
	mouse->setPosition(winw / 2, 0);
	mouseLayer->addChild(mouse, 2);

	this->addChild(stoneLayer, 1);
	this->addChild(mouseLayer, 1);
	return true;
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {

	auto location = touch->getLocation();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float winh = visibleSize.height;
	auto toPos = Vec2((int)location.x, (int)(location.y - winh / 2));
	// 设置奶酪
	cheese = Sprite::create("cheese.png");
	cheese->setPosition(toPos);
	mouseLayer->addChild(cheese);
	auto moveto = MoveTo::create(2.0, toPos);
	auto moveto_ = MoveTo::create(2.0, toPos);
	mouse->runAction(moveto);
	auto fadeout = FadeOut::create(1.0);
	cheese->runAction(Sequence::create(moveto_, fadeout, nullptr));   //这里设置组合动作是为了让老鼠到达奶酪位置，奶酪再消失

	return true;
}

void GameSence::shootMenuCallback(Ref* pSender) {
	Vec2 mousePos = mouse->getPosition();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float winw = visibleSize.width; // 获取屏幕宽度
	float winh = visibleSize.height; // 获取屏幕高度

	// 发射石头
	auto shootstone = Sprite::createWithSpriteFrameName("stone-0.png");
	shootstone->setPosition(stone->getPosition());
	this->addChild(shootstone, 1);
	auto seq = Sequence::create(MoveTo::create(0.8, Vec2(mousePos.x, mousePos.y + winh / 2)), FadeOut::create(0.5), nullptr);
	shootstone->runAction(seq);

	// 老鼠跑开留下钻石
	auto toPos = mousePos;
	auto diamond = Sprite::create("diamond.png");
	diamond->setPosition(toPos);
	mouseLayer->addChild(diamond, 1);
	toPos.x = (int)(CCRANDOM_0_1() * winw);
	toPos.y = (int)(CCRANDOM_0_1() * winh - winh / 2);
	auto mousemoveto = MoveTo::create(2.0, toPos);
	mouse->runAction(mousemoveto);
}
