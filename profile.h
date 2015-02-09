#ifndef _PROFILE_H_
#define _PROFILE_H_

/*
 * DVB API Profiler by Greg Wickham is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.
 */

#include    <sys/time.h>

class Profile {

    public:

        Profile( uint32_t minimum, const char *fmt, ... );
        ~Profile();

    protected:

    private:

        char            *_msg;

        uint32_t        _minimum;
        struct timeval  _begin;
        struct timeval  _end;

};

#endif
