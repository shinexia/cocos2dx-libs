#include "ScreenAdapter.h"

using namespace cocos2d;

namespace ScreenAdapter{

namespace detail{
	cocos2d::Size s_min;
	cocos2d::Size s_max;

	cocos2d::Size s_win;

	cocos2d::Vec2 s_offset;
	cocos2d::Vec2 s_scaler;

	cocos2d::Rect s_rect;

	cocos2d::Vec2 s_center;

	cocos2d::Vec2 s_bottom;
	cocos2d::Vec2 s_top;
	cocos2d::Vec2 s_left;
	cocos2d::Vec2 s_right;

	cocos2d::Vec2 s_bottomLeft;
	cocos2d::Vec2 s_bottomRight;
	cocos2d::Vec2 s_topLeft;
	cocos2d::Vec2 s_topRight;

}//namespace ScreenAdapter::detail

//-- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --

void InitAdaptingResolution()
{
	InitAdaptingResolution(Size(960, 640), Size(1334, 768));
}

void InitAdaptingResolution(const Size min_size, const Size max_size)
{
	detail::s_min = min_size;
	detail::s_max = max_size;

	auto glview = Director::getInstance()->getOpenGLView();

	Size frameSize = glview->getFrameSize();
	if (frameSize.width < frameSize.height)
	{
		glview->setFrameSize(frameSize.height,frameSize.width);
		frameSize = glview->getFrameSize();
	}
	//cocos2d::log("frame size: width:%f, height:%f", frameSize.width, frameSize.height);

	float ratio = frameSize.width * 1.0f / frameSize.height;

	Size tmpWinSize = detail::s_min;
	if( ratio >= detail::s_max.width / detail::s_min.height ){
		tmpWinSize.width  = detail::s_max.width;
		tmpWinSize.height = detail::s_min.height;
	}
	else if( ratio >= detail::s_min.width / detail::s_min.height ){
		tmpWinSize.width  = detail::s_min.height * ratio;
		tmpWinSize.height = detail::s_min.height;
	}
	else if( ratio > detail::s_min.width / detail::s_max.height ){
		tmpWinSize.width  = detail::s_min.width;
		tmpWinSize.height = detail::s_min.width / ratio;
	}
	else{
		tmpWinSize.width  = detail::s_min.width;
		tmpWinSize.height = detail::s_max.height;
	}

	glview->setDesignResolutionSize(tmpWinSize.width, tmpWinSize.height, ResolutionPolicy::EXACT_FIT);

	detail::s_win = Director::getInstance()->getWinSize();	// to save the win size.

	detail::s_offset.x    = (detail::s_win.width  - detail::s_min.width ) / 2.f;
	detail::s_offset.y    = (detail::s_win.height - detail::s_min.height) / 2.f;

	detail::s_rect        = cocos2d::Rect(detail::s_offset.x, detail::s_offset.y, detail::s_min.width , detail::s_min.height);

	detail::s_scaler.x    = detail::s_win.width  / detail::s_min.width;
	detail::s_scaler.y    = detail::s_win.height / detail::s_min.height;

	detail::s_center      = cocos2d::Vec2( detail::s_win.width / 2.f , detail::s_win.height / 2.f );

	detail::s_bottom      = cocos2d::Vec2( detail::s_win.width / 2.f , 0 );
	detail::s_top         = cocos2d::Vec2( detail::s_win.width / 2.f , detail::s_win.height );
	detail::s_left        = cocos2d::Vec2( 0 , detail::s_win.height / 2.f );
	detail::s_right       = cocos2d::Vec2( detail::s_win.width , detail::s_win.height / 2.f );

	detail::s_bottomLeft  = cocos2d::Vec2( 0 , 0 );
	detail::s_bottomRight = cocos2d::Vec2( detail::s_win.width , 0 );
	detail::s_topLeft     = cocos2d::Vec2( 0 , detail::s_win.height );
	detail::s_topRight    = cocos2d::Vec2( detail::s_win.width , detail::s_win.height );
}

}//namespace ScreenAdapter