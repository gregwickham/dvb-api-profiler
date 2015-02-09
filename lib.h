#ifndef _LIB_H_
#define _LIB_H_

/*
 * DVB API Profiler by Greg Wickham is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.
 */

#include    <stdarg.h>
#include    <sys/time.h>
#include    <malloc.h>
#include    <stdint.h>

char *mprintf( const char *fmt, ... );
char *vmprintf( const char *fmt, va_list ap );
uint32_t subtractTimeVal( struct timeval &end, struct timeval &start );

#define mfree( x ) if ( x ) { free( x ); x = NULL; }

#endif
