/**
* created by shine xia <http://www.xgh.io>
*
* Json序列化和反序列化辅助函数
*
*/

#ifndef __JSON_UTIL_H__
#define __JSON_UTIL_H__

#include "cocos2d.h"
#include "json/document.h"

namespace JsonUtil
{

//- -- to json -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- -

std::string toJson(const bool& value);
std::string toJson(const int& value);
std::string toJson(const long& value);
std::string toJson(const double& value);
std::string toJson(const unsigned int& value);
std::string toJson(const unsigned long& value);
std::string toJson(const long long& value);
std::string toJson(const std::string& str);

template<typename T>
std::string toJson(T& proto)
{
	return proto.write();
}

template<typename T>
std::string toJson(const T& proto)
{
	return proto.write();
}

template<typename T>
std::string toJson(const std::vector<T>& list)
{
	std::ostringstream builder;
	builder<<"[";
	for(unsigned int i=0; i<list.size(); i++){
		if(i > 0){
			builder<<",";
		}
		builder<<toJson(list[i]);
	}
	builder<<"]";
	return builder.str();
}

//- -- read json -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- -

//right type and null return true.
bool readJson(bool& t, const rapidjson::Value& doc);
bool readJson(int& t, const rapidjson::Value& doc);
bool readJson(long& t, const rapidjson::Value& doc);
bool readJson(double& t, const rapidjson::Value& doc);
bool readJson(unsigned int& t, const rapidjson::Value& doc);
bool readJson(unsigned long& t, const rapidjson::Value& doc);
bool readJson(long long& t, const rapidjson::Value& doc);
bool readJson(std::string& t, const rapidjson::Value& doc);

template<typename T>
bool readJson(T& proto, const rapidjson::Value& doc){
	if(doc.IsObject()){
		return proto.read(doc);
	}
	else if(doc.IsNull()){
		//unassigned object.
		return true;
	}
	return false;
}

template<typename T>
bool readJson(std::vector<T>& dest, const rapidjson::Value& doc){
	dest.clear();
	if(doc.IsArray() && !doc.IsNull()){
		for(unsigned int i=0; i<doc.Capacity(); i++){
			T t;
			if(readJson(t, doc[i])){
				dest.push_back(t);
			}
		}
		return true;
	}
	else if(doc.IsNull()){
		//unassigned array.
		return true;
	}
	return false;
}

//- -- json pojo factory -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- -

template<typename T>
bool read(T& t, const char* json){
	rapidjson::Document doc;
	doc.Parse<0>(json);
	if (!doc.HasParseError()){
		return readJson(t, doc);
	}
	return false;
}

};//namespace JsonUtil

#endif //
