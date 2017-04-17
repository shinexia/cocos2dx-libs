#include "JsonUtil.h"
#include "json/rapidjson.h"
#include "json/stringbuffer.h"
#include "json/writer.h"


namespace JsonUtil
{

template<typename T>
static std::string toString(const T& value){
	std::ostringstream builder;
	builder << value;
	return builder.str();
}

//- -- to json -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- -

std::string toJson(const bool& value){
	return value ? "true" : "false";
}
std::string toJson(const int& value){
	return toString(value);
}
std::string toJson(const long& value){
	return toString(value);
}
std::string toJson(const double& value){
	return toString(value);
}
std::string toJson(const unsigned int& value){
	return toString(value);
}
std::string toJson(const unsigned long& value){
	return toString(value);
}
std::string toJson(const long long& value){
	return toString(value);
}
std::string toJson(const std::string& str){
	return "\"" + str + "\"";
}

//- -- to json -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- -

bool readJson(bool& t, const rapidjson::Value& doc){
	if(doc.IsBool()){
		t = doc.GetBool();
		return true;
	}
	else if(doc.IsNull()){
		t = false;
		return true;
	}
	else{
		t = false;
		return false;
	}
}
bool readJson(int& t, const rapidjson::Value& doc){
	if(doc.IsInt()){
		t = doc.GetInt();
		return true;
	}
	else if(doc.IsNull()){
		t = 0;
		return true;
	}
	else{
		t = 0;
		return false;
	}
}
bool readJson(long& t, const rapidjson::Value& doc){
	if(doc.IsInt64()){
		t = doc.GetInt64();
		return true;
	}
	else if(doc.IsNull()){
		t = 0;
		return true;
	}
	else{
		t = 0;
		return false;
	}
}
bool readJson(double& t, const rapidjson::Value& doc){
	if(doc.IsDouble()){
		t = doc.GetDouble();
		return true;
	}
	else if(doc.IsNull()){
		t = 0;
		return true;
	}
	else{
		t = 0;
		return false;
	}
}
bool readJson(unsigned int& t, const rapidjson::Value& doc){
	if(doc.IsUint()){
		t = doc.GetUint();
		return true;
	}
	else if(doc.IsNull()){
		t = 0;
		return true;
	}
	else{
		t = 0;
		return false;
	}
}
bool readJson(unsigned long& t, const rapidjson::Value& doc){
	if(doc.IsUint64()){
		t = doc.GetUint64();
		return true;
	}
	else if(doc.IsNull()){
		t = 0;
		return true;
	}
	else{
		t = 0;
		return false;
	}
}
bool readJson(long long& t, const rapidjson::Value& doc){
	if(doc.IsNull()){
		t = 0;
		return true;
	}
	else{
		long long out;
		std::istringstream is(doc.GetString());
		is >> out;
		if (is){
			t = out;
			return true;
		}
		else{
			t = 0;
			return false;
		}
	}
}
bool readJson(std::string& t, const rapidjson::Value& doc){
	if(doc.IsString()){
		t = doc.GetString();
		return true;
	}
	else if(doc.IsNull()){
		t = "";
		return true;
	}
	else{
		t = "";
		return false;
	}
}

}