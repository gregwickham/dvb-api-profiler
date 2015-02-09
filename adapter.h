#ifndef _ADAPTER_H_
#define _ADAPTER_H_

/*
DVB API Profiler by Greg Wickham is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.
*/

#include    <glibmm.h>

class Adapter {

    public:

        Adapter( int index, int delay );

        void run( void );

    protected:

        virtual int tune( void ) = 0;

        gboolean start( void );
        gboolean stop( void );

        gboolean eventFe( Glib::IOCondition io );
        gboolean readDvr( Glib::IOCondition io );
        void startDemux( void );

        int     _index;

        char    *_fe_path;
        int     _fe;
        sigc::connection    _fe_conn;

        char                *_dvr_path;
        int                 _dvr;
        sigc::connection    _dvr_conn;
        struct timeval      _dvr_time;

        char                *_demux_path;
        int                 _demux;

        sigc::connection    _stop_conn;
        sigc::connection    _start_conn;

        int                 _delay;

    private:

};

#endif
