/**
* created by shine xia <http://www.xgh.io>
*/

#ifndef __SLIDE_DIALOG_H__
#define __SLIDE_DIALOG_H__

#include "PlaneDialog.h"

class SlideDialog : public PlaneDialog
{
private:
	typedef PlaneDialog Inherited;

public:
	virtual void setSlidePosition(const cocos2d::Vec2& start, const cocos2d::Vec2& destination);

	virtual void show();
	virtual void dismiss();

protected:
	cocos2d::Vec2  _startPosition;
	cocos2d::Vec2  _destiPosition;
};

#endif //__SLIDE_DIALOG_H__
