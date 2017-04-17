#include "BezierCurveTool.h"

using namespace cocos2d;
using namespace cocos2d::extension;

BezierFunction BezierCurveTool::bezierFunction = nullptr;

BezierCurveTool::BezierCurveTool()
	:_bezier(nullptr)
	,_callback(nullptr)
{
}

BezierCurveTool::~BezierCurveTool()
{
	CC_SAFE_RELEASE_NULL(_bezier);
}

BezierFunction BezierCurveTool::CreateTriple(Node* parentNode, bool increase, float range)
{
	BezierCurve *bezier2, *bezier3;

	auto bezierTool1 = BezierCurveTool::create();
	parentNode->addChild(bezierTool1, 1000);
	bezierTool1->setIntervalValue(-range, range);

	auto bezierTool2 = BezierCurveTool::create(increase);// increase or not.
	parentNode->addChild(bezierTool2, 1000);
	bezierTool2->setPositionX(300);
	bezierTool2->setIntervalValue(-range, range);
	bezier2 = bezierTool2->getBezierCurve();

	auto bezierTool3 = BezierCurveTool::create();
	parentNode->addChild(bezierTool3, 1000);
	bezierTool3->setPositionX(600);
	bezierTool2->setIntervalValue(-range, range);
	bezier3 = bezierTool3->getBezierCurve();

	bezierFunction = [=](float t){
		return bezier2->get( bezier3->get(t) );
	};

	bezierTool1->setDrawFunction(bezierFunction);
	bezierTool2->registerUpdateGraphFunction(bezierTool1->getUpdateGraphFunction());
	bezierTool3->registerUpdateGraphFunction(bezierTool2->getUpdateGraphFunction());

	return bezierFunction;
}

BezierCurveTool* BezierCurveTool::create(bool increase)
{
	BezierCurveTool *pRet = new (std::nothrow)BezierCurveTool();
	if ( pRet && pRet->init(increase) ){
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

bool BezierCurveTool::init(bool increase)
{
	bool bRet = false;
	do {
		CC_BREAK_IF(!Inherited::init());
		
		_isIncrease = increase;

		_drawNode = DrawNode::create();
		this->addChild(_drawNode, 10);

		_sliderNode = Node::create();
		this->addChild(_sliderNode, 100);

		Size winSize = Director::getInstance()->getWinSize();
		_origin = Vec2(0, winSize.height - 130);
		_destination = Vec2(250, winSize.height - 80);

		_maximumValue = 3.f;
		_minimumValue = -3.f;

		createBezier();
		createButton();
		updateGraph();

		bRet = true;
	}while(0);
	
	return bRet;
}

void BezierCurveTool::setDrawFunction(const BezierFunction& func)
{
	_drawFunction = func;
	//updateGraph();
}

void BezierCurveTool::setIntervalValue(float _min, float _max)
{
	_minimumValue = _min;
	_maximumValue = _max;

	//for(auto child : _sliderNode->getChildren()){
	//	auto slider = (ControlSlider*)(child);
	//	slider->setMinimumValue(_minimumValue); // Sets the min value of range
	//	slider->setMaximumValue(_maximumValue); // Sets the max value of range
	//}
}

void BezierCurveTool::createBezier()
{
	_bezier = BezierCurve::create();
	_bezier->retain();

	_bezierParamer.push_back( _isIncrease ? 0.f : 1.f);
	_bezierParamer.push_back( _isIncrease ? 1.f : 0.f);
	_bezier->setParamer(_bezierParamer);

	createSlider(0);
	createSlider(1);

	_drawFunction = CC_CALLBACK_1(BezierCurve::get, _bezier);
}

void BezierCurveTool::createButton()
{
	Size winSize = Director::getInstance()->getWinSize();
	if(SpriteFrameCache::getInstance()->getSpriteFrameByName("subbet_up.png") == nullptr){
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game/00SRC/button.plist");
	}
	auto subButton = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("subbet_up.png"),
		Sprite::createWithSpriteFrameName("subbet_down.png"),
		CC_CALLBACK_1(BezierCurveTool::clickSubButton,this)
		);
	subButton->setPosition(Vec2(20, winSize.height - 170));

	auto addButton = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("addbet_up.png"),
		Sprite::createWithSpriteFrameName("addbet_down.png"),
		CC_CALLBACK_1(BezierCurveTool::clickAddButton,this)
		);
	addButton->setPosition(Vec2(230, winSize.height - 170));

	auto menu = Menu::create(subButton, addButton, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 700);
}

void BezierCurveTool::createSlider(int index)
{
	Size winSize = Director::getInstance()->getWinSize();

	ControlSlider *slider = ControlSlider::create("all/sliderTrack.png","all/sliderProgress.png" ,"all/sliderThumb.png");
	slider->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	slider->setMinimumValue(_minimumValue); // Sets the min value of range
	slider->setMaximumValue(_maximumValue); // Sets the max value of range
	slider->setPosition(slider->getContentSize().width / 2.f, winSize.height - 200 - index * 40);
	slider->setTag(index);
	slider->setValue(_bezierParamer[index]);

	_sliderNode->addChild(slider);

	slider->addTargetWithActionForControlEvents(this, cccontrol_selector(BezierCurveTool::valueChanged), Control::EventType::VALUE_CHANGED);

	auto label = Label::createWithTTF(StringUtils::toString(slider->getValue()) ,"fonts/Marker Felt.ttf", 32);
	label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	label->setPosition(Vec2(slider->getContentSize().width / 2.f, slider->getContentSize().height / 2.f + 18));
	label->setColor(Color3B::ORANGE);
	label->setTag(1000);
	slider->addChild(label);
}

void BezierCurveTool::removeSlider(int index)
{
	_sliderNode->removeChildByTag(index);
}

void BezierCurveTool::valueChanged(Ref *sender, Control::EventType controlEvent)
{
	ControlSlider* slider = (ControlSlider*)sender;

	((Label*)slider->getChildByTag(1000))->setString(StringUtils::toString(slider->getValue()));

	_bezierParamer[slider->getTag()] = slider->getValue();
	_bezier->setParamer(_bezierParamer);

	updateGraph();
}

void BezierCurveTool::clickAddButton(Ref* sender)
{
	_bezierParamer.push_back( _isIncrease ? 1.f : 0.f);
	_bezier->setParamer(_bezierParamer);

	createSlider(_bezierParamer.size()-1);

	updateGraph();
}

void BezierCurveTool::clickSubButton(Ref* sender)
{
	if(_bezierParamer.size() <= 1){
		return;
	}

	_bezierParamer.pop_back();
	_bezier->setParamer(_bezierParamer);
	
	removeSlider(_bezierParamer.size());

	updateGraph();

}

void BezierCurveTool::updateGraph()
{
	_drawNode->clear();

	unsigned int segments = _destination.x - _origin.x;
	
	Vec2* vertices = new (std::nothrow) Vec2[segments + 1];
	Vec2* lineVerts = new (std::nothrow) Vec2[segments + 1];

	float t = 0;
	for (unsigned int i = 0; i < segments; i++)
	{
		vertices[i].x = BezierUtil::calc(t, _origin.x, _destination.x);
		vertices[i].y = _origin.y + (_destination.y - _origin.y) * _drawFunction( t );

		lineVerts[i].x = vertices[i].x;
		lineVerts[i].y = _origin.y;

		t += 1.0f / segments;
	}

	vertices[segments].x = _destination.x;
	vertices[segments].y = _origin.y + (_destination.y - _origin.y) * _drawFunction( 1.f );

	lineVerts[segments].x = _destination.x;
	lineVerts[segments].y = _origin.y;

	_drawNode->drawPoly(vertices, segments+1, false, Color4F::GREEN);
	_drawNode->drawPoly(lineVerts, segments+1, false, Color4F::BLUE);


	CC_SAFE_DELETE_ARRAY(vertices);
	CC_SAFE_DELETE_ARRAY(lineVerts);


	if(_callback){
		_callback();
	}
}