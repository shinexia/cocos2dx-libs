#include "GraphicsUtil.h"
//#include <boost/shared_ptr.hpp>

using namespace cocos2d;

namespace GraphicsUtil{

//static boost::shared_ptr<GLProgram> s_glProgram = nullptr;

GLProgram* getGrayShader(){
	std::string GrayKey = "FT_GRAY_SHADER";
	GLProgram* gl = GLProgramCache::getInstance()->getGLProgram(GrayKey);
	if(gl == nullptr){
		const GLchar* fragByteArrays =
			"#ifdef GL_ES \n \
			precision lowp float; \n \
			#endif \n \
			varying vec4 v_fragmentColor; \n \
			varying vec2 v_texCoord; \n \
			void main() \n \
			{ \n \
			vec4 v_orColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord); \n \
			float gray = dot(v_orColor.rgb, vec3(0.299, 0.587, 0.114)); \n \
			gl_FragColor = vec4(gray, gray, gray, v_orColor.a); \n \
			}";

		gl = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragByteArrays);

		gl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
		gl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
		gl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);

		GLProgramCache::getInstance()->addGLProgram(gl,GrayKey);
	}
	return gl;
}

Texture2D* getGrayTexture2D(const std::string& filePath){
	std::string fullpath = FileUtils::getInstance()->fullPathForFilename(filePath);

	Image* image = new Image();
	image->initWithImageFile(fullpath);

	auto data = image->getData(); 
	auto lenth = image->getDataLen();

	enum {
		RED = 0, GREEN = 1, BLUE = 2, ALPHA = 3  
	};

	for(long p = 0; p + 3 < lenth; p += 4) {
		uint8_t *rgbaPixel = (uint8_t *)(&data[p]);
		uint32_t gray = 0.3 * rgbaPixel[RED] + 0.59 * rgbaPixel[GREEN] + 0.11 * rgbaPixel[BLUE];  
		rgbaPixel[RED] = gray;
		rgbaPixel[GREEN] = gray;
		rgbaPixel[BLUE] = gray;
	}

	Texture2D* texture = new Texture2D();
	if( texture && texture->initWithImage(image)){
		texture->autorelease();
	}
	else if(texture){
		texture->release();
		texture = nullptr;
	}

	image->release();

	return texture;
}

}//namespace GraphicsUtil