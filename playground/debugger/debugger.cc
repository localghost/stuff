#include <iostream>
#include <functional>
#include <stdexcept>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>

unsigned long bpoint = 0x4007d4;
unsigned long rpoint = 0x4007b8;
long orig_data = 0;
bool injected = false;
struct user_regs_struct regs;

struct wait_loop
{
    void run()
    {
        int status{};
        pid_t child{};
        do {
            child = wait(&status);
            if (WIFSTOPPED(status))
            {
                std::cout << "Child has stopped due to signal: "
                          << WSTOPSIG(status) << std::endl;
                if (WSTOPSIG(status) == SIGTRAP)
                {
                    if (!injected)
                    {
                        long data = ptrace(PTRACE_PEEKTEXT, child, (void*)bpoint, NULL);
                        orig_data = data;
                        data = (data & ~0xff) | 0xcc;
                        ptrace(PTRACE_POKETEXT, child, (void*)bpoint, data);
                        injected = true;
                    }
                    else
                    {
                        ptrace(PTRACE_POKETEXT, child, (void*)bpoint, orig_data);
                        ptrace(PTRACE_GETREGS, child, NULL, &regs);
                        regs.rip = rpoint;
                        ptrace(PTRACE_SETREGS, child, NULL, &regs);
                    }
                    ptrace(PTRACE_CONT, child, NULL, NULL);
                }
            }
            else if (WIFSIGNALED(status))
            {
                std::cout << "Child " << child << " received signal: "
                          << WTERMSIG(status) << std::endl;
            }
        } while (!WIFEXITED(status));
    }
};

namespace process {
void fork(std::function<void()> init)
{
    pid_t pid = ::fork();
    if (pid < 0)
        throw std::runtime_error("fork() failed");

    if (pid == 0)
        init();
}

bool traceme()
{
    return ptrace(PTRACE_TRACEME, 0, NULL, NULL) != -1;
}
}

class debugger
{
public:
    void trace(pid_t pid) { }

    void trace(const std::string& path)
    {
        process::fork([&path]()
        {
            if (process::traceme())
                execl(path.c_str(), path.c_str());
            _exit(0);
        });
        wait_loop{}.run();
    }
};

int main()
{
    debugger d;
    d.trace("debuggiee");
}
