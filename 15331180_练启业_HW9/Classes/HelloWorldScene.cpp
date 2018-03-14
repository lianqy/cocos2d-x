#include "HelloWorldScene.h"
#include "tinyxml2/tinyxml2.h"
#include "string"

using namespace tinyxml2;
using namespace std;

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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF(parseXML("./data.xml"), "fonts/set.ttf", 30);
	auto label1 = Label::createWithTTF("15331180", "fonts/font.ttf", 34);
	//auto label = CCLabelTTF::createWithFontDefinition

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

	label1->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label1->getContentSize().height - 30));

    // add the label as a child to this layer
    this->addChild(label, 1);
	this->addChild(label1, 2);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("black.jpg");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

string HelloWorld::parseXML(const char *fileName)
{
	
	//std::string filePath = FileUtils::getInstance()->getWritablePath() + fileName;
	string filePath = fileName;
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	XMLError errorId = pDoc->LoadFile(filePath.c_str());
	string name;
	string error = "open xmlfile failed";

	if (errorId != 0) {
		//xml格式错误
		return error;
	}

	XMLElement *rootEle = pDoc->RootElement();

	//获取第一个节点属性
	const XMLAttribute *attribute = rootEle->FirstAttribute();
	//打印节点属性名和值
	//log("attribute_name = %s,attribute_value = %s", attribute->Name(), attribute->Value());

	//XMLElement *dicEle = rootEle->FirstChildElement("dic");
	XMLElement *keyEle = rootEle->FirstChildElement("key");
	/*if (keyEle) {
		//log("keyEle Text= %s", keyEle->GetText());
		name = keyEle->GetText();
	}*/

	//XMLElement *arrayEle = keyEle->NextSiblingElement();
	XMLElement *childEle = keyEle->NextSiblingElement();
	//childEle = childEle->NextSiblingElement() ->NextSiblingElement();
	//while (childEle) {
		//log("childEle Text= %s", childEle->GetText());
		//childEle = childEle->NextSiblingElement();
	//}
	if (childEle) {
		name = childEle->GetText();
	}

	delete pDoc;
	return name;

}