
/*
 * DVB API Profiler by Greg Wickham is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.
 */
#include    <stdio.h>
#include    <stdint.h>

#include    "lib.h"
#include    "profile.h"

Profile::Profile( uint32_t minimum, const char *fmt, ... ) {
    _minimum = minimum;
    va_list ap;
    va_start( ap, fmt );
    _msg = vmprintf( fmt, ap );
    va_end( ap );
    gettimeofday( &_begin, NULL );
}

Profile::~Profile() {
    gettimeofday( &_end, NULL );
    uint32_t usecs = subtractTimeVal( _end, _begin );
    if ( usecs > _minimum ) {
        printf("%s [%d usecs]\n", _msg, usecs );
    }
    mfree( _msg );
}

/* END OF FILE */
