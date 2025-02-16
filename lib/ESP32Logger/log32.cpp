#include <log32.h>

const char *pathToName(const char *path)
{
    size_t i = 0;
    size_t pos = 0;
    char *p = (char *)path;
    while (*p)
    {
        i++;
        if (*p == '/' || *p == '\\')
        {
            pos = i;
        }
        if (*p == '.')
        {
            *p = '\0';
            break;
        }
        p++;
    }
    return path + pos;
}
