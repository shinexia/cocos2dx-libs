#include "DialogDelegate.h"

using namespace cocos2d;

DialogDelegate::DialogDelegate()
	:_isOpen(false)
	,_d_zOrder(0)
	,_d_parent(nullptr)
	,_dismissCallback(nullptr)
{
	
}
