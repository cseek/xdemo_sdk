#include <string>
#include <stdio.h>
#include <sys/wait.h>

namespace Cmd
{
    int execute(const std::string &command)
    {
        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe)
        {
            return -1;
        }
        // 读取并丢弃所有输出，防止缓冲区溢出导致 SIGPIPE
        char buffer[512];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
            ;
        auto status = pclose(pipe);
        if (-1 == status)
        {
            return -2;
        }
        if (!WIFEXITED(status))
        {
            return -3;
        }
        return WEXITSTATUS(status); // 命令运行结束的状态值 0~255
    }

    int execute(const std::string &command, std::string &output)
    {
        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe)
        {
            return -1;
        }

        char buffer[512];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
        {
            output.append(buffer);
        }

        auto status = pclose(pipe);
        if (-1 == status)
        {
            return -2;
        }
        if (!WIFEXITED(status))
        {
            return -3;
        }
        return WEXITSTATUS(status); // 命令运行结束的状态值 0~255
    }
}