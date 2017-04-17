/**
* created by shine xia <http://www.xgh.io>
*
*
*/

#ifndef __PLANE_DIALOG_H__
#define __PLANE_DIALOG_H__


#include "DialogDelegate.h"


class PlaneDialog : public DialogDelegate
{
private:
	typedef DialogDelegate Inherited;

public:
	virtual bool init();

	virtual void setParentAndzOrder(cocos2d::Node* parent , int zOrder);
	virtual void resetParentAndzOrder();

	virtual void setReady(bool value){_ready = value;};
	inline bool isReady()const{return _ready;};

	virtual void setMaskEnabled(bool );
	inline bool isMaskEnabled()const{return _maskEnabled;};

	cocos2d::Rect getRect() const;

	virtual void show();
	virtual void dismiss();

	virtual bool checkNeedShow();
	virtual bool checkNeedDismiss();

	virtual void onShowActionFinished();
	virtual void onDismissActionFinished();


CC_CONSTRUCTOR_ACCESS:
	PlaneDialog();
	~PlaneDialog();

protected:
	float _duration;
	bool  _onShow;
	bool  _onDismiss;
	bool  _ready;

	cocos2d::Node*  _rootNode;

	bool  _maskEnabled;
	int   _maskOpacity;
	cocos2d::LayerColor* _maskLayer;

	bool  _touchRegisted;
	cocos2d::EventListenerTouchOneByOne* _touchListener;
};

#endif //__PLANE_DIALOG_H__
