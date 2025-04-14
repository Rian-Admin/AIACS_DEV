#ifndef _DECSDKERRNO_H_
#define _DECSDKERRNO_H_

/**
 * 说明：错误码定义文件，该文件将所有错误码合并，包括uniplay错误码、mediaswitch错误码、decsdk内部错误码
 * 1.uniplay错误码范围1~19999
 * 2.mediaswitch错误码范围20001~29999
 * 3.decsdk内部错误码范围30001~39999
 * 4.kdmfileinterface错误码范围40001~40999
 */

#define DEC_OK									0         ///< 操作成功

/**
 * uniplay错误码定义
 * 错误码范围1~19999
 */

#define	ERR_DEC_UNIPLAY_PARA_OVER						             1
#define ERR_DEC_UNIPLAY_ORDER_ERROR						             2
#define	ERR_DEC_UNIPLAY_TIMER_ERROR						             3
#define	ERR_DEC_UNIPLAY_ALLOC_MEMORY_ERROR				             6
#define ERR_DEC_UNIPLAY_BUF_OVER						             11

#define ERR_DEC_UNIPLAY_DEC_VIDEO_ERROR					             4
#define ERR_DEC_UNIPLAY_DEC_AUDIO_ERROR					             5

#define	ERR_DEC_UNIPLAY_SET_VOLUME_ERROR				             7
#define ERR_DEC_UNIPLAY_CREATE_OBJ_ERROR				             8
#define ERR_DEC_UNIPLAY_CREATE_DDRAW_ERROR				             9
#define ERR_DEC_UNIPLAY_CREATE_OFFSCREEN_ERROR			             10

#define ERR_DEC_UNIPLAY_CREATE_SOUND_ERROR				             12	
#define ERR_DEC_UNIPLAY_OPEN_FILE_ERROR					             13
#define ERR_DEC_UNIPLAY_SUPPORT_FILE_ONLY				             14
#define ERR_DEC_UNIPLAY_SUPPORT_STREAM_ONLY				             15
#define ERR_DEC_UNIPLAY_SYS_NOT_SUPPORT					             16
#define ERR_DEC_UNIPLAY_FILEHEADER_UNKNOWN				             17
#define ERR_DEC_UNIPLAY_VERSION_INCORRECT				             18 
#define ERR_DEC_UNIPLAY_INIT_DECODER_ERROR				             19
#define ERR_DEC_UNIPLAY_CHECK_FILE_ERROR				             20
#define ERR_DEC_UNIPLAY_INIT_TIMER_ERROR				             21
#define	ERR_DEC_UNIPLAY_BLT_ERROR						             22
#define ERR_DEC_UNIPLAY_UPDATE_ERROR					             23
#define ERR_DEC_UNIPLAY_OPEN_FILE_ERROR_MULTI			             24
#define ERR_DEC_UNIPLAY_OPEN_FILE_ERROR_VIDEO			             25
#define ERR_DEC_UNIPLAY_JPEG_COMPRESS_ERROR				             26
#define ERR_DEC_UNIPLAY_EXTRACT_NOT_SUPPORT				             27
#define ERR_DEC_UNIPLAY_EXTRACT_DATA_ERROR				             28
#define ERR_DEC_UNIPLAY_SECRET_KEY_ERROR				             29
#define ERR_DEC_UNIPLAY_DECODE_KEYFRAME_ERROR			             30
#define ERR_DEC_UNIPLAY_NEED_MORE_DATA					             31
#define ERR_DEC_UNIPLAY_INVALID_PORT					             32
#define ERR_DEC_UNIPLAY_NOT_FIND						             33
#define	ERR_DEC_UNIPLAY_OPERTION_NOTALLOWED				             34
#define	ERR_DEC_UNIPLAY_FILE_WRITE_ERR					             35
#define	ERR_DEC_UNIPLAY_PARA_POINT_NULL					             36
#define	ERR_DEC_UNIPLAY_NO_DECODE						             37
#define	ERR_DEC_UNIPLAY_NO_STREAM						             38 
#define	ERR_DEC_UNIPLAY_ALREADY_EXIST					             39
#define ERR_DEC_UNIPLAY_BUF_EMPTY                                    40
#define ERR_DEC_UNIPLAY_WAITFOR_KEYFRAME                             41
#define ERR_DEC_UNIPLAY_DEVICE_LOST		                             42
#define ERR_DEC_UNIPLAY_SURFACE_LOST		                         43
#define ERR_DEC_UNIPLAY_LOADDLL_FAIL		                         44
#define	ERR_DEC_UNIPLAY_OUTOF_VIDEO_MEMORY				             45
#define	ERR_DEC_UNIPLAY_WINDOW_SIZE_ERROR				             46
#define	ERR_DEC_UNIPLAY_ERROR_MEDIA_TYPE				             47
#define ERR_DEC_UNIPLAY_LOCK_ERR                                     48
#define ERR_DEC_UNIPLAY_INVALID_DEVICE                               49
#define ERR_DEC_UNIPLAY_UNDEFINED_BEHAVIOR                           50
#define ERR_DEC_UNIPLAY_DEVICE_FAILED                                51
#define ERR_DEC_UNIPLAY_DEVICE_RESET                                 52
#define ERR_DEC_UNIPLAY_ERR_FRAME_TIME                               53
#define ERR_DEC_UNIPLAY_ERR_VERSION_CMP                              54
#define ERR_DEC_UNIPLAY_ERR_FONT_PATH                                61
#define ERR_DEC_UNIPLAY_ERR_STRUCT_SAME                              62
#define ERR_DEC_UNIPLAY_ERR_FUNCFAIL		                         63
#define ERR_DEC_UNIPLAY_ERR_FONT_SIZE		                         64
#define ERR_DEC_UNIPLAY_ERR_DRAWHDC		                             65
#define ERR_DEC_UNIPLAY_FAIL_UNKNOWN					             99

#define ERR_DEC_UNIPLAY_KDVD_GENERAL_ERROR                           1024
#define ERR_DEC_UNIPLAY_KDVD_PORT                                    1025
#define ERR_DEC_UNIPLAY_KDVD_PARAM                                   ERR_DEC_UNIPLAY_PARA_OVER
#define ERR_DEC_UNIPLAY_KDVD_MALLOC                                  ERR_DEC_UNIPLAY_ALLOC_MEMORY_ERROR
#define ERR_DEC_UNIPLAY_KDVD_CREATE_THREAD                           1028
#define ERR_DEC_UNIPLAY_KDVD_CREATE_DECODER                          1029
#define ERR_DEC_UNIPLAY_KDVD_DEL_DECODER                             1030
#define ERR_DEC_UNIPLAY_KDVD_CREATE_TIMER                            1031
#define ERR_DEC_UNIPLAY_KDVD_IN_BUF_FULL                             ERR_DEC_UNIPLAY_BUF_OVER
#define ERR_DEC_UNIPLAY_KDVD_IN_BUF_EMPTY                            1033
#define ERR_DEC_UNIPLAY_KDVD_FRAME_SIZE                              1034
#define ERR_DEC_UNIPLAY_KDVD_MODULE_UNINITED                         1035
#define ERR_DEC_UNIPLAY_KDVD_DECODE_BUFFER_FULL                      1036
#define ERR_DEC_UNIPLAY_MFX_GPU_MEDIA_TYPE		                     1051
#define ERR_DEC_UNIPLAY_MFX_GPU_NOT_INIT			                 1052
#define ERR_DEC_UNIPLAY_MFX_GPU_WND_INVALID		                     1053
#define ERR_DEC_UNIPLAY_MFX_GPU_RECT_OUTREGION	                     1054

#define ERR_DEC_UNIPLAY_KDVD_BUF_SIZE                                1100
#define ERR_DEC_UNIPLAY_KDVD_ERROR                                   1101
#define ERR_DEC_UNIPLAY_KDVD_CODEC_BUSY                              1102
#define ERR_DEC_UNIPLAY_KDVD_CODEC_EMPTY                             1103
#define ERR_DEC_UNIPLAY_KDVD_CODEC_WAIT                              1104
#define ERR_DEC_UNIPLAY_KDVD_LOOPBUF_EMPTY                           1105
#define ERR_DEC_UNIPLAY_KDVD_CODEC_NOT_SURPPORT                      1106
#define ERR_DEC_UNIPLAY_KDVD_MALLOC_CODEC                            1107
#define ERR_DEC_UNIPLAY_KDVD_NO_DISPLAY                              1108
#define ERR_DEC_UNIPLAY_KDVD_PERMISSION_DENIED                       1110
#define ERR_DEC_UNIPLAY_KDVD_CODEC_RETRY                             1111
#define ERR_DEC_UNIPLAY_KDVD_KEYFRAME_RETRY                          1112
#define ERR_DEC_UNIPLAY_KDVD_MODULE_INITED                           1113

#define ERR_DEC_UNIPLAY_KDAD_ERROR                                   3001
#define ERR_DEC_UNIPLAY_KDAD_PARAM                                   3002
#define ERR_DEC_UNIPLAY_KDAD_NO_ID                                   3003
#define ERR_DEC_UNIPLAY_KDAD_MALLOC                                  3004
#define ERR_DEC_UNIPLAY_KDAD_CREATE_THREAD                           3005
#define ERR_DEC_UNIPLAY_KDAD_CREATE_DECODER                          3006
#define ERR_DEC_UNIPLAY_KDAD_DEL_DECODER                             3007
#define ERR_DEC_UNIPLAY_KDAD_DECODE_AFRAME                           3008
#define ERR_DEC_UNIPLAY_KDAD_IN_BUF_FULL                             3009
#define ERR_DEC_UNIPLAY_KDAD_IN_BUF_EMPTY                            3010
#define ERR_DEC_UNIPLAY_KDAD_FRAME_SIZE                              ERR_DEC_UNIPLAY_KDVD_FRAME_SIZE
#define ERR_DEC_UNIPLAY_KDAD_MODULE_UNINITED                         3012
#define ERR_DEC_UNIPLAY_KDAD_MODULE_INITED                           3013

#define ERR_DEC_UNIPLAY_KDVP_ERROR                                   2201
#define ERR_DEC_UNIPLAY_KDVP_MALLOC_MEM                              ERR_DEC_UNIPLAY_ALLOC_MEMORY_ERROR
#define ERR_DEC_UNIPLAY_KDVP_FUNC_NOT_SUPPORT                        ERR_DEC_UNIPLAY_OPERTION_NOTALLOWED
#define ERR_DEC_UNIPLAY_KDVP_MODULE_INITED                           2204
#define ERR_DEC_UNIPLAY_KDVP_MODULE_UNINITED                         2205
#define ERR_DEC_UNIPLAY_KDVP_PLYERID_INVALID                         2206
#define ERR_DEC_UNIPLAY_KDVP_OSDID_INVALID                           2207
#define ERR_DEC_UNIPLAY_KDVP_SHOWRECTID_INVALID                      2208
#define ERR_DEC_UNIPLAY_KDVP_PARAM                                   ERR_DEC_UNIPLAY_PARA_OVER
#define ERR_DEC_UNIPLAY_KDVP_SHOWRECT_CREATED                        2001
#define ERR_DEC_UNIPLAY_KDVP_SHOWRECT_UNCREATED                      2002
#define ERR_DEC_UNIPLAY_KDVP_GRAPHCARD_CAPS                          2003
#define ERR_DEC_UNIPLAY_KDVP_CREATE_TEXTURE                          2004
#define ERR_DEC_UNIPLAY_KDVP_COMPILE_SHADER                          2005  
#define ERR_DEC_UNIPLAY_KDVP_LINK_SHADER                             2006
#define ERR_DEC_UNIPLAY_KDVP_FRAGMENT_LOAD                           2007
#define ERR_DEC_UNIPLAY_KDVP_CREATE_SCREEN                           2008
#define ERR_DEC_UNIPLAY_KDVP_NO_MORE_VIDPLYER_NUM                    2009
#define ERR_DEC_UNIPLAY_KDVP_PLYER_PAUSED                            2010
#define ERR_DEC_UNIPLAY_KDVP_CLRMODE_SETTED                          2011
#define ERR_DEC_UNIPLAY_KDVP_SHOWRECT_DNOT_DRAW                      2012
#define ERR_DEC_UNIPLAY_KDVP_NO_MORE_OSD_NUM                         2013
#define ERR_DEC_UNIPLAY_KDVP_OSD_SET_HIDE                            2014
#define ERR_DEC_UNIPLAY_KDVP_OPEN_DISPLAY                            2015
#define ERR_DEC_UNIPLAY_KDVP_CHOOSE_VISUAL                           2016
#define ERR_DEC_UNIPLAY_KDVP_CREATE_CONTEXT                          2017
#define ERR_DEC_UNIPLAY_KDVP_MAKE_CURRENT_RC                         2018
#define ERR_DEC_UNIPLAY_KDVP_WND_STYLE                               2019
#define ERR_DEC_UNIPLAY_KDVP_CHOOSE_FORMAT                           2020
#define ERR_DEC_UNIPLAY_KDVP_SET_FORMAT                              2021
#define ERR_DEC_UNIPLAY_KDVP_SET_RENDERTYPE                          2022
#define ERR_DEC_UNIPLAY_KDVP_FOUND_VPWND_OF_VIDPLYER                 2023
#define ERR_DEC_UNIPLAY_KDVP_HAVE_NO_OSDLAYER_OF_VPWND               2024
#define ERR_DEC_UNIPLAY_KDVP_THIS_THREAD_CREATED_PLYER               2025
#define ERR_DEC_UNIPLAY_KDVP_NO_MORE_SHOWRECT_NUM                    2026
#define ERR_DEC_UNIPLAY_KDVP_NOT_RIGHT_THREAD_ID                     2027
#define ERR_DEC_UNIPLAY_KDVP_GET_SWSCONTEXT                          2028
#define ERR_DEC_UNIPLAY_KDVP_NO_MORE_WND                             2029
#define ERR_DEC_UNIPLAY_KDVP_MAKE_CURRENT                            2030
#define ERR_DEC_UNIPLAY_KDVP_CREATE_PROGRAM                          2031
#define ERR_DEC_UNIPLAY_KDVP_CREATE_SHADER                           2032
#define ERR_DEC_UNIPLAY_KDVP_ATTACH_SHADER                           2033
#define ERR_DEC_UNIPLAY_KDVP_GET_PROGRAM_SHADER                      2034
#define ERR_DEC_UNIPLAY_KDVP_SHOWRECT_DEATCH_WND                     2035
#define ERR_DEC_UNIPLAY_KDVP_CREATE_OPENGL_OBJ                       2036
#define ERR_DEC_UNIPLAY_KDVP_GET_XWINDOW_ATTRI                       2037
#define ERR_DEC_UNIPLAY_KDVP_WINDOW_SIZE                             ERR_DEC_UNIPLAY_WINDOW_SIZE_ERROR
#define ERR_DEC_UNIPLAY_KDVP_SHOULD_DEL_PLYER                        2039
#define ERR_DEC_UNIPLAY_KDVP_CREATE_VPWND                            2040
#define ERR_DEC_UNIPLAY_KDVP_OPENGL_FUN_NOT_SUPPORT                  2041
#define ERR_DEC_UNIPLAY_KDVP_VP_CREATED                              2042
#define ERR_DEC_UNIPLAY_KDVP_EARLY_INIT                              2100
#define ERR_DEC_UNIPLAY_KDVP_ATTACH_THREAD_TO_JVM                    2101
#define ERR_DEC_UNIPLAY_KDVP_FOUND_VPGLSURFACEVIEW                   2102
#define ERR_DEC_UNIPLAY_KDVP_REG_JNI                                 2103
#define ERR_DEC_UNIPLAY_KDVP_DIDNOT_RENDER                           2104
#define ERR_DEC_UNIPLAY_KDVP_DDRAW_EXCLUSIVEMODEALREADYSET           2142
#define ERR_DEC_UNIPLAY_KDVP_SURFACE                                 2143

#define ERR_DEC_UNIPLAY_KDAC_ERROR                                   4001
#define ERR_DEC_UNIPLAY_KDAC_OPEN_AC                                 4002
#define ERR_DEC_UNIPLAY_KDAC_CREATED                                 4003
#define ERR_DEC_UNIPLAY_KDAC_DEVID_INVALID                           4004
#define ERR_DEC_UNIPLAY_KDAC_DEV_EXISTED                             4005
#define ERR_DEC_UNIPLAY_KDAC_MALLOC                                  4006
#define ERR_DEC_UNIPLAY_KDAC_NO_DEV                                  4007
#define ERR_DEC_UNIPLAY_KDAC_AUD_CAP_OTHER_ERR                       4008
#define ERR_DEC_UNIPLAY_KDAC_AUDDEV_PARAM                            4009
#define ERR_DEC_UNIPLAY_KDAC_PARAM                                   4010
#define ERR_DEC_UNIPLAY_KDAC_CREATE_DEV                              4011   
#define ERR_DEC_UNIPLAY_KDAC_CREATE_THREAD                           4012
#define ERR_DEC_UNIPLAY_KDAC_MODULE_UNINITED                         4013
#define ERR_DEC_UNIPLAY_KDAC_MODULE_INITED                           4014
#define ERR_DEC_UNIPLAY_KDAC_NEED_RESET				                 4015
#define ERR_DEC_UNIPLAY_KDAC_WARN_CAP_RENDER_FAILED                  4100
#define ERR_DEC_UNIPLAY_KDAC_WARN_CAP_NOSURPPORT                     4101
#define ERR_DEC_UNIPLAY_KDAC_OUTBUF_EMPTY                            4102

#define ERR_DEC_UNIPLAY_KDAP_ERROR                                   5001
#define ERR_DEC_UNIPLAY_KDAP_PARAM                                   5002
#define ERR_DEC_UNIPLAY_KDAP_MODULE_UNINITED                         5003
#define ERR_DEC_UNIPLAY_KDAP_STREAMID_INVALID                        5004
#define ERR_DEC_UNIPLAY_KDAP_NO_STREAM                               5005
#define ERR_DEC_UNIPLAY_KDAP_STREAM_FULL                             5006
#define ERR_DEC_UNIPLAY_KDAP_MALLOC_FAIL                             ERR_DEC_UNIPLAY_ALLOC_MEMORY_ERROR
#define ERR_DEC_UNIPLAY_KDAP_CREATE_DEV                              5008
#define ERR_DEC_UNIPLAY_KDAP_OPEN_SND                                5009
#define ERR_DEC_UNIPLAY_KDAP_BUF_FULL                                5010
#define ERR_DEC_UNIPLAY_KDAP_IO_RESET                                5011
#define ERR_DEC_UNIPLAY_KDAP_AP_CODEC_CONTROL                        5012 
#define ERR_DEC_UNIPLAY_KDAP_FRAME_BUF_EMPTY                         5013
#define ERR_DEC_UNIPLAY_KDAP_CREATE_THREAD                           5014
#define ERR_DEC_UNIPLAY_KDAP_AUDDEV_PARAM                            5015
#define ERR_DEC_UNIPLAY_KDAP_OPEN_MIXER                              5016
#define ERR_DEC_UNIPLAY_KDAP_ATTACH_MIXER                            5017
#define ERR_DEC_UNIPLAY_KDAP_LOAD_MIXER                              5018
#define ERR_DEC_UNIPLAY_KDAP_REGISTER_MIXER                          5019
#define ERR_DEC_UNIPLAY_KDAP_FOUND_MASTER                            5020
#define ERR_DEC_UNIPLAY_KDAP_STREAM_MUTEX                            5021
#define ERR_DEC_UNIPLAY_KDAP_FRAMELEN                                5022
#define ERR_DEC_UNIPLAY_KDAP_MODULE_INITED                           5023
#define ERR_DEC_UNIPLAY_KDAP_STOP_SOUND                              5024

#define ERR_DEC_UNIPLAY_KDAE_ERROR                                   6001
#define ERR_DEC_UNIPLAY_KDAE_PARAM                                   6002
#define ERR_DEC_UNIPLAY_KDAE_NO_ID                                   6003
#define ERR_DEC_UNIPLAY_KDAE_MALLOC                                  6004
#define ERR_DEC_UNIPLAY_KDAE_CREATE_THREAD                           6005
#define ERR_DEC_UNIPLAY_KDAE_CREATE_DECODER                          6006
#define ERR_DEC_UNIPLAY_KDAE_DEL_DECODER                             6007
#define ERR_DEC_UNIPLAY_KDAE_DECODE_AFRAME                           6008
#define ERR_DEC_UNIPLAY_KDAE_IN_BUF_FULL                             6009
#define ERR_DEC_UNIPLAY_KDAE_IN_BUF_EMPTY                            6010
#define ERR_DEC_UNIPLAY_KDAE_FRAME_SIZE                              ERR_DEC_UNIPLAY_KDVD_FRAME_SIZE
#define ERR_DEC_UNIPLAY_KDAE_MODULE_UNINITED                         6012
#define ERR_DEC_UNIPLAY_KDAE_MODULE_INITED                           6013

#define ERR_DEC_UNIPLAY_KDVC_ERROR                                   7001
#define ERR_DEC_UNIPLAY_KDVC_PARAM                                   7002
#define ERR_DEC_UNIPLAY_KDVC_INIT                                    7003
#define ERR_DEC_UNIPLAY_KDVC_ENUM                                    7004
#define ERR_DEC_UNIPLAY_KDVC_START                                   7005
#define ERR_DEC_UNIPLAY_KDVC_GETPIN                                  7006
#define ERR_DEC_UNIPLAY_KDVC_RENDER                                  7007
#define ERR_DEC_UNIPLAY_KDVC_GETCAPBILITY                            7008
#define ERR_DEC_UNIPLAY_KDVC_SETCAPBILITY                            7009
#define ERR_DEC_UNIPLAY_KDVC_DEVICE                                  7010


/**
 * mediaswitch错误码定义
 * 错误码范围20001~29999
 */

#define ERR_DEC_MS_PARAM                                             20001
#define ERR_DEC_MS_NOCREATE                                          20002
#define ERR_DEC_MS_MAXCREATE                                         20003
#define ERR_DEC_MS_MEM                                               20004
#define ERR_DEC_MS_UNSUPPORT                                         20005
#define ERR_DEC_MS_UNKNOWN                                           20006
#define ERR_DEC_MS_SOCKET                                            20007
#define ERR_DEC_MS_CONNECTTIMEOUT                                    20008
#define ERR_DEC_MS_CONNECTIONCLOSE                                   20009
#define ERR_DEC_MS_AGAIN                                             20010
#define ERR_DEC_MS_CONNECT                                           20011
#define ERR_DEC_MS_LISTEN                                            20012
#define ERR_DEC_MS_TASKCREATE                                        20013
#define ERR_DEC_MS_NOTRTP                                            20014
#define ERR_DEC_MS_SEMCREATE                                         20015
#define ERR_DEC_MS_NOINIT                                            20016
#define ERR_DEC_MS_BINDSOCKET                                        20017
#define ERR_DEC_MS_FRAMEFULL                                         20018
#define ERR_DEC_MS_PAYLOAD                                           20019
#define ERR_DEC_MS_FRAMELIST                                         20020
#define ERR_DEC_MS_NOSTREAM                                          20021
#define ERR_DEC_MS_GETAGAIN                                          20022
#define ERR_DEC_MS_READSLOW                                          20023
#define ERR_DEC_MS_DATA                                              20024
#define ERR_DEC_MS_NOKEYFRAME                                        20025
#define ERR_DEC_MS_NOTSTART                                          20026
#define ERR_DEC_MS_FRAMELISTFULL                                     20027
#define ERR_DEC_MS_CREATEAGAIN                                       20028
#define ERR_DEC_MS_SPACEFULL                                         20029
#define ERR_DEC_MS_FRAME_LIST_FULL                                   20030
#define ERR_DEC_MS_TCP_SEND                                          20031
#define ERR_DEC_MS_HASH_ERR                                          20032
#define ERR_DEC_MS_BIND_FAILED                                       20033
#define ERR_DEC_MS_INPROCESS                                         20034
#define ERR_DEC_MS_ACCEPTTIMEOUT                                     20035
#define ERR_DEC_MS_BIND_INUSE                                        20036
#define ERR_DEC_MS_UNSUPPORT_PSSTREAMID                              20037
#define ERR_DEC_MS_SEM_TIMEOUT                                       20028


/**
 * decsdk内部错误码定义
 * 错误码范围30001~39999
 */

#define ERR_DEC_INIT_FAILED                                          30001
#define ERR_DEC_INVALID_HANDLE                                       30002
#define ERR_DEC_NO_VALID_HANDLE                                      30003
#define ERR_DEC_CREATE_PLAYER_FAILED                                 30004
#define ERR_DEC_RTPDATA_ERROR                                        30005
#define ERR_DEC_SETRTSPURL_FAILED                                    30006
#define ERR_DEC_RTSPSTART_FAILED                                     30007
#define ERR_DEC_UNIPLAY_NOTINIT                                      30008
#define ERR_DEC_UNIPLAY_GETPORT_FAILED                               30009
#define ERR_DEC_MEDIASWITCH_NOTINIT                                  30010
#define ERR_DEC_INVALID_PARAM                                        30011


/**
 * kdmfileinterface错误码定义
 * 错误码范围40001~40999
 */

#define ERR_DEC_KF_FILENOTFOUND                                      40001
#define ERR_DEC_KF_INVALIDASFFILE                                    40002
#define ERR_DEC_KF_OBJECTNOTFOUND                                    40003
#define ERR_DEC_KF_BADINDEX                                          40004
#define ERR_DEC_KF_VALUENOTFOUND                                     40005
#define ERR_DEC_KF_BADARGUMENT                                       40006
#define ERR_DEC_KF_READONLY                                          40007
#define ERR_DEC_KF_NOTENOUGHSPACE                                    40008
#define ERR_DEC_KF_INTERNALERROR                                     40009
#define ERR_DEC_KF_READFILEERROR                                     40010
#define ERR_DEC_KF_WRITEFILEERROR                                    40011
#define ERR_DEC_KF_SEEKFILEERROR                                     40012
#define ERR_DEC_KF_CREATEFILEERROR                                   40013
#define ERR_DEC_KF_CREATETHREADERROR                                 40014
#define ERR_DEC_KF_OBJECTNULLERROR                                   40015
#define ERR_DEC_KF_TOOMUCHVIDEOSTREAM                                40016
#define ERR_DEC_KF_NOTSUPPORTEDCODEC                                 40017
#define ERR_DEC_KF_FILEEND                                           40018
#define ERR_DEC_KF_UNKNOWNERROR                                      40019
#define ERR_DEC_KF_INVALIDKSJFILE                                    40020
#define ERR_DEC_KF_DISKFULL                                          40021
#define ERR_DEC_KF_FRAMECHECKERROR                                   40022
#define ERR_DEC_KF_READDISKERROR                                     40023
#define ERR_DEC_KF_ASFLIBNOTSTARTUP                                  40024
#define ERR_DEC_KF_TIMERINITERROR                                    40025

#endif
