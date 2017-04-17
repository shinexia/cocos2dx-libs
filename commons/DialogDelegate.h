/**
* created by shine xia <http://www.xgh.io>
*
* 弹窗通用接口
*
*/

#ifndef __DIALOG_DELEGATE_H__
#define __DIALOG_DELEGATE_H__

#include "cocos2d.h"


class DialogDelegate : public cocos2d::Node
{
private:
	typedef cocos2d::Node Inherited;

public:
	typedef std::function<void(cocos2d::Node*)> DismissCallback;

public:
	virtual void setParentAndzOrder(cocos2d::Node* parent , int zOrder) = 0;
	virtual void resetParentAndzOrder() = 0;

	virtual void show() = 0;
	virtual void dismiss() = 0;

	inline bool isOpen() const{return _isOpen;}

	virtual void onShowActionFinished() = 0;
	virtual void onDismissActionFinished() = 0;

	void setDismissCallback(const DismissCallback& c){_dismissCallback = c;}


CC_CONSTRUCTOR_ACCESS:
	DialogDelegate();

protected:
	int   _d_zOrder;
	bool  _isOpen;

	cocos2d::Node*  _d_parent;

	DismissCallback _dismissCallback;
};

#ifndef CREATE_DIALOG
#define CREATE_DIALOG(__TYPE__)\
static __TYPE__* create(Node* parent , int zOrder) \
{ \
	__TYPE__ *pRet = new (std::nothrow)__TYPE__(); \
	if (pRet){ \
		pRet->_d_parent = parent;\
		pRet->_d_zOrder = zOrder;\
		if(pRet->init()){ \
			pRet->autorelease(); \
			return pRet; \
		} \
	} \
	if(pRet){ \
		delete pRet; \
	} \
	return nullptr; \
}
#endif//CREATE_DIALOG

#endif //
