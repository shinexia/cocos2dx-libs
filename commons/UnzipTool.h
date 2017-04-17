/**
* created by shine xia <http://www.xgh.io>
*/


#ifndef __UNZIP_TOOL_H__
#define __UNZIP_TOOL_H__


#include "cocos2d.h"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>


class UnzipTool : public boost::enable_shared_from_this<UnzipTool>
{
public:
	typedef boost::shared_ptr<UnzipTool> ptr;
	typedef std::function<void(int, int, std::string)> ProgressCallback;
	typedef std::function<void(bool, std::string)> CompletedCallback;

public:
	UnzipTool()
		:m_progress_callback(nullptr)
		,m_completed_callback(nullptr)
	{}

	UnzipTool(const ProgressCallback& pc, const CompletedCallback& cc = nullptr)
		:m_progress_callback(pc)
		,m_completed_callback(cc)
	{}

	~UnzipTool()
	{

	}

	void setProgressCallback(const ProgressCallback & pc){m_progress_callback = pc;}
	void setCompletedCallback(const CompletedCallback & cc){m_completed_callback = cc;}

	void unzip_async(const std::string &zipPath, const std::string &dirPath, bool remove);
	bool unzip_sync(const std::string &zipPath, const std::string &dirPath, bool remove);

private:
	ProgressCallback  m_progress_callback;
	CompletedCallback m_completed_callback;
};

#endif //__UNZIP_TOOL_H__
