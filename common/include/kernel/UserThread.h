#include "Thread.h"
#include "ulist.h"
#include "UserProcess.h"
#include "Condition.h"

class UserThread: public Thread{
public:
    //constructor
    UserThread(ustl::string filename,FileSystemInfo *fs_info, uint32 terminal_number, UserProcess *user_process, void *start_routine, void *wrapper,
               size_t tid, uint64 argc, size_t args);

    //copy constructor
    UserThread([[maybe_unused]]const UserThread &process_thread_pointer, UserProcess *parent_process,
               uint32 terminal_number, ustl::string filename, FileSystemInfo *fs_info, size_t thread_id);
    void Run();

private:
    uint32 terminal_number_{};
    UserProcess* user_process_{};
    size_t tid_;


};