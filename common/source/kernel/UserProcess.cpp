#include "ProcessRegistry.h"
#include "UserProcess.h"
#include "kprintf.h"
#include "Thread.h"
#include "Console.h"
#include "Loader.h"
#include "VfsSyscall.h"
#include "File.h"
#include "PageManager.h"
#include "ArchThreads.h"
#include "offsets.h"
#include "Scheduler.h"
#include "UserThread.h"

UserProcess::UserProcess(ustl::string filename, FileSystemInfo *fs_info, uint32 terminal_number) :
    fd_(VfsSyscall::open(filename, O_RDONLY)),
    filename_(filename), fs_info_(fs_info),terminal_number_(terminal_number)
{
  ProcessRegistry::instance()->processStart(); //should also be called if you fork a process

  if (fd_ >= 0)
    loader_ = new Loader(fd_);

  if (!loader_ || !loader_->loadExecutableAndInitProcess())
  {
    debug(USERPROCESS, "Error: loading %s failed!\n", filename.c_str());
    delete this;
    return;
  }

  Thread *user_thread = new UserThread(filename,fs_info,terminal_number, this, NULL, loader_->getEntryFunction(), 0, 0, NULL);
  pid_ = ProcessRegistry::instance()->processCount();
  Scheduler::instance()->addNewThread(user_thread);
  threads_map_.push_back(ustl::make_pair(0,user_thread));

}

UserProcess::~UserProcess()
{
  assert(Scheduler::instance()->isCurrentlyCleaningUp());
  delete loader_;
  loader_ = 0;

  if (fd_ > 0)
    VfsSyscall::close(fd_);

  delete working_dir_;
  working_dir_ = 0;

  ProcessRegistry::instance()->processExit();
}

void UserProcess::Run()
{
  debug(USERPROCESS, "Run: Fail-safe kernel panic - you probably have forgotten to set switch_to_userspace_ = 1\n");
  assert(false);
}

UserThread* UserProcess::createThread(size_t* thread, size_t *attr, void*(*start_routine)(void*), void *wrapper,uint64 argc,size_t args)
{
    debug(USERPROCESS, "New thread creation\n");
    threads_counter_for_id_++;
    *thread = threads_counter_for_id_;
    *attr = NULL;
    process_ = this;
    Thread *new_thread = new UserThread(filename_, fs_info_, terminal_number_, process_, start_routine, wrapper,
                                        threads_counter_for_id_, (void*)argc, args);
    if(new_thread)
    {
        debug(USERPROCESS, "New thread creation successful\n");
    }
    else
    {
        debug(USERPROCESS, "New thread creation ERROR\n");
    }

    Scheduler::instance()->addNewThread(new_thread);
    threads_map_.push_back(ustl::make_pair(threads_counter_for_id_,new_thread));

    return (UserThread*)new_thread;

}