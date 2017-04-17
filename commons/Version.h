/**
* created by shine xia <http://www.xgh.io>
*/


#ifndef __VERSION_H__
#define __VERSION_H__

#include "cocos2d.h"

/*
* 0x0001 FF FF = 1.255.255, 0x0001 01 00 = 1.1, 0x0001 00 00 = 1.0;
*/
class Version
{
public:
	int getValue() const {return _value;};
	std::string getString() const {return _string;};

	void setValue(int value);
	void setString(const std::string& str);


	inline bool const operator == (const Version &other) const{return _value == other._value;}
	inline bool const operator != (const Version &other) const{return _value != other._value;}

	inline bool const operator < (const Version &other) const{return _value < other._value;}
	inline bool const operator > (const Version &other) const{return _value > other._value;}

	inline bool const operator <= (const Version &other) const{return _value <= other._value;}
	inline bool const operator >= (const Version &other) const{return _value >= other._value;}

	inline void operator = (const Version &other){
		_value = other._value;
		_string = other._string;
	}

public:
	Version();
	Version(int value);
	Version(const std::string& str);
	~Version();

	static int parse(const std::string str);
	static std::string parse(int value);

private:
	int _value;
	std::string _string;

	Version(int value, const std::string& description);
};

#endif //__VERSION_H__
