#ifndef _DJN_FILES_H_
#define _DJN_FILES_H_

#include "stdint.h"
// File independant library system

typedef uint64_t djn_timestamp_t;

// Returns the epoch time of the last edit on the given file
// Returns 0 if the file couldn't be opened (does not exist or else)
djn_timestamp_t djn_files_last_edit_time(const char* path_to_file);

#endif