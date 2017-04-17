#include "ScaleDialog.h"
#include "BezierAction.h"

using namespace cocos2d;

void ScaleDialog::show()
{
	if(!checkNeedShow()){
		return;
	}
	Inherited::show();

	_rootNode->stopAllActions();
	_rootNode->setScale(0.0f);
	_rootNode->runAction(Sequence::createWithTwoActions(
		BezierAction::create(ScaleTo::create(_duration, 1.0f ),[](float t){
			return BezierUtil::calc<float>(t, 0.f, 1.34395f, 1.1835f, 1.0557f, 1.f, 1.f);
		}),
		CallFunc::create(std::bind(&ScaleDialog::onShowActionFinished, this))
		));
}

void ScaleDialog::dismiss()
{
	if(!checkNeedDismiss()){
		return;
	}
	Inherited::dismiss();

	_rootNode->stopAllActions();
	_rootNode->runAction(Sequence::createWithTwoActions(
		BezierAction::create(ScaleTo::create(_duration * 0.75f, 0.0f), [](float t){
			return BezierUtil::calc<float>(BezierUtil::calc<float>(t, 0, 0.56538f, 1.f), 0.f, -0.141613f, -0.126358f, 1.f);
		}),
		CallFunc::create(std::bind(&ScaleDialog::onDismissActionFinished, this))
		));
}