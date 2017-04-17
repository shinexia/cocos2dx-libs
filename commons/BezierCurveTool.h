/**
* created by shine xia <http://www.xgh.io>
*
* 贝赛尔曲线调试工具
*/
*

#ifndef __BEZIER_CURVE_TOOL_H__
#define __BEZIER_CURVE_TOOL_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "BezierCurve.h"

class BezierCurveTool : public cocos2d::Node
{
private:
	typedef cocos2d::Node Inherited;
	typedef std::function<void(void)>  UpdateGraphFunction;


public:
	static BezierFunction bezierFunction;

	static BezierFunction CreateTriple(Node* , bool increase = true, float range = 1.2f);

	static BezierCurveTool* create(bool increase = true);
	virtual bool init(bool increase);

	BezierCurve* getBezierCurve(){return _bezier;}

	void setDrawFunction(const BezierFunction& func);
	void setIntervalValue(float _min, float _max);

	void updateGraph();

	void registerUpdateGraphFunction(const UpdateGraphFunction& callback){_callback = callback;};
	UpdateGraphFunction getUpdateGraphFunction(){return std::bind(&BezierCurveTool::updateGraph, this);};


CC_CONSTRUCTOR_ACCESS:
	BezierCurveTool();
	~BezierCurveTool();


private:
	void createBezier();
	void createButton();
	void createSlider(int index);
	void removeSlider(int index);

	void valueChanged(cocos2d::Ref *sender, cocos2d::extension::Control::EventType controlEvent);
	void clickSubButton(cocos2d::Ref* sender);
	void clickAddButton(cocos2d::Ref* sender);


protected:
	BezierCurve*  _bezier;
	std::vector<float> _bezierParamer;

	cocos2d::DrawNode*  _drawNode;
	cocos2d::Node*      _sliderNode;

	BezierFunction  _drawFunction;
	UpdateGraphFunction _callback;

	cocos2d::Vec2  _origin;
	cocos2d::Vec2  _destination;

	float _maximumValue;
	float _minimumValue;

	bool  _isIncrease;

};

#endif //__BEZIER_CURVE_TOOL_H__
