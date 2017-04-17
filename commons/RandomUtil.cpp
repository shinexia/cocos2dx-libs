#include "RandomUtil.h"
#include <boost/random.hpp>

namespace RandomUtil{

static boost::mt19937 _s_rng_engine = boost::mt19937( time(0) );

static boost::uniform_01<boost::mt19937> _s_01_distribution(_s_rng_engine);


void cseed(){
	cseed(time(0));
}

void cseed(uint32_t seed){
	_s_rng_engine = boost::mt19937( seed );
	_s_01_distribution = boost::uniform_01<boost::mt19937>( _s_rng_engine );
}

//-- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --

IntType nextInt(const IntType &max_){
	return _s_01_distribution() * max_;
}

IntType nextInt(const IntType &min_, const IntType &max_){
	return min_ + (max_ - min_) * _s_01_distribution();
}

IntType nextTag()
{
	return _s_01_distribution() >= 0.5 ? 1 : -1;
}

//-- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --

RealType nextReal(){
	return _s_01_distribution();
}

RealType nextReal(const RealType &max_){
	return _s_01_distribution() * max_;
}

RealType nextReal(const RealType &min_, const RealType &max_){
	return min_ + (max_ - min_) * _s_01_distribution();
}

}//namespace RandomUtil