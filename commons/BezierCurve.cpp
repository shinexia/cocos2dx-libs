#include "BezierCurve.h"

using namespace cocos2d;

//-- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --

BezierCurve::BezierCurve()
	:_factor(nullptr)
	,_paramer(nullptr)
	,_order(-1)
{
}

BezierCurve::~BezierCurve()
{
	if(_factor){
		delete[] _factor;
	}

	if(_paramer){
		delete[] _paramer;
	}
}

BezierCurve* BezierCurve::create()
{
	BezierCurve *pRet = new (std::nothrow)BezierCurve();
	if ( pRet ){
		pRet->autorelease();
		return pRet;
	}
	return nullptr;
}

BezierCurve* BezierCurve::create(const float* paramer, unsigned int lenth)
{
	BezierCurve *pRet = new (std::nothrow)BezierCurve();
	if ( pRet ){
		pRet->setParamer(paramer, lenth);
		pRet->autorelease();
		return pRet;
	}
	return nullptr;
}

BezierCurve* BezierCurve::create(const std::vector<float>& paramer)
{
	BezierCurve *pRet = new (std::nothrow)BezierCurve();
	if ( pRet ){
		pRet->setParamer(paramer);
		pRet->autorelease();
		return pRet;
	}
	return nullptr;
}

BezierCurve* BezierCurve::clone() const
{
	BezierCurve *pRet = new (std::nothrow)BezierCurve();
	if ( pRet ){
		if(_order >= 0){
			pRet->setOrder(_order);
			if( _paramer ){
				pRet->_paramer = new float[_order + 1];
				for(int i=0; i <= _order; i++){
					pRet->_paramer[i] = _paramer[i];
				}
			}
		}
		pRet->autorelease();
		return pRet;
	}
	return nullptr;
}

BezierCurve* BezierCurve::reverse() const
{
	BezierCurve *pRet = new (std::nothrow)BezierCurve();
	if ( pRet ){
		if(_order >= 0){
			pRet->setOrder(_order);
			if( _paramer ){
				pRet->_paramer = new float[_order + 1];
				for(int i=0; i <= _order; i++){
					pRet->_paramer[i] = _paramer[0] + _paramer[_order] - _paramer[_order-i];
				}
			}
		}
		pRet->autorelease();
		return pRet;
	}
	return nullptr;
}

//-- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --

void BezierCurve::setOrder(int order)
{
	if( _order == order){
		return;
	}

	CCASSERT(order >= 0, "order >= 0");
	_order = order;

	int* a = new int[_order + 1];

	a[0] = 1;
	for(int i = 1; i <= _order; i++){
		a[i] = a[i-1] * i;
	}

	if( _factor ){
		delete[] _factor;
	}
	_factor = new int[_order + 1];

	for(int i=0; i <= _order; i++){
		_factor[i] = a[_order] / (a[i] * a[_order - i]);
	}

	delete[] a;
}


void BezierCurve::setParamer(const float* paramer, unsigned int lenth)
{
	setOrder(lenth - 1);

	if(_paramer){
		delete[] _paramer;
	}
	_paramer = new float[_order + 1];

	for(int i=0; i <= _order; i++){
		_paramer[i] = paramer[i];
	}
}

void BezierCurve::setParamer(const std::vector<float>& paramer)
{
	setOrder(paramer.size() - 1);

	if(_paramer){
		delete[] _paramer;
	}
	_paramer = new float[_order + 1];

	for(int i=0; i <= _order; i++){
		_paramer[i] = paramer[i];
	}
}


//-- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --

float BezierCurve::get(float t)
{
	float output = 0;

	for(int i=0; i <= _order; i++){
		output += pow(1-t, _order-i) * pow(t, i) * _factor[i] * _paramer[i];
	}
	return output;
}


BezierFunction BezierCurve::getBezierFunction()
{
	return std::bind(&BezierCurve::get, this, std::placeholders::_1);
}

