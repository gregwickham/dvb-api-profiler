
/*
 * DVB API Profiler by Greg Wickham is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.
 */
#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>

#include    "lib.h"

char *vmprintf( const char *fmt, va_list ap ) {
    va_list aq;
    va_copy( aq, ap );
    int len = ::vsnprintf( NULL, 0, fmt, ap ) + 1;
    char *text = (char*)malloc( sizeof( char ) * len );
    ::vsnprintf( text, len, fmt, aq );
    return( text );
}

char *mprintf( const char *fmt, ... ) {
    va_list ap;
    va_start( ap, fmt );
    char *text = vmprintf( fmt, ap );
    va_end( ap );
    return( text );
}

uint32_t subtractTimeVal( struct timeval &end, struct timeval &start ) {
    uint32_t usecs = 0;
    if ( end.tv_usec >= start.tv_usec ) {
        usecs = end.tv_usec - start.tv_usec;
        usecs += 1000000 * ( end.tv_sec - start.tv_sec );
    } else {
        usecs = ( 1000000 + end.tv_usec ) - start.tv_usec;
        usecs += 1000000 * (( end.tv_sec - 1 ) - start.tv_sec );
    }
   
    return( usecs );
}

/* END OF FILE */
