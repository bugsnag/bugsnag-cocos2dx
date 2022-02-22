/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <BugsnagCocos2dx/Bugsnag.hpp>

USING_NS_CC;

using namespace bugsnag;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    /////////////////////////////
    // Bugsnag test code

    menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    topMid = Vec2(visibleSize.width/2 + origin.x, visibleSize.height + origin.y - 20);

    addMenuItem("Close", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    addMenuItem("Unhandled", CC_CALLBACK_1(HelloWorld::unhandledCallback, this));
    addMenuItem("Handled", CC_CALLBACK_1(HelloWorld::handledCallback, this));
    addMenuItem("Metadata (Single)", CC_CALLBACK_1(HelloWorld::addMetadataSingleCallback, this));
    addMenuItem("Metadata (Section)", CC_CALLBACK_1(HelloWorld::addMetadataSectionCallback, this));
    addMenuItem("Leave Breadcrumb", CC_CALLBACK_1(HelloWorld::leaveBreadcrumbCallback, this));
    addMenuItem("Set User", CC_CALLBACK_1(HelloWorld::setUserCallback, this));
    addMenuItem("Start Session", CC_CALLBACK_1(HelloWorld::startSessionCallback, this));
    addMenuItem("Pause Session", CC_CALLBACK_1(HelloWorld::pauseSessionCallback, this));
    addMenuItem("Resume Session", CC_CALLBACK_1(HelloWorld::resumeSessionCallback, this));

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    printf("HelloWorld::menuCloseCallback\n");

    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void HelloWorld::addMenuItem(const string text, const ccMenuCallback& callback) {
    auto mi = MenuItemLabel::create(Label::createWithTTF(text, "fonts/arial.ttf", fontSize), callback);
    mi->setAnchorPoint(Vec2(0.5, 1));
    mi->setPosition(Vec2(topMid.x, topMid.y - menuItemHeight * menuIndex));
    menu->addChild(mi);
    menuIndex++;
}

void HelloWorld::unhandledCallback(cocos2d::Ref* pSender) {
    printf("HelloWorld::unhandledCallback\n");
    *badPointer = 1;
}

void HelloWorld::handledCallback(cocos2d::Ref* pSender) {
    printf("HelloWorld::handledCallback\n");
    Bugsnag::notify("A handled error", "Something broke!");
}

void HelloWorld::addMetadataSingleCallback(cocos2d::Ref* pSender) {
    printf("HelloWorld::addMetadataSingleCallback\n");
    Bugsnag::addMetadata("single", "a section", "a value");
}

void HelloWorld::addMetadataSectionCallback(cocos2d::Ref* pSender) {
    printf("HelloWorld::addMetadataSectionCallback\n");
    Bugsnag::addMetadata("multi", {{"a", "1"}, {"b", "2"}});
}

void HelloWorld::leaveBreadcrumbCallback(cocos2d::Ref* pSender) {
    printf("HelloWorld::leaveBreadcrumbCallback\n");
    Bugsnag::leaveBreadcrumb("Leaving breadcrumb", State, {{"foo", "bar"}});
}

void HelloWorld::setUserCallback(cocos2d::Ref* pSender) {
    printf("HelloWorld::setUserCallback\n");
    Bugsnag::setUser("myuser", "myuser@bugsnag.com", "My User");
}

void HelloWorld::startSessionCallback(cocos2d::Ref* pSender) {
    printf("HelloWorld::startSessionCallback\n");
    Bugsnag::startSession();
}

void HelloWorld::pauseSessionCallback(cocos2d::Ref* pSender) {
    printf("HelloWorld::pauseSessionCallback\n");
    Bugsnag::pauseSession();
}

void HelloWorld::resumeSessionCallback(cocos2d::Ref* pSender) {
    printf("HelloWorld::resumeSessionCallback\n");
    printf("Bugsnag::resumeSession: %d\n", Bugsnag::resumeSession());
}
