#include "djn_files.h"

#define _DJN_IS_WINDOWS_ defined(_MSC_VER)

#ifdef _DJN_IS_WINDOWS_
#include "Windows.h"
#include "stdlib.h"

#define TICKS_PER_SECOND 10000000
#define EPOCH_DIFFERENCE 11644473600LL

djn_timestamp_t WindowsTickToUnixSeconds(uint64_t windowsTicks)
{
     return (unsigned)(windowsTicks / TICKS_PER_SECOND - EPOCH_DIFFERENCE);
}
#endif

djn_timestamp_t djn_files_last_edit_time(const char* path_to_file)
{
#ifdef _DJN_IS_WINDOWS_
	HANDLE file = CreateFileA(path_to_file,
		GENERIC_READ,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (file == NULL)
		return 0;

	FILETIME lastCreate;
	FILETIME lastWrite;

	BOOL success = GetFileTime(file, 
				&lastCreate,
				NULL,
				&lastWrite);

	CloseHandle(file);

	if (!success)
		return 0;
	
	uint64_t create_time = (((ULONGLONG)lastCreate.dwHighDateTime) << 32) + lastCreate.dwLowDateTime;
	uint64_t time = (((ULONGLONG) lastWrite.dwHighDateTime) << 32) + lastWrite.dwLowDateTime;

	uint64_t last_edit_time = max(create_time, time);

	return WindowsTickToUnixSeconds(last_edit_time);
#endif
	return 0;
}






#undef _DJN_IS_WINDOWS_