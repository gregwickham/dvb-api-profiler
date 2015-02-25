
/*
 * DVB API Profiler by Greg Wickham is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.
 */
#include    <stdint.h>
#include    <stdio.h>
#include    <string.h>
#include    <linux/dvb/frontend.h>
#include    <sys/ioctl.h>

#include    "profile.h"
#include    "tuner-ofdm.h"

TunerOFDM::TunerOFDM( int index, int delay, int frequency ) : Adapter( index, delay ) {
    _frequency = frequency;
}

int TunerOFDM::tune( void ) {

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

    props[ dvb_properties.num ].cmd = DTV_FREQUENCY;
    props[ dvb_properties.num ].u.data = _frequency;
    dvb_properties.num ++;
    props[ dvb_properties.num ].cmd = DTV_INVERSION;
    props[ dvb_properties.num ].u.data = INVERSION_AUTO;
    dvb_properties.num ++;
    props[ dvb_properties.num ].cmd = DTV_MODULATION;
    props[ dvb_properties.num ].u.data = QAM_AUTO;
    dvb_properties.num ++;
    props[ dvb_properties.num ].cmd = DTV_BANDWIDTH_HZ;
    props[ dvb_properties.num ].u.data = BANDWIDTH_7_MHZ;
    dvb_properties.num ++;
    props[ dvb_properties.num ].cmd = DTV_CODE_RATE_HP;
    props[ dvb_properties.num ].u.data = FEC_AUTO;
    dvb_properties.num ++;
    props[ dvb_properties.num ].cmd = DTV_CODE_RATE_LP;
    props[ dvb_properties.num ].u.data = FEC_AUTO;
    dvb_properties.num ++;
    props[ dvb_properties.num ].cmd = DTV_GUARD_INTERVAL;
    props[ dvb_properties.num ].u.data = GUARD_INTERVAL_AUTO;
    dvb_properties.num ++;
    props[ dvb_properties.num ].cmd = DTV_TRANSMISSION_MODE;
    props[ dvb_properties.num ].u.data = TRANSMISSION_MODE_AUTO;
    dvb_properties.num ++;
    props[ dvb_properties.num ].cmd = DTV_HIERARCHY;
    props[ dvb_properties.num ].u.data = HIERARCHY_AUTO;
    dvb_properties.num ++;
   
    {
        Profile p(0, "adapter %d frontend( %s ) ioctl( FE_SET_PROPERTY ) [ tune to %d ]", _index, _fe_path, _frequency );
        rv = ioctl( _fe, FE_SET_PROPERTY, &dvb_properties );
    }
#endif

    return( rv );
}

/* END OF FILE */
