/**
* created by shine xia <http://www.xgh.io>
*
* 将一个Sprite更改为灰色
*
*/


#ifndef __GRAPHICS_UTIL_H__
#define __GRAPHICS_UTIL_H__


#include "cocos2d.h"

namespace GraphicsUtil
{
	cocos2d::GLProgram* getGrayShader();

	template<class T>
	T* setGray(T* node){
		if(node != nullptr){
			node->setGLProgram(getGrayShader());
		}
		return node;
	};

	template<class T>
	T* resetGLProgram(T* node){
		if(node != nullptr){
			node->setGLProgramState(cocos2d::GLProgramState::getOrCreateWithGLProgramName(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
		}
		return node;
	};
};


#ifndef FT_CREATE_GRAY_BUTTON
#define FT_CREATE_GRAY_BUTTON(__NORMAL__,__SELECTED__,__CALLBACK__) \
	MenuItemSprite::create( \
	Sprite::createWithSpriteFrameName(__NORMAL__), \
	Sprite::createWithSpriteFrameName(__SELECTED__), \
	GraphicsUtil::setGray(Sprite::createWithSpriteFrameName(__NORMAL__)), \
	__CALLBACK__)
#endif //FT_CREATE_GRAY_BUTTON


#endif //__GRAPHICS_UTIL_H__
