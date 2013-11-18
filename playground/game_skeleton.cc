#include <poll.h>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

void strip_new_line(char* str)
{
    char* s = strchr(str, '\n');
    if (s)
        memset(s, '\0', 1);
}

unsigned get_time()
{
    timeval t;
    gettimeofday(&t, NULL);
    return static_cast<unsigned>(t.tv_sec * 1000 + 
            (static_cast<double>(t.tv_usec) / 1000.0));
}

int main()
{
    termios old_term;
    tcgetattr(fileno(stdin), &old_term);

    termios new_term = old_term;
    new_term.c_lflag &= ~(ICANON | ECHO);
    new_term.c_cc[VMIN] = 1;
    tcsetattr(fileno(stdin), TCSANOW, &new_term);
    
    unsigned last_time = get_time();
    pollfd fds[1] = {};
    fds[0].fd = fileno(stdin);
    fds[0].events = POLLIN;
    int timeout = 1000; // in mili

    while (true)
    {
        int ret = poll(fds, 1, timeout);

        unsigned current_time = get_time();

        if (0 == ret)
        {
            printf("timeout passed\n");
            timeout = 1000;
            last_time = current_time;
        }
        else if (0 < ret)
        {
            printf("read something\n");
            if (fds[0].revents & POLLIN)
            {
                char buffer[1024] = {};
                ssize_t readBytes = -1;
                int c = fgetc(stdin);
                printf("character: %d\n", c);
                if (c == 'q')
                {
                    printf("restoring terminal...\n");
                    tcsetattr(fileno(stdin), TCSANOW, &old_term);
                    printf("quitting...\n");
                    break;
                }
            }

            timeout -= (current_time - last_time);
            if (timeout <= 0)
            {
                printf("tick\n");
                timeout = 1000;
                last_time = current_time;
            }
        }
        else
        {
            printf("error: %s\n", strerror(errno));
        }
    }
}
