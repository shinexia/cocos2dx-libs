/**
* created by shine xia <http://www.xgh.io>
*
* 屏幕适配
*
*/



#ifndef __SCREEN_ADAPTER_H__
#define __SCREEN_ADAPTER_H__



#include "cocos2d.h"


/**
* get the position with x or y direction adapted.
* return Vec2.
*/
#define sagap(__X__,__Y__)		ScreenAdapter::getPosition((__X__), (__Y__))

#define sagapx(__X__,__Y__)		ScreenAdapter::getPositionX((__X__), (__Y__))
#define sagafx(__X__)			ScreenAdapter::getPositionX((__X__))

#define sagapy(__X__,__Y__)		ScreenAdapter::getPositionY((__X__), (__Y__))
#define sagafy(__Y__)			ScreenAdapter::getPositionY((__Y__))

/**
* get the position with x or y direction scaled.
* return Vec2.
*/
#define sagsp(__X__,__Y__)		ScreenAdapter::getScaledPosition((__X__), (__Y__))

#define sagspx(__X__,__Y__)		ScreenAdapter::getScaledPositionX((__X__), (__Y__))
#define sagsfx(__X__)			ScreenAdapter::getScaledPositionX((__X__))

#define sagspy(__X__,__Y__)		ScreenAdapter::getScaledPositionY((__Y__), (__Y__))
#define sagsfy(__Y__)			ScreenAdapter::getScaledPositionY((__Y__))


namespace ScreenAdapter
{
	namespace detail{
		extern cocos2d::Size s_min;        // the standard supported screen size.
		extern cocos2d::Size s_max;        // the maximum supported screen size.

		extern cocos2d::Size s_win;        // win = Director::getWinSize(), also = Director::getVisibleSize().

		extern cocos2d::Vec2 s_offset;     // the offset from the standard size to the win size.
		extern cocos2d::Vec2 s_scaler;     // the scale ratio from the standard size to the win size.

		extern cocos2d::Rect s_rect;       // origin = offset, size = min.

		extern cocos2d::Vec2 s_center;     // the center of the win size, also the center of the visible size.

		extern cocos2d::Vec2 s_bottom;
		extern cocos2d::Vec2 s_top;
		extern cocos2d::Vec2 s_left;
		extern cocos2d::Vec2 s_right;

		extern cocos2d::Vec2 s_bottomLeft;
		extern cocos2d::Vec2 s_bottomRight;
		extern cocos2d::Vec2 s_topLeft;
		extern cocos2d::Vec2 s_topRight;
	}

	/** init with min = 960 x 640 and max = 1334 x 768.*/
	void InitAdaptingResolution();

	void InitAdaptingResolution(const cocos2d::Size min_size, const cocos2d::Size max_size);


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	inline const cocos2d::Size& min(){return detail::s_min;}

	inline const cocos2d::Size& max(){return detail::s_max;}

	inline const cocos2d::Size& win(){return detail::s_win;}

	inline const cocos2d::Vec2& offset(){return detail::s_offset;}

	inline const cocos2d::Vec2& scaler(){return detail::s_scaler;}

	inline const cocos2d::Rect& rect(){return detail::s_rect;}


	// ----- mapping the standard size to the win size -----

	inline cocos2d::Vec2 getPosition(cocos2d::Vec2 p){
		return detail::s_offset + p;
	}

	inline cocos2d::Vec2 getPosition(float x, float y){
		return cocos2d::Vec2( detail::s_offset.x + x , detail::s_offset.y + y );
	}

	// - - - - - - - - - - - - - - - - -

	inline cocos2d::Vec2 getPositionX(cocos2d::Vec2 p){
		return cocos2d::Vec2( detail::s_offset.x + p.x , p.y );
	}

	inline cocos2d::Vec2 getPositionX(float x, float y){
		return cocos2d::Vec2( detail::s_offset.x + x , y );
	}

	inline float getPositionX(float x){
		return detail::s_offset.x + x;
	}

	// - - - - - - - - - - - - - - - - -

	inline cocos2d::Vec2 getPositionY(cocos2d::Vec2 p){
		return cocos2d::Vec2( p.x , detail::s_offset.y + p.y );
	}

	inline cocos2d::Vec2 getPositionY(float x, float y){
		return cocos2d::Vec2( x , detail::s_offset.y + y );
	}

	inline float getPositionY(float y){
		return detail::s_offset.y + y;
	}

	// - - - - - - - - - - - - - - - - -

	inline cocos2d::Vec2 getScaledPosition(cocos2d::Vec2 p){
		return cocos2d::Vec2( p.x * detail::s_scaler.x , p.y * detail::s_scaler.y );
	}

	inline cocos2d::Vec2 getScaledPosition(float x, float y){
		return cocos2d::Vec2( x * detail::s_scaler.x , y * detail::s_scaler.y );
	}

	// - - - - - - - - - - - - - - - - -

	inline cocos2d::Vec2 getScaledPositionX(cocos2d::Vec2 p){
		return cocos2d::Vec2( p.x * detail::s_scaler.x, p.y );
	}

	inline cocos2d::Vec2 getScaledPositionX(float x, float y){
		return cocos2d::Vec2( x * detail::s_scaler.x, y );
	}

	inline float getScaledPositionX(float x){
		return  x * detail::s_scaler.x ;
	}

	// - - - - - - - - - - - - - - - - -

	inline cocos2d::Vec2 getScaledPositionY(cocos2d::Vec2 p){
		return cocos2d::Vec2( p.x , p.y * detail::s_scaler.y );
	}

	inline cocos2d::Vec2 getScaledPositionY(float x, float y){
		return cocos2d::Vec2( x , y * detail::s_scaler.y );
	}

	inline float getScaledPositionY(float y){
		return  y * detail::s_scaler.y ;
	}


	// ----------------------- relative to the the win size ----------------------------

	inline const cocos2d::Vec2& center(){
		return detail::s_center;
	}

	inline const cocos2d::Vec2& bottom(){
		return detail::s_bottom;
	}

	inline const cocos2d::Vec2& top(){
		return detail::s_top;
	}

	inline const cocos2d::Vec2& left(){
		return detail::s_left;
	}

	inline const cocos2d::Vec2& right(){
		return detail::s_right;
	}

	// - - - - - - - - - - - - - - - - -

	inline const cocos2d::Vec2& bottomLeft(){
		return detail::s_bottomLeft;
	}

	inline const cocos2d::Vec2& bottomRight(){
		return detail::s_bottomRight;
	}

	inline const cocos2d::Vec2& topLeft(){
		return detail::s_topLeft;
	}

	inline const cocos2d::Vec2& topRight(){
		return detail::s_topRight;
	}

	// - - - - - - - - - - - - - - - - -

	inline cocos2d::Vec2 center(float x, float y){
		return cocos2d::Vec2( detail::s_center.x + x , detail::s_center.y + y );
	}

	inline cocos2d::Vec2 bottom(float x, float y){
		return cocos2d::Vec2( detail::s_bottom.x + x , detail::s_bottom.y + y );
	}

	inline cocos2d::Vec2 top(float x, float y){
		return cocos2d::Vec2( detail::s_top.x + x , detail::s_top.y + y );
	}

	inline cocos2d::Vec2 left(float x, float y){
		return cocos2d::Vec2( detail::s_left.x + x , detail::s_left.y + y );
	}

	inline cocos2d::Vec2 right(float x, float y){
		return cocos2d::Vec2( detail::s_right.x + x , detail::s_right.y + y );
	}

	// - - - - - - - - - - - - - - - - -

	inline cocos2d::Vec2 bottomLeft(float x, float y){
		return cocos2d::Vec2( detail::s_bottomLeft.x + x , detail::s_bottomLeft.y + y );
	}

	inline cocos2d::Vec2 bottomRight(float x, float y){
		return cocos2d::Vec2( detail::s_bottomRight.x + x , detail::s_bottomRight.y + y );
	}

	inline cocos2d::Vec2 topLeft(float x, float y){
		return cocos2d::Vec2( detail::s_topLeft.x + x , detail::s_topLeft.y + y );
	}

	inline cocos2d::Vec2 topRight(float x, float y){
		return cocos2d::Vec2( detail::s_topRight.x + x , detail::s_topRight.y + y );
	}
};

#endif //__SCREEN_ADAPTER_H__
