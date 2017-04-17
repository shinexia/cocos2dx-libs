#include "UnzipTool.h"
#include "unzip.h"
#include "crypto/md5.h"

using namespace cocos2d;

#define BUFFER_SIZE 8192
#define MAX_FILENAME 512


void UnzipTool::unzip_async(const std::string &zipPath, const std::string &dirPath, bool remove)
{
	std::thread t(std::bind(&UnzipTool::unzip_sync, shared_from_this(), zipPath, dirPath, remove));
	t.detach();
}

bool UnzipTool::unzip_sync(const std::string &zipPath, const std::string &dirPath, bool remove_)
{
	auto fileUtil = FileUtils::getInstance();
	auto scheduler = Director::getInstance()->getScheduler();
	std::string errorInfo("");
	bool hasError = false;
	do{
		// Open the zip file
		unzFile zipfile = unzOpen(zipPath.c_str());
		if(!zipfile)
		{
			hasError = true;
			errorInfo = StringUtils::format("can not open zip file %s", zipPath.c_str());
			break;//global while loop.
		}
    
		// Get info about the zip file
		unz_global_info global_info;
		if(unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
		{
			hasError = true;
			errorInfo = StringUtils::format("can not read file global info of %s", zipPath.c_str());
			unzClose(zipfile);
			break;//global while loop.
		}

		// Buffer to hold data read from the zip file
		char readBuffer[BUFFER_SIZE];
    
		// Loop to extract all files.
		uLong i;
		for (i = 0; i < global_info.number_entry; ++i){
			// Get info about current file.
			unz_file_info fileInfo;
			char fileName[MAX_FILENAME];
			if (unzGetCurrentFileInfo(zipfile, &fileInfo, fileName, MAX_FILENAME, nullptr,  0, nullptr,0) != UNZ_OK)
			{
				hasError = true;
				errorInfo = std::string("can not read file info");
				unzClose(zipfile);
				break;//for loop.
			}
        
			const std::string fullPath = dirPath + fileName;
        
			// Check if this entry is a directory or a file.
			const size_t filenameLength = strlen(fileName);
			if(fileName[filenameLength-1] == '/'){
				// Entry is a directory, so create it.
				// If the directory exists, it will failed silently.
				if (!fileUtil->createDirectory(fullPath.c_str())){
					hasError = true;
					errorInfo = StringUtils::format("can not create directory %s", fullPath.c_str());
					unzClose(zipfile);
					break;//for loop.
				}
			}
			else{
				//There are not directory entry in some case.
				//So we need to test whether the file directory exists when uncompressing file entry
				//, if does not exist then create directory
				const std::string fileNameStr(fileName);
				size_t startIndex = 0;
				size_t index=fileNameStr.find("/",startIndex);
				while(index != std::string::npos)
				{
					const std::string dir=dirPath+fileNameStr.substr(0,index);
					FILE *out = fopen(dir.c_str(), "r");
					if(!out){
						if (!fileUtil->createDirectory(dir.c_str())){
							hasError = true;
							errorInfo = StringUtils::format("can not create directory %s", dir.c_str());
							unzClose(zipfile);
							break;//inner while loop
						}
					}
					else{
						fclose(out);
					}
					startIndex=index+1;
					index=fileNameStr.find("/", startIndex);
				}

				if(hasError) break;//for loop;
            
				// Entry is a file, so extract it.
            
				// Open current file.
				if (unzOpenCurrentFile(zipfile) != UNZ_OK){
					hasError = true;
					errorInfo = StringUtils::format("can not open file %s", fileName);
					unzClose(zipfile);
					break;//for loop
				}
            
				// Create a file to store current file.
				FILE *out = fopen(fullPath.c_str(), "wb");
				if (!out){
					hasError = true;
					errorInfo = StringUtils::format("can not open destination file %s", fullPath.c_str());
					unzCloseCurrentFile(zipfile);
					unzClose(zipfile);
					break;//for loop
				}
            
				// Write current file content to destinate file.
				int code = UNZ_OK;
				do{
					code = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
					if (code < 0)
					{
						hasError = true;
						errorInfo = StringUtils::format("can not read zip file %s, error code is %d", fileName, code);
						unzCloseCurrentFile(zipfile);
						unzClose(zipfile);
						break;//inner while loop
					}
                
					if (code > 0){
						fwrite(readBuffer, code, 1, out);
					}
				} while(code > 0);
            
				fclose(out);

				if(hasError) break;//for loop
			}//end of unzip file.
        
			unzCloseCurrentFile(zipfile);

			if(m_progress_callback){
				//it doesn't matter if this object destructed before the function.
				std::string msg(fileName);
				scheduler->performFunctionInCocosThread(std::bind(
					m_progress_callback, i+1, global_info.number_entry, msg));
			}
        
			// Goto next entry listed in the zip file.
			if ((i+1) < global_info.number_entry)
			{
				if (unzGoToNextFile(zipfile) != UNZ_OK){
					hasError = true;
					errorInfo = std::string("can not read next file");
					unzClose(zipfile);
					break;//for loop
				}
			}
		}//for loop end.

		unzClose(zipfile);
	}while(false);
    

	if(remove_ && remove(zipPath.c_str())){
		errorInfo = StringUtils::format("remove file failed:%s", zipPath.c_str());
	}

	if(m_completed_callback){
		scheduler->performFunctionInCocosThread(std::bind(m_completed_callback, !hasError, errorInfo));
	}

	CCLOG("%s",errorInfo.c_str());
    return !hasError;
};