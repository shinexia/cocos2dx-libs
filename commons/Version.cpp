#include "Version.h"
#include "GeneralUtils.h"

Version::Version()
	:_value(0)
	,_string("0")
{
}

Version::Version(int value)
	:_value(0)
	,_string("0")
{
	setValue(value);
}

Version::Version(const std::string& str)
	:_value(0)
	,_string("0")
{
	setString(str);
}

Version::Version(int value, const std::string& str)
	:_value(value)
	,_string(str)
{
	//
}

Version::~Version()
{
	
}

void Version::setValue(int value)
{
    _string = parse(value);
    _value = value;
}

void Version::setString(const std::string& str)
{
	_string = str;
	_value = parse(str);
}

int Version::parse(const std::string str)
{
	std::vector<std::string> strings = GeneralUtils::split(str, ".");

	int num[] = {0, 0, 0};
	for(int i=0; i<strings.size() && i<3; i++){
		num[i] = GeneralUtils::toValue<int>(strings[i]);
	}

	return ((num[0]&0x0FFF)<<16)|((num[1]&0xFF)<<8)|(num[2]&0xFF);
}

std::string Version::parse(int value)
{
	int num[3];
    num[0] = (value >> 16) & 0x0FFF;
    num[1] = (value >> 8) & 0xFF;
    num[2] = value & 0xFF;

    std::ostringstream builder;

	if(num[2] == 0){
		builder<<num[0]<<"."<<num[1]; 
	}
	else{
		builder<<num[0]<<"."<<num[1]<<"."<<num[2];
	}

    return builder.str();
}