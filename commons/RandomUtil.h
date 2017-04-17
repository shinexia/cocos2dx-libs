/**
* created by shine xia <http://www.xgh.io>
*/


#ifndef __RANDOM_UTIL_H__
#define __RANDOM_UTIL_H__

#include "cocos2d.h"


namespace RandomUtil
{
	void cseed();
	void cseed(uint32_t seed);


	typedef int IntType;

	/**	return [0, max).*/
	IntType nextInt(const IntType &max);

	/**	return [min, max).*/
	IntType nextInt(const IntType &min, const IntType &max);

	/**	return +1 or -1.*/
	IntType nextTag();



	typedef double RealType;

	/**	return [0, 1).*/
	RealType nextReal();

	/**	return [0, max).*/
	RealType nextReal(const RealType &max);

	/**	return [min, max).*/
	RealType nextReal(const RealType &min, const RealType &max);


};


#endif //__RANDOM_UTIL_H__
