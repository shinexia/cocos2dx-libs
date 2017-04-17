#include "SlideDialog.h"


using namespace cocos2d;

void SlideDialog::setSlidePosition(const cocos2d::Vec2& start, const cocos2d::Vec2& destination)
{
	_startPosition = start;
	_destiPosition = destination;
}

void SlideDialog::show()
{
	if(!checkNeedShow()){
		return;
	}
	Inherited::show();

	_rootNode->stopAllActions();
	_rootNode->setPosition(_startPosition);
	_rootNode->runAction(Sequence::createWithTwoActions(
		EaseSineOut::create(MoveTo::create(_duration, _destiPosition)),
		CallFunc::create( CC_CALLBACK_0(SlideDialog::onShowActionFinished, this))
		));
}

void SlideDialog::dismiss()
{
	if(!checkNeedDismiss()){
		return;
	}
	Inherited::dismiss();

	_rootNode->stopAllActions();
	_rootNode->runAction(Sequence::createWithTwoActions(
		EaseSineOut::create(MoveTo::create(_duration * 0.75f, _startPosition)),
		CallFunc::create( CC_CALLBACK_0(SlideDialog::onDismissActionFinished, this))
		));
}
