#include "GameScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

USING_NS_CC;

cocos2d::Scene* GameScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameScene::init() {
    if (!Layer::init())
    {
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;

    score_field = TextField::create("Score", "Arial", 30);
    score_field->setPosition(Size(visibleWidth / 4, visibleHeight / 4 * 3));
    this->addChild(score_field, 2);

    submit_button = Button::create();
    submit_button->setTitleText("Submit");
    submit_button->setTitleFontSize(30);
    submit_button->setPosition(Size(visibleWidth / 4, visibleHeight / 4));
	submit_button->setTouchEnabled(true);
	submit_button->addTouchEventListener(this, toucheventselector(GameScene::submit_touchEvent));
    this->addChild(submit_button, 2);

    rank_field = TextField::create("", "Arial", 30);
    rank_field->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4 * 3));
    this->addChild(rank_field, 2);

    rank_button = Button::create();
    rank_button->setTitleText("Rank");
    rank_button->setTitleFontSize(30);
    rank_button->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4));
    this->addChild(rank_button, 2);

    return true;
}

void GameScene::submit_touchEvent(Ref *pSender, TouchEventType type) {
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		bool check_number;
		check_number = true;
		if (score_field->getString().length() > 0) {
			for (int i = 0; i < score_field->getString().length(); i++) {
				if (!isdigit(score_field->getString()[i])) {
					check_number = false;
				}
			}
			if (check_number) {
				HttpRequest *request = new HttpRequest();
				request->setUrl("localhost:8080/submit");
				request->setRequestType(HttpRequest::Type::POST);
				request->setResponseCallback(CC_CALLBACK_2(GameScene::submit_onGameCompleted, this));

				string score = "score=" + score_field->getString();
				request->setRequestData(score.c_str(), score.size());

				vector<string> headers;
				headers.push_back("Cookie:GAMESESSIONID=" + Global::gameSessionId);
				request->setHeaders(headers);
				cocos2d::network::HttpClient::getInstance()->send(request);
				request->release();
			}
		}
		break;
	default:
		break;
	}
}

void GameScene::submit_onGameCompleted(HttpClient *sender, HttpResponse *response) {
	if (!response) {
		return;
	}

	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	
}
