#pragma once

#include "Thread.h"
#include "UserThread.h"
#include "umap.h"

class UserThread;
class UserProcess
{
  public:
    /**
     * Constructor
     * @param minixfs_filename filename of the file in minixfs to execute
     * @param fs_info filesysteminfo-object to be used
     * @param terminal_number the terminal to run in (default 0)
     *
     */
     Loader* loader_;
    UserProcess(ustl::string minixfs_filename, FileSystemInfo *fs_info, uint32 terminal_number = 0);

    virtual ~UserProcess();

    virtual void Run(); // not used
    UserThread *createThread(size_t *thread, size_t *attr, void *(*start_routine)(void *), void *wrapper, uint64 argc, size_t args);

    size_t threads_counter_for_id_ = 0;
    UserProcess* process_;
    ustl::map<size_t, Thread *> threads_map_;


  private:
    int32 fd_;
    FileSystemInfo *working_dir_;
    ustl::string filename_;
    FileSystemInfo *fs_info_;
    uint32 terminal_number_;
    size_t tid_;
    size_t pid_;
};

