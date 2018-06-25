#ifndef TIME_TOOL_H
#define TIME_TOOL_H

#include <sys/time.h>

class time_tool {
public:
        struct timeval tvBegin;
        struct timeval tvEnd;
        struct timeval tvDiff;
public:

        void begin() {
                gettimeofday (&tvBegin, NULL);
        }

        void end() {
                gettimeofday (&tvEnd, NULL);
        }

        long int diff() {
                return (tvEnd.tv_usec + 1000000 * tvEnd.tv_sec)
                        - (tvBegin.tv_usec + 1000000 * tvBegin.tv_sec);
        }
};


#endif /* TIME_TOOL_H */
