#include "GeneralUtils.h"
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/erase.hpp>


using namespace cocos2d;

namespace GeneralUtils{

std::string doubleString(double num){
	double number = num < 0 ? -num : num;
	number += 0.5;
	auto integer = static_cast<long long>(number);
	std::ostringstream builder;
	if(num < 0 && (integer != 0)){
		builder << '-';
	}
	builder << integer;
	return builder.str();
}


std::string generateUUID()
{
	auto origin_uuid = boost::uuids::random_generator()();
	auto uuid = boost::lexical_cast<std::string>(origin_uuid);
	boost::erase_all(uuid, "-");
	return uuid;
}

std::vector<std::string> split(std::string str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern; //extend
    int size=str.size();
    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

bool newPointIsInPolygon( Vec2 point , std::vector<Vec2> pList )
{
	int pSize = pList.size();
	std::vector<float> plx;
	std::vector<float> ply;
	for(int i = 0 ; i < pSize ; i++){
		Vec2 po = pList.at(i);
		plx.push_back(po.x);
		ply.push_back(po.y);
	}
	int i;
	int j = pSize-1 ;
	bool oddNodes=false;

	for (i=0;i<pSize; i++) {
		if(((ply.at(i)< point.y && ply.at(j)>=point.y)
			||   (ply.at(j)<point.y && ply.at(i)>=point.y))
			&&  (plx.at(i)<=point.x || plx.at(j)<=point.x)){
				if(plx.at(i)+(point.y- ply.at(i))/( ply.at(j)- ply.at(i))*(plx.at(j)-plx.at(i))<point.x) {
					oddNodes=!oddNodes;
				}
		}
		j=i;
	}
	return oddNodes;
}

}//namespace GeneralUtils