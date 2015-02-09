
/*
DVB API Profiler by Greg Wickham is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.
*/

#include    <glibmm.h>
#include    <fcntl.h>
#include    <errno.h>
#include    <string.h>
#include    <sys/ioctl.h>
#include    <linux/dvb/frontend.h>
#include    <linux/dvb/dmx.h>

#include    "lib.h"
#include    "profile.h"
#include    "adapter.h"

Adapter::Adapter( int index, int delay ) {

    _index = index;
    _delay = delay;

    _fe_path = mprintf("/dev/dvb/adapter%d/frontend0", _index );
    _fe = -1;
    _dvr_path = mprintf("/dev/dvb/adapter%d/dvr0", _index );
    _dvr = -1;
    _demux_path = mprintf("/dev/dvb/adapter%d/demux0", _index );
    _demux = -1;

    _start_conn = Glib::signal_timeout().connect_seconds( sigc::mem_fun( *this, &Adapter::start ), _delay );
    printf("adapter %d starting in %d seconds\n", _index, _delay );

}

void Adapter::run( void ) {

}

gboolean Adapter::start( void ) {

    printf("adapter %d starting\n", _index );

    {
        Profile p( 0, "adapter %d frontend( %s ) open", _index, _fe_path );
        _fe = open( _fe_path, O_RDWR | O_NONBLOCK );
    }

    if ( _fe < 0 ) {
        printf("adapter %d frontend( %s ) error: %s\n", _index, _fe_path, strerror( errno ));
        return( false );
    } else {
        printf("adapter %d frontend( %s ) opened with fd %d\n", _index, _fe_path, _fe );

    }

    struct dvb_frontend_info info;
    int rv;
    {
        Profile code(0, "adapter %d frontend( %s ) ioctl( FE_GET_INFO )", _index, _fe_path );
        rv = ioctl( _fe, FE_GET_INFO, &info );
    }

    if ( rv >= 0 ) {
        printf("adapter %d frontend( %s ) name = %s\n", _index, _fe_path, info.name );
    } else {
        printf("adapter %d frontend( %s ) ioctl( FE_GET_INFO ) error: %s\n", _index, _fe_path, strerror( errno ));
    }
    
    struct dtv_properties   dvb_properties;
    struct dtv_property     props[ 20 ];
    memset( &props, 0, sizeof( props ));
    dvb_properties.props = props;
    dvb_properties.num = 0; /* just to be sure */
    
    props[ dvb_properties.num ].cmd = DTV_API_VERSION;
    props[ dvb_properties.num ].u.data = 0;
    dvb_properties.num ++;
    
    {
        Profile p(0, "adapter %d frontend( %s ) ioctl( FE_GET_PROPERTY ) [ DTV_API_VERSION ]", _index, _fe_path );
        rv = ioctl( _fe, FE_GET_PROPERTY, &dvb_properties );
    }

    if ( rv >= 0 ) {
        printf("adapter %d frontend( %s ) DTV_API_VERSION = %d.%d\n", _index, _fe_path, props[ 0 ].u.data >> 8, props[ 0 ].u.data & 0xff );
    } else {
        printf("adapter %d frontend( %s ) ioctl( FE_GET_PROPERTY ) [ DTV_API_VERSION ] error: %s\n", _index, _fe_path, strerror( errno ));
    }

    _fe_conn = Glib::signal_io().connect(
            sigc::mem_fun( *this, &Adapter::eventFe ),
            _fe,
            Glib::IO_IN | Glib::IO_ERR | Glib::IO_HUP );

    /* tune to the frequency */
#if 0
    struct dvb_frontend_parameters param;

    memset( &param, 0, sizeof( param ));

    param.frequency = _frequency;
    param.inversion = INVERSION_AUTO;
    param.u.ofdm.constellation = QAM_AUTO;
    param.u.ofdm.bandwidth = BANDWIDTH_7_MHZ;
    param.u.ofdm.code_rate_HP = FEC_AUTO;
    param.u.ofdm.code_rate_LP = FEC_AUTO;
    param.u.ofdm.transmission_mode = TRANSMISSION_MODE_AUTO;
    param.u.ofdm.guard_interval = GUARD_INTERVAL_AUTO;
    param.u.ofdm.hierarchy_information = HIERARCHY_AUTO;

    {
        Profile code(0, "adapter %d frontend( %s ) ioctl( FE_SET_FRONTEND )", _index, _fe_path );
        rv = ioctl( _fe, FE_SET_FRONTEND, &param );
    }
#endif

    rv = this->tune();

    if ( rv < 0 ) {
        printf("adapter %d frontend( %s ) FE_SET_FRONTEND error: %s\n", _index, _fe_path, strerror( errno ));
        return( false );
    }

    _stop_conn = Glib::signal_timeout().connect_seconds( sigc::mem_fun( *this, &Adapter::stop ), 15 );

    return( false );
}

gboolean Adapter::stop( void ) {
    printf("adapter %d frontend( %s ) stop\n", _index, _fe_path );

    int rv;

    {
        Profile code(0, "adapter %d dvr( %s ) close()", _index, _dvr_path );
        rv = close( _dvr );
        _dvr = -1;
    }
    _dvr_conn.disconnect();

    {
        Profile code(0, "adapter %d demux( %s ) close()", _index, _demux_path );
        rv = close( _demux );
        _demux = -1;
    }

    {
        Profile code(0, "adapter %d frontend( %s ) close()", _index, _fe_path );
        rv = close( _fe );
        _fe = -1;
    }
    _fe_conn.disconnect();

    _start_conn = Glib::signal_timeout().connect_seconds( sigc::mem_fun( *this, &Adapter::start ), _delay );
    printf("adapter %d starting in %d seconds\n", _index, _delay );

    return( false );
}

gboolean Adapter::eventFe( Glib::IOCondition io ) {

    int rv;

    struct dvb_frontend_event event;

    {
        Profile code(0, "adapter %d frontend( %s ) ioctl( FE_GET_EVENT )", _index, _fe_path );
        rv = ioctl( _fe, FE_GET_EVENT, &event );
    }

    if ( rv < 0 ) {
        printf("adapter %d frontend( %s ) FE_GET_EVENT error: %s\n", _index, _fe_path, strerror( errno ));
        return( false );
    }

    printf("adapter %d frontend( %s ) status 0x%02x\n", _index, _fe_path, event.status );

    if (( event.status & FE_HAS_LOCK)&&( _demux < 0 )) {
        this->startDemux();
    }

    return( true );
}

void Adapter::startDemux( void ) {

    {
        Profile p(0,  "adapter %d demux( %s ) open", _index, _demux_path );
        _demux = open( _demux_path, O_RDWR | O_NONBLOCK );
    }

    if ( _demux < 0 ) {
        printf("adapter %d demux( %s ) error: %s\n", _index, _demux_path, strerror( errno ));
        return;
    }

    struct dmx_pes_filter_params pesFilterParams;
    
    memset( &pesFilterParams, 0, sizeof( pesFilterParams ));
    
    pesFilterParams.pid = 8192;
    pesFilterParams.input = DMX_IN_FRONTEND;
    pesFilterParams.output = DMX_OUT_TS_TAP;
    pesFilterParams.pes_type = DMX_PES_OTHER;
    pesFilterParams.flags = DMX_IMMEDIATE_START;
    
    int rv;
    {
        Profile code(0, "adapter %d demux( %s ) ioctl( DMX_SET_PES_FILTER )", _index, _demux_path );
        rv = ::ioctl( _demux, DMX_SET_PES_FILTER, &pesFilterParams );
    }
    
    if ( rv < 0 ) {
        printf("adapter %d demux( %s ) DMX_SET_PES_FILTER error: %s\n", _index, _demux_path, strerror( errno ) );
        return;
    }
    
    {
        Profile p(0, "adapter %d dvr( %s ) open", _index, _dvr_path );
        _dvr = open( _dvr_path, O_RDONLY | O_NONBLOCK );
    }

    if ( _dvr < 0 ) {
        printf("adapter %d dvr( %s ) error: %s\n", _index, _dvr_path, strerror( errno ) );
        return;
    }

    gettimeofday( &_dvr_time, NULL );
    printf("adapter %d dvr( %s ) opened\n", _index, _dvr_path );

    _dvr_conn = Glib::signal_io().connect(
            sigc::mem_fun( *this, &Adapter::readDvr ),
            _dvr,
            Glib::IO_IN | Glib::IO_ERR | Glib::IO_HUP );

}

gboolean Adapter::readDvr( Glib::IOCondition io ) {
    
    struct timeval now;
    gettimeofday( &now, NULL );

    uint8_t buffer[ 65536 * 4 ];
    ssize_t bytes;
    {
        Profile code(100, "adapter %d dvr( %s ) read", _index, _dvr_path );
        bytes = read( _dvr, buffer, sizeof( buffer ));
    }

    uint32_t period = subtractTimeVal( now, _dvr_time );
    _dvr_time = now;

    if ( bytes < 0 ) {
        if ( errno == EOVERFLOW ) {
            printf("adapter %d dvr( %s ) error: kernel DVR buffer overflow (%d usecs since last read)\n", _index, _dvr_path, period  );
        } else {
            printf("adapter %d dvr( %s ) error: %s (%d usecs since last read)\n", _index, _dvr_path, strerror( errno ), period );
        }
    } else if ( bytes == 0 ) {
        printf("adapter %d dvr( %s ) error: end of file (%d usecs since last read)\n", _index, _dvr_path, period );
    } else if ( period >= 75000 ) {
        printf("adapter %d dvr( %s ) read delay: (%d usecs since last read)\n", _index, _dvr_path, period );
    }

    return( true );
}

/* END OF FILE */
