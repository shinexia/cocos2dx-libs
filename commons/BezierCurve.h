/**
* created by shine xia <http://www.xgh.io>
*
* 贝赛尔曲线
*
*/

#ifndef __BEZIER_CURVE_H__
#define __BEZIER_CURVE_H__

#include "cocos2d.h"

typedef std::function<float(float)> BezierFunction;


class BezierCurve : public cocos2d::Ref, public cocos2d::Clonable
{
public:
	static BezierCurve* create();
	static BezierCurve* create(const float* paramer, unsigned int lenth);
	static BezierCurve* create(const std::vector<float>& paramer);


	virtual BezierCurve* clone() const override;

	/** rotate bezier curve. */
	virtual BezierCurve* reverse() const;


	void  setOrder(int order);
	void  setParamer(const float* paramer, unsigned int lenth);
	void  setParamer(const std::vector<float>& paramer);


	BezierFunction getBezierFunction();


	/** t = [0, 1] */
	float get(float t);


	template<typename T>
	T calc(float t, const T* paramer){
		T output = 0;
		for(int i=0; i <= _order; i++){
			output += pow(1-t, _order-i) * pow(t, i) * _factor[i] * paramer[i];
		}
		return output;
	}

	template<typename T>
	T calc(float t, const std::vector<T>& paramer){
		T output = 0;
		for(int i=0; i <= _order; i++){
			output += pow(1-t, _order-i) * pow(t, i) * _factor[i] * paramer[i];
		}
		return output;
	}


public:
	BezierCurve();
	~BezierCurve();


private:
	int     _order;
	int*    _factor;
	float*  _paramer;
};


namespace BezierUtil{
	template<typename T>
	inline T calc(float t, T p0, T p1)
	{
		return ((1-t)*p0 + t*p1);
	}

	template<typename T>
	inline T calc(float t, T p0, T p1, T p2)
	{
		return ((1-t)*(1-t)*p0 + 2*t*(1-t)*p1 + t*t*p2);
	}

	template<typename T>
	T calc(float t, T p0, T p1, T p2, T p3)
	{
		return (powf(1-t,3)*p0 + 3*t*(powf(1-t,2))*p1 + 3*powf(t,2)*(1-t)*p2 + powf(t,3)*p3);
	}

	template<typename T>
	T calc(float t, T p0, T p1, T p2, T p3, T p4)
	{
		return (powf(1-t,4)*p0 + 4*(powf(1-t,3))*t*p1 + 6*powf(1-t,2)*powf(t,2)*p2 + 4*(1-t)*powf(t,3)*p3 + powf(t,4)*p4);
	}

	template<typename T>
	T calc(float t, T p0, T p1, T p2, T p3, T p4, T p5)
	{
		return (powf(1-t,5)*p0 + 5*(powf(1-t,4))*t*p1 + 10*powf(1-t,3)*powf(t,2)*p2 + 10*powf(1-t,2)*powf(t,3)*p3 + 5*(1-t)*powf(t,4)*p4 + powf(t,5)*p5);
	}
}


#endif //__BEZIER_CURVE_H__
