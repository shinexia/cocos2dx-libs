#include "Logger.h"

#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include "boost/log/attributes/named_scope.hpp"
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/record_ordering.hpp>


#ifdef WIN32
#define LOGX_WIN32
#include <windows.h>
#else
#define LOGX_ANDROID
#include <android/log.h>
#endif

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

using namespace logging::trivial;

namespace logx
{
	BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(g_logger, src::severity_logger_mt<severity_level>);
	static boost::mutex s_mutex;

	void default_global_init(const std::string& folder)
	{
		boost::mutex::scoped_lock (s_mutex);

		typedef sinks::text_file_backend text_backend;
		typedef sinks::synchronous_sink<text_backend> text_sink;
		logging::formatter formatter=
			expr::stream
			<<"["<<expr::attr<unsigned int>("LineID")<<"]"
			<<"["<<expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")<<"]"
			//<<"["<<expr::attr<boost::thread::id>("ThreadID")<<"]"
			//<<"["<<expr::format_named_scope("Scope", keywords::format = "%n")<<"]"
			<<"["<<expr::attr<severity_level>("Severity")<<"]"
			<<":"<<expr::smessage;

		uintmax_t rotation_size = 1 * 1024 * 1024;

		auto collector = sinks::file::make_collector(
			keywords::target = folder,
			keywords::max_size = 25 * 1024 * 1024,
			keywords::min_free_space = 1 * 1024 * 1024
			);

		// init sink1
		boost::shared_ptr<sinks::text_file_backend> backend1 = boost::make_shared<sinks::text_file_backend>(
		keywords::file_name = folder + "%Y-%m-%d_Errors.log",
		keywords::open_mode = std::ios::app);
		backend1->set_rotation_size(rotation_size);
		//backend1->set_file_collector(collector);
		backend1->auto_flush(true);

		boost::shared_ptr<text_sink> sink1(new text_sink(backend1));

		sink1->set_filter(expr::attr<severity_level>("Severity") >= warning);
		logging::core::get()->add_sink(sink1);

		// init sink2
		boost::shared_ptr<sinks::text_file_backend> backend2 = boost::make_shared<sinks::text_file_backend>(
		keywords::file_name = folder + "%Y-%m-%d_Debugs.log", //"%Y-%m-%d_%H-%M-%S_Debug.log"
		keywords::open_mode = std::ios::app);
		backend2->set_rotation_size(rotation_size);
		//backend2->set_file_collector(collector);
		backend2->auto_flush(true);

		boost::shared_ptr<text_sink> sink2(new text_sink( backend2 ));

		sink2->set_filter(expr::attr<severity_level>("Severity") < warning);

		logging::core::get()->add_sink(sink2);

		sink1->set_formatter(formatter);
		sink2->set_formatter(formatter);

		//logging::add_console_log();
		logging::add_common_attributes();
		//logging::core::get()->add_thread_attribute("Scope", attrs::named_scope());
	}

	void default_global_purge(){
		boost::mutex::scoped_lock (s_mutex);

		boost::log::core::get()->remove_all_sinks();
	}

	#define BUFFER_SIZE 4096

	void debug(const char* format, ...){
		boost::mutex::scoped_lock (s_mutex);

		char buf[BUFFER_SIZE], *p = buf;
		va_list args;
		va_start(args, format);
		p += vsnprintf(p, sizeof buf - 3, format, args);
		va_end(args);
		while ( p > buf && isspace(p[-1]) ) *--p = '\0';

		BOOST_LOG_SEV(g_logger::get(), severity_level::debug) << buf;

#ifdef LOGX_WIN32
		*p++ = '\r';
        *p++ = '\n';
        *p   = '\0';
		OutputDebugStringA(buf);
#else
#ifdef LOGX_ANDROID
		*p++ = '\n';
		*p   = '\0';
		__android_log_print(ANDROID_LOG_DEBUG, "logx <debug>", "%s", buf);
#endif
#endif
	}

	void info(const char* format, ...){
		boost::mutex::scoped_lock (s_mutex);

		char buf[BUFFER_SIZE], *p = buf;
		va_list args;
		va_start(args, format);
		p += vsnprintf(p, sizeof buf - 3, format, args);
		va_end(args);
		while ( p > buf && isspace(p[-1]) ) *--p = '\0';

		BOOST_LOG_SEV(g_logger::get(), severity_level::info) << buf;

#ifdef LOGX_WIN32
		*p++ = '\r';
		*p++ = '\n';
		*p   = '\0';
		OutputDebugStringA(buf);
#else
#ifdef LOGX_ANDROID
		*p++ = '\n';
		*p   = '\0';
		__android_log_print(ANDROID_LOG_INFO, "logx <info>", "%s", buf);
#endif
#endif
	}

	void warn(const char* format, ...){
		boost::mutex::scoped_lock (s_mutex);

		char buf[BUFFER_SIZE], *p = buf;
		va_list args;
		va_start(args, format);
		p += vsnprintf(p, sizeof buf - 3, format, args);
		va_end(args);
		while ( p > buf && isspace(p[-1]) ) *--p = '\0';

		BOOST_LOG_SEV(g_logger::get(), severity_level::warning) << buf;

#ifdef LOGX_WIN32
		*p++ = '\r';
        *p++ = '\n';
        *p   = '\0';
		OutputDebugStringA(buf);
#else
#ifdef LOGX_ANDROID
		*p++ = '\n';
		*p   = '\0';
		__android_log_print(ANDROID_LOG_WARN, "logx <warn>", "%s", buf);
#endif
#endif
	}

	void error(const char* format, ...){
		boost::mutex::scoped_lock (s_mutex);

		char buf[BUFFER_SIZE], *p = buf;
		va_list args;
		va_start(args, format);
		p += vsnprintf(p, sizeof buf - 3, format, args);
		va_end(args);
		while ( p > buf && isspace(p[-1]) ) *--p = '\0';

		BOOST_LOG_SEV(g_logger::get(), severity_level::error) << buf;

#ifdef LOGX_WIN32
		*p++ = '\r';
        *p++ = '\n';
        *p   = '\0';
		OutputDebugStringA(buf);
#else
#ifdef LOGX_ANDROID
		*p++ = '\n';
		*p   = '\0';
		__android_log_print(ANDROID_LOG_ERROR, "logx <error>", "%s", buf);
#endif
#endif
	}
};
