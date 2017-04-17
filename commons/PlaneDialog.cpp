#include "PlaneDialog.h"

using namespace cocos2d;

PlaneDialog::PlaneDialog()
	:_duration(0.4f)
	,_onShow(false)
	,_onDismiss(false)
	,_rootNode(nullptr)
	,_maskEnabled(true)
	,_maskOpacity(180)
	,_maskLayer(nullptr)
	,_ready(false)
	,_touchRegisted(false)
	,_touchListener(nullptr)
{
	
}

PlaneDialog::~PlaneDialog()
{
	CC_SAFE_RELEASE(_touchListener);
	//destruction of cocos2d::Node will clear _eventDispatcher.
}

bool PlaneDialog::init()
{
	bool bRet = false;
	do {
		CC_BREAK_IF(!Inherited::init());
		
		this->setPosition(Director::getInstance()->getWinSize() * 0.5f);
		
		_rootNode = Node::create();
		this->addChild(_rootNode);
		
		_maskLayer = LayerColor::create(Color4B(0, 0, 0, 0));
		_maskLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_maskLayer->ignoreAnchorPointForPosition(false);
		this->addChild(_maskLayer, -100);

		_touchListener = EventListenerTouchOneByOne::create();
		_touchListener->setSwallowTouches(true);
		_touchListener->onTouchBegan = [this](Touch* touch, Event* event){
			return this->isOpen();
		};
		_touchListener->retain();
		
		bRet = true;
	}while(0);
	
	return bRet;
}

void PlaneDialog::setParentAndzOrder(Node* parent , int zOrder)
{
	//used to change parent.
	//if you are not to resume its state immediately, resetParentAndzOrder should be called at before.
	if(_parent){
		if(parent && parent != _parent){
			this->retain();
			this->removeFromParentAndCleanup(false);
			parent->addChild(this, zOrder);
			this->release();
		}
		else if(zOrder != _localZOrder){
			this->setLocalZOrder(zOrder);
		}
	}

	_d_parent = parent;
	_d_zOrder = zOrder;
}

void PlaneDialog::resetParentAndzOrder()
{
	if(_touchRegisted){
		//other event listeners will not be removed, if needed, call:
		//_eventDispatcher->removeEventListenersForTarget(this, /*recursive*/);
		_eventDispatcher->removeEventListener(_touchListener);
		_touchRegisted = false;
	}
	
	this->removeFromParentAndCleanup(true);
	
	_d_parent = nullptr;
	_d_zOrder = 0;
	_isOpen = false;
	_onDismiss = false;
}

void PlaneDialog::setMaskEnabled(bool value)
{
	_maskEnabled = value;
	_maskLayer->setVisible(value);
}

Rect PlaneDialog::getRect() const
{
	return Rect( _position.x - _contentSize.width * 0.5f,
				_position.y - _contentSize.height * 0.5f,
				_contentSize.width, _contentSize.height);
}

void PlaneDialog::show()
{
	if(!checkNeedShow()){
		return;
	}

	//prepare to show

	if(_onDismiss){
		//clear everything include dismiss actions.
		this->cleanup();
		_onDismiss = false;
	}
	else{
		_d_parent->addChild(this, _d_zOrder);
	}

	_isOpen = true;
	_onShow = true;

	if(_touchListener && !_touchRegisted){
		_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
		_touchRegisted = true;
	}

	if(_maskEnabled){
		_maskLayer->stopAllActions();
		_maskLayer->setOpacity(0.f);
		_maskLayer->runAction(FadeTo::create(_duration, _maskOpacity));
	}
}

void PlaneDialog::dismiss()
{
	if(!checkNeedDismiss()){
		return;
	}
	
	//prepare to dismiss, impl in sub classes.
	//1.stop show actions.
	//2.clear others that could interfere dismiss actions.
	
	_onDismiss = true;

	if(_maskLayer && _maskLayer->isVisible()){
		_maskLayer->stopAllActions();
		_maskLayer->runAction(FadeTo::create(_duration, 0.f));
	}
}

bool PlaneDialog::checkNeedShow()
{
	return (!_isOpen || _onDismiss) && _d_parent;
}

bool PlaneDialog::checkNeedDismiss()
{
	return _isOpen && !_onDismiss && _parent;
}

void PlaneDialog::onShowActionFinished()
{
	_onShow = false;
}

void PlaneDialog::onDismissActionFinished()
{
	//the same to resetParentAndzOrder.
	if(_touchRegisted){
		_eventDispatcher->removeEventListener(_touchListener);
		_touchRegisted = false;
	}

	_isOpen = false;

	_onShow = false;
	_onDismiss = false;

	if(_dismissCallback){
		_dismissCallback(this);
	}

	this->removeFromParentAndCleanup(true);
}
