#include "LoginScene.h"
#include "cocostudio/CocoStudio.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Global.h"
#include "GameScene.h"
#include <regex>
#include "network/HttpClient.h"
using std::to_string;
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
USING_NS_CC;

using namespace cocostudio::timeline;

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;

Scene* LoginScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoginScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}





// on "init" you need to initialize your instance
bool LoginScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;

    textField = TextField::create("Player Name", "Arial", 30);
    textField->setPosition(Size(visibleWidth / 2, visibleHeight / 4 * 3));
    this->addChild(textField, 2);

    auto button = Button::create();
    button->setTitleText("Login");
    button->setTitleFontSize(30);
    button->setPosition(Size(visibleWidth / 2, visibleHeight / 2));
	button->setTouchEnabled(true);
	button->addTouchEventListener(this, toucheventselector(LoginScene::touchEvent));
    this->addChild(button, 2);

    return true;
}

void LoginScene::touchEvent(Ref *pSender, TouchEventType type) {
	switch (type)
	{
		case TOUCH_EVENT_ENDED:
			if (textField->getString() != "") {
				HttpRequest* request = new HttpRequest();
				request->setUrl("localhost:8080/login");
				request->setRequestType(HttpRequest::Type::POST);
				request->setResponseCallback(CC_CALLBACK_2(LoginScene::onLoginCompleted, this));


				string data = "username=" + textField->getString();
				request->setRequestData(data.c_str(), data.size());
				HttpClient::getInstance()->send(request);
				request->release();
			}
			break;
		default:
			break;
	}
}

void LoginScene::onLoginCompleted(HttpClient *sender, HttpResponse *response) {
	if (!response) {
		return;
	}
	
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	
	string responseData = Global::toString(response->getResponseData());
	string responseHeader = Global::toString(response->getResponseHeader());
	string temp = responseHeader + responseData;
	textField->setText(temp);

	rapidjson::Document d;
	d.Parse<0>(responseData.c_str());
	if (d.HasParseError()) {
		CCLOG("GetParseError %s\n", d.GetParseError());
	}
	if (d.IsObject() && d.HasMember("result") && d["result"].GetBool()) {
		Global::gameSessionId = Global::getSessionIdFromHeader(responseHeader);
		//Director::getInstance()->replaceScene(GameScene::createScene());
	}
}

