#include "BezierAction.h"

using namespace cocos2d;

BezierAction::BezierAction()
	:_bezier1(nullptr)
	,_bezier2(nullptr)
{
}


BezierAction::~BezierAction()
{
	CC_SAFE_RELEASE(_bezier1);
	CC_SAFE_RELEASE(_bezier2);
}


BezierAction* BezierAction::create(ActionInterval* action, const BezierFunction& bezier)
{
	BezierAction *pRet = new (std::nothrow)BezierAction();
	if (pRet && pRet->initWithAction(action))
	{
		pRet->setBezierFunction(bezier);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}


BezierAction* BezierAction::create(ActionInterval* action, BezierCurve* bezier1, BezierCurve* bezier2)
{
	BezierAction *pRet = new (std::nothrow)BezierAction();
	if (pRet && pRet->initWithAction(action))
	{
		pRet->setBezierCurve(bezier1, bezier2);
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}


BezierAction* BezierAction::create(ActionInterval* action, const float* paramer, unsigned int lenth)
{
	return BezierAction::create(action, BezierCurve::create(paramer, lenth), nullptr);
}


BezierAction* BezierAction::clone() const
{
	if( _bezier1 && _bezier2 ){
		return BezierAction::create(_inner->clone(), _bezier1->clone(), _bezier2->clone());
	}
	else if( _bezier1 ){
		return BezierAction::create(_inner->clone(), _bezier1->clone(), nullptr);
	}
	else{
		return BezierAction::create(_inner->clone(), _bezierFunction);
	}
}


BezierAction* BezierAction::reverse() const
{
	if( _bezier1 && _bezier2 ){
		return BezierAction::create(_inner->reverse(), _bezier1->reverse(), _bezier2->reverse());
	}
	else if( _bezier1 ){
		return BezierAction::create(_inner->reverse(), _bezier1->reverse(), nullptr);
	}
	else{
		return BezierAction::create(_inner->reverse(), _bezierFunction);
	}
}


void BezierAction::setBezierFunction(const BezierFunction& bezier)
{
	_bezierFunction = bezier;
	
	CC_SAFE_RELEASE_NULL(_bezier1);
	CC_SAFE_RELEASE_NULL(_bezier2);
}

void BezierAction::setBezierCurve(BezierCurve* bezier1, BezierCurve* bezier2)
{
	// retain new item before release origin item.
	CC_SAFE_RETAIN(bezier1);
	CC_SAFE_RETAIN(bezier2);

	CC_SAFE_RELEASE(_bezier1);
	CC_SAFE_RELEASE(_bezier2);

	_bezier1 = bezier1;
	_bezier2 = bezier2;

	if( _bezier1 != nullptr && _bezier2 != nullptr ){
		_bezierFunction = [&](float t){
			return _bezier1->get( _bezier2->get(t) );
		};
	}
	else if(_bezier1 != nullptr){
		_bezierFunction = _bezier1->getBezierFunction();
	}
	else if(_bezier2 != nullptr){
		_bezierFunction = _bezier2->getBezierFunction();
	}
	else{
		_bezierFunction = [](float t){return t;};
	}
}

void BezierAction::update(float time)
{
	_inner->update( _bezierFunction(time) );
}