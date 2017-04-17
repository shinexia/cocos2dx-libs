/**
* created by shine xia <http://www.xgh.io>
*
* 将日志写入文件中
*/

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>

namespace logx
{
	void default_global_init(const std::string& folder);
	void default_global_purge();

	void debug(const char* format, ...);
	void info( const char* format, ...);
	void warn( const char* format, ...);
	void error(const char* format, ...);
};

#endif //__LOGGER_H__
