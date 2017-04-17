/**
* created by shine xia <http://www.xgh.io>
*/

#ifndef __GENERAL_UTILS_H__
#define __GENERAL_UTILS_H__


#include "cocos2d.h"

namespace GeneralUtils
{
	template <class T>
	std::string toString(T num){
		std::ostringstream out;
		out<< num;
		return out.str();
	};

	template <class T>
	T toValue(const std::string &s){
		T out = 0;
		std::istringstream str(s);
		str >> out;
		return out;
	};

	template <class T>
	void swap(T& a, T& b){
		T c = a; a = b; b = c;
	}

	std::vector<std::string> split(std::string str, std::string pattern);

	std::string doubleString(double num);

	std::string generateUUID();

	template <class Type, size_t N>
	size_t getArraySize(Type (&array)[N]) {
	  return N;
	}

	bool newPointIsInPolygon(cocos2d::Vec2 point , std::vector<cocos2d::Vec2> pList);
}
#endif //__GENERAL_UTILS_H__
