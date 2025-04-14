#ifndef _NETSDK_H_
#define _NETSDK_H_

#include "netsdkerrno.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * netsdk���Ͷ���
 */

typedef char           s8;
typedef unsigned char  u8;
typedef short          s16;
typedef unsigned short u16;
typedef int            s32;
typedef s32            BOOL32;

#if defined(_WIN64) || defined(WIN64) || defined (__LP64__) || defined (__64BIT__) || defined (_LP64) || (__WORDSIZE == 64)
	typedef unsigned int  u32;
#else
	typedef unsigned long u32;
#endif

#ifdef _MSC_VER
	typedef __int64            s64;
	typedef unsigned __int64   u64;
#else
	typedef long long          s64;
	typedef unsigned long long u64;
#endif


/**
 * �궨��
 */

#define NET_MAXLEN_8							8
#define NET_MAXLEN_16							16
#define NET_MAXLEN_32							32
#define NET_MAXLEN_64							64
#define NET_MAXLEN_128							128
#define NET_MAXLEN_256							256
#define NET_MAXLEN_512							512
#define NET_NVR_USER_NAME_MAX_LEN				64		///< NVR�û�����󳤶ȣ�32���ַ�
#define NET_NVR_USER_PWD_MAX_LEN				16		///< NVR�û�������󳤶ȣ�16���ַ�
#define NET_NVR_DISK_MAX_NUM					50		///< NVRӲ���������
#define NET_NVR_USER_MAX_NUM					16		///< NVR�û��������
#define NET_NVR_ETH_MAX_NUM						12		///< NVR�����������
#define NET_PER_GET_SEARCHED_DEV_MAX_NUM		32		///< ÿ�λ�ȡ���������豸���������
#define NET_PER_ADD_DEV_MAX_NUM					64		///< ÿ������豸���������
#define NET_PER_DEL_DEV_MAX_NUM					64		///< ÿ��ɾ���豸���������
#define NET_PER_UPGRADE_DEV_MAX_NUM				64		///< ÿ�������豸���������
#define NET_PER_UPLOAD_UPGRADE_FILE_MAX_SIZE    (1 * 1024 * 1024)		///< ÿ���ϴ����������ݵ�����С
#define NET_UPLOAD_UPGRADE_FILE_CGI_MAX_SIZE    (2 * 1024 * 1024)		///< �ϴ�������Э�������С
#define NET_PER_GET_NETDISK_MAX_NUM				16		///< ÿ�λ�ȡ����Ӳ�̵��������
#define NET_PROTO_MAX_NUM						8		///< Э���������
#define NET_PER_GET_CHNLIST_MAX_NUM				32		///< ÿ�λ�ȡͨ���б���������
#define NET_CHNGROUP_CHNITEM_MAX_NUM			64		///< ͨ��������ͨ���������
#define NET_CHNGROUP_MAX_NUM					16		///< ͨ�������������
#define NET_FRAME_STYLE_GRID_MAX_NUM			128		///< �������и��ӵ��������
#define NET_BIND_MAX_NUM						128		///< ���������
#define NET_PIP_MAX_NUM							16		///< ���л��������
#define NET_DEC_STATE_MAX_NUM					256		///< ����״̬�������
#define NET_PER_GET_CHN_CAP_MAX_NUM				128		///< ÿ�λ�ȡͨ�������б���������
#define NET_PER_GET_CHNPERM_MAX_NUM				256		///< ÿ�λ�ȡͨ��Ȩ���б���������
#define NET_VIDEOENC_RECMODE_MAX_NUM			8		///< ��Ƶ�������ʿ���ģʽ�������
#define NET_VIDEOENC_RESOLUTION_MAX_NUM			32		///< ��Ƶ����ֱ����������
#define NET_VIDEOENC_ENCTYPE_MAX_NUM  			8		///< ��Ƶ������������������
#define NET_PER_CHN_ENCCHN_MAX_NUM  			8		///< ÿ��ͨ��������ͨ������
#define NET_AUDIOENC_ENCTYPE_MAX_NUM 			8		///< ��Ƶ������������������
#define NET_LOG_MAINTYPR_MAX_NUM     			5		///< ��־����������
#define NET_LOG_SUBTYPR_MAX_NUM     			20		///< ��־�����������
#define NET_PER_GET_LOG_MAX_NUM         		100		///< ÿ�λ�ȡ��־�������
#define NET_CHN_ALIAS_LIST_MAX_NUM		    	128		///< ͨ�������б��������
#define NET_DISPLAY_PORT_MAX_NUM                4		///< ��ʾ�ӿ��������
#define NET_DISPLAY_PORT_RESOLUTION_MAX_NUM     16		///< ��ʾ�ӿڷֱ����������
#define NET_PER_CHNGETRECLIST_MAX_NUM           /*300*/150		///< ÿ�λ�ȡͨ��¼����������
#define NET_PER_CHNPLAYBACKLIST_MAX_NUM         32		///< ÿ�η���ͨ�����������
#define NET_PER_CHNGETMONTHREC_MAX_NUM          32		///< ÿ�λ�ȡ����ͼͨ�����������
#define NET_GROUP_DISKS_MAX_NUM					64		///< ����֧��Ӳ��ID��Ŀ
#define NET_GROUP_MAX_NUM						16		///< ֧��������Ŀ
#define NET_EXDISK_MAX_NUM						16		///< ֧�ֹ����ⲿUSB
#define NET_INTERNALDISK_MAX_NUM			    16		///< �������Ӳ����Ŀ
#define NET_RECTIMEPLAN_MAX_NUM                 8		///< ¼��ƻ��������
#define NET_PRE_DAY_RECTIMEPLAN_MAX_NUM         8		///< ÿ������¼��ʱ�����
#define NET_ALARM_GUARD_TIME_SEG_MAX_NUM		8		///< ÿ��澯����ʱ��ʱ����������
#define NET_ALARM_GUARD_TIME_MAX_NUM			8		///< �澯����ʱ���������
#define NET_ALARM_IN_ID_MAX_NUM					20		///< �澯Դ�澯������������
#define NET_ALARM_IN_COPY_MAX_NUM				64		///< �澯���븴���������
#define NET_ALARM_LINK_OUT_MAX_NUM				256		///< �澯��������������
#define NET_ALARM_LINK_PTZ_MAX_NUM				256		///< �澯����PTZ�������
#define NET_SYS_ALARM_CFG_MAX_NUM				16		///< ϵͳ�澯�����������
#define NET_COPYCHNRECPLAN_MAX_NUM				256		///< ����ͨ��¼��ƻ��������
#define NET_HOLIDAY_MAX_NUM				        12		///< �����������
#define NET_DISKGROUP_CHN_MAX_NUM               128		///< �������ͨ����
#define NET_PLAYBACK_MAX_NUM    				16		///< �ط����ͨ����
#define NET_PLAYBACK_ITEM_MAX_NUM  				4		///< ͨ���ط�·���������
#define NET_COPYDISKQUOTA_MAX_NUM			    512		///< ����ͨ������������
#define NET_PER_QUERYSNAPPIC_MAX_NUM            300		///< ÿ�β�ѯץ��ͼƬ���������
#define NET_PER_QUERYRECTAG_MAX_NUM             200		///< ÿ�β�ѯ¼���ǩ���������
#define NET_REAL_LOG_MAX_NUM                    100		///< ʵʱ��־�������
#define NET_REAL_STATE_MAX_NUM                  15		///< ʵʱ״̬�������
#define NET_COMPOSITE_CHN_MAX_NUM               16		///< �ϳ�ͨ���������
#define NET_DEVOSD_MAX_NUM  					16		///< �豸OSD�������
#define NET_USERLEVEL_MAX_NUM  					3		///< �û������������
#define NET_PER_GET_CHN_ALARMCFG_MAX_NUM		32		///< ÿ�λ�ȡͨ���澯����ͨ�����������
#define NET_CHNID_AUTO							0		///< ����豸����ʾ��ָ��ͨ�����Զ�����
#define NET_MAX_SMART_ATTR_ITEM_NUM             32      ///< smart��������������
#define NET_MAX_ERROR_SECTOR_NUM                50      ///< ��������ȡ����lba�������
#define NET_UPNP_PORT_MAP_MAX_NUM				10		///< UPnPӳ��˿��������
#define NET_NVR_VIDEO_ENC_MAX_NUM		    	4		///< NVR��Ƶ����ͨ���������
#define NET_NVR_AUDIO_ENC_MAX_NUM		    	4		///< NVR��Ƶ����ͨ���������
#define NET_PER_GET_CHNRECSTATE_MAX_NUM			32		///< ÿ�λ�ȡͨ��¼��״̬���������
#define NET_PER_GET_ALARM_STATE_MAX_NUM			128		///< ÿ�λ�ȡ�澯״̬���������
#define NET_CLOUD_QR_CODE_MAX_NUM				4		///< �Ʒ����ά���������
#define NET_PER_DEV_ACTIVE_MAX_NUM			    32		///< ÿ����Ӽ����豸���������
#define NET_PER_LOCK_REC_MAX_NUM			    100		///< ÿ������¼����������
#define NET_NVR_NTY_EVENT_MAX_NUM			    50		///< NVR֪ͨ�¼��������
#define NET_GB28181_PRECHN_ENC_MAX_NUM		    4		///< �����豸ÿ��ͨ������ͨ���������
#define NET_GB28181_CHN_MAX_NUM		            64		///< �����豸ÿ��ͨ������ͨ���������
#define NET_SIM_CARD_MAX_NUM					4		///< SIM���������
#define NET_GB28181_REG_PLAT_MAX_NUM			4		///< ����ע��ƽ̨�������
#define NET_PUBSEC_REG_PLAT_MAX_NUM				4		///< ��ͼ��ע��ƽ̨�������
#define NET_AIU_RECENT_SNAP_PIC_GROUP_MAX_NUM			10		///< ��������ץ��ͼƬ�����������
#define NET_AIU_RECENT_SNAP_PIC_ITEM_MAX_NUM	5		///< ��������ץ��ÿ��ͼƬ�������
#define NET_PER_GET_AIU_CHN_CAP_MAX_NUM			32		///< ÿ�λ�ȡAIUͨ�������б���������
#define NET_FACE_MAX_NUM						16		///< �����������
#define NET_CAR_PLATE_MAX_NUM					32		///< �����������
#define NET_PIC_UPLOAD_PUBSEC_PLAT_MAX_NUM		4		///< ͼƬ�ϴ���ͼ��ƽ̨�������
#define NET_MANUALEVENT_MAXNUM                  32      ///< �ֶ��¼��������
#define NET_IPADDRFILTERLIST_MAXNUM             64      ///< IP��ַ�����б��������
#define NET_SYSTIME_SYNCTYPE_MAX_NUM            16      ///< ʱ��ͬ�������������
#define NET_FISH_EYE_RECT_MAX_NUM				8		///< ����ptzģʽ����ѡȡ���������
#define NET_DISK_PART_MAX_NUM					4		///< �����Զ�������������
#define NET_PTZ_3D_NRM_MAX_NUM					64		///< ptz3D����ͨ���������
#define NET_CTRLLIB_MAX_NUM						32		///< ���ؿ��������
#define NET_CTRLLIB_CUSTOM_LABEL_MAX_NUM		3		///< ���ؿ��Զ����ǩ�������
#define NET_RECOG_ENGINE_MAX_NUM				4		///< ʶ�������������
#define NET_PERSON_ITEM_MAX_NUM					36		///< ÿ�λ�ȡ��������Ա���������
#define NET_GET_ERR_PERSON_ITEM_MAX_NUM			100		///< ÿ�λ�ȡ������Ϣ��Ա���������
#define NET_PER_DEL_PERSON_MAX_NUM				36		///< ÿ��ɾ����Ա���������
#define NET_CTRLLIB_EXPORT_FILE_MAX_NUM			10		///< ���ؿ⵼���ļ����������
#define NET_ALGENGINE_MAX_NUM				    16		///< �����㷨�����������
#define NET_ALG_TYPE_MAX_NUM					2		///< ֧�ֵ��㷨�����������
#define NET_AIUUPLOADCAPS_MAX_NUM				16		///< �ϴ�ģʽ��֧�����͵��������
#define NET_AIU_GETBIGDATACFG_MAX_NUM			16		///< ��ȡͨ������ͼ���������õ��������
#define NET_AIU_BIGDATA_UPLOAD_MAX_NUM			16		///< ����ͼ�����������ϴ����������
#define NET_AIU_IMPORT_CTRLLIB_FILE_MAX_SIZE		(1 * 1024 * 1024)		///< ÿ�β��ؿ⵼���ϴ���������С
#define NET_AIU_IMPORT_CTRLLIB_FILE_CGI_MAX_SIZE    (2 * 1024 * 1024)		///< �ϴ����ؿ⵼���ϴ����ݵ�����С
#define NET_AIP_DETECT_CHN_MAX_NUM				128		///< ���ܴ���֧�ּ���ͨ���б��������
#define NET_AIP_UPLOAD_TYPE_MAX_NUM				4		///< ���ܴ���֧���ϴ���ʽ�������
#define NET_AIP_AI_CHN_ALG_PARAM_MAX_NUM		4		///< ����ͨ����Ӧ�㷨�����б��������
#define NET_AIP_COMPARE_RULE_CHN_MAX_NUM		64		///< ���ܴ���ȶ�ͨ���������
#define NET_AIP_COMPARE_RULE_MAX_NUM			30		///< ���ܴ���ȶԹ����б��������
#define NET_AISEARCH_CHN_MAX_NUM				128		///< ���ܼ���ÿ�λ�ȡ�����ͨ����
#define NET_FACE_SNAP_ITEM_MAX_NUM				36		///< ���ܼ���ÿ�λ�ȡ��Ա���������
#define NET_DETECT_SNAP_LIST_MAX_NUM			16		///< ���ץ���б��������
#define NET_CMP_ALARM_LIST_MAX_NUM				50		///< �ȶԱ����б��������
#define NET_GB28181_ENC_CHN_MAX_NUM		        260		///< �������ƽ̨�����Ƶͨ����
#define NET_GB28181_ALARM_CHN_MAX_NUM		    256		///< �������ƽ̨���澯ͨ����
#define NET_SUBSCRIBE_MSG_MAX_NUM				5		///< ������Ϣ�������
#define NET_HEALTHLOG_TYPE_MAX_NUM				5		///< ����״̬��Ϣ�����������
#define NET_GET_HEALTHLOG_DETAIL_MAX_NUM		30		///< ��ȡ����״̬�����������
#define NET_PTZ_PRESET_MAX_NUM					256		///< ptzԤ��λ������
#define NET_AI_RULELINK_UPLOAD_MAX_NUM			2		///< �ȶԹ���������ʽ�ϴ���ͼ���������
#define NET_ISP_SCENE_CUSTOMLIST_MAX_NUM		8		///< ISP����ģʽ�û��Զ����б��������
#define NET_ISP_SCENE_DAY_DYNAMIC_MAX_NUM       10		///< ISP����ģʽÿ�����Ķ�̬ģʽʱ�����
#define NET_AREA_POINT_MAX_NUM					8       ///< �������ö����������


/**
 * ö�ٶ���
 */

enum EInterType
{
	INTERTYPE_NONE = 0,
	INTERTYPE_GetChnVideoEncAsync,			///< ��ȡͨ����Ƶ�������
	INTERTYPE_GetChnAudioEncAsync,			///< ��ȡͨ����Ƶ�������
	INTERTYPE_CtrlLibExportGetFileAsync,	///< ��ȡ���ؿ⵼���ļ�
};

enum EMsgType
{
	MSGTYPE_FACE_DETECT_SNAP	= 1,	///< �������ץ����Ϣ����Ϣ��TNetDetectSnapItem
	MSGTYPE_CAR_DETECT_SNAP		= 2,	///< �������ץ����Ϣ����Ϣ��TNetDetectSnapItem
	MSGTYPE_PERSON_DETECT_SNAP	= 3,	///< ���˼��ץ����Ϣ����Ϣ��TNetDetectSnapItem
	MSGTYPE_CMP_ALARM			= 4,	///< �ȶԱ�����Ϣ����Ϣ��TNetCmpAlarmItem
	MSGTYPE_PIC_UPLOAD_STATUS	= 5,	///< ͼƬ�ϴ�״̬��Ϣ����Ϣ��TNetPicUploadStatus
};

enum EGuiLangType
{
	GUILANGTYPE_CN	= 0x01, ///< ��������
	GUILANGTYPE_CHT = 0x02, ///< ��������
	GUILANGTYPE_EN	= 0x04, ///< Ӣ��
	GUILANGTYPE_ES	= 0x08, ///< ��������
	GUILANGTYPE_TR	= 0x10, ///< ��������
	GUILANGTYPE_RU	= 0x20, ///< ����
	GUILANGTYPE_AR	= 0x40, ///< ��������
};

enum ENvrState
{
	NVRSTATE_DISCONNECTED	= 0x00000001, ///< ���ӶϿ�
	NVRSTATE_CHNLIST		= 0x00000002, ///< ͨ���б�ı�
	NVRSTATE_CHNALIAS		= 0x00000004, ///< ͨ�������ı�
	NVRSTATE_CHNSTATE		= 0x00000008, ///< ͨ��״̬�ı�
	NVRSTATE_CHNGROUP		= 0x00000010, ///< ͨ����ı�
	NVRSTATE_RESOLUTION		= 0x00000020, ///< �ֱ��ʸı�
	NVRSTATE_DISKHOTSWAP	= 0x00000040, ///< Ӳ���Ȳ��
	NVRSTATE_REALLOG		= 0x00000080, ///< ʵʱ��־
	NVRSTATE_REALSTATE		= 0x00000100, ///< ʵʱ״̬
	NVRSTATE_SYSSTATE		= 0x00000200, ///< ϵͳ״̬��U�̲�Ρ����߲�εȣ�
	NVRSTATE_NETSTATE		= 0x00000400, ///< ����״̬�ı�
	NVRSTATE_AIUSTATE		= 0x00000800, ///< ����״̬�ı�
	NVRSTATE_ALARMSTATE		= 0x00001000, ///< �澯״̬�ı�
};

enum EProtoType
{
	PROTOTYPE_UNKNOWN   = 0x00,   ///< δ֪Э��
	PROTOTYPE_ONVIF		= 0x01,   ///< onvifЭ��
	PROTOTYPE_GB28181	= 0x02,   ///< GB28181Э��
	PROTOTYPE_RTSP      = 0x04,   ///< RTSPЭ��
	PROTOTYPE_VSIP      = 0x08,   ///< VSIPЭ��
	PROTOTYPE_IPCSEARCH = 0x10,   ///< ipcsearchЭ��
	PROTOTYPE_LCAM      = 0x20,   ///< ��������ͷ
	PROTOTYPE_ALL		= 0xFF,   ///< ����Э��
};

enum ETransProto
{
	ETRANSPROTO_AUTO			= 0x01,   ///< �Զ�
	ETRANSPROTO_TCP				= 0x02,   ///< tcp
	ETRANSPROTO_UDP				= 0x04,   ///< udp
	ETRANSPROTO_RTP_OVER_TCP	= 0x08,   ///< rtp_over_tcp
};

enum ETcpMode
{
	TCPMODE_AUTO			= 0x01,   ///< �Զ�
	TCPMODE_CONNECT			= 0x02,   ///< ����
	TCPMODE_LISTEN			= 0x04,   ///< ����
};

enum EDevType
{
	DEVTYPE_NULL	= 0,    ///< ��Чֵ
	DEVTYPE_IPC		= 1,    ///< IPC
	DEVTYPE_DVS		= 2,    ///< DVS
	DEVTYPE_DVR		= 3,    ///< DVR
	DEVTYPE_NVR		= 4,    ///< NVR
	DEVTYPE_FISHEYE = 5,    ///< SINGLE_SRC_FISHEYE
};

enum EActiveState
{
	ACTIVESTATE_ACTIVE		= 1, ///< �Ѽ���
	ACTIVESTATE_NOTACTIVE	= 2, ///< δ����
	ACTIVESTATE_ABNORMAL	= 3, ///< �쳣
	ACTIVESTATE_UNKNOWN		= 4, ///< δ֪
	ACTIVESTATE_UNSUPPORT	= 5, ///< ��֧��
};

enum EDevAddMode
{
	DEVADDMODE_AUTO     		= 0x00,    ///< �Զ�
	DEVADDMODE_SINGLESRC		= 0x01,    ///< ��Դ���:single_source
	DEVADDMODE_MULITSRC	    	= 0x02,    ///< ��Դ���:multi_source
	DEVADDMODE_KEDAFISHEYE		= 0x04,    ///< KEDA����:keda_fisheye
};

enum EChnMask
{
	CHNMASK_CHNALIAS	= 0x01, ///< ��ȡͨ���б�ֻ��ȡͨ������
	CHNMASK_CHNSTATE	= 0x02, ///< ��ȡͨ���б�ֻ��ȡͨ��״̬
	CHNMASK_ALL			= 0xFF, ///< ��ȡͨ���б��ȡȫ����Ϣ
};

enum ECompositeChnType
{
	COMPOSITECHNTYPE_COMPOSITE	        = 1, ///< �ϳ�ͨ����composite
	COMPOSITECHNTYPE_COMPOSITE_EXTENC	= 2, ///< �����������ñ������ĺϳ�ͨ����composite_extenc
	COMPOSITECHNTYPE_FROMREMOTE         = 3, ///< ����Զ��ͨ����from_remote
	COMPOSITECHNTYPE_TOREMOTE           = 4, ///< ����Զ��ͨ����to_remote
};

enum EDevErrNo
{
	DEVERRNO_UNKNOWN		= 0, ///< δ֪����
	DEVERRNO_LINKING		= 1, ///< ������
	DEVERRNO_CONNECTFAILED	= 2, ///< ����ʧ�ܣ��������粻ͨ
	DEVERRNO_AUTHFAILED		= 3, ///< ��֤ʧ�ܣ�����onvif���û������벻��
	DEVERRNO_PARAMERR		= 4, ///< ��������
	DEVERRNO_CHNABNORMAL	= 5, ///< ͨ���쳣�������ȡ�������ʧ��
	DEVERRNO_NOSTREAM		= 6, ///< ȡ��ʧ��
	DEVERRNO_NOREGISTER		= 7, ///< δע��
};

enum EDevActiveState
{
	DEVACTIVESTATE_NONE		    = 0, ///< ��û�п�ʼ����
	DEVACTIVESTATE_DOING		= 1, ///< ���ڽ�����
	DEVACTIVESTATE_SUCCESS     	= 2, ///< �ɹ�
	DEVACTIVESTATE_FAIL		    = 3, ///< ʧ��
};

enum EDevActiveErrReason
{
	DEVACTIVEERRREASON_UNKNOWN		    = 0, ///< δ֪����
	DEVACTIVEERRREASON_AUTHFAIL		    = 1, ///< ��Ȩʧ��
	DEVACTIVEERRREASON_FORBIDDEN   	    = 2, ///< ��Ȩʧ�ܹ���
	DEVACTIVEERRREASON_TIMEOUT  		= 3, ///< ��ʱ
	DEVACTIVEERRREASON_ACTIVEAGAIN      = 4, ///< �ظ�����
	DEVACTIVEERRREASON_NOSUPPORTVER  	= 5, ///< ��֧�ֵľɰ汾ָ��
	DEVACTIVEERRREASON_INVALIDARG  		= 6, ///< ��Ч����(SECURE CODE����ȷ)
	DEVACTIVEERRREASON_PWDWEAK  		= 7, ///< �������밲ȫ�Բ���
};

enum EDevUpgradeState
{
	DEVUPGRADESTATE_PREPARING		= 0, ///< ׼��������
	DEVUPGRADESTATE_SENDING     	= 1, ///< ���ڷ���������
	DEVUPGRADESTATE_CHECKING		= 2, ///< ���ڼ��ǰ���Ƿ�����
	DEVUPGRADESTATE_DONE    		= 3, ///< �����������
	DEVUPGRADESTATE_ERROR        	= 4, ///< �����г���
	DEVUPGRADESTATE_NOTSUPPORT		= 5, ///< ��֧������
	DEVUPGRADESTATE_DEVNOTEXIST		= 6, ///< �豸������
};

enum EDevUpgradeErrReason
{
	DEVUPGRADEERRREASON_AUTHFAIL		= 0, ///< ��Ȩʧ��
	DEVUPGRADEERRREASON_PREPAREERR		= 1, ///< ׼���׶�ʧ��
	DEVUPGRADEERRREASON_UPLOADERR   	= 2, ///< �ϴ�������ʧ��
	DEVUPGRADEERRREASON_TIMEOUT  		= 3, ///< ��ʱ
	DEVUPGRADEERRREASON_CHECKERR  		= 4, ///< �������״̬ʧ��
};

enum ETimeSyncType
{
	TIMESYNCTYPE_CLOSE         = 0x01,   ///< �ر�
	TIMESYNCTYPE_TIMEANDZONE   = 0x02,   ///< ʱ����ʱ���ʽ
	TIMESYNCTYPE_LOCALTIME     = 0x04,   ///< ����ʱ���ʽ
	TIMESYNCTYPE_UTCTIME       = 0x08,   ///< UTCʱ���ʽ
};

enum EOsdResolutionType
{
	OSDRESOLUTIONTYPE_1080         = 0x0001,   ///< 1080p
	OSDRESOLUTIONTYPE_720          = 0x0002,   ///< 720p
	OSDRESOLUTIONTYPE_D1           = 0x0004,   ///< D1
	OSDRESOLUTIONTYPE_CIF          = 0x0008,   ///< cif
	OSDRESOLUTIONTYPE_2048X1520    = 0x0010,   ///< 2048*1520
	OSDRESOLUTIONTYPE_2560X1440    = 0x0020,   ///< 2560*1440
	OSDRESOLUTIONTYPE_2592X1520    = 0x0040,   ///< 2592*1520
	OSDRESOLUTIONTYPE_3072X1728    = 0x0080,   ///< 3072*1728
	OSDRESOLUTIONTYPE_2816X2112    = 0x0100,   ///< 2816*2112
	OSDRESOLUTIONTYPE_3840X2160    = 0x0200,   ///< 3840*2160
	OSDRESOLUTIONTYPE_4096X2160    = 0x0400,   ///< 4096*2160
	OSDRESOLUTIONTYPE_3840X1800    = 0x0800,   ///< 3840*1800
	OSDRESOLUTIONTYPE_4096X1800    = 0x1000,   ///< 4096*1800
};

enum EOsdPicType
{
	OSDPICTYPE_BMP         = 0x01,   ///< bmp��ʽ
	OSDPICTYPE_JPG         = 0x02,   ///< jpg��ʽ
};

enum EOsdLanguageType
{
	OSDLANGUAGETYPE_CHS               = 0x0001,   ///< ���ļ���
	OSDLANGUAGETYPE_CHT               = 0x0002,   ///< ���ķ���
	OSDLANGUAGETYPE_EN                = 0x0004,   ///< Ӣ��
	OSDLANGUAGETYPE_KOR               = 0x0008,   ///< ����
};

enum EOsdTextColor
{
	OSDTEXTCOLOR_BLACK              = 0x0001,   ///< ��ɫ
	OSDTEXTCOLOR_WHITE              = 0x0002,   ///< ��ɫ
	OSDTEXTCOLOR_RED                = 0x0004,   ///< ��ɫ
	OSDTEXTCOLOR_GREEN              = 0x0008,   ///< ��ɫ
	OSDTEXTCOLOR_BLUE               = 0x0010,   ///< ��ɫ
	OSDTEXTCOLOR_AQUA               = 0x0020,   ///< ��ɫ
	OSDTEXTCOLOR_FUCHSIA            = 0x0040,   ///< ��ɫ
	OSDTEXTCOLOR_GRAY               = 0x0080,   ///< ��ɫ
	OSDTEXTCOLOR_LIME               = 0x0100,   ///< ǳ��ɫ
	OSDTEXTCOLOR_MAROON             = 0x0200,   ///< ��ɫ
	OSDTEXTCOLOR_NAVY               = 0x0400,   ///< ����ɫ
	OSDTEXTCOLOR_OLIVE              = 0x0800,   ///< ���ɫ
	OSDTEXTCOLOR_PURPLE             = 0x1000,   ///< ��ɫ
	OSDTEXTCOLOR_SILVER             = 0x2000,   ///< ��ɫ
	OSDTEXTCOLOR_TEAL               = 0x4000,   ///< ��ɫ
	OSDTEXTCOLOR_YELLOW             = 0x8000,   ///< ��ɫ
};

enum EOsdType
{
	OSDTYPE_UNKNOWN                 = 0x0000,   ///< δ֪����OSD
	OSDTYPE_USER                    = 0x0001,   ///< �Զ��壬user
	OSDTYPE_LABEL                   = 0x0002,   ///< ̨�꣬label
	OSDTYPE_TIME                    = 0x0004,   ///< ʱ�䣬time
	OSDTYPE_ALARM                   = 0x0008,   ///< �澯��alarm
	OSDTYPE_PTZ                     = 0x0010,   ///< ��λͼ��ptz
	OSDTYPE_3G                      = 0x0020,   ///< 3Gͼ�꣬3g
	OSDTYPE_RECORD                  = 0x0040,   ///< ¼���־��record
	OSDTYPE_GPS                     = 0x0080,   ///< ��γ�ȣ�gps
	OSDTYPE_SERIESID                = 0x0100,   ///< �豸���кţ�seriesid
	OSDTYPE_PTPOINT                 = 0x0200,   ///< �����ǣ�ptpoint
	OSDTYPE_SPEED                   = 0x0400,   ///< �ٶȣ�speed
	OSDTYPE_TF                      = 0x0400,   ///< �����쳣��tf
};

enum EOsdContentType
{
	OSDCONTENTTYPE_TXT              = 0x0001,   ///< �ı���txt
	OSDCONTENTTYPE_PIC              = 0x0002,   ///< ͼƬ��pic
	OSDCONTENTTYPE_DEV              = 0x0004,   ///< ���裬dev
	OSDCONTENTTYPE_ZM               = 0x0008,   ///< �Ŵ��ʣ�zm
	OSDCONTENTTYPE_BAT              = 0x0010,   ///< ��ص�����bat
	OSDCONTENTTYPE_GPS              = 0x0020,   ///< GPS��Ϣ��gps
	OSDCONTENTTYPE_STATIC           = 0x0040,   ///< ���ɱ༭��Ļ(������Ļ)��static
	OSDCONTENTTYPE_EDIT             = 0x0080,   ///< �ɱ༭��Ļ(������Ļ)��edit
	OSDCONTENTTYPE_DYNAMIC          = 0x0100,   ///< ��̬��Ļ(������Ļ))��dynamic
};

enum EOsdMarginType
{
	OSDMARGINTYPE_0         = 0x01,   ///< �߾�Ϊ0
	OSDMARGINTYPE_1         = 0x02,   ///< �߾�Ϊ1
	OSDMARGINTYPE_2         = 0x04,   ///< �߾�Ϊ2
	OSDMARGINTYPE_3         = 0x08,   ///< �߾�Ϊ3
	OSDMARGINTYPE_4         = 0x10,   ///< �߾�Ϊ4
};

enum EOsdTimeFormat
{
	OSDTIMEFORMAT_MFIRST         = 0x01,   ///< MM-DD-YYYY
	OSDTIMEFORMAT_YFIRST         = 0x02,   ///< YYYY-MM-DD
	OSDTIMEFORMAT_DFIRST         = 0x04,   ///< DD-MM-YYYY
};

enum EOsdFontType
{
	OSDFONTTYPE_SONG          = 0x01,   ///< ����(song)
	OSDFONTTYPE_BLACK         = 0x02,   ///< ����(black)
	OSDFONTTYPE_COURIERNEW    = 0x04,   ///< Courier New(CourierNew)
};

enum EOsdFontSizeType
{
	OSDFONTSIZETYPE_12          = 0x0001,   ///< 12
	OSDFONTSIZETYPE_16          = 0x0002,   ///< 16
	OSDFONTSIZETYPE_20          = 0x0004,   ///< 20
	OSDFONTSIZETYPE_24          = 0x0008,   ///< 24
	OSDFONTSIZETYPE_32          = 0x0010,   ///< 32
	OSDFONTSIZETYPE_40          = 0x0020,   ///< 40
	OSDFONTSIZETYPE_48          = 0x0040,   ///< 48
	OSDFONTSIZETYPE_64          = 0x0080,   ///< 64
	OSDFONTSIZETYPE_72          = 0x0100,   ///< 72
	OSDFONTSIZETYPE_80          = 0x0200,   ///< 80
	OSDFONTSIZETYPE_96          = 0x0400,   ///< 96
	OSDFONTSIZETYPE_108         = 0x0800,   ///< 108
	OSDFONTSIZETYPE_116         = 0x1000,   ///< 116
};

enum EOsdFontAttrType
{
	OSDFONTATTRTYPE_NAME          = 0x01,   ///< ���壬name
	OSDFONTATTRTYPE_SIZE          = 0x02,   ///< ��С��size
	OSDFONTATTRTYPE_COLOR         = 0x04,   ///< ��ɫ��color
	OSDFONTATTRTYPE_BKCLR         = 0x08,   ///< ����ɫ��bkclr
	OSDFONTATTRTYPE_TRANSPARENT   = 0x10,   ///< ͸���ȣ�transparent
	OSDFONTATTRTYPE_EDGE          = 0x20,   ///< ��ߣ�edge
};

enum EDiskState
{
	DISKSTATE_NORMAL	= 0, ///< ����
	DISKSTATE_DORMANCY	= 1, ///< ����
	DISKSTATE_ABNORMAL	= 2, ///< �쳣
	DISKSTATE_UNINIT	= 3, ///< δ��ʼ��
	DISKSTATE_OFFLINE	= 4, ///< ������
};

enum EDiskAttribute
{
	DISKATTRIBUTE_READWRITE	= 0, ///< �ɶ�д
	DISKATTRIBUTE_READONLY	= 1, ///< ֻ��
	DISKATTRIBUTE_UNKNOWN	= 2, ///< δ֪
};

enum EDiskType
{
	DISKTYPE_INTERNAL	= 0x0001, ///< ����Ӳ��
	DISKTYPE_ESATA		= 0x0002, ///< esata
	DISKTYPE_USB		= 0x0004, ///< usb
	DISKTYPE_IPSAN		= 0x0008, ///< ipsan
	DISKTYPE_NAS		= 0x0010, ///< nas
	DISKTYPE_RAID		= 0x0020, ///< raid
	DISKTYPE_LOCAL_SD	= 0x0040, ///< �洢������-��������
	DISKTYPE_INTERNAL_SD = 0x0080, ///< �洢������-��������
};

enum EDiskPartType
{
	DISKPARTTYPE_REC	= 0x0001, ///< ¼�����
	DISKPARTTYPE_IMG	= 0x0002, ///< ץ�ķ���
};

enum EDiskSmartTestState
{
	DISKSMARTTESTSTATE_HEALTH	        = 0, ///< health,����
	DISKSMARTTESTSTATE_BAD		        = 1, ///< bad,����
	DISKSMARTTESTSTATE_NOTSUPPORT		= 2, ///< not_support,��֧��
	DISKSMARTTESTSTATE_PARAERROR		= 3, ///< para_err,������
};

enum EDiskUsingType
{
	DISKUSINGTYPE_NORMAL	        = 0, ///< ��ͨ�洢
	DISKUSINGTYPE_SMART		        = 1, ///< ���ܴ洢
};

enum ENetDiskType
{
	NETDISKTYPE_IPSAN	= 0x01, ///< ipsan
	NETDISKTYPE_NAS		= 0x02, ///< nas
};

enum EStoreMode
{
	STOREMODE_QUOTA       = 0x01,   ///< ���
	STOREMODE_DISKPACK    = 0x02,   ///< ����
};

enum ERaidLevel
{
	RAIDLEVEL_X  = 0x01, ///< RAIDX
	RAIDLEVEL_0  = 0x02, ///< RAID0
	RAIDLEVEL_1  = 0x04, ///< RAID1
	RAIDLEVEL_5  = 0x08, ///< RAID5
	RAIDLEVEL_6  = 0x10, ///< RAID6
	RAIDLEVEL_10 = 0x20, ///< RAID10
};

enum ERaidCreateType
{
	RAIDCREATETYPE_FAST  = 0x01, ///< fast,���ٴ���
	RAIDCREATETYPE_FULL  = 0x02, ///< full,��������
};

enum ERaidStatus
{
	RAIDSTATUS_CLEAN       = 0x0001, ///< clean,����
	RAIDSTATUS_DEGRADED    = 0x0002, ///< degraded,����
	RAIDSTATUS_RECOVERING  = 0x0004, ///< recovering,�ָ�
	RAIDSTATUS_FAILED      = 0x0008, ///< failed,ʧЧ
	RAIDSTATUS_RESYNCING   = 0x0010, ///< resyncing,ͬ��
	RAIDSTATUS_REPAIRING   = 0x0020, ///< repairing,�޸���
	RAIDSTATUS_INITING     = 0x0040, ///< initing,��ʼ��
	RAIDSTATUS_CHECKING    = 0x0080, ///< checking,���
	RAIDSTATUS_DELING      = 0x0100, ///< deling,ɾ��
};

enum EFrameStyle
{
	FRAMESTYLE_1  = 1,		///< 1����(1*1)
	FRAMESTYLE_4  = 4,		///< 4����(2*2)
	FRAMESTYLE_6  = 6,		///< 6����(1+5)
	FRAMESTYLE_8  = 8,		///< 8����(1+7)
	FRAMESTYLE_9  = 9,		///< 9����(3*3)
	FRAMESTYLE_16 = 16,		///< 16����(4*4)
	FRAMESTYLE_25 = 25,		///< 25����(5*5)
	FRAMESTYLE_36 = 36,		///< 36����(6*6)
	FRAMESTYLE_49 = 49,		///< 49����(7*7)
	FRAMESTYLE_64 = 64,		///< 64����(8*8)
};

enum EPipType
{
	PIPTYPE_MONITOR				= 1, ///< ʵʱ������л�
	PIPTYPE_MONITORDIGITALZOOM	= 2, ///< ʵʱ������ַŴ��л�
	PIPTYPE_PLAYREC				= 3, ///< �طŻ��л�
	PIPTYPE_PLAYRECDIGITALZOOM  = 4, ///< �ط����ַŴ��л�
	PIPTYPE_PLAYRECPIC			= 5, ///< �ط�ʱ����Ԥ��ͼƬ
};

enum ETransType
{
	TRANSTYPE_RTPOVERTCP			= 0x01, ///< ���䷽ʽ��rtp_over_udp
	TRANSTYPE_RTPOVERUDP			= 0x02, ///< ���䷽ʽ��rtp_over_tcp
	TRANSTYPE_RTPOVERRTSPOVERTCP	= 0x04, ///< ���䷽ʽ��rtp_over_rtsp_over_tcp
	TRANSTYPE_RTPOVERHTTP			= 0x08, ///< ���䷽ʽ��rtp_over_http
};

enum EStreamPackType
{
	STREAMPACKTYPE_PS	= 0x01, ///< ���������ʽ��PS
	STREAMPACKTYPE_ES	= 0x02, ///< ���������ʽ��ES
};

enum ECallMode
{
	CAllMODE_LOCALCALL			= 1, ///< ���غ���
	CAllMODE_LOCALBROADCAST		= 2, ///< ���ع㲥
	CAllMODE_REMOTECALL			= 3, ///< Զ�˺���
	CAllMODE_REMOTEBROADCAST	= 4, ///< Զ�˹㲥
};

enum ECallType
{
	CALLTYPE_CHN = 2, ///< ����ͨ��
};

enum EPTZCMD
{
	PTZCMD_MOVEUP			= 1,  ///< �����ƶ�
	PTZCMD_MOVEDOWN			= 2,  ///< �����ƶ�
	PTZCMD_MOVELEFT			= 3,  ///< �����ƶ�
	PTZCMD_MOVERIGHT		= 4,  ///< �����ƶ�
	PTZCMD_MOVELEFTUP		= 5,  ///< �����ƶ�
	PTZCMD_MOVELEFTDOWN		= 6,  ///< �����ƶ�
	PTZCMD_MOVERIGHTUP		= 7,  ///< �����ƶ�
	PTZCMD_MOVERIGHTDOWN	= 8,  ///< �����ƶ�
	PTZCMD_MOVESTOP			= 9,  ///< ֹͣ�ƶ�
	PTZCMD_RESET			= 10, ///< ��λ
	PTZCMD_FOCUSFAR			= 11, ///< �����Զ
	PTZCMD_FOCUSNEAR		= 12, ///< �������
	PTZCMD_FOCUSAUTO		= 13, ///< �Զ�����
	PTZCMD_FOCUSSTOP		= 14, ///< ֹͣ����
	PTZCMD_IRISPLUS			= 15, ///< ��Ȧ����
	PTZCMD_IRISMINUS		= 16, ///< ��Ȧ��С
	PTZCMD_IRISAUTO			= 17, ///< �Զ���Ȧ
	PTZCMD_IRISSTOP			= 18, ///< ֹͣ��Ȧ����
	PTZCMD_ZOOMTELE			= 19, ///< ����
	PTZCMD_ZOOMWIDE			= 20, ///< ��Զ
	PTZCMD_ZOOMSTOP			= 21, ///< ֹͣ��Ұ����
	PTZCMD_LIGHTOPEN		= 22, ///< ����
	PTZCMD_LIGHTCLOSE		= 23, ///< �ص�
	PTZCMD_WIPEROPEN		= 24, ///< ����ˢ
	PTZCMD_WIPERCLOSE		= 25, ///< ����ˢ
	PTZCMD_PRESET_CALL		= 26, ///< Ԥ��λ����
	PTZCMD_PRESET_SAVE		= 27, ///< ����Ԥ��λ
	PTZCMD_PRESET_DEL		= 28, ///< ɾ��Ԥ��λ
	PTZCMD_PATHCRUISE_CALL	= 29, ///< Ѳ��·������
	PTZCMD_PATHCRUISE_STOP	= 30, ///< Ѳ��·��ֹͣ
	PTZCMD_ZOOM_PART        = 31, ///< �ֲ��Ŵ󣨿�ѡ�Ŵ�
	PTZCMD_ZOOM_WHOLE		= 32, ///< �ֲ���С����ѡ��С��
	PTZCMD_GOTO_POINT		= 33, ///< ���Ķ�λ��˫�����У�
};

enum EFishInstallType
{
	FISHINSTALLTYPE_CEIL	= 0x01,		///< ����
	FISHINSTALLTYPE_MT		= 0x02,		///< ǽ��
	FISHINSTALLTYPE_DT		= 0x04,		///< ����
};

enum EFishDisplayType
{
	FISHDISPLAYTYPE_FE			= 0x0001,		///< ����
	FISHDISPLAYTYPE_OV			= 0x0002,		///< ȫ��
	FISHDISPLAYTYPE_OV180		= 0x0004,		///< 180��ȫ��
	FISHDISPLAYTYPE_OV360		= 0x0008,		///< 360��ȫ��
	FISHDISPLAYTYPE_OV_3PTZ		= 0x0010,		///< ȫ��+3PTZ
	FISHDISPLAYTYPE_OV_8PTZ		= 0x0020,		///< ȫ��+8PTZ
	FISHDISPLAYTYPE_OV360_PTZ	= 0x0040,		///< 360��ȫ��+PTZ 
	FISHDISPLAYTYPE_OV360_3PTZ  = 0x0080,		///< 360��ȫ��+3PTZ
	FISHDISPLAYTYPE_OV360_6PTZ  = 0x0100,		///< 360��ȫ��+6PTZ
	FISHDISPLAYTYPE_OV360_8PTZ  = 0x0200,		///< 360��ȫ��+8PTZ
	FISHDISPLAYTYPE_2PTZ		= 0x0400,		///< 2PTZ
	FISHDISPLAYTYPE_4PTZ		= 0x0800,		///< 4PTZ
	FISHDISPLAYTYPE_FE_3PTZ		= 0x1000,		///< ����+3PTZ
	FISHDISPLAYTYPE_FE_8PTZ		= 0x2000,		///< ����+8PTZ
};

enum EIspNeedMask
{
	ISPNEEDMASK_IMAGEPARAM		= 0x00000001,		///< ͼ�����
	ISPNEEDMASK_GAIN			= 0x00000002,		///< �������
	ISPNEEDMASK_IRIS			= 0x00000004,		///< ��Ȧ����
	ISPNEEDMASK_FOCUS			= 0x00000008,		///< �۽�����
	ISPNEEDMASK_SHUTTER			= 0x00000010,		///< ���Ų���
	ISPNEEDMASK_FREQMODE		= 0x00000020,		///< ����˸ģʽ����
	ISPNEEDMASK_WHITEBALANCE	= 0x00000040,		///< ��ƽ�����
	ISPNEEDMASK_DAYNIGHTSWITCH	= 0x00000080,		///< ��ҹת������
	ISPNEEDMASK_DENOISE2D		= 0x00000100,		///< 2D�������
	ISPNEEDMASK_DENOISE3D		= 0x00000200,		///< 3D�������
	ISPNEEDMASK_DYNAMICREGULATE	= 0x00000400,		///< ��̬���ڲ���
	ISPNEEDMASK_IMAGEENHANCE	= 0x00000800,		///< ͼ����ǿ����
	ISPNEEDMASK_STABLIZER		= 0x00001000,		///< ��������
	ISPNEEDMASK_COMBINHDR		= 0x00002000,		///< �ϳɿ�̬����
	ISPNEEDMASK_DOUBLELENSMODE	= 0x00004000,		///< ������ģʽ����
	ISPNEEDMASK_EXPALLAUTO		= 0x00008000,		///< �ع�ȫ�Զ�ģʽ����
	ISPNEEDMASK_PROEXPOSURE		= 0x00010000,		///< �߼��Զ��ع����
	ISPNEEDMASK_SCENEMODE		= 0x00020000,		///< ����ģʽ����
	ISPNEEDMASK_FLIPANDPLAYBACK	= 0x00040000,		///< ��ת�ͻ��Բ���
	ISPNEEDMASK_INFRARED		= 0x00080000,		///< ��������ò���
	ISPNEEDMASK_LASER			= 0x00100000,		///< �������ò���
	ISPNEEDMASK_LDC				= 0x00200000,		///< ����������ò���
};

enum EIspGainMode
{
	ISPGAINMODE_MANUAL			= 0x0001,		///< �ֶ�����ģʽ
	ISPGAINMODE_AUTO			= 0x0002,		///< �Զ�����ģʽ
};

enum EIspIrisMode
{
	ISPIRISMODE_DC_IRISAUTO		= 0x0001,		///< DC-IRIS�Զ�
	ISPIRISMODE_DC_IRISMANUAL	= 0x0002,		///< DC-IRIS�ֶ�
	ISPIRISMODE_P_IRISMANUAL	= 0x0004,		///< P-IRIS�ֶ�
	ISPIRISMODE_P_IRISAUTO		= 0x0008,		///< P-IRIS�Զ�
};

enum EIspFocusMode
{
	ISPFOCUSMODE_MANUAL			= 0x0001,		///< �ֶ��۽�
	ISPFOCUSMODE_CONTINU_AUTO	= 0x0002,		///< �����Զ��۽�
	ISPFOCUSMODE_SINGLE_AUTO	= 0x0004,		///< �����Զ��۽�
	ISPFOCUSMODE_LOCK			= 0x0008,		///< ��ͷ����
};

enum EIspFocusMin
{
	ISPFOCUSMIN_30CM			= 0x0001,		///< 30cm
	ISPFOCUSMIN_1M				= 0x0002,		///< 1m
	ISPFOCUSMIN_2M				= 0x0004,		///< 2m
	ISPFOCUSMIN_5M				= 0x0008,		///< 5m
	ISPFOCUSMIN_10M				= 0x0010,		///< 10m
	ISPFOCUSMIN_20M				= 0x0020,		///< 20m
};	

enum EIspShutterMode
{
	ISPSHUTTERMODE_MANUAL		= 0x0001,		///< �ֶ�����ģʽ
	ISPSHUTTERMODE_AUTO			= 0x0002,		///< �Զ�����ģʽ
};

enum EIspShutterOpt
{
	ISPSHUTTEROPT_1_1			= 0x00000001,
	ISPSHUTTEROPT_1_2			= 0x00000002,
	ISPSHUTTEROPT_1_7_5			= 0x00000004,
	ISPSHUTTEROPT_1_10			= 0x00000008,
	ISPSHUTTEROPT_1_15			= 0x00000010,
	ISPSHUTTEROPT_1_20			= 0x00000020,
	ISPSHUTTEROPT_1_25			= 0x00000040,
	ISPSHUTTEROPT_1_30			= 0x00000080,
	ISPSHUTTEROPT_1_50			= 0x00000100,
	ISPSHUTTEROPT_1_60			= 0x00000200,
	ISPSHUTTEROPT_1_100			= 0x00000400,
	ISPSHUTTEROPT_1_120			= 0x00000800,
	ISPSHUTTEROPT_1_150			= 0x00001000,
	ISPSHUTTEROPT_1_200			= 0x00002000,
	ISPSHUTTEROPT_1_240			= 0x00004000,
	ISPSHUTTEROPT_1_300			= 0x00008000,
	ISPSHUTTEROPT_1_400			= 0x00010000,
	ISPSHUTTEROPT_1_480			= 0x00020000,
	ISPSHUTTEROPT_1_960			= 0x00040000,
	ISPSHUTTEROPT_1_1024		= 0x00080000,
	ISPSHUTTEROPT_1_1250		= 0x00100000,
	ISPSHUTTEROPT_1_1500		= 0x00200000,
	ISPSHUTTEROPT_1_2000		= 0x00400000,
	ISPSHUTTEROPT_1_3000		= 0x00800000,
	ISPSHUTTEROPT_1_4000		= 0x01000000,
	ISPSHUTTEROPT_1_8000		= 0x02000000,
	ISPSHUTTEROPT_1_16000		= 0x04000000,
	ISPSHUTTEROPT_1_30000		= 0x08000000,
	ISPSHUTTEROPT_1_100000		= 0x10000000,
	ISPSHUTTEROPT_1_124000		= 0x20000000,
};

enum EIspFreqMode
{
	ISPFREQMODE_AUTO		= 0x0001,		///< ��Ȼ��
	ISPFREQMODE_50HZ		= 0x0002,		///< 50hz
	ISPFREQMODE_60HZ		= 0x0004,		///< 60hz
};

enum EIspWhiteBalance
{
	WHITEBALANCE_MANUAL				= 0x0001,		///< �ֶ���ƽ��
	WHITEBALANCE_AUTO1				= 0x0002,		///< �Զ���ƽ��1
	WHITEBALANCE_AUTO2				= 0x0004,		///< �Զ���ƽ��2
	WHITEBALANCE_LOCK				= 0x0008,		///< ������ƽ��
	WHITEBALANCE_FLUORESCENT		= 0x0010,		///< �չ��ģʽ
	WHITEBALANCE_INCANDESCENT		= 0x0020,		///< �׳��ģʽ
	WHITEBALANCE_WARMLIGHT			= 0x0040,		///< ů���ģʽ
	WHITEBALANCE_SUNLIGHT			= 0x0080,		///< ��Ȼ��ģʽ
	WHITEBALANCE_COLDLIGHT			= 0x0100,		///< ����ģʽ
	WHITEBALANCE_OUTDOORDAYLIGHT	= 0x0200,		///< �������ģʽ
	WHITEBALANCE_OUTDOORCLOUDY		= 0x0400,		///< �������ģʽ
	WHITEBALANCE_OUTDOORNIGHT		= 0x0800,		///< ����ҹ��ģʽ
	WHITEBALANCE_SHADOW				= 0x1000,		///< ��Ӱģʽ
};

enum EIspDayNightSwitch
{
	ISPDAYNIGHTSWITCH_DAY			= 0x0001,		///< ����ģʽ
	ISPDAYNIGHTSWITCH_NIGHT			= 0x0002,		///< ҹ��ģʽ
	ISPDAYNIGHTSWITCH_AUTO_GAIN		= 0x0004,		///< �����Զ�ģʽ
	ISPDAYNIGHTSWITCH_TIME			= 0x0008,		///< ��ʱģʽ
	ISPDAYNIGHTSWITCH_TRIGGER		= 0x0010,		///< �澯����ģʽ
	ISPDAYNIGHTSWITCH_AUTO_LADR		= 0x0020,		///< �����Զ�
};

enum EIspDenoiseMode
{
	ISPDENOISEMODE_CLOSE		= 0x0001,		///< �ر�
	ISPDENOISEMODE_OPEN			= 0x0002,		///< ����
	ISPDENOISEMODE_AUTO			= 0x0004,		///< �Զ�
};

enum EIspDynamicRegulateMode
{
	ISPDYNAMICREGULATEMODE_CLOSE		= 0x0001,		///< �ر�
	ISPDYNAMICREGULATEMODE_AUTO			= 0x0002,		///< �Զ����ⲹ��ģʽ
	ISPDYNAMICREGULATEMODE_SLC			= 0x0004,		///< ǿ������ģʽ
	ISPDYNAMICREGULATEMODE_REGION		= 0x0008,		///< �ֶ����ⲹ��ģʽ
	ISPDYNAMICREGULATEMODE_WDR			= 0x0010,		///< ��̬
	ISPDYNAMICREGULATEMODE_SMARTIR		= 0x0020,		///< 
};

enum EIspBlcRegionType
{
	ISPBLCREGIONTYPE_UP						= 0x0001,		///< ������
	ISPBLCREGIONTYPE_DOWN					= 0x0002,		///< ������
	ISPBLCREGIONTYPE_LEFT					= 0x0004,		///< ������
	ISPBLCREGIONTYPE_RIGHT					= 0x0008,		///< ������
	ISPBLCREGIONTYPE_CENTER					= 0x0010,		///< ������
	ISPBLCREGIONTYPE_CUSTOM					= 0x0020,		///< �Զ���
};

enum EIspImageEnhanceMode
{
	ISPIMAGEENHANCEMODE_CLOSE			= 0x0001,		///< �ر�
	ISPIMAGEENHANCEMODE_ADAPTIVE		= 0x0002,		///< ����Ӧgamma
	ISPIMAGEENHANCEMODE_DEHAZE			= 0x0004,		///< ����ȥ��
	ISPIMAGEENHANCEMODE_WDR				= 0x0008,		///< ���ֿ�̬
	ISPIMAGEENHANCEMODE_DEHAZE_OPTICS	= 0x0010,		///< ��ѧ͸��
};

enum EIspBasicMode
{
	ISPBASICMODE_CLOSE			= 0x0001,		///< close
	ISPBASICMODE_OPEN			= 0x0002,		///< open
	ISPBASICMODE_AUTO			= 0x0004,		///< auto
};

enum EIspCombinHdrMode
{
	ISPCOMBINHDRMODE_CLOSE			= 0x0001,		///< �ر�
	ISPCOMBINHDRMODE_MODE1			= 0x0002,		///< һ֡�ϳɿ�̬
	ISPCOMBINHDRMODE_MODE2			= 0x0004,		///< ��֡�ϳɿ�̬
	ISPCOMBINHDRMODE_MODE3			= 0x0008,		///< ��֡�ϳɿ�̬
};

enum EIspSceneMode
{
	ISPSCENEMODE_STANDARD			= 0x0001,		///< ��׼����ģʽ
	ISPSCENEMODE_COURT				= 0x0002,		///< ��Ժ����ģʽ
	ISPSCENEMODE_DYNAMIC			= 0x0004,		///< ��̬ģʽ
	ISPSCENEMODE_USER_SAVE			= 0x0008,		///< �������Ϊģʽ
	ISPSCENEMODE_USER_LOAD			= 0x0010,		///< �û��Զ���ģʽ
	ISPSCENEMODE_USER_DEL			= 0x0020,		///< ����ɾ��
	ISPSCENEMODE_VEHICLE			= 0x0040,		///< ����ģʽ
	ISPSCENEMODE_SPEED_20_30		= 0x0080,		///< ����ģʽ20-30km
	WHITEBALANCE_SPEED_30_40		= 0x0100,		///< ����ģʽ30-40km
	WHITEBALANCE_SPEED_40_50		= 0x0200,		///< ����ģʽ40-50km
	WHITEBALANCE_SPEED_50_60		= 0x0400,		///< ����ģʽ50-60km
	WHITEBALANCE_BLC				= 0x0800,		///< ���ⳡ��ģʽ
	WHITEBALANCE_FACE				= 0x1000,		///< ��������ģʽ
};

enum EIspSceneDynamicType
{
	ISPSCENEDYNAMICTYPE_DAYNIGHT				= 0x0001,		///< ��ҹת��ģʽ
	ISPSCENEDYNAMICTYPE_TIMELIST				= 0x0002,		///< ʱ���
};

enum EIspFlipMode
{
	ISPFLIPMODE_AUTO					= 0x0001,		///< �Զ�
	ISPFLIPMODE_MANUAL					= 0x0002,		///< �ֶ�
};

enum EIspMirrorMode
{
	ISPMIRRORMODE_CLOSE					= 0x0001,		///< �����
	ISPMIRRORMODE_UP_DOWN				= 0x0002,		///< ����
	ISPMIRRORMODE_LEFT_RIGHT			= 0x0004,		///< ����
	ISPMIRRORMODE_CENTER				= 0x0008,		///< ����
};

enum EIspRotateMode
{
	ISPROTATEMODE_NON			= 0x0001,		///< ����ת
	ISPROTATEMODE_RIGHT			= 0x0002,		///< ��ת
	ISPROTATEMODE_LEFT			= 0x0004,		///< ��ת
};

enum EIspPlayBackMode
{
	ISPPLAYBACKMODE_CLOSE			= 0x0001,		///< �ر�
	ISPPLAYBACKMODE_CVBS_PAL		= 0x0002,		///< ����CVBS P��
	ISPPLAYBACKMODE_CVBS_NTSC		= 0x0004,		///< ����CVBS N��
	ISPPLAYBACKMODE_HDSDI_P			= 0x0008,		///< ����SDI ����
	ISPPLAYBACKMODE_HDSDI_I			= 0x0010,		///< ����SDI ����
	ISPPLAYBACKMODE_HDMI			= 0x0020,		///< ����HDMI
};

enum EIspInfaredMode
{
	ISPINFAREDMODE_OPEN				= 0x0001,		///< �Զ�����
	ISPINFAREDMODE_MORE_NEAR		= 0x0002,		///< ������ģʽ
	ISPINFAREDMODE_NEAR				= 0x0004,		///< ����ģʽ
	ISPINFAREDMODE_MID				= 0x0008,		///< �е�ģʽ
	ISPINFAREDMODE_FAR				= 0x0010,		///< Զ��ģʽ
	ISPINFAREDMODE_CLOSE			= 0x0020,		///< �ر�
	ISPINFAREDMODE_MANUAL			= 0x0040,		///< �ֶ�ģʽ
};

enum EIspLaserMode
{
	ISPLASERMODE_DEFAULT				= 0x0001,		///< Ĭ��ģʽ
	ISPLASERMODE_SMALL					= 0x0002,		///< ���Сģʽ
	ISPLASERMODE_LARGE					= 0x0004,		///< ��ߴ�ģʽ
	ISPLASERMODE_AUTO					= 0x0008,		///< ����Զ�
	ISPLASERMODE_MANUAL					= 0x0010,		///< ����ֶ�
};

enum EIspLaserCentradMode
{
	ISPLASERCENTRADMODE_AUTO				= 0x0001,		///< �Զ�
	ISPLASERCENTRADMODE_MANUAL				= 0x0002,		///< �ֶ�
};

enum EDecMode
{
	DECMODE_REALTIME = 0,  ///< ʵʱ
	DECMODE_BALANCE  = 1,  ///< ����
	DECMODE_SMOOTH   = 2,  ///< ����
};

enum ENetMode
{
	NETMODE_MULTIADDR    = 0x01,   ///< ��ַ�趨
	NETMODE_FAULTTOL     = 0x02,   ///< �����ݴ�
	NETMODE_LOADBAL      = 0x04,   ///< ���ؾ���
};

enum ENetCardSpeed
{
	NETCARDSPEED_AUTO    = 0x01,   ///< ����Ӧ
	NETCARDSPEED_10M     = 0x02,   ///< 10M
	NETCARDSPEED_100M    = 0x04,   ///< 100M
	NETCARDSPEED_1000M   = 0x08,   ///< 1000M
};

enum EUPNPMODE
{
	UPNPMODE_AUTO   = 0x01, ///< �Զ�
	UPNPMODE_MANUAL = 0x02, ///< �ֶ�
};

enum EDDNSType
{
	DDNSTYPE_OARY			= 0x01, ///< ������
	DDNSTYPE_DYNDNS			= 0x02, ///< DynDNS
	DDNSTYPE_DYNIP			= 0x04, ///< DynIP
	DDNSTYPE_NOIP			= 0x08, ///< noip
	DDNSTYPE_CHANGEIP		= 0x10, ///< ChangeIP
	DDNSTYPE_EASYDNS		= 0x20, ///< easyDNS
	DDNSTYPE_CAMTRONICS		= 0x40, ///< CAMTRONICS
};

enum EPORTTYPE
{
	PORTTYPE_HTTP = 0, ///< HTTP�˿�
	PORTTYPE_RTSP = 1, ///< RTSP�˿�
};

enum EWifiMode
{
	WIFIMODE_AP			= 0,	///< APģʽ
	WIFIMODE_WLAN		= 1,	///< WLANģʽ
};

enum EAuthenType
{
	AUTHENTYPE_MD5			= 0x01,   ///< md5
	AUTHENTYPE_SHA			= 0x02,   ///< sha
};

enum EKeyAlgType
{
	KEYALGTYPE_DES			= 0x01,   ///< des
	KEYALGTYPE_AES			= 0x02,   ///< aes
};

enum ESnmpVersion
{
	SNMPVERSION_V1V2			= 0x01,   ///< v1&v2
	SNMPVERSION_V3				= 0x02,   ///< v3
};

enum EAutoLogoutTime
{
	AUTOLOGOUTTIME_NEVER = 0x01,   ///< �Ӳ�
	AUTOLOGOUTTIME_1MIN  = 0x02,   ///< 1����
	AUTOLOGOUTTIME_2MIN  = 0x04,   ///< 2����
	AUTOLOGOUTTIME_5MIN  = 0x08,   ///< 5����
	AUTOLOGOUTTIME_10MIN = 0x10,   ///< 10����
	AUTOLOGOUTTIME_20MIN = 0x20,   ///< 20����
	AUTOLOGOUTTIME_30MIN = 0x40,   ///< 30����
};

enum EAudioListen
{
	AUDIOLISTEN_NONE = 0,	///< ��Ƶ������
	AUDIOLISTEN_CHN1 = 1,	///< ��Ƶ����ͨ��1
	AUDIOLISTEN_CHN2 = 2,	///< ��Ƶ����ͨ��2
	AUDIOLISTEN_ALL			///< ��Ƶ����ȫ��
};

enum EFrameRateType
{
	FRAMERATETYPE_1FPS   = 0x0001,   ///< 1fps
	FRAMERATETYPE_5FPS   = 0x0002,   ///< 5fps
	FRAMERATETYPE_6FPS   = 0x0004,   ///< 6fps
	FRAMERATETYPE_8FPS   = 0x0008,   ///< 8fps
	FRAMERATETYPE_10FPS  = 0x0010,   ///< 10fps
	FRAMERATETYPE_15FPS  = 0x0020,   ///< 15fps
	FRAMERATETYPE_20FPS  = 0x0040,   ///< 20fps
	FRAMERATETYPE_25FPS  = 0x0080,   ///< 25fps
	FRAMERATETYPE_30FPS  = 0x0100,   ///< 30fps
};

enum EBitRateType
{
	BITRATETYPE_256K   = 0x0001,   ///< 256Kbps
	BITRATETYPE_512K   = 0x0002,   ///< 512Kbps
	BITRATETYPE_1M     = 0x0004,   ///< 1Mbps
	BITRATETYPE_2M     = 0x0008,   ///< 2Mbps
	BITRATETYPE_4M     = 0x0010,   ///< 4Mbps
};

enum EZeroChnVideoSrcType
{
	ZEROCHNVIDEOSRCTYPE_HDMI   = 0x0001,   ///< HDMI
	ZEROCHNVIDEOSRCTYPE_VGA    = 0x0002,   ///< VGA
	ZEROCHNVIDEOSRCTYPE_HDMI2  = 0x0004,   ///< HDMI2
};

enum EUserLevel
{
	USERLEVEL_ADMIN		= 0, ///< ����Ա
	USERLEVEL_OPERATOR	= 1, ///< ����Ա
	USERLEVEL_VIEWER	= 2, ///< �����
};

enum EUserInfoType
{
	USERINFOTYPE_BASEINFO		   = 0x01, ///< ������Ϣ
	USERINFOTYPE_REMOTECTRL        = 0x02, ///< Զ�̿���
	USERINFOTYPE_SYSPERM           = 0x04, ///< ϵͳȨ��
	USERINFOTYPE_CHNPERMLIST       = 0x08, ///< ͨ��Ȩ��
};

enum ESysPermType
{
	SYSPERMTYPE_ADMIN              = 0x0001, ///< adminȨ��
	SYSPERMTYPE_RECSNAP            = 0x0002, ///< ¼��ץ��Ȩ��
	SYSPERMTYPE_CHNMGR             = 0x0004, ///< ͨ������Ȩ��
	SYSPERMTYPE_NETMGR             = 0x0008, ///< �������Ȩ��
	SYSPERMTYPE_SYSMGR             = 0x0010, ///< ϵͳ����Ȩ��
	SYSPERMTYPE_DISKMGR            = 0x0020, ///< Ӳ�̹���Ȩ��
	SYSPERMTYPE_GUIPLANMGR         = 0x0040, ///< GUIԤ������Ȩ��
	SYSPERMTYPE_SHUTDOWN           = 0x0080, ///< �ػ���������Ȩ��
	SYSPERMTYPE_SCENARIO           = 0x0100, ///< Ӧ�ó�������Ȩ��
	SYSPERMTYPE_CAMERA			   = 0x0200, ///< ���������Ȩ��
	SYSPERMTYPE_EVENT			   = 0x0400, ///< �¼�����Ȩ��
	SYSPERMTYPE_INTELL			   = 0x0800, ///< ���ܹ���Ȩ��
};

enum EChnPermType
{
	CHNPERMTYPE_VIEW              = 0x01, ///< ���Ȩ��
	CHNPERMTYPE_LT                = 0x02, ///< �����Խ�Ȩ��
	CHNPERMTYPE_PB                = 0x04, ///< �ط�Ȩ��
	CHNPERMTYPE_BD                = 0x08, ///< ��������Ȩ��
	CHNPERMTYPE_IS                = 0x10, ///< ��������Ȩ��
	CHNPERMTYPE_PTZ               = 0x20, ///< ��̨Ȩ��
};

enum ERtspAuthType
{
	RTSPAUTHTYPE_NONE		= 0x01, ///< none:�����Ȩ
	RTSPAUTHTYPE_BASIC   	= 0x02, ///< basic:BASIC��ʽ
	RTSPAUTHTYPE_DIGEST 	= 0x04, ///< digest:ժҪ��֤��ʽ
};

enum EIpAddrFilterType
{
	IPADDRFILTERTYPE_WHITE		= 0x01, ///< ����������
	IPADDRFILTERTYPE_BLACK   	= 0x02, ///< ����������
};

enum EVideoEncType
{
	VIDEOENCTYPE_H264          = 0x01, ///< h264
	VIDEOENCTYPE_H265          = 0x02, ///< h265
	VIDEOENCTYPE_MPEG4         = 0x04, ///< mpeg4
	VIDEOENCTYPE_MJPEG         = 0x08, ///< mjpeg
	VIDEOENCTYPE_SVAC          = 0x10, ///< svac
};

enum ERcModeType
{
	RCMODETYPE_VBR                = 0x01, ///< vbr
	RCMODETYPE_CBR                = 0x02, ///< cbr
};

enum EImgQualityType
{
	IMGQUALITYTYPE_LOWEST         = 0x01, ///< lowest
	IMGQUALITYTYPE_LOWER          = 0x02, ///< lower
	IMGQUALITYTYPE_LOW            = 0x04, ///< low
	IMGQUALITYTYPE_MID            = 0x08, ///< mid
	IMGQUALITYTYPE_BETTER         = 0x10, ///< better
	IMGQUALITYTYPE_BEST           = 0x20, ///< best
};

enum EVideoEncProfileType
{
	VIDEOENCPROFILETYPE_BASELINE  = 0x01, ///< baseline
	VIDEOENCPROFILETYPE_MAIN      = 0x02, ///< main
	VIDEOENCPROFILETYPE_HIGH      = 0x04, ///< high
	VIDEOENCPROFILETYPE_SP        = 0x08, ///< sp
	VIDEOENCPROFILETYPE_ASP       = 0x10, ///< asp
	VIDEOENCPROFILETYPE_LOW		  = 0x20, ///< low
	VIDEOENCPROFILETYPE_MID		  = 0x40, ///< middle
};

enum EAudioInType
{
	AUDIOINTYPE_LINEIN            = 0x01, ///< LINE-IN
	AUDIOINTYPE_MICIN             = 0x02, ///< MIC-IN
};

enum ECordonType
{
	CORDONTYPE_ATOB   = 0x01, ///< �����߷���A->B
	CORDONTYPE_BTOA   = 0x02, ///< �����߷���B->A
	CORDONTYPE_AANDB  = 0x04, ///< �����߷���A<->B
};

enum ECheckMode
{
	CHECKMODE_MEDIA  = 0x01, ///< ������
	CHECKMODE_MAG    = 0x02, ///< Ӳ�����
};

enum ENetLinkSate
{
	NETLINK_CONNECT			     = 1,
	NETLINK_UNCONNECT			 = 2,
	NETLINK_UNKNOW				 = 3,
};

enum EAudioEncType
{
	AUDIOENCTYPE_G711             = 0x01, ///< G711
	AUDIOENCTYPE_PCMA             = 0x02, ///< G711a
	AUDIOENCTYPE_PCMU             = 0x04, ///< G711u
	AUDIOENCTYPE_G7221C           = 0x08, ///< G722.1c
	AUDIOENCTYPE_ADPCM            = 0x10, ///< ADPCM
	AUDIOENCTYPE_G722             = 0x20, ///< G722
	AUDIOENCTYPE_AACLC            = 0x40, ///< AACLC
	AUDIOENCTYPE_G726             = 0x80, ///< G726
};

enum ERecBlockSize
{
	RECBLOCKSIZE_128M             = 0x01, ///< 128M
	RECBLOCKSIZE_256M             = 0x02, ///< 256M
	RECBLOCKSIZE_512M             = 0x04, ///< 512M
	RECBLOCKSIZE_1024M            = 0x08, ///< 1024M
};

enum EAudioSampleRateType
{
	AUDIOSAMPLERATETYPE_8K        = 0x01, ///< 8K
	AUDIOSAMPLERATETYPE_16K       = 0x02, ///< 16K
	AUDIOSAMPLERATETYPE_32K       = 0x04, ///< 32K
	AUDIOSAMPLERATETYPE_44K       = 0x08, ///< 44K
	AUDIOSAMPLERATETYPE_48K       = 0x10, ///< 48K
	AUDIOSAMPLERATETYPE_64K       = 0x20, ///< 64K
};

enum ERecModeType
{
	RECMODETYPR_AUTO              = 0x01, ///< ���ƻ����¼�
	RECMODETYPR_START             = 0x02, ///< ʼ�տ���
	RECMODETYPR_STOP              = 0x04, ///< ʼ�չر�
};

enum ESnapModeType
{
	SNAPMODETYPE_AUTO             = 0x01,  ///< �Զ������ƻ����¼���
	SNAPMODETYPE_STOP             = 0x02,  ///< ֹͣ
};

enum ERecStreamType
{
	RECSTREAMTYPE_MAIN            = 0x01, ///< ����
	RECSTREAMTYPE_2ND             = 0x02, ///< ����
	RECSTREAMTYPE_3RD             = 0x04, ///< ����·��
};

enum ENetPackFilter
{
	NETPACK_NOFILTER  = 0, ///< ȫ��
	NETPACK_TCPFILTER = 1, ///< TCP��
	NETPACK_UDPFILTER = 2, ///< UDP��
};

enum ERecDayType
{
	RECDAYTYPE_MONDAY       = 0, ///< ��һ
	RECDAYTYPE_TUESDAY      = 1, ///< �ܶ�
	RECDAYTYPE_WEDNESDAY    = 2, ///< ����
	RECDAYTYPE_THURSDAY     = 3, ///< ����
	RECDAYTYPE_FRIDAY       = 4, ///< ����
	RECDAYTYPE_SATURDAY     = 5, ///< ����
	RECDAYTYPE_SUNDAY       = 6, ///< ����
	RECDAYTYPE_HOLIDAY      = 7, ///< ����
};

enum ERecDownloadSpeed
{
	RECDOWNLOADSPEED_128KB   = 0x01,   ///< 128KB
	RECDOWNLOADSPEED_256KB   = 0x02,   ///< 256KB
	RECDOWNLOADSPEED_512KB   = 0x04,   ///< 512KB
	RECDOWNLOADSPEED_1024KB  = 0x08,   ///< 1024KB
};

enum EHolidaySelMode
{
	HOLIDAYSELMODE_DAY    = 0x01,   ///< ��
	HOLIDAYSELMODE_WEEK   = 0x02,   ///< ��
	HOLIDAYSELMODE_MON    = 0x04,   ///< ��
};

enum EPlaybackType
{
	PLAYBACKTYPE_EVENTINDEX   = 1,  ///< event_index:�¼������ط�
	PLAYBACKTYPE_LABELINDEX   = 2,  ///< label_index:��ǩ�����ط�
	PLAYBACKTYPE_LOCKED       = 3,  ///< locked:����¼��ط�
	PLAYBACKTYPE_IMAGEINDEX   = 4,  ///< image_index:ͼƬ�����ط�
	PLAYBACKTYPE_EXTERNALFILE = 5,  ///< external_file:�ⲿ�ļ������ط�
	PLAYBACKTYPE_TIMESCAL     = 6,  ///< time_scale:ʱ��λط�
	PLAYBACKTYPE_BYFILE       = 7,  ///< by_file:���ļ��ط�
	PLAYBACKTYPE_BYTIME       = 8,  ///< by_time:��ʱ��ط�
};

enum ERecEventType
{
	RECEVENTTYPE_RECORD           = 0x00000000, ///< ��¼,���¼�
	RECEVENTTYPE_ALL              = 0x00000001, ///< all_event,ȫ���¼�
	RECEVENTTYPE_PIN              = 0x00000002, ///< pin,���ڸ澯����������
	RECEVENTTYPE_MOTIVE           = 0x00000004, ///< motive,�ƶ����澯
	//RECEVENTTYPE_EXTERN           = 0x00000008, ///< extern,�ⲿ�澯¼��,����������ѯʱ������ [10/28/2016 ����]
	RECEVENTTYPE_MANUAL           = 0x00000010, ///< manual,�ֶ�¼��
	RECEVENTTYPE_TIMER            = 0x00000020, ///< timer,��ʱ¼��
	//RECEVENTTYPE_WEEKLY           = 0x00000040, ///< weekly,����¼��,����������ѯʱ������ [10/28/2016 ����]
	//RECEVENTTYPE_FILE             = 0x00000080, ///< file,����ʱ����ļ�¼��,����������ѯʱ������ [10/28/2016 ����]
	RECEVENTTYPE_INTEDETECT       = 0x10000000, ///< ������⣬����Ӧ�����ѯ���ͣ�ֻ��������֮һ
	RECEVENTTYPE_COVERIMG         = 0x00000100, ///< cover_image,ͼ���ڵ��澯
	RECEVENTTYPE_TRIPLINE         = 0x00000200, ///< trip_line,���߼��
	RECEVENTTYPE_DEFOCUS          = 0x00000400, ///< defocus,�齹���
	RECEVENTTYPE_SCENECHANGE      = 0x00000800, ///< scene_change,�����任
	RECEVENTTYPE_REGIONINVASION   = 0x00001000, ///< region_invasion,�������ּ��
	RECEVENTTYPE_REGIONLEAVING    = 0x00002000, ///< region_leaving,�����뿪���
	RECEVENTTYPE_OBJTAKEN         = 0x00004000, ///< object_taken,��Ʒ��ȥ���
	RECEVENTTYPE_OBJLEFT          = 0x00008000, ///< object_left,��Ʒ�������
	RECEVENTTYPE_PEOPLEGATHER     = 0x00010000, ///< people_gather,��Ա�ۼ����
	RECEVENTTYPE_AUDIOABNORMAL    = 0x00020000, ///< audio_abnormal,�����쳣
	RECEVENTTYPE_DETECTIN         = 0x00040000, ///< region_enter,�������
	RECEVENTTYPE_FACEDETECTION    = 0x00080000, ///< face_detect,�������
	RECEVENTTYPE_IMPPERSON		  = 0x00100000, ///< imp_person,�ص���Ա
	RECEVENTTYPE_STRANGEPERSON	  = 0x00200000, ///< strange_person,İ����Ա
	RECEVENTTYPE_PERSONDETECT	  = 0x00400000, ///< person_detect,���˼��
	RECEVENTTYPE_ACCESSPROTECT	  = 0x00800000, ///< access_protect,���𱨾�
	RECEVENTTYPE_LOCKED           = 0x20000000, ///< locked,����¼���¼�
	RECEVENTTYPE_UNLOCKED         = 0x40000000, ///< unlocked,δ����¼���¼�
};

enum ERecBackupType
{
	RECBACKUPTYPE_BACKUP    = 1,   ///< ¼�񱸷�,rec_backup
	RECBACKUPTYPE_CLIP      = 2,   ///< ¼�����,rec_clip
};

enum EBackupState
{
	BACKUPSTATE_IDLE    = 1,   ///< ���У�idle
	BACKUPSTATE_RUN     = 2,   ///< ���У�run
	BACKUPSTATE_FAIL    = 3,   ///< ʧ�ܣ�fail
	BACKUPSTATE_DONE    = 4,   ///< ��ɣ�done
	BACKUPSTATE_UNKNOWN = 5,   ///< ��ȡ����״̬ʧ�ܣ�unknown
};

enum EPlaybackVcrCmdType
{
	PLAYBACKVCRCMDTYPE_NORMAL      = 1,  ///< normal:���ٲ���
	PLAYBACKVCRCMDTYPE_PAUSE       = 2,  ///< pause:��ͣ
	PLAYBACKVCRCMDTYPE_RESUME      = 3,  ///< resume:�ָ�����
	PLAYBACKVCRCMDTYPE_FAST2X      = 4,  ///< 2xfast:2���ٿ��
	PLAYBACKVCRCMDTYPE_FAST4X      = 5,  ///< 4xfast:4���ٿ��
	PLAYBACKVCRCMDTYPE_FAST8X      = 6,  ///< 8xfast:8���ٿ��
	PLAYBACKVCRCMDTYPE_FAST16X     = 7,  ///< 16xfast:16���ٿ��
	PLAYBACKVCRCMDTYPE_FAST32X     = 8,  ///< 32xfast:16���ٿ��
	PLAYBACKVCRCMDTYPE_FAST64X     = 9,  ///< 64xfast:16���ٿ��
	PLAYBACKVCRCMDTYPE_SLOW2X      = 10, ///< 2xslow:1/2��������
	PLAYBACKVCRCMDTYPE_SLOW4X      = 11, ///< 4xslow:1/4��������
	PLAYBACKVCRCMDTYPE_SLOW8X      = 12, ///< 8xslow:1/8��������
	PLAYBACKVCRCMDTYPE_SLOW16X     = 13, ///< 16xslow:16��������
	PLAYBACKVCRCMDTYPE_FRAME       = 14, ///< frame:��֡����
	PLAYBACKVCRCMDTYPE_DRAG        = 15, ///< drag:ʱ������ק
	PLAYBACKVCRCMDTYPE_SKIPF       = 16, ///< skipf:ǰ��
	PLAYBACKVCRCMDTYPE_SKIPB       = 17, ///< skipb:����
};

enum EPlaybackFrameMode
{
	PLAYBACKFRAMEMODE_ALL          = 1, ///< �����������͵�֡���� I/P/B֡
	PLAYBACKFRAMEMODE_INTRA        = 2, ///< ֻ���͹ؼ�֡ I֡
	PLAYBACKFRAMEMODE_PREDICTED    = 3, ///< ֻ���͹ؼ�֡��Ԥ��֡ I/P֡
};

enum EPlaybackVcrState
{
	PLAYBACKSTATE_PLAY                       = 1, ///< play:���ڲ���
	PLAYBACKSTATE_OVER                       = 2, ///< over:���Ž���
	PLAYBACKSTATE_DISCON                     = 3, ///< discon:����
	PLAYBACKSTATE_NOTSTART                   = 4, ///< notstart:δ��ʼ
	PLAYBACKSTATE_PAUSE                      = 5, ///< pause:������ͣ
	PLAYBACKSTATE_SINGLE                     = 6, ///< single:��֡
	PLAYBACKSTATE_START_FAILED               = 7, ///< allfail:����ȫ��ʧ��
	PLAYBACKSTATE_START_PARTIAL_FAILED       = 8, ///< partfail:���񲿷�ʧ��
	PLAYBACKSTATE_CHN_NO_STREAM              = 9, ///< chn_no_stream:��ǰͨ��û������
};

enum ESnapPicEventType
{
	SNAPPICEVENTTYPE_ALL						= 0x01000000, ///< �������ͣ�all_event
	SNAPPICEVENTTYPE_MANUAL						= 0x02000000, ///< �ֶ�ץ�ģ�manual
	SNAPPICEVENTTYPE_TIMER						= 0x04000000, ///< ��ʱץ�ģ�timer
	SNAPPICEVENTTYPE_MD							= 0x08000000, ///< �ƶ���⣬md
	SNAPPICEVENTTYPE_PIN						= 0x10000000, ///< �澯���룬pin
	SNAPPICEVENTTYPE_INTEDETECT					= 0x20000000, ///< �������
	SNAPPICEVENTTYPE_INTEDETECT_ALL				= 0x20000001, ///< ������⣬ȫ��
	SNAPPICEVENTTYPE_INTEDETECT_TRIPLINE		= 0x20000002, ///< ������⣬�����ߴ�Խ��trip_line
	SNAPPICEVENTTYPE_INTEDETECT_REGIONINVASION	= 0x20000004, ///< ������⣬�������֣�region_invasion
	SNAPPICEVENTTYPE_INTEDETECT_REGINENTER		= 0x20000008, ///< ������⣬��������region_entering
	SNAPPICEVENTTYPE_INTEDETECT_REGIONLEAVE		= 0x20000010, ///< ������⣬�뿪����region_leaving
	SNAPPICEVENTTYPE_INTEDETECT_OBJLEFT			= 0x20000020, ///< ������⣬��Ʒ������object_left
	SNAPPICEVENTTYPE_INTEDETECT_OBJTAKEN		= 0x20000040, ///< ������⣬��Ʒ��ȡ��object_taken
	SNAPPICEVENTTYPE_INTEDETECT_PEOPLEGATHER	= 0x20000080, ///< ������⣬��Ա�ۼ���people_gather
	SNAPPICEVENTTYPE_INTEDETECT_DETECTFACE		= 0x20000100, ///< ������⣬������⣬detect_face
	SNAPPICEVENTTYPE_INTEDETECT_SHADE			= 0x20000200, ///< ������⣬�ڵ���⣬shade
	SNAPPICEVENTTYPE_INTEDETECT_DEFOCUS			= 0x20000400, ///< ������⣬�齹��⣬defocus
	SNAPPICEVENTTYPE_INTEDETECT_SCENECHANGE		= 0x20000800, ///< ������⣬�����仯��scene_change
	SNAPPICEVENTTYPE_INTEDETECT_AUDIOABNORMAL	= 0x20001000, ///< ������⣬��Ƶ���죬audio_abnormal
	SNAPPICEVENTTYPE_INTEDETECT_IMPPERSON		= 0x20002000, ///< ������⣬�ص���Ա��imp_person
	SNAPPICEVENTTYPE_INTEDETECT_STRANGEPERSON	= 0x20004000, ///< ������⣬İ����Ա��strange_person
	SNAPPICEVENTTYPE_INTEDETECT_PERSONDETECT	= 0x20008000, ///< ������⣬���˼�⣬person_detect
	SNAPPICEVENTTYPE_INTEDETECT_ACCESSPROTECT	= 0x20010000, ///< ������⣬���𱨾���access_protect
};

enum ERecTagOperaType
{
	TAGOPERATE_ADD       = 0x0001, ///< ���¼���ǩ
	TAGOPERATE_DEL       = 0x0002, ///< ɾ��¼���ǩ
	TAGOPERATE_UPDATE    = 0x0004, ///< ����¼���ǩ
};

enum EDisplayResolution
{
	DISPLAYRESOLUTION_1024X768       = 0x0001, ///< 1024*768
	DISPLAYRESOLUTION_1280X720       = 0x0002, ///< 1280*720
	DISPLAYRESOLUTION_1280X800       = 0x0004, ///< 1280*800
	DISPLAYRESOLUTION_1280X960       = 0x0008, ///< 1280*960
	DISPLAYRESOLUTION_1280X1024      = 0x0010, ///< 1280*1024
	DISPLAYRESOLUTION_1366X768       = 0x0020, ///< 1366*768
	DISPLAYRESOLUTION_1440X900       = 0x0040, ///< 1440*900
	DISPLAYRESOLUTION_1600X1200      = 0x0080, ///< 1600*1200
	DISPLAYRESOLUTION_1680X1050      = 0x0100, ///< 1680*1050
	DISPLAYRESOLUTION_1920X1080      = 0x0200, ///< 1920*1080
	DISPLAYRESOLUTION_3840X2160      = 0x0400, ///< 3840*2160
};

enum EDisplayFreshRate
{
	DISPLAYFRESHRATE_30HZ              = 0x01, ///< 30Hz
	DISPLAYFRESHRATE_50HZ              = 0x02, ///< 50Hz
	DISPLAYFRESHRATE_60HZ              = 0x04, ///< 60Hz
};

enum EOutputType
{
	OUTPUTTYPE_HDMI   = 0x0001, ///< �������HDMI
	OUTPUTTYPE_HDMI2  = 0x0002, ///< �������HDMI2
	OUTPUTTYPE_VGA    = 0x0004, ///< �������VGA
	OUTPUTTYPE_CVBS   = 0x0010, ///< �������CVBS
	OUTPUTTYPE_BT1120 = 0x0020, ///< �������BT1120
};

enum ESerialType
{
	SERIALTYPE_RS232 = 1,    ///< RS232
	SERIALTYPE_RS485 = 2,    ///< RS485
};

enum ESerialBaudRate
{
	SERIALBAUDRATE_2400   = 0x00000001,  ///< 2400, 2400������
	SERIALBAUDRATE_4800   = 0x00000002,  ///< 4800, 4800������
	SERIALBAUDRATE_9600   = 0x00000004,  ///< 9600, 9600������
	SERIALBAUDRATE_19200  = 0x00000008,  ///< 19200, 19200������
	SERIALBAUDRATE_38400  = 0x00000010,  ///< 38400, 38400������
	SERIALBAUDRATE_57600  = 0x00000020,  ///< 57600, 57600������
	SERIALBAUDRATE_115200 = 0x00000040,  ///< 115200, 115200������
};

enum ESerialDataBit
{
	SERIALDATABIT_5 = 0x00000001,  ///< databit5, ����λ: 5λ
	SERIALDATABIT_6 = 0x00000002,  ///< databit6, ����λ: 6λ
	SERIALDATABIT_7 = 0x00000004,  ///< databit7, ����λ: 7λ
	SERIALDATABIT_8 = 0x00000008,  ///< databit8, ����λ: 8λ
};

enum ESerialStopBit
{
	SERIALSTOPBIT_1 = 0x00000001,  ///< stopbit1, ֹͣλ: 1λ
	SERIALSTOPBIT_2 = 0x00000002,  ///< stopbit2, ֹͣλ: 2λ
};

enum ESerialParityBit
{
	SERIALPARITYBIT_NONE = 0x00000001,  ///< none, У��λ: ��У��
	SERIALPARITYBIT_ODD  = 0x00000002,  ///< odd, У��λ: ��У��
	SERIALPARITYBIT_EVEN = 0x00000004,  ///< even, У��λ: żУ��
};

enum ESerialFlowCtrl
{
	SERIALFLOWCTRL_NONE     = 0x00000001,  ///< none, ����: ��
	SERIALFLOWCTRL_SOFT     = 0x00000002,  ///< soft, ����: ������
	SERIALFLOWCTRL_HARDWARE = 0x00000004,  ///< hardware, ����: Ӳ����
};

enum ESerialWorkType
{
	SERIALWORKTYPE_KEYBOARD      = 0x00000001,  ///< keyboard, ���ڹ���: ���Ӳ��
	SERIALWORKTYPE_ALARMEXTCARD  = 0x00000002,  ///< alarmextcard, ���ڹ���: �澯��չ��
	SERIALWORKTYPE_TRANSDATA     = 0x00000004,  ///< transdata, ���ڹ���: ͸������
};

enum EDisMode
{
	DISMODE_STANDARD = 0, ///< ��׼
	DISMODE_BRIGHT   = 1, ///< ����
	DISMODE_SOFT     = 2, ///< ���
	DISMODE_FRESH    = 3, ///< ����
	DISMODE_CUSTOM   = 4, ///< �Զ���
	DISMODE_COUNT
};

enum ERtspTransType
{
	RTSPTRANSTYPE_UDP_UNICAST   = 1, ///< udp_unicast
	RTSPTRANSTYPE_UDP_MULTICAST = 2, ///< udp_multicast
	RTSPTRANSTYPE_TCP           = 3, ///< tcp
	RTSPTRANSTYPE_HTTP          = 4, ///< http
};

enum EDecState
{
	DECSTATE_NORMAL				= 0, ///< ��������
	DECSTATE_DECCAPOVER			= 1, ///< ������������
	DECSTATE_ZOOMCAPOVER		= 2, ///< ������������
	DECSTATE_STREAMPREFAILED	= 3, ///< ȡ��ʧ��
};

enum ELogMainType
{
	LOGMAINTYPE_ALL                                          = 0x10000000, ///< ȫ��
	LOGMAINTYPE_ALARM                                        = 0x20000000, ///< �澯
	LOGMAINTYPE_SYSEXEPTION                                  = 0x40000000, ///< ϵͳ�쳣
	LOGMAINTYPE_USEROPERATE                                  = 0x80000000, ///< �û�����
	LOGMAINTYPE_SYSINFO		                                 = 0x01000000, ///< ϵͳ��Ϣ
};

enum ERealLogMainType
{
	REALLOGMAINTYPE_NVRALARM                                 = 0x21000000, ///< NVR�澯
	REALLOGMAINTYPE_DEVALARM                                 = 0x22000000, ///< ǰ�˸澯
	REALLOGMAINTYPE_SYSEXEPTION                              = 0x40000000, ///< ϵͳ�쳣
	REALLOGMAINTYPE_USEROPERATE                              = 0x80000000, ///< �û�����
	REALLOGMAINTYPE_SYSINFO		                             = 0x00100000, ///< ϵͳ��Ϣ
};

enum EAlarmType
{
	ALARMTYPE_ALARMIN	    = 1, ///< �澯����
	ALARMTYPE_MOVEDETECT    = 2, ///< �ƶ����
	ALARMTYPE_SMARTDETECT   = 3, ///< �������
	ALARMTYPE_VIDEOLOST	    = 4, ///< ��Ƶ��ʧ
	ALARMTYPE_ACCESSPROTECT	= 5, ///< ���𱨾�
	ALARMTYPE_SYSALARM      = 6, ///< ϵͳ�澯
};

enum ESmartAlarmType
{
	SMARTALARMTYPE_WARNINGLINE		= 1,  ///< ������
	SMARTALARMTYPE_AREAINVADE		= 2,  ///< ��������
	SMARTALARMTYPE_ENTERAREA		= 3,  ///< �������
	SMARTALARMTYPE_LEAVEATEA		= 4,  ///< �����뿪
	SMARTALARMTYPE_SHIELDDETECT		= 5,  ///< �ڵ����
	SMARTALARMTYPE_GOODSTAKE		= 6,  ///< ��Ʒ��ȡ
	SMARTALARMTYPE_GOODSLEFT		= 7,  ///< ��Ʒ����
	SMARTALARMTYPE_VIRFOCUS			= 8,  ///< �齹���
	SMARTALARMTYPE_GATHER			= 9,  ///< ��Ա�ۼ�
	SMARTALARMTYPE_SCENECHANGE		= 10, ///< �������
	SMARTALARMTYPE_AUDIOABNORMAL	= 11, ///< �����쳣
	SMARTALARMTYPE_FACEDETECT		= 12, ///< �������
};

enum ESysAlarmType
{
	SYSALARMTYPE_DISKFAULT		= 0x0001, ///< Ӳ�̹���
	SYSALARMTYPE_DISKNONE		= 0x0002, ///< ��Ӳ��
	SYSALARMTYPE_RECFULL		= 0x0004, ///< ¼��ռ���
	SYSALARMTYPE_SNAPFULL		= 0x0008, ///< ץ�Ŀռ���
	SYSALARMTYPE_CHNOFFLINE		= 0x0010, ///< ǰ�˵���
	SYSALARMTYPE_ILLACCESS		= 0x0020, ///< �Ƿ�����
	SYSALARMTYPE_NETFAULT		= 0x0040, ///< �������
	SYSALARMTYPE_IPCONFLICT		= 0x0080, ///< IP��ͻ
	SYSALARMTYPE_MACCONFLICT	= 0x0100, ///< MAC��ͻ
	SYSALARMTYPE_NOREC			= 0x0200, ///< ¼��ʱ������
};

enum EAlarmBasicLink
{
	ALARMBASICLINK_AUDIOALARM = 0x01, ///< �澯������������������
	ALARMBASICLINK_SENDMAIL   = 0x02, ///< �澯���������������ʼ�
	ALARMBASICLINK_UPCENTER   = 0x04, ///< �澯�����������ϱ�����
	ALARMBASICLINK_HDMI1      = 0x08, ///< �澯���������HDMI1
	ALARMBASICLINK_HDMI2      = 0x10, ///< �澯���������HDMI2
	ALARMBASICLINK_VGA1       = 0x20, ///< �澯���������VAG1
	ALARMBASICLINK_VGA2       = 0x40, ///< �澯���������VGA2
	ALARMBASICLINK_CLOUDSRV   = 0x80, ///< �澯����������Ʒ���
};

enum EAlarmLinkPtzType
{
	ALARMLINKPTZTYPE_NONE	= 0, ///< ��
	ALARMLINKPTZTYPE_PRESET	= 1, ///< Ԥ��λ
	ALARMLINKPTZTYPE_CRUISE	= 2, ///< Ѳ��
};

enum EDayType
{
	DAYTYPE_MONDAY		= 1, ///< ��һ
	DAYTYPE_TUESDAY		= 2, ///< �ܶ�
	DAYTYPE_WEDNESDAY	= 3, ///< ����
	DAYTYPE_THURSDAY	= 4, ///< ����
	DAYTYPE_FRIDAY		= 5, ///< ����
	DAYTYPE_SATURDAY	= 6, ///< ����
	DAYTYPE_SUNDAY		= 7, ///< ����
	DAYTYPE_HOLIDAY		= 8, ///< ����
};

enum EAlarmInType
{
	ALARMINTYPE_OPEN  = 1, ///< ����    
	ALARMINTYPE_CLOSE = 2, ///< ����    
};

enum ESmartTestAttrStatus	
{
	SMARTTESTATTRSTATUS_OK	        = 1,			///< OK
	SMARTTESTATTRSTATUS_NOTOK		= 2,			///< NOT_OK
};

enum ESmartTestResultType	
{
	SMARTTESTRESULTTYPE_PASSED	    = 1,			///< ͨ��,״̬����
	SMARTTESTRESULTTYPE_WARNING		= 2,			///< ����,������
	SMARTTESTRESULTTYPE_NOTPASSED	= 3,			///< δͨ��,��
};

enum EBadSectorCheckType	
{
	BADSECTORCHECKTYPE_KEYAREA	    = 1,			///< �ؼ������
	BADSECTORCHECKTYPE_ALLAREA		= 2,			///< ��ȫ���
};

enum EBadSectorCheckState
{
	BADSECTORCHECKSTATE_NOTRUN	    = 1,			///< δ���м��
	BADSECTORCHECKSTATE_RUNNING		= 2,			///< ���ڼ����
	BADSECTORCHECKSTATE_PAUSE		= 3,			///< ��ͣ���
	BADSECTORCHECKSTATE_ERROR		= 4,			///< �������г���
	BADSECTORCHECKSTATE_FINISHE		= 5,			///< ������
};

enum ESipSecurityLev
{
	SIPSECURITYLEV_NO			= 0x01,			///< ������
	SIPSECURITYLEV_CLASS_A		= 0x02,			///< A�����
	SIPSECURITYLEV_CLASS_B		= 0x04,			///< B�����
	SIPSECURITYLEV_CLASS_C		= 0x08,			///< C�����
};

enum EBadSectorCheckCtrlType	
{
	BADSECTORCHECKCTRLTYPE_PAUSE	    = 1,			///< ��ͣ
	BADSECTORCHECKCTRLTYPE_CONTINUE		= 2,			///< ����
};

enum EPppoeStatus
{
	PPPOESTATUS_STOP		= 1, ///< ֹͣ����
	PPPOESTATUS_LINKUP		= 2, ///< ���ųɹ�
	PPPOESTATUS_LINKDOWN	= 3, ///< ���ӶϿ�
	PPPOESTATUS_DAILING		= 4, ///< ���ڲ���
	PPPOESTATUS_AUTHERR		= 5, ///< �û�У��ʧ��
	PPPOESTATUS_TIMEOUT		= 6, ///< ���ų�ʱ
	PPPOESTATUS_NOISP		= 7, ///< �Ҳ��������ṩ��
	PPPOESTATUS_SRVERR		= 8, ///< PPPoE������쳣
};

enum ESmtpTestStatus
{
	SMTPTESTSTATUS_RUNNING    = 0x01,   ///< ����������
	SMTPTESTSTATUS_DONE       = 0x02,   ///< �������
};

enum EOnvifAuthType
{
	ONVIFAUTHTYPE_NONE		= 1, ///< none(��)
	ONVIFAUTHTYPE_DIGEST    	= 2, ///< digest(digest+WS-Username token��ʽ)
};

enum EQRBmpType
{
	QRBMPTYPE_IOS		= 1, ///< ios��ά��
	QRBMPTYPE_ANDROID	= 2, ///< android��ά��
	QRBMPTYPE_SN		= 3, ///< sn��ά��
};

enum ECloudStatus
{
	CLOUDSTATUS_NOTCON		= 1, ///< δ��������
	CLOUDSTATUS_CONNING		= 2, ///< ��������
	CLOUDSTATUS_CONSUC		= 3, ///< ���ӳɹ�
	CLOUDSTATUS_CONFAILED	= 4, ///< ����ʧ��
};

enum EPingReply
{
	PING_REPLY			= 1,			///< ����
	PING_TIMEOUT		= 2,			///< ��ʱ
	PING_ERROR			= 3,			///< ����
	PING_UNREACHABLE	= 4,			///< ���ɴ�
};

enum EChnRecState
{
	CHNRECSTATE_RECORDING			= 1,			///< ¼����
	CHNRECSTATE_RECORDSTOP		    = 2,			///< ֹͣ
	CHNRECSTATE_RECORDCLOSE			= 3,			///< �ر�
};

enum ELogSubType
{
	LOGSUBTYPE_ALL_ALL                                       = 0x10000001, ///< ȫ��-ȫ��

	LOGSUBTYPE_ALARM_ALL                                     = 0x20000001, ///< �澯-ȫ��
	LOGSUBTYPE_ALARM_ALARMIN                                 = 0x20000002, ///< �澯-�澯����
	LOGSUBTYPE_ALARM_MOVING                                  = 0x20000004, ///< �澯-�ƶ����
	LOGSUBTYPE_ALARM_VIDEOLOST                               = 0x20000008, ///< �澯-��Ƶ��ʧ
	LOGSUBTYPE_ALARM_WARNINGLINE                             = 0x20000010, ///< �澯-������
	LOGSUBTYPE_ALARM_DETECT                                  = 0x20000020, ///< �澯-��������
	LOGSUBTYPE_ALARM_DETECTIN                                = 0x20000040, ///< �澯-��������
	LOGSUBTYPE_ALARM_DETECTOUT                               = 0x20000080, ///< �澯-�뿪����
	LOGSUBTYPE_ALARM_PROPERTYLOST                            = 0x20000100, ///< �澯-��Ʒ����
	LOGSUBTYPE_ALARM_PROPERTYTAKE                            = 0x20000200, ///< �澯-��Ʒ��ȡ
	LOGSUBTYPE_ALARM_PEOPLEGATHERING                         = 0x20000400, ///< �澯-��Ա�ۼ�
	LOGSUBTYPE_ALARM_FACEDETECTION                           = 0x20000800, ///< �澯-�������
	LOGSUBTYPE_ALARM_SHADE                                   = 0x20001000, ///< �澯-�ڵ�
	LOGSUBTYPE_ALARM_OUTOFFOCUS                              = 0x20002000, ///< �澯-�齹
	LOGSUBTYPE_ALARM_SCENECHANGE                             = 0x20004000, ///< �澯-�����仯
	LOGSUBTYPE_ALARM_AUDIOEXCEPTION                          = 0x20008000, ///< �澯-��Ƶ����
	LOGSUBTYPE_ALARM_GPSOVERSPEED                            = 0x20010000, ///< �澯-���ٱ���
	LOGSUBTYPE_ALARM_ACCESS_PROTECT                          = 0x20020000, ///< �澯-�Ž��豸����

	LOGSUBTYPE_SYSEXEPTION_ALL                               = 0x40000001, ///< ϵͳ�쳣-ȫ��
	LOGSUBTYPE_SYSEXEPTION_NETFAULT                          = 0x40000002, ///< ϵͳ�쳣-�������
	LOGSUBTYPE_SYSEXEPTION_IPCONFLIT                         = 0x40000004, ///< ϵͳ�쳣-IP��ͻ
	LOGSUBTYPE_SYSEXEPTION_MACCONFLIT                        = 0x40000008, ///< ϵͳ�쳣-MAC��ͻ
	LOGSUBTYPE_SYSEXEPTION_MONITORDROPPED                    = 0x40000010, ///< ϵͳ�쳣-��ص����
	LOGSUBTYPE_SYSEXEPTION_NORECDISK                         = 0x40000020, ///< ϵͳ�쳣-��Ӳ��
	LOGSUBTYPE_SYSEXEPTION_DISKFAULT                         = 0x40000040, ///< ϵͳ�쳣-Ӳ�̹���
	LOGSUBTYPE_SYSEXEPTION_RECSPACEFULL                      = 0x40000080, ///< ϵͳ�쳣-¼��ռ���
	LOGSUBTYPE_SYSEXEPTION_SNAPSPACEFULL                     = 0x40000100, ///< ϵͳ�쳣-ץ�Ŀռ���
	LOGSUBTYPE_SYSEXEPTION_ILLEGAACCESS                      = 0x40000200, ///< ϵͳ�쳣-�Ƿ�����
	LOGSUBTYPE_SYSEXEPTION_HOTBACKUP                         = 0x40000400, ///< ϵͳ�쳣-�ȱ��쳣    ***** ����û��
	LOGSUBTYPE_SYSEXEPTION_MEDIASTREAMLOSTALL                = 0x40001000, ///< ϵͳ�쳣-��ý�嶪ʧ
	LOGSUBTYPE_SYSEXEPTION_EXCEPTIONREBOOTALL                = 0x40002000, ///< ϵͳ�쳣-����ػ�
	LOGSUBTYPE_SYSEXEPTION_RECALL							 = 0x40004000, ///< ϵͳ�쳣-¼���쳣
	LOGSUBTYPE_SYSEXEPTION_NO_REC_RECORD_ALL				 = 0x40008000, ///< ϵͳ�쳣-¼��ʱ������

	LOGSUBTYPE_USEROPRATE_ALL                                = 0x80000001, ///< �û�����-ȫ��
	LOGSUBTYPE_USEROPRATE_BOOTDEV                            = 0x80000002, ///< �û�����-�����Ϳ����豸
	LOGSUBTYPE_USEROPRATE_LOGIN                              = 0x80000004, ///< �û�����-��¼ע��
	LOGSUBTYPE_USEROPRATE_BROWSE                             = 0x80000008, ///< �û�����-��ʼֹͣ���
	LOGSUBTYPE_USEROPRATE_PTZ                                = 0x80000010, ///< �û�����-��̨����
	LOGSUBTYPE_USEROPRATE_RECIMAGEOPERATE                    = 0x80000020, ///< �û�����-¼��ͼƬ����
	LOGSUBTYPE_USEROPRATE_RECIMAGEREPLAY                     = 0x80000040, ///< �û�����-¼��ͼƬ�ط�
	LOGSUBTYPE_USEROPRATE_LABEL                              = 0x80000080, ///< �û�����-��ǩ����
	LOGSUBTYPE_USEROPRATE_BACKUPORDL                         = 0x80000100, ///< �û�����-��������
	LOGSUBTYPE_USEROPRATE_CHN                                = 0x80000200, ///< �û�����-ͨ������
	LOGSUBTYPE_USEROPRATE_CFGOPERATE                         = 0x80000400, ///< �û�����-���뵼������
	LOGSUBTYPE_USEROPRATE_PARAMCONFIG                        = 0x80000800, ///< �û�����-��������
	LOGSUBTYPE_USEROPRATE_STORAGE                            = 0x80001000, ///< �û�����-Ӳ�̲���
	LOGSUBTYPE_USEROPRATE_UPDATE                             = 0x80002000, ///< �û�����-��������
	LOGSUBTYPE_USEROPRATE_RECOVERY                           = 0x80004000, ///< �û�����-�ָ�����
	LOGSUBTYPE_USEROPRATE_GUIOUTPUTCUT                       = 0x80008000, ///< �û�����-GUI����л�
	LOGSUBTYPE_USEROPRATE_VOICECALLORBROADCAST               = 0x80010000, ///< �û�����-�������й㲥
	LOGSUBTYPE_USEROPRATE_EXTSTORAGE                         = 0x80020000, ///< �û�����-��Ӵ洢�豸����
	LOGSUBTYPE_USEROPRATE_AIANLS							 = 0x80040000, ///< �û�����-���ܷ���
	LOGSUBTYPE_USEROPRATE_CTRLLIB							 = 0x80080000, ///< �û�����-���ؿ�

	LOGSUBTYPE_SYSINFO_ALL                                   = 0x01000001, ///< ϵͳ��Ϣ-ȫ��
	LOGSUBTYPE_SYSINFO_LINE_CONNECT                          = 0x01000002, ///< ϵͳ��Ϣ-�������
	LOGSUBTYPE_SYSINFO_USB_INFO								 = 0x01000004, ///< ϵͳ��Ϣ-U�̲�����γ�
	LOGSUBTYPE_SYSINFO_SATA_DISK							 = 0x01000008, ///< ϵͳ��Ϣ-Ӳ�̲�����γ�
	LOGSUBTYPE_SYSINFO_SDCARD_INFO                           = 0x01000010, ///< ϵͳ��Ϣ-�洢��������γ�
	LOGSUBTYPE_SYSINFO_SIMCARD_INFO                          = 0x01000020, ///< ϵͳ��Ϣ-sim������γ�
	LOGSUBTYPE_SYSINFO_SRV_CENTER_REGIST                     = 0x01000040, ///< ϵͳ��Ϣ-��������ע��
	LOGSUBTYPE_SYSINFO_IPC_ONLINE							 = 0x01000080, ///< ϵͳ��Ϣ-���������
	LOGSUBTYPE_SYSINFO_REC_INFO								 = 0x01000100, ///< ϵͳ��Ϣ-¼��ʼ��ֹͣ
};

enum ELogSrcType
{
	LOGSRCTYPE_ALL                                           = 0x01, ///< ȫ��
	LOGSRCTYPE_NVR                                           = 0x02, ///< NVR
	LOGSRCTYPE_CHN                                           = 0x04, ///< ��ͨ��
	LOGSRCTYPE_USER                                          = 0x08, ///< ���û�
};

enum EHealthLogTimeType
{
	HEALTHLOGTIMETYPE_ONE_MONTH                              = 1, ///< һ����
	HEALTHLOGTIMETYPE_TWO_MONTH                              = 2, ///< ������
	HEALTHLOGTIMETYPE_THREE_MONTH                            = 3, ///< ������
};

enum EHealthInfoType
{
	HEALTHINFOTYPE_POWER_OFF                                 = 0x01, ///< ����ػ�
	HEALTHINFOTYPE_ABNORMAL_REBOOT                           = 0x02, ///< �쳣����
	HEALTHINFOTYPE_NET_ABNORMAL                              = 0x04, ///< �����쳣
	HEALTHINFOTYPE_SHUTDOWN                                  = 0x08, ///< �����ػ�
	HEALTHINFOTYPE_REBOOT                                    = 0x10, ///< ��������
};

enum ESummerTimeOffset
{
	SUMMERTIMEOFFSET_30MIN = 0x01,    ///< 30����
	SUMMERTIMEOFFSET_60MIN  = 0x02,   ///< 60����
	SUMMERTIMEOFFSET_90MIN  = 0x04,   ///< 90����
	SUMMERTIMEOFFSET_120MIN  = 0x08,  ///< 120����
};

enum EDiskInitState
{
	DISKINITSTATE_IDLE        = 0x01,      ///< ����
	DISKINITSTATE_RUNNING     = 0x02,      ///< ������
	DISKINITSTATE_DONE        = 0x04,      ///< ���
	DISKINITSTATE_FAILED      = 0x08,      ///< ʧ��
};

enum ESummerTimeType
{
	SUMMERTIMETYPE_DEC_12 = 0,
	SUMMERTIMETYPE_DEC_11 = 1,
	SUMMERTIMETYPE_DEC_10 = 2,
	SUMMERTIMETYPE_DEC_9 = 3,
	SUMMERTIMETYPE_DEC_8 = 4,
	SUMMERTIMETYPE_DEC_7 = 5,
	SUMMERTIMETYPE_DEC_6 = 6,
	SUMMERTIMETYPE_DEC_5 = 7,
	SUMMERTIMETYPE_DEC_4_30 = 8,
	SUMMERTIMETYPE_DEC_4 = 9,
	SUMMERTIMETYPE_DEC_3_30 = 10,
	SUMMERTIMETYPE_DEC_3 = 11,
	SUMMERTIMETYPE_DEC_2 = 12,
	SUMMERTIMETYPE_DEC_1 = 13,
	SUMMERTIMETYPE_0 = 14,
	SUMMERTIMETYPE_ADD_1 = 15,
	SUMMERTIMETYPE_ADD_2 = 16,
	SUMMERTIMETYPE_ADD_3 = 17,
	SUMMERTIMETYPE_ADD_3_30= 18,
	SUMMERTIMETYPE_ADD_4 = 19,
	SUMMERTIMETYPE_ADD_4_30 = 20,
	SUMMERTIMETYPE_ADD_5 = 21,
	SUMMERTIMETYPE_ADD_5_30 = 22,
	SUMMERTIMETYPE_ADD_5_45 = 23,
	SUMMERTIMETYPE_ADD_6 = 24,
	SUMMERTIMETYPE_ADD_6_30 = 25,
	SUMMERTIMETYPE_ADD_7 = 26,
	SUMMERTIMETYPE_ADD_8 = 27,
	SUMMERTIMETYPE_ADD_9 = 28,
	SUMMERTIMETYPE_ADD_9_30 = 29,
	SUMMERTIMETYPE_ADD_10 = 30,
	SUMMERTIMETYPE_ADD_11 = 31,
	SUMMERTIMETYPE_ADD_12 = 32,
	SUMMERTIMETYPE_ADD_13 = 33,
};

enum ESysTimeSyncType
{
	SYSTIMESYNCTYPE_NTP				= 0x0001,   ///< NTPͬ��
	SYSTIMESYNCTYPE_VSIP			= 0x0002,   ///< VSIPƽ̨ͬ��
	SYSTIMESYNCTYPE_ONVIF			= 0x0004,	///< ONVIFƽ̨ͬ��
	SYSTIMESYNCTYPE_GB1				= 0x0008,   ///< GB28181-1ƽ̨ͬ��
	SYSTIMESYNCTYPE_GB2				= 0x0010,   ///< GB28181-2ƽ̨ͬ��
	SYSTIMESYNCTYPE_ANDROID			= 0x0020,   ///< ANDROIDϵͳͬ��
	SYSTIMESYNCTYPE_GPS				= 0x0040,   ///< GPSͬ��
	SYSTIMESYNCTYPE_WORKSTATION		= 0x0080,   ///< �ɼ�����վͬ��
	SYSTIMESYNCTYPE_AUTO			= 0x0100,   ///< ����Ӧ
};

enum EAlarmInDayType
{
	ALARMINDAYTYPE_MONDAY       = 0, ///< ��һ
	ALARMINDAYTYPE_TUESDAY      = 1, ///< �ܶ�
	ALARMINDAYTYPE_WEDNESDAY    = 2, ///< ����
	ALARMINDAYTYPE_THURSDAY     = 3, ///< ����
	ALARMINDAYTYPE_FRIDAY       = 4, ///< ����
	ALARMINDAYTYPE_SATURDAY     = 5, ///< ����
	ALARMINDAYTYPE_SUNDAY       = 6, ///< ����
	ALARMINDAYTYPE_HOLIDAY      = 7, ///< ����
};

enum EAlarmNumType
{
	ALARMNUM_SMART_WARNINGLINE                     = 1010, ///< �澯-������
	ALARMNUM_SMART_DETECT                          = 1011, ///< �澯-��������
	ALARMNUM_SMART_DETECTIN                        = 1012, ///< �澯-��������
	ALARMNUM_SMART_DETECTOUT                       = 1013, ///< �澯-�뿪����
	ALARMNUM_SMART_PROPERTYLOST                    = 1014, ///< �澯-��Ʒ����
	ALARMNUM_SMART_PROPERTYTAKE                    = 1015, ///< �澯-��Ʒ��ȡ
	ALARMNUM_SMART_PEOPLEGATHERING                 = 1016, ///< �澯-��Ա�ۼ�
	ALARMNUM_SMART_FACEDETECTION                   = 1017, ///< �澯-�������
	ALARMNUM_SMART_SHADE                           = 1018, ///< �澯-�ڵ�
	ALARMNUM_SMART_OUTOFFOCUS                      = 1019, ///< �澯-�齹
	ALARMNUM_SMART_SCENECHANGE                     = 1020, ///< �澯-�����仯
	ALARMNUM_SMART_AUDIOEXCEPTION                  = 1021, ///< �澯-��Ƶ����

	ALARMNUM_SYS_NETFAULT                          = 1030, ///< ϵͳ�쳣-�������
	ALARMNUM_SYS_IPCONFLIT                         = 1031, ///< ϵͳ�쳣-IP��ͻ
	ALARMNUM_SYS_MACCONFLIT                        = 1032, ///< ϵͳ�쳣-MAC��ͻ
	ALARMNUM_SYS_MONITORDROPPED                    = 1033, ///< ϵͳ�쳣-��ص����
	ALARMNUM_SYS_NORECDISK                         = 1034, ///< ϵͳ�쳣-��Ӳ��
	ALARMNUM_SYS_DISKFAULT                         = 1035, ///< ϵͳ�쳣-Ӳ�̹���
	ALARMNUM_SYS_RECSPACEFULL                      = 1036, ///< ϵͳ�쳣-¼��ռ���
	ALARMNUM_SYS_SNAPSPACEFULL                     = 1037, ///< ϵͳ�쳣-ץ�Ŀռ���
	ALARMNUM_SYS_ILLEGAACCESS                      = 1038, ///< ϵͳ�쳣-�Ƿ�����
	ALARMNUM_SYS_HOTBACKUP                         = 1039, ///< ϵͳ�쳣-�ȱ��쳣    ***** ����û��
};

enum ERecoverFactoryType
{
	RECOVERFACTORYTYPE_NONE	= 0, ///< ���ָ�
	RECOVERFACTORYTYPE_ALL	= 1, ///< ��ȫ�ָ�
};

enum EAutoServiceType
{
	AUTOSERVICETYPE_NONE     = 0, ///< ��ά��
	AUTOSERVICETYPE_PERDAY   = 1, ///< ÿ��
	AUTOSERVICETYPE_PERWEEK  = 2, ///< ÿ��
	AUTOSERVICETYPE_PERMONTH = 3, ///< ÿ��
};

enum ESimStatus
{
	SIMSTATUS_INSERT	= 1, ///< ����
	SIMSTATUS_UNINSERT	= 2, ///< δ����
};

enum ENetType
{
	NETTYPE_UNKNOWN	= 0, ///< δ֪
	NETTYPE_2G		= 1, ///< 2G
	NETTYPE_3G		= 2, ///< 3G
	NETTYPE_4G		= 3, ///< 4G
	NETTYPE_5G		= 4, ///< 5G
};

enum EGB28181RegPlatStatus
{
	GB28181REGPLATSTATUS_SUCCESS	= 1, ///< ע��ɹ�
	GB28181REGPLATSTATUS_FAILED		= 2, ///< ע��ʧ��
};

enum EPubSecRegPlatStatus
{
	PUBSECREGPLATSTATUS_SUCCESS	= 1, ///< ע��ɹ�
	PUBSECREGPLATSTATUS_FAILED	= 2, ///< ע��ʧ��
};

enum EAiuSnapType
{
	AIUSNAPTYPE_MANUAL		= 1, ///< �ֶ�ץ��
};

enum EAiuPicType
{
	AIUPICTYPE_FACE	= 1, ///< ����
	AIUPICTYPE_CAR	= 2, ///< ����
};

enum EPicType
{
	PICTYPE_BMP		= 1, ///< bmp
	PICTYPE_JPEG	= 2, ///< jpeg
	PICTYPE_PNG		= 3, ///< png
	PICTYPE_YUV420	= 4, ///< yuv420
	PICTYPE_GIF		= 5, ///< gif
};

enum EPicSizeType
{
	PICSIZETYPE_ORIGINAL	= 1, ///< ԭͼ
	PICSIZETYPE_SMALL		= 2, ///< Сͼ
};

enum ESeriesSnapPicType
{
	SNAPPICTYPE_FACE_ORIG					= 0x0001,   ///< ����ԭͼ
	SNAPPICTYPE_FACE_PART					= 0x0002,   ///< �����ֲ���Ӧ
	SNAPPICTYPE_CAR_ORIG					= 0x0004,	///< ����ͼƬ
	SNAPPICTYPE_CAR_NO						= 0x0008,   ///< ����
	SNAPPICTYPE_REC_THUMBNAIL				= 0x0010,   ///< ¼������ͼ
	SNAPPICTYPE_REC_CLIP_THUMBNAIL			= 0x0020,   ///< ¼��Ƭ������ͼ
	SNAPPICTYPE_MANUAL_SNAP					= 0x0040,   ///< �ֶ�ץ��
	SNAPPICTYPE_EVENT_REC					= 0x0080,   ///< �¼�¼��
	SNAPPICTYPE_EVENT_REC_THUMBNAIL			= 0x0100,   ///< �¼�¼������ͼ
};

enum EAiuAlgDetectType
{
	AIUALGDETECTTYPE_MANUAL	= 1, ///< �㷨������ͣ��ֶ�
	AIUALGDETECTTYPE_AUTO	= 2, ///< �㷨������ͣ��Զ�
};

enum EPicUploadPubSecStatus
{
	PICUPLOADPUBSECSTATUS_NOUPLOAD	= 1, ///< δ�ϴ�
	PICUPLOADPUBSECSTATUS_UPLOADING	= 2, ///< �ϴ���
	PICUPLOADPUBSECSTATUS_UPLOADED	= 3, ///< ���ϴ�
};

enum EPicUploadStatus
{
	PICUPLOADSTATUS_NOUPLOAD		= 1, ///< δ�ϴ�
	PICUPLOADSTATUS_UPLOADING		= 2, ///< �ϴ���
	PICUPLOADSTATUS_UPLOADED		= 3, ///< ���ϴ�
	PICUPLOADSTATUS_UPLOADFAILED	= 4, ///< �ϴ�ʧ��
};

enum EAiuType
{
	AIUTYPE_FACE	= 1, ///< �������ͣ��������
	AIUTYPE_CAR 	= 2, ///< �������ͣ����Ƽ��
};

enum ESexType
{
	SEXTYPE_MALE		= 0x0001, ///< ����
	SEXTYPE_FEMALE		= 0x0002, ///< Ů��
	SEXTYPE_UNLIMITED	= 0x0004, ///< ����
};

enum EIdType
{
	IDTYPE_ID		= 0x0001, ///< ���֤
	IDTYPE_OFFICER	= 0x0002, ///< ����֤
	IDTYPE_PASSPORT = 0x0004, ///< ����
	IDTYPE_OTHER	= 0x0008, ///< ����
	IDTYPE_UNLIMITED= 0x0010, ///< ����
};

enum ECtrlLibAddType
{
	CTRLLIBADDTYPE_NORMAL		= 0, ///< ��ͨ���
	CTRLLIBADDTYPE_FORCE		= 1, ///< ǿ�����
	CTRLLIBADDTYPE_DED_UPDATE	= 2, ///< ����ȥ���жϣ��ظ�ʱ���¸ó�Ա
	CTRLLIBADDTYPE_DED_NO_ADD	= 3, ///< ����ȥ���жϣ��ظ�ʱ���ض�Ӧ������
};

enum ECtrlLibExportType
{
	CTRLLIBEXPORTTYPE_WHOLE		= 0x0001, ///< �������ؿ�
	CTRLLIBEXPORTTYPE_PART		= 0x0002, ///< ���ֳ�Ա
};

enum EEigenvalueType
{
	EIGENVALUETYPE_SUCCESS			= 0x0001, ///< ��ģ�ɹ�
	EIGENVALUETYPE_FAIL				= 0x0002, ///< ��ģʧ��
	EIGENVALUETYPE_UNLIMITED		= 0x0004, ///< ȫ��
};

enum ECtrlLibType
{
	CTRLLIBTYPE_FACE = 0x0001, ///< ������
	CTRLLIBTYPE_CAR  = 0x0002, ///< ������
};

enum ECtrlLibExportState
{
	CTRLLIBEXPORTSTATE_IDLE				= 0x0001, ///< ����
	CTRLLIBEXPORTSTATE_CREAT_FILE		= 0x0002, ///< �������ɵ����ļ�
	CTRLLIBEXPORTSTATE_CREAT_FILE_DONE	= 0x0004, ///< ���ɵ����ļ����
	CTRLLIBEXPORTSTATE_EXPORTING		= 0x0008, ///< ������
	CTRLLIBEXPORTSTATE_DONE				= 0x0010, ///< ���
	CTRLLIBEXPORTSTATE_ERR				= 0x0020, ///< ����
};

enum ECtrlLibImportState
{
	CTRLLIBIMPORTSTATE_IDLE				= 0x0001, ///< ����
	CTRLLIBIMPORTSTATE_PREPARING		= 0x0002, ///< ׼�����
	CTRLLIBIMPORTSTATE_SENDING			= 0x0004, ///< �����ļ�
	CTRLLIBIMPORTSTATE_PROCESSING		= 0x0008, ///< ������
	CTRLLIBIMPORTSTATE_DONE				= 0x0010, ///< ���
	CTRLLIBIMPORTSTATE_ERR_DONE			= 0x0020, ///< ���,����ȡ����ֵ����ӳ�Ա�������
	CTRLLIBIMPORTSTATE_ERR				= 0x0040, ///< ����
	CTRLLIBIMPORTSTATE_OVER_SUP_MAX		= 0x0080, ///< �����������
	CTRLLIBIMPORTSTATE_NO_CSV			= 0x0100, ///< δ�ҵ������ļ�
	CTRLLIBIMPORTSTATE_CHECK_FAIL		= 0x0200, ///< �ļ�У��ʧ��
};

enum ECtrlLibBatchType
{
	CTRLLIBBATCHTYPE_PIC_AND_CSV		= 0, ///< ͼƬ��.csv����ļ�
	CTRLLIBBATCHTYPE_ONLY_PIC			= 1, ///< ����ͼƬ
};

enum EClientType
{
	CLIENTTYPE_GUI			= 0, ///< gui
	CLIENTTYPE_WEB			= 1, ///< web
};

enum EAlgType
{
	ALGTYPE_FACE_DETECT			= 0x0001, ///< �������
	ALGTYPE_FACE_COMPARE		= 0x0002, ///< �����ȶ�
	ALGTYPE_CAR_DETECT			= 0x0004, ///< �������
	ALGTYPE_PERSON_DETECT		= 0x0008, ///< ���˼��
	ALGTYPE_ALL_DETECT			= 0xffff, ///< ȫ��
};

enum EAiuUploadType
{
	AIUUPLOADTYPE_PUBSEC1 	= 0x0001,		///< ��ͼ��1
	AIUUPLOADTYPE_PUBSEC2 	= 0x0002,		///< ��ͼ��2
	AIUUPLOADTYPE_PAD 		= 0x0004,		///< pad
	AIUUPLOADTYPE_FTP 		= 0x0008,		///< ftp
};

enum EAiuUploadCaps
{
	AIUUPLOADCAPS_MDPIC 	= 0x0001,		///< ����ͼƬ
	AIUUPLOADCAPS_CARPIC 	= 0x0002,		///< ����ͼƬ
	AIUUPLOADCAPS_PIC 		= 0x0004,		///< ����ͼƬ
	AIUUPLOADCAPS_AUD 		= 0x0008,		///< ��Ƶ
	AIUUPLOADCAPS_VID 		= 0x0010,		///< ��Ƶ
	AIUUPLOADCAPS_EVENTVID 	= 0x0020,		///< �¼���Ƶ
};

enum EAipDetectMode
{
	AIPDETECTMODE_LOCAL 	= 0x0001,		///< ���ؼ��
	AIPDETECTMODE_REMOTE 	= 0x0002,		///< �������,IPC���
};

enum EAipRmRepeatMode
{
	AIPRMREPEATMODE_H_SCORE 	= 0x0001,		///< �������
	AIPRMREPEATMODE_FIRST_OK 	= 0x0002,		///< ���źϸ�
};

enum EAipUploadMode
{
	AIPUPLOADMODE_ALL 				= 0x0001,		///< ȫ��
	AIPUPLOADMODE_SMALL 			= 0x0002,		///< Сͼ
	AIPUPLOADMODE_BODY 				= 0x0004,		///< ȫ����
	AIPUPLOADMODE_BG 				= 0x0008,		///< ԭͼ
	AIPUPLOADMODE_SBG 				= 0x0010,		///< Сͼ��ԭͼ
	AIPUPLOADMODE_SMALL_THUMBNAIL	= 0x0020,		///< Сͼ������ͼ
	AIPUPLOADMODE_SMALL_GPS			= 0x0040,		///< Сͼ�ӵ���λ��
};

enum EAipCompareMode
{
	AIPCOMPAREMODE_BLACK 	= 0x0001,		///< ������
	AIPCOMPAREMODE_WHITE 	= 0x0002,		///< ������
};

enum ELogAlarmType
{
	LOGALARMTYPE_BLACKLIST 		= 0x0001,		///< ����������
	LOGALARMTYPE_WHITELIST 		= 0x0002,		///< ����������
	LOGALARMTYPE_WHITELOG 		= 0x0004,		///< ��������־
};

enum EAlarmLinkUploadMode
{
	ALARMLINKUPLOADMODE_ALARM_ONLY 		= 0x0001,		///< ��������Ա
	ALARMLINKUPLOADMODE_ALL 			= 0x0002,		///< ȫ����Ա
};

enum EAlarmLinkUploadType
{
	ALARMLINKUPLOADTYPE_FACE 				= 0x0001,		///< ������
	ALARMLINKUPLOADMODE_BACKGROUND 			= 0x0002,		///< ȫ����
	ALARMLINKUPLOADMODE_FACE_BACKGROUND 	= 0x0004,		///< ����+ȫ����
};

enum ESnapPicQuailty
{
	SNAPPICQUAILTY_HIGH 			= 0x0001,		///< ������
	SNAPPICQUAILTY_MIDIUM 			= 0x0002,		///< ȫ����
	SNAPPICQUAILTY_LOW 				= 0x0004,		///< ����+ȫ����
};

/**
 * �ṹ�嶨��
 */

#ifdef WIN32
	#pragma pack(push)
	#pragma pack(1)
	#define NETPACKED
#else
	#define NETPACKED __attribute__((packed))
#endif

struct TNetRect
{
	s32 nX;             ///< left
	s32 nY;             ///< top
	s32 nWidth;         ///< width
	s32 nHeight;        ///< height
}NETPACKED;

struct TNetPoint
{
	s32 nX;	///< x����
	s32 nY;	///< y����
}NETPACKED;

struct TNetNvrCap
{
	s8 szDevModel[NET_MAXLEN_64];	///< NVR�ͺ�
	BOOL32 bActive;					///< NVR�Ƿ񼤻�
	s32 nMaxChnNum;					///< NVR���ͨ����
	s32 nMaxChnGroupNum;			///< NVR���ͨ��������
	s32 nNicNum;					///< ��������
	s32 nSerialNum;					///< ��������
	s32 nMaxAlarmInNum;				///< ���澯��������
	s32 nGB28181InPlatNum;			///< ��������ƽ̨������0��ʾ��֧��GB28181����Э��
	BOOL32 bSupVsip;				///< �Ƿ�֧��vsip����Э��
	BOOL32 bSupGB28181Ctl;          ///< �Ƿ�֧�ֹ�����������
	BOOL32 bSupPubSec;				///< �Ƿ�֧����ͼ������Э��
	BOOL32 bSupPubSecCtl;			///< �Ƿ�֧����ͼ������Э��
	BOOL32 bSupOnvif;               ///< �Ƿ�֧��onvif����Э��
	BOOL32 bSupCloudServer;         ///< �Ƿ�֧���Ʒ���
	BOOL32 bSupZeroChnEnc;			///< �Ƿ�֧����ͨ�����빦��
	BOOL32 bSupAI;                  ///< �Ƿ�֧��AI
	BOOL32 bSupSnmp;			    ///< �Ƿ�֧��snmp����
	BOOL32 bSupPcap;                ///< �Ƿ�֧������ץ��
	BOOL32 bSupBroadcast;           ///< �Ƿ�֧�ֹ㲥
	BOOL32 bSupIpFilter;            ///< �Ƿ�֧��ip����
	BOOL32 bSupSXTServer;           ///< �Ƿ�֧����Ѷͨ����(Ŀǰpad�ͻ���ʹ��)
	BOOL32 bDisableListenStateCtrl;	///< �Ƿ���ü���״̬����
	BOOL32 bWirelessVeh;            ///< �Ƿ��ǳ����豸
	BOOL32 bSupAisCtrlLib;          ///< �Ƿ�֧�ֲ��ؿ�
	BOOL32 bSupSnap;                ///< �Ƿ�֧��ץ��
	BOOL32 bSupAiSearch;			///< �Ƿ�֧�����ܼ���
	BOOL32 bSupWebsocket;			///< �Ƿ�֧��websocket
	BOOL32 bSupDDNS;				///< �Ƿ�֧��Ddns����
	BOOL32 bSupDragCusCanvas;		///< �Ƿ�֧���Զ��廭��
	BOOL32 bSupSysHealth;			///< �Ƿ�֧�ֽ���״̬
	BOOL32 bSupIsp;					///< �Ƿ�֧��Isp
	BOOL32 bSupMulticast;			///< �Ƿ�֧�ֶಥ
	BOOL32 bSupGpSecurity;			///< �Ƿ�֧��GP��ȫ
}NETPACKED;

struct TNetGuiLangInfo
{
	u32 dwLangMask;			///< gui֧�ֵ����ԣ�EGuiLangType���ֵ
	EGuiLangType eCurLang;	///< ��ǰ����
}NETPACKED;

struct TNetLoginInfo
{
	u32 dwNvrIp;									///< NVR IP��������
	u16 wPort;										///< �˿ڣ�������
	s8  szUserName[NET_NVR_USER_NAME_MAX_LEN + 1];	///< �û���
	s8  szPwd[NET_NVR_USER_PWD_MAX_LEN + 1];		///< ����
	s8  szEmail[NET_MAXLEN_32 + 1];					///< ����
	EGuiLangType eGuiLangType;						///< gui�������ͣ�����guiʹ�ã�
	BOOL32 bEnableWebsocket;						///< �Ƿ�����websocket
}NETPACKED;

struct TNetLoginErrInfo
{
	s32 nLoginRetryTimes;       ///< ʣ���¼���Դ���
	s32 nLoginLockedTime;       ///< ʣ���½����ʱ��
}NETPACKED;

struct TNetCheckInfo
{
	s8 szUserName[NET_NVR_USER_NAME_MAX_LEN + 1];	///< �û���
	s8 szPwd[NET_NVR_USER_PWD_MAX_LEN + 1];			///< ����
}NETPACKED;

struct TNetNvrState
{
	u32 dwNvrStateMask;					///< NVR״̬��ENvrState���ֵ
	u32 dwRealLogTimeStamp;				///< ʵʱ��־ʱ���
	u32 dwRealLogNum;					///< ʵʱ��־����
	u32 dwRealStateNum;					///< ʵʱ״̬����
	s32 nNvrNtyEventNum;				///< NVR֪ͨ�¼�����
	BOOL32 bFaceSnapChanged;			///< ����ץ�ı仯��NVRSTATE_AIUSTATUSʱ��Ч
	s8 szFaceSnapChnId[NET_MAXLEN_128];	///< ����ץ�ı仯��ͨ��id��NVRSTATE_AIUSTATUSʱ��Ч
	BOOL32 bCarSnapChanged;				///< ����ץ�ı仯��NVRSTATE_AIUSTATUSʱ��Ч
	s8 szCarSnapChnId[NET_MAXLEN_128];	///< ����ץ�ı仯��ͨ��id��NVRSTATE_AIUSTATUSʱ��Ч
	BOOL32 bAiuCfgChanged;				///< �������ñ仯��NVRSTATE_AIUSTATUSʱ��Ч
	s8 szAiuCfgChnId[NET_MAXLEN_128];	///< �������ñ仯��ͨ��id��NVRSTATE_AIUSTATUSʱ��Ч
	BOOL32 bNvrAlarmChanged;			///< nvr�澯�仯��NVRSTATE_ALARMSTATEʱ��Ч
	BOOL32 bChnAlarmChanged;			///< ͨ���澯�仯��NVRSTATE_ALARMSTATEʱ��Ч
	s8 szChnAlarmChnId[NET_MAXLEN_128];	///< ͨ���澯�仯��ͨ��id��NVRSTATE_ALARMSTATEʱ��Ч
	s8 szChnListChnId[NET_MAXLEN_512];	///< ͨ���б�仯��ͨ��id��NVRSTATE_CHNLISTʱ��Ч
}NETPACKED;

struct TNetCallBackInfo
{
	u32 dwHandle;			///< ���
	s32 nErrNo;				///< ������
	void *pParam;			///< �ص���������
	void *pResult;			///< �첽����ִ�н��
	s32 nResultLen;			///< pResult����
	EInterType eInterType;	///< �ӿ�����
}NETPACKED;

struct TNetMsgItem
{
	u32 dwHandle;		///< ���
	EMsgType eMsgType;	///< ��Ϣ����
	void *pData;		///< ����
	s32 nDataLen;		///< ���ݳ���
	u32 dwData;			///< eMsgType = MSGTYPE_FACE_DETECT_SNAP������ץ����������
						///< eMsgType = MSGTYPE_CAR_DETECT_SNAP������ץ�ĳ�������
						///< eMsgType = MSGTYPE_PERSON_DETECT_SNAP���������˼������
	// void *pParam;	///< �ص���������
}NETPACKED;

struct TNetSubscribeMsgItem
{
	EMsgType eMsgType;	///< ��Ϣ����
}NETPACKED;

struct TNetSubscribeMsgList
{
	s32 nNum;															///< ������Ϣ����
	TNetSubscribeMsgItem atSubscribeMsgItem[NET_SUBSCRIBE_MSG_MAX_NUM];	///< ������Ϣ��Ϣ
}NETPACKED;

struct TNetProtoItem
{
	EProtoType eProtoType;	///< Э������
	BOOL32 bSupportSearch;	///< Э���Ƿ�֧���豸����
	u32 dwTransProtoMask;	///< ý�崫��Э�飬ETransProto���ֵ
	u32 dwTcpConnectModeMask;///< TCP���ӷ���ETcpMode���ֵ
}NETPACKED;

struct TNetProtoList
{
	u8 byProtoNum;                                 ///< Э������
	TNetProtoItem atProtoItem[NET_PROTO_MAX_NUM];  ///< Э������
}NETPACKED;

struct TNetSearchDevParam
{
	u32 dwProtoMask;		///< ����ָ��Э�����͵��豸��EProtoType���ֵ
	BOOL32 bEnableIpFilter;	///< �Ƿ�����IP����
	u32 dwStartIp;			///< ��ʼIP��������
	u32 dwEndIp;			///< ����IP��������
}NETPACKED;

struct TNetSearchDevTask
{
	u32 dwTaskId;		///< ��������id
	u32 dwTaskTimes;	///< �����������ʱ������λs
}NETPACKED;

struct TNetDevExtInfo
{
	BOOL32 bSupActive;                     ///< �Ƿ�֧�ּ���
	BOOL32 bActive;                        ///< �Ƿ񼤻�
	BOOL32 bSupSetAddr;                    ///< �Ƿ�֧�����õ�ַ
	u32 dwSubnetMask;	                   ///< �������룬������
	u32 dwDefGateway;	                   ///< Ĭ�����أ�������
	s8 szExtProtoVer[NET_MAXLEN_64 + 1];   ///< ��ȡ��չ��Ϣʹ�õ�Э��汾
	s8 szMac[NET_MAXLEN_32 + 1];           ///< MAC��ַ
	s8 szSecureCode[NET_MAXLEN_64 + 1];    ///< ��ȫ��
	EActiveState eActiveState;             ///< ����״̬
}NETPACKED;

struct TNetDevInfo
{
	u32 dwIp;								///< �豸IP��ַ��������
	EProtoType eProtoType;					///< Э������
	u16 wProtoPort;							///< Э��˿�
	u16 wVideoEncChnNum;					///< ��Ƶ����ͨ������
	u16 wAudioEncNum;						///< ��Ƶ��������
	u16 wAudioDecNum;						///< ��Ƶ��������
	s8 szDevModel[NET_MAXLEN_32 + 1];		///< �豸�ͺ�
	s8 szManufacturer[NET_MAXLEN_32 + 1];	///< ������Ϣ
	s8 szUuid[NET_MAXLEN_128 + 1];          ///< �豸��Ӧuuid
	EDevType eDevType;						///< �豸����
	s8 szSoftVer[NET_MAXLEN_64];		    ///< ����汾��
	TNetDevExtInfo tDevExtInfo;             ///< �豸��չ��Ϣ
}NETPACKED;

struct TNetSearchedDevList
{
	BOOL32 bFinished;	                                     ///< �����Ƿ����
	u16 wDevNum;                                             ///< �豸����
	TNetDevInfo atDevInfo[NET_PER_GET_SEARCHED_DEV_MAX_NUM]; ///< �豸��Ϣ
}NETPACKED;

struct TNetAddDev
{
	s32 nChnId;					    ///< ͨ��id��1~���ͨ������NET_CHNID_AUTO��ʾ��ָ��ͨ�����Զ�����
	s8  szDevInfo[NET_MAXLEN_512];	///< �豸��Ϣ��xml��ʽ
}NETPACKED;

struct TNetAddDevList
{
	u16 wDevNum;                                  ///< �豸����        
	TNetAddDev atAddDev[NET_PER_ADD_DEV_MAX_NUM]; ///< �豸��Ϣ
}NETPACKED;

struct TNetDelDevList
{ 
	u16 wDevNum;                            ///< �豸����
	s32 anChnId[NET_PER_DEL_DEV_MAX_NUM];   ///< ͨ��id����
}NETPACKED;

struct TNetDevActiveInfo
{ 
	u32 dwIp;								///< �豸IP��ַ��������
	EProtoType eProtoType;					///< Э������
	u16 wProtoPort;							///< Э��˿�
	s8 szExtProtoVer[NET_MAXLEN_64 + 1];    ///< ��ȡ��չ��Ϣʹ�õ�Э��汾
	s8 szMac[NET_MAXLEN_32 + 1];            ///< MAC��ַ
	s8 szSecureCode[NET_MAXLEN_64 + 1];     ///< ��ȫ��
}NETPACKED;

struct TNetCreateDevActiveTaskInfo
{ 
	BOOL32 bAdminPwd;                                              ///< �Ƿ�ʹ��NVR����Ա����
	BOOL32 bAdminEmail;                                            ///< �Ƿ�ʹ��NVR����Ա����
	s8 szPassword[NET_MAXLEN_64];                                  ///< �Զ��弤������(bAdminPwdΪfalseʱ��Ч)
	s8 szEmail[NET_MAXLEN_64];                                     ///< �Զ����������(bAdminEmailΪfalseʱ��Ч)
	u16 wDevNum;                                                   ///< �豸����
	TNetDevActiveInfo atDevActiveInfo[NET_PER_DEV_ACTIVE_MAX_NUM]; ///< �豸��Ϣ
}NETPACKED;

struct TNetAddDevActiveInfo
{ 
	s32 nTaskId;                                                   ///< ��������ID
	u16 wDevNum;                                                   ///< �豸����
	TNetDevActiveInfo atDevActiveInfo[NET_PER_DEV_ACTIVE_MAX_NUM]; ///< �豸��Ϣ
}NETPACKED;

struct TNetQueryDevActiveTaskState
{ 
	s32 nTaskId;                                                   ///< ��������ID
	u16 wDevNum;                                                   ///< �豸����
	TNetDevActiveInfo atDevActiveInfo[NET_PER_DEV_ACTIVE_MAX_NUM]; ///< �豸��Ϣ
}NETPACKED;

struct TNetDevActiveState
{ 
	EDevActiveState eDevActiveState;	       ///< ����״̬
	EDevActiveErrReason eDevActiveErrReason;   ///< �������ԭ��(��ѡ��)
	TNetDevActiveInfo tDevActiveInfo;          ///< �豸��Ϣ
}NETPACKED;

struct TNetDevActiveTaskState
{ 
	BOOL32 bFinished;                                                ///< �Ƿ����
	u16 wDevNum;                                                     ///< �豸����
	TNetDevActiveState atDevActiveState[NET_PER_DEV_ACTIVE_MAX_NUM]; ///< �豸��Ϣ
}NETPACKED;

struct TNetModifyDevIpInfo
{ 
	s8  szUserName[NET_MAXLEN_64];	                ///< �û���
	s8  szPwd[NET_MAXLEN_64 + 1];		            ///< ����
	u32 dwIp;		    							///< �޸ĺ��IP��ַ
	u32 dwSubnetMask;		    					///< �޸ĺ����������
	u32 dwDefaultGateway;		    				///< �޸ĺ��Ĭ������
	TNetDevActiveInfo tDevInfo;                     ///< �豸��Ϣ
}NETPACKED;

struct TNetCreateDevUpgradeTaskInfo
{ 
	u16 wDevNum;                                ///< �豸����
	s32 anChnId[NET_PER_UPGRADE_DEV_MAX_NUM];   ///< ͨ��id����
	s8 szPkgHead[NET_MAXLEN_512 + 1];           ///< pkg��ͷ��Ϣ
	s32 nPkgHeadSize;                           ///< pkg��ͷ��С
	s32 nPkgTotalSize;                          ///< pkg���ܴ�С
}NETPACKED;

struct TNetDevUpgradeStateInfo
{ 
	s32 nChnId;                                  ///< ͨ��id
	EDevUpgradeState eDevUpgradeState;           ///< ��ǰ����״̬
	EDevUpgradeErrReason eDevUpgradeErrReason;   ///< ��������ԭ��,ֻ������״̬ΪDEVUPGRADESTATE_ERRORʱ��Ч
}NETPACKED;

struct TNetDevUpgradeStateList
{ 
	BOOL32 bAllFinished;                                                         ///< �Ƿ�ȫ����������
	s32 nDevStateNum;                                                            ///< �����豸����
	TNetDevUpgradeStateInfo atDevUpgradeStateInfo[NET_PER_UPGRADE_DEV_MAX_NUM];  ///< �����豸״̬�б�
}NETPACKED;

struct TNetDevUpgradePkgData
{ 
	s32 nDataStartPos;                                    ///< ������ʼƫ��
	s32 nDataLen;                                         ///< ����buffer����
	s8 *pszDataBuf;                                       ///< ����buffer����
}NETPACKED;

struct TNetGetNvrChnList
{
	u32 dwChnMask;				///< ��ȡͨ���б�ָ����Ϣ��EChnMask���ֵ
	s32 nChnIdStart;			///< ��ʼͨ��id
	s32 nChnIdEnd;				///< ����ͨ��id
	s8 szChnId[NET_MAXLEN_256];	///< ͨ��id����nChnIdStart��nChnIdEnd���⣬���ͨ��ʹ�ö��ŷָ���磺1,3,9
}NETPACKED;

struct TNetDevItem
{
	TNetDevInfo tDevInfo;	///< ������Ϣ
}NETPACKED;

struct TNetChnState
{
	BOOL32 bDevOnline;		///< �豸����
	BOOL32 bIsRecording;	///< �Ƿ���¼��
	BOOL32 bMoveDetect;     ///< �Ƿ񴥷��ƶ����澯
	BOOL32 bVideoLost;		///< �Ƿ񴥷���ƵԴ��ʧ�澯
	BOOL32 bAlmIn;			///< �Ƿ񴥷��澯����澯
	BOOL32 bSmart;			///< �Ƿ񴥷����ܸ澯
	BOOL32 bAccessProtect;	///< �Ƿ񴥷����𱨾�
	EDevErrNo eDevErrNo;	///< �豸��������µĴ�����
}NETPACKED;

struct TNetChnInfo
{
	s32 nId;						///< ͨ��id
	s32 nDevId;						///< �豸id
	s8  szAlias[NET_MAXLEN_64 + 1];	///< ͨ��������0-32���ַ�
	s32 nVideoSourceId;				///< ��ƵԴid
	BOOL32 bIdle;					///< ͨ���Ƿ����
	BOOL32 bNewAdd;					///< ͨ���Ƿ������
	TNetChnState tChnState;			///< ͨ��״̬
}NETPACKED;

struct TNetChnItem
{
	TNetChnInfo tChnInfo;            ///< ͨ����Ϣ
	TNetDevItem tDevItem;            ///< �豸��Ϣ
}NETPACKED;

struct TNetNvrChnList
{
	u16 wChnNum;				///< [in]-���鳤�ȣ�[out]-ʵ�ʻ�ȡ��ͨ������
	TNetChnItem *patChnItem;	///< ָ��TNetChnItem����
}NETPACKED;

struct TNetCompositeChnItem
{
	s32 nChnId;                              ///< ͨ��ID
	ECompositeChnType eCompositeChnType;     ///< �ϳ�ͨ������
	BOOL32 bDisplayFirst;                    ///< �Ƿ�չʾ���б���λ
	BOOL32 bSupportRec;                      ///< �Ƿ�֧��¼��
	BOOL32 bSupportEditName;                 ///< �Ƿ�֧�ֱ༭����
}NETPACKED;

struct TNetNvrCompositeChnCap
{
	u16 wChnNum;				                                        ///< ��ȡ��ͨ������
	TNetCompositeChnItem atCompositeChnItem[NET_COMPOSITE_CHN_MAX_NUM];	///< ָ��TNetChnItem����
}NETPACKED;

struct TNetOnvifChnCfg
{
	u32 dwIp;								///< �豸IP��ַ��������
	u16 wProtoPort;							///< Э��˿�
	EDevAddMode eDevAddMode;                ///< �豸���ģʽ
	u16 wSrcId;		            			///< ��ƵԴid
	u16 wSrcNum;                            ///< ��ƵԴ����(ֻ�е�eDevAddModeȡֵΪDEVADDMODE_KEDAFISHEYEʱ��Ч)
	ETransProto eTransProto;    	        ///< ����Э������
	s8 szAuthName[NET_MAXLEN_64 + 1];       ///< ��֤�û���
	s8 szAuthPwd[NET_MAXLEN_64 + 1];        ///< ��֤����
}NETPACKED;

struct TNetRtspChnCfg
{
	u32 dwIp;								///< �豸IP��ַ��������
	ETransProto eTransProto;    	        ///< ����Э������
	s8 szMainStreamUrl[NET_MAXLEN_128 + 1]; ///< ��������ַ
	s8 szSecStreamUrl[NET_MAXLEN_128 + 1];  ///< ��������ַ
	s8 szAuthName[NET_MAXLEN_64 + 1];       ///< ��֤�û���
	s8 szAuthPwd[NET_MAXLEN_64 + 1];        ///< ��֤����
	BOOL32 bSendRtspKplvForTcp;             ///< tcpģʽ���Ƿ���rtsp����
}NETPACKED;

struct TNetGb28181EncChnCfg
{
	s8 szEncId[NET_MAXLEN_32];                                               ///< ����ͨ��ID
}NETPACKED;

struct TNetGb28181ChnCfg
{
	s32 nEncChnNum;                                                          ///< ����ͨ������
	TNetGb28181EncChnCfg atGb28181EncChnCfg[NET_GB28181_PRECHN_ENC_MAX_NUM]; ///< ����ͨ��ID
}NETPACKED;

struct TNetGb28181AlarmInCfg
{
	s8 szAlarmInId[NET_MAXLEN_32];                                           ///< ����ͨ��ID
}NETPACKED;

struct TNetGb28181DevCfg
{
	s8 szDevId[NET_MAXLEN_32];                                               ///< �豸ID
	s8 szDevPwd[NET_MAXLEN_64];                                              ///< ����
	ETransProto eTransProto;    	                                         ///< ����Э������
	ETcpMode eTcpMode;														 ///< ���ӷ���	
	s32 nChnNum;                                                             ///< ͨ������
	TNetGb28181ChnCfg atGb28181ChnCfg[NET_GB28181_CHN_MAX_NUM];              ///< ͨ����Ϣ
	s32 nAlarmInNum;                                                         ///< �澯��������
	TNetGb28181AlarmInCfg atGb28181AlarmInCfg[NET_GB28181_CHN_MAX_NUM];       ///< �澯������Ϣ
}NETPACKED;

struct TNetChnCfg
{
	s32 nChnId;					            ///< ͨ��id
	TNetOnvifChnCfg tOnvifChnCfg;           ///< Onvif�豸����
	TNetRtspChnCfg tRtspChnCfg;             ///< Rtsp�豸����
	TNetGb28181DevCfg tGb28181DevCfg;       ///< GB28181�豸����
}NETPACKED;

struct TNetChnAlias
{
	s32 nChnId;                         ///< ͨ��id
    s8 szChnAlias[NET_MAXLEN_64 + 1];	///< ͨ��������0~32���ַ�
	BOOL32 bSetDevOsd;                  ///< ͨ����չ���ã��Ƿ�����ΪOSD̨�ֻ꣬��ͨ���������ü���ȡͨ����չ����ʱʹ��
	BOOL32 bSetDevName;                 ///< ͨ����չ���ã��Ƿ�����Ϊͨ���豸���ƣ�ֻ��ͨ���������ü���ȡͨ����չ����ʱʹ��
}NETPACKED;

struct TNetChnAliasList 
{
	u16 wNum;                                              ///< ͨ�������б�����
	TNetChnAlias atChnAlias[NET_CHN_ALIAS_LIST_MAX_NUM];   ///< ͨ�������б�
}NETPACKED;

struct TNetChnGroupItem
{
	s32 nId;									///< ͨ����Id
	s8  szAlias[NET_MAXLEN_64 + 1];	            ///< ͨ���������0(��������ͨ��)����1~32���ַ�
	u16 wChnNum;                                ///< ͨ������
	s32 anChnId[NET_CHNGROUP_CHNITEM_MAX_NUM];	///< ͨ��id�б�
}NETPACKED;

struct TNetChnGroupList
{
	u16 wChnGroupNum;                                      ///< ͨ����������
	TNetChnGroupItem atChnGroupItem[NET_CHNGROUP_MAX_NUM]; ///< ͨ��������Ϣ
}NETPACKED;

struct TNetTimeSyncCfg
{
	s32 nChnId;                           ///< ͨ��ID
	u32 dwTimeSyncTypeMask;               ///< ʱ��ͬ���������룬ETimeSyncType���ֵ
	ETimeSyncType eCurTimeSyncType;       ///< ��ǰʱ��ͬ������
	BOOL32 bSyncTimeZone;                 ///< �Ƿ���nvrʱ��ͬ����TRUE-ͬ����FALSE-��ͬ��
}NETPACKED;

struct TNetOsdMargin
{
	EOsdMarginType eOsdMarginTypeTop;        ///< ��ǰ�����߾�
	EOsdMarginType eOsdMarginTypeLeft;       ///< ��ǰ���߾�
	EOsdMarginType eOsdMarginTypeRight;      ///< ��ǰ�Ҳ�߾�
	EOsdMarginType eOsdMarginTypeBottom;     ///< ��ǰ�ײ��߾�
	u32 dwOsdMarginMask;                     ///< ֧�ֵı߾����ͣ�EOsdMarginType�������ֵ
}NETPACKED;

struct TNetOsdPic
{
	BOOL32 bCustomAreaOnePic;                             ///< �û��Զ��������Ƿ�ֻ����һ��ͼƬ
	EOsdPicType eCurOsdPicType;                           ///< ��ǰOSDͼƬ����
	u32 dwSupOsdPicType;                                  ///< ֧�ֵ�ͼƬ���ͣ�EOsdPicType�������ֵ
	s32 nPicBits;                                         ///< ͼƬλ��
}NETPACKED;

struct TNetOsdTime
{
	EOsdTimeFormat eOsdTimeFormat;                 ///< ʱ���ʽ
	u32 dwOsdTimeFormatMask;                       ///< ֧�ֵ�ʱ���ʽ��EOsdTimeFormat�������ֵ
	BOOL32 bSingleRow;                             ///< �Ƿ�����ʾ��true:����,false:���У�
}NETPACKED;

struct TNetOsdFontSize
{
	EOsdResolutionType eOsdResolutionType;                ///< �ֱ�������
	EOsdFontSizeType eOsdFontSizeType;                    ///< �����С
	u32 dwOsdFontSizeTypeMask;                            ///< ֧�ֵ������С��EOsdFontSizeType�������ֵ
	s32 nOsdFontSize;                                     ///< �����С(���ڳ���������Χʱ��Ч)
}NETPACKED;

struct TNetOsdFont
{
	EOsdFontType OsdFontType;                             ///< ��������
	u32 dwOsdFontTypeMask;                                ///< ֧�ֵ��������ͣ�EOsdFontType�������ֵ
	s8 szOsdTextFont[NET_MAXLEN_64 + 1];                  ///< �������ƣ�PicGenByPicȡֵΪfalseʱ���ϴ����Զ�����������
	EOsdTextColor eOsdTextColor;                          ///< ������ɫ
	u32 dwOsdTextColorMask;                               ///< ֧�ֵ�������ɫ��EOsdTextColor�������ֵ              
	EOsdTextColor eOsdTextBkColor;                        ///< ���ֱ�����ɫ
	u32 dwOsdTextBkColorMask;                             ///< ֧�ֵ����ֱ�����ɫ��EOsdTextColor�������ֵ
	BOOL32 bTextBkTransparent;                            ///< ���ֱ����Ƿ�͸��
	BOOL32 bTextEdge;                                     ///< �����Ƿ����
	s32 nOsdFontSizeNums;                                 ///< osd�����С����
	TNetOsdFontSize atOsdFontSize[NET_DEVOSD_MAX_NUM];    ///< osd�����С
}NETPACKED;

struct TNetSingleOsdCfg
{
	EOsdType eOsdType;                       ///< OSD����
	BOOL32 bVisible;                         ///< �Ƿ�ɼ�
	s32 nUserOsdSN;                          ///< �û��Զ���OSD��ʶid��ȡֵ��ΧΪ: 1 - UserMaxNumֵ,����OSDȡ0
	TNetRect tOsdRect;                       ///< osd λ��
	EOsdContentType eOsdContentType;         ///< osd ��������
	u32 dwOsdContentTypeMask;                ///< osd �����������룬EOsdContentType�������ֵ
	s8 szContent[NET_MAXLEN_128];            ///< osd ����
	s32 nMaxChar;                            ///< osd ����ַ���
	s8 szDevLineId[NET_MAXLEN_128];          ///< ��������Ϊdevʱ,��Ӧ�������ַ�������id
}NETPACKED;

struct TNetOsdCap
{
	BOOL32 bOsdMoveFree;                                      ///< �Ƿ�֧��ÿ��osd��Ļ�����϶�
	u32 dwSupOsdLanguageType;                                 ///< ǰ��֧�����õ�osd�����������룬EOsdLanguageType�������ֵ
	u32 dwSupFontAttr;                                        ///< ֧�ֵ��������ԣ�EOsdFontAttrType�������ֵ
	u32 dwSupOsdType;                                         ///< ֧�ֵ�osd���ͣ�EOsdType�������ֵ
	s32 nUserOsdMaxNum;                                       ///< �û��Զ�����Ļ֧�ֵ�������
	s32 nUserAreaMaxSize;                                     ///< �û���Ļ֧�ֵ�������
	s32 nOsdArea2MaxSize;                                     ///< ����ʱ�䡢�ٶȡ��ź�����REC��Ļ��OSD����2������(1080p�ֱ�����,����ʹ��)��
	                                                          ///< ��֧��ÿ��osd��Ļ�����϶�,���ֵ��Ч
}NETPACKED;

struct TNetDevOsdCfg
{
	BOOL32 bMultiLine;                                    ///< �Ƿ�OSD1~OSD5֧�ֶ��У��༭ʱ����һ��osd��ͬʱ֧�ֻس����У���GUI��webĬ��false��
	BOOL32 bCgiSetOsd;                                    ///< �Ƿ���cgi��osd����
	BOOL32 bCreateByDev;                                  ///< �Ƿ���ǰ������ͼƬ
	BOOL32 bSupCreateByDev;                               ///< �Ƿ�֧����ǰ������ͼƬ
	BOOL32 bSupChangeCreateByDev;                         ///< �Ƿ�֧���޸���ǰ������ͼƬ
	s32 nResolutionWidth;                                 ///< ��Ƶ������Ƶ������ķֱ��ʲ���ͬһ���
	s32 nResolutionHeight;                                ///< ��Ƶ�ߣ�����Ƶ������ķֱ��ʲ���ͬһ���
	EOsdResolutionType eOsdResolutionType;                ///< �ֱ�������
	s8 szConfigType[NET_MAXLEN_128];                      ///< ��������
	TNetOsdMargin tOsdMargin;                             ///< OSD����߾�
	TNetOsdPic tOsdPic;                                   ///< OSDͼƬ
	TNetRect tOsdCustomArea;                              ///< �û��Զ�������
	TNetOsdTime tOsdTime;                                 ///< ʱ������
	TNetOsdFont tOsdFont;                                 ///< ��������
	s32 nOsdNums;                                         ///< osd����
	TNetSingleOsdCfg atSingleOsdCfg[NET_DEVOSD_MAX_NUM];  ///< ����osd��Ϣ
	TNetOsdCap tOsdCap;                                   ///< osd����
}NETPACKED;

struct TNetOsdPicFile
{
	EOsdType eOsdType;                       ///< OSD����
	s8 szFilePath[NET_MAXLEN_128];           ///< osd ͼƬ�ļ�λ��
}NETPACKED;

struct TNetOsdPicFileList
{
	s32 nOsdPicNums;                         ///< osd ͼƬ����
	TNetOsdPicFile atOsdPicFile[NET_DEVOSD_MAX_NUM]; ///< osd ͼƬ��Ϣ����
}NETPACKED;

struct TNetRcModeItem 
{
	ERcModeType eRcModeType;
	u32 dwImgQualityMask;               ///< ���������������룬EImgQualityType���ֵ
}NETPACKED;

struct TNetEncResolution
{
	s16 nEncResolutionWidth;            ///< ����ֱ��ʿ�
	s16 nEncResolutionHeight;           ///< ����ֱ��ʸ�
}NETPACKED;

struct TNetResFrameRate
{
	TNetEncResolution tEncResolution;            ///< ����ֱ���
	s16 nFrameRateMin;                           ///< �˷ֱ����µ������С֡��
	s16 nFrameRateMax;
}NETPACKED;

struct TNetVideoEncCapItem 
{
	EVideoEncType eEncType;											///< ��������
	TNetEncResolution atEncResolution[NET_VIDEOENC_RESOLUTION_MAX_NUM]; ///< ����ֱ�����������
	u16 wEncResNum;  
	TNetResFrameRate atResFrameRate[NET_VIDEOENC_RESOLUTION_MAX_NUM];	///< �˷ֱ����µ������С֡��
	u32 dwRcModeMask;												///< ���ʿ���ģʽ���룬ERcModeType���ֵ
	u16 wRcModeNum;													///< ���ʿ���ģʽ����
	TNetRcModeItem atRcModeItem[NET_VIDEOENC_RECMODE_MAX_NUM];		///< ���ʿ�����Ϣ�б�
	u32 dwImgQualityMask;											///< ���������������룬EImgQualityType���ֵ
	u32 dwEncProfileMask;											///< ���븴�Ӷ��������룬EVideoEncProfileType���ֵ
	s32 nMaxKeyRateMin;												///< �ؼ�֡�����Χ
	s32 nMaxKeyRateMax;
	s32 nBitRateMin;												///< �������޷�Χ
	s32 nBitRateMax;
	BOOL32 bSupSmartEnc;										    ///< �Ƿ�֧��smart����
	BOOL32 bSupBitSmooth;											///< �Ƿ�֧������ƽ��
	s32 nBitSmoothMin;												///< ����ƽ����Χ
	s32 nBitSmoothMax;
}NETPACKED;

struct TNetVideoEncParamItem 
{
	u16 wEncId;                                                            ///< ����ͨ��id
	EVideoEncType eEncType;                                                ///< ��������
	u32 dwEncTypeMask;                                                     ///< �����������룬EVideoEncType���ֵ
	u16 wEncTypeNums;                                                      ///< ֧�ֵı�����������
	TNetVideoEncCapItem atVideoEncCapItem[NET_VIDEOENC_ENCTYPE_MAX_NUM];   ///< ��������������Ϣ�б�
	ERcModeType eRcModeType;                                               ///< ���ʿ���ģʽ
	s32 nFrameRate;                                                        ///< ��Ƶ֡��
	TNetEncResolution tEncResolution;                                      ///< ����ֱ���
	s32 nMaxBitRate;                                                       ///< ������������
	s32 nMaxKeyRate;                                                       ///< I֡���
	EImgQualityType eImgQualityType;                                       ///< ͼ������
	EVideoEncProfileType eEncProfileType;                                  ///< ���븴�Ӷ�
	BOOL32 bEnableSmartEnc;												   ///< �Ƿ���smart����
	s32 nBitSmoothLevel;                                                   ///< ����ƽ��
}NETPACKED;

struct TNetChnVideoEncParam
{
	s32 nChnId;                                                            ///< ͨ��id
	u16 wEncChnNum;                                                        ///< ����ͨ������
	TNetVideoEncParamItem atVideoEncParamItem[NET_PER_CHN_ENCCHN_MAX_NUM]; ///< ����ͨ����Ϣ
}NETPACKED;

struct TNetAudioEncCapItem
{
	EAudioEncType eAudioEncType;                                            ///< ��Ƶ��������
	u32 dwAudioSampleRateTypeMask;                                          ///< �������������룬EAudioSampleRateType���ֵ
	u16 wEncVolMin;                                                         ///< ����������Сֵ
	u16 wEncVolMax;                                                         ///< �����������ֵ
}NETPACKED;

struct TNetAudioEncParamItem 
{
	u16 wEncId;                                                             ///< ��Ƶ����ͨ��ID
	EAudioInType eAudioInType;                                              ///< ��ƵԴ����
	EAudioEncType eAudioEncType;                                            ///< ��Ƶ��������
	EAudioSampleRateType eAudioSampleRateType;                              ///< ���������
	u16 wEncVol;                                                            ///< ��������
	u32 dwAudioEncTypeMask;                                                 ///< ��Ƶ�����������룬EAudioEncType���ֵ
	u16 wEncTypeNum;                                                        ///< ������������
	TNetAudioEncCapItem atAudioEncCapItem[NET_AUDIOENC_ENCTYPE_MAX_NUM];    ///< ��������������Ϣ�б�	
}NETPACKED;

struct TNetChnAudioEncParam
{
	s32 nChnId;                                                             ///< ͨ��id
	u16 wEncChnNum;                                                         ///< ����ͨ������
	u32 dwAudioInTypeMask;                                                  ///< ��ǰ֧����ƵԴ�������룬EAudioInType���ֵ
	EAudioInType eCurAudioInType;											///< ��ǰѡ�е���Ƶֵ
	TNetAudioEncParamItem atAudioEncParamItem[NET_PER_CHN_ENCCHN_MAX_NUM];  ///< ����ͨ����Ϣ
}NETPACKED;

struct TNetChnAudioDecParam 
{
	s32 nChnId;                                                             ///< ͨ��id
	BOOL32 bEnableAudMixRec;												///< �Ƿ�֧�ֻ���¼��
	BOOL32 bAudMixRec;                                                      ///< �Ƿ����¼��
	u16 wAudDecVol;                                                         ///< ��������
	u16 wAudDecVolMin;                                                      ///< ����������Сֵ
	u16 wAudDecVolMax;                                                      ///< �����������ֵ
}NETPACKED;

struct TNetCordonCfg
{
	s32 nErrNo;					///< ����� NET_OK����ȡ�ɹ�
	BOOL32 bEnable;				///< �Ƿ�����
	s32 nCurSchemeId;			///< ��ǰ��������1��ʼ
	s32 nSchemeMaxNum;			///< �����������
	s32 nVgaWidth;				///< �������
	s32 nVgaHeight;				///< �����߶�
	BOOL32 bSupPreset;			///< �Ƿ�֧�ֹ���Ԥ��λ
	s32 nPresetId;				///< Ԥ��λid����0��ʼ��0����ʾ����Ԥ��λ
	s32 nPresetMaxNum;			///< Ԥ��λ���������������Ԥ��λ0
	ECordonType eCordonType;	///< �����߷���
	u32 dwCordonTypeMask;		///< �����߷������룬ECordonType���ֵ
	s32 nTargetRatio;			///< Ŀ�����
	s32 nTargetRatioMin;		///< Ŀ�������Сֵ
	s32 nTargetRatioMax;		///< Ŀ��������ֵ
	s32 nSensi;					///< ������
	s32 nSensiMin;				///< ��������Сֵ
	s32 nSensiMax;				///< ���������ֵ
	TNetRect tTargetFilterRect;	///< Ŀ���������
	TNetPoint tStartPoint;		///< ���������
	TNetPoint tEndPoint;		///< �������յ�
	TNetPoint tAPoint;			///< A���������
	TNetPoint tBPoint;			///< B���������
}NETPACKED;

struct TNetAreaCfg
{
	s32 nErrNo;									///< ����� NET_OK����ȡ�ɹ�
	BOOL32 bEnable;								///< �Ƿ�����
	s32 nCurSchemeId;							///< ��ǰ��������1��ʼ
	s32 nSchemeMaxNum;							///< �����������
	s32 nVgaWidth;								///< �������
	s32 nVgaHeight;								///< �����߶�
	BOOL32 bSupPreset;							///< �Ƿ�֧�ֹ���Ԥ��λ
	s32 nPresetId;								///< Ԥ��λid����0��ʼ��0����ʾ����Ԥ��λ
	s32 nPresetMaxNum;							///< Ԥ��λ���������������Ԥ��λ0
	s32 nTargetRatio;							///< Ŀ�����
	s32 nTargetRatioMin;						///< Ŀ�������Сֵ
	s32 nTargetRatioMax;						///< Ŀ��������ֵ
	s32 nSensi;									///< ������
	s32 nSensiMin;								///< ��������Сֵ
	s32 nSensiMax;								///< ���������ֵ
	BOOL32 bSupTimeThreshold;					///< �Ƿ�֧��ʱ����ֵ��������롢�����뿪��Ч
	s32 nTimeThreshold;							///< ʱ����ֵ��������롢�����뿪��Ч
	s32 nTimeThresholdMin;						///< ʱ����ֵ��Сֵ��������롢�����뿪��Ч
	s32 nTimeThresholdMax;						///< ʱ����ֵ���ֵ��������롢�����뿪��Ч
	TNetRect tTargetFilterRect;					///< Ŀ���������
	s32 nPointMaxNum;							///< ����������
	s32 nPointNum;								///< �������
	TNetPoint atPoint[NET_AREA_POINT_MAX_NUM];	///< point
}NETPACKED;

struct TNetVirFocusDetectCfg
{
	s32 nErrNo;     ///< ����� NET_OK����ȡ�ɹ�
	BOOL32 bEnable;	///< �Ƿ�����
	s32 nSensi;		///< ������
	s32 nSensiMin;	///< ��������Сֵ
	s32 nSensiMax;	///< ���������ֵ
}NETPACKED;

struct TNetSceneChangeCfg
{
	s32 nErrNo;				///< ����� NET_OK����ȡ�ɹ�
	BOOL32 bEnable;			///< �Ƿ�����
	ECheckMode eCheckMode;	///< ���ģʽ
	u32 dwCheckModeMask;	///< ���ģʽ���룬ECheckMode���ֵ
	s32 nSensi;				///< ������
	s32 nSensiMin;			///< ��������Сֵ
	s32 nSensiMax;			///< ���������ֵ
}NETPACKED;

struct TNetPeopleGatherCfg
{
	s32 nErrNo;									///< ����� NET_OK����ȡ�ɹ�
	BOOL32 bEnable;								///< �Ƿ�����
	s32 nCurSchemeId;							///< ��ǰ��������1��ʼ
	s32 nSchemeMaxNum;							///< �����������
	s32 nVgaWidth;								///< �������
	s32 nVgaHeight;								///< �����߶�
	BOOL32 bSupPreset;							///< �Ƿ�֧�ֹ���Ԥ��λ
	s32 nPresetId;								///< Ԥ��λid����0��ʼ��0����ʾ����Ԥ��λ
	s32 nPresetMaxNum;							///< Ԥ��λ���������������Ԥ��λ0
	s32 nRatio;									///< ռ��
	s32 nRatioMin;								///< ռ����Сֵ
	s32 nRatioMax;								///< ռ�����ֵ
	s32 nPointMaxNum;							///< ����������
	s32 nPointNum;								///< �������
	TNetPoint atPoint[NET_AREA_POINT_MAX_NUM];	///< point
}NETPACKED;

struct TNetAudioAbnormalCfg
{
	s32 nErrNo;						///< ����� NET_OK����ȡ�ɹ�
	BOOL32 bEnable;					///< �Ƿ�����
	s32 nSensi;						///< ������
	s32 nSensiMin;					///< ��������Сֵ
	s32 nSensiMax;					///< ���������ֵ
	s32 nSoundIntenThreshold;		///< ����ǿ����ֵ
	s32 nSoundIntenThresholdMin;	///< ����ǿ����ֵ��Сֵ
	s32 nSoundIntenThresholdMax;	///< ����ǿ����ֵ���ֵ
}NETPACKED;

struct TNetAudioMeter
{
	s32 nErrNo;						///< ����� NET_OK����ȡ�ɹ�
	s32 nAudMeter;					///< ��������ͼʵʱ��ֵ
}NETPACKED;

struct TNetFaceDetectCfg
{
	s32 nErrNo;						///< ����� NET_OK����ȡ�ɹ�
	BOOL32 bEnable;					///< �Ƿ�����
}NETPACKED;

struct TNetAccessProtectCfg
{
	s32 nErrNo;						///< ����� NET_OK����ȡ�ɹ�
	BOOL32 bEnable;					///< �Ƿ�����
}NETPACKED;

struct TNetRecMgrCap 
{
	BOOL32 bRecAudioCap;            ///< �Ƿ�֧��¼��Ƶ
	BOOL32 bStartAnrCap;            ///< �Ƿ�֧��ת¼
	BOOL32 bHolidayCap;             ///< �Ƿ�֧�ּ���
	s32 nHolidayNum;                ///< ���ո���
	u32 dwRecModeTypeMask;          ///< ¼��ģʽ�������룬ERecModeType���ֵ
	u32 dwRecStreamTypeMask;        ///< ¼�������������룬ERecStreamType���ֵ
	BOOL32 bSupThumbnail;           ///< �Ƿ�֧������ͼ
	BOOL32 bSupSlowRecover;         ///< �Ƿ�֧�ֵ���ʽ
	s32 nMaxSegPlayNum;             ///< �ֶλط����ֶ���
}NETPACKED;

struct TNetRecTimePlanItem
{
	s32 nStartTime;                       ///< ʱ��ο�ʼʱ��
	s32 nEndTime;                         ///< ʱ��ν���ʱ��
}NETPACKED;

struct TNetRecDayTimePlan
{
	ERecDayType eRecDayType;                                                  ///< ��������
	u16 wItemNum;                                                             ///< ����ʱ�����
	TNetRecTimePlanItem atRecTimePlanItem[NET_PRE_DAY_RECTIMEPLAN_MAX_NUM];   ///< ʱ�����Ϣ
}NETPACKED;

struct TNetChnRecPlan
{
	s32 nChnId;                                                   ///< ͨ��ID
	ERecModeType eRecModeType;                                    ///< ¼��ģʽ����
	ERecStreamType eRecStreamType;                                ///< ¼����������
	u32 dwRecStreamTypeMask;                                      ///< ¼��������������, ERecStreamType�������ֵ
	BOOL32 bRecAudio;                                             ///< �Ƿ��¼��Ƶ
	s32 nRecOverTime;                                             ///< ¼�����ʱ��,��λСʱ
	TNetRecDayTimePlan atRecDayTimePlan[NET_RECTIMEPLAN_MAX_NUM]; ///< ¼��ʱ��ƻ�
}NETPACKED;

struct TNetCopyChnRecPlan
{
	s32 nSrcChnId;                                                ///< Դͨ��ID
	s32 nDstChnNum;                                               ///< Ŀ��ͨ��ID����
	s32 anDstChnId[NET_COPYCHNRECPLAN_MAX_NUM];                   ///< Ŀ��ͨ��ID�б�
}NETPACKED;

struct TNetSnapTimePlanItem
{
	s32 nStartTime;                       ///< ʱ��ο�ʼʱ��
	s32 nEndTime;                         ///< ʱ��ν���ʱ��
}NETPACKED;

struct TNetSnapDayTimePlan
{
	ERecDayType eSnapDayType;                                                   ///< ��������
	u16 wItemNum;                                                               ///< ����ʱ�����
	TNetSnapTimePlanItem atSnapTimePlanItem[NET_PRE_DAY_RECTIMEPLAN_MAX_NUM];   ///< ʱ�����Ϣ
}NETPACKED;

struct TNetChnSnapPlan
{
	s32 nChnId;                                                     ///< ͨ��ID
	ESnapModeType eSnapModeType;                                    ///< ¼��ģʽ����
	s32 nAlarmInterval;                                             ///< �澯ץ�ļ��,��λ��
	s32 nMinAlarmInterval;                                          ///< �澯ץ����С���,��λ��
	s32 nMaxAlarmInterval;                                          ///< �澯ץ�������,��λ��
	BOOL32 bSupTimerInterval;										///< �Ƿ�֧�ֶ�ʱץ��
	s32 nTimerInterval;                                             ///< ��ʱץ�ļ��,��λ��
	s32 nMinTimerInterval;                                          ///< ��ʱץ����С���,��λ��
	s32 nMaxTimerInterval;                                          ///< ��ʱץ�������,��λ��
	TNetSnapDayTimePlan atSnapDayTimePlan[NET_RECTIMEPLAN_MAX_NUM]; ///< ¼��ʱ��ƻ�
}NETPACKED;

struct TNetCopyChnSnapPlan
{
	s32 nSrcChnId;                                                ///< Դͨ��ID
	s32 nDstChnNum;                                               ///< Ŀ��ͨ��ID����
	s32 anDstChnId[NET_COPYCHNRECPLAN_MAX_NUM];                   ///< Ŀ��ͨ��ID�б�
}NETPACKED;

struct TNetTime
{
	s32 nHour;                ///< ʱ
	s32 nMin;                 ///< ��
	s32 nSec;                 ///< ��
}NETPACKED;

struct TNetRecAdvanceOption
{
	BOOL32 bStopRecSpaceFull;                                     ///< ¼��ռ������Ƿ�ֹͣ
	BOOL32 bStopPicSpaceFull;                                     ///< ͼƬ�ռ������Ƿ�ֹͣ
	s32 nPreRecTime;                                              ///< Ԥ¼ʱ��
	s32 nMinPreRecTime;                                           ///< Ԥ¼ʱ����Сֵ
	s32 nMaxPreRecTime;                                           ///< Ԥ¼ʱ�����ֵ
	s32 nRecDelay;                                                ///< ¼����ʱ
	s32 nMinRecDelay;                                             ///< ¼����ʱ��Сֵ
	s32 nMaxRecDelay;                                             ///< ¼����ʱ���ֵ
	ERecDownloadSpeed eRecDownloadSpeed;                          ///< ¼�������ٶ�
	u32 dwRecDownloadSpeed;                                       ///< ¼�������ٶ����룬ERecDownloadSpeed���ֵ
	BOOL32 bEnableAnr;                                            ///< �Ƿ���ת¼
	BOOL32 bEnableLongTimeRecCfg;                                 ///< �Ƿ����ó���ʱ��¼������
	s32 nFullRecTime;                                             ///< ����¼��ʱ��
	s32 nEventRecTime;                                            ///< ʱ��¼��ʱ��
	s32 nSmallVideoPreTime;										  ///< С��Ƶ����ǰ������
	s32 nMinSmallVideoPreTime;									  ///< С��Ƶ����ǰ��������Сֵ
	s32 nMaxSmallVideoPreTime;									  ///< С��Ƶ����ǰ���������ֵ
	s32 nSmallVideoOverTime;									  ///< С��Ƶ���Ⱥ������
	s32 nMinSmallVideoOverTime;									  ///< С��Ƶ���Ⱥ��������Сֵ
	s32 nMaxSmallVideoOverTime;									  ///< С��Ƶ���Ⱥ���������ֵ
	BOOL32 bSupNoRecCheck;                                        ///< �Ƿ�֧����¼�����ò���
	s32 nNoRecDays;												  ///< ¼�񱣴�ʱ�����죩
	s32 nNoRecDaysMin;									          ///< ¼�񱣴�ʱ����Сֵ
	s32 nNoRecDaysMax;									          ///< ¼�񱣴�ʱ�����ֵ
	TNetTime tNoRecTime;										  ///< ¼�񱣴�ʱ��
}NETPACKED;

struct TNetHolidayTime
{
	s32 nYear;                   ///< �꣬���շ�ʽ��Ч
	s32 nMonth;                  ///< �£����շ�ʽ�����·�ʽ�����ܷ�ʽ��Ч
	s32 nDay;                    ///< �죬���շ�ʽ�����·�ʽ��Ч
	s32 nWeek;                   ///< �ڼ��ܣ����ܷ�ʽ��Ч
	s32 nWeekDay;                ///< �ܼ������ܷ�ʽ��Ч
}NETPACKED;

struct TNetHolidayCfg
{
	s32 nId;                                  ///< ����ID
	BOOL32 bEnable;                           ///< �Ƿ�����
	s8 szName[NET_MAXLEN_64 + 1];             ///< ��������
	EHolidaySelMode eHolidaySelMode;          ///< ����ѡ��ģʽ
	TNetHolidayTime tHolidayStartTime;        ///< ���տ�ʼʱ��
	TNetHolidayTime tHolidayEndTime;          ///< ���ս���ʱ��
}NETPACKED;

struct  TNetHolidayCfgList
{
	s32 nHolidayNum;                                                ///< ��������
	TNetHolidayCfg atHolidayCfg[NET_HOLIDAY_MAX_NUM];               ///< ������������
}NETPACKED;

struct  TNetRecBackupItem
{
	s32 nChnId;                             ///< ͨ��ID;
	s8 szStartTime[NET_MAXLEN_32];          ///< ��ʼʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	s8 szEndTime[NET_MAXLEN_32];            ///< ����ʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	s32 nStartTime;                         ///< ��ʼʱ��(��ѡ��ɲ���ʼ��)
	s32 nDur;                               ///< ¼�����ʱ��(��ѡ��ɲ���ʼ��)
}NETPACKED;

struct  TNetRecBackupData
{
	s32 nRecBackupItemNum;                                                          ///< ����������������
	TNetRecBackupItem atRecBackupItem[NET_PER_CHNGETRECLIST_MAX_NUM];               ///< ������������
}NETPACKED;

struct  TNetRecBackupStateItem
{
	u64 ullCurrTime;                                       ///< ��ǰ����ʱ�䣨Ϊ��ǰ���ƫ��ʱ�䣩
	s8 szCurrTime[NET_MAXLEN_32];                          ///< ��ǰ����ʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	EBackupState eRecBackupState;                          ///< ����״̬
}NETPACKED;

struct  TNetRecBackupState
{
	s32 nRecBackupStateItemNum;                                                          ///< ����������������
	TNetRecBackupStateItem atRecBackupStateItem[NET_PER_CHNGETRECLIST_MAX_NUM];          ///< ������������
}NETPACKED;

struct  TNetLockRecItem
{
	s32 nChnId;                            ///< ͨ��id
	s8 szStartTime[NET_MAXLEN_32];         ///< ��ʼʱ��
    s8 szEndTime[NET_MAXLEN_32];           ///< ����ʱ��
}NETPACKED;

struct  TNetLockRecList
{
	s32 nLockRecNum;                                                       ///< ����¼������
	TNetLockRecItem atLockRecItem[NET_PER_LOCK_REC_MAX_NUM];               ///< ����¼����������
}NETPACKED;

struct  TNetDataTime
{
	s32 nYear;                ///< ��
	s32 nMonth;               ///< ��
	s32 nDay;                 ///< ��
	s32 nHour;                ///< ʱ
	s32 nMin;                 ///< ��
	s32 nSec;                 ///< ��
	s32 nMSec;                ///< ����
	s8 szTime[NET_MAXLEN_32]; ///< ʱ��
}NETPACKED;

struct TNetDate
{
	s32 nYear;  ///< ��
	s32 nMonth; ///< ��
	s32 nDay;   ///< ��
}NETPACKED;

struct  TNetLockRecDataItem
{
	s32 nChnId;                     ///< ͨ��ID
	u64 ullLockRecSize;             ///< ¼�����ݴ�С
	TNetDataTime tStartTime;        ///< ��ʼʱ��
	TNetDataTime tEndTime;          ///< ����ʱ��
}NETPACKED;

struct  TNetLockRecData
{
	s32 nLockRecNum;                                                               ///< ����¼������
	TNetLockRecDataItem atLockRecDataItem[NET_PER_LOCK_REC_MAX_NUM];               ///< ¼�����ݴ�С����
}NETPACKED;

struct  TNetPicBackupItem
{
	s32 nChnId;                             ///< ͨ��ID;
	s8 szPicUri[NET_MAXLEN_256];            ///< ͼƬuri
}NETPACKED;

struct  TNetPicBackupData
{
	s32 nPicBackupItemNum;                                                          ///< ����������������
	TNetPicBackupItem atPicBackupItem[NET_PER_CHNGETRECLIST_MAX_NUM];               ///< ������������
}NETPACKED;

struct  TNetPicBackupStateItem
{
	EBackupState ePicBackupState;                       ///< ����״̬
}NETPACKED;

struct  TNetPicBackupState
{
	s32 nPicBackupStateItemNum;                                                          ///< ����������������
	TNetPicBackupStateItem atPicBackupStateItem[NET_PER_CHNGETRECLIST_MAX_NUM];          ///< ������������
}NETPACKED;

struct TNetGetChnMonthRec
{
	s32 nChnNum;                                       ///< ͨ������
	s32 anChnId[NET_PER_CHNGETMONTHREC_MAX_NUM];       ///< ͨ��id����
	s32 nYear;                                         ///< ���
	s32 nMonth;                                        ///< �·�
}NETPACKED;

struct TNetChnMonthRecItem
{
	s32 nChnId;                                        ///< ͨ��id
	u32 dwMonthRec;                                    ///< ����ͼ��Ϣ
}NETPACKED;

struct TNetChnMonthRecList
{
	s32 nChnNum;                                                             ///< ͨ������
	TNetChnMonthRecItem atChnMonthRecItem[NET_PER_CHNGETMONTHREC_MAX_NUM];   ///< ����ͼ��Ϣ
}NETPACKED;

struct TNetCreatQueryRecTask 
{
	s32 nChnId;                             ///< ͨ��id
	BOOL32 bQueryRecord;                    ///< �Ƿ��¼����
	s8 szStartTime[NET_MAXLEN_32];          ///< ��ʼʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	s8 szEndTime[NET_MAXLEN_32];            ///< ����ʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	u32 dwRecEventTypeMask;                 ///< ¼���¼��������룬ERecEventType���ֵ
	BOOL32 bMergedRec;                      ///< �Ƿ�ϲ�¼���¼
}NETPACKED;

struct TNetGetRecTaskResult 
{
	s32 nTaskId;                            ///< ����id
	s32 nStartIndex;                        ///< ��ʼ��¼����
	s32 nNeedNum;                           ///< ϣ����ȡ������¼����
}NETPACKED;

struct TNetChnRecItem
{
	u64 ullRecSize;                          ///< ¼���С,��λBYTE
	u64 ullStartTime;                        ///< ��ʼʱ��(��λ������)
	s8 szStartTime[NET_MAXLEN_32];           ///< ��ʼʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	u32 dwRecDur;                            ///< ¼��ʱ��(��λ������)
	ERecEventType eRecEventType;             ///< ¼���¼�����
	BOOL32 bLocked;                          ///< ¼���Ƿ�����
	s8 szThumbnailUrl[NET_MAXLEN_512 + 1];   ///< ¼���¼��Ӧ����ͼURL
}NETPACKED;

struct TNetChnRecList
{
	BOOL32 bFinished;                                            ///< �Ƿ����
	s32 nTotalRecNum;                                            ///< �ܵ�¼������
	s32 nCurrRecNum;                                             ///< ��ǰ¼������
	s32 nRealRecNum;                                             ///< �ϲ�ǰ¼������
	TNetChnRecItem atChnRecItem[NET_PER_CHNGETRECLIST_MAX_NUM];  ///< ͨ��¼������
}NETPACKED;

struct TNetPlaybackItem
{
	s32 nWinId;                              ///< ����Id
	s32 nChnId;                              ///< ͨ��Id
	s8 szStartTime[NET_MAXLEN_32];           ///< ��ʼʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	s8 szEndTime[NET_MAXLEN_32];             ///< ����ʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	s8 szResStartTime[NET_MAXLEN_32];        ///< ��Դ��ʼʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	s8 szResEndTime[NET_MAXLEN_32];          ///< ��Դ����ʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	s8 szUri[NET_MAXLEN_512];            ///< �ļ�·����asf��mp4�طţ� 
}NETPACKED;

struct TNetPlaybackList
{
	s32 nItemNums;                                                      ///< item����
	TNetPlaybackItem atPlaybackItem[NET_PER_CHNPLAYBACKLIST_MAX_NUM];   ///< item��Ϣ
}NETPACKED;

struct TNetRecFileInfo
{
	s8 szUri[NET_MAXLEN_512];                ///< �ļ�·����asf��mp4�طţ�
	u64 ullFileSize;                         ///< �ļ���С
	u64 ullStartTime;                        ///< ¼��ʼʱ��(��λ������)
	s32 nDuration;                           ///< ¼��ʱ��
}NETPACKED;

struct TNetPlaybackVcrCtrl
{
	s32 nTaskId;                              ///< ����Id
	s32 nChnId;                               ///< ͨ��Id
	BOOL32 bBackwards;                        ///< �Ƿ񵹷�
	EPlaybackVcrCmdType ePlaybackVcrCmdType;  ///< vcr��������
	EPlaybackFrameMode ePlaybackFrameMode;    ///< �ط�֡ģʽѡ�񣨷ǵ����ٶȵ�vcr���Ʋ���Ч��
	s8 szSeekTime[NET_MAXLEN_32];             ///< Ҫ��ק���ı���ʱ��,��ȷ������(��ʽΪ:2016-03-15T12:48:01.330)
	s32 nSkipTime;                            ///< ǰ������ʱ����
}NETPACKED;

struct TNetPlaybackState
{
	u64 ullPlaybackTime;                      ///< ����ʱ��(��λ������)
	EPlaybackVcrState ePlaybackState;         ///< �طŻ���״̬
	s8 szCurrPlayTime[NET_MAXLEN_32];         ///< ��ǰ����λ��,��ȷ������(��ʽΪ:2016-03-15T12:48:01.330)
	s32 nCurrRtpTime;                         ///< rtpʱ��
}NETPACKED;

struct TNetEventCyclePlaybackInfo
{
	s32 nTaskId;                              ///< ����Id
	s32 nChnId;                               ///< ͨ��Id(ͬ����ʽ�ò�����Ч)
	s8 szStartTime[NET_MAXLEN_32];            ///< ��ʼʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	s8 szEndTime[NET_MAXLEN_32];              ///< ����ʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	BOOL32 bFilePlayback;                         ///< �Ƿ��ļ��ط�
}NETPACKED;

struct TNetQuerySnapPicInfo
{
	s32 nChnId;                               ///< ͨ��Id
	s8 szStartTime[NET_MAXLEN_32];            ///< ��ʼʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	s8 szEndTime[NET_MAXLEN_32];              ///< ����ʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	ESnapPicEventType eSnapPicEventType;      ///< ��ѯ��ץ��ͼƬ����
	s32 nStartIndex;                          ///< Ҫ��ѯ����ʼ����
	s32 nNeedNum;                             ///< ������Ҫ��ѯ������   
}NETPACKED;

struct TNetSnapPicInfo 
{
	s8 szUri[NET_MAXLEN_512 + 1];              ///< ͼƬΨһ��ʶ
	s8 szSnapPicTime[NET_MAXLEN_32];           ///< ͼƬץ��ʱ��
	u32 dwSnapPicEventMask;                    ///< ץ��ͼƬ�¼����ͣ�ESnapPicEventType���ֵ
	s32 nPicSize;                              ///< ͼƬ��С����λ�ֽ�
	s32 nPicWidth;                             ///< ͼƬ���
	s32 nPicHeight;                            ///< ͼƬ�߶�
}NETPACKED;

struct TNetSnapPicList
{
	BOOL32 bFinished;                                             ///< ��ѯ�Ƿ����
	s32 nTotalNum;                                                ///< �ܵ�ͼƬ����
	s32 nCurrNum;                                                 ///< ����ͼƬ����
	TNetSnapPicInfo atSnapPicInfo[NET_PER_QUERYSNAPPIC_MAX_NUM];  ///< ͼƬ��Ϣ����
}NETPACKED;

struct TNetGetSnapPic
{
	s32 nChnId;									///< ͨ��Id
	s8 szUri[NET_MAXLEN_512 + 1];               ///< ͼƬΨһ��ʶ
}NETPACKED;

struct TNetRecTagInfo 
{
	s32 nChnId;                         ///< ͨ��ID
	s8 szUri[NET_MAXLEN_256];           ///< ��ǩΨһ��ʶ
	s8 szTime[NET_MAXLEN_32];           ///< ��ǩʱ��
	s32 nTime;                          ///< ����ʱ��ƫ��
	s8 szTagName[NET_MAXLEN_64 + 1];    ///< ��ǩ���֣����ݣ�
}NETPACKED;

struct  TNetRecTagList
{
	s32 nNum;                                                   ///< ����
	TNetRecTagInfo atRecTagInfo[NET_PER_QUERYRECTAG_MAX_NUM];   ///< ��ǩ��Ϣ����
}NETPACKED;

struct  TNetRecTagOperateParam
{
	ERecTagOperaType eRecTagOperaType;                ///< ��ǩ��������
	TNetRecTagInfo tRecTagInfo;                       ///< ¼���ǩ��Ϣ
	TNetRecTagInfo tOldRecTagInfo;                    ///< �ɵ�¼���ǩ��Ϣ��TAGOPERATE_UPDATE����ʱ��Ч��
}NETPACKED;

struct  TNetCreateQueryRecTagTaskParam
{
	s32 nChnId;                              ///< ͨ��ID
	s8 szStartTime[NET_MAXLEN_32];           ///< ��ʼʱ��
	s8 szEndTime[NET_MAXLEN_32];             ///< ����ʱ��
	s8 szTagKey[NET_MAXLEN_64 + 1];          ///< ��ǩ�����ؼ���
}NETPACKED;

struct TNetQueryRecTagResultParam
{
	s32 nTaskId;                         ///< ͨ��ID
	s32 nBeginIndex;                     ///< ��ʼ����
	s32 nQueryNum;                       ///< ��ѯ����
}NETPACKED;

struct  TNetQueryRecTagResult
{
	BOOL32 bFinished;                        ///< ��ѯ�Ƿ����
	TNetRecTagList tRecTagList;              ///< ¼���ǩ�б�            
}NETPACKED;

struct TNetDstVideoItem
{
	u16 wDstRtpPort;	///< Ŀ��rtp�˿�
	u16 wDstRtcpPort;	///< Ŀ��rtcp�˿�
	u8 byPayload;		///< payload
}NETPACKED;

struct TNetDstAudioItem
{
	u16 wDstRtpPort;	///< Ŀ��rtp�˿�
	u16 wDstRtcpPort;	///< Ŀ��rtcp�˿�
	u8 byPayload;		///< payload
}NETPACKED;

struct TNetPlaybackDstChnItem
{
	s32 nChnId;													///< ͨ��id
	s8 szStartTime[NET_MAXLEN_32];								///< ��ʼʱ��
	s8 szEndTime[NET_MAXLEN_32];								///< ����ʱ��
	s8 szResStartTime[NET_MAXLEN_32];							///< ��Դ��ʼʱ��
	s8 szResEndTime[NET_MAXLEN_32];								///< ��Դ����ʱ��
	u32 dwDstIp;												///< ��������Ŀ�ĵ�ַ��������
	s32 nDstVideoItemNum;										///< ��Ƶ����
	TNetDstVideoItem atDstVideoItem[NET_PLAYBACK_ITEM_MAX_NUM];	///< ��Ƶ��Ϣ
	s32 nDstAudioItemNum;										///< ��Ƶ����
	TNetDstAudioItem atDstAudioItem[NET_PLAYBACK_ITEM_MAX_NUM];	///< ��Ƶ��Ϣ
}NETPACKED;

struct TNetPlaybackDstChnList
{
	s32 nNum;															///< ����
	TNetPlaybackDstChnItem atPlaybackDstChnItem[NET_PLAYBACK_MAX_NUM];	///< ¼��ط�ͨ����Ϣ
}NETPACKED;

struct TNetSrcVideoItem
{
	u16 wSrcRtpPort;	///< Դrtp�˿�
	u16 wSrcRtcpPort;	///< Դrtcp�˿�
}NETPACKED;

struct TNetSrcAudioItem
{
	u16 wSrcRtpPort;	///< Դrtp�˿�
	u16 wSrcRtcpPort;	///< Դrtcp�˿�
}NETPACKED;

struct TNetPlaybackSrcChnItem
{
	s32 nChnId;													///< ͨ��id
	s32 nSrcVideoItemNum;										///< ��Ƶ����
	TNetSrcVideoItem atSrcVideoItem[NET_PLAYBACK_ITEM_MAX_NUM];	///< ��Ƶ��Ϣ
	s32 nSrcAudioItemNum;										///< ��Ƶ����
	TNetSrcAudioItem atSrcAudioItem[NET_PLAYBACK_ITEM_MAX_NUM];	///< ��Ƶ��Ϣ
}NETPACKED;

struct TNetPlaybackSrcChnList
{
	s32 nNum;															///< ����
	TNetPlaybackSrcChnItem atPlaybackSrcChnItem[NET_PLAYBACK_MAX_NUM];	///< ¼��ط�ͨ����Ϣ
}NETPACKED;

struct TNetDiskMgrCap
{
	u32 dwNetDiskType;	          ///< ֧�ֵ�����Ӳ�����ͣ�ENetDiskType���ֵ
	BOOL32 bSupRaid;              ///< �Ƿ�֧��raid
	u32 dwRaidLevel;	          ///< ֧�ֵ�RAID�ļ���ERaidLevel���ֵ
	u32 dwSmartTestDiskType;      ///< ֧��SMART����Ӳ�����ͣ�EDiskType���ֵ
	u32 dwBadSectorCheckDiskType; ///< ֧�ֻ���������Ӳ�����ͣ�EDiskType���ֵ
	BOOL32 bSupDiskCust;		  ///< �Ƿ�֧���Զ������
	BOOL32 bSupSmartStorage;	  ///< �Ƿ�֧�����ܴ洢
}NETPACKED;

struct TNetDiskPartCap
{
	u8 byId;					///< ����id
	EDiskPartType ePartType;	///< ��������
}NETPACKED;

struct TNetDiskPartType
{
	u8  byDiskId;														///< Ӳ��id
	s32 nNum;															///< ��������
	TNetDiskPartCap atDiskPartCap[NET_DISK_PART_MAX_NUM];				///< ��������
}NETPACKED;

struct TNetDiskPartTypeList
{
	s32 nNum;															///< ���̸���
	TNetDiskPartType atDiskPartType[NET_NVR_DISK_MAX_NUM];				///< ������;
}NETPACKED;

struct TNetDiskPartTypeParam
{
	BOOL32 bAll;									///< ��ʾ��ȡȫ��ID
	s32 nNum;										///< ���̸���
	u8  abyDiskId[NET_NVR_DISK_MAX_NUM];			///< Ӳ��id 
}NETPACKED;

struct TNetDiskPartInfo
{
	u8 byId;					///< ����id
	s32 nPartSize;				///< ������С
	s32 nFreeSize;				///< ʣ���С
}NETPACKED;

struct TNetDiskInfo
{
	u8  byDiskId;					///< Ӳ��id
	s32 nDiskCapacity;				///< Ӳ����������λMB
	EDiskState eDiskState;			///< Ӳ��״̬
	EDiskAttribute eDiskAttribute;	///< Ӳ������
	BOOL32 bAttrCanModify;          ///< Ӳ�������Ƿ�ɱ༭
	EDiskType eDiskType;			///< Ӳ������
	s32	nDiskFreeCapacity;			///< Ӳ��ʣ����������λMB
	u8	byDiskGroupId;				///< ����id
	EDiskSmartTestState eDiskSmartTestState; ///< Ӳ��smart���״̬
	s32 nPartNum;					///< ��������
	s32 nPartMax;					///< ��������
	TNetDiskPartInfo atNetDiskPartInfo[NET_DISK_PART_MAX_NUM];	///< ������Ϣ
	EDiskUsingType eDiskUsingType;	///< Ӳ�̴洢��;
	s8 szNetDiskName[NET_MAXLEN_64 + 1];    ///< �����������
	s8 szNetDiskIpAddr[NET_MAXLEN_32];    ///< �������IP��ַ
}NETPACKED;

struct TNetDiskList
{
	s16 swDiskNum;                                 ///< Ӳ������
	TNetDiskInfo atDiskInfo[NET_NVR_DISK_MAX_NUM]; ///< Ӳ����Ϣ
}NETPACKED;

struct TNetDiskAttributeInfo
{
	u8  byDiskId;					///< Ӳ��id
	EDiskAttribute eDiskAttribute;	///< Ӳ������
}NETPACKED;

struct TNetDiskAttributeList
{
	s16 swDiskNum;                                          ///< Ӳ������
	TNetDiskAttributeInfo atDiskInfo[NET_NVR_DISK_MAX_NUM]; ///< Ӳ����Ϣ
}NETPACKED;

struct TNetDiskUsingTypeParam
{
	u8  byDiskId;					///< Ӳ��id
	EDiskUsingType eDiskUsingType;	///< Ӳ�̴洢��;
}NETPACKED;

struct TNetDiskUsingTypeList
{
	s32 nDiskNum;                                          ///< Ӳ������
	TNetDiskUsingTypeParam atDiskUsingTypeParam[NET_NVR_DISK_MAX_NUM];	///< Ӳ�̴洢��;
}NETPACKED;

struct TNetDiskInitParam
{
	u8  byDiskId;					///< Ӳ��id
	s32 nPartNum;					///< ��������
	TNetDiskPartInfo atNetDiskPartInfo[NET_DISK_PART_MAX_NUM];	///< ������Ϣ
}NETPACKED;

struct TNetNetDiskParam
{
	ENetDiskType eNetDiskType;				            ///< ����Ӳ������
	s8  szDiskIp[NET_MAXLEN_32 + 1];		            ///< Ӳ��IP
	u16 wDiskPort;							            ///< Ӳ�̶˿�
	s8  szUserName[NET_NVR_USER_NAME_MAX_LEN + 1];		///< �û���
	s8  szPwd[NET_MAXLEN_32 + 1];			            ///< ����
	s8  szNetDiskName[NET_MAXLEN_128 + 1];	            ///< ����Ӳ�����ƣ��������������Ӳ��
}NETPACKED;

struct TNetNetDiskInfo
{
	s8 szNetDiskName[NET_MAXLEN_128 + 1];	///< ����Ӳ������
}NETPACKED;

struct TNetNetDiskList
{
	BOOL32 bFinished;	                                         ///< �����Ƿ����
	u8 byNetDiskNum;                                             ///< ����Ӳ������
	TNetNetDiskInfo atNetDiskInfo[NET_PER_GET_NETDISK_MAX_NUM];  ///< ����Ӳ����Ϣ
}NETPACKED;

struct TNetDiskQuotaCfg
{
	s32 nChnId;                     ///< ͨ��ID
	s32 nRecQuota;					///< ¼�����
	s32 nRecUsedQuota;				///< ¼���������
	s32 nPicQuota;					///< ͼƬ���
	s32 nPicUsedQuota;				///< ͼƬ�������
	s32 nDigestQuota;			    ///< ժҪ���
	s32 nDigestUsedQuota;			///< ժҪ�������
	u64 ullTotalFreeQuotaSize;      ///< ʣ�����������(��λMB)
	u64 ullTotalDiskSize;           ///< ����������(��λMB)
}NETPACKED;

struct TNetChnDiskQuotaCopyInfo 
{
	s32 nSrcChn;                              ///< Դͨ��ID
	s32 nDstNum;                              ///< Ŀ��ͨ����Ŀ
	s32 anDstChn[NET_COPYDISKQUOTA_MAX_NUM];  ///< Ŀ��ͨ��
}NETPACKED;

struct TNetChnDiskGroupCfg
{
	s32 nDiskGroupId;                           ///< Ӳ�̷���ID
	s32 nChnNum;                                ///< ͨ������
	s32 anChnId[NET_DISKGROUP_CHN_MAX_NUM];     ///< ͨ��id����
}NETPACKED;

struct TNetChnDiskGroupList
{
	s32 nNum;                                                             ///< ����
	TNetChnDiskGroupCfg atChnDiskGroupCfg[NET_GROUP_MAX_NUM];             ///< ͨ��id����
}NETPACKED;

struct  TNetDiskAdvanceCfg
{
	BOOL32 bEnableDiskSleep;   ///< �Ƿ�����Ӳ������
	s32 nDiskSleepTime;        ///< ���ж���ʱ�俪��Ӳ������
	s32 nDiskSleepTimeMax;     ///< �����ʱ�俪��Ӳ������
	s32 nDiskSleepTimeMin;     ///< �������ʱ�俪��Ӳ������
}NETPACKED;

struct TNetRaidDiskInfo
{
	s32 nDiskId;              ///< Ӳ��id
	BOOL32 bHotBackupDisk;    ///< �Ƿ�Ϊ�ȱ���
}NETPACKED;

struct TNetCreateRaidParam
{
	ERaidLevel eRaidLevel;                                      ///< Raid�ȼ�
	ERaidCreateType eRaidCreateType;                            ///< ������ʽ
	s8 szRaidName[NET_MAXLEN_64 + 1];                           ///< Raid����
	s32 nDiskNum;                                               ///< Ӳ������
	TNetRaidDiskInfo atRaidDiskInfo[NET_INTERNALDISK_MAX_NUM];  ///< Ӳ����Ϣ����
}NETPACKED;

struct TNetRaidInfo
{
	ERaidLevel eRaidLevel;                                      ///< Raid�ȼ�
	s8 szRaidName[NET_MAXLEN_64 + 1];                           ///< Raid����
	u64 ullRaidSize;                                            ///< raid�ռ��С
	ERaidStatus eRaidStatus;                                    ///< raid״̬
	s32 nDiskNum;                                               ///< Ӳ������
	TNetRaidDiskInfo atRaidDiskInfo[NET_INTERNALDISK_MAX_NUM];  ///< Ӳ����Ϣ����
}NETPACKED;

struct TNetFrameCap
{
	u32 dwOutputTypeMask;				///< ����������룬EOutputType���ֵ
	s32 nGridItemMaxNum;				///< �����񴰸��������
	s8 szFrameStyleMask[NET_MAXLEN_64];	///< ���������룬���磺1,4,6,8,9
}NETPACKED;

struct TNetGridItem
{
	BOOL32 bShow;						///< �����Ƿ���ʾ
	u16 wGridId;                        ///< grid id
	s32 nX;                             ///< grid left
	s32 nY;                             ///< grid top
	s32 nWidth;                         ///< grid width
	s32 nHeight;                        ///< grid height
}NETPACKED;

struct  TNetGridList
{
	s32 nFrameX;                                           ///< �������������Ͻ�X����
	s32 nFrameY;                                           ///< �������������Ͻ�Y����
	s32 nFrameWidth;                                       ///< �����������
	s32 nFrameHeight;                                      ///< �����������
	EFrameStyle eFrameStyle;                               ///< ������
	BOOL32 bSaveCfg;                                       ///< �Ƿ񱣴浽����
	BOOL32 bStartAllMonitor;							   ///< �Ƿ���ȫ�����
	BOOL32 bNotDestroyDec;			    				   ///< �Ƿ����ٽ�����
	u16 wGridNum;                                          ///< ������
	TNetGridItem atGridItem[NET_FRAME_STYLE_GRID_MAX_NUM]; ///< ������Ϣ
}NETPACKED;

struct TNetBindItem
{
	u16 wGridId;		///< grid id
	s32 nChnId;			///< chn id
	BOOL32 bNotSave;	///< �Ƿ񲻱���
}NETPACKED;

struct TNetBindList
{
	u16 wBindNum;                                ///< ������
	TNetBindItem atBindItem[NET_BIND_MAX_NUM];   ///< ����Ϣ����
}NETPACKED;

struct TNetPipItem
{
	BOOL32 bSetPip;		///< �Ƿ����û��л�
	EPipType ePipType;  ///< ���л�����
	s32 nTaskId;        ///< �ط�����ID
	s32 nChnId;			///< ͨ��id
	s32 nX;				///< ���Ͻ�����(ż��)
	s32 nY;				///< ���Ͻ�����(ż��)
	s32 nWidth;			///< ��(ż��)
	s32 nHeight;		///< ��(ż��)
}NETPACKED;

struct TNetPipList
{
	u16 wPipNum;							///< ���л�����
	TNetPipItem atPipItem[NET_PIP_MAX_NUM];	///< ���л�����
}NETPACKED;

struct TNetChnSnap
{
	s32 nChnId;						///< ͨ��id
	EAiuSnapType eAiuSnapType;		///< ץ������
	s32 nSnapNum;                   ///< ץ������
}NETPACKED;

struct TNetChnSnapFileInfo
{
	u64 ullFileId;        ///< ץ�ĵ��ļ�id
	s32 nErrCode;         ///< ץ�Ĵ�����
}NETPACKED;

struct TNetVolume
{
	BOOL32 bMute; ///< �Ƿ���
	s32 nVolume;  ///< ����
}NETPACKED;

struct TNetAudioDec
{
	BOOL32 bMonitor;			///< TRUE�������Ƶ���룬FALSE���ط���Ƶ����
	s32 nChnId;					///< ͨ��ID
	EAudioListen eAudioListen;	///< ��Ƶͨ��
	s32 nPlaybackTaskId;		///< �ط�����id
}NETPACKED;

struct TNetCallCap
{
	u32 dwAudioEncTypeMask;                                                 ///< ��Ƶ�����������룬EAudioEncType���ֵ
	u16 wEncTypeNum;                                                        ///< ������������
	TNetAudioEncCapItem atAudioEncCapItem[NET_AUDIOENC_ENCTYPE_MAX_NUM];    ///< ��������������Ϣ�б�wEncVolMin��wEncVolMin�ֶ���Ч
	u32 dwTransTypeMask;													///< ���䷽ʽ���룬ETransType���ֵ
	u32 dwStreamPackTypeMask;												///< ���������ʽ���룬EStreamPackType���ֵ
}NETPACKED;

struct TNetCall
{
	s32 nCallId;         ///< ����id����ʼ���з���
	ECallMode eCallMode; ///< ���з�ʽ�����غ��С����ع㲥
	ECallType eCallType; ///< �������ͣ�����ͨ��
	s32 nChnId;			 ///< ����ͨ��id
}NETPACKED;

struct TNetRemoteCallParam
{
	EAudioEncType eAudioEncType;				///< ��Ƶ��������
	EAudioSampleRateType eAudioSampleRateType;	///< ���������
	ETransType eTransType;						///< ���䷽ʽ
	EStreamPackType eStreamPackType;			///< ���������ʽ
	u32 dwIp;									///< ���ж˵�IP��������
	u16 wRtpPort;								///< ���ж˵�rtp�˿�
	u16 wRtcpPort;								///< ���ж˵�rtcp�˿�
}NETPACKED;

struct TNetCallParam
{
	ECallMode eCallMode;					///< ���з�ʽ
	ECallType eCallType;					///< ��������
	s32 nChnId;								///< ����ͨ��id
	TNetRemoteCallParam tRemoteCallParam;	///< Զ�˺��в�����Զ�˺���ʱ��Ч
}NETPACKED;

struct TNetRemoteCallResult
{
	u16 wRtpPort;	///< nvr������Ƶ��rtp�˿�
	u16 wRtcpPort;	///< nvr������Ƶ��rtcp�˿�
}NETPACKED;

struct TNetCallResult
{
	s32 nCallId;							///< ����id
	TNetRemoteCallResult tRemoteCallResult;	///< Զ�˺��в�����Զ�˺���ʱ��Ч
}NETPACKED;

struct TNetPtzCtrl
{
	s32 nChnId;			///< ͨ��id
	EPTZCMD ePtzCmd;	///< ��̨��������
	s32 nIspSpeed;		///< ISP��������ٶ�(0~100)��������PTZCMD_ZOOMTELE��PTZCMD_ZOOMWIDE��PTZCMD_FOCUSFAR��PTZCMD_FOCUSNEAR
	s32 nPanSpeed;		///< ˮƽת���ٶ�(0~100)��������PTZCMD_MOVELEFT��PTZCMD_MOVERIGHT��PTZCMD_MOVELEFTUP��PTZCMD_MOVELEFTDOWN��PTZCMD_MOVERIGHTUP��PTZCMD_MOVERIGHTDOWN
	s32 nTilSpeed;		///< ��ֱת���ٶ�(0~100)��������PTZCMD_MOVEUP��PTZCMD_MOVEDOWN��PTZCMD_MOVELEFTUP��PTZCMD_MOVELEFTDOWN��PTZCMD_MOVERIGHTUP��PTZCMD_MOVERIGHTDOWN
	s32 nNumber;		///< Ԥ��λ(0~255)��Ѳ��·��(1~32)��������PTZCMD_PRESET_CALL��PTZCMD_PRESET_SAVE��PTZCMD_PRESET_DEL��PTZCMD_PATHCRUISE_CALL��PTZCMD_PATHCRUISE_STOP
	s32 nX;				///< ��ѡ����ˮƽ����(ȡֵ��Χ��0~255 ������PTZCMD_GOTO_POINT��PTZCMD_ZOOM_PART��PTZCMD_ZOOM_WHOLE)
	s32 nY;				///< ��ѡ���Ŵ�ֱ����(ȡֵ��Χ��0~255 ������PTZCMD_GOTO_POINT��PTZCMD_ZOOM_PART��PTZCMD_ZOOM_WHOLE)
	s32 nWidth;			///< ��ѡ���ſ�(ȡֵ��Χ��0~255 ������PTZCMD_GOTO_POINT��PTZCMD_ZOOM_PART��PTZCMD_ZOOM_WHOLE)
	s32 nHeight;		///< ��ѡ���Ÿ�(ȡֵ��Χ��0~255 ������PTZCMD_GOTO_POINT��PTZCMD_ZOOM_PART��PTZCMD_ZOOM_WHOLE)
	s32 nWinWidth;		///< �Զ����ѡ���ſ�������PTZCMD_GOTO_POINT��PTZCMD_ZOOM_PART��PTZCMD_ZOOM_WHOLE
	s32 nWinHeight;		///< �Զ����ѡ���Ÿߣ�������PTZCMD_GOTO_POINT��PTZCMD_ZOOM_PART��PTZCMD_ZOOM_WHOLE
}NETPACKED;

struct TNetPtzPreset
{
	s32 nPresetId;									///< Ԥ��λID
	BOOL32 bEnable;									///< Ԥ�õ�ʹ��
	s8 szName[NET_MAXLEN_64 + 1];					///< Ԥ��λ����
	BOOL32 bSpecial;								///< �Ƿ�Ϊ����Ԥ��λ(����ʱ�����˽ڵ�)
}NETPACKED;

struct TNetPtzPresetList
{
	s32 nMax;											///< ֧�ֵ����Ԥ��λ����
	s32 nNum;											///< Ԥ��λ����	
	TNetPtzPreset atPtzPreset[NET_PTZ_PRESET_MAX_NUM];  ///< Ԥ��λ�б�
}NETPACKED;

struct TNetFishEyeModeCfg
{
	s32 nChnId;							///< ͨ��id
	EFishInstallType eFishInstallType;	///< ���۰�װ��ʽ
	u32 dwFishInstallTypeMask;			///< ���۰�װ��ʽȡֵ��Χ��EFishInstallType���ֵ
	BOOL32 bSupDevHardInfo;				///< �Ƿ�֧������Ӳ��
	u32 dwFishDisplayTypeMask;			///< ��ʾ��ʽȡֵ��Χ��EFishDisplayType���ֵ(������ǽ�棬���湲��)
	EFishDisplayType eCeilDisplayType;	///< ������ʾ��ʽ
	EFishDisplayType eMtDisplayType;	///< ǽ����ʾ��ʽ
	EFishDisplayType eDtDisplayType;	///< ������ʾ��ʽ
}NETPACKED;

struct TNetFishEyePtzRect
{
	s32 nIndex;				///< ����
	s32 nX;					///< �������ĵ��x����
	s32 nY;					///< �������ĵ��y����
}NETPACKED;

struct TNetFishEyePtzRectCfg
{
	s32 nChnId;													///< ͨ��id
	s32 nRectNum;											    ///< ���۵�ptz�������
	TNetFishEyePtzRect atNetFishEyePtzRect[NET_FISH_EYE_RECT_MAX_NUM];		///< ���۵�ptz����
	s32 nCanavsWidth;											///< ������
	s32 nCanavsHeight;											///< ������
}NETPACKED;

struct TNetIspCaps
{
	s32 nChnId;								///< ͨ��id
	BOOL32 bSupImageAdjustment;				///< �Ƿ�֧��ͼ��Ч��
	u32 dwIspModeMask;						///< ֧�ֵ�ISPģʽ(EIspNeedMask���ֵ)
}NETPACKED;

struct TNetIspCfg
{
	s32 nChnId;			///< ͨ��id
	s32 nBright;		///< ����
	s32 nContrast;		///< �Աȶ�
	s32 nSaturation;	///< ���Ͷ�
	s32 nSharpness;		///< ���
}NETPACKED;

struct TNetIspGainCfg
{
	s32 nChnId;					///< ͨ��id
	u32 dwGainModeMask;			///< ֧�ֵ�����ģʽ(EIspGainMode���ֵ)
	EIspGainMode eGainMode;		///< ����ģʽ
	s32 nGainLevel;				///< ����ȼ�,�ֶ�ģʽ����Ч(����ʱ��ѡ)(ȡֵ��Χ[0,100])
	s32 nGainMax;				///< ��������,�Զ�ģʽ����Ч(����ʱ��ѡ)(ȡֵ��Χ[0,100])
}NETPACKED;

struct TNetIspIrisCfg
{
	s32 nChnId;					///< ͨ��id
	u32 dwIrisModeMask;			///< ֧�ֵĹ�Ȧģʽ(EIspIrisMode���ֵ)
	EIspIrisMode eIrisMode;		///< ��Ȧģʽ
	s32 nIrisSize;				///< ��Ȧ��С(����ʱ��ѡ)(ȡֵ��Χ[0,100])
	s32 nIrisLevel;				///< ��Ȧ������(����ʱ��ѡ)(ȡֵ��Χ[0,100])
}NETPACKED;

struct TNetIspFocusCfg
{
	s32 nChnId;					///< ͨ��id
	u32 dwFocusModeMask;		///< ֧�ֵľ۽�ģʽ(EIspFocusMode���ֵ)
	EIspFocusMode eFocusMode;	///< �۽�ģʽ
	u32 dwFocusMinMask;			///< ֧�ֵ���С�۽�����(EIspFocusMin���ֵ)
	EIspFocusMin eFocusMin;		///< ��С�۽�����(����ʱ��ѡ)
}NETPACKED;

struct TNetIspShutterCfg
{
	s32 nChnId;						///< ͨ��id
	u32 dwShutterModeMask;			///< ֧�ֵĿ���ģʽ(EIspShutterMode���ֵ)
	EIspShutterMode eShutterMode;	///< ����ģʽ
	u32 dwShutterMinMask;			///< ֧�ֵ���С�۽�����(EIspShutterOpt���ֵ)
	EIspShutterOpt eShutterMin;		///< ��������,�Զ�ģʽ����Ч(����ʱ��ѡ)
	u32 dwShutterLevelMask;			///< ֧�ֵ���С�۽�����(EIspShutterOpt���ֵ)
	EIspShutterOpt eShutterLevel;	///< ���ŵȼ�,�ֶ�ģʽ����Ч(����ʱ��ѡ)
}NETPACKED;

struct TNetIspFreqModeCfg
{
	s32 nChnId;					///< ͨ��id
	u32 dwFreqModeMask;			///< ֧�ֵķ���˸ģʽ(EIspFreqMode���ֵ)
	EIspFreqMode eFreqMode;		///< ����˸ģʽ
}NETPACKED;

struct TNetIspWhiteBalanceCfg
{
	s32 nChnId;									///< ͨ��id
	u32 dwWhiteBalanceMask;						///< ֧�ֵķ���˸ģʽ(EIspWhiteBalance���ֵ)
	EIspWhiteBalance eWhiteBalanceMode;			///< ����˸ģʽ
	s32 nWBalanceRed;							///< ������,�ֶ�ģʽ����Ч(����ʱ��ѡ)(ȡֵ��Χ[0,100])
	s32 nWBalanceBlue;							///< ������,�ֶ�ģʽ����Ч(����ʱ��ѡ)(ȡֵ��Χ[0,100])
}NETPACKED;

struct TNetIspFixedtimeDay
{
	EDayType eDayType;							///< ���ڼ�����֧�ּ���
	BOOL32 bEnable;								///< �Ƿ���
	s8 szStartTime[NET_MAXLEN_32];				///< ��ʼʱ��
	s8 szEndTime[NET_MAXLEN_32];				///< ����ʱ��
}NETPACKED;

struct TNetIspFixedtimeAllDay
{
	BOOL32 bEnable;							 ///< �Ƿ�ÿ��
	s8 szStartTime[NET_MAXLEN_32];           ///< ��ʼʱ��
	s8 szEndTime[NET_MAXLEN_32];             ///< ����ʱ��
}NETPACKED;

struct TNetIspDayNightSwitchFixedtime
{
	TNetIspFixedtimeAllDay	tIspFixedtimeEveryDay;								///< ÿ��
	s32 nNum;																	///< ����
	TNetIspFixedtimeDay atIspFixedtimeDay[NET_ALARM_GUARD_TIME_SEG_MAX_NUM];	///< ����(��ÿ�컥��)
}NETPACKED;

struct TNetIspDayNightSwitchCfg
{
	s32 nChnId;									///< ͨ��id
	u32 dwDayNightSwitchMask;					///< ֧�ֵ���ҹ�л�ģʽ(EIspDayNightSwitch���ֵ)
	EIspDayNightSwitch eDayNightSwitchMode;		///< ��ҹ�л�ģʽ
	u32 dwTriggerMask;							///< ֧�ֵĸ澯����״̬(EIspDayNightSwitch���ֵ)
	EIspDayNightSwitch eTrigger;				///< �澯����״̬(�澯����ģʽ����Ч,����ʱ��ѡ)	
	s32 nSensLevel;								///< ��ҹת��������(�����Զ�ģʽ����Ч,����ʱ��ѡ)
	s32 nSensLevelMin;							///< ��ҹת����������Сֵ
	s32 nSensLevelMax;							///< ��ҹת�����������ֵ
	s32 nGainThreshold;							///< ���쵽ҹ��ת��������ֵ(�����Զ�ģʽ����Ч,����ʱ��ѡ)
	s32 nGainThresholdMin;						///< ���쵽ҹ��ת��������ֵ��Сֵ
	s32 nGainThresholdMax;						///< ���쵽ҹ��ת��������ֵ���ֵ
	s32 nDelayTime;								///< ת��ʱ��(�����Զ�ģʽ����Ч,����ʱ��ѡ)
	s32 nDelayTimeMin;							///< ת��ʱ����Сֵ
	s32 nDelayTimeMax;							///< ת��ʱ�����ֵ
	s32 nLightSens;								///< ��ҹת������������(�����Զ�ģʽ����Ч,����ʱ��ѡ)
	s32 nLightSensMin;							///< ��ҹת��������������Сֵ
	s32 nLightSensMax;							///< ��ҹת���������������ֵ
	s32 nLightThr;								///< ��ҹת��������ֵ(�����Զ�ģʽ����Ч,����ʱ��ѡ)
	s32 nLightThrMin;							///< ��ҹת��������ֵ��Сֵ
	s32 nLightThrMax;							///< ��ҹת��������ֵ���ֵ
	TNetIspDayNightSwitchFixedtime tFixedtime;	///< ��ʱ����(��ʱģʽ����Ч,����ʱ��ѡ)
}NETPACKED;

struct TNetIspDenoiseCfg
{
	s32 nChnId;									///< ͨ��id
	u32 dwDenoiseModeMask;						///< ֧�ֵĽ���ģʽ(EIspDenoiseMode���ֵ)
	EIspDenoiseMode eDenoiseMode;				///< ����ģʽ
	s32 nLevel;									///< ����ȼ�(ȡֵ��Χ[0,100])
}NETPACKED;

struct TNetIspDynamicRegulateCfg
{
	s32 nChnId;									///< ͨ��id
	u32 dwModeMask;								///< ֧�ֵı��ⲹ��ģʽ(EIspDynamicRegulateMode���ֵ)
	EIspDynamicRegulateMode eMode;				///< ���ⲹ��ģʽ
	s32 nBlcLevel;								///< ���ⲹ���ȼ�(����ʱ��ѡ)(ȡֵ��Χ[0,100])
	s32 nSlcLevel;								///< ǿ�����Ƶȼ�(����ʱ��ѡ)(ȡֵ��Χ[0,100])
	s32 nWdrLevel;								///< ��̬�ȼ�(����ʱ��ѡ)(ȡֵ��Χ[0,100])
	s32 nSmartIrLevel;							///< SmartIr�ȼ�(����ʱ��ѡ)(ȡֵ��Χ[0,100])
	EIspBlcRegionType eBlcRegionType;			///< �ֶ����ⲹ���Զ�����������
	TNetRect tBlcRegion;						///< �Զ�������(����ʱ��ѡ)
}NETPACKED;

struct TNetIspImageEnhanceCfg
{
	s32 nChnId;									///< ͨ��id
	u32 dwModeMask;								///< ֧�ֵ�ͼ����ǿģʽ(EIspImageEnhanceMode���ֵ)
	EIspImageEnhanceMode eMode;					///< ͼ����ǿģʽ
	s32 nDigDynRgLevel;							///< ���ֿ�̬�ȼ�(����ʱ��ѡ)(ȡֵ��Χ[0,100])
	s32 nAdapGammaLevel;						///< ����Ӧgamma�ȼ�(����ʱ��ѡ)(ȡֵ��Χ[0,100])
	s32 nDefogLevel;							///< ����ȥ��ȼ�(����ʱ��ѡ)(ȡֵ��Χ[0,100])
}NETPACKED;

struct TNetIspStablizerCfg
{
	s32 nChnId;									///< ͨ��id
	u32 dwModeMask;								///< ֧�ֵķ���ģʽ(EIspBasicMode���ֵ)
	EIspBasicMode eMode;						///< ����ģʽ
	s32 nStablLevel;							///< �����ȼ�(����ʱ��ѡ)(ȡֵ��Χ[0,100])
}NETPACKED;

struct TNetIspCombinHdrCfg
{
	s32 nChnId;									///< ͨ��id
	u32 dwModeMask;								///< ֧�ֵĺϳɿ�̬ģʽ(EIspCombinHdrMode���ֵ)
	EIspCombinHdrMode eMode;					///< �ϳɿ�̬ģʽ
	s32 nCombinHdrLevel;						///< �ȼ�(ȡֵ��Χ[0,100])
}NETPACKED;

struct TNetIspDoubleLensCfg
{
	s32 nChnId;									///< ͨ��id
	u32 dwModeMask;								///< ֧�ֵĶ�����ģʽ����(EIspBasicMode���ֵ)
	EIspBasicMode eMode;						///< ������ģʽ����
}NETPACKED;

struct TNetIspExpAllAutoCfg
{
	s32 nChnId;									///< ͨ��id
	u32 dwModeMask;								///< ֧�ֵĶ�����ģʽ����(EIspBasicMode���ֵ)
	EIspBasicMode eMode;						///< ������ģʽ����
}NETPACKED;

struct TNetIspProExposureCfg
{
	s32 nChnId;									///< ͨ��id
	u32 dwModeMask;								///< ֧�ֵ��ع����ģʽ(EIspBasicMode���ֵ)
	EIspBasicMode eMode;						///< �ع����ģʽ
	s32 nExposureLevel;							///< �ع�Ŀ������
	s32 nExposureSpeed;							///< �ع�����ٶ�
	TNetRect tExposureWindow;					///< �ع�ο�����
}NETPACKED;

struct TNetIspSceneCustomItem
{
	s32 nIndex;									///< �Զ��峡������
	s8 szName[NET_MAXLEN_64 + 1];				///< �Զ��峡������
}NETPACKED;

struct TNetIspSceneCustomList
{
	s32 nMaxNum;																	///< �Զ��峡���������
	s32 nNum;																		///< ����
	TNetIspSceneCustomItem atSceneCustomItem[NET_ISP_SCENE_CUSTOMLIST_MAX_NUM];		///< �Զ��峡��
}NETPACKED;

struct TNetIspSceneDynamicDaynight
{
	BOOL32 bCustom;								///< �Ƿ��Զ���ģʽ (�Զ���ģʽ���Զ��峡��������Ч����֮����ģʽ��Ч)
	s8 szCustomName[NET_MAXLEN_64 + 1];			///< �Զ��峡������
	EIspSceneMode eDynamicType;					///< ����ģʽ
}NETPACKED;

struct TNetIspSceneDynamicTime
{
	BOOL32 bEnble;										///< ��ʱ����Ƿ�����	
	s8 szStartTime[NET_MAXLEN_32];						///< ʱ��ο�ʼʱ��
	s8 szEndTime[NET_MAXLEN_32];						///< ʱ��ν���ʱ��
	TNetIspSceneDynamicDaynight tSceneDynamicDaynight;	///< ��̬����ģʽ
}NETPACKED;

struct TNetIspSceneDynamicWeek
{
	EDayType eDayType;															///< ��������
	u16 wItemNum;																///< ����ʱ�����
	TNetIspSceneDynamicTime atSceneDynamicTime[NET_ISP_SCENE_DAY_DYNAMIC_MAX_NUM]; ///< ʱ�����Ϣ
}NETPACKED;

struct TNetIspSceneDynamicTimelist
{
	s32 nNum;																///< ����							
	TNetIspSceneDynamicWeek atSceneDynamicPlan[NET_RECTIMEPLAN_MAX_NUM];	///< ¼��ʱ��ƻ�
}NETPACKED;

struct TNetIspSceneModeDynamic
{
	u32 dwDynamicTypeMask;								///< ֧�ֵĶ�̬ģʽ����(EIspSceneDynamicType���ֵ)
	EIspSceneDynamicType eDynamicType;					///< ��̬ģʽ����
	TNetIspSceneDynamicDaynight tSceneDynamicDayMode;	///< �ճ���ģʽ
	TNetIspSceneDynamicDaynight tSceneDynamicNightMode;	///< ҹ����ģʽ
	TNetIspSceneDynamicTimelist tSceneDynamicTimelist;	///< ʱ���
}NETPACKED;

struct TNetIspSceneModeCfg
{
	s32 nChnId;										///< ͨ��id
	u32 dwModeMask;									///< ֧�ֵĲ���ģʽ(EIspSceneMode���ֵ)
	EIspSceneMode eMode;							///< ����ģʽ
	TNetIspSceneCustomList tSceneCustomList;		///< �û��Զ����б�
	TNetIspSceneModeDynamic tSceneModeDynamic;		///< ��̬ģʽ
}NETPACKED;

struct TNetIspFlipAndPlayBackCfg
{
	s32 nChnId;									///< ͨ��id
	BOOL32 bSupportFlip;						///< �Ƿ�֧�ַ�תģʽ
	u32 dwFlipModeMask;							///< ֧�ֵķ�תģʽ(EIspFlipMode���ֵ)
	EIspFlipMode eFlipMode;						///< ��תģʽ
	BOOL32 bSupportMirror;						///< �Ƿ�֧�־���ģʽ
	u32 dwMirrorModeMask;						///< ֧�ֵľ���ģʽ(EIspMirrorMode���ֵ)
	EIspMirrorMode eMirrorMode;					///< ��תģʽ
	BOOL32 bSupportRotate;						///< �Ƿ�֧������ģʽ
	u32 dwRotateModeMask;						///< ֧�ֵ�����ģʽ(EIspRotateMode���ֵ)
	EIspRotateMode eRotateMode;					///< ����ģʽ
	BOOL32 bSupportPlayBack;					///< �Ƿ�֧�ֱ��ػ���ģʽ
	u32 dwPlayBackModeMask;						///< ֧�ֵı��ػ���ģʽ(EIspPlayBackMode���ֵ)
	EIspPlayBackMode ePlayBackMode;				///< ���ػ���ģʽ
}NETPACKED;

struct TNetIspInfaredLevel
{
	BOOL32 bLevelMode;							///< �Ƿ����ǿ��ģʽ����֮Զ����ǿ��ģʽ
	s32 nLevel;									///< ����ǿ��
	s32 nLevelMax;								///< ����ǿ�����ֵ
	s32 nLevelMin;								///< ����ǿ����Сֵ
	s32 nFar;									///< Զ��ǿ��
	s32 nFarMax;								///< Զ��ǿ�����ֵ
	s32 nFarMin;								///< Զ��ǿ����Сֵ
	s32 nNear;									///< ����ǿ��
	s32 nNearMax;								///< ����ǿ�����ֵ
	s32 nNearMin;								///< ����ǿ����Сֵ
}NETPACKED;

struct TNetIspInfraredCfg
{
	s32 nChnId;									///< ͨ��id
	u32 dwModeMask;								///< ֧�ֵĺ����ģʽ(EIspInfaredMode���ֵ)
	EIspInfaredMode eMode;						///< �����ģʽ
	TNetIspInfaredLevel tIspInfaredLevel;		///< �����ǿ��(����ǿ��ģʽ��Զ����ǿ��ģʽ��ѡһ)
}NETPACKED;

struct TNetIspLaserCfg
{
	s32 nChnId;									///< ͨ��id
	u32 dwSwitchMask;							///< ֧�ֵĿ���ģʽ(EIspBasicMode���ֵ)
	EIspBasicMode eSwitch;						///< ����
	s32 nIntensity;								///< ����ǿ��
	s32 nIntensityMin;							///< ����ǿ����Сֵ
	s32 nIntensityMax;							///< ����ǿ�����ֵ
	u32 dwModeMask;								///< ֧�ֵľ۹�ģʽ(EIspLaserMode���ֵ)
	EIspLaserMode eMode;						///< �۹�ģʽ
	BOOL32 bSupCentradMode;						///< �Ƿ�֧�ּ������Ķ�ģʽ
	u32 dwCentradModeMask;						///< ֧�ֵľ۹�ģʽ(EIspLaserCentradMode���ֵ)
	EIspLaserCentradMode eCentradMode;			///< �������Ķ�ģʽ
	BOOL32 bSupCentradModeSpeed;				///< �Ƿ�֧�ּ������Ķȵ����ٶ�
	s32 nCentradModeSpeedMin;					///< �������Ķȵ����ٶ���Сֵ
	s32 nCentradModeSpeedMax;					///< �������Ķȵ����ٶ����ֵ
	s32 nCentradModeSpeed;						///< �������Ķȵ����ٶ�
}NETPACKED;

struct TNetIspLdcCfg
{
	s32 nChnId;									///< ͨ��id
	u32 dwSwitchMask;							///< ֧�ֵĿ���ģʽ(EIspBasicMode���ֵ)
	EIspBasicMode eSwitch;						///< ����
	BOOL32 bSupMainParam;						///< �Ƿ�֧������������
	s32 nMainParamMin;							///< ������������Сֵ
	s32 nMainParamMax;							///< �������������ֵ
	s32 nMainParam;								///< ����������
	BOOL32 bSupAssistParam;						///< �Ƿ�֧�ָ���������
	s32 nAssistParamMin;						///< ������������Сֵ
	s32 nAssistParamMax;						///< �������������ֵ
	s32 nAssistParam;							///< ����������
	BOOL32 bSupSharpLevel;						///< �Ƿ�֧���񻯲���
	s32 nSharpLevelMin;							///< �񻯲�����Сֵ
	s32 nSharpLevelMax;							///< �񻯲������ֵ
	s32 nSharpLevelParam;						///< �񻯲���
}NETPACKED;

struct TNetDigitalZoom
{
	u16 wGridId;			///< grid id
	BOOL32 bDigitalZoom;	///< �Ƿ����ַŴ�
	s32 nX;					///< ��ʼ����
	s32 nY;					///< ��ʼ����
	s32 nWidth;				///< ��
	s32 nHeight;			///< ��
}NETPACKED;

struct TNetDecMode
{
	u16 wGridId;        ///< ����id
	EDecMode eDecMode;  ///< ����ģʽ
}NETPACKED;

struct TNetVideoStreamInfo
{
	s32 nFrameRate;				///< ֡��
	s32 nBitRate;				///< ����
	EVideoEncType eEncType;		///< �����ʽ
	TNetEncResolution tEncResolution; ///< �ֱ���
}NETPACKED;

struct TNetDisModeItem
{
	EDisMode eDisMode;	///< ��ʾģʽ
	s32 nBright;		///< ����
	s32 nContrast;		///< �Աȶ�
	s32 nSaturation;	///< ���Ͷ�
	s32 nHue;			///< ɫ��
}NETPACKED;

struct TNetDisModeList
{
	EDisMode eCurDisMode;							///< ��ǰ��ʾģʽ
	TNetDisModeItem atDisModeItem[DISMODE_COUNT];	///< ��ʾģʽ��Ϣ
}NETPACKED;

struct TNetGetRtspRealStreamUrlParam
{
	s32 nChnId;                                    ///< ͨ��ID
	ERtspTransType eRtspTransType;                 ///< rtsp������������
	s32 nVideoEncNum;                              ///< ��Ҫ�������Ƶ����ͨ������
	s32 anVideoEncId[NET_NVR_VIDEO_ENC_MAX_NUM];   ///< ��Ƶ����ͨ��ID����
	s32 nAudioEncNum;                              ///< ��Ҫ�������Ƶ����ͨ������
	s32 anAudioSrcId[NET_NVR_AUDIO_ENC_MAX_NUM];   ///< ��ƵԴͨ��ID����
}NETPACKED;

struct TNetRtspRealStreamUrl
{
	s8 szRtspUrl[NET_MAXLEN_256 + 1];        ///< ���ɵ�rtsp url
	s32 nRtspPort;                           ///< rtsp url�еĶ˿�
}NETPACKED;

struct TNetGetRtspPlaybackUrlParam
{
	s32 nTaskID;									///< �Ѵ����ķ�������ID
	s32 nChnID;                                  	///< ͨ��ID
	s8 szStartTime[NET_MAXLEN_32];					///< ��ʼʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	s8 szEndTime[NET_MAXLEN_32];					///< ����ʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	s8 szResStartTime[NET_MAXLEN_32];				///< ¼����Դ��ʼʱ��(��ʽΪ:2016-03-15T12:48:01.330)(�˽ڵ��ǿ�ѡ��,����д���ʾ������ǰseek)
	s8 szResEndTime[NET_MAXLEN_32];					///< ¼����Դ����ʱ��(��ʽΪ:2016-03-15T12:48:01.330)(�˽ڵ��ǿ�ѡ��,����д���ʾһֱ��󲥷�)
	s32 nVideoEncNum;								///< ��Ҫ�������Ƶ����ͨ������
	s32 anVideoEncId[NET_NVR_VIDEO_ENC_MAX_NUM];	///< ��Ƶ����ͨ��ID����
	s32 nAudioEncNum;								///< ��Ҫ�������Ƶ����ͨ������
	s32 anAudioSrcId[NET_NVR_AUDIO_ENC_MAX_NUM];	///< ��ƵԴͨ��ID����
}NETPACKED;

struct TNetRtspPlaybackUrl
{
	s8 szRtspUrl[NET_MAXLEN_256 + 1];        ///< ���ɵ�rtsp url
	s32 nRtspPort;                           ///< rtsp url�еĶ˿�
}NETPACKED;

struct TNetGetDecState
{
	EOutputType eOutputType;	///< ��ʾ����
	u16 wGridIdStart;			///< ��ʼ����id
	u16 wGridIdEnd;				///< ��������id
}NETPACKED;

struct TNetDecState
{
	EDecState eDecState;	///< ����״̬
}NETPACKED;

struct TNetDecStateList
{
	s32 nNum;										///< ����
	TNetDecState atDecState[NET_DEC_STATE_MAX_NUM];	///< ����״̬
}NETPACKED;

struct TNetEthMode
{
	ENetMode eNetMode;        ///< ��������ģʽ
	u32 dwNetModeMask;        ///< ��������ģʽ������ENetMode���ֵ
	s32 nDefRouting;          ///< Ĭ��·�� 
}NETPACKED;

struct TNetEthCap
{
	BOOL32 bEthReadOnly;                 ///< ���������Ƿ�ֻ��
	BOOL32 bDnsReadOnly;                 ///< dns�����Ƿ�ֻ��
	BOOL32 bDefRoutingReadOnly;			 ///< Ĭ��·�������Ƿ�ֻ��
}NETPACKED;

struct TNetEthParam
{
	s32 nEthId;                  ///< ����ID
	s8 szName[NET_MAXLEN_128];   ///< ������
	BOOL32 bSupRouter;           ///< �Ƿ�֧������Ϊ·��
	BOOL32 bDhcp;		         ///< IP��÷�ʽ��TRUE-�Զ���ã�FALSE-�ֶ��趨
	u32 dwIp;			         ///< IP��������
	u32 dwSubnetMask;	         ///< �������룬������
	u32 dwDefGateway;	         ///< Ĭ�����أ�������
	BOOL32 bDnsAuto;	         ///< DNS��÷�ʽ��TRUE-�Զ���ã�FALSE-�ֶ��趨
	u32 dwFirstDns;		         ///< ��ѡDNS��������
	u32 dwSecondDns;	         ///< ��ѡDNS��������
	u32 dwNetCardSpeedMask;      ///< �����ٶ����룬ENetCardSpeed���ֵ
	ENetCardSpeed eNetCardSpeed; ///< �����ٶ�
	ENetCardSpeed eActualSpeed;  ///< �����ٶ�Ϊ����Ӧ(NETCARDSPEED_AUTO)ʱ��ʵ�������ٶ�
	ENetLinkSate eNetLinkState;	 ///< �Ƿ�����
	s8 szMac[NET_MAXLEN_32];	 ///< Mac��ַ
	BOOL32 bDefRouting;			 ///< Ĭ��·��
	TNetEthCap tEthCap;          ///< ��������
}NETPACKED;

struct TNetEthInfo
{
	u8 byEthNum;                                  ///< ��������
	TNetEthParam atEthParam[NET_NVR_ETH_MAX_NUM]; ///< ������Ϣ
}NETPACKED;

struct TNetServerPort
{
	u16 wHttpPort;   ///< HTTP�˿�
	u16 wRtspPort;   ///< RTSP�˿�
	u16 wServerPort; ///< ����˿�
}NETPACKED;

struct TNetWifiMode
{
	EWifiMode eWifiMode;			///< wifiģʽ
}NETPACKED;

struct TNetMobileNetworkStatus
{
	ESimStatus eSimStatus;				///< sim��״̬
	s8 szNetOperators[NET_MAXLEN_64];	///< ������Ӫ��
	ENetType  eModeType;				///< ��������
	BOOL32 bConnect;					///< �Ƿ�����
	s32 nSignal;						///< �ź�ǿ��
	s8 szIpAddr[NET_MAXLEN_32];			///< ip��ַ
	s8 szDnsAddr[NET_MAXLEN_32];		///< dns��ַ
	s8 szIMEI[NET_MAXLEN_64];			///< IMEI��
}NETPACKED;

struct TNetMulticastCfg
{
	s8 szMultiAddr[NET_MAXLEN_32];	///< �ಥ��ַ
	u16 wStartPort;					///< ��ʼ�˿�
	s32 nTtl;						///< ttlֵ
	s32 nTtlMin;					///< ttl��Сֵ
	s32 nTtlMax;					///< ttl���ֵ
}NETPACKED;

struct TNetSysBaseParam
{
	s8 szDevName[NET_MAXLEN_64 + 1];        ///< �豸����
	u16 wDevNo;                             ///< �豸���
	u16 wDevNoMin;                          ///< �豸�����Сֵ
	u16 wDevNoMax;                          ///< �豸������ֵ
	u32 dwLangMask;			                ///< gui֧�ֵ����ԣ�EGuiLangType���ֵ
	EGuiLangType eCurLang;	                ///< ��ǰ����
	u32 dwAutoLogoutTimeMask;			    ///< ֧�ֵ��Զ�ע��ʱ���б�EAutoLogoutTime���ֵ
	EAutoLogoutTime eCurAutoLogoutTime ;	///< ��ǰ�Զ�ע��ʱ��
	u8 byMenuTran;                          ///< �˵�͸����
	u8 byMenuTranMin;                       ///< �˵�͸����
	u8 byMenuTranMax;                       ///< �˵�͸����
	EAudioListen eDefAudioListen;			///< Ĭ����Ƶ����
	BOOL32 bBootGuideEnable;                ///< �Ƿ��������򵼣�TRUE-���ã�FALSE-������
	BOOL32 bOptPwdEnable;                   ///< �Ƿ����ò������룬TRUE-���ã�FALSE-������
}NETPACKED;

struct TNetZeroChnEncParam
{
	BOOL32 bEnable;                                ///< �Ƿ�������ͨ�����룬TRUE-���ã�FALSE-������
	u16 wEncResNum;                                ///< ����ֱ�������
	TNetEncResolution atEncResolution[NET_VIDEOENC_RESOLUTION_MAX_NUM]; ///< ����ֱ����б�
	TNetEncResolution tEncResolution;              ///< ��ǰ����ֱ���
	u32 dwFrameRateMask;                           ///< ����֡���б�EFrameRateType���ֵ
	EFrameRateType eCurFrameRateType;              ///< ��ǰ����֡��
	u32 dwBitRateMask;                             ///< ���������б�EBitRateType���ֵ
	EBitRateType eCurBitRateType;                  ///< ��ǰ��������
	u32 dwZeroChnVideoSrcMask;                     ///< ��ͨ������ͼ����Դ�б�EZeroChnVideoSrcType���ֵ
	EZeroChnVideoSrcType eCurZeroChnVideoSrcType;  ///< ��ͨ�����뵱ǰͼ����Դ
	u32 dwEncTypeMask;							   ///< �����ʽ�б�EVideoEncType���ֵ
	EVideoEncType eCurEncType;					   ///< ��ǰ�����ʽ
}NETPACKED;

struct TNetUserInfo
{
	s8 szUserName[NET_NVR_USER_NAME_MAX_LEN + 1];	///< �û���
	BOOL32 bDisable;								///< �û��Ƿ�ͣ�ã�TRUE-ͣ�ã�FALSE-����
	u16 wLoginTimes;								///< ��¼����
	EUserLevel eUserLevel;							///< �û�����
}NETPACKED;

struct TNetUserList
{
	u16 wUserNum;                                   ///< �û�����
	TNetUserInfo atUserInfo[NET_NVR_USER_MAX_NUM];  ///< �û���Ϣ����
}NETPACKED;

struct TNetAddUser
{
	s8 szUserName[NET_NVR_USER_NAME_MAX_LEN + 1];	///< �û�����1~32���ַ�
	s8 szPwd[NET_NVR_USER_PWD_MAX_LEN + 1];			///< ���룬���볤��8~16
	EUserLevel eUserLevel;							///< �û�����
}NETPACKED;

struct TNetEditUser
{
	s8 szOldUserName[NET_NVR_USER_NAME_MAX_LEN + 1];	///< ���û���
	s8 szNewUserName[NET_NVR_USER_NAME_MAX_LEN + 1];	///< ���û�����1~32���ַ���û���޸��û������ֶ�����
	s8 szPwd[NET_NVR_USER_PWD_MAX_LEN + 1];			    ///< ���룬���볤��8~16
	EUserLevel eUserLevel;								///< �û�����
	BOOL32 bDisable;									///< �û��Ƿ�ͣ�ã�TRUE-ͣ�ã�FALSE-����
	BOOL32 bPwdReset;									///< ��������
}NETPACKED;

struct TNetModifyUserPwd
{
	s8 szOldPwd[NET_NVR_USER_PWD_MAX_LEN + 1];		///< ������
	s8 szNewPwd[NET_NVR_USER_PWD_MAX_LEN + 1];		///< �����룬���볤��8~16
}NETPACKED;

struct TNetUserLevelPermInfo 
{
	EUserLevel eUserLevel;                              ///< �û��ȼ�
	u32 dwSysPermMask;                                  ///< ϵͳȨ�����룬ESysPermType���ֵ
	u32 dwChnPermMask;                                  ///< ͨ��Ȩ�����룬EChnPermType���ֵ
}NETPACKED;

struct TNetUserPermCapInfo
{
	s32 nUserMaxNum;	                                               ///< �û��������
	s32 nUserNameMaxLen;                                               ///< �û�����󳤶�
	s32 nPasswordMinLen;                                               ///< �û�������С����
	s32 nPasswordMaxLen;                                               ///< �û�������󳤶�
	BOOL32 bDisableUserSup;                                            ///< �Ƿ�֧���û�ͣ�ù���
	u32 dwUserLevelMask;                                               ///< �û��������룬EUserLevel�������ֵ
	BOOL32 bPermSup;                                                   ///< �Ƿ�֧��Ȩ������
	BOOL32 bSysPermSup;                                                ///< �Ƿ�֧��ϵͳȨ������
	u32 dwSysPermMask;                                                 ///< ϵͳȨ�����룬ESysPermType���ֵ
	BOOL32 bChnPermSup;                                                ///< �Ƿ�֧��ͨ��Ȩ������
	u32 dwChnPermMask;                                                 ///< ͨ��Ȩ�����룬EChnPermType���ֵ
	BOOL32 bRmtCtrlSup;                                                ///< Զ�̷��ʿ���Ȩ�޹����Ƿ�֧��
	s32 nRmtIpStrMaxLen;                                               ///< Զ�̷��ʿ����ַ�����󳤶�
	u16 wUserLevelNum;                                                 ///< �û���������
	TNetUserLevelPermInfo atUserLevelPermInfo[NET_USERLEVEL_MAX_NUM];  ///< �û�����Ȩ��
}NETPACKED;

struct TNetChnPermInfo
{
	s32 nId;                  ///< ͨ��Id
	u32 dwChnPermMask;        ///< ͨ��Ȩ��
}NETPACKED;

struct TNetUserPermInfo
{
	s8 szUserName[NET_NVR_USER_NAME_MAX_LEN + 1];	             ///< �û���
	BOOL32 bSetRemoteInfo;                                       ///< �Ƿ�����Զ����Ϣ
	BOOL32 bEnRemoteCtrl;                                        ///< �Ƿ���Զ�̿��ƣ�TRUE-������FALSE-������
	s8 szRemoteIpInfo[NET_MAXLEN_256 + 1];                       ///< Զ�̷��ʿ���ip�б�,����Զ�̿��ƺ���Ч��Ĭ�Ͽգ���Χ7-255�ַ�;ip��ַ���Էֺŷָ֧�֡�*��ͨ���
	u32 dwSysPermMask;                                           ///< �û�ϵͳȨ�ޣ�ESysPermType���ֵ
	u16 wChnNums;                                                ///< ͨ������
	TNetChnPermInfo atChnPermInfo[NET_PER_GET_CHNPERM_MAX_NUM];  ///< ͨ��Ȩ������
}NETPACKED;

struct TNetRtspAuthInfo
{
	ERtspAuthType eRtspAuthType;             ///< RTSP��֤��ʽ
	u32 dwRtspAuthTypeMask;                  ///< RTSP��֤��ʽ���룬ERtspAuthType�������ֵ
}NETPACKED;

struct TNetSecurityServiceInfo
{
	BOOL32 bEnableSsh;                          ///< �Ƿ�����SSH����
	BOOL32 bEnableIllLoginLock;					///< �Ƿ����÷Ƿ���¼����
	s8 szManageMailbox[NET_MAXLEN_64 + 1];      ///< ���������ַ
}NETPACKED;

struct TNetIpAddrFilterCfg
{
	BOOL32 bEnable;                                                 ///< �Ƿ�����Ip��ַ����
	EIpAddrFilterType eCurrIpAddrFilterType;                        ///< ��ǰ��ip��ַ��������
	u32 dwIpAddrFilterTypeMask;                                     ///< ֧�ֵ�ip��ַ�����������룬EIpAddrFilterType�������ֵ
	s32 nBlackIpAddrMaxNum;                                         ///< ������ip��ַ�������
	s32 nBlackIpAddrNum;                                            ///< ������ip��ַ����
	s8 aszBlackIpAddr[NET_IPADDRFILTERLIST_MAXNUM][NET_MAXLEN_16];  ///< ������ip��ַ����
	s32 nWhiteIpAddrMaxNum;                                         ///< ������ip��ַ�������
	s32 nWhiteIpAddrNum;                                            ///< ������ip��ַ����
	s8 aszWhiteIpAddr[NET_IPADDRFILTERLIST_MAXNUM][NET_MAXLEN_16];  ///< ������ip��ַ����
}NETPACKED;

struct  TNetDisplayPortFreshRateCap
{
	EDisplayResolution eDisplayResolution;       ///< ��ʾ�ֱ���
	u32 dwDisplayFreshRateMask;                  ///< ��ʾˢ�������룬EDisplayFreshRate���ֵ
}NETPACKED;

struct TNetDisplayPort
{
	u16 wPortId;                                                                                  ///< �˿�Id
	BOOL32 bEnable;                                                                               ///< �Ƿ����
	EDisplayResolution eDisplayResolution;                                                        ///< ��ʾ�ֱ���
	u32 dwDisplayResolutionMask;                                                                  ///< ��ʾ�ֱ������룬EDisplayResolution���ֵ
	EDisplayFreshRate eDisplayFreshRate;                                                          ///< ��ʾˢ����
	u16 wFreshRateCapNum;                                                                         ///< ��ʾˢ������������
	TNetDisplayPortFreshRateCap tDisplayPortFreshRateCap[NET_DISPLAY_PORT_RESOLUTION_MAX_NUM];    ///< ��ʾˢ��������
}NETPACKED;

struct TNetDisplayPortVgaList 
{
	BOOL32 bValid;                                              ///< �Ƿ���Ч
	u16 wPortNum;                                               ///< ��ʾ�ӿ�����
	TNetDisplayPort atDisplayPort[NET_DISPLAY_PORT_MAX_NUM];    ///< ��ʾ�ӿ���Ϣ
}NETPACKED;

struct TNetDisplayPortHdmiList 
{
	BOOL32 bValid;                                              ///< �Ƿ���Ч
	u16 wPortNum;                                               ///< ��ʾ�ӿ�����
	TNetDisplayPort atDisplayPort[NET_DISPLAY_PORT_MAX_NUM];    ///< ��ʾ�ӿ���Ϣ
}NETPACKED;

struct TNetDisplayPortCvbsList 
{
	BOOL32 bValid;                                              ///< �Ƿ���Ч
	u16 wPortNum;                                               ///< ��ʾ�ӿ�����
	TNetDisplayPort atDisplayPort[NET_DISPLAY_PORT_MAX_NUM];    ///< ��ʾ�ӿ���Ϣ
}NETPACKED;

struct TNetSerialPortInfo 
{
	ESerialType eSerialType;                 ///< ��������
	u32 dwSerialWorkTypeMask;                ///< ���ڹ�������, ESerialWorkType�������ֵ
	ESerialWorkType eSerialWorkType;         ///< ���ڹ���
	u32 dwSerialBaudRateMask;                ///< ����������, ESerialBaudRate�������ֵ
	ESerialBaudRate eSerialBaudRate;         ///< ������
	u32 dwSerialDataBitMask;                 ///< ����λ����, ESerialDataBit�������ֵ
	ESerialDataBit eSerialDataBit;           ///< ����λ
	u32 dwSerialStopBitMask;                 ///< ֹͣλ����, ESerialStopBit�������ֵ
	ESerialStopBit eSerialStopBit;           ///< ֹͣλ
	u32 dwSerialParityBitMask;               ///< У��λ����, ESerialParityBit�������ֵ
	ESerialParityBit eSerialParityBit;       ///< У��λ
	u32 dwSerialFlowCtrlMask;                ///< ��������, ESerialFlowCtrl�������ֵ
	ESerialFlowCtrl eSerialFlowCtrl;         ///< ����
	s8 szSerialPortName[NET_MAXLEN_64 + 1];  ///< ��������
}NETPACKED;

struct TNetKtsmActiveStatus
{
	s8 szPinCode[NET_MAXLEN_64 + 1];		///< PIN��
	BOOL32 bActive;                         ///< �Ƿ񼤻�
}NETPACKED;

struct TNetActiveKtsmModule
{
	BOOL32 bError;                         ///< �Ƿ����
	s8 szErrCode[NET_MAXLEN_64 + 1];	   ///< ������
}NETPACKED;

struct TNetKtsmStatus
{
	s32 nStatus;						   ///< �Լ�״̬,1Ϊ�����ɹ���
	s32 nSigSpeed;						   ///< ǩ���ٶ�,��λ:��/��
	s32 nVerifySpeed;					   ///< ��ǩ�ٶ�,��λ:��/��
	s32 nHashSpeed;					       ///< hash�ٶ�,��λ:kb/s
	s32 nEncSpeed;					       ///< �����ٶ�,��λ:kb/s
	s32 nDecSpeed;					       ///< �����ٶ�,��λ:kb/s
	s8 szVer[NET_MAXLEN_64];			   ///< ģ��汾��
	s8 szModInfo[NET_MAXLEN_128];		   ///< ģ����Ϣ�ַ���
}NETPACKED;

struct TNetKtsmMgrSrvInfo
{
	s8 szIpAddr[NET_MAXLEN_32];				///< �ܹܷ���ip
	s32 nPort;								///< �ܹܷ���˿�
}NETPACKED;

struct TNetKtsmUserInfo
{
	s8  szUserName[NET_MAXLEN_64];	                ///< �û���
	s8  szPwd[NET_MAXLEN_64 + 1];		            ///< ����
}NETPACKED;

struct TNetNvrDevInfo
{
	s8 szDevType[NET_MAXLEN_64 + 1];                            ///< �豸����
	s8 szDevSerialNum[NET_MAXLEN_64 + 1];                       ///< �豸���к�
	s8 szDevHwVer[NET_MAXLEN_64 + 1];                           ///< �豸Ӳ���汾��
	s8 szDevProductionDate[NET_MAXLEN_64 + 1];                  ///< �豸��������
	s8 szDevSoftVer[NET_MAXLEN_64 + 1];                         ///< �豸����汾��
	s32 nCpuUsed;                                               ///< cpuռ����
	s32 nMemUsed;                                               ///< �ڴ�ռ����
	BOOL32 bSupDevRomVer;										///< �Ƿ�֧���豸ROM�汾��
	s8 szDevRomVer[NET_MAXLEN_64 + 1];                          ///< �豸ROM�汾��
	s8 szDevImei[NET_MAXLEN_32 + 1];							///< �豸���(IMEI��)
}NETPACKED;

struct TNetChnRecState
{
	s32 nChnId;                              ///< ͨ��ID
	EChnRecState eChnRecState;               ///< ¼��״̬
	ERecStreamType eRecStreamType;           ///< ��������
	s32 nFrameRate;                          ///< ��Ƶ֡��
	s32 nResolutionW;                        ///< �ֱ��ʿ�
	s32 nResolutionH;                        ///< �ֱ��ʸ�
	s32 nBitRate;                            ///< ����
	BOOL32 bAudioRec;                        ///< �Ƿ�¼����Ƶ
}NETPACKED;

struct TNetChnRecStateList
{
	s32 nChnNum;                                                    ///< ͨ����
	TNetChnRecState atChnRecState[NET_PER_GET_CHNRECSTATE_MAX_NUM]; ///< ͨ��¼��״̬��Ϣ
}NETPACKED;

struct TNetLogSubTypeItem
{
	ELogSubType eLogSubType;                                          ///< ��־����
	u32 dwLogSrcTypeMask;                                             ///< ��־��Դ�������룬 ELogSrcType���ֵ
}NETPACKED;

struct TNetLogMainTypeItem
{
	ELogMainType eLogMainType;                                        ///< ��־���
	u16 wSubTypeNums;                                                 ///< ��־��������
	TNetLogSubTypeItem atLogSubTypeItem[NET_LOG_SUBTYPR_MAX_NUM];     ///< ��־��������
}NETPACKED;

struct TNetCreateSearchLogTaskParam 
{
	s8 szStartTime[NET_MAXLEN_32];                                    ///< ��ʼʱ��
	s8 szEndTime[NET_MAXLEN_32];                                      ///< ����ʱ��
	ELogMainType eLogMainType;                                        ///< ��־���
	ELogSubType eLogSubType;                                          ///< ��־����
	ELogSrcType eLogSrcType;                                          ///< ��־��Դ
	s32 nChnId;                                                       ///< ��־��Դ-ͨ��ID
	s8 szUserName[NET_NVR_USER_NAME_MAX_LEN + 1];                     ///< ��־��Դ-�û���
	EGuiLangType eGuiLangType;                                        ///< ����������
}NETPACKED;

struct TNetSearchLogTaskInfo
{
	u32 dwTaskId;                                                     ///< ����id
	u32 dwLogTotalNums;                                               ///< ��־������
}NETPACKED;

struct TNetSearchLogItem 
{
	u32 dwTimeStamp;                                                  ///< ʱ���
	s8 szCreateTime[NET_MAXLEN_32];                                   ///< ����ʱ��
	s8 szLogSrc[NET_MAXLEN_128];                                      ///< ��־��Դ
	s8 szLogType[NET_MAXLEN_64];                                      ///< ��־����
	s8 szDetail[NET_MAXLEN_512 + 1];                                  ///< ��־����
}NETPACKED;

struct TNetHealthLogItem 
{
	EHealthInfoType eHealthInfoType;							///< ��¼����
	s32 nRecordNum;												///< ��¼����
}NETPACKED;

struct TNetHealthLogList 
{
	s32 nNum;														///< ����
	TNetHealthLogItem atHealthLogItem[NET_HEALTHLOG_TYPE_MAX_NUM];	///< ����״̬��Ϣ�б�
}NETPACKED;

struct TNetHealthDetailParam
{
	EHealthLogTimeType eHealthLogTimeType;							///< ʱ�������
	EHealthInfoType eHealthInfoType;								///< ��¼����
	s32 nStartIndex;												///< ��ʼ����
	s32 nNeedNum;													///< ��Ҫ����
}NETPACKED;

struct TNetHealthDetail
{
	s8 szSource[NET_MAXLEN_128];									///< ��Դ
	EHealthInfoType eHealthInfoType;								///< ��¼����
	s8 szDetail[NET_MAXLEN_512 + 1];								///< ��������
	s8 szTime[NET_MAXLEN_32];										///< ʱ��
}NETPACKED;

struct TNetHealthDetailList 
{
	s32 nNum;															///< ����
	TNetHealthDetail atHealthDetail[NET_GET_HEALTHLOG_DETAIL_MAX_NUM];	///< ����״̬�����б�
}NETPACKED;

struct TNetGetRealLog
{
	u32 dwStartTimeStamp;	///< ��ʼʱ���
	u32 dwEndTimeStamp;		///< ��ֹʱ���
}NETPACKED;

struct TNetRealLogItem
{
	u32 dwTimeStamp;                ///< ʵʱ��־ʱ���
	s8 szLogTime[NET_MAXLEN_32];    ///< ʵʱ��־ʱ��
	ERealLogMainType eMainType;     ///< ʵʱ��־���
	s8 szType[NET_MAXLEN_64];       ///< ʵʱ��־����
}NETPACKED;

struct TNetRealLogList
{
	u16 wLogNum;                                          ///< ��־����
	TNetRealLogItem atRealLogItem[NET_REAL_LOG_MAX_NUM];  ///< ��־��Ϣ
}NETPACKED;

struct TNetGetRealLogDetail
{
	u32 dwTimeStamp;	         ///< ʵʱ��־ʱ���
	s8 szType[NET_MAXLEN_64];    ///< ʵʱ��־����
	EGuiLangType eLang;	         ///< ʵʱ��־����
}NETPACKED;

struct TNetRealLogDetail
{
	s8 szTime[NET_MAXLEN_32];			///< ʵʱ��־ʱ��
	s8 szSource[NET_MAXLEN_128];        ///< ʵʱ��־��Դ
	s8 szType[NET_MAXLEN_64];           ///< ʵʱ��־����
	s8 szDetail[NET_MAXLEN_512 + 1];    ///< ʵʱ��־����
}NETPACKED;

struct TNetRealStateItem
{
	s8 szType[NET_MAXLEN_32];    ///< ʵʱ״̬����
}NETPACKED;

struct TNetRealStateList
{
	u16 wRealStateNum;                                         ///< ʵʱ״̬��Ϣ����
	TNetRealStateItem atRealStateItem[NET_REAL_STATE_MAX_NUM]; ///< ʵʱ״̬��Ϣ
}NETPACKED;

struct TNetSystemTimeInfo
{
	BOOL32 bTimeZone;	///< �Ƿ��ȡʱ��
	BOOL32 bManualSync; ///< �Ƿ��ȡ�ֶ����ò���
	BOOL32 bSummer;		///< �Ƿ��ȡ����ʱ����
	BOOL32 bAutoSync;	///< �Ƿ��ȡʱ���Զ�ͬ������
}NETPACKED;

struct TNetSystemTimeParam
{
	ESummerTimeType eSummerTimeZone; ///< ʱ��
	BOOL32 bSyncEnable;			     ///< ���������Ƿ���Ч
	s8 szTime[NET_MAXLEN_32];	     ///< ���õ�ʱ���ַ���
	BOOL32 bSummerEnable;		     ///< �Ƿ���������ʱ
	u32	dwSummerOffset;			     ///< ����ʱƫ��ʱ���б� ESummerTimeOffset
	ESummerTimeOffset eSummerOffset; ///< ��ǰ����ʱʱ��
	s32 nBeginMonth;			     ///< ��ʼʱ����
	s32 nBeginWeek;				     ///< ��ʼʱ����
	s32 nBeginDay;				     ///< ��ʼʱ����
	s32 nBeginHour;				     ///< ��ʼʱ��Сʱ
	s32 nEndMonth;				     ///< ����ʱ����
	s32 nEndWeek;				     ///< ����ʱ����
	s32 nEndDay;				     ///< ����ʱ����
	s32 nEndHour;				     ///< ����ʱ��Сʱ
	BOOL32 bAutoSyncEnable;		     ///< �Ƿ������Զ�Уʱ
	s8 szType[NET_MAXLEN_16];	     ///< �Զ�Уʱ���� ntp ����proto
	s8 szServerIP[NET_MAXLEN_32 + 1];    ///< IP��������
	s32 nServerPort;			     ///< �˿�
	s32 nInterval;				     ///< �ڲ�ʱ����
}NETPACKED;

struct TNetSysTimePriority
{
	ESysTimeSyncType eAutoSyncType;		///< ����Ӧ����
	BOOL32 bEnable;						///< �Ƿ�����
}NETPACKED;

struct TNetSysTimeAutoSyncParam
{
	BOOL32 bEnable;									///< �Ƿ������Զ�Уʱ
	ESysTimeSyncType eAutoSyncType;					///< �Զ�Уʱ����
	u32 dwAutoSyncTypeMask;							///< ֧���Զ�Уʱ����,EAutoSyncType���ֵ
	ESysTimeSyncType eSyncCurIndex;					///< ��ǰ��Ч����
	s32 nAdaptLockTime;								///< Уʱ����ʱ��
	s32 nAdaptLockTimeMin;							///< Уʱ����ʱ����Сֵ
	s32 nAdaptLockTimeMax;							///< Уʱ����ʱ�����ֵ
	s32 nAutoAdaptNum;								///< ����Ӧͬ��Դ����
	TNetSysTimePriority atNetSysTimePriority[NET_SYSTIME_SYNCTYPE_MAX_NUM]; ///< ���ȼ��б�	
}NETPACKED;

struct TNetSystemTimeParamEx
{
	ESummerTimeType eSummerTimeZone;				///< ʱ��
	s8 szTime[NET_MAXLEN_32];						///< ʱ���ַ���(��ʽΪ:2016-03-15T12:48:01.330)
	TNetSysTimeAutoSyncParam tNetAutoSyncParam;		///< �Զ�Уʱ����
	TNetSysTimeAutoSyncParam tNetDefSyncParam;		///< Ĭ���Զ�Уʱ����
	BOOL32 bNtpEnable;								///< �Ƿ�����NTPЭ��Уʱ
	s8 szNtpIP[NET_MAXLEN_32 + 1];					///< IP��������
	s32 nNtpPort;									///< �˿�
	s32 nNtpInterval;								///< �ڲ�ʱ����
	BOOL32 bSummerEnable;							///< �Ƿ���������ʱ
	u32	dwSummerOffset;								///< ����ʱƫ��ʱ���б� ESummerTimeOffset
	ESummerTimeOffset eSummerOffset;				///< ��ǰ����ʱʱ��
	s32 nBeginMonth;								///< ��ʼʱ����
	s32 nBeginWeek;									///< ��ʼʱ����
	s32 nBeginDay;									///< ��ʼʱ����
	s32 nBeginHour;									///< ��ʼʱ��Сʱ
	s32 nEndMonth;									///< ����ʱ����
	s32 nEndWeek;									///< ����ʱ����
	s32 nEndDay;									///< ����ʱ����
	s32 nEndHour;									///< ����ʱ��Сʱ
}NETPACKED;

struct TNetDiskNumInfo
{
	s32 nGroupID;							///< GroupIDֵ
	s32 nDiskIDNum;							///< һ��GROUP��ID
	s32 nDiskID[NET_GROUP_DISKS_MAX_NUM];	///< ͬһ����������Ӳ��ID
}NETPACKED;

struct TNetGroupDiskList
{
	s32 nGroupNum;		                                ///< ������Ŀ
	TNetDiskNumInfo tNetDiskNumInfo[NET_GROUP_MAX_NUM]; ///< ������Ϣ����
}NETPACKED;

struct TNetSetGroupDiskErrDisk
{
	s32 nDiskId;         ///< �������ID
	s32 nGroupId;        ///< ������̼��������ID
	s32 nErrCode;        ///< ҵ�������
}NETPACKED;

struct TNetSetGroupDiskErrInfo
{
	BOOL32 bSuccess;                                                        ///< �����Ƿ�ɹ�
	s32 nErrNum;                                                            ///< �����������
	TNetSetGroupDiskErrDisk atSetGroupDiskErrDisk[NET_GROUP_DISKS_MAX_NUM]; ///< ���������Ϣ
}NETPACKED;

struct TNetUpgradeExDisk
{
	s32 nCapacity;					///< Ӳ������ MB
	EDiskType eDiskType;			///< Ӳ������
	s8  szPartition[NET_MAXLEN_32]; ///< ��������
	s8	szMount[NET_MAXLEN_64];		///< ����·��
	s32 nDiskFree;					///< Ӳ��ʣ��ռ� MB
	s8	szFSType[NET_MAXLEN_16];	///< �ļ�ϵͳ����
}NETPACKED;

struct TNetExDiskList
{
	s32 nExDiskNum;                                          ///< ��չӲ������
	TNetUpgradeExDisk tNetUpgradeExDisk[NET_EXDISK_MAX_NUM]; ///< ��չӲ����Ϣ
}NETPACKED;

struct TNetUpgradePrepareParam
{
	s8  szSecurityInfo[NET_MAXLEN_256];	///< ��Ȩ��Ϣ
	s8	szPartition[NET_MAXLEN_32];		///< ��������
	s8	szMount[NET_MAXLEN_64];			///< ����Ӳ�̹��ص�
	s8  szFilePath[NET_MAXLEN_512];		///< �ļ����·��
	s8  szFSType[NET_MAXLEN_16];		///< �ļ�ϵͳ����
}NETPACKED;

struct TNetSystemFactoryDef
{
	s8 szType[NET_MAXLEN_16];			///< Part���ָֻ� All��ȫ�ָ�
	BOOL32 bChnCfgEnable;				///< ͨ��
	BOOL32 bRecCfgEnable;				///< ¼��
	BOOL32 bNetCfgEnable;				///< ����
	BOOL32 bBaseCfgEnable;				///< ��������
	BOOL32 bEventCfgEnable;				///< �¼�
	BOOL32 bIntellCfgEnable;			///< ����
}NETPACKED;

struct TNetRecoverFactoryInfo
{
	ERecoverFactoryType eRecoverFactoryType;	///< �ָ���������
}NETPACKED;

struct TNetGetChnCap
{
	s8 szChnId[NET_MAXLEN_512]; ///< ͨ��id�����ͨ��ʹ�ö��ŷָ���磺1,2,3,4
	BOOL32 bGetHardwareCap;		///< �Ƿ��ȡӲ������
	BOOL32 bGetMoveDetectCap;	///< �Ƿ��ȡ�ƶ��������
	BOOL32 bGetVideoLostCap;	///< �Ƿ��ȡ��Ƶ��ʧ����
	BOOL32 bGetOsdCap;          ///< �Ƿ��ȡOSD����
	BOOL32 bGetDevNameCap;      ///< �Ƿ��ȡ�豸��������
	BOOL32 bGetUpgradeCap;      ///< �Ƿ��ȡ�豸��������
	BOOL32 bGetOverlayAreaCap;  ///< �Ƿ���Ҫ�ڵ��澯����
}NETPACKED;

struct TNetChnCap
{
	s32 nChnId;						///< ͨ��id
	s32 nAlarmInNum;				///< �澯��������
	s32 nAlarmOutNum;				///< �澯�������
	BOOL32 bSupMoveDetectCfg;		///< �Ƿ�֧���ƶ��������
	BOOL32 bSupOsdCfg;              ///< �Ƿ�֧��OSD����
	BOOL32 bSupDevNameCfg;          ///< �Ƿ�֧���豸��������
	BOOL32 bSupChangeDevNameCfg;	///< �Ƿ�ɸ����豸��������
	BOOL32 bSupUpgrade;	            ///< �Ƿ�֧������
	BOOL32 bSupOverlayArea;	        ///< ��֧�ַ��ڵ��澯����
}NETPACKED;

struct TNetChnCapList
{
	s32 nNum;											///< ����
	TNetChnCap atChnCap[NET_PER_GET_CHN_CAP_MAX_NUM];	///< ͨ������
}NETPACKED;

struct TNetChnInteDetectCap
{
	s32 nErrNo;					///< ����� NET_OK����ȡ�ɹ�
	BOOL32 bSupCordon;			///< �Ƿ�֧�־���������
	BOOL32 bSupAreaInvasion;	///< �Ƿ�֧��������������
	BOOL32 bSupAreaEnter;		///< �Ƿ�֧�������������
	BOOL32 bSupAreaLeave;		///< �Ƿ�֧�������뿪����
	BOOL32 bSupObjectPick;		///< �Ƿ�֧����Ʒ��ȡ����
	BOOL32 bSupObjectLeft;		///< �Ƿ�֧����Ʒ��������
	BOOL32 bSupVirtualFocus;	///< �Ƿ�֧���齹�������
	BOOL32 bSupPeopleGather;	///< �Ƿ�֧����Ա�ۼ�����
	BOOL32 bSupSceneChange;		///< �Ƿ�֧�ֳ����������
	BOOL32 bSupAudAbnormal;		///< �Ƿ�֧�������쳣����
	BOOL32 bSupFaceDetect;		///< �Ƿ�֧�������������
	BOOL32 bSupPlateDetect;		///< �Ƿ�֧�ֳ���ץ������
	BOOL32 bSupAccessProtect;	///< �Ƿ�֧�ַ��𱨾�����
}NETPACKED;

struct TNetAlarmInCfg
{
	s32 nAlarmInId;						///< �澯�����
	EAlarmInType eAlarmInType;			///< �澯��������
	s8 szAlarmName[NET_MAXLEN_64 + 1];	///< �澯����
	BOOL32 bEnable;						///< �Ƿ����ã�����ʱ��Ч
}NETPACKED;

struct TNetAlarmInCfgList
{
	s32 nNum;												///< ����
	TNetAlarmInCfg atAlarmInCfg[NET_ALARM_IN_ID_MAX_NUM];	///< �澯��������
}NETPACKED;

struct TNetAlarmSource
{
	s8 szAlarmSource[NET_MAXLEN_16];	///< �澯��Դ(��ȡֵ��nvr����ʾNVR ���֣���ʾĳ��ͨ��)
	EAlarmType eAlarmType;				///< �澯����
	s32 nAlarmInId;						///< �澯����ţ�eAlarmTypeΪALARMTYPE_ALARMINʱ��Ч
	ESmartAlarmType eSmartAlarmType;	///< ���ܸ澯���ͣ�eAlarmTypeΪALARMTYPE_SMARTDETECTʱ��Ч
}NETPACKED;

struct TNetTimeSeg
{
	s32 nStartTime;	///< ��ʼʱ��
	s32 nEndTime;	///< ����ʱ��
}NETPACKED;

struct TNetAlarmGuardTimeItem
{
	EDayType eDayType;											///< ĳ��Ĳ���ʱ��
	s32 nNum;													///< ����
	TNetTimeSeg atTimeSeg[NET_ALARM_GUARD_TIME_SEG_MAX_NUM];	///< ����ʱ���
}NETPACKED;

struct TNetAlarmGuardTime
{
	TNetAlarmSource tAlarmSource;												///< �澯Դ
	s32 nNum;																	///< ����
	TNetAlarmGuardTimeItem atAlarmGuardTimeItem[NET_ALARM_GUARD_TIME_MAX_NUM];	///< ����ʱ��
}NETPACKED;

struct TNetSysAlarmCfgItem
{
	ESysAlarmType eSysAlarmType;	///< ϵͳ�澯����
	BOOL32 bAudioAlarm;				///< �Ƿ������澯
	BOOL32 bSendMail;				///< �Ƿ����ʼ�
	BOOL32 bLightAlm;				///< �Ƿ�澯����˸
}NETPACKED;

struct TNetSysAlarmCfg
{
	s32 nNum;															///< ����
	TNetSysAlarmCfgItem atSysAlarmCfgItem[NET_SYS_ALARM_CFG_MAX_NUM];	///< ϵͳ�澯������
}NETPACKED;

struct TNetAlarmInCopy
{
	s8 szAlarmSource[NET_MAXLEN_16];	///< �澯��Դ(��ȡֵ��nvr����ʾNVR ���֣���ʾĳ��ͨ��)
	s32 nAlarmInId;						///< �澯�����
}NETPACKED;

struct TNetAlarmInCopyParam
{
	TNetAlarmInCopy tCopySrc;								///< �澯���븴��Դ
	s32 nNum;												///< ����Ŀ������
	TNetAlarmInCopy atCopyDst[NET_ALARM_IN_COPY_MAX_NUM];	///< �澯���븴��Ŀ��
}NETPACKED;

struct TNetAlarmLinkCap
{
	BOOL32 bSupLinkNvrAudioAlarm;			///< �Ƿ�֧��������nvr��������
	BOOL32 bSupLinkNvrSendMail;				///< �Ƿ�֧��������nvr�����ʼ�
	BOOL32 bSupLinkNvrUpCenter;				///< �Ƿ�֧��������nvr�ϱ�����
	s8 szLinkNvrAlarmOutId[NET_MAXLEN_64];	///< ������nvr�ĸ澯����ţ�����ʹ�ö��ŷָ�
	s8 szLinkHdmi[NET_MAXLEN_32];			///< ������ʾ��HDMI�ӿںţ�����ʹ�ö��ŷָ�
	s8 szLinkVga[NET_MAXLEN_32];			///< ������ʾ��VGA�ӿںţ�����ʹ�ö��ŷָ�
	BOOL32 bSupLinkChnRec;					///< �Ƿ�֧��������ͨ��¼��
	BOOL32 bSupLinkChnSnap;					///< �Ƿ�֧��������ͨ��ץ��
	BOOL32 bSupLinkChnPreset;				///< �Ƿ�֧��������ͨ��Ԥ��λ
	s32 nLinkChnPresetMin;					///< Ԥ��λ�����Сֵ
	s32 nLinkChnPresetMax;					///< Ԥ��λ������ֵ
	BOOL32 bSupLinkChnCruise;				///< �Ƿ�֧��������ͨ��Ѳ��
	s32 nLinkChnCruiseMin;					///< Ѳ�������Сֵ
	s32 nLinkChnCruiseMax;					///< Ѳ��������ֵ
	BOOL32 bSysAudAlm;						///< �Ƿ�֧����������
	BOOL32 bSysMail;						///< �Ƿ�֧�ַ����ʼ�
	BOOL32 bSysLight;						///< �Ƿ�֧�ָ澯����˸
	BOOL32 bSupLinkNvrCloudSrv;				///< �Ƿ�֧�������Ʒ���
	u32 dwSupUploadModeMask;				///< ֧�������ϴ���ʽEAiuUploadType���ֵ
}NETPACKED;

struct TNetAlarmLinkOutItem
{
	s8 szLinkOutObj[NET_MAXLEN_8];	///< �����������(��ȡֵ��nvr����ʾNVR ���֣���ʾĳ��ͨ��)
	s8 szAlarmOutId[NET_MAXLEN_64];	///< �澯����ţ�����ʹ�ö��ŷָ�
}NETPACKED;

struct TNetAlarmLinkPtzItem
{
	s32 nChnId;								///< ͨ��id
	EAlarmLinkPtzType eAlarmLinkPtzType;	///< ����PTZ����
	s32 nNumber;							///< Ԥ��λ��Ѳ�����
}NETPACKED;

struct TNetAlarmLinkCfg
{
	TNetAlarmSource tAlarmSource;											///< �澯Դ
	u32 dwAlarmBasicLinkMask;												///< ����������EAlarmBasicLink���ֵ
	s32 nAlarmLinkOutNum;													///< �����������
	TNetAlarmLinkOutItem atAlarmLinkOutItem[NET_ALARM_LINK_OUT_MAX_NUM];	///< �������
	s8 szLinkChnRec[NET_MAXLEN_512];										///< ����ͨ��¼��ͨ��id������ʹ�ö��ŷָ�
	s8 szLinkChnSnap[NET_MAXLEN_512];										///< ����ͨ��ץ�ģ�ͨ��id������ʹ�ö��ŷָ�
	s32 nAlarmLinkPtzNum;													///< ����PTZ����
	TNetAlarmLinkPtzItem atAlarmLinkPtzItem[NET_ALARM_LINK_PTZ_MAX_NUM];	///< ����PTZ
}NETPACKED;

struct TNetSmtpLimitLen
{
	s32 nServerMin;						 ///< ��������С����
	s32 nServerMax;						 ///< ��������󳤶�
	u16 nPortMin;						 ///< �˿���Сֵ
	u16 nPortMax;						 ///< �˿����ֵ
	s32 nUserNameMin;					 ///< �û�����С����
	s32 nUserNameMax;					 ///< �û�����󳤶�
	s32 nPassWordMin;					 ///< ������С����
	s32 nPassWordMax;					 ///< ������󳤶�
	s32 nSenderNameMin;					 ///< ������������С����
	s32 nSenderNameMax;					 ///< ������������󳤶�
	s32 nSenderEmailMin;				 ///< �����˵�ַ��С����
	s32 nSenderEmailMax;				 ///< �����˵�ַ��󳤶�
	s32 nRecverNameMin;					 ///< �ռ���������С����
	s32 nRecverNameMax;					 ///< �ռ���������󳤶�
	s32 nRecverEmailMin;				 ///< �ռ��˵�ַ��С����
	s32 nRecverEmailMax;				 ///< �ռ��˵�ַ��󳤶�
	s32 nRecverMaxNum;					 ///< �ռ�����Ŀ��󳤶�
}NETPACKED;

struct TNetRecver
{
	s8 szRecverName[NET_MAXLEN_64 + 1];		 ///< �ռ�������
	s8 szRecverEmail[NET_MAXLEN_64 + 1];	 ///< �ռ��˵�ַ
}NETPACKED;

struct TNetSmtpCfg
{
	s8 szServer[NET_MAXLEN_64 + 1];			///< ������
	u16 nPort;								///< �˿�
	s8 szUserName[NET_MAXLEN_64 + 1];		///< �û���
	s8 szPassWord[NET_MAXLEN_64];			///< ����
	s8 szSenderName[NET_MAXLEN_64 + 1];		///< ����������
	s8 szSenderEmail[NET_MAXLEN_64 + 1];	///< �����˵�ַ
	BOOL32 bSsl;							///< �Ƿ�����SSL
	BOOL32 bAttachPic;						///< �Ƿ����Ӹ���
	s32 nRecverNum;							///< �����˸���
	TNetRecver tRecver[NET_MAXLEN_8];
}NETPACKED;

struct TNetSmtpCfgAll
{
	TNetSmtpLimitLen tSmtpLimitLen;
	TNetSmtpCfg	tSmtpCfg;
}NETPACKED;

struct TNetPortMap
{
	EPORTTYPE ePortType;	///< �˿�����
	u16 wPort;				///< �˿�
	u16 wMapPort;			///< ӳ��˿�
	BOOL32 bUpnpState;		///< UPnP״̬ TRUE������Ч FALSE����Ч
}NETPACKED;

struct TNetPppoeCfg
{
	BOOL32 bEnable;					///< �Ƿ�����PPPoE
	s32 nNetCardId;					///< ����id����������������Ч
	s8 szUserName[NET_MAXLEN_128];	///< PPPoE�����û���
	s8 szPwd[NET_MAXLEN_64];		///< PPPoE��������
}NETPACKED;

struct TNetPppoeStatus
{
	EPppoeStatus ePppoeStatus;	///< PPPoE״̬
	s8 szIp[NET_MAXLEN_32];		///< ���ųɹ����IP
}NETPACKED;

struct TNetUpnpCfg
{
	BOOL32 bEnable;										///< �Ƿ�����UPnP
	s8 szAlias[NET_MAXLEN_64 + 1];						///< ����
	EUPNPMODE eUpnpMode;								///< UPnPģʽ
	s8 szMapIP[NET_MAXLEN_32];							///< ӳ��IP
	s32 nPortMapNum;									///< �˿�ӳ������
	TNetPortMap atPortMap[NET_UPNP_PORT_MAP_MAX_NUM];	///< �˿�ӳ����Ϣ
}NETPACKED;

struct TNetDDNSCfg
{
	BOOL32 bEnable;										///< �Ƿ�����
	s8 szUsername[NET_MAXLEN_64 + 1];					///< DDNS�����û���
	s8 szPassword[NET_MAXLEN_64];						///< DDNS��������(��ȡ����ʱ,�˽ڵ�����Ϊ��)
	u32 dwDDNSModeSupMask;								///< ֧�ֵ�DDNS�����ṩ�̣�EDDNSType���ֵ
	EDDNSType eDDNSMode;								///< DDNS�����ṩ��
	s8 szDomainName[NET_MAXLEN_256 + 1];				///< DDNS����,ip������
	s32 nPort;											///< DDNS�˿�
	s8 szSrvAddr[NET_MAXLEN_256 + 1];					///< ��������ַ,ip������
	s8 szDevMac[NET_MAXLEN_32 + 1];						///< �豸Mac(����ʱ�����˽ڵ�)
}NETPACKED;

struct TNetDDNSState
{
	s8 szStatusMsg[NET_MAXLEN_512];						///< ״̬��Ϣ
}NETPACKED;

struct TNetSnmp3ReadParam
{
	s8 szUserName[NET_MAXLEN_64 + 1];						///< �û�����
	s32 nUserNameMin;										///< �û����Ƴ�����Сֵ
	s32 nUserNameMax;										///< �û����Ƴ������ֵ
	EAuthenType eAuthenType;								///< ��Ȩ��ʽ
	u32 dwAuthenTypeOpt;									///< ֧�ּ�Ȩ��ʽ����,EAuthenType���ֵ
	s8 szAuthentPass[NET_MAXLEN_64 + 1];					///< ��Ȩ����
	s32 nAuthentPassMin;									///< ��Ȩ���볤����Сֵ
	s32 nAuthentPassMax;									///< ��Ȩ���볤�����ֵ
	EKeyAlgType eKeyAlgType;								///< ���ܷ�ʽ
	u32 dwKeyAlgTypeOpt;									///< ֧�ּ��ܷ�ʽ����,EKeyAlgType���ֵ
	s8 szPrivateKeyPass[NET_MAXLEN_64 + 1];					///< ˽Կ����
	s32 nPrivateKeyPassMin;									///< ˽Կ���볤����Сֵ
	s32 nPrivateKeyPassMax;									///< ˽Կ���볤�����ֵ
};

struct TNetSnmp3WriteParam
{
	s8 szUserName[NET_MAXLEN_64 + 1];						///< �û�����
	s32 nUserNameMin;										///< �û����Ƴ�����Сֵ
	s32 nUserNameMax;										///< �û����Ƴ������ֵ
	EAuthenType eAuthenType;								///< ��Ȩ��ʽ
	u32 dwAuthenTypeOpt;									///< ֧�ּ�Ȩ��ʽ����,EAuthenType���ֵ
	s8 szAuthentPass[NET_MAXLEN_64 + 1];					///< ��Ȩ����
	s32 nAuthentPassMin;									///< ��Ȩ���볤����Сֵ
	s32 nAuthentPassMax;									///< ��Ȩ���볤�����ֵ
	EKeyAlgType eKeyAlgType;								///< ���ܷ�ʽ
	u32 dwKeyAlgTypeOpt;									///< ֧�ּ��ܷ�ʽ����,EKeyAlgType���ֵ
	s8 szPrivateKeyPass[NET_MAXLEN_64 + 1];					///< ˽Կ����
	s32 nPrivateKeyPassMin;									///< ˽Կ���볤����Сֵ
	s32 nPrivateKeyPassMax;									///< ˽Կ���볤�����ֵ
};

struct TNetSnmp3Param
{
	TNetSnmp3ReadParam tNetSnmp3ReadParam;
	TNetSnmp3WriteParam tNetSnmp3WriteParam;
};

struct TNetSnmpCfg
{
	BOOL32 bSnmpEnable;									///< snmp��������
	ESnmpVersion eSnmpVersion;							///< snmp�汾
	u32 dwSnmpVersionOpt;								///< ֧��snmp�汾����,ESnmpVersion���ֵ
	s8 szReadUnionName[NET_MAXLEN_64 + 1];				///< ����ͬ������
	s32 nReadUnionNameMin;								///< ����ͬ�����Ƴ�����Сֵ
	s32 nReadUnionNameMax;								///< ����ͬ�����Ƴ������ֵ
	s8 szWriteUnionName[NET_MAXLEN_64 + 1];				///< д��ͬ������
	s32 nWriteUnionNameMin;								///< д��ͬ�����Ƴ�����Сֵ
	s32 nWriteUnionNameMax;								///< д��ͬ�����Ƴ������ֵ
	s32 nSnmpPort;										///< snmp�˿�
	s32 nSnmpPortMin;									///< snmp�˿ڳ�����Сֵ
	s32 nSnmpPortMax;									///< snmp�˿ڳ������ֵ
	s8  szTrapAddr[NET_MAXLEN_32];						///< Trap��ַ
	s32 nTrapPort;										///< Trap�˿�
	s32 nTrapPortMin;									///< Trap�˿ڳ�����Сֵ
	s32 nTrapPortMax;									///< Trap�˿ڳ������ֵ
	s8  szTrapName[NET_MAXLEN_64 + 1];					///< Trap������
	s32 nTrapNameMin;									///< Trap������������Сֵ
	s32 nTrapNameMax;									///< Trap�������������ֵ
	s8  szPuId[NET_MAXLEN_64 + 1];						///< �豸����ID
	TNetSnmp3Param tNetSnmp3Param;						///< snmp3����
};

struct TNetGpsInfo
{
	s32 nSatNum;				///< ʹ�õ���������
	BOOL32 bAvailable;			///< �Ƿ���Ч
	s8 szTime[NET_MAXLEN_32];	///< ʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	double dLatitude;			///< γ��
	double dLongitude;			///< ����
	double dDirection;			///< ���淽��������Ϊ0�ȣ�����Ϊ90�ȣ��Ϸ�Ϊ180�ȣ�����Ϊ270��
	double dAltitude;			///< ���θ߶ȣ���λ:m
	double dSpeed;				///< �����ٶȣ���λ:ǧ��/Сʱ
}NETPACKED;

struct TNetCaptureStart
{
	s8 szNetID[NET_MAXLEN_8];				///< ����ID
	ENetPackFilter eCaptureFilter;			///< ��������
	s8 szFilePath[NET_MAXLEN_256 + 1];		///< ץ��Ŀ¼
	u32 dwIpFilter;							///< IP���ˣ�������
	s8 szPortFilter[NET_MAXLEN_32 + 1];		///< �˿ڹ���
}NETPACKED;

struct TNetCaptureStop
{
	s32 nNetId;								///< ����ID
	s8 szFileName[NET_MAXLEN_256 + 1];		///< �ļ�����
}NETPACKED;

struct TNetCaptureState
{
	BOOL32 bCapture;                        ///< �Ƿ�����ץ��
	BOOL32 bFileExist;						///< ץ���ļ��Ƿ����
	s32 nNetIdNum;							///< ����ID����
	s8 szNetID[NET_MAXLEN_8];				///< ����ID
	s8 szFileName[NET_MAXLEN_256 + 1];		///< �ļ�����
	ENetPackFilter eCaptureFilter;			///< ��������
	u32 dwIpFilter;							///< IP���ˣ�������
	s8 szPortFilter[NET_MAXLEN_32 + 1];		///< �˿ڹ���
}NETPACKED;

struct TNetPingStart
{
	s32 nChnID;							///< ͨ��ID
	s8 szDestIP[NET_MAXLEN_32+1];		///< ǰ��IP
	s32 nMtu;							///< MTU
	s32 nTimes;							///< PING ����
	s32 nTimeOut;						///< ��ʱʱ�䣬��
}NETPACKED;

struct TNetPingResult
{
	EPingReply ePingReply;				///< PING����
	s32 nTTL;							///< TTL
	s32 nPackage;						///< ��
	s32 nDelay;							///< �ӳ�
}NETPACKED;

struct TNetPingResultList
{
	BOOL32 bFinish;						///< �Ƿ����
	s32 nResultNum;						///< ����
	TNetPingResult tPingResult[NET_MAXLEN_8];
}NETPACKED;

struct TNetGetChnAlarmCfg
{
	s8 szChnID[NET_MAXLEN_256];    ///< ͨ��id�����ͨ��ʹ��','�ָ������磺1,2,3,4
	BOOL32 bMoveDetect;            ///< �Ƿ��ȡͨ���ƶ��������
	BOOL32 bVideoLost;			   ///< �Ƿ��ȡͨ����Ƶ��ʧ����
}NETPACKED;

struct TNetMdReg
{
	s32 nX;			///< ���ϵ�����
	s32 nY; 		///< ���ϵ�����
	s32 nWidth;		///< ������
	s32 nHeight;	///< ����߶�
}NETPACKED;

struct TNetChnAlarmCfg
{
	s32 nChnID;                         ///< ͨ��ID
	s32 nErrNo;							///< ����� NET_OK����ȡ�ɹ�
	BOOL32 bMdEnable;	                ///< �Ƿ������ƶ����
	s32 nSensiValue;                    ///< ������
	s32 nSensiMin;						///< ��������Сֵ
	s32 nSensiMax;						///< ���������ֵ
	s32 nRegNum;                        ///< ������Ŀ
	s32 nRegMax;						///< ������Ŀ���ֵ
	s32 nRegWidthMax;                   ///< ���������
	s32 nRegHeightMax;					///< �������߶�
	TNetMdReg tMdReg[NET_MAXLEN_8];     ///< �ƶ����������Ϣ
}NETPACKED;

struct TNetChnAlarmCfgList
{
	s32 nNum;															///< ����
	TNetChnAlarmCfg atChnAlarmCfg[NET_PER_GET_CHN_ALARMCFG_MAX_NUM];	///< ͨ���澯����(�ƶ����)
}NETPACKED;

struct TNetGetChnAlarmState
{
	s32 nChnIdStart;	///< ��ʼͨ��id
	s32 nChnIdEnd;		///< ����ͨ��id
}NETPACKED;

struct TNetAlarmState
{
	s8 szAlarmSource[NET_MAXLEN_16];	///< �澯��Դ(��ȡֵ��nvr����ʾNVR ���֣���ʾĳ��ͨ��)
	EAlarmType eAlarmType;				///< �澯����
	s32 nAlarmInId;						///< �澯����ţ�eAlarmTypeΪALARMTYPE_ALARMINʱ��Ч
	ESmartAlarmType eSmartAlarmType;	///< ���ܸ澯���ͣ�eAlarmTypeΪALARMTYPE_SMARTDETECTʱ��Ч
	ESysAlarmType eSysAlarmType;		///< ϵͳ�澯���ͣ�eAlarmTypeΪALARMTYPE_SYSALARMʱ��Ч
	s32 nSysAlarmParam;					///< eAlarmTypeΪALARMTYPE_SYSALARMʱ��Ч��(1)Ӳ�̹���ʱ��ʾӲ��ID (2)¼������ץ������ǰ�˵���ʱ��ʾͨ��ID (3)������ϡ�ip��ͻ��mac��ͻʱ��ʾ����ID
	s8 szEthName[NET_MAXLEN_32];		///< eAlarmTypeΪALARMTYPE_SYSALARMʱ��Ч��������ϡ�ip��ͻ��mac��ͻʱ��ʾ��������
	s8 szAlarmTime[NET_MAXLEN_32];		///< �澯ʱ��
}NETPACKED;

struct TNetAlarmStateList
{
	s32 nNum;														///< ����
	TNetAlarmState atAlarmState[NET_PER_GET_ALARM_STATE_MAX_NUM];	///< �澯״̬
}NETPACKED;

struct TNetDiskSmartTestAttr
{
	s32 nAttrID;							    ///< ����ID
	s8 szAttrName[NET_MAXLEN_64];			    ///< ������
	s32 nFlag;				                    ///< Flag�ֶ�
	s32 nValue;					                ///< Value�ֶ�
	s32 nWorst;				                    ///< Worst�ֶ�
	s32 nThresh;				                ///< Thresh�ֶ�
	s8 szRawValue[NET_MAXLEN_16];				///< RawValue�ֶ�
	ESmartTestAttrStatus eSmartTestAttrStatus;  ///< ����״̬
}NETPACKED;

struct TNetDiskSmartTestInfo
{
	s8 szModelNo[NET_MAXLEN_64];		            ///< Ӳ���ͺ�
	s8 szSerialNo[NET_MAXLEN_64];					///< Ӳ�����к�
	s32 nDiskTemperature;				            ///< �¶�
	s32 nDiskUseTime;					            ///< ʹ��ʱ��
	ESmartTestResultType eSmartTestResultType;	    ///< �����
	s32 nAttrNum;                                   ///< ��������
	TNetDiskSmartTestAttr atDiskSmartTestAttr[NET_MAX_SMART_ATTR_ITEM_NUM]; ///< smart��������б�
}NETPACKED;

struct TNetDiskGeometryInfo
{
	s32 nCylindeNum;              ///< ��������
	s32 nHeadNum;                 ///< ��ͷ����
	s32 nSectorNum;               ///< ������
	s32 nSectorSize;              ///< ������С
	u64 ullLBATotalSize;          ///< LBA�ܴ�С
	u64 ullGeoStart;              ///< ������ʼλ��
}NETPACKED;

struct TNetBadSectorCheckTaskInfo
{
	s32 nTaskId;                                 ///< �����������id
	s32 nNvrDiskID;                              ///< NVR����id
	EBadSectorCheckType eBadSectorCheckType;     ///< �����������
	EBadSectorCheckState eBadSectorCheckState;   ///< �������״̬
}NETPACKED;

struct TNetErrLbaInfo
{
	u64 ullLBA;                   ///< ��ǰ�߼����ַ
	s32 nCylindeId;               ///< ��ǰ�����
	s32 nHeadId;                  ///< ��ǰ��ͷ��
	s32 nSectorId;                ///< ��ǰ������
	s32 nSectorNum;               ///< ������Ŀ
	s32 nTimeCount;               ///< ��ʱ
	s8 szErrInfo[NET_MAXLEN_32];  ///< ������Ϣ
}NETPACKED;

struct TNetBadSectorCheckInfo
{
	EBadSectorCheckState eBadSectorCheckState;                ///< �������״̬
	u64 ullCurCheckLBA;                                       ///< ��ǰ���ڼ���lba
	s32 nErrLbaNum;                                           ///< �����lba����
	TNetErrLbaInfo atErrLbaInfo[NET_MAX_ERROR_SECTOR_NUM];    ///< �����lba��Ϣ
}NETPACKED;

struct TNetBandInfo
{
	s32 nNetID;							///< ����ID
	u32 dwChnLinkBand;					///< ͨ�����Ӵ���
	u32 dwChnRemainBand;				///< ͨ��ʣ�����
	u32 dwCurNetPost;					///< ��ǰ���ʹ���
	u32 dwRemainNetPostCap;				///< ʣ���������
}NETPACKED;

struct TNetBandParam
{
	s32 nNum;
	TNetBandInfo tNetBand[NET_MAXLEN_8];
}NETPACKED;

struct TNetVsipParam
{
	BOOL32 bEnable;				   ///< �Ƿ�����
	BOOL32 bSendNat;			   ///< �Ƿ���Nat̽���
	s8  szRegMethod[NET_MAXLEN_8]; ///< ��ַ����(url��ip)
	s8  szIpAddr[NET_MAXLEN_32];   ///< Ip��ַ
	s32 nIpAddrMax;				   ///< Ip��ַ����ַ�
	s8  szDomain[NET_MAXLEN_32];   ///< ����
	s32 nPlatPort;				   ///< ƽ̨�˿�
	s32 nPlatPortMax;			   ///< ƽ̨�˿����ֵ
	s32 nPlatPortMin;			   ///< ƽ̨�˿���Сֵ
	s8  szUUID[NET_MAXLEN_64];     ///< ����UUID
	s32 nUUIDMax;                  ///< ����UUID���ֵ
	s8  szPassWord[NET_MAXLEN_64]; ///< �����豸����
	s32 nPassWordMax;			   ///< �����豸��������ַ�
	BOOL32 bReportSecStream;       ///< �Ƿ��ϱ�����
	BOOL32 bReportZeroChn;         ///< �Ƿ��ϱ���ͨ��
}NETPACKED;

struct TNetGB28181Info
{
	s32 nPlatIndex;					///< ƽ̨����
}NETPACKED;

struct TNetGBEncChns
{
	s8  szChnID[NET_MAXLEN_32];    ///< ����ͨ������id
	s8  szChnName[NET_MAXLEN_64 + 1];  ///< ����ͨ��������
	s32 nChnID;                    ///< ��ǰ�������ͨ����Ӧ��ͨ����
	s32 nEncID;                    ///< ��ǰ�������ͨ����Ӧ�ı���ͨ��ID(��ѡֵ: 1��ʾ���� 2��ʾ����)
}NETPACKED;

struct TNetGBAlmChns
{
	s8  szAlmID[NET_MAXLEN_32];		///< �澯ͨ������id
	s8  szAlmName[NET_MAXLEN_64 + 1];	///< �澯ͨ��������
	s8  szAlmSource[NET_MAXLEN_32];	///< �澯ͨ����Դ
	s32 nAlmInID;					///< �澯�����
}NETPACKED;

struct TNetGB28181Param
{
	BOOL32 bEnable;                          ///< �Ƿ�����
	s32 nLocalPort;							 ///< ���ض˿�
	s8  szDevID[NET_MAXLEN_32 + 1];          ///< ����id
	s32 nDevIDMax;							 ///< ����id���ֵ
	s8  szDevName[NET_MAXLEN_64 + 1];        ///< �豸����
	s32 nDevNameMax;						 ///< �豸��������ַ���
	s8 szPlatID[NET_MAXLEN_32 + 1];          ///< ƽ̨id
	s32 nPlatIDMax;							 ///< ƽ̨id���ֵ
	s8 szPlatIp[NET_MAXLEN_64 + 1];          ///< ƽ̨ip
	s32 nPlatPort;							 ///< ƽ̨�˿�
	s8 szUsrName[NET_MAXLEN_128 + 1];		 ///< �û���
	s32 nUsrNameMax;						 ///< �û�������ַ���
	s8 szPassWord[NET_MAXLEN_64 + 1];		 ///< ����
	s32 nPassWordMax;						 ///< ��������ַ���
	s32 nEncChnNum;                          ///< ��Ƶ����ͨ������
	s32 nEncChnNumMin;                       ///< ��Ƶ����ͨ��������Сֵ
	s32 nEncChnNumMax;                       ///< ��Ƶ����ͨ���������ֵ
	TNetGBEncChns tNetGBEnc[NET_GB28181_ENC_CHN_MAX_NUM]; ///< ��Ƶ����ͨ����Ϣ
	s32 nAlarmChnNum;                        ///< �澯ͨ������
	s32 nAlarmChnNumMin;					 ///< �澯ͨ��������Сֵ
	s32 nAlarmChnNumMax;					 ///< �澯ͨ���������ֵ
	TNetGBAlmChns tNetGBAlm[NET_GB28181_ALARM_CHN_MAX_NUM]; ///< �澯ͨ����Ϣ
	s32 nReletSpan;							 ///< ����ʱ��
	s32 nReletSpanMin;						 ///< ����ʱ����Сֵ
	s32 nReletSpanMax;						 ///< ����ʱ�����ֵ
	s32 nKplvSpan;							 ///< �������
	s32 nKplvSpanMin;						 ///< ���������Сֵ
	s32 nKplvSpanMax;						 ///< ����������ֵ
	s32 nKplvTimeOutMaxTimes;				 ///< ��ʱ����
	s32 nKplvTimeOutMaxTimesMin;			 ///< ��ʱ������Сֵ
	s32 nKplvTimeOutMaxTimesMax;			 ///< ��ʱ�������ֵ
	s8 szOwner[NET_MAXLEN_256 + 1];			 ///< �豸����
	s32 nOwnerMax;							 ///< �豸��������ַ�
	s8 szCivilCode[NET_MAXLEN_256 + 1];		 ///< ��������
	s32 nCivilCodeMax;						 ///< ������������ַ�
	s8 szPoliceRgn[NET_MAXLEN_256 + 1];		 ///< ������Ϣ
	s32 nPoliceRgnMax;						 ///< ������Ϣ����ַ�
	s8 szSetupAddress[NET_MAXLEN_256 + 1];	 ///< ��װ��ַ
	s32 nSetupAddrMax;						 ///< ��װ��ַ����ַ�
	s8 szExtendCfgOpt[NET_MAXLEN_512];		 ///< ��չ������
	s8 szExtendCfg[NET_MAXLEN_256 * 4];		 ///< ����߼���չ����
	s32 nExtendCfgMax;						 ///< ����߼���չ��������ַ�����
	s8 szGbOrderOpt[NET_MAXLEN_64];          ///< opt��ʾ��ѡ�Ĺ�����ݴ���ģ����
	s8 szGbOrder[NET_MAXLEN_64];			 ///< ������ݴ���
	BOOL32 bSupSecurityLev;					 ///< �Ƿ�֧�ְ�ȫ�ȼ�����
	u32 dwSecurityLevMask;				     ///< ֧�ֵİ�ȫ�ȼ�
	ESipSecurityLev eSipSecurityLev;		 ///< ��ȫ�ȼ�	
}NETPACKED;

struct TNetOnvifParam
{
	BOOL32 bEnable;	                 ///< �Ƿ�����
	s8 szUrl[NET_MAXLEN_256 + 1];    ///< ��������ַ(��ȡʱ��Ч,�����޸�)
	EOnvifAuthType eOnvifAuthType;   ///< ��Ȩ��ʽ
}NETPACKED;

struct TNetPubSecCfg
{
	BOOL32 bEnable;                          ///< �Ƿ�����
	s8 szServerIp[NET_MAXLEN_64 + 1];        ///< ƽ̨ip
	s32 nServerPort;						 ///< �������˿�
	s8 szUsrName[NET_MAXLEN_128 + 1];		 ///< �û���
	s32 nUsrNameMax;						 ///< �û�����������ַ���
	s8 szPassWord[NET_MAXLEN_64 + 1];		 ///< ����
	s32 nPassWordMax;						 ///< ���볤������ַ���
	s32 nKeepAlive;							 ///< �������
	s32 nKeepAliveMax;						 ///< ����������ֵ
	s32 nKeepAliveMin;						 ///< ���������Сֵ
	s8  szDevID[NET_MAXLEN_32 + 1];          ///< �豸ID
	s32 nDevIDMax;							 ///< �豸ID�������ֵ
	s8  szSetupAddress[NET_MAXLEN_32 + 1];   ///< ��װ��ַ
	s32 nSetupAddressMax;					 ///< ��װ��ַ�������ֵ
	s8  szCivilCode[NET_MAXLEN_32 + 1];      ///< ��������
	s32 nCivilCodeMax;						 ///< �������򳤶����ֵ
}NETPACKED;

struct TNetQRCode
{
	EQRBmpType eQRBmpType;	///< ��ά��ͼƬ����
	s32 nQRBmpSize;			///< ��ά��ͼƬ��С
	s8 szQRBmpData[8192];	///< ��ά��ͼƬ����
}NETPACKED;

struct TNetCloudCfg
{
	BOOL32 bEnable;									///< �Ƿ������Ʒ���
	s8 szUmid[NET_MAXLEN_128];						///< �豸���к�
	s32 nQRCodeNum;									///< ��ά������
	TNetQRCode atQRCode[NET_CLOUD_QR_CODE_MAX_NUM];	///< ��ά����Ϣ
}NETPACKED;

struct TNetGB28181CtlInfo
{
	s8 szGbId[NET_MAXLEN_64];			///< ����ID
	s32 nLocalPort;						///< ����˿�
	s32 nKplvSpan;						///< �������(��λ��)
	s32 nKplvSpanMin;					///< ���������Сֵ
	s32 nKplvSpanMax;					///< ����������ֵ
	s32 nKplvTimeOutMaxTimes;           ///< ����������
	s32 nKplvTimeOutMaxTimesMin;        ///< ������������Сֵ  
	s32 nKplvTimeOutMaxTimesMax;        ///< �������������ֵ
}NETPACKED;

struct TNetPubSecCltCfg
{
	u16 wServerPort;						///< ����˿�
	s8 szUserName[NET_MAXLEN_32 * 3 + 1];	///< �û���
	s8 szPwd[NET_MAXLEN_32 + 1];			///< ����
}NETPACKED;

struct TNetAutoServiceCfg
{
	EAutoServiceType eAutoServiceType;	///< �Զ�ά������
	s32 nDay;							///< �ܼ��򼸺ţ�eAutoServiceTypeΪAUTOSERVICETYPE_PERWEEK��AUTOSERVICETYPE_PERMONTHʱ��Ч��ΪAUTOSERVICETYPE_PERWEEKʱ��ֵΪ[1,7]���ֱ��ʾ��һ�����գ�ΪAUTOSERVICETYPE_PERMONTHʱ��ֵΪ[1,31]
	s32 nHour;							///< ʱ
	s32 nMinute;						///< ��
	s32 nSecond;						///< ��
}NETPACKED;

struct TNetNetAdvancedParam
{
	BOOL32 bEnable;					///< �Ƿ�����
	s32 nCheckPoint1;               ///< ��һ�ش�����
	s32 nCheckPoint2;				///< �ڶ��ش�����
	s32 nCheckPoint3;				///< �����ش�����
	s32 nOverdueDiscard;            ///< ���ڶ���ʱ��
}NETPACKED;

struct TNetSysAdvancedCfg
{
	BOOL32 bEnableDiskPreRec;		///< �Ƿ�����Ӳ��Ԥ¼
	BOOL32 bEnableThirdStreamSup;	///< �Ƿ�����������֧��
	BOOL32 bSupDoubleAudio;			///< �Ƿ�֧��˫·��Ƶ֧��
	BOOL32 bEnableDoubleAudioSup;   ///< �Ƿ�����˫·��Ƶ֧��
	EAudioEncType eCurAudioCallEncType; ///< ��ǰ�ĺ��б����ʽ
	u32 dwSupAudioCallEncTypeMask;  ///< ֧�ֵ���Ƶ���б����ʽ���룬EAudioEncType���ֵ
	BOOL32 bSupRecBlockSize;		///< �Ƿ�֧������¼����С
	u32 dwSupRecBlockSizeMask;		///< ֧������¼����С�����ͣ�ERecBlockSize���ֵ
	ERecBlockSize eCurRecBlockSize;	///< ��ǰ¼����С
	BOOL32 bSupPtzCtrlTime;			///< �Ƿ�֧��PTZ��ռ�ͷ�ʱ��
	s32 nPtzCtrlTimeMax;			///< PTZ��ռ�ͷ�ʱ�����ֵ
	s32 nPtzCtrlTimeMin;			///< PTZ��ռ�ͷ�ʱ����Сֵ
	s32 nPtzCtrlTime;				///< PTZ��ռ�ͷ�ʱ��
	BOOL32 bSupAlarmDelayTime;		///< �Ƿ�֧�ֱ����ϲ����ʱ��
	s32 nAlarmDelayTimeMax;			///< �����ϲ����ʱ�����ֵ
	s32 nAlarmDelayTimeMin;			///< �����ϲ����ʱ����Сֵ
	s32 nAlarmDelayTime;			///< �����ϲ����ʱ��
}NETPACKED;

struct TNetNvrNtyEventItem
{
	s8 szEventType[NET_MAXLEN_64];	///< �¼�����
}NETPACKED;

struct TNetNvrNtyEvent
{
	s32 nNum;													///< NVR֪ͨ�¼�����
	TNetNvrNtyEventItem atEventItem[NET_NVR_NTY_EVENT_MAX_NUM];	///< NVR֪ͨ�¼���Ϣ
}NETPACKED;

struct TNetSimInfo
{
	s32 nId;				///< SIM��id����1��ʼ
	ESimStatus eSimStatus;	///< SIM��״̬
	ENetType eNetType;		///< ��������
	s32 nSignal;			///< �ź�
}NETPACKED;

struct TNetGB28181RegPlatStatus
{
	s32 nPlatId;									///< ƽ̨id����1��ʼ
	EGB28181RegPlatStatus eGB28181RegPlatStatus;	///< ע��״̬
}NETPACKED;

struct TNetPubSecRegPlatStatus
{
	s32 nPlatId;								///< ƽ̨id����1��ʼ
	EPubSecRegPlatStatus ePubSecRegPlatStatus;	///< ע��״̬
}NETPACKED;

struct TNetNetRealTimeStatus
{
	BOOL32 bEnableGps;																///< �Ƿ���GPS
	BOOL32 bEnableWifiProbe;														///< �Ƿ���WIFI̽��
	s32 nSimNum;																	///< SIM������
	TNetSimInfo atSimInfo[NET_SIM_CARD_MAX_NUM];									///< SIM��״̬
	s32 nGB28181RegPlatNum;															///< ����ע��ƽ̨����
	TNetGB28181RegPlatStatus atGB28181RegPlatStatus[NET_GB28181_REG_PLAT_MAX_NUM];	///< ����ע��ƽ̨״̬
	s32 nPubSecRegPlatNum;															///< ��ͼ��ע��ƽ̨����
	TNetPubSecRegPlatStatus atPubSecRegPlatStatus[NET_PUBSEC_REG_PLAT_MAX_NUM];		///< ��ͼ��ע��ƽ̨״̬
}NETPACKED;

struct TNetAiuChnCap
{
	s32 nChnId;						///< ͨ��id
	BOOL32 bSupFaceDetect;			///< �Ƿ�֧���������
	BOOL32 bSupCarPlateRecognise;	///< �Ƿ�֧�ֳ���ʶ��
}NETPACKED;

struct TNetAiuChnCapList
{
	s32 nNum;													///< ����
	TNetAiuChnCap atAiuChnCap[NET_PER_GET_AIU_CHN_CAP_MAX_NUM];	///< AIUͨ������
}NETPACKED;

struct TNetRegion
{
	s32 nX;	///< x����
	s32 nY;	///< y����
	s32 nW;	///< ��
	s32 nH;	///< ��
}NETPACKED;

struct TNetGetAiuRecentSnapPicList
{
	s32 nChnId;					///< ͨ��id
	EAiuPicType eAiuPicType;	///< AiuͼƬ����
}NETPACKED;

struct TNetAiuFaceInfoItem
{
	TNetRegion tRegion;	///< ��������
}NETPACKED;

struct TNetAiuCarPlateInfoItem
{
	s8 szCarPlateNo[NET_MAXLEN_16];		///< ���ƺ�
	s8 szCarPlateColor[NET_MAXLEN_32];	///< ������ɫ
	TNetRegion tRegion;					///< ��������
}NETPACKED;

struct TNetAiuAlgResult
{
	EPicSizeType ePicSizeType;												///< ͼƬ��С����
	EAiuAlgDetectType eAiuAlgDetectType;									///< �㷨�������
	s32 nScore;																///< �÷�
	s32 nAiuFaceInfoItemNum;												///< ��������������ץ��ʱ��Ч
	TNetAiuFaceInfoItem atAiuFaceInfoItem[NET_FACE_MAX_NUM];				///< ������Ϣ������ץ��ʱ��Ч
	s32 nAiuCarPlateInfoItemNum;											///< ���Ƹ���������ץ��ʱ��Ч
	TNetAiuCarPlateInfoItem atAiuCarPlateInfoItem[NET_CAR_PLATE_MAX_NUM];	///< ������Ϣ������ץ��ʱ��Ч
}NETPACKED;

struct TNetPicUploadPubSecStatusItem
{
	s32 nPlatId;									///< ƽ̨id
	EPicUploadPubSecStatus ePicUploadPubSecStatus;	///< �ϴ�״̬
}NETPACKED;

struct TNetAiuSnapPicItem
{
	u64 ullFileId;																					///< �ļ�id
	s8 szFilePath[NET_MAXLEN_512];																	///< �ļ�����·��
	EPicType ePicType;																				///< ͼƬ����
	s8 szSnapTime[NET_MAXLEN_32];																	///< ץ��ʱ��
	TNetAiuAlgResult tAiuAlgResult;																	///< �㷨ʶ����
	s32 nItemNum;																					///< ����
	TNetPicUploadPubSecStatusItem atPicUploadPubSecStatusItem[NET_PIC_UPLOAD_PUBSEC_PLAT_MAX_NUM];	///< �ϴ�״̬
}NETPACKED;

struct TNetAiuSnapPicList
{
	s32 nSnapPicTotalNum;													///< ץ��ͼƬ����
	s32 nGroupNum;															///< ��ȡ����ͼƬ����
	s32 nItemNum;															///< ��ȡ����ÿ��ͼƬ����
	TNetAiuSnapPicItem atAiuSnapPicItem[NET_AIU_RECENT_SNAP_PIC_GROUP_MAX_NUM][NET_AIU_RECENT_SNAP_PIC_ITEM_MAX_NUM];	///< ץ��ͼƬ��Ϣ
}NETPACKED;

struct TNetStartEventRec
{
	s32 nChnId;							///< ͨ��id
	s32 nEncId;							///< ����ͨ��id����1��ʼ
	s8 szEventName[NET_MAXLEN_32 + 1];	///< �¼�����
	s8 szEventDesc[NET_MAXLEN_128 + 1];	///< �¼�����
	s32 nPreRecTime;					///< Ԥ¼ʱ������λ��
	s32 nRecDelay;						///< ¼����ʱ����λ��
	BOOL32 bSnapPic;					///< �Ƿ�ץ��ͼƬ
}NETPACKED;

struct TNetEventRecInfo
{
	s32 nRecId;							///< ¼��id
	s8 szEventName[NET_MAXLEN_32 + 1];	///< �¼�����
	s8 szEventDesc[NET_MAXLEN_128 + 1];	///< �¼�����
}NETPACKED;

struct TNetManualEventInfo
{
	s8 szEventName[NET_MAXLEN_16 * 3 + 1];	///< �¼�����
	s8 szEventDesc[NET_MAXLEN_64 * 3 + 1];	///< �¼�����
	BOOL32 bDefault;						///< �Ƿ�Ĭ��
}NETPACKED;

struct TNetManualEventList
{
	BOOL32 bEventSnap;                                              ///< �Ƿ񴥷��¼�ץ��
	s32 nEventNum;                                                  ///< �ֶ��¼�����
	TNetManualEventInfo atManualEventInfo[NET_MANUALEVENT_MAXNUM];  ///< �ֶ��¼���Ϣ
}NETPACKED;

struct TNetManualEventCap
{
	BOOL32 bSup;       ///< �Ƿ�֧���ֶ��¼�����
	s32 nEventMaxNum;  ///< �ֶ��¼��������
}NETPACKED;

struct TNetAiuManualTrigParam
{
	s32 nNvrChnID;        ///< nvrͨ����
	EAiuType eAiuType;    ///< ��������
}NETPACKED;

struct TNetCtrlLibCap
{
	s32 nCtrlLibMaxNum;		///< ���ؿ�������� 
	s32 nCustomLabelMaxNum;	///< �Զ����ǩ�������
	u32 dwCtrlLibTypeMask;	///< ֧�ֵĲ��ؿ����ͣ�ECtrlLibType���ֵ
	u32 dwIdTypeMask;		///< ֧�ֵ�֤�����ͣ�EIdType���ֵ
	u32 dwSupMemNum;		///< ���ؿ�֧�ֵ�������
}NETPACKED;

struct TNetCustomLabelItem
{
	BOOL32 bEnable;							///< �Ƿ�����
	s8 szLabelName[NET_MAXLEN_64 + 1];		///< ��ǩ����
	s8 szLabelContent[NET_MAXLEN_64 + 1];	///< ��ǩ����
}NETPACKED;

struct TNetCustomLabelList
{
	s32 nNum;																	///< �Զ����ǩ����
	TNetCustomLabelItem atCustomLabelItem[NET_CTRLLIB_CUSTOM_LABEL_MAX_NUM];	///< �Զ����ǩ��Ϣ
}NETPACKED;

struct TNetCtrlLibItem
{
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];	///< ���ؿ�����
	ECtrlLibType eCtrlLibType;				///< ���ؿ�����
	u32 dwNum;								///< ���ؿ��Ա����
	s8 szRecogEngine[NET_MAXLEN_32 + 1];	///< ʶ������
	TNetCustomLabelList tCustomLabelList;	///< �Զ����ǩ�б�
	s8 szCreater[NET_MAXLEN_64 + 1];		///< ������
	s8 szCreateTime[NET_MAXLEN_32];			///< ����ʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	u32 dwEigenvalueErr;					///< ���ؿ����Ƿ�������ֵ��ȡ����ĳ�Ա����(����ʱ�����˽ڵ�)
}NETPACKED;

struct TNetCtrlLibList
{
	s32 nNum;											///< ���ؿ�����
	TNetCtrlLibItem atCtrlLibItem[NET_CTRLLIB_MAX_NUM];	///< ���ؿ���Ϣ
}NETPACKED;

struct TNetEditCtrlLibState
{
	ECtrlLibImportState eState;	///< �༭״̬
	s32 nCurIndex;				///< ��ǰ���ڱ༭�Ĳ��ؿ��Ա����
	s32 nTotalNum;				///< ��Ҫ�༭�Ĳ��ؿ��Ա����
}NETPACKED;

struct TNetSearchPersonLib
{
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];	///< ���ؿ�����
	u32 dwBeginIndex;						///< ������ʼ����
	u32 dwNeedNum;							///< ��Ҫ����������
	s8 szName[NET_MAXLEN_64 + 1];			///< ����
	ESexType eSexType;						///< �Ա�
	EIdType eIdType;						///< ֤������
	s8 szIdNumber[NET_MAXLEN_32 + 1];		///< ֤������
	s8 szAddr[NET_MAXLEN_64 + 1];			///< ��ϵ��ַ
	s8 szPhone[NET_MAXLEN_32 + 1];			///< ��ϵ�绰
	TNetDate tBirthDateStart;				///< ����������������ʼ����
	TNetDate tBirthDateEnd;					///< �������������Ľ�������
	s8 szAddTimeStart[NET_MAXLEN_32];		///< ���ʱ����������ʼʱ��
	s8 szAddTimeEnd[NET_MAXLEN_32];			///< ���ʱ�������Ľ���ʱ��
	EEigenvalueType eEigenvalueType;		///< ����ֵ�Ƿ��������
	BOOL32 bGetTotalNum;					///< �Ƿ���Ҫ����
}NETPACKED;

struct TNetPersonItem
{
	u32 dwPrimarykey;						///< ����
	s8 szName[NET_MAXLEN_64 + 1];			///< ����
	ESexType eSexType;						///< �Ա�
	EIdType eIdType;						///< ֤������
	s8 szIdNumber[NET_MAXLEN_32 + 1];		///< ֤������
	TNetDate tBirthDate;					///< ��������
	s8 szAddr[NET_MAXLEN_64 + 1];			///< ��ϵ��ַ
	s8 szPhone[NET_MAXLEN_32 + 1];			///< ��ϵ�绰
	TNetCustomLabelList tCustomLabelList;	///< �Զ����ǩ�б�
	s8 szOperator[NET_MAXLEN_64 + 1];		///< ������
	s8 szAddTime[NET_MAXLEN_32];			///< ���ʱ��
	s8 szPicPath[NET_MAXLEN_128];			///< ͼƬ·��
	s8 *pszPicData;							///< ͼƬ��Ϣ
	s32 nPicDataLen;						///< ͼƬ����
	BOOL32 bEgiErr;							///< ����Ա����ֵ��ȡ�Ƿ����
	ECtrlLibAddType eAddType;				///< �������
}NETPACKED;

struct TNetPersonList
{
	BOOL32 bFinish;			///< �Ƿ����
	u32 dwTotalNum;			///< ��������������
	u32 dwPersonItemNum;	///< ���η��ص���Ա����
	TNetPersonItem atPersonItem[NET_PERSON_ITEM_MAX_NUM];	///< ��Ա��Ϣ
}NETPACKED;

struct TNetDelPerson
{
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];			///< ���ؿ�����
	s32 nNum;										///< ��������
	u32 adwPrimarykey[NET_PER_DEL_PERSON_MAX_NUM];	///< ����
}NETPACKED;

struct TNetPicData
{
	s8 szPicPath[NET_MAXLEN_128];	///< ͼƬ·��
	s8 *pszPicData;					///< ͼƬ����
	s32 nPicDataLen;				///< ͼƬ���ݳ���
}NETPACKED;

struct TNetCtrlLibExport
{
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];			///< ���ؿ�����
	s8 szExportPath[NET_MAXLEN_512];				///< GUI����·��
	ECtrlLibExportType eExportType;					///< ��������
	s32 nNum;										///< ��������
	u32 adwPrimarykey[NET_PER_DEL_PERSON_MAX_NUM];	///< ����
	EClientType	eClientType;						///< �ͻ�������
}NETPACKED;

struct TNetCtrlLibExportFlieList
{
	s32 nNum;												///< �ļ�����
	s8 aszFileName[NET_CTRLLIB_EXPORT_FILE_MAX_NUM][512];	///< �ļ���
}NETPACKED;

struct TNetCtrlLibExportFlieData
{
	ECtrlLibExportState eState;		///< ����״̬
	s8 *pszData;					///< �ļ�����
	s32 nDataLen;					///< ���ݳ���
}NETPACKED;

struct TNetCtrlLibImportState
{
	ECtrlLibImportState eImportState;					///< ����״̬
	u32 dwTotalNum;										///< ����������
	u32 dwCurIndex;										///< ��ǰ����λ��
}NETPACKED;

struct TNetCtrlLibImportSend
{ 
	s8 szFileName[NET_MAXLEN_512];						  ///< �����ļ���
	s32 nDataStartPos;                                    ///< ������ʼƫ��
	s32 nDataLen;                                         ///< ����buffer����
	s8 *pszDataBuf;                                       ///< ����buffer����
}NETPACKED;

struct TNetCtrlLibImportParam
{ 
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];			///< ���ؿ�����
	EClientType	eClientType;						///< �ͻ�������
	s8 szImportPath[NET_MAXLEN_512];				///< GUI����·��
}NETPACKED;

struct TNetCtrlLibCopy
{
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];						///< ���Ƶ�Դ���ؿ�����
	s32 nNum;													///< ��������
	u32 adwPrimarykey[NET_PER_DEL_PERSON_MAX_NUM];				///< ����
	s32 nDstNum;												///< Ŀ�겼�ؿ�����
	s8 aszDstCtrlLib[NET_CTRLLIB_MAX_NUM][NET_MAXLEN_64 + 1];	///< Ŀ�겼�ؿ�����
}NETPACKED;

struct TNetCopyErrLibInfo
{
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];							///< ��ȡ����ֵ�����Ӧ�Ĳ��ؿ�����
	s32 nNum;														///< ��Ա��������
	s8 aszDstCtrlLib[NET_PERSON_ITEM_MAX_NUM][NET_MAXLEN_64 + 1];	///< ��Ա����
}NETPACKED;

struct TNetCtrlLibCopyErrList
{
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];						///< ���Ƶ�Դ���ؿ�����
	s32 nNum;													///< ���ؿ���ȡ����ֵ������Ϣ�б�����
	TNetCopyErrLibInfo atCopyErrLibInfo[NET_CTRLLIB_MAX_NUM];	///< ���ؿ���ȡ����ֵ������Ϣ�б�
}NETPACKED;

struct TNetCtrlLibBatch
{
	ECtrlLibBatchType eBatchType;								///< �������ģʽ
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];						///< ���ؿ�����
	s8 szFilePath[NET_MAXLEN_512];								///< GUI�ļ�·��
}NETPACKED;

struct TNetCtrlLibBatchPrepare
{
	BOOL32 bContinue;											///< �Ƿ���������
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];						///< ���ؿ�����
	EClientType	eClientType;									///< �ͻ�������
	s8 szMac[NET_MAXLEN_64];									///< ������ӶԶ˵�mac��ַ
	ECtrlLibBatchType eBatchType;								///< �������ģʽ
	s8 szFilePath[NET_MAXLEN_512];								///< GUI�ļ�·��
}NETPACKED;

struct TNetCtrlLibBatchState
{
	ECtrlLibImportState eBatchState;	///< ���ؿ��������״̬
	u32 dwTotalNum;						///< �������
	u32 dwErrNum;						///< ��ӳ��������
	u32 dwSucNum;						///< ��ӳɹ�������
}NETPACKED;

struct TNetCtrlLibErrInfo
{
	u32 dwBeginIndex;					///< ������ʼ����
	u32 dwNeedNum;						///< ��ǰ��Ҫ����
}NETPACKED;

struct TNetCtrlLibErrList
{
	u32 dwTotalNum;															///< ��������
	u32 dwReturnNum;														///< ��ǰ��������Ŀ
	s8 aszMemberName[NET_GET_ERR_PERSON_ITEM_MAX_NUM][NET_MAXLEN_64 + 1];	///< ��Ա����
}NETPACKED;

struct TNetAlgEngineCap
{
	BOOL32 bSupAlgDownload;				///< �Ƿ�֧���㷨������غ�ж��
	u32 dwSupType;						///< ֧�ֵ��㷨����(EAlgType���ֵ)
	u32 dwAlgTotalSize;					///< ��������㷨�����ܴ�С(��λ�ֽ�)
}NETPACKED;

struct TNetAlgEngine
{
	s8 szName[NET_MAXLEN_64 + 1];			///< �㷨��������
	u32 dwSize;								///< �㷨�����С(��λ�ֽ�)
	u32 dwSupType;							///< ֧�ֵĹ�������(EAlgType���ֵ)
}NETPACKED;

struct TNetAlgEngineList
{
	s32 nNum;														///< ����
	TNetAlgEngine atNetAlgEngine[NET_ALGENGINE_MAX_NUM];			///< �㷨�����б�
}NETPACKED;

struct TNetAlgEngineUpload
{
	s32 nTaskId;					///< �����㷨��������ID
	s8* pszAlgEngineData;			///< ����buffer����(base64����)
	s32 nAlgEngineDataLen;			///< ����buffer����
}NETPACKED;

struct TNetAiuUploadCaps
{
	EAiuUploadType eAiuUploadTypes;			///< �ϴ�����֧�ֵ�����(EAiuUploadType���ֵ)
	u32 dwAiuUploadCaps;					///< ���ݿ�ѡֵ,֧�ֶ�������ʱ�� "," �ָ�(EAiuUploadCaps���ֵ)
}NETPACKED;

struct TNetNvrAiuCap
{
	s32 nAlgMaxChnNum;					///< ֧�������㷨�������·��
	s32 nManDetectMaxNum;				///< ֧����������㷨���·��
	s32 nCarPlateRegMaxNum;				///< ֧�ֳ���ʶ���㷨���·��
	BOOL32 bVAPData;					///< �Ƿ�֧������ͼ������	
	BOOL32 bSupGuardTime;				///< �Ƿ�֧�����ܲ���ʱ��	
	BOOL32 bSupBackUp;					///< �������ݺ�����ͼ�Ƿ�֧�ֱ��ݹ���	
	BOOL32 bFace;						///< �Ƿ�֧�ֽ���ipc������ԱͼƬ	
	BOOL32 bCarPlate;					///< �Ƿ�֧�ֽ���ipc���ܳ���ͼƬ
	s32 nAlgUploadCapsNum;				///< �ϴ�ģʽ��֧����������
	TNetAiuUploadCaps atNetAiuUploadCaps[NET_AIUUPLOADCAPS_MAX_NUM]; ///< �ϴ�ģʽ��֧������
}NETPACKED;

struct TNetNvrVAPDataCap
{
	BOOL32 bVAPData;					///< �Ƿ�֧������ͼ������	
	BOOL32 bSupGuardTime;				///< �Ƿ�֧�����ܲ���ʱ��	
	s32 nAlgUploadCapsNum;				///< �ϴ�ģʽ��֧����������
	TNetAiuUploadCaps atNetAiuUploadCaps[NET_AIUUPLOADCAPS_MAX_NUM]; ///< �ϴ�ģʽ��֧������
}NETPACKED;

struct TNetAisChnCap
{
	s32 nChnId;							///< ͨ����
	BOOL32 bEvtData;					///< ͨ���Ƿ�֧���¼���Ƶ	
	BOOL32 bAudData;					///< ͨ���Ƿ�֧����Ƶ������	
	BOOL32 bVidData;					///< ͨ���Ƿ�֧����Ƶ������	
	BOOL32 bPicData;					///< ͨ���Ƿ�֧��ͼƬ������	
}NETPACKED;

struct TNetGetAiuBigDataCfg
{
	s32 nChnNum;									///< NVRͨ��ID����
	s32 anChnId[NET_AIU_GETBIGDATACFG_MAX_NUM];		///< ͨ��ID
	BOOL32 bGuardTime;								///< �Ƿ���ҪЯ������ʱ��
}NETPACKED;

struct TNetAiuBigDataUpload
{
	BOOL32 bEnable;					///< �Ƿ������ϴ�
	EAiuUploadType eAiuUploadType;	///< �ϴ�ģʽ
}NETPACKED;

struct TNetAiuBigDataEvent
{
	s8 szName[NET_MAXLEN_64 + 1];		///< �¼�����	
}NETPACKED;

struct TNetAiuBigDataEventVideo
{
	BOOL32 bNotUseMobileNet;					///< �Ƿ�ʹ��2g/3g/4g�ƶ�����ͬ��
	s32 nUploadNum;								///< �ϴ��б�����
	TNetAiuBigDataUpload atNetAiuBigDataUpload[NET_AIU_BIGDATA_UPLOAD_MAX_NUM];///< �ϴ��б�
	s32 nEventNum;								///< �¼������б�����
	TNetAiuBigDataEvent atNetAiuBigDataEvent[NET_AIU_BIGDATA_UPLOAD_MAX_NUM];///< �¼������б�
}NETPACKED;

struct TNetAiuBigDataCommonVideo
{
	BOOL32 bNotUseMobileNet;					///< �Ƿ�ʹ��2g/3g/4g�ƶ�����ͬ��
	s32 nUploadNum;								///< �ϴ��б�����
	TNetAiuBigDataUpload atNetAiuBigDataUpload[NET_AIU_BIGDATA_UPLOAD_MAX_NUM];///< �ϴ��б�
}NETPACKED;

struct TNetAiuBigDataCfg
{
	s32 nChnId;												///< ͨ��ID
	TNetAiuBigDataEventVideo tNetAiuBigDataEventVideo;		///< �¼���Ƶ
	TNetAiuBigDataCommonVideo tNetAiuBigDataCommonVideo;	///< ������Ƶ
	TNetAiuBigDataCommonVideo tNetAiuBigDataCommonAudio;	///< ������Ƶ
	TNetAiuBigDataCommonVideo tNetAiuBigDataCommonPic;		///< ����ͼƬ
	s32 nGuardTimeNum;															///< ����
	TNetAlarmGuardTimeItem atAlarmGuardTimeItem[NET_ALARM_GUARD_TIME_MAX_NUM];	///< ����ʱ��
}NETPACKED;

struct TNetAiuBigDataCfgList
{
	s32 nChnNum;									///< ͨ������
	TNetAiuBigDataCfg atNetAiuBigDataCfg[NET_AIU_GETBIGDATACFG_MAX_NUM];///< ͨ��������Ϣ
}NETPACKED;

struct TNetAiChnDetect
{
	s32 nChnId;							///< ͨ����
	u32 dwDetectType;					///< ͨ��֧�ּ���㷨����(EAlgType���ֵ)
}NETPACKED;

struct TNetAiProcessUploadMode
{
	EAiuUploadType eAiuUploadType;		///< �ϴ���ʽ
	u32 dwUploadMode;					///< ֧�ֵ��ϴ�����(EAipUploadMode���ֵ)
}NETPACKED;

struct TNetAiProcessUploadAlgType
{
	EAlgType eAlgType;														///< �㷨����
	s32 nNum;																///< ����						
	TNetAiProcessUploadMode atAiProcessUpload[NET_AIP_UPLOAD_TYPE_MAX_NUM];	///< �ϴ���ʽ			
}NETPACKED;

struct TNetAiProcessUpload
{
	s32 nNum;															///< ����
	TNetAiProcessUploadAlgType atUploadAlgType[NET_ALG_TYPE_MAX_NUM];	///< �㷨��Ӧ�ϴ���ʽ����			
}NETPACKED;

struct TNetAiProcessCap
{
	BOOL32 bFixChnDetect;				///< �Ƿ�̶��Ǽ���ͨ�������ܼ��
	s32 nFaceMaxNum;					///< ֧������������ܼ����
	s32 nCarMaxNum;						///< ֧�ֳ���������ܼ����
	s32 nPersonMaxNum;					///< ֧������������ܼ����
	s32 nDetectNumPerChn;				///< ÿ��ͨ��֧�ֵļ������
	s32 nDetectChnNum;					///< ֧��������ܼ��ͨ����(�ǹ̶�)
	u32 dwDetectType;					///< ͨ��֧�ּ���㷨����(EAlgType���ֵ)(�ǹ̶�)
	s32 nNum;							///< ֧�ּ���ͨ���б�����(�̶�)
	TNetAiChnDetect atNetAiChnDetect[NET_AIP_DETECT_CHN_MAX_NUM];	///< ֧�ּ���ͨ���б�(�̶�)
	s32 nCompareRuleNum;				///< ֧�ֵ����ȶԹ������
	s32 nCompareChnNum;					///< ֧�ֵ����ȶ�ͨ������
	u32 dwDetectMode;					///< ֧�ֵļ��ģʽ(EAipDetectMode���ֵ)
	u32 dwRmRepeatMode;					///< ֧�ֵ�ȥ��ģʽ(EAipRmRepeatMode���ֵ)	
	TNetAiProcessUpload	tAiProcessUpload; ///< �㷨֧�ֵ��ϴ�ģʽ/�ϴ���ʽ
	u32 dwCompareMode;					///< ֧�ֵıȶ�ģʽ(EAipCompareMode���ֵ)
	BOOL32 bSupDetectArea;				///< �Ƿ�֧�ֻ�������
	s32 nWidthMin;						///< ���Ƶľ��ο����Сֵ
	s32 nWidthMax;						///< ���Ƶľ��ο�����ֵ
	s32 nHeightMin;						///< ���Ƶľ��θ߶���Сֵ
	s32 nHeightMax;						///< ���Ƶľ��θ߶����ֵ
	s32 nSenMin;						///< �����������Сֵ
	s32 nSenMax;						///< ������������ֵ
	s32 nSimilarityMin;					///< ���ƶȷ�Χ��Сֵ
	s32 nSimilarityMax;					///< ���ƶȷ�Χ���ֵ
	u32 dwDetectIntervalTypeMask;		///< �����ʱ��֧���㷨����,(EAlgType���ֵ)
	s32 nDetectIntervalMin;				///< �����ʱ�䷶Χ��Сֵ����λs
	s32 nDetectIntervalMax;				///< �����ʱ�䷶Χ���ֵ����λs
	BOOL32 bSimCard;					///< �Ƿ�֧���ƶ�����
	u32	dwAlarmDedupIntervalSupMask;	///< �澯ȥ��ʱ�����Ƿ�֧��(EAipCompareMode���ֵ)
	s32 nAlarmDedupIntervalMin;			///< �澯ȥ��ʱ������Χ��Сֵ
	s32 nAlarmDedupIntervalMax;			///< �澯ȥ��ʱ������Χ���ֵ
	u32	dwAlarmLinkUploadModeMask;		///< ���ܸ澯�����ϴ�ģʽ֧��(EAlarmLinkUploadMode���ֵ)
	u32	dwAlarmLinkUploadTypeMask;		///< ���ܸ澯�����ϴ�����֧��(EAlarmLinkUploadType���ֵ)
	s32 nAlgDetectTotalNum;				///< ֧�ֵ��㷨�������
	BOOL32 bSupSnapPicQuailty;			///< ץ��ͼƬ���������Ƿ�֧��
	u32 dwSnapPicQuailtyMask;			///< ֧�ֵ�ץ��ͼƬ����(ESnapPicQuailty���ֵ)
}NETPACKED;

struct TNetIntellChnParam
{
	s32 nChnIDStart;					///< ��ʼͨ����
	s32 nMaxNum;						///< Ҫ��ȡ��������
}NETPACKED;

struct TNetIntellChn
{
	s32 nChnID;								///< ͨ��ID
	s32 nDevId;								///< ��ͨ����Ӧ�豸���
	s32 nDevSrcID;						    ///< �豸����ƵԴID
	s8 szChnAlias[NET_MAXLEN_64 + 1];		///< NVRͨ������
	BOOL32 bNormalAdd;						///< �Ƿ�����Ϊ��ͨͨ�����(���ò����˽ڵ�)
	s8 szIpAddr[NET_MAXLEN_32 + 1];			///< ����ͨ��IP
	BOOL32 bOnline;							///< �Ƿ�����(���ò����˽ڵ�)
}NETPACKED;

struct TNetIntellChnList
{
	s32 nNum;												///< ����ͨ���б�����
	TNetIntellChn atIntellChn[NET_AIP_DETECT_CHN_MAX_NUM];	///< ����ͨ���б�
}NETPACKED;

struct TNetAiDetectAlgParam
{
	BOOL32 bEnable;							///< �Ƿ�����
	EAlgType eAlgType;						///< �������
	EAipDetectMode eDetectMode;				///< ���ģʽ
}NETPACKED;

struct TNetAiDetectChn
{
	s32 nChnId;								///< ͨ����
	s32 nNum;								///< ����ͨ���б�����
	TNetAiDetectAlgParam atNetAiDetectAlgParam[NET_AIP_AI_CHN_ALG_PARAM_MAX_NUM];	///< ����ͨ���б�
}NETPACKED;

struct TNetChnDetectAlgList
{
	s32 nNum;								///< ͨ������㷨�б�����
	TNetAiDetectChn atNetAiDetectChn[NET_AIP_DETECT_CHN_MAX_NUM];	///< ͨ������㷨�б�
}NETPACKED;

struct TNetDelChnDetectAlg
{
	s32 nChnId;								///< ͨ����
	EAlgType eAlgType;						///< �������
}NETPACKED;

struct TNetGetChnDetectAlgParam
{
	s32 nChnId;								///< ͨ����
	EAlgType eAlgType;						///< �������
}NETPACKED;

struct TNetChnDetectAlgParamSet
{
	BOOL32 bEnable;							///< �Ƿ�����
	EAlgType eAlgType;						///< �������
	EAipDetectMode eDetectMode;				///< ���ģʽ
	s8 szEngineName[NET_MAXLEN_64 + 1];		///< ��������
	TNetRegion tNetRectRegion;				///< �����������10000*10000
	s32 nSensitivity;						///< ������
	EAipRmRepeatMode eAipRmRepeatMode;		///< ȥ��ģʽ
	BOOL32 bOnlyDetectPic;					///< �����ץ��ͼƬ,FALSE��ʾ��Ƶ�����ֶ�ͼƬ�����,TRUE��ʾ�����ץ��ͼƬ
	s32 nDetectInterval;					///< �����ʱ��
	ESnapPicQuailty eSnapPicQuailty;		///< ץ��ͼƬ����
}NETPACKED;

struct TNetAipUpload
{
	EAiuUploadType eAiuUploadType;	///< �ϴ���ʽ
	BOOL32 bEnable;					///< �Ƿ������ϴ�
	EAipUploadMode eAipUploadMode;	///< �ϴ�ģʽ
}NETPACKED;

struct TNetChnDetectAlgProcessParam
{
	EAlgType eAlgType;						///< �������
	s32 nNum;								///< �ϴ��б�����
	TNetAipUpload atNetAipUpload[NET_AIU_BIGDATA_UPLOAD_MAX_NUM];			///< �ϴ��б�
	BOOL32 bNotUseMobileNet;				///< �Ƿ�ʹ���ƶ������ϴ�
}NETPACKED;

struct TNetChnDetectAlgParam
{
	s32 nChnId;														///< ͨ����
	TNetChnDetectAlgParamSet tNetChnDetectAlgParamSet;				///< ��������
	TNetChnDetectAlgProcessParam tNetChnDetectAlgProcessParam;		///< ����ʽ
}NETPACKED;

struct TNetDetectAlgParam
{
	TNetChnDetectAlgParamSet tNetChnDetectAlgParamSet;				///< ��������
	TNetChnDetectAlgProcessParam tNetChnDetectAlgProcessParam;		///< ����ʽ
}NETPACKED;

struct TNetDetectAlgDefaultParam
{
	TNetDetectAlgParam tFaceDetectAlgParam;				///< ����
	TNetDetectAlgParam tCarDetectAlgParam;				///< ����
}NETPACKED;

struct TNetCompareRule
{
	s32 nIndex;									///< �ȶԹ�������
	s8 szRuleName[NET_MAXLEN_64 + 1];			///< ��������
	s8 szLibName[NET_MAXLEN_64 + 1];			///< ���ؿ�����
	EAipCompareMode eAipCompareMode;				///< �ȶ�ģʽ
	s32 nSimilarity;							///< ���ƶ�
	s32 nChnNum;								///< ͨ������
	s32 anChnId[NET_AIP_COMPARE_RULE_CHN_MAX_NUM];///< ͨ����
	s32 nAlarmDedupInterval;					///< �澯ȥ��ʱ����
}NETPACKED;

struct TNetCompareRuleList
{
	s32 nNum;									///< �б�����
	TNetCompareRule atCompareRule[NET_AIP_COMPARE_RULE_MAX_NUM];///< �ȶԹ�����Ϣ�б�
}NETPACKED;

struct TNetCompareRuleTime
{
	s32 nNum;															///< ����
	TNetAlarmGuardTimeItem atTimeItem[NET_ALARM_GUARD_TIME_MAX_NUM];	///< ����ʱ��
}NETPACKED;

struct TNetRuleLinkUpload
{
	BOOL32 bEnable;									///< �Ƿ������ϴ�
	EAiuUploadType eUploadType;						///< �ϴ�ģʽ
	EAlarmLinkUploadMode eUploadPicMode;			///< �ϴ�ͼƬģʽ
	EAlarmLinkUploadType eUploadPicType;			///< �ϴ�ͼƬ����
}NETPACKED;

struct TNetRuleLinkCfg
{
	s32 nIndex;																///< �ȶԹ�������
	u32 dwAlarmBasicLinkMask;												///< ����������EAlarmBasicLink���ֵ
	s32 nAlarmLinkOutNum;													///< �����������
	TNetAlarmLinkOutItem atAlarmLinkOutItem[NET_ALARM_LINK_OUT_MAX_NUM];	///< �������
	s32 nRuleLinkUploadNum;													///< �ϴ���ͼ������
	TNetRuleLinkUpload atRuleLinkUpload[NET_AI_RULELINK_UPLOAD_MAX_NUM];	///< �ϴ���ͼ���б�
	s8 szLinkChnRec[NET_MAXLEN_512];										///< ����ͨ��¼��ͨ��id������ʹ�ö��ŷָ�
	s8 szLinkChnSnap[NET_MAXLEN_512];										///< ����ͨ��ץ�ģ�ͨ��id������ʹ�ö��ŷָ�
	s32 nAlarmLinkPtzNum;													///< ����PTZ����
	TNetAlarmLinkPtzItem atAlarmLinkPtzItem[NET_ALARM_LINK_PTZ_MAX_NUM];	///< ����PTZ
}NETPACKED;

struct TNetFaceSnapSearch
{
	s32 nChnNum;															///< ͨ������
	s32 anChnId[NET_AISEARCH_CHN_MAX_NUM];									///< ͨ��ID
	s32 nStartIndex;														///< ��ʼ����
	s32 nNeedNum;															///< ��ǰ��Ҫ������
	s8 szStartTime[NET_MAXLEN_32];											///< ��������ʼʱ��
	s8 szEndTime[NET_MAXLEN_32];											///< �����Ľ���ʱ��
	s32 nSimilar;															///< ���ƶ�
	s8 *pszPicData;															///< ͼƬ��Ϣ
	s32 nPicDataLen;														///< ͼƬ����
}NETPACKED;

struct TNetFaceSnapItem
{
	s32 nChnId;															///< ͨ���� 
	s8 szTime[NET_MAXLEN_32];											///< ץ��ʱ��
	s32 nAge;															///< ����
	ESexType eSexType;													///< �Ա�
	s8 szPicPath[NET_MAXLEN_128];										///< ͼƬ·��
	s32 nSimilar;														///< ���ƶ�
}NETPACKED;

struct TNetFaceSnapList
{
	BOOL32 bFinish;															///< �Ƿ����
	u32 dwTotalNum;															///< ��������
	u32 dwItemNum;															///< ��ǰ���ص�����
	TNetFaceSnapItem atFaceSnapItem[NET_FACE_SNAP_ITEM_MAX_NUM];			///< ��������б�
	s8 szLastItemTime[NET_MAXLEN_32];										///< ��ǰ�����������һ��ʱ��
}NETPACKED;

struct TNetLogAlarmSearch
{															
	s8 szLibName[NET_MAXLEN_64 + 1];										///< ���ؿ�����
	s32 nChnNum;															///< ͨ������
	s32 anChnId[NET_AISEARCH_CHN_MAX_NUM];									///< ͨ��ID
	ELogAlarmType eLogAlarmType;											///< ��־����
	s32 nStartIndex;														///< ��ʼ����
	s32 nNeedNum;															///< ��ǰ��Ҫ������
	s8 szStartTime[NET_MAXLEN_32];											///< ��������ʼʱ��
	s8 szEndTime[NET_MAXLEN_32];											///< �����Ľ���ʱ��
	s8 szName[NET_MAXLEN_64 + 1];											///< ����
	ESexType eSexType;														///< �Ա�
	EIdType eIdType;														///< ֤������
	s8 szIdNumber[NET_MAXLEN_32 + 1];										///< ֤������
	s8 szAddr[NET_MAXLEN_64 + 1];											///< ��ϵ��ַ
	s8 szPhone[NET_MAXLEN_32 + 1];											///< ��ϵ�绰
	TNetDate tBirthDateStart;												///< ����������������ʼ����
	TNetDate tBirthDateEnd;													///< �������������Ľ�������
}NETPACKED;

struct TNetPersonInfo
{
	s8 szName[NET_MAXLEN_64 + 1];			///< ����
	ESexType eSexType;						///< �Ա�
	EIdType eIdType;						///< ֤������
	s8 szIdNumber[NET_MAXLEN_32 + 1];		///< ֤������
	TNetDate tBirthDate;					///< ��������
	s8 szAddr[NET_MAXLEN_64 + 1];			///< ��ϵ��ַ
	s8 szPhone[NET_MAXLEN_32 + 1];			///< ��ϵ�绰
	TNetCustomLabelList tCustomLabelList;	///< �Զ����ǩ�б�
	s8 szOperator[NET_MAXLEN_64 + 1];		///< ������
	s8 szAddTime[NET_MAXLEN_32];			///< ���ʱ��
	s8 szPicPath[NET_MAXLEN_128];			///< ͼƬ·��
}NETPACKED;

struct TNetLogAlarmItem
{
	s32 nChnId;															///< ͨ���� 
	s8 szTime[NET_MAXLEN_32];											///< ץ��ʱ��
	s32 nSimilar;														///< ���ƶ�
	s8 szPicPath[NET_MAXLEN_128];										///< ͼƬ·��
	s8 szLibPicPath[NET_MAXLEN_128];									///< ��ͼ���г�ԱͼƬ·��
	TNetPersonInfo tPersonInfo;											///< ��Ա��Ϣ
}NETPACKED;

struct TNetLogAlarmList
{
	BOOL32 bFinish;															///< �Ƿ����
	u32 dwTotalNum;															///< ��������
	u32 dwItemNum;															///< ��ǰ���ص�����
	TNetLogAlarmItem atAlarmLogItem[NET_FACE_SNAP_ITEM_MAX_NUM];			///< ��������б�
	s8 szLastItemTime[NET_MAXLEN_32];										///< ��ǰ�����������һ��ʱ��
}NETPACKED;

struct TNetPersonSnapSearch
{
	s32 nChnNum;															///< ͨ������
	s32 anChnId[NET_AISEARCH_CHN_MAX_NUM];									///< ͨ��ID
	s32 nStartIndex;														///< ��ʼ����
	s32 nNeedNum;															///< ��ǰ��Ҫ������
	s8 szStartTime[NET_MAXLEN_32];											///< ��������ʼʱ��
	s8 szEndTime[NET_MAXLEN_32];											///< �����Ľ���ʱ��
	s8 *pszPicData;															///< ͼƬ��Ϣ
	s32 nPicDataLen;														///< ͼƬ����
}NETPACKED;

struct TNetPersonSnapItem
{
	s32 nChnId;															///< ͨ���� 
	s8 szTime[NET_MAXLEN_32];											///< ץ��ʱ��
	s8 szPicPath[NET_MAXLEN_128];										///< ͼƬ·��
}NETPACKED;

struct TNetPersonSnapList
{
	BOOL32 bFinish;															///< �Ƿ����
	u32 dwTotalNum;															///< ��������
	u32 dwItemNum;															///< ��ǰ���ص�����
	TNetPersonSnapItem atPersonSnapItem[NET_FACE_SNAP_ITEM_MAX_NUM];		///< ��������б�
	s8 szLastItemTime[NET_MAXLEN_32];										///< ��ǰ�����������һ��ʱ��
}NETPACKED;

struct TNetSmallVideoParam
{
	s32 nPreStartTime;	///< ����Ƶ��ǰ����ʱ�䣬Ĭ����ǰ5s
	s32 nDuration;		///< ʱ����Ĭ��60s
}NETPACKED;

struct TNetSnapCfg
{
	s32 nTimerInterval;		///< ץ��ʱ����(��)
	s32 nSnapNum;			///< ץ������
}NETPACKED;

struct TNetManualSnap
{
	s8 szEventName[NET_MAXLEN_32 + 1];			///< �¼�����
	s8 szDesc[NET_MAXLEN_128 + 1];				///< ������Ϣ
}NETPACKED;

struct TNetSeriesSnap
{
	s32 nChnID;							///< ͨ��ID
	s32 nEncID;							///< ����ID
	ESeriesSnapPicType ePicType;		///< ͼƬ����
	TNetManualSnap tManualSnap;			///< �ֶ�ץ��ͼƬ��Ϣ(��ePicTypeΪSNAPPICTYPE_MANUAL_SNAPʱ��Ч)
}NETPACKED;

struct TNetPicUploadStatus
{
	BOOL32 bUploadPubSec1;					///< �Ƿ��ϴ���ͼ��1
	EPicUploadStatus ePubSec1UploadStatus;	///< ��ͼ��1�ϴ�״̬
	BOOL32 bUploadPubSec2;					///< �Ƿ��ϴ���ͼ��2
	EPicUploadStatus ePubSec2UploadStatus;	///< ��ͼ��2�ϴ�״̬
	BOOL32 bUploadFtp;						///< �Ƿ��ϴ�FTP
	EPicUploadStatus eFtpUploadStatus;		///< FTP�ϴ�״̬
	BOOL32 bUploadPad;						///< �Ƿ��ϴ�PAD
	EPicUploadStatus ePadUploadStatus;		///< PAD�ϴ�״̬
	s8 szPicPath[NET_MAXLEN_128];			///< ͼƬ·��������ͼƬ״̬��Ϣʱ��Ч
}NETPACKED;

struct TNetDetectSnapItem
{
	s32 nChnId;																///< ͨ��id
	s8 szSnapTime[NET_MAXLEN_32];											///< ץ��ʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	EAlgType eAlgType;														///< �������
	EPicType ePicType;														///< ͼƬ����
	TNetPicData tSmallPic;													///< Сͼ
	TNetPicData tOrigPic;													///< ԭͼ
	TNetPicUploadStatus tSmallPicUploadStatus;								///< Сͼ�ϴ�״̬
	TNetPicUploadStatus tOrigPicUploadStatus;								///< ԭͼ�ϴ�״̬
	s32 nAiuCarPlateInfoItemNum;											///< ���Ƹ���������ץ��ʱ��Ч
	TNetAiuCarPlateInfoItem atAiuCarPlateInfoItem[NET_CAR_PLATE_MAX_NUM];	///< ������Ϣ������ץ��ʱ��Ч
}NETPACKED;

struct TNetDetectSnapList
{
	s32 nSnapFaceTotalNum;												///< ����ץ����������
	s32 nSnapCarTotalNum;												///< ����ץ�ĳ�������
	s32 nPersonDetectTotalNum;											///< �������˼������
	s32 nNum;															///< ���ץ������
	TNetDetectSnapItem atDetectSnapItem[NET_DETECT_SNAP_LIST_MAX_NUM];	///< ���ץ����Ϣ
}NETPACKED;

struct TNetCmpAlarmItem
{
	s32 nChnId;								///< ͨ��id
	s8 szAlarmTime[NET_MAXLEN_32];			///< ����ʱ��(��ʽΪ:2016-03-15T12:48:01.330)
	ELogAlarmType eLogAlarmType;			///< ��������
	EPicType ePicType;						///< ͼƬ����
	s32 nSimilar;							///< ���ƶȣ�������������Ч
	s8 szName[NET_MAXLEN_64 + 1];			///< ������������������Ч
	ESexType eSexType;						///< �Ա𣬰�����������Ч
	EIdType eIdType;						///< ֤�����ͣ�������������Ч
	s8 szIdNumber[NET_MAXLEN_32 + 1];		///< ֤�����룬������������Ч
	TNetDate tBirthDate;					///< �������ڣ�������������Ч
	s8 szAddr[NET_MAXLEN_64 + 1];			///< ��ϵ��ַ��������������Ч
	s8 szPhone[NET_MAXLEN_32 + 1];			///< ��ϵ�绰��������������Ч
	TNetCustomLabelList tCustomLabelList;	///< �Զ����ǩ�б�������������Ч
	s8 szOperator[NET_MAXLEN_64 + 1];		///< �����ˣ�������������Ч
	s8 szAddTime[NET_MAXLEN_32];			///< ���ʱ��(��ʽΪ:2016-03-15T12:48:01.330)��������������Ч
	TNetPicData tSmallPic;					///< Сͼ
	TNetPicData tCtrlLicPic;				///< ���ؿ��е�ͼƬ��������������Ч
}NETPACKED;

struct TNetCmpAlarmList
{
	s32 nNum;														///< �Ƚϱ�������
	TNetCmpAlarmItem atCmpAlarmItem[NET_CMP_ALARM_LIST_MAX_NUM];	///< �ȶԱ�����Ϣ
}NETPACKED;


#ifdef WIN32
#pragma pack(pop)
#endif


/**
 * �ӿڶ���
 */

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef max
#define max(a,b)        (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)        (((a) < (b)) ? (a) : (b))
#endif

#ifdef WIN32
	#ifdef _NETSDK_
		#define NVR_API __declspec(dllexport)
	#else
		#define NVR_API __declspec(dllimport)
	#endif
	#define STDCALL _stdcall
#else
	#define NVR_API
	#define STDCALL
#endif

typedef void (*pfNvrStateCallBack)(u32 dwHandle, TNetNvrState tNvrState);
typedef void (*pfAPICallBack)(TNetCallBackInfo tCallBackInfo);
typedef void (*pfMsgCallBack)(TNetMsgItem tMsgItem);

/**
 * @brief  ��ʼ��kdnetsdk
 * @param  ��
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_Init();

/**
 * @brief  �ͷ�kdnetsdk��Դ
 * @param  ��
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_Cleanup();

/**
 * @brief  ��ȡ�豸����
 * @param  dwNvrIp  [in]NVR IP��������
 *         wPort    [in]�˿ڣ�������
 *         ptNvrCap [out]�豸����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNvrCap(u32 dwNvrIp, u16 wPort, TNetNvrCap *ptNvrCap);

/**
 * @brief  ��ȡgui������Ϣ��������guiʹ�ã�
 * @param  dwNvrIp       [in]NVR IP��������
 *         wPort         [in]�˿ڣ�������
 *         ptGuiLangInfo [out]gui������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetGuiLangInfo(u32 dwNvrIp, u16 wPort, TNetGuiLangInfo *ptGuiLangInfo);

/**
 * @brief  ��ȡ�����û��б�������guiʹ�ã�
 * @param  dwNvrIp    [in]NVR IP��������
 *         wPort      [in]�˿ڣ�������
 *         ptUserList [out]�û��б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetLightUserList(u32 dwNvrIp, u16 wPort, TNetUserList *ptUserList);

/**
 * @brief  ����
 * @param  tLoginInfo [in]������Ϣ
 *         pdwHandle  [out]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_Active(TNetLoginInfo tLoginInfo, u32 *pdwHandle);

/**
 * @brief  ��¼
 * @param  tLoginInfo [in]��¼��Ϣ
 *         pdwHandle  [out]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_Login(TNetLoginInfo tLoginInfo, u32 *pdwHandle);

/**
 * @brief  ��¼
 * @param  tLoginInfo     [in]��¼��Ϣ
 *         pdwHandle      [out]���
 *         ptLoginErrInfo [out]��չ�ظ���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_LoginEx(TNetLoginInfo tLoginInfo, u32 *pdwHandle, TNetLoginErrInfo *ptLoginErrInfo);

/**
 * @brief  ע��
 * @param  dwHandle  [in]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_Logout(u32 dwHandle);

/**
 * @brief  ����
 * @param  dwHandle  [in]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_Reboot(u32 dwHandle);

/**
 * @brief  �ػ�
 * @param  dwHandle  [in]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_Shutdown(u32 dwHandle);

/**
 * @brief  У���û���Ϣ
 * @param  dwHandle   [in]���
 *         tCheckInfo [in]У����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CheckUserInfo(u32 dwHandle, TNetCheckInfo tCheckInfo);

/**
 * @brief  ���õ�¼�˿�
 * @param  dwHandle [in]���
 *         wPort    [in]�˿ڣ�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetLoginPort(u32 dwHandle, u16 wPort);

/**
 * @brief  ע��nvr״̬֪ͨ
 * @param  dwHandle [in]���
 *         pfFun    [in]nvr״̬֪ͨ�ص�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_RegNvrStateNty(u32 dwHandle, pfNvrStateCallBack pfFun);

/**
 * @brief  ע��nvr״̬֪ͨ
 * @param  dwHandle [in]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_UnregNvrStateNty(u32 dwHandle);

/**
 * @brief  ע����Ϣ֪ͨ
 * @param  pfFun [in]��Ϣ֪ͨ�ص�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_RegMsgNty(pfMsgCallBack pfFun);

/**
 * @brief  ������Ϣ
 * @param  dwHandle          [in]���
 *         tSubscribeMsgList [in]������Ϣ�б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SubscribeMsg(u32 dwHandle, TNetSubscribeMsgList tSubscribeMsgList);

/**
 * @brief  �����������
 * @param  dwHandle [in]���
 *         nValue   [in]�����������Χ(1~30)��
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetHeartbeatInterval(u32 dwHandle, s32 nValue);

/**
 * @brief  ��ȡʵʱ��־��ʼʱ���
 * @param  dwHandle          [in]���
 *         pdwStartTimeStamp [out]��ʼʱ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRealLogTimeStamp(u32 dwHandle, u32 *pdwStartTimeStamp);

/**
 * @brief  ����ʵʱ��־��ʼʱ���
 * @param  dwHandle    [in]���
 *         dwTimeStamp [in]ʱ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetRealLogStartTimeStamp(u32 dwHandle, u32 dwTimeStamp);

/**
 * @brief  ��ȡЭ���б�
 * @param  dwHandle    [in]���
 *         ptProtoList [out]Э���б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetProtoList(u32 dwHandle, TNetProtoList *ptProtoList);

/**
 * @brief  ���������豸����
 * @param  dwHandle        [in]���
 *         dwProtoMask     [in]����ָ��Э�����͵��豸��EProtoType���ֵ
 *         ptSearchDevTask [out]����������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreateSearchDevTask(u32 dwHandle, u32 dwProtoMask, TNetSearchDevTask *ptSearchDevTask);

/**
 * @brief  ���������豸����֧�ָ�����������
 * @param  dwHandle        [in]���
 *         tSearchDevParam [in]��������
 *         ptSearchDevTask [out]����������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreateSearchDevTaskEx(u32 dwHandle, TNetSearchDevParam tSearchDevParam, TNetSearchDevTask *ptSearchDevTask);

/**
 * @brief  ���������豸����
 * @param  dwHandle [in]���
 *         dwTaskId [in]��������id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroySearchDevTask(u32 dwHandle, u32 dwTaskId);

/**
 * @brief  ��ȡ���������豸
 * @param  dwHandle          [in]���
 *         dwTaskId          [in]��������id
 *         ptSearchedDevList [out]���������豸�б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSearchedDevList(u32 dwHandle, u32 dwTaskId, TNetSearchedDevList *ptSearchedDevList);

/**
 * @brief  ��ȡnvrͨ���б�һ�����ɻ�ȡNET_PER_GET_CHNLIST_MAX_NUM
 * @param  dwHandle       [in]���
 *         tGetNvrChnList [in]ָ���Ļ�ȡ����
 *         ptNvrChnList   [out]��ȡ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNvrChnList(u32 dwHandle, TNetGetNvrChnList tGetNvrChnList, TNetNvrChnList *ptNvrChnList);

/**
 * @brief  ��ȡnvr�ϳ�ͨ���б����ɻ�ȡNET_COMPOSITE_CHN_MAX_NUM
 * @param  dwHandle               [in]���
 *         ptNvrChnCompositeCap   [out]��ȡ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNvrCompositeChnCap(u32 dwHandle, TNetNvrCompositeChnCap *ptNvrChnCompositeCap);

/**
 * @brief  ��ȡͨ������
 * @param  dwHandle       [in]���
 *         ptNvrChnCfg    [out]��ȡ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnCfg(u32 dwHandle, TNetChnCfg *ptNvrChnCfg);

/**
 * @brief  ����ͨ������
 * @param  dwHandle       [in]���
 *         dwChnId        [in]ͨ��id
 *         pszChnCfgInfo  [in]ͨ��������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetChnCfg(u32 dwHandle, s32 nChnId, const s8 *pszChnCfgInfo);

/**
 * @brief  ����豸��һ���������NET_PER_ADD_DEV_MAX_NUM
 * @param  dwHandle    [in]���
 *         tAddDevList [in]�豸�б�
 *         ptChnCfg    [out]�豸��Ϣ, �����豸ʱ��Ч, ֵΪNULLʱ��ʾ����ע
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_AddDev(u32 dwHandle, TNetAddDevList tAddDevList, TNetChnCfg *ptChnCfg);

/**
 * @brief  ���GB28181�豸
 * @param  dwHandle      [in]���
 *         pszChnCfgInfo [in]�����豸��Ϣ
 *         ptChnCfg      [out]�豸��Ϣ, �����豸ʱ��Ч, ֵΪNULLʱ��ʾ����ע
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_AddGB28181Dev(u32 dwHandle, const s8 *pszChnCfgInfo, TNetChnCfg *ptChnCfg);

/**
 * @brief  ɾ���豸
 * @param  dwHandle [in]���
 *         dwChnId  [in]ͨ��id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DelDev(u32 dwHandle, s32 nChnId);

/**
 * @brief  ɾ���豸
 * @param  dwHandle    [in]���
 *         tDelDevList [in]ɾ���豸�б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DelDevEx(u32 dwHandle, TNetDelDevList tDelDevList);

/**
 * @brief  ����ǰ��������������
 * @param  dwHandle                  [in]���
 *         tCreateDevActiveTaskInfo  [in]�����豸��Ϣ
 *         pnTaskId                  [out]������������ID
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreateDevActiveTask(u32 dwHandle, TNetCreateDevActiveTaskInfo tCreateDevActiveTaskInfo, s32 *pnTaskId);

/**
 * @brief  ����ǰ��������������
 * @param  dwHandle                  [in]���
 *         nTaskId                   [in]��������ID
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroyDevActiveTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  ���ǰ�˵�������������
 * @param  dwHandle                  [in]���
 *         tAddDevActiveInfo         [in]Ҫ��ӵ�ǰ����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_AddDevToActiveTask(u32 dwHandle, TNetAddDevActiveInfo tAddDevActiveInfo);

/**
 * @brief  ��ѯǰ��������������״̬
 * @param  dwHandle                  [in]���
 *         tQueryDevActiveTaskState  [in]��ѯ�����Ϣ
 *         ptDevActiveTaskState      [out]����״̬��Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_QueryDevActiveState(u32 dwHandle, TNetQueryDevActiveTaskState tQueryDevActiveTaskState, TNetDevActiveTaskState *ptDevActiveTaskState);

/**
 * @brief  �޸���������ǰ���������
 * @param  dwHandle                  [in]���
 *         tModifyDevIpInfo          [in]�޸���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_ModifyDevIp(u32 dwHandle, TNetModifyDevIpInfo tModifyDevIpInfo);

/**
 * @brief  ����ǰ��������������
 * @param  dwHandle                  [in]���
 *         tCreateDevUpgradeTaskInfo [in]�����豸����������Ϣ
 *         pnTaskId                  [out]��������ID
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreateDevUpgradeTask(u32 dwHandle, TNetCreateDevUpgradeTaskInfo tCreateDevUpgradeTaskInfo, s32 *pnTaskId);

/**
 * @brief  ����ǰ��������������
 * @param  dwHandle                  [in]���
 *         nTaskId                   [in]��������ID
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroyDevUpgradeTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  ��ѯǰ��������������״̬
 * @param  dwHandle                  [in]���
 *         nTaskId                   [in]��������ID
 *         ptDevUpgradeStateList     [out]����״̬��Ϣ�б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_QueryDevUpgradeState(u32 dwHandle, s32 nTaskId, TNetDevUpgradeStateList *ptDevUpgradeStateList);

/**
 * @brief  �ϴ�ǰ������������
 * @param  dwHandle                  [in]���
 *         nTaskId                   [in]��������ID
 *         tDevUpgradePkgData        [in]����������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_UploadDevUpgradePkg(u32 dwHandle, s32 nTaskId, TNetDevUpgradePkgData tDevUpgradePkgData);

/**
 * @brief  ��ȡͨ������
 * @param  dwHandle         [in]���
 *         nChnIdStart      [in]��ʼͨ��id
 *         nChnIdEnd        [in]����ͨ��id
 *         ptChnAliasList   [out]��ȡ�Ľ��
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnAlias(u32 dwHandle, s32 nChnIdStart, s32 nChnIdEnd, TNetChnAliasList *ptChnAliasList);

/**
 * @brief  ����ͨ������
 * @param  dwHandle       [in]���
 *         tChnAliasList  [in]ͨ�������б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetChnAlias(u32 dwHandle, TNetChnAliasList tChnAliasList);

/**
 * @brief  ���õ���ͨ������
 * @param  dwHandle       [in]���
 *         nChnId         [in]ͨ��id
 *         pszChnAlias    [in]ͨ������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetChnAliasEx(u32 dwHandle, s32 nChnId,  const char* pszChnAlias);

/**
 * @brief  ��ȡͨ�������б�
 * @param  dwHandle         [in]���
 *         dwStartId        [in]ͨ������ʼid����1��ʼ
 *         dwEndId          [in]ͨ�������id�����NET_CHNGROUP_MAX_NUM
 *         ptChnGroupList   [out]��ȡ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnGroupList(u32 dwHandle, s32 nStartId, s32 nEndId, TNetChnGroupList *ptChnGroupList);

/**
 * @brief  ����ͨ�������б�
 * @param  dwHandle         [in]���
 *         tChnGroupList    [in]ͨ������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetChnGroupList(u32 dwHandle, TNetChnGroupList tChnGroupList);

/**
 * @brief  ��ȡͨ��ʱ��ͬ������
 * @param  dwHandle            [in]���
 *         ptTimeSyncCfg       [out]��ȡ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetTimeSyncCfg(u32 dwHandle, TNetTimeSyncCfg *ptTimeSyncCfg);

/**
 * @brief  ����ͨ��ʱ��ͬ������
 * @param  dwHandle            [in]���
 *         tTimeSyncCfg        [in]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetTimeSyncCfg(u32 dwHandle, TNetTimeSyncCfg tTimeSyncCfg);

/**
 * @brief  ��ȡǰ��OSD����
 * @param  dwHandle            [in]���
 *         nChnId              [in]ͨ��id
 *         ptDevOsdCfg         [out]��ȡ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDevOsdCfg(u32 dwHandle, s32 nChnId, TNetDevOsdCfg *ptDevOsdCfg);

/**
 * @brief  ��ȡǰ��OSD����
 * @param  dwHandle            [in]���
 *         nChnId              [in]ͨ��id
 *         ptDevOsdCfg         [in]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetDevOsdCfg(u32 dwHandle, s32 nChnId, TNetDevOsdCfg tDevOsdCfg);

/**
 * @brief  �ϴ�ǰ��OSDͼƬ
 * @param  dwHandle            [in]���
 *         nChnId              [in]ͨ��id
 *         tOsdPicList         [in]ͼƬ��Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_UploadDevOsdPic(u32 dwHandle, s32 nChnId, TNetOsdPicFileList tOsdPicList);

/**
 * @brief  ��ȡͨ����Ƶ��������
 * @param  dwHandle            [in]���
 *         ptChnVideoEncParam  [out]��ȡ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnVideoEnc(u32 dwHandle, TNetChnVideoEncParam *ptChnVideoEncParam);

/**
 * @brief  ��ȡͨ����Ƶ��������(�첽)
 * @param  dwHandle   [in]���
 *         nChnId     [in]ͨ��id
 *         pfCallBack [in]�ص�����
 *         pParam     [in]�ص���������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnVideoEncAsync(u32 dwHandle, s32 nChnId, pfAPICallBack pfCallBack, void *pParam);

/**
 * @brief  ����ͨ����Ƶ��������
 * @param  dwHandle            [in]���
 *         tChnVideoEncParam   [in]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetChnVideoEnc(u32 dwHandle, TNetChnVideoEncParam tChnVideoEncParam);

/**
 * @brief  ��ȡͨ����Ƶ��������
 * @param  dwHandle            [in]���
 *         ptChnAudioEncParam  [out]��ȡ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnAudioEnc(u32 dwHandle, TNetChnAudioEncParam *ptChnAudioEncParam);

/**
 * @brief  ��ȡͨ����Ƶ��������(�첽)
 * @param  dwHandle   [in]���
 *         nChnId     [in]ͨ��id
 *         pfCallBack [in]�ص�����
 *         pParam     [in]�ص���������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnAudioEncAsync(u32 dwHandle, s32 nChnId, pfAPICallBack pfCallBack, void *pParam);

/**
 * @brief  ����ͨ����Ƶ��������
 * @param  dwHandle            [in]���
 *         tChnAudioEncParam   [in]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetChnAudioEnc(u32 dwHandle, TNetChnAudioEncParam tChnAudioEncParam);

/**
 * @brief  ��ȡͨ����Ƶ��������
 * @param  dwHandle            [in]���
 *         ptChnAudioDecParam  [out]��ȡ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnAudioDec(u32 dwHandle, TNetChnAudioDecParam *ptChnAudioDecParam);

/**
 * @brief  ����ͨ����Ƶ��������
 * @param  dwHandle            [in]���
 *         tChnAudioDecParam   [in]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetChnAudioDec(u32 dwHandle, TNetChnAudioDecParam tChnAudioDecParam);

/**
 * @brief  ͨ���ַ�����ʶת��Ϊͨ��id
 * @param  dwHandle  [in]���
 *         pszChnStr [in]ͨ���ַ�����ʶ
 *         pnChnId   [out]ͨ��id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_ChnStr2ChnId(u32 dwHandle, const s8 *pszChnStr, s32 *pnChnId);

/**
 * @brief  ��ȡ����������
 * @param  dwHandle    [in]���
 *         nChnId      [in]ͨ��id
 *         nSchemeId   [in]����id����1��ʼ������Ϊ0��0����ʾ��ȡ��ǰ����
 *         ptCordonCfg [out]����������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCordonCfg(u32 dwHandle, s32 nChnId, s32 nSchemeId, TNetCordonCfg *ptCordonCfg);

/**
 * @brief  ���þ���������
 * @param  dwHandle   [in]���
 *         nChnId     [in]ͨ��id
 *         tCordonCfg [in]����������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetCordonCfg(u32 dwHandle, s32 nChnId, TNetCordonCfg tCordonCfg);

/**
 * @brief  ��ȡ��������
 * @param  dwHandle        [in]���
 *         nChnId          [in]ͨ��id
 *         nSchemeId       [in]����id����1��ʼ������Ϊ0��0����ʾ��ȡ��ǰ����
 *         eSmartAlarmType [in]�����������ͣ�֧��������롢�����뿪���������֡���Ʒ��ȡ����Ʒ����
 *         ptAreaCfg       [out]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAreaCfg(u32 dwHandle, s32 nChnId, s32 nSchemeId, ESmartAlarmType eSmartAlarmType, TNetAreaCfg *ptAreaCfg);

/**
 * @brief  ������������
 * @param  dwHandle        [in]���
 *         nChnId          [in]ͨ��id
 *         eSmartAlarmType [in]�����������ͣ�֧��������롢�����뿪���������֡���Ʒ��ȡ����Ʒ����
 *         tAreaCfg        [in]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetAreaCfg(u32 dwHandle, s32 nChnId, ESmartAlarmType eSmartAlarmType, TNetAreaCfg tAreaCfg);

/**
 * @brief  ��ȡ�齹�������
 * @param  dwHandle            [in]���
 *         nChnId              [in]ͨ��id
 *         ptVirFocusDetectCfg [out]�齹�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetVirFocusDetectCfg(u32 dwHandle, s32 nChnId, TNetVirFocusDetectCfg *ptVirFocusDetectCfg);

/**
 * @brief  �����齹�������
 * @param  dwHandle           [in]���
 *         nChnId             [in]ͨ��id
 *         tVirFocusDetectCfg [in]�齹�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetVirFocusDetectCfg(u32 dwHandle, s32 nChnId, TNetVirFocusDetectCfg tVirFocusDetectCfg);

/**
 * @brief  ��ȡ�����������
 * @param  dwHandle         [in]���
 *         nChnId           [in]ͨ��id
 *         ptSceneChangeCfg [out]�����������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSceneChangeCfg(u32 dwHandle, s32 nChnId, TNetSceneChangeCfg *ptSceneChangeCfg);

/**
 * @brief  ���ó����������
 * @param  dwHandle        [in]���
 *         nChnId          [in]ͨ��id
 *         tSceneChangeCfg [in]�����������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetSceneChangeCfg(u32 dwHandle, s32 nChnId, TNetSceneChangeCfg tSceneChangeCfg);

/**
 * @brief  ��ȡ��Ա�ۼ�����
 * @param  dwHandle          [in]���
 *         nChnId            [in]ͨ��id
 *         nSchemeId         [in]����id����1��ʼ������Ϊ0��0����ʾ��ȡ��ǰ����
 *         ptPeopleGatherCfg [out]��Ա�ۼ�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetPeopleGatherCfg(u32 dwHandle, s32 nChnId, s32 nSchemeId, TNetPeopleGatherCfg *ptPeopleGatherCfg);

/**
 * @brief  ������Ա�ۼ�����
 * @param  dwHandle         [in]���
 *         nChnId           [in]ͨ��id
 *         tPeopleGatherCfg [in]��Ա�ۼ�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetPeopleGatherCfg(u32 dwHandle, s32 nChnId, TNetPeopleGatherCfg tPeopleGatherCfg);

/**
 * @brief  ��ȡ�����쳣����
 * @param  dwHandle           [in]���
 *         nChnId             [in]ͨ��id
 *         ptAudioAbnormalCfg [out]�����쳣����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAudioAbnormalCfg(u32 dwHandle, s32 nChnId, TNetAudioAbnormalCfg *ptAudioAbnormalCfg);

/**
 * @brief  ���������쳣����
 * @param  dwHandle          [in]���
 *         nChnId            [in]ͨ��id
 *         tAudioAbnormalCfg [in]�����쳣����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetAudioAbnormalCfg(u32 dwHandle, s32 nChnId, TNetAudioAbnormalCfg tAudioAbnormalCfg);

/**
 * @brief  ��ȡ��������ͼʵʱ��ֵ
 * @param  dwHandle           [in]���
 *         nChnId             [in]ͨ��id
 *         ptAudioAbnormalCfg [out]�����쳣����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAudioMeter(u32 dwHandle, s32 nChnId, TNetAudioMeter *ptAudioMeter);

/**
 * @brief  ��ȡ�����������
 * @param  dwHandle        [in]���
 *         nChnId          [in]ͨ��id
 *         ptFaceDetectCfg [out]�����������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetFaceDetectCfg(u32 dwHandle, s32 nChnId, TNetFaceDetectCfg *ptFaceDetectCfg);

/**
 * @brief  ���������������
 * @param  dwHandle       [in]���
 *         nChnId         [in]ͨ��id
 *         tFaceDetectCfg [in]�����������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetFaceDetectCfg(u32 dwHandle, s32 nChnId, TNetFaceDetectCfg tFaceDetectCfg);

/**
 * @brief  ��ȡ���𱨾�����
 * @param  dwHandle				[in]���
 *         nChnId				[in]ͨ��id
 *         ptAccessProtectCfg	[out]���𱨾�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAccessProtectCfg(u32 dwHandle, s32 nChnId, TNetAccessProtectCfg *ptAccessProtectCfg);

/**
 * @brief  ���÷��𱨾�����
 * @param  dwHandle       [in]���
 *         nChnId         [in]ͨ��id
 *         tFaceDetectCfg [in]�����������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetAccessProtectCfg(u32 dwHandle, s32 nChnId, TNetAccessProtectCfg tAccessProtectCfg);

/**
 * @brief  ��ȡ¼�����ģ������
 * @param  dwHandle            [in]���
 *         ptRecMgrCap         [out]��ȡ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRecMgrCap(u32 dwHandle, TNetRecMgrCap *ptRecMgrCap);

/**
 * @brief  ��ȡ¼���¼�����
 * @param  dwHandle          [in]���
 *         pdwRecEventMask   [out]֧�ֵ�¼���¼��������룬ERecEventType���ֵ
 *  	   pdwInteDetectMask [out]֧�ֵ���������������룬ERecEventType���ֵ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRecEventType(u32 dwHandle, u32 *pdwRecEventMask, u32 *pdwInteDetectMask);

/**
 * @brief  ��ȡץ��ͼƬ�¼�����
 * @param  dwHandle            [in]���
 *         pdwSnapPicEventMask [out]֧�ֵ�ץ��ͼƬ�¼��������룬ESnapPicEventType���ֵ
 *  	   pdwInteDetectMask   [out]֧�ֵ���������������룬ESnapPicEventType���ֵ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSnapPicEventType(u32 dwHandle, u32 *pdwSnapPicEventMask, u32 *pdwInteDetectMask);

/**
 * @brief  ��ȡͨ��¼��ƻ�
 * @param  dwHandle            [in]���
 *         ptChnRecPlan        [in/out]����id���������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnRecPlan(u32 dwHandle, TNetChnRecPlan *ptChnRecPlan);

/**
 * @brief  ����ͨ��¼��ƻ�
 * @param  dwHandle            [in]���
 *         tChnRecPlan         [in]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetChnRecPlan(u32 dwHandle, TNetChnRecPlan tChnRecPlan);

/**
 * @brief  ����ͨ��¼��ƻ�
 * @param  dwHandle            [in]���
 *         tCopyChnRecPlan     [in]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CopyChnRecPlan(u32 dwHandle, TNetCopyChnRecPlan tCopyChnRecPlan);

/**
 * @brief  ��ȡͨ��ץ�ļƻ�
 * @param  dwHandle            [in]���
 *         ptChnSnapPlan       [in/out]����id���������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnSnapPlan(u32 dwHandle, TNetChnSnapPlan *ptChnSnapPlan);

/**
 * @brief  ����ͨ��ץ�ļƻ�
 * @param  dwHandle            [in]���
 *         tChnSnapPlan        [in]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetChnSnapPlan(u32 dwHandle, TNetChnSnapPlan tChnSnapPlan);

/**
 * @brief  ����ͨ��ץ�ļƻ�
 * @param  dwHandle            [in]���
 *         tCopyChnSnapPlan    [in]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CopyChnSnapPlan(u32 dwHandle, TNetCopyChnSnapPlan tCopyChnSnapPlan);

/**
 * @brief  ��ȡ¼��߼�����
 * @param  dwHandle            [in]���
 *         ptRecAdvanceOption  [out]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRecAdvanceOption(u32 dwHandle, TNetRecAdvanceOption *ptRecAdvanceOption);

/**
 * @brief  ����¼��߼�����
 * @param  dwHandle            [in]���
 *         ptRecAdvanceOption  [in]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetRecAdvanceOption(u32 dwHandle, TNetRecAdvanceOption tRecAdvanceOption);

/**
 * @brief  ��ȡ��������
 * @param  dwHandle            [in]���
 *         ptHolidayCfgList    [out]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetHolidayCfg(u32 dwHandle, TNetHolidayCfgList *ptHolidayCfgList);

/**
 * @brief  ���ü�������
 * @param  dwHandle            [in]���
 *         tHolidayCfgLis      [in]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetHolidayCfg(u32 dwHandle, TNetHolidayCfgList tHolidayCfgLis);

/**
 * @brief  ����¼�񱸷�����
 * @param  dwHandle            [in]���
 *         tRecBackupData      [in]��������
 *         pnTaskId            [out]����������ID
 *         pullDataSize        [out]���δ�������ݴ�С
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreateRecBackupTask(u32 dwHandle, TNetRecBackupData tRecBackupData, s32 *pnTaskId, u64 *pullDataSize);

/**
 * @brief  ���¼�񱸷�����
 * @param  dwHandle            [in]���
 *         nTaskId             [in]����ID
 *         tRecBackupData      [in]��������
 *         pullDataSize        [out]���δ�������ݴ�С
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_AddRecBackupData(u32 dwHandle, s32 nTaskId, TNetRecBackupData tRecBackupData, u64 *pullDataSize);

/**
 * @brief  ����¼�񱸷�����
 * @param  dwHandle            [in]���
 *         nTaskId             [in]����ID
 *         eRecBackupType      [in]��������
 *         pszBackupPath       [in]����·��
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StartRecBackupTask(u32 dwHandle, s32 nTaskId, ERecBackupType eRecBackupType, s8 *pszBackupPath);

/**
 * @brief  ��ѯ¼�񱸷�״̬
 * @param  dwHandle            [in]���
 *         nTaskId             [in]����ID
 *         tRecBackupData      [in]��ѯ����
 *         ptRecBackupState    [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_QueryRecBackupState(u32 dwHandle, s32 nTaskId, TNetRecBackupData tRecBackupData, TNetRecBackupState *ptRecBackupState);

/**
 * @brief  ����¼�񱸷�����
 * @param  dwHandle            [in]���
 *         nTaskId             [in]����ID
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroyRecBackupTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  web���ؿ�ʼ
 * @param  dwHandle            [in]���
 * @param  wPort			   [in]���ض˿�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_WebRecDownloadStart(u32 dwHandle, u16 wPort);

/**
 * @brief  web���ر���
 * @param  dwHandle            [in]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_WebRecDownloadKeep(u32 dwHandle);

/**
 * @brief  web���ؽ���
 * @param  dwHandle            [in]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_WebRecDownloadStop(u32 dwHandle);

/**
 * @brief  ����¼��ǰ��ȡ¼����Ϣ
 * @param  dwHandle            [in]���
 *         tLockRecList        [in]��������
 *         ptLockRecData       [out]����¼����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRecInfoBeforeLock(u32 dwHandle, TNetLockRecList tLockRecList, TNetLockRecData *ptLockRecData);

/**
 * @brief  ����¼��
 * @param  dwHandle            [in]���
 *         tLockRecList        [in]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_LockRec(u32 dwHandle, TNetLockRecList tLockRecList);

/**
 * @brief  ����¼��
 * @param  dwHandle            [in]���
 *         tLockRecList        [in]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_UnLockRec(u32 dwHandle, TNetLockRecList tLockRecList);

/**
 * @brief  ����ͼƬ��������
 * @param  dwHandle            [in]���
 *         ptPicBackupData     [in]��������
 *         pnTaskId            [out]����������ID
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreatePicBackupTask(u32 dwHandle, TNetPicBackupData *ptPicBackupData, s32 *pnTaskId);

/**
 * @brief  ���ͼƬ��������
 * @param  dwHandle            [in]���
 *         nTaskId             [in]����ID
 *         ptPicBackupData     [in]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_AddPicBackupData(u32 dwHandle, s32 nTaskId, TNetPicBackupData *ptPicBackupData);

/**
 * @brief  ����ͼƬ��������
 * @param  dwHandle            [in]���
 *         nTaskId             [in]����ID
 *         pszBackupPath       [in]����·��
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StartPicBackupTask(u32 dwHandle, s32 nTaskId, s8 *pszBackupPath);

/**
 * @brief  ��ѯͼƬ����״̬
 * @param  dwHandle            [in]���
 *         nTaskId             [in]����ID
 *         ptPicBackupData     [in]��ѯ����
 *         ptRecBackupState    [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_QueryPicBackupState(u32 dwHandle, s32 nTaskId, TNetPicBackupData *ptPicBackupData, TNetPicBackupState *ptPicBackupState);

/**
 * @brief  ����ͼƬ��������
 * @param  dwHandle            [in]���
 *         nTaskId             [in]����ID
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroyPicBackupTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  ��ȡͨ��¼������ͼ
 * @param  dwHandle            [in]���
 *         tChnMonthRec        [in]��ȡ����
 *         pdwMonthRec         [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnMonthRec(u32 dwHandle, TNetGetChnMonthRec tGetChnMonthRec, TNetChnMonthRecList *ptChnMonthRecList);

/**
 * @brief  ������ѯ¼���¼���¼�������
 * @param  dwHandle                   [in]���
 *         tCreateQueryRecTask        [in]��ȡ����
 *         pnTaskId                   [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreatQueryRecTask(u32 dwHandle, TNetCreatQueryRecTask tCreateQueryRecTask, s32 *pnTaskId);

/**
 * @brief  ��ѯ¼���¼���
 * @param  dwHandle                      [in]���
 *         tGetQueryRecTaskResult        [in]��ȡ����
 *         tChnRecList                   [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRecTaskResult(u32 dwHandle, TNetGetRecTaskResult tGetRecTaskResult, TNetChnRecList *ptChnRecList);

/**
 * @brief  ���ٲ�ѯ¼���¼���¼�������
 * @param  dwHandle       [in]���
 *         nTaskId        [in]����id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroyQueryRecTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  �����ط�����
 * @param  dwHandle       [in]���
 *         ePlaybackType  [in]�ط�����
 *         pnTaskId       [out]����id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreatePlaybackTask(u32 dwHandle, EPlaybackType ePlaybackType, s32 *pnTaskId);

/**
 * @brief  ��ʼ�ط�
 * @param  dwHandle        [in]���
 *         tPlaybackList   [in]�ط���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StartPlayback(u32 dwHandle, EOutputType eOutputType, s32 nTaskId, TNetPlaybackList tPlaybackList);

/**
 * @brief  ֹͣ�ط�
 * @param  dwHandle        [in]���
 *         tPlaybackList   [in]�ط���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StopPlayback(u32 dwHandle, EOutputType eOutputType, s32 nTaskId, TNetPlaybackList tPlaybackList);

/**
 * @brief  ����Իط����������ͨ��
 * @param  dwHandle        [in]���
 *         nTaskId         [in]�ط�����ID
 *         tPlaybackList   [in]�ط���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_AddPlaybackChn(u32 dwHandle, s32 nTaskId, TNetPlaybackList tPlaybackList);

/**
 * @brief  �ӻ��Իط�������ɾ��ͨ��
 * @param  dwHandle        [in]���
 *         nTaskId         [in]�ط�����ID
 *         tPlaybackList   [in]�ط���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DelPlaybackChn(u32 dwHandle, s32 nTaskId, TNetPlaybackList tPlaybackList);

/**
 * @brief  ��ȡ¼���ļ���Ϣ��mp4��asf��
 * @param  dwHandle        [in]���
 *         pszUri          [in]�ļ�·��
 *         ptRecFileInfo   [out]�ļ���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRecFileInfo(u32 dwHandle, s8 *pszUri, TNetRecFileInfo *ptRecFileInfo);

/**
 * @brief  �ط�Vcr����
 * @param  dwHandle           [in]���
 *         tPlaybackVcrCtrl   [in]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_PlaybackVcrCtrl(u32 dwHandle, TNetPlaybackVcrCtrl tPlaybackVcrCtrl);

/**
 * @brief  �طŽ��Ȳ�ѯ
 * @param  dwHandle        [in]���
 *         nTaskId         [in]����id
 *         nChnId          [in]ͨ��id
 *         ptPlaybackState [out]�ط�״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_QueryPlaybackProgress(u32 dwHandle, s32 nTaskId, s32 nChnId, TNetPlaybackState *ptPlaybackState);

/**
 * @brief  ���ûط�ͬ������
 * @param  dwHandle       [in]���
 *         nTaskId        [in]����id
 *         bSync          [in]�Ƿ�ͬ��
 *         nRefChnID      [in]�ο�ͨ��id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetPlaybackSyncProp(u32 dwHandle, s32 nTaskId, BOOL32 bSync, s32 nRefChnID);

/**
 * @brief  ���ٻط�������
 * @param  dwHandle       [in]���
 *         nTaskId        [in]����id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroyPlaybackTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  ��ʼ�¼�ѭ���ط�
 * @param  dwHandle                       [in]���
 *         tEventCyclePlaybackInfo        [in]�¼�ѭ�������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StartEventCyclePlayback(u32 dwHandle, TNetEventCyclePlaybackInfo tEventCyclePlaybackInfo);

/**
 * @brief  ֹͣ�¼�ѭ���ط�
 * @param  dwHandle       [in]���
 *         nTaskId        [in]����id
 *         nChnId         [in]ͨ��id(ͬ����ʽ�ò�����Ч)
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StopEventCyclePlayback(u32 dwHandle, s32 nTaskId, s32 nChnId);

/**
 * @brief  ��ѯץ��ͼƬ
 * @param  dwHandle                 [in]���
 *         tQuerySnapPicInfo        [in]Ҫ��ѯ��ͨ����Ϣ
 *         ptSnapPicList            [out]����ͼƬ��Ϣ�б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_QuerySnapPic(u32 dwHandle, TNetQuerySnapPicInfo tQuerySnapPicInfo, TNetSnapPicList *ptSnapPicList);

/**
 * @brief  ��ȡץ��ͼƬ
 * @param  dwHandle			[in]���
 *         tGetSnapPic		[in]ͼƬ����
 *         ptGetPicData		[out]ͼƬ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSnapPic(u32 dwHandle, TNetGetSnapPic tGetSnapPic, TNetPicData *ptGetPicData);

/**
 * @brief  ��ѯ��ʷ��ǩ
 * @param  dwHandle                 [in]���
 *         ptSnapPicList            [out]���ر�ǩ��Ϣ�б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_QueryHistroyRecTag(u32 dwHandle, TNetRecTagList *ptRecTagList);

/**
 * @brief  ¼���ǩ����
 * @param  dwHandle                 [in]���
 *         tRecTagOperateParam      [in]��ǩ��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_RecTagOperate(u32 dwHandle, TNetRecTagOperateParam tRecTagOperateParam);

/**
 * @brief  ����¼���ǩ��ѯ����
 * @param  dwHandle                         [in]���
 *         tCreateQueryRecTagTaskParam      [in]�����������
 *         pnTaskId                         [out]��ѯ����ID
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreateQueryRecTagTask(u32 dwHandle, TNetCreateQueryRecTagTaskParam tCreateQueryRecTagTaskParam, s32 *pnTaskId);

/**
 * @brief  ��ȡ¼���ǩ��ѯ���
 * @param  dwHandle                         [in]���
 *         tQueryRecTagResultParam          [in]��ѯ����
 *         ptQueryRecTagResult              [out]¼���ǩ��ѯ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetQueryRecTagResult(u32 dwHandle, TNetQueryRecTagResultParam tQueryRecTagResultParam, TNetQueryRecTagResult *ptQueryRecTagResult);

/**
 * @brief  ����¼���ǩ��ѯ����
 * @param  dwHandle                         [in]���
 *         nTaskId                          [in]����ID
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroyQueryRecTagTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  �����ط�����
 * @param  dwHandle             [in]���
 *         ePlaybackType        [in]�ط�����
 *		   bCreateRtspUrl		[in]�Ƿ����� rtsp url
 *         tPlaybackDstChnList  [in]�ط�ͨ����Ϣ
 *         pnTaskId             [out]����id
 *         ptPlaybackSrcChnList [out]�ط�ͨ����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreatePlaybackTaskEx(u32 dwHandle, EPlaybackType ePlaybackType, BOOL32 bCreateRtspUrl, TNetPlaybackDstChnList tPlaybackDstChnList, s32 *pnTaskId, TNetPlaybackSrcChnList *ptPlaybackSrcChnList);

/**
 * @brief  ���ٻط�����
 * @param  dwHandle       [in]���
 *         nTaskId        [in]����id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroyPlaybackTaskEx(u32 dwHandle, s32 nTaskId);

/**
 * @brief  ��ȡ������ȡ�ط�������rtsp url
 * @param  dwHandle		               [in]���
 *         tNetGetRtspPlaybackUrlParam   [in]�������
 *         ptNetRtspPlaybackUrl       [out]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRtspPlaybackUrl(u32 dwHandle, TNetGetRtspPlaybackUrlParam tNetGetRtspPlaybackUrlParam, TNetRtspPlaybackUrl *ptNetRtspPlaybackUrl);

/**
 * @brief  ��ʼ�ط�
 * @param  dwHandle [in]���
 *         nTaskId  [in]����id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StartPlaybackEx(u32 dwHandle, s32 nTaskId);

/**
 * @brief  ֹͣ�ط�
 * @param  dwHandle [in]���
 *         nTaskId  [in]����id
 *         nChnId   [in]ͨ��id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StopPlaybackEx(u32 dwHandle, s32 nTaskId, s32 nChnId);

/**
 * @brief  ��ط��������ͨ��
 * @param  dwHandle             [in]���
 *         nTaskId              [in]����id
 *         tPlaybackDstChnList  [in]�ط�ͨ����Ϣ
 *         ptPlaybackSrcChnList [out]�ط�ͨ����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_AddPlaybackChnEx(u32 dwHandle, s32 nTaskId, TNetPlaybackDstChnList tPlaybackDstChnList, TNetPlaybackSrcChnList *ptPlaybackSrcChnList);

/**
 * @brief  �ӻط�����ɾ��ͨ��
 * @param  dwHandle [in]���
 *         nTaskId  [in]����ID
 *         panChnId [in]ͨ������
 *         nNum     [in]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DelPlaybackChnEx(u32 dwHandle, s32 nTaskId, const s32 *panChnId, s32 nNum);

/**
 * @brief  ��ȡӲ�̹�������
 * @param  dwHandle     [in]���
 *         ptDiskMgrCap [out]Ӳ�̹�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDiskMgrCap(u32 dwHandle, TNetDiskMgrCap *ptDiskMgrCap);

/**
 * @brief  ��ȡӲ�̷�����;��Ϣ
 * @param  dwHandle				[in]���
 *         ptDiskPartTypeList	[out]Ӳ�̷�����;��Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDiskPartUsingInfo(u32 dwHandle, TNetDiskPartTypeParam tDiskPartTypeParam, TNetDiskPartTypeList *ptDiskPartTypeList);

/**
 * @brief  ��ȡӲ���б�
 * @param  dwHandle   [in]���
 *         ptDiskList [out]Ӳ���б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDiskList(u32 dwHandle, TNetDiskList *ptDiskList);

/**
 * @brief  ��������Ӳ��
 * @param  dwHandle      [in]���
 *         tNetDiskParam [in]��������
 *         pdwTaskId     [out]��������id�����ڻ�ȡ����Ӳ��
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SearchNetDisk(u32 dwHandle, TNetNetDiskParam tNetDiskParam, u32 *pdwTaskId);

/**
 * @brief  ��ȡ����Ӳ��
 * @param  dwHandle      [in]���
 *         dwTaskId      [in]��������id
 *         ptNetDiskList [out]����Ӳ���б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNetDisk(u32 dwHandle, u32 dwTaskId, TNetNetDiskList *ptNetDiskList);

/**
 * @brief  �������Ӳ��
 * @param  dwHandle      [in]���
 *         tNetDiskParam [in]��Ӳ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_AddNetDisk(u32 dwHandle, TNetNetDiskParam tNetDiskParam);

/**
 * @brief  ж��Ӳ�̣�һ�ο�ж�ض��Ӳ��
 * @param  dwHandle  [in]���
 *         tDiskList [in]ж��Ӳ���б�ֻ����д�ֶ�TNetDiskList::swDiskNum TNetDiskInfo::byDiskId
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_UnmountDisk(u32 dwHandle, TNetDiskList tDiskList);

/**
 * @brief  ����Ӳ�̴洢��;
 * @param  dwHandle				[in]���
 *         tDiskUsingTypeParam	[in]�洢��;����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetDiskUsingType(u32 dwHandle, TNetDiskUsingTypeList tDiskUsingTypeList);

/**
 * @brief  ��ʼ��Ӳ��
 * @param  dwHandle				[in]���
 *         tNetDiskInitParam	[in]��ʼ������
 *         pwTaskId				[out]���س�ʼ��task id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_InitDisk(u32 dwHandle, TNetDiskInitParam tNetDiskInitParam, u16 *pwTaskId);

/**
 * @brief  ��ʼ��Ӳ��(r2b2����ǰ�汾)
 * @param  dwHandle  [in]���
 *         byDeskId  [in]Ӳ��id
 *         pwTaskId  [out]���س�ʼ��task id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_InitDiskEx(u32 dwHandle, u8 byDeskId, u16 *pwTaskId);

/**
 * @brief  ��ʼ��USB���Ӳ��(��ʽ��ΪNTFS)
 * @param  dwHandle     [in]���
 *         pszPartName  [in]��Ҫ��ʽ���ķ�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_InitUsbExtDisk(u32 dwHandle, s8 *pszPartName);

/**
 * @brief  ����Ӳ�̶�д����
 * @param  dwHandle           [in]���
 *         tDiskAttributeList [in]Ӳ��������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_EditDiskAttribute(u32 dwHandle, TNetDiskAttributeList tDiskAttributeList);

/**
 * @brief  ��ȡ��ǰӲ�̵Ĺ���ģʽ
 * @param  dwHandle        [in]���
 *         peStoreMode     [out]������ǰ��ЧӲ�̵Ĺ���ģʽ��Ϣ
 *         peCfgStoreMode  [out]��������Ӳ�̵Ĺ���ģʽ��Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDiskStoreMode(u32 dwHandle, EStoreMode *peStoreMode, EStoreMode *peCfgStoreMode, u32 *pdwStoreModeCap);

/**
 * @brief  ���õ�ǰӲ�̵Ĺ���ģʽ
 * @param  dwHandle        [in]���
 *         eStoreMode      [in]Ҫ���õ����̹���ģʽ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetDiskStoreMode(u32 dwHandle, EStoreMode eStoreMode);

/**
 * @brief  ��ȡӲ���������
 * @param  dwHandle        [in]���
 *         ptDiskQuotaCfg  [in/out]����ͨ��id�����������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDiskQuotaCfg(u32 dwHandle, TNetDiskQuotaCfg *ptDiskQuotaCfg);

/**
 * @brief  ����Ӳ���������
 * @param  dwHandle        [in]���
 *         tDiskQuotaCfg   [in]�����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetDiskQuotaCfg(u32 dwHandle, TNetDiskQuotaCfg tDiskQuotaCfg);

/**
 * @brief  ����ͨ��Ӳ���������
 * @param  dwHandle                [in]���
 *         tChnDiskQuotaCopyInfo   [in]���������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CopyChnDiskQuotaCfg(u32 dwHandle, TNetChnDiskQuotaCopyInfo tChnDiskQuotaCopyInfo);

/**
 * @brief  ��ȡͨ����������
 * @param  dwHandle            [in]���
 *         ptChnDiskGroupList  [out]����ͨ������������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnDiskGroupCfg(u32 dwHandle, TNetChnDiskGroupList *ptChnDiskGroupList);

/**
 * @brief  ����ͨ����������
 * @param  dwHandle            [in]���
 *         tChnDiskGroupList   [in]ͨ������������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetChnDiskGroupCfg(u32 dwHandle, TNetChnDiskGroupList tChnDiskGroupList);

/**
 * @brief  ��ȡӲ�̸߼�����
 * @param  dwHandle            [in]���
 *         ptDiskAdvanceCfg    [out]Ӳ�̸߼�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDiskAdvanceCfg(u32 dwHandle, TNetDiskAdvanceCfg *ptDiskAdvanceCfg);

/**
 * @brief  ����Ӳ�̸߼�����
 * @param  dwHandle            [in]���
 *         tDiskAdvanceCfg     [in]Ӳ�̸߼�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetDiskAdvanceCfg(u32 dwHandle, TNetDiskAdvanceCfg tDiskAdvanceCfg);

/**
 * @brief  ����RAID
 * @param  dwHandle         [in]���
 *         tCreateRaidParam [in]����raid����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreateRaid(u32 dwHandle, TNetCreateRaidParam tCreateRaidParam);

/**
 * @brief  ��ȡ����RAID״̬
 * @param  dwHandle         [in]���
 * @return ������ɷ���NET_OK���������з���ERR_NET_DM_DISK_RAID_CREATING��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCreateRaidStatus(u32 dwHandle);

/**
 * @brief  ��ȡRAID��Ϣ
 * @param  dwHandle         [in]���
 *         tRaidInfo        [out]raid��Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRaidInfo(u32 dwHandle, TNetRaidInfo *ptRaidInfo);

/**
 * @brief  ����ȱ���
 * @param  dwHandle         [in]���
 *         nDiskId          [in]����ID
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_AddHotBackupDisk(u32 dwHandle, s32 nDiskId);

/**
 * @brief  ɾ���ȱ���
 * @param  dwHandle         [in]���
 *         nDiskId          [in]����ID
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DelHotBackupDisk(u32 dwHandle, s32 nDiskId);

/**
 * @brief  ɾ��RAID
 * @param  dwHandle         [in]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DelRaid(u32 dwHandle);

/**
 * @brief  ��ȡɾ��RAID״̬
 * @param  dwHandle         [in]���
 * @return ɾ����ɷ���NET_OK��ɾ���з���ERR_NET_DM_DISK_RAID_DELING��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDelRaidStatus(u32 dwHandle);

/**
 * @brief  �ָ�Raid
 * @param  dwHandle         [in]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_RecoveryRaid(u32 dwHandle);

/**
 * @brief  ��ȡ����ط��������
 * @param  dwNvrIp    [in]NVR IP��������
 *         wPort      [in]�˿ڣ�������
 *         ptFrameCap [out]��ȡ���������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetFrameCap(u32 dwNvrIp, u16 wPort, TNetFrameCap *ptFrameCap);

/**
 * @brief  �����ʼ��
 * @param  dwNvrIp [in]NVR IP��������
 *         wPort   [in]�˿ڣ�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_MonitorInit(u32 dwNvrIp, u16 wPort);

/**
 * @brief  ��ȡ���б�
 * @param  dwNvrIp     [in]NVR IP��������
 *         wPort       [in]�˿ڣ�������
 *         eOutputType [in]��ʾ����
 *         ptBindList  [out]���б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetBindList(u32 dwNvrIp, u16 wPort, EOutputType eOutputType, TNetBindList *ptBindList);

/**
 * @brief  ��ȡ������
 * @param  dwNvrIp     [in]NVR IP��������
 *         wPort       [in]�˿ڣ�������
 *         eOutputType [in]��ʾ����
 *         ptGridList  [out]��������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetFrameStyle(u32 dwNvrIp, u16 wPort, EOutputType eOutputType, TNetGridList *ptGridList);

/**
 * @brief  ���û�����
 * @param  dwNvrIp     [in]NVR IP��������
 *         wPort       [in]�˿ڣ�������
 *         eOutputType [in]��ʾ����
 *         tGridList   [in]��������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetFrameStyle(u32 dwNvrIp, u16 wPort, EOutputType eOutputType, TNetGridList tGridList);

/**
 * @brief  ��
 * @param  dwHandle    [in]���
 *         eOutputType [in]��ʾ����
 *         tBindList   [in]���б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_Bind(u32 dwHandle, EOutputType eOutputType, TNetBindList tBindList);

/**
 * @brief  ������
 * @param  dwHandle    [in]���
 *         eOutputType [in]��ʾ����
 *         wGridId1    [in]��������1
 *         wGridId2    [in]��������2
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SwitchBind(u32 dwHandle, EOutputType eOutputType, u16 wGridId1, u16 wGridId2);

/**
 * @brief  ���
 * @param  dwHandle    [in]���
 *         eOutputType [in]��ʾ����
 *         wGridId     [in]grid id
 *		   nChnId      [in]chn id
 *         bNotSave    [in]�Ƿ񲻱���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_Unbind(u32 dwHandle, EOutputType eOutputType, u16 wGridId, s32 nChnId, BOOL32 bNotSave);

/**
 * @brief  �������
 * @param  dwHandle    [in]���
 *         eOutputType [in]��ʾ����
 *         tUnbindList [in]����б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_UnbindEx(u32 dwHandle, EOutputType eOutputType, TNetBindList tUnbindList);

/**
 * @brief  ���û��л�
 * @param  dwHandle    [in]���
 *         eOutputType [in]��ʾ����
 *         tPipList    [in]���л��б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetPip(u32 dwHandle, EOutputType eOutputType, TNetPipList tPipList);

/**
 * @brief  ץ��
 * @param  dwHandle    [in]���
 *         eOutputType [in]��ʾ����
 *         wGridId     [in]����id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_Snap(u32 dwHandle, EOutputType eOutputType, u16 wGridId);

/**
 * @brief  ͨ��ץ��
 * @param  dwHandle    [in]���
 *         tChnSnap    [in]ץ�Ĳ���
 *         paChnSnapFileInfo [out]ץ�ĵ��ļ���Ϣ����
 *         pnFileNum         [in/out]����paChnSnapFileInfoָ��������С������ļ���Ϣ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_ChnSnap(u32 dwHandle, TNetChnSnap tChnSnap, TNetChnSnapFileInfo *paChnSnapFileInfo, s32 *pnFileNum);

/**
 * @brief  ��ȡ������������Ϣ
 * @param  dwHandle    [in]���
 *         eOutputType [in]��ʾ����
 *         ptVolume    [out]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetVolume(u32 dwHandle, EOutputType eOutputType, TNetVolume *ptVolume);

/**
 * @brief  ��������������
 * @param  dwHandle    [in]���
 *         eOutputType [in]��ʾ����
 *         tVolume     [in]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetVolume(u32 dwHandle, EOutputType eOutputType, TNetVolume tVolume);

/**
 * @brief  ������Ƶ����
 * @param  dwHandle  [in]���
 *         tAudioDec [in]��Ƶ�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StartAudioDec(u32 dwHandle, TNetAudioDec tAudioDec);

/**
 * @brief  ֹͣ��Ƶ����
 * @param  dwHandle  [in]���
 *         tAudioDec [in]��Ƶ�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StopAudioDec(u32 dwHandle, TNetAudioDec tAudioDec);

/**
 * @brief  ��ȡ��������
 * @param  dwHandle  [in]���
 *         ptCallCap [out]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCallCap(u32 dwHandle, TNetCallCap *ptCallCap);

/**
 * @brief  ��ʼ���У���֧�ֱ��غ���
 * @param  dwHandle [in]���
 *         tCall    [in/out]���в���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StartCall(u32 dwHandle, TNetCall *ptCall);

/**
 * @brief  ��ʼ���У�֧�ֱ��غ��С�Զ�˺���
 * @param  dwHandle [in]���
 *         tCall    [in/out]���в���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StartCalleEx(u32 dwHandle, TNetCallParam tCallParam, TNetCallResult *ptCallResult);

/**
 * @brief  ֹͣ����
 * @param  dwHandle [in]���
 *         nCallId  [in]����id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StopCall(u32 dwHandle, s32 nCallId);

/**
 * @brief  ��ȡ��������
 * @param  dwHandle     [in]���
 *         pnCallVolume [in]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCallVolume(u32 dwHandle, s32 *pnCallVolume);

/**
 * @brief  ���ú�������
 * @param  dwHandle    [in]���
 *         nCallVolume [in]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetCallVolume(u32 dwHandle, s32 nCallVolume);

/**
 * @brief  ����PTZ����
 * @param  dwHandle [in]���
 *         tPtzCtrl [in]PTZ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SendPtzCmd(u32 dwHandle, TNetPtzCtrl tPtzCtrl);

/**
 * @brief  ��ȡͨ������̨����Ԥ��λ��Ϣ
 * @param  dwHandle			[in]���
 *         nChnId			[in]ͨ����
 *         ptPtzPresetList	[out]��̨����Ԥ��λ��Ϣ			
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetPtzPresetAll(u32 dwHandle, s32 nChnId, TNetPtzPresetList *ptPtzPresetList);

/**
 * @brief  ��ȡǰ�˵�����ģʽ����
 * @param  dwHandle				[in]���
 *         ptNetFishEyeModeCfg	[in/out]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetFishEyeModeCfg(u32 dwHandle, TNetFishEyeModeCfg *ptNetFishEyeModeCfg);

/**
 * @brief  ����ǰ�˵�����ģʽ����
 * @param  dwHandle				[in]���
 *         tNetFishEyeModeCfg	[in]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetFishEyeModeCfg(u32 dwHandle, TNetFishEyeModeCfg tNetFishEyeModeCfg);

/**
 * @brief  ��ȡǰ�˵���������ptz����
 * @param  dwHandle					[in]���
 *         ptNetFishEyePtzRectCfg	[in/out]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetFishEyePtzRectCfg(u32 dwHandle, TNetFishEyePtzRectCfg *ptNetFishEyePtzRectCfg);

/**
 * @brief  ����ǰ�˵���������ptz����
 * @param  dwHandle					[in]���
 *         tNetFishEyePtzRectCfg	[in]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetFishEyePtzRectCfg(u32 dwHandle, TNetFishEyePtzRectCfg tNetFishEyePtzRectCfg);

/**
 * @brief  ��ȡISP����
 * @param  dwHandle		[in]���
 *         ptIspCaps	[out]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspCap(u32 dwHandle, TNetIspCaps *ptIspCaps);

/**
 * @brief  ��ȡISP����
 * @param  dwHandle [in]���
 *         ptIspCfg [in/out]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspCfg(u32 dwHandle, TNetIspCfg *ptIspCfg);

/**
 * @brief  ����ISP����
 * @param  dwHandle [in]���
 *         ptIspCfg [in]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspCfg(u32 dwHandle, TNetIspCfg tIspCfg);

/**
 * @brief  ��ȡͨ����isp ��������������
 * @param  dwHandle     [in]���
 *		   tIspGainCfg  [in/out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspGainCfg(u32 dwHandle, TNetIspGainCfg *ptIspGainCfg);

/**
 * @brief  ����ͨ����isp ��������������
 * @param  dwHandle     [in]���
 *		   tIspGainCfg  [in]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspGainCfg(u32 dwHandle, TNetIspGainCfg tIspGainCfg);

/**
 * @brief  ��ȡͨ����isp ��Ȧ�����������
 * @param  dwHandle     [in]���
 *		   ptIspIrisCfg  [in/out]��Ȧ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspIrisCfg(u32 dwHandle, TNetIspIrisCfg *ptIspIrisCfg);

/**
 * @brief  ����ͨ����isp ��Ȧ�����������
 * @param  dwHandle     [in]���
 *		   tIspIrisCfg  [in]��Ȧ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspIrisCfg(u32 dwHandle, TNetIspIrisCfg tIspIrisCfg);

/**
 * @brief  ��ȡͨ����isp �۽������������
 * @param  dwHandle			[in]���
 *		   ptIspFocusCfg	[in/out]�۽�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspFocusCfg(u32 dwHandle, TNetIspFocusCfg *ptIspFocusCfg);

/**
 * @brief  ����ͨ����isp �۽������������
 * @param  dwHandle      [in]���
 *		   tIspFocusCfg  [in]�۽�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspFocusCfg(u32 dwHandle, TNetIspFocusCfg tIspFocusCfg);

/**
 * @brief  ��ȡͨ����isp ���Ų����������
 * @param  dwHandle			[in]���
 *		   ptIspFocusCfg	[in/out]���Ų���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspShutterCfg(u32 dwHandle, TNetIspShutterCfg *ptIspShutterCfg);

/**
 * @brief  ����ͨ����isp ���Ų����������
 * @param  dwHandle			[in]���
 *		   tIspShutterCfg	[in]���Ų���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspShutterCfg(u32 dwHandle, TNetIspShutterCfg tIspShutterCfg);

/**
 * @brief  ��ȡͨ����isp ����˸ģʽ����
 * @param  dwHandle			[in]���
 *		   ptIspFreqModeCfg	[in/out]����˸ģʽ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspFreqModeCfg(u32 dwHandle, TNetIspFreqModeCfg *ptIspFreqModeCfg);

/**
 * @brief  ����ͨ����isp ����˸ģʽ����
 * @param  dwHandle			[in]���
 *		   tIspFreqModeCfg	[in]����˸ģʽ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspFreqModeCfg(u32 dwHandle, TNetIspFreqModeCfg tIspFreqModeCfg);

/**
 * @brief  ��ȡͨ����isp ��ƽ���������
 * @param  dwHandle			[in]���
 *		   ptIspFreqModeCfg	[in/out]��ƽ���������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspWhiteBalanceCfg(u32 dwHandle, TNetIspWhiteBalanceCfg *ptIspWhiteBalanceCfg);

/**
 * @brief  ����ͨ����isp ��ƽ���������
 * @param  dwHandle				[in]���
 *		   tIspWhiteBalanceCfg	[in]��ƽ���������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspWhiteBalanceCfg(u32 dwHandle, TNetIspWhiteBalanceCfg tIspWhiteBalanceCfg);

/**
 * @brief  ��ȡͨ����isp ��ҹת������
 * @param  dwHandle					[in]���
 *		   ptIspDayNightSwitchCfg	[in/out]��ҹת������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspDayNightSwitchCfg(u32 dwHandle, TNetIspDayNightSwitchCfg *ptIspDayNightSwitchCfg);

/**
 * @brief  ����ͨ����isp ��ҹת������
 * @param  dwHandle					[in]���
 *		   tIspDayNightSwitchCfg	[in]��ҹת������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspDayNightSwitchCfg(u32 dwHandle, TNetIspDayNightSwitchCfg tIspDayNightSwitchCfg);

/**
 * @brief  ��ȡͨ����isp 2D�������
 * @param  dwHandle			[in]���
 *		   ptIspDenoiseCfg	[in/out]2D�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspDenoise2DCfg(u32 dwHandle, TNetIspDenoiseCfg *ptIspDenoiseCfg);

/**
 * @brief  ����ͨ����isp 2D�������
 * @param  dwHandle			[in]���
 *		   tIspDenoiseCfg	[in]2D�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspDenoise2DCfg(u32 dwHandle, TNetIspDenoiseCfg tIspDenoiseCfg);

/**
 * @brief  ��ȡͨ����isp 3D�������
 * @param  dwHandle			[in]���
 *		   ptIspDenoiseCfg	[in/out]3D�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspDenoise3DCfg(u32 dwHandle, TNetIspDenoiseCfg *ptIspDenoiseCfg);

/**
 * @brief  ����ͨ����isp 3D�������
 * @param  dwHandle			[in]���
 *		   tIspDenoiseCfg	[in]3D�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspDenoise3DCfg(u32 dwHandle, TNetIspDenoiseCfg tIspDenoiseCfg);

/**
 * @brief  ��ȡͨ����isp ��̬���ڲ���
 * @param  dwHandle					[in]���
 *		   ptIspDynamicRegulateCfg	[in/out]��̬���ڲ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspDynamicRegulateCfg(u32 dwHandle, TNetIspDynamicRegulateCfg *ptIspDynamicRegulateCfg);

/**
 * @brief  ����ͨ����isp ��̬���ڲ���
 * @param  dwHandle					[in]���
 *		   tIspDynamicRegulateCfg	[in]��̬���ڲ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspDynamicRegulateCfg(u32 dwHandle, TNetIspDynamicRegulateCfg tIspDynamicRegulateCfg);

/**
 * @brief  ��ȡͨ����isp ͼ����ǿ����
 * @param  dwHandle					[in]���
 *		   ptIspImageEnhanceCfg		[in/out]ͼ����ǿ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspImageEnhanceCfg(u32 dwHandle, TNetIspImageEnhanceCfg *ptIspImageEnhanceCfg);

/**
 * @brief  ����ͨ����isp ͼ����ǿ����
 * @param  dwHandle					[in]���
 *		   tIspImageEnhanceCfg		[in]ͼ����ǿ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspImageEnhanceCfg(u32 dwHandle, TNetIspImageEnhanceCfg tIspImageEnhanceCfg);

/**
 * @brief  ��ȡͨ����isp ��������
 * @param  dwHandle					[in]���
 *		   ptIspStablizerCfg		[in/out]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspStablizerCfg(u32 dwHandle, TNetIspStablizerCfg *ptIspStablizerCfg);

/**
 * @brief  ����ͨ����isp ��������
 * @param  dwHandle				[in]���
 *		   tIspStablizerCfg		[in]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspStablizerCfg(u32 dwHandle, TNetIspStablizerCfg tIspStablizerCfg);

/**
 * @brief  ��ȡͨ����isp �ϳɿ�̬ģʽ����
 * @param  dwHandle					[in]���
 *		   ptIspCombinHdrCfg		[in/out]�ϳɿ�̬ģʽ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspCombinHdrCfg(u32 dwHandle, TNetIspCombinHdrCfg *ptIspCombinHdrCfg);

/**
 * @brief  ����ͨ����isp �ϳɿ�̬ģʽ����
 * @param  dwHandle				[in]���
 *		   tIspCombinHdrCfg		[in]�ϳɿ�̬ģʽ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspCombinHdrCfg(u32 dwHandle, TNetIspCombinHdrCfg tIspCombinHdrCfg);

/**
 * @brief  ��ȡͨ����isp ������ģʽ����
 * @param  dwHandle					[in]���
 *		   ptIspCombinHdrCfg		[in/out]������ģʽ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspDoubleLensModeCfg(u32 dwHandle, TNetIspDoubleLensCfg *ptIspDoubleLensModeCfg);

/**
 * @brief  ����ͨ����isp ������ģʽ����
 * @param  dwHandle				[in]���
 *		   tIspCombinHdrCfg		[in]������ģʽ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspDoubleLensModeCfg(u32 dwHandle, TNetIspDoubleLensCfg tIspDoubleLensModeCfg);

/**
 * @brief  ��ȡͨ����isp �ع�ȫ�Զ�ģʽ����
 * @param  dwHandle					[in]���
 *		   ptIspCombinHdrCfg		[in/out]�ع�ȫ�Զ�ģʽ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspExpAllAutoCfg(u32 dwHandle, TNetIspExpAllAutoCfg *ptIspExpAllAutoModeCfg);

/**
 * @brief  ����ͨ����isp �ع�ȫ�Զ�ģʽ����
 * @param  dwHandle				[in]���
 *		   tIspCombinHdrCfg		[in]�ع�ȫ�Զ�ģʽ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspExpAllAutoCfg(u32 dwHandle, TNetIspExpAllAutoCfg tIspExpAllAutoModeCfg);

/**
 * @brief  ��ȡͨ����isp �߼��Զ��ع����
 * @param  dwHandle					[in]���
 *		   ptProExposureCfg			[in/out]�߼��Զ��ع����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspProExposureCfg(u32 dwHandle, TNetIspProExposureCfg *ptProExposureCfg);

/**
 * @brief  ����ͨ����isp �߼��Զ��ع����
 * @param  dwHandle					[in]���
 *		   tProExposureCfg			[in]�߼��Զ��ع����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspProExposureCfg(u32 dwHandle, TNetIspProExposureCfg tProExposureCfg);

/**
 * @brief  ��ȡͨ����isp ����ģʽ����
 * @param  dwHandle					[in]���
 *		   ptSceneModeCfg			[in/out]����ģʽ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspSceneModeCfg(u32 dwHandle, TNetIspSceneModeCfg *ptSceneModeCfg);

/**
 * @brief  ����ͨ����isp ����ģʽ����
 * @param  dwHandle					[in]���
 *		   tSceneModeCfg			[in]����ģʽ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspSceneModeCfg(u32 dwHandle, TNetIspSceneModeCfg tSceneModeCfg);

/**
 * @brief  ��ȡͨ����isp ��ת�ͻ��Բ���
 * @param  dwHandle					[in]���
 *		   ptFlipAndPlayBackCfg		[in/out]��ת�ͻ��Բ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspFlipAndPlayBackCfg(u32 dwHandle, TNetIspFlipAndPlayBackCfg *ptFlipAndPlayBackCfg);

/**
 * @brief  ����ͨ����isp ��ת�ͻ��Բ���
 * @param  dwHandle					[in]���
 *		   tFlipAndPlayBackCfg		[in]��ת�ͻ��Բ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspFlipAndPlayBackCfg(u32 dwHandle, TNetIspFlipAndPlayBackCfg tFlipAndPlayBackCfg);

/**
 * @brief  ��ȡͨ����isp ��������ò���
 * @param  dwHandle				[in]���
 *		   ptIspInfraredCfg		[in/out]��������ò���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspInfraredCfg(u32 dwHandle, TNetIspInfraredCfg *ptIspInfraredCfg);

/**
 * @brief  ����ͨ����isp ��������ò���
 * @param  dwHandle				[in]���
 *		   tIspInfraredCfg		[in]��������ò���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspInfraredCfg(u32 dwHandle, TNetIspInfraredCfg tIspInfraredCfg);

/**
 * @brief  ��ȡͨ����isp �������ò���
 * @param  dwHandle				[in]���
 *		   tIspLaserCfg			[in/out]�������ò���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspLaserCfg(u32 dwHandle, TNetIspLaserCfg *ptIspLaserCfg);

/**
 * @brief  ����ͨ����isp �������ò���
 * @param  dwHandle				[in]���
 *		   tIspLaserCfg			[in]�������ò���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspLaserCfg(u32 dwHandle, TNetIspLaserCfg tIspLaserCfg);

/**
 * @brief  ��ȡͨ����isp �����������
 * @param  dwHandle				[in]���
 *		   ptIspLdcCfg			[in/out]�������ò���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIspLdcCfg(u32 dwHandle, TNetIspLdcCfg *ptIspLdcCfg);

/**
 * @brief  ����ͨ����isp �����������
 * @param  dwHandle				[in]���
 *		   tIspLdcCfg			[in]�������ò���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIspLdcCfg(u32 dwHandle, TNetIspLdcCfg tIspLdcCfg);

/**
 * @brief  ���ַŴ�
 * @param  dwHandle     [in]���
 *         eOutputType  [in]��ʾ����
 *         tDigitalZoom [in]���ַŴ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DigitalZoom(u32 dwHandle, EOutputType eOutputType, TNetDigitalZoom tDigitalZoom);

/**
 * @brief  ��ȡ����ģʽ
 * @param  dwHandle    [in]���
 *         eOutputType [in]��ʾ����
 *         tDecMode    [in/out]����ģʽ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDecMode(u32 dwHandle, EOutputType eOutputType, TNetDecMode *ptDecMode);

/**
 * @brief  ���ý���ģʽ
 * @param  dwHandle    [in]���
 *         eOutputType [in]��ʾ����
 *         tDecMode    [in]����ģʽ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetDecMode(u32 dwHandle, EOutputType eOutputType, TNetDecMode tDecMode);

/**
 * @brief  ��ȡ������Ϣ
 * @param  dwHandle			 [in]���
 *         eOutputType       [in]��ʾ����
 *         wGridId           [in]grid id
 *         ptVideoStreamInfo [out]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetVideoStreamInfo(u32 dwHandle, EOutputType eOutputType, u16 wGridId, TNetVideoStreamInfo *ptVideoStreamInfo);

/**
 * @brief  ��ȡ��ʾģʽ
 * @param  dwHandle		 [in]���
 *         eOutputType   [in]��ʾ����
 *         ptDisModeList [out]��ʾģʽ��Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDisMode(u32 dwHandle, EOutputType eOutputType, TNetDisModeList *ptDisModeList);

/**
 * @brief  ������ʾģʽ
 * @param  dwHandle		[in]���
 *         eOutputType  [in]��ʾ����
 *         tDisModeItem [in]��ʾģʽ��Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetDisMode(u32 dwHandle, EOutputType eOutputType, TNetDisModeItem tDisModeItem);

/**
 * @brief  ��ȡ������ȡʵʱ������rtsp url
 * @param  dwHandle		               [in]���
 *         tGetRtspRealStreamUrlParam  [in]�������
 *         ptRtspRealStreamUrl         [out]rtsp url
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRtspRealStreamUrl(u32 dwHandle, TNetGetRtspRealStreamUrlParam tGetRtspRealStreamUrlParam, TNetRtspRealStreamUrl *ptRtspRealStreamUrl);

/**
 * @brief  ʵʱ����ǿ�ƹؼ�֡
 * @param  dwHandle	 [in]���
 *         nChnId    [in]ͨ��id
 *         nVidEncId [in]��Ƶ����ͨ��id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_RealStreamForceKeyFrame(u32 dwHandle, s32 nChnId, s32 nVidEncId);

/**
 * @brief  ��ȡ����״̬
 * @param  dwHandle	      [in]���
 *         tGetDecState   [in]��ȡ����
 *         ptDecStateList [out]����״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDecState(u32 dwHandle, TNetGetDecState tGetDecState, TNetDecStateList *ptDecStateList);

/**
 * @brief  ��ȡ����ģʽ
 * @param  dwHandle  [in]���
 *         ptEthMode [out]����ģʽ��Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetEthMode(u32 dwHandle, TNetEthMode *ptEthMode);

/**
 * @brief  ��������ģʽ
 * @param  dwHandle [in]���
 *         tEthMode [in]����ģʽ��Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetEthMode(u32 dwHandle, TNetEthMode tEthMode);

/**
 * @brief  ��ȡ������Ϣ
 * @param  dwHandle  [in]���
 *         ptEthInfo [out]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetEthInfo(u32 dwHandle, TNetEthInfo *ptEthInfo);

/**
 * @brief  ����������Ϣ
 * @param  dwHandle [in]���
 *         tEthInfo [in]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetEthInfo(u32 dwHandle, TNetEthInfo tEthInfo);

/**
 * @brief  ��ȡ����������Ϣ
 * @param  dwHandle   [in]���
 *         eNetMode   [in]��������ģʽ����NETMODE_FAULTTOL��NETMODE_LOADBAL��Ч
 *         ptEthParam [out]����������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetVirEthInfo(u32 dwHandle, ENetMode eNetMode, TNetEthParam *ptEthParam);

/**
 * @brief  ��������������Ϣ
 * @param  dwHandle  [in]���
 *         eNetMode  [in]��������ģʽ����NETMODE_FAULTTOL��NETMODE_LOADBAL��Ч
 *         tEthParam [in]����������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetVirEthInfo(u32 dwHandle, ENetMode eNetMode, TNetEthParam tEthParam);

/**
 * @brief  ��ȡ����˿�
 * @param  dwHandle     [in]���
 *         ptServerPort [out]����˿�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetServerPort(u32 dwHandle, TNetServerPort *ptServerPort);

/**
 * @brief  ���÷���˿�
 * @param  dwHandle    [in]���
 *         tServerPort [in]����˿�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetServerPort(u32 dwHandle, TNetServerPort tServerPort);

/**
 * @brief  ��ȡwifiʹ��
 * @param  dwHandle    [in]���
 *         bEnable	   [out]wifi�Ƿ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetWifiEnable(u32 dwHandle, BOOL32 *pbEnable);

/**
 * @brief  ����wifiʹ��
 * @param  dwHandle    [in]���
 *         bEnable	   [in]wifi�Ƿ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetWifiEnable(u32 dwHandle, BOOL32 bEnable);

/**
 * @brief  ��ȡwifiģʽ
 * @param  dwHandle		[in]���
 *         ptWifiMode	[out]wifiģʽ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetWifiMode(u32 dwHandle, TNetWifiMode *ptWifiMode);

/**
 * @brief  ����wifiģʽ
 * @param  dwHandle		[in]���
 *         tWifiMode	[in]wifiģʽ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetWifiMode(u32 dwHandle, TNetWifiMode tWifiMode);

/**
 * @brief  ��ȡ�ƶ�����״̬
 * @param  dwHandle				[in]���
 *         nSimCardId			[in]simId����1��ʼ
 *         ptMobileNetStatus	[out]�ƶ�����״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetMobileNetworkStatus(u32 dwHandle, s32 nSimCardId, TNetMobileNetworkStatus *ptMobileNetStatus);

/**
 * @brief  ��ȡ�ಥ����
 * @param  dwHandle	      [in]���
 *         ptMulticastCfg [out]�ಥ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetMulticastCfg(u32 dwHandle, TNetMulticastCfg *ptMulticastCfg);

/**
 * @brief  ���öಥ����
 * @param  dwHandle		 [in]���
 *         tMulticastCfg [in]�ಥ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetMulticastCfg(u32 dwHandle, TNetMulticastCfg tMulticastCfg);

/**
 * @brief  ��ȡϵͳ��������
 * @param  dwHandle       [in]���
 *         ptSysBaseParam [out]ϵͳ��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSysBaseParam(u32 dwNvrIp, u16 wPort, TNetSysBaseParam *ptSysBaseParam);

/**
 * @brief  ����ϵͳ��������
 * @param  dwHandle      [in]���
 *         tSysBaseParam [in]ϵͳ��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetSysBaseParam(u32 dwHandle, TNetSysBaseParam tSysBaseParam);

/**
 * @brief  ��ȡ��ͨ���������
 * @param  dwHandle          [in]���
 *         ptZeroChnEncParam [out]��ͨ���������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSysZeroChnEncParam(u32 dwHandle, TNetZeroChnEncParam *ptZeroChnEncParam);

/**
 * @brief  ��ȡ��ͨ���������
 * @param  dwHandle         [in]���
 *         tZeroChnEncParam [in]��ͨ���������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetSysZeroChnEncParam(u32 dwHandle, TNetZeroChnEncParam tZeroChnEncParam);

/**
 * @brief  ��ȡ�û��б�
 * @param  dwHandle   [in]���
 *         ptUserList [out]�û��б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetUserList(u32 dwHandle, TNetUserList *ptUserList);

/**
 * @brief  ����û�
 * @param  dwHandle [in]���
 *         tAddUser [in]��ӵ��û�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_AddUser(u32 dwHandle, TNetAddUser tAddUser);

/**
 * @brief  ɾ���û�
 * @param  dwHandle    [in]���
 *         pszUserName [in]ɾ�����û�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DelUser(u32 dwHandle, const s8 *pszUserName);

/**
 * @brief  �༭�û�
 * @param  dwHandle  [in]���
 *         tEditUser [in]�༭���û�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_EditUser(u32 dwHandle, TNetEditUser tEditUser);

/**
 * @brief  �޸��û�����
 * @param  dwHandle       [in]���
 *         tModifyUserPwd [in]�û�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_ModifyUserPwd(u32 dwHandle, TNetModifyUserPwd tModifyUserPwd);

/**
 * @brief  ��ȡ�û�Ȩ������
 * @param  dwHandle          [in]���
 *         ptUserPermCapInfo [out]��ȡ����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetUserPermCapInfo(u32 dwHandle, TNetUserPermCapInfo *ptUserPermCapInfo);

/**
 * @brief  ��ȡ�û�Ȩ��
 * @param  dwHandle       [in]���
 *         ptUserPermInfo [out]��ȡ����Ϣ,��Ҫ�����û���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetUserPermInfo(u32 dwHandle, TNetUserPermInfo *ptUserPermInfo);

/**
 * @brief  �����û�Ȩ��
 * @param  dwHandle       [in]���
 *         tUserPermInfo  [in]Ҫ���õ���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetUserPermInfo(u32 dwHandle, TNetUserPermInfo tUserPermInfo);

/**
 * @brief  ��ȡrtsp��֤��ʽ
 * @param  dwHandle       [in]���
 *         ptRtspAuthInfo [out]��ȡ����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRtspAuthInfo(u32 dwHandle, TNetRtspAuthInfo *ptRtspAuthInfo);

/**
 * @brief  ����rtsp��֤��ʽ
 * @param  dwHandle       [in]���
 *         tRtspAuthInfo  [in]Ҫ���õ���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetRtspAuthInfo(u32 dwHandle, TNetRtspAuthInfo tRtspAuthInfo);

/**
 * @brief  ��ȡ��ȫ������Ϣ
 * @param  dwHandle              [in]���
 *         ptSecurityServiceInfo [out]��ȡ����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSecurityServiceInfo(u32 dwHandle, TNetSecurityServiceInfo *ptSecurityServiceInfo);

/**
 * @brief  ���ð�ȫ������Ϣ
 * @param  dwHandle              [in]���
 *         tSecurityServiceInfo  [in]Ҫ���õ���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetSecurityServiceInfo(u32 dwHandle, TNetSecurityServiceInfo tSecurityServiceInfo);

/**
 * @brief  ��ȡip��ַ��������
 * @param  dwHandle              [in]���
 *         ptIpAddrFilterCfg     [out]��ȡ����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIpAddrFilterCfg(u32 dwHandle, TNetIpAddrFilterCfg *ptIpAddrFilterCfg);

/**
 * @brief  ����ip��ַ��������
 * @param  dwHandle              [in]���
 *         tIpAddrFilterCfg      [out]��ȡ����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIpAddrFilterCfg(u32 dwHandle, TNetIpAddrFilterCfg tIpAddrFilterCfg);

/**
 * @brief  ��ȡVGA��ʾ�ӿڲ���
 * @param  dwNvrIp               [in]�豸ip
 *         wPort                 [in]�豸�˿ں�
 *         ptDisplayPortVgaList  [out]��ȡ����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDisplayPortVgaParam(u32 dwNvrIp, u16 wPort, TNetDisplayPortVgaList *ptDisplayPortVgaList);

/**
 * @brief  ����VGA��ʾ�ӿڲ���
 * @param  dwHandle              [in]���
 *         tDisplayPortVgaList   [in]Ҫ���õ���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetDisplayPortVgaParam(u32 dwHandle, TNetDisplayPortVgaList tDisplayPortVgaList);

/**
 * @brief  ��ȡHDMI��ʾ�ӿڲ���
 * @param  dwNvrIp               [in]�豸ip
 *         wPort                 [in]�豸�˿ں�
 *         ptDisplayPortHdmiList [out]��ȡ����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDisplayPortHdmiParam(u32 dwNvrIp, u16 wPort, TNetDisplayPortHdmiList *ptDisplayPortHdmiList);

/**
 * @brief  ����HDMI��ʾ�ӿڲ���
 * @param  dwHandle             [in]���
 *         tDisplayPortHdmiList [in]Ҫ���õ���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetDisplayPortHdmiParam(u32 dwHandle, TNetDisplayPortHdmiList tDisplayPortHdmiList);

/**
 * @brief  ��ȡCVBS��ʾ�ӿڲ���
 * @param  dwNvrIp               [in]�豸ip
 *         wPort                 [in]�豸�˿ں�
 *         ptDisplayPortCvbsList [out]��ȡ����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDisplayPortCvbsParam(u32 dwNvrIp, u16 wPort, TNetDisplayPortCvbsList *ptDisplayPortCvbsList);

/**
 * @brief  ����CVBS��ʾ�ӿڲ���
 * @param  dwHandle             [in]���
 *         tDisplayPortCvbsList [in]Ҫ���õ���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetDisplayPortCvbsParam(u32 dwHandle, TNetDisplayPortCvbsList tDisplayPortCvbsList);

/**
 * @brief  ��ȡ�������
 * @param  dwNvrIp      [in]�豸ip
 *         wPort        [in]�豸�˿ں�
 *         peOutputType [out]��ȡ���������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetOutputType(u32 dwNvrIp, u16 wPort, EOutputType *peOutputType);

/**
 * @brief  �����������
 * @param  dwHandle    [in]���
 *         eOutputType [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetOutputType(u32 dwHandle, BOOL32 bValid, EOutputType eOutputType);

/**
 * @brief  ��ȡ����������Ϣ
 * @param  dwHandle            [in] ���
 *         nSerialPortIndex    [in] ��������
 *         ptSerialPortInfo    [out]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSerialPortInfo(u32 dwHandle, s32 nSerialPortIndex, TNetSerialPortInfo *ptSerialPortInfo);

/**
 * @brief  ���ô���������Ϣ
 * @param  dwHandle            [in] ���
 *         nSerialPortIndex    [in] ��������
 *         tSerialPortInfo    [in]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetSerialPortInfo(u32 dwHandle, s32 nSerialPortIndex, TNetSerialPortInfo tSerialPortInfo);

/**
 * @brief  ��ȡktsmģ���Ƿ񼤻�
 * @param  dwHandle					[in] ���
 *         ptKtsmActiveStatus		[out]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetKtsmActiveStatus(u32 dwHandle, TNetKtsmActiveStatus *ptKtsmActiveStatus);

/**
 * @brief  ����ktsmģ��
 * @param  dwHandle			[in]���
 *         pszPinCode		[in]PIN��
 *         ptActiveKtsmModule [out]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_ActiveKtsmModule(u32 dwHandle, s8 *pszPinCode, TNetActiveKtsmModule *ptActiveKtsmModule);

/**
 * @brief  ���ktsmģ��״̬
 * @param  dwHandle			[in] ���
 *         ptKtsmStatus		[out]ktsmģ��״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetKtsmStatus(u32 dwHandle, TNetKtsmStatus *ptKtsmStatus);

/**
 * @brief  ��ȡktsm�ܹܷ���ip�Ͷ˿�
 * @param  dwHandle				[in] ���
 *         ptKtsmMgrSrvInfo		[out]ktsm�ܹܷ�����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetKtsmPwdMgrSrvInfo(u32 dwHandle, TNetKtsmMgrSrvInfo *ptKtsmMgrSrvInfo);

/**
 * @brief  ����ktsm�ܹܷ���ip�Ͷ˿�
 * @param  dwHandle				[in] ���
 *         tKtsmMgrSrvInfo		[in]ktsm�ܹܷ�����Ϣ
 *		   ptActiveKtsmModule	[out]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetKtsmPwdMgrSrvInfo(u32 dwHandle, TNetKtsmMgrSrvInfo tKtsmMgrSrvInfo, TNetActiveKtsmModule *ptActiveKtsmModule);

/**
 * @brief  ����ktsm�������к�
 * @param  dwHandle				[in] ���

 *		   ptActiveKtsmModule	[out]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_ResetKtsmNetLicense(u32 dwHandle, TNetActiveKtsmModule *ptActiveKtsmModule);

/**
 * @brief  ��ȡģ���û�������
 * @param  dwHandle				[in] ���
 *         ptKtsmUserInfo		[out]�û�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetKtsmUserInfo(u32 dwHandle, TNetKtsmUserInfo *ptKtsmUserInfo);

/**
 * @brief  ����ģ���û�������
 * @param  dwHandle				[in] ���
 *         tKtsmMgrSrvInfo		[in]�û�������
 *		   ptActiveKtsmModule	[out]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetKtsmUserInfo(u32 dwHandle, TNetKtsmUserInfo tKtsmUserInfo, TNetActiveKtsmModule *ptActiveKtsmModule);

/**
 * @brief  ��ȡ�豸��Ϣ
 * @param  dwHandle                [in]���
 *         ptNvrDevInfo            [out]�豸��Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNvrDevInfo(u32 dwHandle, TNetNvrDevInfo *ptNvrDevInfo);

/**
 * @brief  ��ȡͨ��¼��״̬
 * @param  dwHandle               [in]���
 *         nStartChnId            [in]��ʼͨ��ID
 *         nEndChnId              [in]����ͨ��ID
 *         ptChnRecStateList      [out]ͨ��¼��״̬�б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnRecState(u32 dwHandle, s32 nStartChnId, s32 nEndChnId, TNetChnRecStateList *ptChnRecStateList);

/**
 * @brief  ��ȡ��־����
 * @param  dwHandle                [in]���
 *         pdwLangCap              [out]��־��������,ָ��Ϊ�ղ���ȡ
 *         ptLogMainTypeItem       [out]��ȡ����Ϣ
 *         ptLogMainTypeItem       [in/out]���������������ʵ������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetLogCap(u32 dwHandle, u32 *pdwLangCap, TNetLogMainTypeItem *patLogMainTypeItem, s32 *pLen);

/**
 * @brief  ����������־����
 * @param  dwHandle                       [in]���
 *         tCreateSearchLogTaskParam      [in]����task�Ĳ���
 *         ptSearchLogTaskInfo            [out]��ȡ����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreateSearchLogTask(u32 dwHandle, TNetCreateSearchLogTaskParam tCreateSearchLogTaskParam, TNetSearchLogTaskInfo* ptSearchLogTaskInfo);

/**
 * @brief  ����������־����
 * @param  dwHandle                       [in]���
 *         dwTaskId                       [in]����id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroySearchLogTask(u32 dwHandle, u32 dwTaskId);

/**
 * @brief  ��ȡ������־���
 * @param  dwHandle                       [in]���
 *         dwTaskId                       [in]����id
 *         ptSearchLogItem                [out]��ȡ����Ϣ
 *         pNums                          [in/out]���������������ʵ������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSearchLogResult(u32 dwHandle, u32 dwTaskId, u32 dwStart, TNetSearchLogItem *patSearchLogItem, u32 *pNums);

/**
 * @brief  ����������־���
 * @param  dwHandle                       [in]���
 *         dwTaskId                       [in]����id
 *         pszPath                        [in]����·��
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_ExportSearchLogResult(u32 dwHandle, u32 dwTaskId, s8 *pszPath);

/**
 * @brief  д������־
 * @param  dwHandle                       [in]���
 *         pszLogType                     [in]��־����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_WriteOperationLog(u32 dwHandle, s8 *pszLogType);

/**
 * @brief  ��ȡ����״̬��Ϣ
 * @param  dwHandle               [in]���
 		   eHealthLogTimeType     [in]ʱ�������
		   ptHealthLogList        [out]����״̬��Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetHealthState(u32 dwHandle, EHealthLogTimeType eHealthLogTimeType, TNetHealthLogList *ptHealthLogList);

/**
 * @brief  ��ȡ����״̬����
 * @param  dwHandle               [in]���
 		   eHealthLogTimeType     [in]ʱ�������
		   ptHealthLogList        [out]����״̬��Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetHealthStateDetail(u32 dwHandle, TNetHealthDetailParam tHealthDetailParam, TNetHealthDetailList *ptHealthDetailList);

/**
 * @brief  ��ȡʵʱ��־
 * @param  dwHandle      [in]���
 *         tGetRealLog   [in]��ȡʵʱ��־����
 *         ptRealLogList [out]��ȡ��ʵʱ��־�б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRealLog(u32 dwHandle, TNetGetRealLog tGetRealLog, TNetRealLogList *ptRealLogList);

/**
 * @brief  ��ȡʵʱ��־����
 * @param  dwHandle          [in]���
 *         tGetRealLogDetail [in]��ȡʵʱ��־�������
 *         ptRealLogDetail   [out]ʵʱ��־����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRealLogDetail(u32 dwHandle, TNetGetRealLogDetail tGetRealLogDetail, TNetRealLogDetail *ptRealLogDetail);

/**
 * @brief  ��ȡʵʱ״̬
 * @param  dwHandle        [in]���
 *         ptRealStateList [out]��ȡ��ʵʱ״̬�б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRealState(u32 dwHandle, TNetRealStateList *ptRealStateList);

/**
 * @brief  ������ȡϵͳʱ�����
 * @param  dwHandle                       [in]���
 *         tSystemTimeInfo			      [in]����system time�Ĳ���
 *         ptSystemTimeParam              [out]��ȡ����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSystemTimeParam(u32 dwHandle, TNetSystemTimeInfo tSystemTimeInfo, TNetSystemTimeParam* ptSystemTimeParam);

/**
 * @brief  ��������ϵͳʱ�����
 * @param  dwHandle                       [in]���
 *         tSystemTimeParam			      [in]���õĲ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetSystemTimeParam(u32 dwHandle, TNetSystemTimeParam tSystemTimeParam);

/**
 * @brief  ������ȡϵͳʱ�����
 * @param  dwHandle                       [in]���
 *         ptSystemTimeParam              [out]��ȡ����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSystemTimeParamEx(u32 dwHandle, TNetSystemTimeParamEx* ptSystemTimeParam);

/**
 * @brief  ��������ϵͳʱ�����
 * @param  dwHandle                       [in]���
 *         tSystemTimeParam			      [in]���õĲ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetSystemTimeParamEx(u32 dwHandle, TNetSystemTimeParamEx tSystemTimeParam);

/**
 * @brief  �������������Ӳ��ID
 * @param  dwHandle                       [in]���
 *         tGroupDiskList			      [in]��������Ӳ��ID�Ĳ���
 *         ptSetGroupDiskErrInfo          [out]��������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetGroupDiskList(u32 dwHandle, TNetGroupDiskList tGroupDiskList, TNetSetGroupDiskErrInfo *ptSetGroupDiskErrInfo);

/**
 * @brief  ��ѯӲ�̳�ʼ������
 * @param  dwHandle                       [in]���
 *         dwTaskId					      [in]task id
 *  	   pwProgress					  [out]������
 *  	   peDiskInitState                [out]״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_QueryDiskInitProgress(u32 dwHandle, u16 dwTaskId, s32* pwProgress, EDiskInitState *peDiskInitState);

/**
 * @brief  ��ȡ��չ�����б�
 * @param  dwHandle                       [in]���
 *         ptExDiskList         		  [out]�ⲿ�����̲���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetExDiskList(u32 dwHandle, TNetExDiskList *ptExDiskList);

/**
 * @brief  ���õ���
 * @param  dwHandle                       [in]���
 *         pszFolderPath        		  [in]Ҫ���������ļ���·��
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_ExportCfg(u32 dwHandle, s8 *pszFolderPath);

/**
 * @brief  ���õ���
 * @param  dwHandle                       [in]���
 *         pszFilePath           		  [in]Ҫ������ļ�ȫ·���������ļ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_ImportCfg(u32 dwHandle, s8 *pszFilePath);

/**
 * @brief  ����׼��
 * @param  dwHandle                       [in]���
 *         TNetUpgradePrepareParam		  [in]����׼������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_UpgradePrepare(u32 dwHandle, TNetUpgradePrepareParam tUpgradePrepareParam);

/**
 * @brief  ��ѯ����״̬
 * @param  dwHandle                       [in]���
 *  	   state						  [out]״̬ Idel(���У���ǰ�豸û������)|Upgrade(������)|UpgradeSuccess(�����ɹ�)|UpgradeFailure(����ʧ��)
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_QueryUpgradeState(u32 dwHandle, s8* pState);

/**
 * @brief  �ָ�����״̬
 * @param  dwHandle          [in]���
 *  	   tSystemFactoryDef [in]�ָ���������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetSystemFactoryDef(u32 dwHandle, TNetSystemFactoryDef tSystemFactoryDef);

/**
 * @brief  ��ȡ�ָ�������Ϣ
 * @param  dwHandle             [in]���
 *  	   ptRecoverFactoryInfo	[out]�ָ�������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRecoverFactoryInfo(u32 dwHandle, TNetRecoverFactoryInfo *ptRecoverFactoryInfo);

/**
 * @brief  ��ȡͨ������
 * @param  dwHandle     [in]���
 *  	   tGetChnCap	[in]�������
 *  	   ptChnCapList [out]ͨ������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnCap(u32 dwHandle, TNetGetChnCap tGetChnCap, TNetChnCapList *ptChnCapList);

/**
 * @brief  ��ȡͨ�������������
 * @param  dwHandle           [in]���
 *  	   nChnId	          [in]ͨ��id
 *  	   ptChnInteDetectCap [out]ͨ�������������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnInteDetectCap(u32 dwHandle, s32 nChnId, TNetChnInteDetectCap *ptChnInteDetectCap);

/**
 * @brief  ��ȡNVR�澯��������
 * @param  dwHandle         [in]���
 *  	   pszAlarmInId   	[in]�澯����ţ�����澯�����ʹ�ö��ŷָ�
 *		   ptAlarmInCfgList [out]�澯��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNvrAlarmInCfg(u32 dwHandle, const s8 *pszAlarmInId, TNetAlarmInCfgList *ptAlarmInCfgList);

/**
 * @brief  ����NVR�澯��������
 * @param  dwHandle        [in]���
 *		   tAlarmInCfgList [in]�澯��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetNvrAlarmInCfg(u32 dwHandle, TNetAlarmInCfgList tAlarmInCfgList);

/**
 * @brief  ��ȡͨ���澯��������
 * @param  dwHandle         [in]���
 *         nChnId           [in]ͨ��id
 *  	   pszAlarmInId 	[in]�澯����ţ�����澯�����ʹ�ö��ŷָ�
 *		   ptAlarmInCfgList [out]�澯��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnAlarmInCfg(u32 dwHandle, s32 nChnId, const s8 *pszAlarmInId, TNetAlarmInCfgList *ptAlarmInCfgList);

/**
 * @brief  ����ͨ���澯��������
 * @param  dwHandle        [in]���
 *         nChnId          [in]ͨ��id
 *		   tAlarmInCfgList [in]�澯��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetChnAlarmInCfg(u32 dwHandle, s32 nChnId, TNetAlarmInCfgList tAlarmInCfgList);

/**
 * @brief  ��ȡ�澯����ʱ��
 * @param  dwHandle         [in]���
 *  	   tAlarmSource		[in]�澯Դ
 *  	   ptAlarmGuardTime [out]�澯����ʱ��
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAlarmGuardTime(u32 dwHandle, TNetAlarmSource tAlarmSource, TNetAlarmGuardTime *ptAlarmGuardTime);

/**
 * @brief  ���ø澯����ʱ��
 * @param  dwHandle        [in]���
 *  	   tAlarmGuardTime [in]�澯����ʱ��
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetAlarmGuardTime(u32 dwHandle, TNetAlarmGuardTime tAlarmGuardTime);

/**
 * @brief  ��ȡϵͳ�澯����
 * @param  dwHandle      [in]���
 *         ptSysAlarmCfg [out]ϵͳ�澯����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSysAlarmCfg(u32 dwHandle, TNetSysAlarmCfg *ptSysAlarmCfg);

/**
 * @brief  ����ϵͳ�澯����
 * @param  dwHandle     [in]���
 *  	   tSysAlarmCfg [in]ϵͳ�澯����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetSysAlarmCfg(u32 dwHandle, TNetSysAlarmCfg tSysAlarmCfg);

/**
 * @brief  �澯���븴��
 * @param  dwHandle          [in]���
 *  	   tAlarmInCopyParam [in]���Ʋ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_AlarmInCopy(u32 dwHandle, TNetAlarmInCopyParam tAlarmInCopyParam);

/**
 * @brief  ��ȡ�澯��������
 * @param  dwHandle       [in]���
 *  	   ptAlarmLinkCfg [in/out]�澯��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAlarmLinkCfg(u32 dwHandle, TNetAlarmLinkCfg *ptAlarmLinkCfg);

/**
 * @brief  ���ø澯��������
 * @param  dwHandle      [in]���
 *  	   tAlarmLinkCfg [in]�澯��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetAlarmLinkCfg(u32 dwHandle, TNetAlarmLinkCfg tAlarmLinkCfg);

/**
 * @brief  ��ȡ�澯��������
 * @param  dwHandle       [in]���
 *  	   ptAlarmLinkCap [out]�澯��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAlarmLinkCap(u32 dwHandle, TNetAlarmLinkCap *ptAlarmLinkCap);

/**    
 * @brief  gbkתutf8
 * @param  pGbk							[in]Gbk�ַ���
 *  	   pUtf8						[out]Utf8�ַ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GBK2UTF8(const s8 *pGbk, s8 *pUtf8);

/**
 * @brief  utf8תgbk
 * @param  pUtf8						[in]Utg8�ַ���
 *  	   pGbk							[out]Gbk�ַ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_UTF82GBK(const s8 *pUtf8, s8 *pGbk);

/**
 * @brief  ����ͨ���б�
 * @param  dwHandle            [in]���
 *         pDevList			   [out]ͨ�������б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_ExportDevList(u32 dwHandle, s8* pDevList);

/**
 * @brief  ����ͨ���б�
 * @param  dwHandle            [in]���
 *         pDevList			   [in]ͨ�������б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_ImportDevList(u32 dwHandle, s8* pDevList, s32 *pnErrLine);

/**
 * @brief  ��ȡsmtp����
 * @param  dwHandle     [in]���
 *         ptSmtpCfgAll	[out]smtp����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSmtpCfg(u32 dwHandle, TNetSmtpCfgAll *ptSmtpCfgAll);

/**
 * @brief  ����smtp����
 * @param  dwHandle [in]���
 *         tSmtpCfg [in]smtp����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetSmtpCfg(u32 dwHandle, TNetSmtpCfg tSmtpCfg);

/**
 * @brief  ��ʼ�������
 * @param  dwHandle            [in]���
 *         tSmtpCfg            [in]smtp������Ϣ
 *         pnTaskId            [out]�����������id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StartSmtpTest(u32 dwHandle, TNetSmtpCfg tSmtpCfg, s32 *pnTaskId);

/**
 * @brief  ��ȡ�������״̬
 * @param  dwHandle            [in]���
 *         nTaskId             [in]�����������id
 *         peSmtpTestStatus    [out]�����������״̬
 *         pnResult            [out]�������������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSmtpTestStatus(u32 dwHandle, s32 nTaskId, ESmtpTestStatus *peSmtpTestStatus, s32 *pnResult);

/**
 * @brief  ֹͣ�������
 * @param  dwHandle            [in]���
 *         nTaskId             [in]�����������id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StopSmtpTest(u32 dwHandle, s32 nTaskId);

/**
 * @brief  ��ȡPPPoE����
 * @param  dwHandle   [in]���
 *         ptPppoeCfg [out]PPPoE����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetPppoeCfg(u32 dwHandle, TNetPppoeCfg *ptPppoeCfg);

/**
 * @brief  ����PPPoE����
 * @param  dwHandle  [in]���
 *         tPppoeCfg [out]PPPoE����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetPppoeCfg(u32 dwHandle, TNetPppoeCfg tPppoeCfg);

/**
 * @brief  ��ȡPPPoE״̬
 * @param  dwHandle      [in]���
 *         ptPppoeStatus [out]PPPoE״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetPppoeStatus(u32 dwHandle, TNetPppoeStatus *ptPppoeStatus);

/**
 * @brief  ��ȡUPnP����
 * @param  dwHandle  [in]���
 *         ptUpnpCfg [out]UPnP����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetUpnpCfg(u32 dwHandle, TNetUpnpCfg *ptUpnpCfg);

/**
 * @brief  ����UPnP����
 * @param  dwHandle [in]���
 *         tUpnpCfg	[in]UPnP����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetUpnpCfg(u32 dwHandle, TNetUpnpCfg tUpnpCfg);

/**
 * @brief  ��ȡDDNS����
 * @param  dwHandle  [in]���
 *         ptDDNSCfg [out]DDNS����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDDNSCfg(u32 dwHandle, TNetDDNSCfg *ptDDNSCfg);

/**
 * @brief  ����DDNS����
 * @param  dwHandle [in]���
 *         tDDNSCfg	[in]DDNS����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetDDNSCfg(u32 dwHandle, TNetDDNSCfg tDDNSCfg);

/**
 * @brief  ��ȡDDNS����״̬
 * @param  dwHandle		[in]���
 *         ptDDNSState	[out]����״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDDNSState(u32 dwHandle, TNetDDNSState *ptDDNSState);

/**
 * @brief  ��ȡSNMP����
 * @param  dwHandle		[in]���
 *         ptNetSnmpCfg [out]Snmp����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSnmpCfg(u32 dwHandle, TNetSnmpCfg *ptNetSnmpCfg);

/**
 * @brief  ��ȡGPS��Ϣ
 * @param  dwHandle  [in]���
 *         ptGpsInfo [out]GPS��Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetGpsInfo(u32 dwHandle, TNetGpsInfo *ptGpsInfo);

/**
 * @brief  ����SNMP����
 * @param  dwHandle		[in]���
 *         tNetSnmpCfg  [in]Snmp����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetSnmpCfg(u32 dwHandle, TNetSnmpCfg tNetSnmpCfg);

/**
 * @brief  ����
 * @param  dwHandle        [in]���
 *         tAlarmStateList [in]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_AlarmRemove(u32 dwHandle, TNetAlarmStateList tAlarmStateList);

/**
 * @brief  ��ʼץ��
 * @param  dwHandle            [in]���
 *         TNetCaptureStart	   [in]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StartCapturePack(u32 dwHandle, TNetCaptureStart tCapureStart);

/**
 * @brief  ֹͣץ��
 * @param  dwHandle            [in]���
 *         TNetCaptureStop	   [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StopCapturePack(u32 dwHandle, TNetCaptureStop *ptCaptureStop);

/**
 * @brief  ��ѯץ��״̬
 * @param  dwHandle            [in]���
 *         ptCaptureState 	   [out]ץ��״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCaptureState(u32 dwHandle, TNetCaptureState *ptCaptureState);

/**
 * @brief  ��ʼPing
 * @param  dwHandle            [in]���
 *         TNetPingStart	   [in]ping����
 *         TaskID			   [out]ptTaskID
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StartPing(u32 dwHandle, TNetPingStart tPingStart, s32 *ptTaskID);

/**
 * @brief  ��ȡPing���
 * @param  dwHandle            [in]���
 *         nTaskID			   [in]ID
 *         TNetPingStart	   [out]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetPingResult(u32 dwHandle, s32 nTaskID, TNetPingResultList *ptPingResultList);

/**
 * @brief  ֹͣPing
 * @param  dwHandle            [in]���
 *   	   TaskID			   [in]ID
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StopPing(u32 dwHandle, s32 nTaskID);

/**
 * @brief  ��ȡvsipЭ����������
 * @param  dwHandle            [in]���
 *         TNetBandParam	   [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNetVsip(u32 dwHandle, TNetVsipParam *ptNetVsipParam);

/**
 * @brief  ����vsipЭ����������
 * @param  dwHandle            [in]���
 *         TNetBandParam	   [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetNetVsip(u32 dwHandle, TNetVsipParam ptNetVsipParam);

/**
 * @brief  ��ȡvsipЭ��ע��ƽ̨״̬
 * @param  dwHandle    [in]���
 *         pbRegStatus [out]ע��״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetVsipRegStatus(u32 dwHandle, BOOL32 *pbRegStatus);

/**
 * @brief  ��ȡgb28181Э������
 * @param  dwHandle           [in]���
 *	       TNetGBInfo		  [in]�������
 *	       TNetGBParam		  [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNetGB28181(u32 dwHandle, TNetGB28181Info tNetGBInfo, TNetGB28181Param *ptNetGBParam);

/**
 * @brief  ����gb28181Э������
 * @param  dwHandle           [in]���
 *	       nPlatIndex		  [in]ƽ̨����
 *	       TNetGBParam		  [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetNetGB28181(u32 dwHandle, s32 nPlatIndex, TNetGB28181Param tNetGBParam);

/**
 * @brief  ��ȡgb28181Э��ע��ƽ̨״̬
 * @param  dwHandle    [in]���
 *	       nPlatIndex  [in]ƽ̨����
 *	       pbRegStatus [out]ע��״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetGB28181RegStatus(u32 dwHandle, s32 nPlatIndex, BOOL32 *pbRegStatus);

/**
 * @brief  ��ȡOnvifЭ������
 * @param  dwHandle           [in]���
 *	       ptOnvifParam	  [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNetOnvif(u32 dwHandle, TNetOnvifParam *ptOnvifParam);

/**
 * @brief  ����OnvifЭ������
 * @param  dwHandle           [in]���
 *	       tOnvifParam	  [in]����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetNetOnvif(u32 dwHandle, TNetOnvifParam tOnvifParam);

/**
 * @brief  ��ȡ��ͼ��Э��ע��ƽ̨״̬
 * @param  dwHandle    [in]���
 *	       nPlatIndex  [in]ƽ̨����
 *	       pbRegStatus [out]ע��״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetPubSecRegStatus(u32 dwHandle, s32 nPlatIndex, BOOL32 *pbRegStatus);

/**
 * @brief  ��ȡ��ͼ��Э����������
 * @param  dwHandle           [in]���
 *	       nPlatIndex		  [in]��ͼ��ƽ̨����
 *	       ptPubSecAppCfg	  [out]��ͼ��Э����������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetPubSecCfg(u32 dwHandle, s32 nPlatIndex, TNetPubSecCfg *ptPubSecAppCfg);

/**
 * @brief  ������ͼ��Э����������
 * @param  dwHandle           [in]���
 *	       nPlatIndex		  [in]��ͼ��ƽ̨����
 *	       tPubSecAppCfg	  [in]��ͼ��Э����������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetPubSecCfg(u32 dwHandle, s32 nPlatIndex, TNetPubSecCfg tPubSecAppCfg);

/**
 * @brief  ��ȡ�Ʒ�������
 * @param  dwHandle   [in]���
 *	       ptCloudCfg [out]�Ʒ�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCloudCfg(u32 dwHandle, TNetCloudCfg *ptCloudCfg);

/**
 * @brief  �����Ʒ�������
 * @param  dwHandle [in]���
 *	       bEnable  [in]�Ƿ������Ʒ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetCloudCfg(u32 dwHandle, BOOL32 bEnable);

/**
 * @brief  ��ȡ�Ʒ���״̬
 * @param  dwHandle      [in]���
 *	       peCloudStatus [out]�Ʒ���״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCloudStatus(u32 dwHandle, ECloudStatus *peCloudStatus);

/**
 * @brief  ��ȡ��������������Ϣ
 * @param  dwHandle        [in]���
 *	       pGB28181CtlInfo [out]��������������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetGB28181CtlInfo(u32 dwHandle, TNetGB28181CtlInfo *ptGB28181CtlInfo);

/**
 * @brief  ���ù�������������Ϣ
 * @param  dwHandle        [in]���
 *	       tGB28181CtlInfo [out]��������������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetGB28181CtlInfo(u32 dwHandle, TNetGB28181CtlInfo tGB28181CtlInfo);

/**
 * @brief  ��ȡ��ͼ����������
 * @param  dwHandle       [in]���
 *	       ptPubSecCltCfg [out]��ͼ������������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetPubSecCltCfg(u32 dwHandle, TNetPubSecCltCfg *ptPubSecCltCfg);

/**
 * @brief  ������ͼ����������
 * @param  dwHandle      [in]���
 *	       tPubSecCltCfg [in]��ͼ������������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetPubSecCltCfg(u32 dwHandle, TNetPubSecCltCfg tPubSecCltCfg);

/**
 * @brief  ��ȡͨ���澯����(�ƶ����)
 * @param  dwHandle          [in]���
 *		   tGetChnAlarmCfg   [in]��ȡ����
 *		   ptChnAlarmCfgList [out]ͨ���澯����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnAlarmCfg(u32 dwHandle, TNetGetChnAlarmCfg tGetChnAlarmCfg, TNetChnAlarmCfgList *ptChnAlarmCfgList);

/**
 * @brief  ����ͨ���澯����(�ƶ����)
 * @param  dwHandle     [in]���
 *		   tChnAlarmCfg [in]ͨ���澯����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetChnAlarmCfg(u32 dwHandle, TNetChnAlarmCfg tChnAlarmCfg);

/**
 * @brief  ��ȡͨ����ƵԴ��ʧ�澯����
 * @param  dwHandle          [in]���
 *		   nChnId            [in]ͨ��
 *		   pbEnable          [out]�Ƿ�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnVideoLostAlarmCfg(u32 dwHandle, s32 nChnId, BOOL32 *pbEnable);

/**
 * @brief  ����ͨ����ƵԴ��ʧ�澯����
 * @param  dwHandle          [in]���
 *		   nChnId            [in]ͨ��
 *		   pbEnable          [in]�Ƿ�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetChnVideoLostAlarmCfg(u32 dwHandle, s32 nChnId, BOOL32 bEnable);

/**
 * @brief  ��ȡ�������-�ڵ��������
 * @param  dwHandle          [in]���
 *		   ptNetChnAlarmCfg	 [in/out]�ڵ��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnShiledDetectCfg(u32 dwHandle, TNetChnAlarmCfg *ptNetChnAlarmCfg);

/**
 * @brief  �����������-�ڵ��������
 * @param  dwHandle     [in]���
 *		   tChnAlarmCfg [in]ͨ���澯����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetChnShiledDetectCfg(u32 dwHandle, TNetChnAlarmCfg tChnAlarmCfg);

/**
 * @brief  ��ȡNVR�澯״̬
 * @param  dwHandle         [in]���
 *         ptAlarmStateList	[out]�澯״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNvrAlarmState(u32 dwHandle, TNetAlarmStateList *ptAlarmStateList);

/**
 * @brief  ��ȡͨ���澯״̬
 * @param  dwHandle          [in]���
 *         tGetChnAlarmState [in]�������
 *         ptAlarmStateList	 [out]�澯״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnAlarmState(u32 dwHandle, TNetGetChnAlarmState tGetChnAlarmState, TNetAlarmStateList *ptAlarmStateList);

/**
 * @brief  Ӳ��SMART���
 * @param  dwHandle               [in]���
 *         nDiskId                [in]Ӳ��ID
 *         ptDiskSmartTestInfo	  [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DiskSmartTest(u32 dwHandle, s32 nDiskId, TNetDiskSmartTestInfo *ptDiskSmartTestInfo);

/**
 * @brief  ��ȡӲ�̽ṹ��Ϣ
 * @param  dwHandle               [in]���
 *         nDiskId                [in]Ӳ��ID
 *         ptDiskGeometryInfo	  [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDiskGeometryInfo(u32 dwHandle, s32 nDiskId, TNetDiskGeometryInfo *ptDiskGeometryInfo);

/**
 * @brief  ��ȡ�������������Ϣ
 * @param  dwHandle                   [in]���
 *         ptBadSectorCheckTaskInfo	  [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetBadSectorCheckTaskInfo(u32 dwHandle, TNetBadSectorCheckTaskInfo *ptBadSectorCheckTaskInfo);

/**
 * @brief  �����������������Ϣ
 * @param  dwHandle               [in]���
 *         nDiskId                [in]Ӳ��ID
 *         eBadSectorCheckType    [in]�����������
 *         pTaskId          	  [out]����ID
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreateBadSectorCheckTask(u32 dwHandle, s32 nDiskId, EBadSectorCheckType eBadSectorCheckType, s32 *pnTaskId);

/**
 * @brief  ��������������
 * @param  dwHandle                   [in]���
 *         nTaskId                    [in]����ID
 *         eBadSectorCheckCtrlType    [in]������������������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_BadSectorCheckTaskCtrl(u32 dwHandle, s32 nTaskId, EBadSectorCheckCtrlType eBadSectorCheckCtrlType);

/**
 * @brief  ֹͣ�����������
 * @param  dwHandle               [in]���
 *         nTaskId                [in]����ID
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StopBadSectorCheckTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  ��ȡ���������Ϣ
 * @param  dwHandle               [in]���
 *         nTaskId                [in]����ID
 *         ullStartPos            [in]��ʼλ��
 *         ptBadSectorCheckInfo   [out]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetBadSectorCheckResult(u32 dwHandle, s32 nTaskId, u64 ullStartPos, TNetBadSectorCheckInfo *ptBadSectorCheckInfo);

/**
 * @brief  ��ȡ�������
 * @param  dwHandle               [in]���
 *         TNetBandParam		  [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNetBand(u32 dwHandle, TNetBandParam *ptNetBandParam);

/**
 * @brief  ��ȡ�Զ�ά������
 * @param  dwHandle         [in]���
 *         ptAutoServiceCfg [out]�Զ�ά������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAutoServiceCfg(u32 dwHandle, TNetAutoServiceCfg *ptAutoServiceCfg);

/**
 * @brief  �����Զ�ά������
 * @param  dwHandle        [in]���
 *         tAutoServiceCfg [in]�Զ�ά������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetAutoServiceCfg(u32 dwHandle, TNetAutoServiceCfg tAutoServiceCfg);

/**
 * @brief  ��ȡ�߼�����
 * @param  dwHandle           [in]���
 *	       TNetAdvancedInfo	  [in]�������
 *	       TNetAdvancedParam  [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNetAdvancedCfg(u32 dwHandle, TNetNetAdvancedParam *ptAdvancedParam);

/**
 * @brief  ���ø߼�����
 * @param  dwHandle           [in]���
 *	       TNetAdvancedParam  [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetNetAdvancedCfg(u32 dwHandle, TNetNetAdvancedParam tAdvancedParam);

/**
 * @brief  ��ȡϵͳ�߼�����
 * @param  dwHandle         [in]���
 *	       ptSysAdvancedCfg [out]ϵͳ�߼�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSysAdvancedCfg(u32 dwHandle, TNetSysAdvancedCfg *ptSysAdvancedCfg);

/**
 * @brief  ����ϵͳ�߼�����
 * @param  dwHandle        [in]���
 *	       tSysAdvancedCfg [in]ϵͳ�߼�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetSysAdvancedCfg(u32 dwHandle, TNetSysAdvancedCfg tSysAdvancedCfg);

/**
 * @brief  ��ȡNVR֪ͨ�¼�
 * @param  dwHandle      [in]���
 *	       ptNvrNtyEvent [out]NVR֪ͨ�¼���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNvrNtyEvent(u32 dwHandle, TNetNvrNtyEvent *ptNvrNtyEvent);

/**
 * @brief  ͸��ͨ��
 * @param  dwHandle           [in]���
 *	       pInBuf             [in]���뻺����
 *	       nInBufLen          [in]���뻺������С
 *	       pOutBuf            [out]���������
 *	       pnOutLen           [in/out]���������������С������ʵ�ʽ��մ�С
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_TransparentChn(u32 dwHandle, s8 *pInBuf, s32 nInBufLen, s8 *pOutBuf, s32 *pnOutLen);

/**
 * @brief  ��ȡ�Զ�������
 * @param  dwHandle [in]���
 *	       pszKey   [in]����������
 *	       pszVal   [out]������ֵ
 *	       nSize    [in]pszVal��������С
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCustomCfg(u32 dwHandle, const s8 *pszKey, s8 *pszVal, s32 nSize);

/**
 * @brief  �����Զ�������
 * @param  dwHandle [in]���
 *	       pszKey   [in]����������
 *	       pszVal   [in]������ֵ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetCustomCfg(u32 dwHandle, const s8 *pszKey, const s8 *pszVal);

/**
 * @brief  ��ȡ����ʵʱ״̬
 * @param  dwHandle            [in]���
 *	       ptNetRealTimeStatus [out]����ʵʱ״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNetRealTimeStatus(u32 dwHandle, TNetNetRealTimeStatus *ptNetRealTimeStatus);

/**
 * @brief  ��ȡAIUͨ������
 * @param  dwHandle        [in]���
 *         pszChnId        [in]ͨ��id�����ͨ��ʹ�ö��ŷָ���磺1,2,3,4�����32��ͨ��
 *	       ptAiuChnCapList [out]AIUͨ������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAiuChnCap(u32 dwHandle, const s8 *pszChnId, TNetAiuChnCapList *ptAiuChnCapList);

/**
 * @brief  ��ȡ��������ץ��ͼƬ�б�
 * @param  dwHandle                 [in]���
 *         tGetAiuRecentSnapPicList [in]��ȡ����
 *	       ptAiuSnapPicList         [out]ץ��ͼƬ�б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAiuRecentSnapPicList(u32 dwHandle, TNetGetAiuRecentSnapPicList tGetAiuRecentSnapPicList, TNetAiuSnapPicList *ptAiuSnapPicList);

/**
 * @brief  �����¼�¼��
 * @param  dwHandle       [in]���
 *         tStartEventRec [in]�����¼�¼�����
 *	       pnRecId        [out]¼��id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StartEventRec(u32 dwHandle, TNetStartEventRec tStartEventRec, s32 *pnRecId);

/**
 * @brief  �����¼�¼����Ϣ
 * @param  dwHandle      [in]���
 *         tEventRecInfo [in]�¼�¼����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetEventRecInfo(u32 dwHandle, TNetEventRecInfo tEventRecInfo);

/**
 * @brief  ��ȡ�ֶ��¼�����
 * @param  dwHandle          [in]���
 *         ptManualEventList [out]�ֶ��¼���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetManualEventInfo(u32 dwHandle, TNetManualEventList *ptManualEventList);

/**
 * @brief  �����ֶ��¼�����
 * @param  dwHandle          [in]���
 *         tManualEventList  [in]�ֶ��¼���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetManualEventInfo(u32 dwHandle, TNetManualEventList tManualEventList);

/**
 * @brief  ��ȡ�ֶ��¼�����
 * @param  dwHandle          [in]���
 *         ptManualEventCap  [out]�ֶ��¼�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetManualEventCap(u32 dwHandle, TNetManualEventCap *ptManualEventCap);

/**
 * @brief  �ֶ�����ͨ�����ܴ���
 * @param  dwHandle             [in]���
 *         tAiuManualTrigParam  [in]��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_AiuManualTrig(u32 dwHandle, TNetAiuManualTrigParam tAiuManualTrigParam);

/**
 * @brief  ��ȡ���ؿ�����
 * @param  dwHandle     [in]���
 *         ptCtrlLibCap [out]���ؿ�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCtrlLibCap(u32 dwHandle, TNetCtrlLibCap *ptCtrlLibCap);

/**
 * @brief  ��ȡ���ؿ��б�
 * @param  dwHandle      [in]���
 *         ptCtrlLibList [out]���ؿ��б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCtrlLibList(u32 dwHandle, TNetCtrlLibList *ptCtrlLibList);

/**
 * @brief  �½����ؿ�
 * @param  dwHandle     [in]���
 *         tCtrlLibItem [in]���ؿ���Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreateCtrlLib(u32 dwHandle, TNetCtrlLibItem tCtrlLibItem);

/**
 * @brief  ɾ�����ؿ�
 * @param  dwHandle       [in]���
 *         pszCtrlLibName [in]���ؿ�����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DelCtrlLib(u32 dwHandle, const s8 *pszCtrlLibName);

/**
 * @brief  �����༭���ؿ�����
 * @param  dwHandle          [in]���
 *         pszOldCtrlLibName [in]ԭ���ؿ�����
 *         tCtrlLibItem      [in]�²��ؿ���Ϣ
 *         pnTaskId          [out]����id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreateEditCtrlLibTask(u32 dwHandle, const s8 *pszOldCtrlLibName, TNetCtrlLibItem tCtrlLibItem, s32 *pnTaskId);

/**
 * @brief  ��ȡ�༭���ؿ�״̬
 * @param  dwHandle           [in]���
 *         nTaskId            [in]����id
 *         ptEditCtrlLibState [out]�༭���ؿ�״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetEditCtrlLibState(u32 dwHandle, s32 nTaskId, TNetEditCtrlLibState *ptEditCtrlLibState);

/**
 * @brief  ���ٱ༭���ؿ�����
 * @param  dwHandle [in]���
 *         nTaskId  [in]����id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroyEditCtrlLibTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  ����������
 * @param  dwHandle         [in]���
 *         tSearchPersonLib [in]��������
 *         ptPersonList     [out]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SearchPersonLib(u32 dwHandle, TNetSearchPersonLib tSearchPersonLib, TNetPersonList *ptPersonList);

/**
 * @brief  �����Ա
 * @param  dwHandle       [in]���
 *         pszCtrlLibName [in]���ؿ�����
 *         tPersonItem    [in]��Ա��Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_AddPerson(u32 dwHandle, const s8 *pszCtrlLibName, TNetPersonItem tPersonItem);

/**
 * @brief  �༭��Ա
 * @param  dwHandle       [in]���
 *         pszCtrlLibName [in]���ؿ�����
 *         tPersonItem    [in]��Ա��Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_EditPerson(u32 dwHandle, const s8 *pszCtrlLibName, TNetPersonItem tPersonItem);

/**
 * @brief  ɾ����Ա
 * @param  dwHandle   [in]���
 *         tDelPerson [in]ɾ������Ա
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DelPerson(u32 dwHandle, TNetDelPerson tDelPerson);

/**
 * @brief  ɾ��ȫ����Ա
 * @param  dwHandle			[in]���
 *         pszCtrlLibName	[in]���ؿ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DelAllPerson(u32 dwHandle, s8 *pszCtrlLibName);

/**
 * @brief  ����·����ȡͼƬ����
 * @param  dwHandle   [in]���
 *         pPicPath	  [in]ͼƬ·��
 *         ptNetGetPicData [out]ͼƬ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetPicData(u32 dwHandle, const s8* pszPicPath, TNetPicData *ptNetGetPicData);

/**
 * @brief  ���ؿ⵼��׼��
 * @param  dwHandle				[in]���
 *         tNetCtrlLibExport	[in]��������
 *		   pdwNeedSpace			[out]��������Ĵ�С
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CtrlLibExportPrepare(u32 dwHandle, TNetCtrlLibExport tNetCtrlLibExport, u32 *pdwNeedSpace);

/**
 * @brief  ���ؿ⵼�����ɵ����ļ�
 * @param  dwHandle				[in]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CtrlLibExportCreateFile(u32 dwHandle, TNetCtrlLibExportFlieList *ptCtrlLibExportFlieList);

/**
 * @brief  ��ȡ���ؿ⵼���ļ�(�첽)
 * @param  dwHandle   [in]���
 *         pszFileName[in]�ļ���
 *         pfCallBack [in]�ص�����
 *         pParam     [in]�ص���������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CtrlLibExportGetFileAsync(u32 dwHandle, s8 *pszFileName, pfAPICallBack pfCallBack, void *pParam);

/**
 * @brief  ��ȡ���ؿ⵼���ļ�
 * @param  dwHandle   [in]���
 *         pszFileName[in]�ļ���
 *         pfCallBack [in]�ص�����
 *         pParam     [in]�ص���������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CtrlLibExportGetFile(u32 dwHandle, s8 *pszFileName, pfAPICallBack pfCallBack, void *pParam);

/**
 * @brief ���ؿ⵼����ȡ״̬
 * @param  dwHandle				[in]���
 *         peCtrlLibExportState	[out]����״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCtrlLibExportState(u32 dwHandle, ECtrlLibExportState *peCtrlLibExportState);

/**
 * @brief  ���ٲ��ؿ⵼������
 * @param  dwHandle				[in]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroyCtrlLibExportTask(u32 dwHandle);

/**
 * @brief  ���ؿ⵼��׼��
 * @param  dwHandle				[in]���
 *         dwTotalSize			[in]�����ļ���С
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CtrlLibImportPrepare(u32 dwHandle, u32 dwTotalSize);

/**
 * @brief  ���ؿ⵼���ȡ״̬
 * @param  dwHandle				[in]���
 *         ptCtrlLibExportState	[out]����״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCtrlLibImportState(u32 dwHandle, TNetCtrlLibImportState *ptCtrlLibExportState);

/**
 * @brief  ���ؿ⵼���ϴ�����
 * @param  dwHandle				[in]���
 *         tNetCtrlLibImportSend[in]���ݲ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CtrlLibImportSend(u32 dwHandle, TNetCtrlLibImportSend tNetCtrlLibImportSend);

/**
 * @brief  ��ʼ���벼�ؿ�
 * @param  dwHandle				[in]���
 *         tCtrlLibImportParam	[in]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StartCtrlLibImport(u32 dwHandle, TNetCtrlLibImportParam tCtrlLibImportParam);

/**
 * @brief  ���ؿ��ȡ���������Ϣ
 * @param  dwHandle				[in]���
 * @param  tCtrlLibErrInfo		[in]����
 * @param  ptCtrlLibErrList		[out]�����б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCtrlLibImportErrInfo(u32 dwHandle, TNetCtrlLibErrInfo tCtrlLibErrInfo, TNetCtrlLibErrList *ptCtrlLibErrList);

/**
 * @brief  ���ٲ��ؿ⵼������
 * @param  dwHandle				[in]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroyCtrlLibImportTask(u32 dwHandle);

/**
 * @brief  ���ؿ⸴��
 * @param  dwHandle				[in]���
 *         tCtrlLibCopy			[in]���Ʋ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CtrlLibCopy(u32 dwHandle, TNetCtrlLibCopy tCtrlLibCopy);

/**
 * @brief  ���ؿ⸴�ƻ�ȡ״̬
 * @param  dwHandle				[in]���
 *         peState				[out]״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCtrlLibCopyState(u32 dwHandle, ECtrlLibImportState *peState);

/**
 * @brief  ���ؿ⸴�ƻ�ȡ������Ϣ
 * @param  dwHandle				[in]���
 *         ptCtrlLibCopyErrList	[out]������Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCtrlLibCopyErrInfo(u32 dwHandle, TNetCtrlLibCopyErrList *ptCtrlLibCopyErrList);

/**
 * @brief  ���ٲ��ؿ⸴��
 * @param  dwHandle				[in]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroyCtrlLibCopyTask(u32 dwHandle);

/**
 * @brief  ��ȡ���ؿ�����ʣ���������
 * @param  dwHandle				[in]���
 * @param  pdwFreeMemNum		[out]ʣ���������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetBatchCtrlLibFreeMemNum(u32 dwHandle, u32 *pdwFreeMemNum);

/**
 * @brief  ���ؿ�������ӻ�ȡ�Ƿ�����������
 * @param  dwHandle						[in]���
 * @param  ptCtrlLibBatchPrepare		[in]�����������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetBatchCtrlLibContinueTask(u32 dwHandle, TNetCtrlLibBatchPrepare *ptCtrlLibBatchPrepare);

/**
 * @brief  ���ؿ��������׼��
 * @param  dwHandle					[in]���
 * @param  tCtrlLibBatchPrepare		[in]׼������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_BatchCtrlLibPrepare(u32 dwHandle, TNetCtrlLibBatchPrepare tCtrlLibBatchPrepare);

/**
 * @brief  ���ؿ�������ӿ�ʼ
 * @param  dwHandle				[in]���
 * @param  tCtrlLibBatch		[in]�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_StartCtrlLibBatch(u32 dwHandle, TNetCtrlLibBatch tCtrlLibBatch);

/**
 * @brief  ���ؿ�������ӻ�ȡ״̬
 * @param  dwHandle				[in]���
 * @param  ptCtrlLibBatchState	[out]״̬
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCtrlLibBatchState(u32 dwHandle, TNetCtrlLibBatchState *ptCtrlLibBatchState);

/**
 * @brief  ���ؿ�������ӻ�ȡ������Ϣ
 * @param  dwHandle				[in]���
 * @param  tCtrlLibErrInfo		[in]����
 * @param  ptCtrlLibErrList		[out]�����б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCtrlLibBatchErrInfo(u32 dwHandle, TNetCtrlLibErrInfo tCtrlLibErrInfo, TNetCtrlLibErrList *ptCtrlLibErrList);

/**
 * @brief  �������ȡ���ϴε���������
 * @param  dwHandle				[in]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CancelContinueCtrlLibBatchTask(u32 dwHandle);

/**
 * @brief  ���ٲ��ؿ������������
 * @param  dwHandle				[in]���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroyCtrlLibBatchTask(u32 dwHandle);

/**
 * @brief  ��ȡ�㷨��������
 * @param  dwHandle				[in]���
 *         ptNetAlgEngineCap	[out]�㷨��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAlgEngineCap(u32 dwHandle, TNetAlgEngineCap *ptNetAlgEngineCap);

/**
 * @brief  ��ȡ�㷨�����б�
 * @param  dwHandle      [in]���
 *         ptCtrlLibList [out]�㷨�����б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAlgEngineList(u32 dwHandle, TNetAlgEngineList *ptNetAlgEngineList);

/**
 * @brief  ���������㷨��������
 * @param  dwHandle				[in]���
 *         dwAlgEngineSize		[in]�㷨�����С
 *		   pnTaskId				[out]����id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreateLoadAlgEngineTask(u32 dwHandle, u32 dwAlgEngineSize, s32 *pnTaskId);

/**
 * @brief  ���������㷨��������
 * @param  dwHandle				[in]���
 *         nTaskId				[in]����id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroyLoadAlgEngineTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  �ϴ������㷨��������
 * @param  dwHandle				[in]���
 *         tNetAlgEngineUpload	[in]�����㷨��������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_UploadAlgEngine(u32 dwHandle, TNetAlgEngineUpload tNetAlgEngineUpload);

/**
 * @brief  ���������㷨ж������
 * @param  dwHandle				[in]���
 *         pszAlgEngineName		[in]�㷨������
 *		   pnTaskId				[out]����id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreateUnloadAlgEngineTask(u32 dwHandle, s8 *pszAlgEngineName, s32 *pnTaskId);

/**
 * @brief  ��ȡ�����㷨ж�ؽ���
 * @param  dwHandle				[in]���
 *         nTaskId				[in]����id
 *		   pbFinish				[out]�Ƿ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetUnloadAlgEngineProgress(u32 dwHandle, s32 nTaskId, BOOL32 *pbFinish);

/**
 * @brief  ���������㷨ж������
 * @param  dwHandle				[in]���
 *         nTaskId				[in]����id
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DestroyUnloadAlgEngineTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  ��ȡNVR��AIU����
 * @param  dwHandle				[in]���
 *         ptNetNvrAiuCap		[out]AIU����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNvrAiuCap(u32 dwHandle, TNetNvrAiuCap *ptNetNvrAiuCap);

/**
 * @brief  ��ȡNVR��VAPData����
 * @param  dwHandle					[in]���
 *         ptNetNvrVAPDataCap		[out]VAPData����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetNvrVAPDataCap(u32 dwHandle, TNetNvrVAPDataCap *ptNetNvrVAPDataCap);

/**
 * @brief  ��ȡAISͨ������
 * @param  dwHandle					[in]���
 *         ptNetAisChnCap			[in/out]ͨ������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAisChnCap(u32 dwHandle, TNetAisChnCap *ptNetAisChnCap);

/**
 * @brief  ��ȡͨ������ͼ����������
 * @param  dwHandle				[in]���
 *         tNetGetAiuBigDataCfg	[in]��ȡ����
 *		   ptNetAiuBigDataCfgList[out]���ò���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAisVAPDataCfg(u32 dwHandle, TNetGetAiuBigDataCfg tNetGetAiuBigDataCfg, TNetAiuBigDataCfgList *ptNetAiuBigDataCfgList);

/**
 * @brief  ����ͨ������ͼ����������
 * @param  dwHandle				[in]���
 *         tNetAiuBigDataCfg	[in]���ò���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetAisVAPDataCfg(u32 dwHandle, TNetAiuBigDataCfg tNetAiuBigDataCfg);

/**
 * @brief  ��ȡ���ܴ����������
 * @param  dwHandle				[in]���
 *         ptNetAiProcessCap	[out]���ܴ�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAiProcessCap(u32 dwHandle, TNetAiProcessCap *ptNetAiProcessCap);

/**
 * @brief  ��ȡͨ������㷨Ĭ�ϲ���
 * @param  dwHandle					[in]���
 *         ptDetectAlgDefaultParam	[out]ͨ������㷨Ĭ�ϲ���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnDetectAlgDefParam(u32 dwHandle, TNetDetectAlgDefaultParam *ptDetectAlgDefaultParam);

/**
 * @brief  ��ȡ����ͨ���б�
 * @param  dwHandle					[in]���
 *         tIntellChnParam			[in]����
 *         ptIntellChnParam			[out]����ͨ���б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetIntellChnList(u32 dwHandle, TNetIntellChnParam tIntellChnParam, TNetIntellChnList *ptIntellChnList);

/**
 * @brief  ��������ͨ���б�
 * @param  dwHandle					[in]���
 *         tIntellChnList			[in]����ͨ���б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetIntellChnList(u32 dwHandle, TNetIntellChnList tIntellChnList);

/**
 * @brief  ��ȡ����ͨ������㷨�����б�
 * @param  dwHandle					[in]���
 *         ptNetChnDetectAlgList	[out]�����б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetAipDetectChnAlgList(u32 dwHandle, TNetChnDetectAlgList *ptNetChnDetectAlgList);

/**
 * @brief  ɾ������ͨ����Ӧ�㷨������Ϣ
 * @param  dwHandle					[in]���
 *         tNetDelChnDetectAlg		[in]ͨ����Ӧ�㷨����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DelChnDetectAlgCfg(u32 dwHandle, TNetDelChnDetectAlg tNetDelChnDetectAlg);

/**
 * @brief  ��ȡͨ������㷨����
 * @param  dwHandle					[in]���
 *         tNetDelChnDetectAlg		[in]ͨ����Ӧ�㷨����
 *		   ptNetChnDetectAlgParam   [out]ͨ������㷨����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetChnDetectAlgParam(u32 dwHandle, TNetGetChnDetectAlgParam tNetGetChnDetectAlg, TNetChnDetectAlgParam *ptNetChnDetectAlgParam);

/**
 * @brief  ����ͨ������㷨����
 * @param  dwHandle					[in]���
 *         tNetChnDetectAlgParam	[in]ͨ������㷨����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetChnDetectAlgParam(u32 dwHandle, TNetChnDetectAlgParam tNetChnDetectAlgParam);

/**
 * @brief  ��ȡͨ�����˼�Ⲽ��ʱ��ʱ��
 * @param  dwHandle					[in]���
 *		   nChnId					[in]ͨ����
 *		   ptNetCompareRuleTime     [out]����ʱ��
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetPersonDetectTime(u32 dwHandle, s32 nChnId, TNetCompareRuleTime *ptNetCompareRuleTime);

/**
 * @brief  ����ͨ�����˼�Ⲽ��ʱ��ʱ��
 * @param  dwHandle					[in]���
 *		   nChnId					[in]ͨ����
 *		   ptNetCompareRuleTime     [in]����ʱ��
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetPersonDetectTime(u32 dwHandle, s32 nChnId, TNetCompareRuleTime tNetCompareRuleTime);

/**
 * @brief  ��ȡͨ�����˼��������ʽ
 * @param  dwHandle					[in]���
 *		   nChnId					[in]ͨ����
 *		   ptNetRuleLinkCfg			[out]������ʽ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetPersonDetectLinkMode(u32 dwHandle,  s32 nChnId, TNetRuleLinkCfg *ptNetRuleLinkCfg);

/**
 * @brief  ����ͨ�����˼��������ʽ
 * @param  dwHandle					[in]���
 *		   nChnId					[in]ͨ����
 *		   tNetRuleLinkCfg			[in]������ʽ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetPersonDetectLinkMode(u32 dwHandle,  s32 nChnId, TNetRuleLinkCfg tNetRuleLinkCfg);

/**
 * @brief  ��ȡ�ȶ��Ѿ������ȶԹ�����Ϣ
 * @param  dwHandle					[in]���
 *		   ptNetCompareRuleList		[out]�ȶԹ�����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCompareRuleList(u32 dwHandle, TNetCompareRuleList *ptNetCompareRuleList);

/**
 * @brief  ��ȡĬ�ϱȶԹ�������
 * @param  dwHandle					[in]���
 *		   ptCompareRule			[out]�ȶԹ�����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetDefaultCompareRule(u32 dwHandle, TNetCompareRule *ptCompareRule);

/**
 * @brief  �½��ȶԹ���
 * @param  dwHandle					[in]���
 *		   tNetCompareRule			[in/out]�ȶԹ�����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_CreateCompareRule(u32 dwHandle, TNetCompareRule *ptNetCompareRule);

/**
 * @brief  ɾ���ȶԹ���
 * @param  dwHandle					[in]���
 *		   nIndex					[in]�ȶԹ�������
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_DelCompareRule(u32 dwHandle, s32 nIndex);

/**
 * @brief  �༭�ȶԹ���
 * @param  dwHandle					[in]���
 *		   tNetCompareRule			[in]�ȶԹ�����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_EditCompareRule(u32 dwHandle, TNetCompareRule *ptNetCompareRule);

/**
 * @brief  ��ȡ�ȶԹ��򲼷�ʱ��
 * @param  dwHandle					[in]���
 *		   nIndex					[in]�ȶԹ�������
 *		   ptNetCompareRuleTime     [out]����ʱ��
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCompareRuleTime(u32 dwHandle, s32 nIndex, TNetCompareRuleTime *ptNetCompareRuleTime);

/**
 * @brief  ���ñȶԹ��򲼷�ʱ��
 * @param  dwHandle					[in]���
 *		   nIndex					[in]�ȶԹ�������
 *		   ptNetCompareRuleTime     [in]����ʱ��
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetCompareRuleTime(u32 dwHandle, s32 nIndex, TNetCompareRuleTime tNetCompareRuleTime);

/**
 * @brief  ��ȡ�ȶԹ���������ʽ
 * @param  dwHandle					[in]���
 *		   ptNetRuleLinkCfg			[in/out]�ȶԹ�������/������ʽ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetCompareRuleLinkMode(u32 dwHandle, TNetRuleLinkCfg *ptNetRuleLinkCfg);

/**
 * @brief  ���ñȶԹ���������ʽ
 * @param  dwHandle					[in]���
 *		   tNetRuleLinkCfg			[out]������ʽ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetCompareRuleLinkMode(u32 dwHandle, TNetRuleLinkCfg tNetRuleLinkCfg);

/**
 * @brief  ��Աץ�ļ���
 * @param  dwHandle					[in]���
 *		   tFaceSnapCfg				[in]��������
 *		   ptFaceSnapList			[out]��Աץ���б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_FaceSnapSearch(u32 dwHandle, TNetFaceSnapSearch tFaceSnapCfg, TNetFaceSnapList *ptFaceSnapList);

/**
 * @brief  ��־��������
 * @param  dwHandle					[in]���
 *		   tLogSearch				[in]��������
 *		   ptAlarmLogList			[out]��־���������б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_LogAlarmSearch(u32 dwHandle, TNetLogAlarmSearch tLogSearch, TNetLogAlarmList *ptAlarmLogList);

/**
 * @brief  ����ץ�ļ���
 * @param  dwHandle					[in]���
 *		   tPersonSnapCfg			[in]��������
 *		   ptFacePersonList			[out]��Աץ���б�
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_PersonSnapSearch(u32 dwHandle, TNetPersonSnapSearch tPersonSnapCfg, TNetPersonSnapList *ptFacePersonList);

/**
 * @brief  ��ȡ����Ƶ����
 * @param  dwHandle          [in]���
 *		   ptSmallVideoParam [out]����Ƶ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSmallVideoParam(u32 dwHandle, TNetSmallVideoParam *ptSmallVideoParam);

/**
 * @brief  ��ȡץ�����ò���
 * @param  dwHandle			[in]���
 * @param  ptNetSnapCfg     [out]ץ�����ò���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetSnapCfg(u32 dwHandle, TNetSnapCfg *ptSnapCfg);

/**
 * @brief  ����ץ�����ò���
 * @param  dwHandle			[in]���
 * @param  tSnapCfg			[in]���ò���
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SetSnapCfg(u32 dwHandle, TNetSnapCfg tSnapCfg);

/**
 * @brief  �ɼ�����ץ��
 * @param  dwHandle			[in]���
 * @param  tSeriesSnap		[in]�ɼ�����ץ�Ĳ���
 * @param  pnId				[out]��ʶ����
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_SeriesSnap(u32 dwHandle, TNetSeriesSnap tSeriesSnap, s32 *pnId);

/**
 * @brief  ��ȡ���ڼ��ץ���б�
 * @param  dwHandle		    [in]���
 *         pszChnId         [in]ͨ��id�����ͨ��ʹ�ö��ŷָ���磺1,2,3,4
 *         eAlgType         [in]�������
 *         ptDetectSnapList [out]���ץ����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRecentDetectSnapList(u32 dwHandle, const s8 *pszChnId, EAlgType eAlgType, TNetDetectSnapList *ptDetectSnapList);

/**
 * @brief  ��ȡ���ڱȶԱ����б�
 * @param  dwHandle		  [in]���
 *         ptCmpAlarmList [out]�ȶԱ�����Ϣ
 * @return �ɹ�����NET_OK��ʧ�ܷ��ش�����
 */
NVR_API s32 STDCALL NET_GetRecentCmpAlarmList(u32 dwHandle, TNetCmpAlarmList *ptCmpAlarmList);

#ifdef __cplusplus
}
#endif

#endif