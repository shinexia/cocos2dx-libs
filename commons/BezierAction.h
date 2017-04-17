/**
* created by shine xia <http://www.xgh.io>
*
* 由贝赛尔曲线控制的动作
*
*/

#ifndef __BEZIER_ACTION_H__
#define __BEZIER_ACTION_H__


#include "cocos2d.h"
#include "BezierCurve.h"

class BezierAction : public cocos2d::ActionEase
{
private:
	typedef cocos2d::ActionEase Inherited;

public:
	static BezierAction* create(cocos2d::ActionInterval* action, const BezierFunction& bezier);
	static BezierAction* create(cocos2d::ActionInterval* action, BezierCurve* bezier1, BezierCurve* bezier2 = nullptr);
	static BezierAction* create(cocos2d::ActionInterval* action, const float* paramer, unsigned int lenth);

	void  setBezierCurve(BezierCurve* bezier1, BezierCurve* bezier2 = nullptr);
	void  setBezierFunction(const BezierFunction& bezier);

	const BezierFunction& getBezierFunction(){return _bezierFunction;};
	BezierCurve* getBezierCurve1(){return _bezier1;};
	BezierCurve* getBezierCurve2(){return _bezier2;};


	virtual BezierAction* clone() const override;

	/** avoid using reverse() unless you exactly know what you are doing.*/
	virtual BezierAction* reverse() const override;

	virtual void update(float time) override;


CC_CONSTRUCTOR_ACCESS:
	BezierAction();
	~BezierAction();


protected:
	BezierCurve* _bezier1;
	BezierCurve* _bezier2;
	BezierFunction  _bezierFunction;


private:
	CC_DISALLOW_COPY_AND_ASSIGN(BezierAction);

};

#endif //__BEZIER_ACTION_H__
