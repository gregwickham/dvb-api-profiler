
/*
 * DVB API Profiler by Greg Wickham is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.
 */
#include    <stdint.h>
#include    <stdio.h>
#include    <string.h>
#include    <linux/dvb/frontend.h>
#include    <sys/ioctl.h>

#include    "profile.h"
#include    "tuner-qpsk.h"

TunerQPSK::TunerQPSK( int index, int delay, int frequency, int sr, fe_sec_voltage_t pol ) : Adapter( index, delay ) {
    _frequency = frequency;
    _sr = sr;
    _pol = pol;
}

int TunerQPSK::tune( void ) {

    int rv;
    
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
#else
    
    struct dtv_properties   dvb_properties;
    struct dtv_property     props[ DTV_IOCTL_MAX_MSGS ];
    
    memset( &props, 0, sizeof( props ));
    dvb_properties.props = props;
    dvb_properties.num = 0; /* just to be sure */

    props[ dvb_properties.num ].cmd = DTV_CLEAR;
    props[ dvb_properties.num ].u.data = _frequency;
    dvb_properties.num ++;

    props[ dvb_properties.num ].cmd = DTV_DELIVERY_SYSTEM;
    props[ dvb_properties.num ].u.data = SYS_DVBS;
    dvb_properties.num ++;

    props[ dvb_properties.num ].cmd = DTV_MODULATION;
    props[ dvb_properties.num ].u.data = QPSK;
    dvb_properties.num ++;

    props[ dvb_properties.num ].cmd = DTV_FREQUENCY;
    props[ dvb_properties.num ].u.data = _frequency;
    dvb_properties.num ++;

    props[ dvb_properties.num ].cmd = DTV_INVERSION;
    props[ dvb_properties.num ].u.data = INVERSION_AUTO;
    dvb_properties.num ++;

    props[ dvb_properties.num ].cmd = DTV_INNER_FEC;
    props[ dvb_properties.num ].u.data = FEC_AUTO;
    dvb_properties.num ++;

    props[ dvb_properties.num ].cmd = DTV_SYMBOL_RATE;
    props[ dvb_properties.num ].u.data = _sr;
    dvb_properties.num ++;

    props[ dvb_properties.num ].cmd = DTV_VOLTAGE;
    props[ dvb_properties.num ].u.data = _pol;
    dvb_properties.num ++;

    props[ dvb_properties.num ].cmd = DTV_TONE;
    props[ dvb_properties.num ].u.data = 0;
    dvb_properties.num ++;
   
    props[ dvb_properties.num ].cmd = DTV_TUNE;
    dvb_properties.num ++;
   
    {
        Profile p(0, "adapter %d frontend( %s ) ioctl( FE_SET_PROPERTY ) [ tune to %d ]", _index, _fe_path, _frequency );
        rv = ioctl( _fe, FE_SET_PROPERTY, &dvb_properties );
    }
#endif

    return( rv );
}

/* END OF FILE */
