#ifndef _NETSDK_H_
#define _NETSDK_H_

#include "netsdkerrno.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * netsdk类型定义
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
 * 宏定义
 */

#define NET_MAXLEN_8							8
#define NET_MAXLEN_16							16
#define NET_MAXLEN_32							32
#define NET_MAXLEN_64							64
#define NET_MAXLEN_128							128
#define NET_MAXLEN_256							256
#define NET_MAXLEN_512							512
#define NET_NVR_USER_NAME_MAX_LEN				64		///< NVR用户名最大长度，32个字符
#define NET_NVR_USER_PWD_MAX_LEN				16		///< NVR用户密码最大长度，16个字符
#define NET_NVR_DISK_MAX_NUM					50		///< NVR硬盘最大数量
#define NET_NVR_USER_MAX_NUM					16		///< NVR用户最大数量
#define NET_NVR_ETH_MAX_NUM						12		///< NVR网卡最大数量
#define NET_PER_GET_SEARCHED_DEV_MAX_NUM		32		///< 每次获取搜索到的设备的最大数量
#define NET_PER_ADD_DEV_MAX_NUM					64		///< 每次添加设备的最大数量
#define NET_PER_DEL_DEV_MAX_NUM					64		///< 每次删除设备的最大数量
#define NET_PER_UPGRADE_DEV_MAX_NUM				64		///< 每次升级设备的最大数量
#define NET_PER_UPLOAD_UPGRADE_FILE_MAX_SIZE    (1 * 1024 * 1024)		///< 每次上传升级包数据的最大大小
#define NET_UPLOAD_UPGRADE_FILE_CGI_MAX_SIZE    (2 * 1024 * 1024)		///< 上传升级包协议的最大大小
#define NET_PER_GET_NETDISK_MAX_NUM				16		///< 每次获取网络硬盘的最大数量
#define NET_PROTO_MAX_NUM						8		///< 协议最大数量
#define NET_PER_GET_CHNLIST_MAX_NUM				32		///< 每次获取通道列表的最大数量
#define NET_CHNGROUP_CHNITEM_MAX_NUM			64		///< 通道分组内通道最大数量
#define NET_CHNGROUP_MAX_NUM					16		///< 通道分组最大数量
#define NET_FRAME_STYLE_GRID_MAX_NUM			128		///< 画面风格中格子的最大数量
#define NET_BIND_MAX_NUM						128		///< 绑定最大数量
#define NET_PIP_MAX_NUM							16		///< 画中画最大数量
#define NET_DEC_STATE_MAX_NUM					256		///< 解码状态最大数量
#define NET_PER_GET_CHN_CAP_MAX_NUM				128		///< 每次获取通道能力列表的最大数量
#define NET_PER_GET_CHNPERM_MAX_NUM				256		///< 每次获取通道权限列表的最大数量
#define NET_VIDEOENC_RECMODE_MAX_NUM			8		///< 视频编码码率控制模式最大数量
#define NET_VIDEOENC_RESOLUTION_MAX_NUM			32		///< 视频编码分辨率最大数量
#define NET_VIDEOENC_ENCTYPE_MAX_NUM  			8		///< 视频编码编码类型最大数量
#define NET_PER_CHN_ENCCHN_MAX_NUM  			8		///< 每个通道最大编码通道数量
#define NET_AUDIOENC_ENCTYPE_MAX_NUM 			8		///< 音频编码编码类型最大数量
#define NET_LOG_MAINTYPR_MAX_NUM     			5		///< 日志类别最大数量
#define NET_LOG_SUBTYPR_MAX_NUM     			20		///< 日志类型最大数量
#define NET_PER_GET_LOG_MAX_NUM         		100		///< 每次获取日志最大条数
#define NET_CHN_ALIAS_LIST_MAX_NUM		    	128		///< 通道别名列表最大数量
#define NET_DISPLAY_PORT_MAX_NUM                4		///< 显示接口最大数量
#define NET_DISPLAY_PORT_RESOLUTION_MAX_NUM     16		///< 显示接口分辨率最大数量
#define NET_PER_CHNGETRECLIST_MAX_NUM           /*300*/150		///< 每次获取通道录像的最大数量
#define NET_PER_CHNPLAYBACKLIST_MAX_NUM         32		///< 每次放像通道的最大数量
#define NET_PER_CHNGETMONTHREC_MAX_NUM          32		///< 每次获取月视图通道的最大数量
#define NET_GROUP_DISKS_MAX_NUM					64		///< 盘组支持硬盘ID数目
#define NET_GROUP_MAX_NUM						16		///< 支持盘组数目
#define NET_EXDISK_MAX_NUM						16		///< 支持挂载外部USB
#define NET_INTERNALDISK_MAX_NUM			    16		///< 最大内置硬盘数目
#define NET_RECTIMEPLAN_MAX_NUM                 8		///< 录像计划最大数量
#define NET_PRE_DAY_RECTIMEPLAN_MAX_NUM         8		///< 每天最大的录像时间段数
#define NET_ALARM_GUARD_TIME_SEG_MAX_NUM		8		///< 每天告警布防时间时间段最大数量
#define NET_ALARM_GUARD_TIME_MAX_NUM			8		///< 告警布防时间最大数量
#define NET_ALARM_IN_ID_MAX_NUM					20		///< 告警源告警输入号最大数量
#define NET_ALARM_IN_COPY_MAX_NUM				64		///< 告警输入复制最大数量
#define NET_ALARM_LINK_OUT_MAX_NUM				256		///< 告警联动输出最大数量
#define NET_ALARM_LINK_PTZ_MAX_NUM				256		///< 告警联动PTZ最大数量
#define NET_SYS_ALARM_CFG_MAX_NUM				16		///< 系统告警配置最大数量
#define NET_COPYCHNRECPLAN_MAX_NUM				256		///< 拷贝通道录像计划最大数量
#define NET_HOLIDAY_MAX_NUM				        12		///< 假日最大数量
#define NET_DISKGROUP_CHN_MAX_NUM               128		///< 盘组最大通道数
#define NET_PLAYBACK_MAX_NUM    				16		///< 回放最大通道数
#define NET_PLAYBACK_ITEM_MAX_NUM  				4		///< 通道回放路数最大数量
#define NET_COPYDISKQUOTA_MAX_NUM			    512		///< 拷贝通道配额最大数量
#define NET_PER_QUERYSNAPPIC_MAX_NUM            300		///< 每次查询抓拍图片的最大数量
#define NET_PER_QUERYRECTAG_MAX_NUM             200		///< 每次查询录像标签的最大数量
#define NET_REAL_LOG_MAX_NUM                    100		///< 实时日志最大条数
#define NET_REAL_STATE_MAX_NUM                  15		///< 实时状态最大条数
#define NET_COMPOSITE_CHN_MAX_NUM               16		///< 合成通道最大数量
#define NET_DEVOSD_MAX_NUM  					16		///< 设备OSD最大数量
#define NET_USERLEVEL_MAX_NUM  					3		///< 用户级别最大数量
#define NET_PER_GET_CHN_ALARMCFG_MAX_NUM		32		///< 每次获取通道告警配置通道的最大数量
#define NET_CHNID_AUTO							0		///< 添加设备，表示不指定通道，自动分配
#define NET_MAX_SMART_ATTR_ITEM_NUM             32      ///< smart检测最大属性数量
#define NET_MAX_ERROR_SECTOR_NUM                50      ///< 坏道检测获取错误lba最大数量
#define NET_UPNP_PORT_MAP_MAX_NUM				10		///< UPnP映射端口最大数量
#define NET_NVR_VIDEO_ENC_MAX_NUM		    	4		///< NVR视频编码通道最大数量
#define NET_NVR_AUDIO_ENC_MAX_NUM		    	4		///< NVR音频编码通道最大数量
#define NET_PER_GET_CHNRECSTATE_MAX_NUM			32		///< 每次获取通道录像状态的最大数量
#define NET_PER_GET_ALARM_STATE_MAX_NUM			128		///< 每次获取告警状态的最大数量
#define NET_CLOUD_QR_CODE_MAX_NUM				4		///< 云服务二维码最大数量
#define NET_PER_DEV_ACTIVE_MAX_NUM			    32		///< 每次添加激活设备的最大数量
#define NET_PER_LOCK_REC_MAX_NUM			    100		///< 每次锁定录像的最大数量
#define NET_NVR_NTY_EVENT_MAX_NUM			    50		///< NVR通知事件最大数量
#define NET_GB28181_PRECHN_ENC_MAX_NUM		    4		///< 国标设备每个通道编码通道最大数量
#define NET_GB28181_CHN_MAX_NUM		            64		///< 国标设备每个通道编码通道最大数量
#define NET_SIM_CARD_MAX_NUM					4		///< SIM卡最大数量
#define NET_GB28181_REG_PLAT_MAX_NUM			4		///< 国标注册平台最大数量
#define NET_PUBSEC_REG_PLAT_MAX_NUM				4		///< 视图库注册平台最大数量
#define NET_AIU_RECENT_SNAP_PIC_GROUP_MAX_NUM			10		///< 近期智能抓拍图片组数最大数量
#define NET_AIU_RECENT_SNAP_PIC_ITEM_MAX_NUM	5		///< 近期智能抓拍每组图片最大数量
#define NET_PER_GET_AIU_CHN_CAP_MAX_NUM			32		///< 每次获取AIU通道能力列表的最大数量
#define NET_FACE_MAX_NUM						16		///< 人脸最大数量
#define NET_CAR_PLATE_MAX_NUM					32		///< 车牌最大数量
#define NET_PIC_UPLOAD_PUBSEC_PLAT_MAX_NUM		4		///< 图片上传视图库平台最大数量
#define NET_MANUALEVENT_MAXNUM                  32      ///< 手动事件最大数量
#define NET_IPADDRFILTERLIST_MAXNUM             64      ///< IP地址过滤列表最大数量
#define NET_SYSTIME_SYNCTYPE_MAX_NUM            16      ///< 时间同步类型最大数量
#define NET_FISH_EYE_RECT_MAX_NUM				8		///< 鱼眼ptz模式矩形选取框最大数量
#define NET_DISK_PART_MAX_NUM					4		///< 磁盘自定义分区最大数量
#define NET_PTZ_3D_NRM_MAX_NUM					64		///< ptz3D缩放通道最大数量
#define NET_CTRLLIB_MAX_NUM						32		///< 布控库最大数量
#define NET_CTRLLIB_CUSTOM_LABEL_MAX_NUM		3		///< 布控库自定义标签最大数量
#define NET_RECOG_ENGINE_MAX_NUM				4		///< 识别引擎最大数量
#define NET_PERSON_ITEM_MAX_NUM					36		///< 每次获取人脸库人员的最大数量
#define NET_GET_ERR_PERSON_ITEM_MAX_NUM			100		///< 每次获取错误信息人员的最大数量
#define NET_PER_DEL_PERSON_MAX_NUM				36		///< 每次删除人员的最大数量
#define NET_CTRLLIB_EXPORT_FILE_MAX_NUM			10		///< 布控库导出文件的最大数量
#define NET_ALGENGINE_MAX_NUM				    16		///< 加载算法引擎最大数量
#define NET_ALG_TYPE_MAX_NUM					2		///< 支持的算法类型最大数量
#define NET_AIUUPLOADCAPS_MAX_NUM				16		///< 上传模式的支持类型的最大数量
#define NET_AIU_GETBIGDATACFG_MAX_NUM			16		///< 获取通道视音图大数据配置的最大数量
#define NET_AIU_BIGDATA_UPLOAD_MAX_NUM			16		///< 视音图大数据配置上传的最大数量
#define NET_AIU_IMPORT_CTRLLIB_FILE_MAX_SIZE		(1 * 1024 * 1024)		///< 每次布控库导入上传数据最大大小
#define NET_AIU_IMPORT_CTRLLIB_FILE_CGI_MAX_SIZE    (2 * 1024 * 1024)		///< 上传布控库导入上传数据的最大大小
#define NET_AIP_DETECT_CHN_MAX_NUM				128		///< 智能处理支持检测的通道列表最大数量
#define NET_AIP_UPLOAD_TYPE_MAX_NUM				4		///< 智能处理支持上传方式最大数量
#define NET_AIP_AI_CHN_ALG_PARAM_MAX_NUM		4		///< 智能通道对应算法参数列表最大数量
#define NET_AIP_COMPARE_RULE_CHN_MAX_NUM		64		///< 智能处理比对通道最大数量
#define NET_AIP_COMPARE_RULE_MAX_NUM			30		///< 智能处理比对规则列表最大数量
#define NET_AISEARCH_CHN_MAX_NUM				128		///< 智能检索每次获取的最大通道数
#define NET_FACE_SNAP_ITEM_MAX_NUM				36		///< 智能检索每次获取人员的最大数量
#define NET_DETECT_SNAP_LIST_MAX_NUM			16		///< 检测抓拍列表最大数量
#define NET_CMP_ALARM_LIST_MAX_NUM				50		///< 比对报警列表最大数量
#define NET_GB28181_ENC_CHN_MAX_NUM		        260		///< 国标接入平台最大视频通道数
#define NET_GB28181_ALARM_CHN_MAX_NUM		    256		///< 国标接入平台最大告警通道数
#define NET_SUBSCRIBE_MSG_MAX_NUM				5		///< 订阅消息最大数量
#define NET_HEALTHLOG_TYPE_MAX_NUM				5		///< 健康状态信息类型最大数量
#define NET_GET_HEALTHLOG_DETAIL_MAX_NUM		30		///< 获取健康状态详情最大数量
#define NET_PTZ_PRESET_MAX_NUM					256		///< ptz预置位最大个数
#define NET_AI_RULELINK_UPLOAD_MAX_NUM			2		///< 比对规则联动方式上传视图库最大数量
#define NET_ISP_SCENE_CUSTOMLIST_MAX_NUM		8		///< ISP场景模式用户自定义列表最大数量
#define NET_ISP_SCENE_DAY_DYNAMIC_MAX_NUM       10		///< ISP场景模式每天最大的动态模式时间段数
#define NET_AREA_POINT_MAX_NUM					8       ///< 区域设置定点最大数量


/**
 * 枚举定义
 */

enum EInterType
{
	INTERTYPE_NONE = 0,
	INTERTYPE_GetChnVideoEncAsync,			///< 获取通道视频编码参数
	INTERTYPE_GetChnAudioEncAsync,			///< 获取通道音频编码参数
	INTERTYPE_CtrlLibExportGetFileAsync,	///< 获取布控库导出文件
};

enum EMsgType
{
	MSGTYPE_FACE_DETECT_SNAP	= 1,	///< 人脸检测抓拍消息，消息体TNetDetectSnapItem
	MSGTYPE_CAR_DETECT_SNAP		= 2,	///< 车辆检测抓拍消息，消息体TNetDetectSnapItem
	MSGTYPE_PERSON_DETECT_SNAP	= 3,	///< 行人检测抓拍消息，消息体TNetDetectSnapItem
	MSGTYPE_CMP_ALARM			= 4,	///< 比对报警消息，消息体TNetCmpAlarmItem
	MSGTYPE_PIC_UPLOAD_STATUS	= 5,	///< 图片上传状态消息，消息体TNetPicUploadStatus
};

enum EGuiLangType
{
	GUILANGTYPE_CN	= 0x01, ///< 简体中文
	GUILANGTYPE_CHT = 0x02, ///< 繁体中文
	GUILANGTYPE_EN	= 0x04, ///< 英文
	GUILANGTYPE_ES	= 0x08, ///< 西班牙语
	GUILANGTYPE_TR	= 0x10, ///< 土耳其语
	GUILANGTYPE_RU	= 0x20, ///< 俄语
	GUILANGTYPE_AR	= 0x40, ///< 阿拉伯语
};

enum ENvrState
{
	NVRSTATE_DISCONNECTED	= 0x00000001, ///< 链接断开
	NVRSTATE_CHNLIST		= 0x00000002, ///< 通道列表改变
	NVRSTATE_CHNALIAS		= 0x00000004, ///< 通道别名改变
	NVRSTATE_CHNSTATE		= 0x00000008, ///< 通道状态改变
	NVRSTATE_CHNGROUP		= 0x00000010, ///< 通道组改变
	NVRSTATE_RESOLUTION		= 0x00000020, ///< 分辨率改变
	NVRSTATE_DISKHOTSWAP	= 0x00000040, ///< 硬盘热插拔
	NVRSTATE_REALLOG		= 0x00000080, ///< 实时日志
	NVRSTATE_REALSTATE		= 0x00000100, ///< 实时状态
	NVRSTATE_SYSSTATE		= 0x00000200, ///< 系统状态（U盘插拔、网线插拔等）
	NVRSTATE_NETSTATE		= 0x00000400, ///< 网络状态改变
	NVRSTATE_AIUSTATE		= 0x00000800, ///< 智能状态改变
	NVRSTATE_ALARMSTATE		= 0x00001000, ///< 告警状态改变
};

enum EProtoType
{
	PROTOTYPE_UNKNOWN   = 0x00,   ///< 未知协议
	PROTOTYPE_ONVIF		= 0x01,   ///< onvif协议
	PROTOTYPE_GB28181	= 0x02,   ///< GB28181协议
	PROTOTYPE_RTSP      = 0x04,   ///< RTSP协议
	PROTOTYPE_VSIP      = 0x08,   ///< VSIP协议
	PROTOTYPE_IPCSEARCH = 0x10,   ///< ipcsearch协议
	PROTOTYPE_LCAM      = 0x20,   ///< 本地摄像头
	PROTOTYPE_ALL		= 0xFF,   ///< 所有协议
};

enum ETransProto
{
	ETRANSPROTO_AUTO			= 0x01,   ///< 自动
	ETRANSPROTO_TCP				= 0x02,   ///< tcp
	ETRANSPROTO_UDP				= 0x04,   ///< udp
	ETRANSPROTO_RTP_OVER_TCP	= 0x08,   ///< rtp_over_tcp
};

enum ETcpMode
{
	TCPMODE_AUTO			= 0x01,   ///< 自动
	TCPMODE_CONNECT			= 0x02,   ///< 主动
	TCPMODE_LISTEN			= 0x04,   ///< 被动
};

enum EDevType
{
	DEVTYPE_NULL	= 0,    ///< 无效值
	DEVTYPE_IPC		= 1,    ///< IPC
	DEVTYPE_DVS		= 2,    ///< DVS
	DEVTYPE_DVR		= 3,    ///< DVR
	DEVTYPE_NVR		= 4,    ///< NVR
	DEVTYPE_FISHEYE = 5,    ///< SINGLE_SRC_FISHEYE
};

enum EActiveState
{
	ACTIVESTATE_ACTIVE		= 1, ///< 已激活
	ACTIVESTATE_NOTACTIVE	= 2, ///< 未激活
	ACTIVESTATE_ABNORMAL	= 3, ///< 异常
	ACTIVESTATE_UNKNOWN		= 4, ///< 未知
	ACTIVESTATE_UNSUPPORT	= 5, ///< 不支持
};

enum EDevAddMode
{
	DEVADDMODE_AUTO     		= 0x00,    ///< 自动
	DEVADDMODE_SINGLESRC		= 0x01,    ///< 单源添加:single_source
	DEVADDMODE_MULITSRC	    	= 0x02,    ///< 多源添加:multi_source
	DEVADDMODE_KEDAFISHEYE		= 0x04,    ///< KEDA鱼眼:keda_fisheye
};

enum EChnMask
{
	CHNMASK_CHNALIAS	= 0x01, ///< 获取通道列表只获取通道别名
	CHNMASK_CHNSTATE	= 0x02, ///< 获取通道列表只获取通道状态
	CHNMASK_ALL			= 0xFF, ///< 获取通道列表获取全部信息
};

enum ECompositeChnType
{
	COMPOSITECHNTYPE_COMPOSITE	        = 1, ///< 合成通道，composite
	COMPOSITECHNTYPE_COMPOSITE_EXTENC	= 2, ///< 码流来自外置编码器的合成通道，composite_extenc
	COMPOSITECHNTYPE_FROMREMOTE         = 3, ///< 来自远端通道，from_remote
	COMPOSITECHNTYPE_TOREMOTE           = 4, ///< 发往远端通道，to_remote
};

enum EDevErrNo
{
	DEVERRNO_UNKNOWN		= 0, ///< 未知错误
	DEVERRNO_LINKING		= 1, ///< 上线中
	DEVERRNO_CONNECTFAILED	= 2, ///< 连接失败，可能网络不通
	DEVERRNO_AUTHFAILED		= 3, ///< 认证失败，例如onvif的用户名密码不对
	DEVERRNO_PARAMERR		= 4, ///< 参数错误
	DEVERRNO_CHNABNORMAL	= 5, ///< 通道异常，例如获取编码参数失败
	DEVERRNO_NOSTREAM		= 6, ///< 取流失败
	DEVERRNO_NOREGISTER		= 7, ///< 未注册
};

enum EDevActiveState
{
	DEVACTIVESTATE_NONE		    = 0, ///< 还没有开始进行
	DEVACTIVESTATE_DOING		= 1, ///< 正在进行中
	DEVACTIVESTATE_SUCCESS     	= 2, ///< 成功
	DEVACTIVESTATE_FAIL		    = 3, ///< 失败
};

enum EDevActiveErrReason
{
	DEVACTIVEERRREASON_UNKNOWN		    = 0, ///< 未知错误
	DEVACTIVEERRREASON_AUTHFAIL		    = 1, ///< 鉴权失败
	DEVACTIVEERRREASON_FORBIDDEN   	    = 2, ///< 鉴权失败过多
	DEVACTIVEERRREASON_TIMEOUT  		= 3, ///< 超时
	DEVACTIVEERRREASON_ACTIVEAGAIN      = 4, ///< 重复激活
	DEVACTIVEERRREASON_NOSUPPORTVER  	= 5, ///< 不支持的旧版本指令
	DEVACTIVEERRREASON_INVALIDARG  		= 6, ///< 无效参数(SECURE CODE不正确)
	DEVACTIVEERRREASON_PWDWEAK  		= 7, ///< 激活密码安全性不足
};

enum EDevUpgradeState
{
	DEVUPGRADESTATE_PREPARING		= 0, ///< 准备升级中
	DEVUPGRADESTATE_SENDING     	= 1, ///< 正在发送升级包
	DEVUPGRADESTATE_CHECKING		= 2, ///< 正在检测前端是否升级
	DEVUPGRADESTATE_DONE    		= 3, ///< 升级正常完成
	DEVUPGRADESTATE_ERROR        	= 4, ///< 升级中出错
	DEVUPGRADESTATE_NOTSUPPORT		= 5, ///< 不支持升级
	DEVUPGRADESTATE_DEVNOTEXIST		= 6, ///< 设备不存在
};

enum EDevUpgradeErrReason
{
	DEVUPGRADEERRREASON_AUTHFAIL		= 0, ///< 鉴权失败
	DEVUPGRADEERRREASON_PREPAREERR		= 1, ///< 准备阶段失败
	DEVUPGRADEERRREASON_UPLOADERR   	= 2, ///< 上传升级包失败
	DEVUPGRADEERRREASON_TIMEOUT  		= 3, ///< 超时
	DEVUPGRADEERRREASON_CHECKERR  		= 4, ///< 检查升级状态失败
};

enum ETimeSyncType
{
	TIMESYNCTYPE_CLOSE         = 0x01,   ///< 关闭
	TIMESYNCTYPE_TIMEANDZONE   = 0x02,   ///< 时区和时间格式
	TIMESYNCTYPE_LOCALTIME     = 0x04,   ///< 本地时间格式
	TIMESYNCTYPE_UTCTIME       = 0x08,   ///< UTC时间格式
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
	OSDPICTYPE_BMP         = 0x01,   ///< bmp格式
	OSDPICTYPE_JPG         = 0x02,   ///< jpg格式
};

enum EOsdLanguageType
{
	OSDLANGUAGETYPE_CHS               = 0x0001,   ///< 中文简体
	OSDLANGUAGETYPE_CHT               = 0x0002,   ///< 中文繁体
	OSDLANGUAGETYPE_EN                = 0x0004,   ///< 英语
	OSDLANGUAGETYPE_KOR               = 0x0008,   ///< 韩文
};

enum EOsdTextColor
{
	OSDTEXTCOLOR_BLACK              = 0x0001,   ///< 黑色
	OSDTEXTCOLOR_WHITE              = 0x0002,   ///< 白色
	OSDTEXTCOLOR_RED                = 0x0004,   ///< 红色
	OSDTEXTCOLOR_GREEN              = 0x0008,   ///< 绿色
	OSDTEXTCOLOR_BLUE               = 0x0010,   ///< 蓝色
	OSDTEXTCOLOR_AQUA               = 0x0020,   ///< 青色
	OSDTEXTCOLOR_FUCHSIA            = 0x0040,   ///< 粉色
	OSDTEXTCOLOR_GRAY               = 0x0080,   ///< 灰色
	OSDTEXTCOLOR_LIME               = 0x0100,   ///< 浅绿色
	OSDTEXTCOLOR_MAROON             = 0x0200,   ///< 栗色
	OSDTEXTCOLOR_NAVY               = 0x0400,   ///< 深蓝色
	OSDTEXTCOLOR_OLIVE              = 0x0800,   ///< 橄榄色
	OSDTEXTCOLOR_PURPLE             = 0x1000,   ///< 紫色
	OSDTEXTCOLOR_SILVER             = 0x2000,   ///< 银色
	OSDTEXTCOLOR_TEAL               = 0x4000,   ///< 茶色
	OSDTEXTCOLOR_YELLOW             = 0x8000,   ///< 黄色
};

enum EOsdType
{
	OSDTYPE_UNKNOWN                 = 0x0000,   ///< 未知类型OSD
	OSDTYPE_USER                    = 0x0001,   ///< 自定义，user
	OSDTYPE_LABEL                   = 0x0002,   ///< 台标，label
	OSDTYPE_TIME                    = 0x0004,   ///< 时间，time
	OSDTYPE_ALARM                   = 0x0008,   ///< 告警，alarm
	OSDTYPE_PTZ                     = 0x0010,   ///< 方位图，ptz
	OSDTYPE_3G                      = 0x0020,   ///< 3G图标，3g
	OSDTYPE_RECORD                  = 0x0040,   ///< 录像标志，record
	OSDTYPE_GPS                     = 0x0080,   ///< 经纬度，gps
	OSDTYPE_SERIESID                = 0x0100,   ///< 设备序列号，seriesid
	OSDTYPE_PTPOINT                 = 0x0200,   ///< 俯仰角，ptpoint
	OSDTYPE_SPEED                   = 0x0400,   ///< 速度，speed
	OSDTYPE_TF                      = 0x0400,   ///< 磁盘异常，tf
};

enum EOsdContentType
{
	OSDCONTENTTYPE_TXT              = 0x0001,   ///< 文本，txt
	OSDCONTENTTYPE_PIC              = 0x0002,   ///< 图片，pic
	OSDCONTENTTYPE_DEV              = 0x0004,   ///< 外设，dev
	OSDCONTENTTYPE_ZM               = 0x0008,   ///< 放大倍率，zm
	OSDCONTENTTYPE_BAT              = 0x0010,   ///< 电池电量，bat
	OSDCONTENTTYPE_GPS              = 0x0020,   ///< GPS信息，gps
	OSDCONTENTTYPE_STATIC           = 0x0040,   ///< 不可编辑字幕(无线字幕)，static
	OSDCONTENTTYPE_EDIT             = 0x0080,   ///< 可编辑字幕(无线字幕)，edit
	OSDCONTENTTYPE_DYNAMIC          = 0x0100,   ///< 动态字幕(无线字幕))，dynamic
};

enum EOsdMarginType
{
	OSDMARGINTYPE_0         = 0x01,   ///< 边距为0
	OSDMARGINTYPE_1         = 0x02,   ///< 边距为1
	OSDMARGINTYPE_2         = 0x04,   ///< 边距为2
	OSDMARGINTYPE_3         = 0x08,   ///< 边距为3
	OSDMARGINTYPE_4         = 0x10,   ///< 边距为4
};

enum EOsdTimeFormat
{
	OSDTIMEFORMAT_MFIRST         = 0x01,   ///< MM-DD-YYYY
	OSDTIMEFORMAT_YFIRST         = 0x02,   ///< YYYY-MM-DD
	OSDTIMEFORMAT_DFIRST         = 0x04,   ///< DD-MM-YYYY
};

enum EOsdFontType
{
	OSDFONTTYPE_SONG          = 0x01,   ///< 宋体(song)
	OSDFONTTYPE_BLACK         = 0x02,   ///< 黑体(black)
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
	OSDFONTATTRTYPE_NAME          = 0x01,   ///< 字体，name
	OSDFONTATTRTYPE_SIZE          = 0x02,   ///< 大小，size
	OSDFONTATTRTYPE_COLOR         = 0x04,   ///< 颜色，color
	OSDFONTATTRTYPE_BKCLR         = 0x08,   ///< 背景色，bkclr
	OSDFONTATTRTYPE_TRANSPARENT   = 0x10,   ///< 透明度，transparent
	OSDFONTATTRTYPE_EDGE          = 0x20,   ///< 描边，edge
};

enum EDiskState
{
	DISKSTATE_NORMAL	= 0, ///< 正常
	DISKSTATE_DORMANCY	= 1, ///< 休眠
	DISKSTATE_ABNORMAL	= 2, ///< 异常
	DISKSTATE_UNINIT	= 3, ///< 未初始化
	DISKSTATE_OFFLINE	= 4, ///< 不在线
};

enum EDiskAttribute
{
	DISKATTRIBUTE_READWRITE	= 0, ///< 可读写
	DISKATTRIBUTE_READONLY	= 1, ///< 只读
	DISKATTRIBUTE_UNKNOWN	= 2, ///< 未知
};

enum EDiskType
{
	DISKTYPE_INTERNAL	= 0x0001, ///< 内置硬盘
	DISKTYPE_ESATA		= 0x0002, ///< esata
	DISKTYPE_USB		= 0x0004, ///< usb
	DISKTYPE_IPSAN		= 0x0008, ///< ipsan
	DISKTYPE_NAS		= 0x0010, ///< nas
	DISKTYPE_RAID		= 0x0020, ///< raid
	DISKTYPE_LOCAL_SD	= 0x0040, ///< 存储卡类型-本地外置
	DISKTYPE_INTERNAL_SD = 0x0080, ///< 存储卡类型-本地内置
};

enum EDiskPartType
{
	DISKPARTTYPE_REC	= 0x0001, ///< 录像分区
	DISKPARTTYPE_IMG	= 0x0002, ///< 抓拍分区
};

enum EDiskSmartTestState
{
	DISKSMARTTESTSTATE_HEALTH	        = 0, ///< health,健康
	DISKSMARTTESTSTATE_BAD		        = 1, ///< bad,坏盘
	DISKSMARTTESTSTATE_NOTSUPPORT		= 2, ///< not_support,不支持
	DISKSMARTTESTSTATE_PARAERROR		= 3, ///< para_err,检测出错
};

enum EDiskUsingType
{
	DISKUSINGTYPE_NORMAL	        = 0, ///< 普通存储
	DISKUSINGTYPE_SMART		        = 1, ///< 智能存储
};

enum ENetDiskType
{
	NETDISKTYPE_IPSAN	= 0x01, ///< ipsan
	NETDISKTYPE_NAS		= 0x02, ///< nas
};

enum EStoreMode
{
	STOREMODE_QUOTA       = 0x01,   ///< 配额
	STOREMODE_DISKPACK    = 0x02,   ///< 盘组
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
	RAIDCREATETYPE_FAST  = 0x01, ///< fast,快速创建
	RAIDCREATETYPE_FULL  = 0x02, ///< full,完整创建
};

enum ERaidStatus
{
	RAIDSTATUS_CLEAN       = 0x0001, ///< clean,正常
	RAIDSTATUS_DEGRADED    = 0x0002, ///< degraded,降级
	RAIDSTATUS_RECOVERING  = 0x0004, ///< recovering,恢复
	RAIDSTATUS_FAILED      = 0x0008, ///< failed,失效
	RAIDSTATUS_RESYNCING   = 0x0010, ///< resyncing,同步
	RAIDSTATUS_REPAIRING   = 0x0020, ///< repairing,修复中
	RAIDSTATUS_INITING     = 0x0040, ///< initing,初始化
	RAIDSTATUS_CHECKING    = 0x0080, ///< checking,检查
	RAIDSTATUS_DELING      = 0x0100, ///< deling,删除
};

enum EFrameStyle
{
	FRAMESTYLE_1  = 1,		///< 1画面(1*1)
	FRAMESTYLE_4  = 4,		///< 4画面(2*2)
	FRAMESTYLE_6  = 6,		///< 6画面(1+5)
	FRAMESTYLE_8  = 8,		///< 8画面(1+7)
	FRAMESTYLE_9  = 9,		///< 9画面(3*3)
	FRAMESTYLE_16 = 16,		///< 16画面(4*4)
	FRAMESTYLE_25 = 25,		///< 25画面(5*5)
	FRAMESTYLE_36 = 36,		///< 36画面(6*6)
	FRAMESTYLE_49 = 49,		///< 49画面(7*7)
	FRAMESTYLE_64 = 64,		///< 64画面(8*8)
};

enum EPipType
{
	PIPTYPE_MONITOR				= 1, ///< 实时浏览画中画
	PIPTYPE_MONITORDIGITALZOOM	= 2, ///< 实时浏览数字放大画中画
	PIPTYPE_PLAYREC				= 3, ///< 回放画中画
	PIPTYPE_PLAYRECDIGITALZOOM  = 4, ///< 回放数字放大画中画
	PIPTYPE_PLAYRECPIC			= 5, ///< 回放时间轴预览图片
};

enum ETransType
{
	TRANSTYPE_RTPOVERTCP			= 0x01, ///< 传输方式，rtp_over_udp
	TRANSTYPE_RTPOVERUDP			= 0x02, ///< 传输方式，rtp_over_tcp
	TRANSTYPE_RTPOVERRTSPOVERTCP	= 0x04, ///< 传输方式，rtp_over_rtsp_over_tcp
	TRANSTYPE_RTPOVERHTTP			= 0x08, ///< 传输方式，rtp_over_http
};

enum EStreamPackType
{
	STREAMPACKTYPE_PS	= 0x01, ///< 码流打包方式，PS
	STREAMPACKTYPE_ES	= 0x02, ///< 码流打包方式，ES
};

enum ECallMode
{
	CAllMODE_LOCALCALL			= 1, ///< 本地呼叫
	CAllMODE_LOCALBROADCAST		= 2, ///< 本地广播
	CAllMODE_REMOTECALL			= 3, ///< 远端呼叫
	CAllMODE_REMOTEBROADCAST	= 4, ///< 远端广播
};

enum ECallType
{
	CALLTYPE_CHN = 2, ///< 呼叫通道
};

enum EPTZCMD
{
	PTZCMD_MOVEUP			= 1,  ///< 向上移动
	PTZCMD_MOVEDOWN			= 2,  ///< 向下移动
	PTZCMD_MOVELEFT			= 3,  ///< 向左移动
	PTZCMD_MOVERIGHT		= 4,  ///< 向右移动
	PTZCMD_MOVELEFTUP		= 5,  ///< 左上移动
	PTZCMD_MOVELEFTDOWN		= 6,  ///< 左下移动
	PTZCMD_MOVERIGHTUP		= 7,  ///< 右上移动
	PTZCMD_MOVERIGHTDOWN	= 8,  ///< 右下移动
	PTZCMD_MOVESTOP			= 9,  ///< 停止移动
	PTZCMD_RESET			= 10, ///< 复位
	PTZCMD_FOCUSFAR			= 11, ///< 焦距调远
	PTZCMD_FOCUSNEAR		= 12, ///< 焦距调近
	PTZCMD_FOCUSAUTO		= 13, ///< 自动调焦
	PTZCMD_FOCUSSTOP		= 14, ///< 停止调焦
	PTZCMD_IRISPLUS			= 15, ///< 光圈调大
	PTZCMD_IRISMINUS		= 16, ///< 光圈调小
	PTZCMD_IRISAUTO			= 17, ///< 自动光圈
	PTZCMD_IRISSTOP			= 18, ///< 停止光圈调节
	PTZCMD_ZOOMTELE			= 19, ///< 拉近
	PTZCMD_ZOOMWIDE			= 20, ///< 拉远
	PTZCMD_ZOOMSTOP			= 21, ///< 停止视野调节
	PTZCMD_LIGHTOPEN		= 22, ///< 开灯
	PTZCMD_LIGHTCLOSE		= 23, ///< 关灯
	PTZCMD_WIPEROPEN		= 24, ///< 开雨刷
	PTZCMD_WIPERCLOSE		= 25, ///< 关雨刷
	PTZCMD_PRESET_CALL		= 26, ///< 预置位调用
	PTZCMD_PRESET_SAVE		= 27, ///< 保存预置位
	PTZCMD_PRESET_DEL		= 28, ///< 删除预置位
	PTZCMD_PATHCRUISE_CALL	= 29, ///< 巡航路径调用
	PTZCMD_PATHCRUISE_STOP	= 30, ///< 巡航路径停止
	PTZCMD_ZOOM_PART        = 31, ///< 局部放大（框选放大）
	PTZCMD_ZOOM_WHOLE		= 32, ///< 局部缩小（框选缩小）
	PTZCMD_GOTO_POINT		= 33, ///< 中心定位（双击居中）
};

enum EFishInstallType
{
	FISHINSTALLTYPE_CEIL	= 0x01,		///< 吸顶
	FISHINSTALLTYPE_MT		= 0x02,		///< 墙壁
	FISHINSTALLTYPE_DT		= 0x04,		///< 桌面
};

enum EFishDisplayType
{
	FISHDISPLAYTYPE_FE			= 0x0001,		///< 鱼眼
	FISHDISPLAYTYPE_OV			= 0x0002,		///< 全景
	FISHDISPLAYTYPE_OV180		= 0x0004,		///< 180度全景
	FISHDISPLAYTYPE_OV360		= 0x0008,		///< 360度全景
	FISHDISPLAYTYPE_OV_3PTZ		= 0x0010,		///< 全景+3PTZ
	FISHDISPLAYTYPE_OV_8PTZ		= 0x0020,		///< 全景+8PTZ
	FISHDISPLAYTYPE_OV360_PTZ	= 0x0040,		///< 360度全景+PTZ 
	FISHDISPLAYTYPE_OV360_3PTZ  = 0x0080,		///< 360度全景+3PTZ
	FISHDISPLAYTYPE_OV360_6PTZ  = 0x0100,		///< 360度全景+6PTZ
	FISHDISPLAYTYPE_OV360_8PTZ  = 0x0200,		///< 360度全景+8PTZ
	FISHDISPLAYTYPE_2PTZ		= 0x0400,		///< 2PTZ
	FISHDISPLAYTYPE_4PTZ		= 0x0800,		///< 4PTZ
	FISHDISPLAYTYPE_FE_3PTZ		= 0x1000,		///< 鱼眼+3PTZ
	FISHDISPLAYTYPE_FE_8PTZ		= 0x2000,		///< 鱼眼+8PTZ
};

enum EIspNeedMask
{
	ISPNEEDMASK_IMAGEPARAM		= 0x00000001,		///< 图像参数
	ISPNEEDMASK_GAIN			= 0x00000002,		///< 增益参数
	ISPNEEDMASK_IRIS			= 0x00000004,		///< 光圈参数
	ISPNEEDMASK_FOCUS			= 0x00000008,		///< 聚焦参数
	ISPNEEDMASK_SHUTTER			= 0x00000010,		///< 快门参数
	ISPNEEDMASK_FREQMODE		= 0x00000020,		///< 防闪烁模式参数
	ISPNEEDMASK_WHITEBALANCE	= 0x00000040,		///< 白平衡参数
	ISPNEEDMASK_DAYNIGHTSWITCH	= 0x00000080,		///< 日夜转换参数
	ISPNEEDMASK_DENOISE2D		= 0x00000100,		///< 2D降噪参数
	ISPNEEDMASK_DENOISE3D		= 0x00000200,		///< 3D降噪参数
	ISPNEEDMASK_DYNAMICREGULATE	= 0x00000400,		///< 动态调节参数
	ISPNEEDMASK_IMAGEENHANCE	= 0x00000800,		///< 图像增强参数
	ISPNEEDMASK_STABLIZER		= 0x00001000,		///< 防抖参数
	ISPNEEDMASK_COMBINHDR		= 0x00002000,		///< 合成宽动态参数
	ISPNEEDMASK_DOUBLELENSMODE	= 0x00004000,		///< 二倍镜模式参数
	ISPNEEDMASK_EXPALLAUTO		= 0x00008000,		///< 曝光全自动模式参数
	ISPNEEDMASK_PROEXPOSURE		= 0x00010000,		///< 高级自动曝光参数
	ISPNEEDMASK_SCENEMODE		= 0x00020000,		///< 场景模式参数
	ISPNEEDMASK_FLIPANDPLAYBACK	= 0x00040000,		///< 翻转和回显参数
	ISPNEEDMASK_INFRARED		= 0x00080000,		///< 红外灯配置参数
	ISPNEEDMASK_LASER			= 0x00100000,		///< 激光配置参数
	ISPNEEDMASK_LDC				= 0x00200000,		///< 畸变矫正配置参数
};

enum EIspGainMode
{
	ISPGAINMODE_MANUAL			= 0x0001,		///< 手动增益模式
	ISPGAINMODE_AUTO			= 0x0002,		///< 自动增益模式
};

enum EIspIrisMode
{
	ISPIRISMODE_DC_IRISAUTO		= 0x0001,		///< DC-IRIS自动
	ISPIRISMODE_DC_IRISMANUAL	= 0x0002,		///< DC-IRIS手动
	ISPIRISMODE_P_IRISMANUAL	= 0x0004,		///< P-IRIS手动
	ISPIRISMODE_P_IRISAUTO		= 0x0008,		///< P-IRIS自动
};

enum EIspFocusMode
{
	ISPFOCUSMODE_MANUAL			= 0x0001,		///< 手动聚焦
	ISPFOCUSMODE_CONTINU_AUTO	= 0x0002,		///< 连续自动聚焦
	ISPFOCUSMODE_SINGLE_AUTO	= 0x0004,		///< 单次自动聚焦
	ISPFOCUSMODE_LOCK			= 0x0008,		///< 镜头锁定
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
	ISPSHUTTERMODE_MANUAL		= 0x0001,		///< 手动快门模式
	ISPSHUTTERMODE_AUTO			= 0x0002,		///< 自动快门模式
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
	ISPFREQMODE_AUTO		= 0x0001,		///< 自然光
	ISPFREQMODE_50HZ		= 0x0002,		///< 50hz
	ISPFREQMODE_60HZ		= 0x0004,		///< 60hz
};

enum EIspWhiteBalance
{
	WHITEBALANCE_MANUAL				= 0x0001,		///< 手动白平衡
	WHITEBALANCE_AUTO1				= 0x0002,		///< 自动白平衡1
	WHITEBALANCE_AUTO2				= 0x0004,		///< 自动白平衡2
	WHITEBALANCE_LOCK				= 0x0008,		///< 锁定白平衡
	WHITEBALANCE_FLUORESCENT		= 0x0010,		///< 日光灯模式
	WHITEBALANCE_INCANDESCENT		= 0x0020,		///< 白炽灯模式
	WHITEBALANCE_WARMLIGHT			= 0x0040,		///< 暖光灯模式
	WHITEBALANCE_SUNLIGHT			= 0x0080,		///< 自然光模式
	WHITEBALANCE_COLDLIGHT			= 0x0100,		///< 冷光灯模式
	WHITEBALANCE_OUTDOORDAYLIGHT	= 0x0200,		///< 室外白天模式
	WHITEBALANCE_OUTDOORCLOUDY		= 0x0400,		///< 室外多云模式
	WHITEBALANCE_OUTDOORNIGHT		= 0x0800,		///< 室外夜晚模式
	WHITEBALANCE_SHADOW				= 0x1000,		///< 阴影模式
};

enum EIspDayNightSwitch
{
	ISPDAYNIGHTSWITCH_DAY			= 0x0001,		///< 白天模式
	ISPDAYNIGHTSWITCH_NIGHT			= 0x0002,		///< 夜晚模式
	ISPDAYNIGHTSWITCH_AUTO_GAIN		= 0x0004,		///< 增益自动模式
	ISPDAYNIGHTSWITCH_TIME			= 0x0008,		///< 定时模式
	ISPDAYNIGHTSWITCH_TRIGGER		= 0x0010,		///< 告警触发模式
	ISPDAYNIGHTSWITCH_AUTO_LADR		= 0x0020,		///< 光敏自动
};

enum EIspDenoiseMode
{
	ISPDENOISEMODE_CLOSE		= 0x0001,		///< 关闭
	ISPDENOISEMODE_OPEN			= 0x0002,		///< 开启
	ISPDENOISEMODE_AUTO			= 0x0004,		///< 自动
};

enum EIspDynamicRegulateMode
{
	ISPDYNAMICREGULATEMODE_CLOSE		= 0x0001,		///< 关闭
	ISPDYNAMICREGULATEMODE_AUTO			= 0x0002,		///< 自动背光补偿模式
	ISPDYNAMICREGULATEMODE_SLC			= 0x0004,		///< 强光抑制模式
	ISPDYNAMICREGULATEMODE_REGION		= 0x0008,		///< 手动背光补偿模式
	ISPDYNAMICREGULATEMODE_WDR			= 0x0010,		///< 宽动态
	ISPDYNAMICREGULATEMODE_SMARTIR		= 0x0020,		///< 
};

enum EIspBlcRegionType
{
	ISPBLCREGIONTYPE_UP						= 0x0001,		///< 区域上
	ISPBLCREGIONTYPE_DOWN					= 0x0002,		///< 区域下
	ISPBLCREGIONTYPE_LEFT					= 0x0004,		///< 区域左
	ISPBLCREGIONTYPE_RIGHT					= 0x0008,		///< 区域右
	ISPBLCREGIONTYPE_CENTER					= 0x0010,		///< 区域中
	ISPBLCREGIONTYPE_CUSTOM					= 0x0020,		///< 自定义
};

enum EIspImageEnhanceMode
{
	ISPIMAGEENHANCEMODE_CLOSE			= 0x0001,		///< 关闭
	ISPIMAGEENHANCEMODE_ADAPTIVE		= 0x0002,		///< 自适应gamma
	ISPIMAGEENHANCEMODE_DEHAZE			= 0x0004,		///< 数字去雾
	ISPIMAGEENHANCEMODE_WDR				= 0x0008,		///< 数字宽动态
	ISPIMAGEENHANCEMODE_DEHAZE_OPTICS	= 0x0010,		///< 光学透雾
};

enum EIspBasicMode
{
	ISPBASICMODE_CLOSE			= 0x0001,		///< close
	ISPBASICMODE_OPEN			= 0x0002,		///< open
	ISPBASICMODE_AUTO			= 0x0004,		///< auto
};

enum EIspCombinHdrMode
{
	ISPCOMBINHDRMODE_CLOSE			= 0x0001,		///< 关闭
	ISPCOMBINHDRMODE_MODE1			= 0x0002,		///< 一帧合成宽动态
	ISPCOMBINHDRMODE_MODE2			= 0x0004,		///< 两帧合成宽动态
	ISPCOMBINHDRMODE_MODE3			= 0x0008,		///< 三帧合成宽动态
};

enum EIspSceneMode
{
	ISPSCENEMODE_STANDARD			= 0x0001,		///< 标准场景模式
	ISPSCENEMODE_COURT				= 0x0002,		///< 法院场景模式
	ISPSCENEMODE_DYNAMIC			= 0x0004,		///< 动态模式
	ISPSCENEMODE_USER_SAVE			= 0x0008,		///< 参数另存为模式
	ISPSCENEMODE_USER_LOAD			= 0x0010,		///< 用户自定义模式
	ISPSCENEMODE_USER_DEL			= 0x0020,		///< 参数删除
	ISPSCENEMODE_VEHICLE			= 0x0040,		///< 车牌模式
	ISPSCENEMODE_SPEED_20_30		= 0x0080,		///< 车牌模式20-30km
	WHITEBALANCE_SPEED_30_40		= 0x0100,		///< 车牌模式30-40km
	WHITEBALANCE_SPEED_40_50		= 0x0200,		///< 车牌模式40-50km
	WHITEBALANCE_SPEED_50_60		= 0x0400,		///< 车牌模式50-60km
	WHITEBALANCE_BLC				= 0x0800,		///< 背光场景模式
	WHITEBALANCE_FACE				= 0x1000,		///< 人脸场景模式
};

enum EIspSceneDynamicType
{
	ISPSCENEDYNAMICTYPE_DAYNIGHT				= 0x0001,		///< 日夜转换模式
	ISPSCENEDYNAMICTYPE_TIMELIST				= 0x0002,		///< 时间表
};

enum EIspFlipMode
{
	ISPFLIPMODE_AUTO					= 0x0001,		///< 自动
	ISPFLIPMODE_MANUAL					= 0x0002,		///< 手动
};

enum EIspMirrorMode
{
	ISPMIRRORMODE_CLOSE					= 0x0001,		///< 镜像关
	ISPMIRRORMODE_UP_DOWN				= 0x0002,		///< 上下
	ISPMIRRORMODE_LEFT_RIGHT			= 0x0004,		///< 左右
	ISPMIRRORMODE_CENTER				= 0x0008,		///< 中心
};

enum EIspRotateMode
{
	ISPROTATEMODE_NON			= 0x0001,		///< 不旋转
	ISPROTATEMODE_RIGHT			= 0x0002,		///< 右转
	ISPROTATEMODE_LEFT			= 0x0004,		///< 左转
};

enum EIspPlayBackMode
{
	ISPPLAYBACKMODE_CLOSE			= 0x0001,		///< 关闭
	ISPPLAYBACKMODE_CVBS_PAL		= 0x0002,		///< 回显CVBS P制
	ISPPLAYBACKMODE_CVBS_NTSC		= 0x0004,		///< 回显CVBS N制
	ISPPLAYBACKMODE_HDSDI_P			= 0x0008,		///< 回显SDI 逐行
	ISPPLAYBACKMODE_HDSDI_I			= 0x0010,		///< 回显SDI 隔行
	ISPPLAYBACKMODE_HDMI			= 0x0020,		///< 回显HDMI
};

enum EIspInfaredMode
{
	ISPINFAREDMODE_OPEN				= 0x0001,		///< 自动开启
	ISPINFAREDMODE_MORE_NEAR		= 0x0002,		///< 超近灯模式
	ISPINFAREDMODE_NEAR				= 0x0004,		///< 近灯模式
	ISPINFAREDMODE_MID				= 0x0008,		///< 中灯模式
	ISPINFAREDMODE_FAR				= 0x0010,		///< 远灯模式
	ISPINFAREDMODE_CLOSE			= 0x0020,		///< 关闭
	ISPINFAREDMODE_MANUAL			= 0x0040,		///< 手动模式
};

enum EIspLaserMode
{
	ISPLASERMODE_DEFAULT				= 0x0001,		///< 默认模式
	ISPLASERMODE_SMALL					= 0x0002,		///< 光斑小模式
	ISPLASERMODE_LARGE					= 0x0004,		///< 光斑大模式
	ISPLASERMODE_AUTO					= 0x0008,		///< 光斑自动
	ISPLASERMODE_MANUAL					= 0x0010,		///< 光斑手动
};

enum EIspLaserCentradMode
{
	ISPLASERCENTRADMODE_AUTO				= 0x0001,		///< 自动
	ISPLASERCENTRADMODE_MANUAL				= 0x0002,		///< 手动
};

enum EDecMode
{
	DECMODE_REALTIME = 0,  ///< 实时
	DECMODE_BALANCE  = 1,  ///< 均衡
	DECMODE_SMOOTH   = 2,  ///< 流畅
};

enum ENetMode
{
	NETMODE_MULTIADDR    = 0x01,   ///< 多址设定
	NETMODE_FAULTTOL     = 0x02,   ///< 网络容错
	NETMODE_LOADBAL      = 0x04,   ///< 负载均衡
};

enum ENetCardSpeed
{
	NETCARDSPEED_AUTO    = 0x01,   ///< 自适应
	NETCARDSPEED_10M     = 0x02,   ///< 10M
	NETCARDSPEED_100M    = 0x04,   ///< 100M
	NETCARDSPEED_1000M   = 0x08,   ///< 1000M
};

enum EUPNPMODE
{
	UPNPMODE_AUTO   = 0x01, ///< 自动
	UPNPMODE_MANUAL = 0x02, ///< 手动
};

enum EDDNSType
{
	DDNSTYPE_OARY			= 0x01, ///< 花生壳
	DDNSTYPE_DYNDNS			= 0x02, ///< DynDNS
	DDNSTYPE_DYNIP			= 0x04, ///< DynIP
	DDNSTYPE_NOIP			= 0x08, ///< noip
	DDNSTYPE_CHANGEIP		= 0x10, ///< ChangeIP
	DDNSTYPE_EASYDNS		= 0x20, ///< easyDNS
	DDNSTYPE_CAMTRONICS		= 0x40, ///< CAMTRONICS
};

enum EPORTTYPE
{
	PORTTYPE_HTTP = 0, ///< HTTP端口
	PORTTYPE_RTSP = 1, ///< RTSP端口
};

enum EWifiMode
{
	WIFIMODE_AP			= 0,	///< AP模式
	WIFIMODE_WLAN		= 1,	///< WLAN模式
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
	AUTOLOGOUTTIME_NEVER = 0x01,   ///< 从不
	AUTOLOGOUTTIME_1MIN  = 0x02,   ///< 1分钟
	AUTOLOGOUTTIME_2MIN  = 0x04,   ///< 2分钟
	AUTOLOGOUTTIME_5MIN  = 0x08,   ///< 5分钟
	AUTOLOGOUTTIME_10MIN = 0x10,   ///< 10分钟
	AUTOLOGOUTTIME_20MIN = 0x20,   ///< 20分钟
	AUTOLOGOUTTIME_30MIN = 0x40,   ///< 30分钟
};

enum EAudioListen
{
	AUDIOLISTEN_NONE = 0,	///< 音频收听无
	AUDIOLISTEN_CHN1 = 1,	///< 音频收听通道1
	AUDIOLISTEN_CHN2 = 2,	///< 音频收听通道2
	AUDIOLISTEN_ALL			///< 音频收听全部
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
	USERLEVEL_ADMIN		= 0, ///< 管理员
	USERLEVEL_OPERATOR	= 1, ///< 操作员
	USERLEVEL_VIEWER	= 2, ///< 浏览者
};

enum EUserInfoType
{
	USERINFOTYPE_BASEINFO		   = 0x01, ///< 基本信息
	USERINFOTYPE_REMOTECTRL        = 0x02, ///< 远程控制
	USERINFOTYPE_SYSPERM           = 0x04, ///< 系统权限
	USERINFOTYPE_CHNPERMLIST       = 0x08, ///< 通道权限
};

enum ESysPermType
{
	SYSPERMTYPE_ADMIN              = 0x0001, ///< admin权限
	SYSPERMTYPE_RECSNAP            = 0x0002, ///< 录像抓拍权限
	SYSPERMTYPE_CHNMGR             = 0x0004, ///< 通道管理权限
	SYSPERMTYPE_NETMGR             = 0x0008, ///< 网络管理权限
	SYSPERMTYPE_SYSMGR             = 0x0010, ///< 系统管理权限
	SYSPERMTYPE_DISKMGR            = 0x0020, ///< 硬盘管理权限
	SYSPERMTYPE_GUIPLANMGR         = 0x0040, ///< GUI预案管理权限
	SYSPERMTYPE_SHUTDOWN           = 0x0080, ///< 关机重启管理权限
	SYSPERMTYPE_SCENARIO           = 0x0100, ///< 应用场景管理权限
	SYSPERMTYPE_CAMERA			   = 0x0200, ///< 摄像机管理权限
	SYSPERMTYPE_EVENT			   = 0x0400, ///< 事件管理权限
	SYSPERMTYPE_INTELL			   = 0x0800, ///< 智能管理权限
};

enum EChnPermType
{
	CHNPERMTYPE_VIEW              = 0x01, ///< 浏览权限
	CHNPERMTYPE_LT                = 0x02, ///< 监听对讲权限
	CHNPERMTYPE_PB                = 0x04, ///< 回放权限
	CHNPERMTYPE_BD                = 0x08, ///< 备份下载权限
	CHNPERMTYPE_IS                = 0x10, ///< 智能搜索权限
	CHNPERMTYPE_PTZ               = 0x20, ///< 云台权限
};

enum ERtspAuthType
{
	RTSPAUTHTYPE_NONE		= 0x01, ///< none:无需鉴权
	RTSPAUTHTYPE_BASIC   	= 0x02, ///< basic:BASIC方式
	RTSPAUTHTYPE_DIGEST 	= 0x04, ///< digest:摘要认证方式
};

enum EIpAddrFilterType
{
	IPADDRFILTERTYPE_WHITE		= 0x01, ///< 黑名单过滤
	IPADDRFILTERTYPE_BLACK   	= 0x02, ///< 白名单过滤
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
	CORDONTYPE_ATOB   = 0x01, ///< 警戒线方向，A->B
	CORDONTYPE_BTOA   = 0x02, ///< 警戒线方向，B->A
	CORDONTYPE_AANDB  = 0x04, ///< 警戒线方向，A<->B
};

enum ECheckMode
{
	CHECKMODE_MEDIA  = 0x01, ///< 软件检测
	CHECKMODE_MAG    = 0x02, ///< 硬件检测
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
	RECMODETYPR_AUTO              = 0x01, ///< 按计划和事件
	RECMODETYPR_START             = 0x02, ///< 始终开启
	RECMODETYPR_STOP              = 0x04, ///< 始终关闭
};

enum ESnapModeType
{
	SNAPMODETYPE_AUTO             = 0x01,  ///< 自动（按计划和事件）
	SNAPMODETYPE_STOP             = 0x02,  ///< 停止
};

enum ERecStreamType
{
	RECSTREAMTYPE_MAIN            = 0x01, ///< 主流
	RECSTREAMTYPE_2ND             = 0x02, ///< 辅流
	RECSTREAMTYPE_3RD             = 0x04, ///< 第三路流
};

enum ENetPackFilter
{
	NETPACK_NOFILTER  = 0, ///< 全部
	NETPACK_TCPFILTER = 1, ///< TCP包
	NETPACK_UDPFILTER = 2, ///< UDP包
};

enum ERecDayType
{
	RECDAYTYPE_MONDAY       = 0, ///< 周一
	RECDAYTYPE_TUESDAY      = 1, ///< 周二
	RECDAYTYPE_WEDNESDAY    = 2, ///< 周三
	RECDAYTYPE_THURSDAY     = 3, ///< 周四
	RECDAYTYPE_FRIDAY       = 4, ///< 周五
	RECDAYTYPE_SATURDAY     = 5, ///< 周六
	RECDAYTYPE_SUNDAY       = 6, ///< 周日
	RECDAYTYPE_HOLIDAY      = 7, ///< 假日
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
	HOLIDAYSELMODE_DAY    = 0x01,   ///< 日
	HOLIDAYSELMODE_WEEK   = 0x02,   ///< 周
	HOLIDAYSELMODE_MON    = 0x04,   ///< 月
};

enum EPlaybackType
{
	PLAYBACKTYPE_EVENTINDEX   = 1,  ///< event_index:事件检索回放
	PLAYBACKTYPE_LABELINDEX   = 2,  ///< label_index:标签检索回放
	PLAYBACKTYPE_LOCKED       = 3,  ///< locked:锁定录像回放
	PLAYBACKTYPE_IMAGEINDEX   = 4,  ///< image_index:图片检索回放
	PLAYBACKTYPE_EXTERNALFILE = 5,  ///< external_file:外部文件检索回放
	PLAYBACKTYPE_TIMESCAL     = 6,  ///< time_scale:时间段回放
	PLAYBACKTYPE_BYFILE       = 7,  ///< by_file:按文件回放
	PLAYBACKTYPE_BYTIME       = 8,  ///< by_time:按时间回放
};

enum ERecEventType
{
	RECEVENTTYPE_RECORD           = 0x00000000, ///< 记录,非事件
	RECEVENTTYPE_ALL              = 0x00000001, ///< all_event,全部事件
	RECEVENTTYPE_PIN              = 0x00000002, ///< pin,并口告警，报警输入
	RECEVENTTYPE_MOTIVE           = 0x00000004, ///< motive,移动侦测告警
	//RECEVENTTYPE_EXTERN           = 0x00000008, ///< extern,外部告警录像,类型搜索查询时不考虑 [10/28/2016 刘飞]
	RECEVENTTYPE_MANUAL           = 0x00000010, ///< manual,手动录像
	RECEVENTTYPE_TIMER            = 0x00000020, ///< timer,定时录像
	//RECEVENTTYPE_WEEKLY           = 0x00000040, ///< weekly,周期录像,类型搜索查询时不考虑 [10/28/2016 刘飞]
	//RECEVENTTYPE_FILE             = 0x00000080, ///< file,定长时间的文件录像,类型搜索查询时不考虑 [10/28/2016 刘飞]
	RECEVENTTYPE_INTEDETECT       = 0x10000000, ///< 智能侦测，不对应具体查询类型，只是主类型之一
	RECEVENTTYPE_COVERIMG         = 0x00000100, ///< cover_image,图像遮挡告警
	RECEVENTTYPE_TRIPLINE         = 0x00000200, ///< trip_line,拌线检测
	RECEVENTTYPE_DEFOCUS          = 0x00000400, ///< defocus,虚焦检测
	RECEVENTTYPE_SCENECHANGE      = 0x00000800, ///< scene_change,场景变换
	RECEVENTTYPE_REGIONINVASION   = 0x00001000, ///< region_invasion,区域入侵检测
	RECEVENTTYPE_REGIONLEAVING    = 0x00002000, ///< region_leaving,区域离开检测
	RECEVENTTYPE_OBJTAKEN         = 0x00004000, ///< object_taken,物品拿去检测
	RECEVENTTYPE_OBJLEFT          = 0x00008000, ///< object_left,物品遗留检测
	RECEVENTTYPE_PEOPLEGATHER     = 0x00010000, ///< people_gather,人员聚集检测
	RECEVENTTYPE_AUDIOABNORMAL    = 0x00020000, ///< audio_abnormal,声音异常
	RECEVENTTYPE_DETECTIN         = 0x00040000, ///< region_enter,区域进入
	RECEVENTTYPE_FACEDETECTION    = 0x00080000, ///< face_detect,人脸检测
	RECEVENTTYPE_IMPPERSON		  = 0x00100000, ///< imp_person,重点人员
	RECEVENTTYPE_STRANGEPERSON	  = 0x00200000, ///< strange_person,陌生人员
	RECEVENTTYPE_PERSONDETECT	  = 0x00400000, ///< person_detect,行人检测
	RECEVENTTYPE_ACCESSPROTECT	  = 0x00800000, ///< access_protect,防拆报警
	RECEVENTTYPE_LOCKED           = 0x20000000, ///< locked,锁定录像事件
	RECEVENTTYPE_UNLOCKED         = 0x40000000, ///< unlocked,未锁定录像事件
};

enum ERecBackupType
{
	RECBACKUPTYPE_BACKUP    = 1,   ///< 录像备份,rec_backup
	RECBACKUPTYPE_CLIP      = 2,   ///< 录像剪辑,rec_clip
};

enum EBackupState
{
	BACKUPSTATE_IDLE    = 1,   ///< 空闲，idle
	BACKUPSTATE_RUN     = 2,   ///< 运行，run
	BACKUPSTATE_FAIL    = 3,   ///< 失败，fail
	BACKUPSTATE_DONE    = 4,   ///< 完成，done
	BACKUPSTATE_UNKNOWN = 5,   ///< 获取备份状态失败，unknown
};

enum EPlaybackVcrCmdType
{
	PLAYBACKVCRCMDTYPE_NORMAL      = 1,  ///< normal:常速播放
	PLAYBACKVCRCMDTYPE_PAUSE       = 2,  ///< pause:暂停
	PLAYBACKVCRCMDTYPE_RESUME      = 3,  ///< resume:恢复播放
	PLAYBACKVCRCMDTYPE_FAST2X      = 4,  ///< 2xfast:2倍速快放
	PLAYBACKVCRCMDTYPE_FAST4X      = 5,  ///< 4xfast:4倍速快放
	PLAYBACKVCRCMDTYPE_FAST8X      = 6,  ///< 8xfast:8倍速快放
	PLAYBACKVCRCMDTYPE_FAST16X     = 7,  ///< 16xfast:16倍速快放
	PLAYBACKVCRCMDTYPE_FAST32X     = 8,  ///< 32xfast:16倍速快放
	PLAYBACKVCRCMDTYPE_FAST64X     = 9,  ///< 64xfast:16倍速快放
	PLAYBACKVCRCMDTYPE_SLOW2X      = 10, ///< 2xslow:1/2倍速慢放
	PLAYBACKVCRCMDTYPE_SLOW4X      = 11, ///< 4xslow:1/4倍速慢放
	PLAYBACKVCRCMDTYPE_SLOW8X      = 12, ///< 8xslow:1/8倍速慢放
	PLAYBACKVCRCMDTYPE_SLOW16X     = 13, ///< 16xslow:16倍速慢放
	PLAYBACKVCRCMDTYPE_FRAME       = 14, ///< frame:单帧播放
	PLAYBACKVCRCMDTYPE_DRAG        = 15, ///< drag:时间轴拖拽
	PLAYBACKVCRCMDTYPE_SKIPF       = 16, ///< skipf:前跳
	PLAYBACKVCRCMDTYPE_SKIPB       = 17, ///< skipb:后跳
};

enum EPlaybackFrameMode
{
	PLAYBACKFRAMEMODE_ALL          = 1, ///< 发送所有类型的帧数据 I/P/B帧
	PLAYBACKFRAMEMODE_INTRA        = 2, ///< 只发送关键帧 I帧
	PLAYBACKFRAMEMODE_PREDICTED    = 3, ///< 只发送关键帧和预测帧 I/P帧
};

enum EPlaybackVcrState
{
	PLAYBACKSTATE_PLAY                       = 1, ///< play:正在播放
	PLAYBACKSTATE_OVER                       = 2, ///< over:播放结束
	PLAYBACKSTATE_DISCON                     = 3, ///< discon:断链
	PLAYBACKSTATE_NOTSTART                   = 4, ///< notstart:未开始
	PLAYBACKSTATE_PAUSE                      = 5, ///< pause:放像暂停
	PLAYBACKSTATE_SINGLE                     = 6, ///< single:单帧
	PLAYBACKSTATE_START_FAILED               = 7, ///< allfail:放像全部失败
	PLAYBACKSTATE_START_PARTIAL_FAILED       = 8, ///< partfail:放像部分失败
	PLAYBACKSTATE_CHN_NO_STREAM              = 9, ///< chn_no_stream:当前通道没有码流
};

enum ESnapPicEventType
{
	SNAPPICEVENTTYPE_ALL						= 0x01000000, ///< 所有类型，all_event
	SNAPPICEVENTTYPE_MANUAL						= 0x02000000, ///< 手动抓拍，manual
	SNAPPICEVENTTYPE_TIMER						= 0x04000000, ///< 定时抓拍，timer
	SNAPPICEVENTTYPE_MD							= 0x08000000, ///< 移动侦测，md
	SNAPPICEVENTTYPE_PIN						= 0x10000000, ///< 告警输入，pin
	SNAPPICEVENTTYPE_INTEDETECT					= 0x20000000, ///< 智能侦测
	SNAPPICEVENTTYPE_INTEDETECT_ALL				= 0x20000001, ///< 智能侦测，全部
	SNAPPICEVENTTYPE_INTEDETECT_TRIPLINE		= 0x20000002, ///< 智能侦测，警戒线穿越，trip_line
	SNAPPICEVENTTYPE_INTEDETECT_REGIONINVASION	= 0x20000004, ///< 智能侦测，区域入侵，region_invasion
	SNAPPICEVENTTYPE_INTEDETECT_REGINENTER		= 0x20000008, ///< 智能侦测，进入区域，region_entering
	SNAPPICEVENTTYPE_INTEDETECT_REGIONLEAVE		= 0x20000010, ///< 智能侦测，离开区域，region_leaving
	SNAPPICEVENTTYPE_INTEDETECT_OBJLEFT			= 0x20000020, ///< 智能侦测，物品遗留，object_left
	SNAPPICEVENTTYPE_INTEDETECT_OBJTAKEN		= 0x20000040, ///< 智能侦测，物品拿取，object_taken
	SNAPPICEVENTTYPE_INTEDETECT_PEOPLEGATHER	= 0x20000080, ///< 智能侦测，人员聚集，people_gather
	SNAPPICEVENTTYPE_INTEDETECT_DETECTFACE		= 0x20000100, ///< 智能侦测，人脸检测，detect_face
	SNAPPICEVENTTYPE_INTEDETECT_SHADE			= 0x20000200, ///< 智能侦测，遮挡检测，shade
	SNAPPICEVENTTYPE_INTEDETECT_DEFOCUS			= 0x20000400, ///< 智能侦测，虚焦检测，defocus
	SNAPPICEVENTTYPE_INTEDETECT_SCENECHANGE		= 0x20000800, ///< 智能侦测，场景变化，scene_change
	SNAPPICEVENTTYPE_INTEDETECT_AUDIOABNORMAL	= 0x20001000, ///< 智能侦测，音频异响，audio_abnormal
	SNAPPICEVENTTYPE_INTEDETECT_IMPPERSON		= 0x20002000, ///< 智能侦测，重点人员，imp_person
	SNAPPICEVENTTYPE_INTEDETECT_STRANGEPERSON	= 0x20004000, ///< 智能侦测，陌生人员，strange_person
	SNAPPICEVENTTYPE_INTEDETECT_PERSONDETECT	= 0x20008000, ///< 智能侦测，行人检测，person_detect
	SNAPPICEVENTTYPE_INTEDETECT_ACCESSPROTECT	= 0x20010000, ///< 智能侦测，防拆报警，access_protect
};

enum ERecTagOperaType
{
	TAGOPERATE_ADD       = 0x0001, ///< 添加录像标签
	TAGOPERATE_DEL       = 0x0002, ///< 删除录像标签
	TAGOPERATE_UPDATE    = 0x0004, ///< 更新录像标签
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
	OUTPUTTYPE_HDMI   = 0x0001, ///< 输出类型HDMI
	OUTPUTTYPE_HDMI2  = 0x0002, ///< 输出类型HDMI2
	OUTPUTTYPE_VGA    = 0x0004, ///< 输出类型VGA
	OUTPUTTYPE_CVBS   = 0x0010, ///< 输出类型CVBS
	OUTPUTTYPE_BT1120 = 0x0020, ///< 输出类型BT1120
};

enum ESerialType
{
	SERIALTYPE_RS232 = 1,    ///< RS232
	SERIALTYPE_RS485 = 2,    ///< RS485
};

enum ESerialBaudRate
{
	SERIALBAUDRATE_2400   = 0x00000001,  ///< 2400, 2400波特率
	SERIALBAUDRATE_4800   = 0x00000002,  ///< 4800, 4800波特率
	SERIALBAUDRATE_9600   = 0x00000004,  ///< 9600, 9600波特率
	SERIALBAUDRATE_19200  = 0x00000008,  ///< 19200, 19200波特率
	SERIALBAUDRATE_38400  = 0x00000010,  ///< 38400, 38400波特率
	SERIALBAUDRATE_57600  = 0x00000020,  ///< 57600, 57600波特率
	SERIALBAUDRATE_115200 = 0x00000040,  ///< 115200, 115200波特率
};

enum ESerialDataBit
{
	SERIALDATABIT_5 = 0x00000001,  ///< databit5, 数据位: 5位
	SERIALDATABIT_6 = 0x00000002,  ///< databit6, 数据位: 6位
	SERIALDATABIT_7 = 0x00000004,  ///< databit7, 数据位: 7位
	SERIALDATABIT_8 = 0x00000008,  ///< databit8, 数据位: 8位
};

enum ESerialStopBit
{
	SERIALSTOPBIT_1 = 0x00000001,  ///< stopbit1, 停止位: 1位
	SERIALSTOPBIT_2 = 0x00000002,  ///< stopbit2, 停止位: 2位
};

enum ESerialParityBit
{
	SERIALPARITYBIT_NONE = 0x00000001,  ///< none, 校验位: 无校验
	SERIALPARITYBIT_ODD  = 0x00000002,  ///< odd, 校验位: 奇校验
	SERIALPARITYBIT_EVEN = 0x00000004,  ///< even, 校验位: 偶校验
};

enum ESerialFlowCtrl
{
	SERIALFLOWCTRL_NONE     = 0x00000001,  ///< none, 流控: 无
	SERIALFLOWCTRL_SOFT     = 0x00000002,  ///< soft, 流控: 软流控
	SERIALFLOWCTRL_HARDWARE = 0x00000004,  ///< hardware, 流控: 硬流控
};

enum ESerialWorkType
{
	SERIALWORKTYPE_KEYBOARD      = 0x00000001,  ///< keyboard, 串口功能: 监控硬盘
	SERIALWORKTYPE_ALARMEXTCARD  = 0x00000002,  ///< alarmextcard, 串口功能: 告警扩展卡
	SERIALWORKTYPE_TRANSDATA     = 0x00000004,  ///< transdata, 串口功能: 透明数据
};

enum EDisMode
{
	DISMODE_STANDARD = 0, ///< 标准
	DISMODE_BRIGHT   = 1, ///< 明亮
	DISMODE_SOFT     = 2, ///< 柔和
	DISMODE_FRESH    = 3, ///< 鲜艳
	DISMODE_CUSTOM   = 4, ///< 自定义
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
	DECSTATE_NORMAL				= 0, ///< 解码正常
	DECSTATE_DECCAPOVER			= 1, ///< 超出解码能力
	DECSTATE_ZOOMCAPOVER		= 2, ///< 超出缩放能力
	DECSTATE_STREAMPREFAILED	= 3, ///< 取流失败
};

enum ELogMainType
{
	LOGMAINTYPE_ALL                                          = 0x10000000, ///< 全部
	LOGMAINTYPE_ALARM                                        = 0x20000000, ///< 告警
	LOGMAINTYPE_SYSEXEPTION                                  = 0x40000000, ///< 系统异常
	LOGMAINTYPE_USEROPERATE                                  = 0x80000000, ///< 用户操作
	LOGMAINTYPE_SYSINFO		                                 = 0x01000000, ///< 系统信息
};

enum ERealLogMainType
{
	REALLOGMAINTYPE_NVRALARM                                 = 0x21000000, ///< NVR告警
	REALLOGMAINTYPE_DEVALARM                                 = 0x22000000, ///< 前端告警
	REALLOGMAINTYPE_SYSEXEPTION                              = 0x40000000, ///< 系统异常
	REALLOGMAINTYPE_USEROPERATE                              = 0x80000000, ///< 用户操作
	REALLOGMAINTYPE_SYSINFO		                             = 0x00100000, ///< 系统信息
};

enum EAlarmType
{
	ALARMTYPE_ALARMIN	    = 1, ///< 告警输入
	ALARMTYPE_MOVEDETECT    = 2, ///< 移动侦测
	ALARMTYPE_SMARTDETECT   = 3, ///< 智能侦测
	ALARMTYPE_VIDEOLOST	    = 4, ///< 视频丢失
	ALARMTYPE_ACCESSPROTECT	= 5, ///< 防拆报警
	ALARMTYPE_SYSALARM      = 6, ///< 系统告警
};

enum ESmartAlarmType
{
	SMARTALARMTYPE_WARNINGLINE		= 1,  ///< 警戒线
	SMARTALARMTYPE_AREAINVADE		= 2,  ///< 区域入侵
	SMARTALARMTYPE_ENTERAREA		= 3,  ///< 区域进入
	SMARTALARMTYPE_LEAVEATEA		= 4,  ///< 区域离开
	SMARTALARMTYPE_SHIELDDETECT		= 5,  ///< 遮挡检测
	SMARTALARMTYPE_GOODSTAKE		= 6,  ///< 物品拿取
	SMARTALARMTYPE_GOODSLEFT		= 7,  ///< 物品遗留
	SMARTALARMTYPE_VIRFOCUS			= 8,  ///< 虚焦检测
	SMARTALARMTYPE_GATHER			= 9,  ///< 人员聚集
	SMARTALARMTYPE_SCENECHANGE		= 10, ///< 场景变更
	SMARTALARMTYPE_AUDIOABNORMAL	= 11, ///< 声音异常
	SMARTALARMTYPE_FACEDETECT		= 12, ///< 人脸检测
};

enum ESysAlarmType
{
	SYSALARMTYPE_DISKFAULT		= 0x0001, ///< 硬盘故障
	SYSALARMTYPE_DISKNONE		= 0x0002, ///< 无硬盘
	SYSALARMTYPE_RECFULL		= 0x0004, ///< 录象空间满
	SYSALARMTYPE_SNAPFULL		= 0x0008, ///< 抓拍空间满
	SYSALARMTYPE_CHNOFFLINE		= 0x0010, ///< 前端掉线
	SYSALARMTYPE_ILLACCESS		= 0x0020, ///< 非法访问
	SYSALARMTYPE_NETFAULT		= 0x0040, ///< 网络故障
	SYSALARMTYPE_IPCONFLICT		= 0x0080, ///< IP冲突
	SYSALARMTYPE_MACCONFLICT	= 0x0100, ///< MAC冲突
	SYSALARMTYPE_NOREC			= 0x0200, ///< 录像时长不足
};

enum EAlarmBasicLink
{
	ALARMBASICLINK_AUDIOALARM = 0x01, ///< 告警常规联动到声音报警
	ALARMBASICLINK_SENDMAIL   = 0x02, ///< 告警常规联动到发送邮件
	ALARMBASICLINK_UPCENTER   = 0x04, ///< 告警常规联动到上报中心
	ALARMBASICLINK_HDMI1      = 0x08, ///< 告警联动输出到HDMI1
	ALARMBASICLINK_HDMI2      = 0x10, ///< 告警联动输出到HDMI2
	ALARMBASICLINK_VGA1       = 0x20, ///< 告警联动输出到VAG1
	ALARMBASICLINK_VGA2       = 0x40, ///< 告警联动输出到VGA2
	ALARMBASICLINK_CLOUDSRV   = 0x80, ///< 告警联动输出到云服务
};

enum EAlarmLinkPtzType
{
	ALARMLINKPTZTYPE_NONE	= 0, ///< 无
	ALARMLINKPTZTYPE_PRESET	= 1, ///< 预置位
	ALARMLINKPTZTYPE_CRUISE	= 2, ///< 巡航
};

enum EDayType
{
	DAYTYPE_MONDAY		= 1, ///< 周一
	DAYTYPE_TUESDAY		= 2, ///< 周二
	DAYTYPE_WEDNESDAY	= 3, ///< 周三
	DAYTYPE_THURSDAY	= 4, ///< 周四
	DAYTYPE_FRIDAY		= 5, ///< 周五
	DAYTYPE_SATURDAY	= 6, ///< 周六
	DAYTYPE_SUNDAY		= 7, ///< 周日
	DAYTYPE_HOLIDAY		= 8, ///< 假日
};

enum EAlarmInType
{
	ALARMINTYPE_OPEN  = 1, ///< 常开    
	ALARMINTYPE_CLOSE = 2, ///< 常闭    
};

enum ESmartTestAttrStatus	
{
	SMARTTESTATTRSTATUS_OK	        = 1,			///< OK
	SMARTTESTATTRSTATUS_NOTOK		= 2,			///< NOT_OK
};

enum ESmartTestResultType	
{
	SMARTTESTRESULTTYPE_PASSED	    = 1,			///< 通过,状态良好
	SMARTTESTRESULTTYPE_WARNING		= 2,			///< 警告,即将损坏
	SMARTTESTRESULTTYPE_NOTPASSED	= 3,			///< 未通过,损坏
};

enum EBadSectorCheckType	
{
	BADSECTORCHECKTYPE_KEYAREA	    = 1,			///< 关键区检测
	BADSECTORCHECKTYPE_ALLAREA		= 2,			///< 完全检测
};

enum EBadSectorCheckState
{
	BADSECTORCHECKSTATE_NOTRUN	    = 1,			///< 未进行检测
	BADSECTORCHECKSTATE_RUNNING		= 2,			///< 正在检测中
	BADSECTORCHECKSTATE_PAUSE		= 3,			///< 暂停检测
	BADSECTORCHECKSTATE_ERROR		= 4,			///< 检测过程中出错
	BADSECTORCHECKSTATE_FINISHE		= 5,			///< 检测完成
};

enum ESipSecurityLev
{
	SIPSECURITYLEV_NO			= 0x01,			///< 不加密
	SIPSECURITYLEV_CLASS_A		= 0x02,			///< A类加密
	SIPSECURITYLEV_CLASS_B		= 0x04,			///< B类加密
	SIPSECURITYLEV_CLASS_C		= 0x08,			///< C类加密
};

enum EBadSectorCheckCtrlType	
{
	BADSECTORCHECKCTRLTYPE_PAUSE	    = 1,			///< 暂停
	BADSECTORCHECKCTRLTYPE_CONTINUE		= 2,			///< 继续
};

enum EPppoeStatus
{
	PPPOESTATUS_STOP		= 1, ///< 停止拨号
	PPPOESTATUS_LINKUP		= 2, ///< 拨号成功
	PPPOESTATUS_LINKDOWN	= 3, ///< 连接断开
	PPPOESTATUS_DAILING		= 4, ///< 正在拨号
	PPPOESTATUS_AUTHERR		= 5, ///< 用户校验失败
	PPPOESTATUS_TIMEOUT		= 6, ///< 拨号超时
	PPPOESTATUS_NOISP		= 7, ///< 找不到服务提供商
	PPPOESTATUS_SRVERR		= 8, ///< PPPoE服务端异常
};

enum ESmtpTestStatus
{
	SMTPTESTSTATUS_RUNNING    = 0x01,   ///< 任务运行中
	SMTPTESTSTATUS_DONE       = 0x02,   ///< 任务完成
};

enum EOnvifAuthType
{
	ONVIFAUTHTYPE_NONE		= 1, ///< none(无)
	ONVIFAUTHTYPE_DIGEST    	= 2, ///< digest(digest+WS-Username token方式)
};

enum EQRBmpType
{
	QRBMPTYPE_IOS		= 1, ///< ios二维码
	QRBMPTYPE_ANDROID	= 2, ///< android二维码
	QRBMPTYPE_SN		= 3, ///< sn二维码
};

enum ECloudStatus
{
	CLOUDSTATUS_NOTCON		= 1, ///< 未启动连接
	CLOUDSTATUS_CONNING		= 2, ///< 正在连接
	CLOUDSTATUS_CONSUC		= 3, ///< 连接成功
	CLOUDSTATUS_CONFAILED	= 4, ///< 连接失败
};

enum EPingReply
{
	PING_REPLY			= 1,			///< 正常
	PING_TIMEOUT		= 2,			///< 超时
	PING_ERROR			= 3,			///< 错误
	PING_UNREACHABLE	= 4,			///< 不可达
};

enum EChnRecState
{
	CHNRECSTATE_RECORDING			= 1,			///< 录象中
	CHNRECSTATE_RECORDSTOP		    = 2,			///< 停止
	CHNRECSTATE_RECORDCLOSE			= 3,			///< 关闭
};

enum ELogSubType
{
	LOGSUBTYPE_ALL_ALL                                       = 0x10000001, ///< 全部-全部

	LOGSUBTYPE_ALARM_ALL                                     = 0x20000001, ///< 告警-全部
	LOGSUBTYPE_ALARM_ALARMIN                                 = 0x20000002, ///< 告警-告警输入
	LOGSUBTYPE_ALARM_MOVING                                  = 0x20000004, ///< 告警-移动侦测
	LOGSUBTYPE_ALARM_VIDEOLOST                               = 0x20000008, ///< 告警-视频丢失
	LOGSUBTYPE_ALARM_WARNINGLINE                             = 0x20000010, ///< 告警-警戒线
	LOGSUBTYPE_ALARM_DETECT                                  = 0x20000020, ///< 告警-区域入侵
	LOGSUBTYPE_ALARM_DETECTIN                                = 0x20000040, ///< 告警-进入区域
	LOGSUBTYPE_ALARM_DETECTOUT                               = 0x20000080, ///< 告警-离开区域
	LOGSUBTYPE_ALARM_PROPERTYLOST                            = 0x20000100, ///< 告警-物品遗留
	LOGSUBTYPE_ALARM_PROPERTYTAKE                            = 0x20000200, ///< 告警-物品拿取
	LOGSUBTYPE_ALARM_PEOPLEGATHERING                         = 0x20000400, ///< 告警-人员聚集
	LOGSUBTYPE_ALARM_FACEDETECTION                           = 0x20000800, ///< 告警-人脸检测
	LOGSUBTYPE_ALARM_SHADE                                   = 0x20001000, ///< 告警-遮挡
	LOGSUBTYPE_ALARM_OUTOFFOCUS                              = 0x20002000, ///< 告警-虚焦
	LOGSUBTYPE_ALARM_SCENECHANGE                             = 0x20004000, ///< 告警-场景变化
	LOGSUBTYPE_ALARM_AUDIOEXCEPTION                          = 0x20008000, ///< 告警-音频异响
	LOGSUBTYPE_ALARM_GPSOVERSPEED                            = 0x20010000, ///< 告警-超速报警
	LOGSUBTYPE_ALARM_ACCESS_PROTECT                          = 0x20020000, ///< 告警-门禁设备防拆

	LOGSUBTYPE_SYSEXEPTION_ALL                               = 0x40000001, ///< 系统异常-全部
	LOGSUBTYPE_SYSEXEPTION_NETFAULT                          = 0x40000002, ///< 系统异常-网络故障
	LOGSUBTYPE_SYSEXEPTION_IPCONFLIT                         = 0x40000004, ///< 系统异常-IP冲突
	LOGSUBTYPE_SYSEXEPTION_MACCONFLIT                        = 0x40000008, ///< 系统异常-MAC冲突
	LOGSUBTYPE_SYSEXEPTION_MONITORDROPPED                    = 0x40000010, ///< 系统异常-监控点掉线
	LOGSUBTYPE_SYSEXEPTION_NORECDISK                         = 0x40000020, ///< 系统异常-无硬盘
	LOGSUBTYPE_SYSEXEPTION_DISKFAULT                         = 0x40000040, ///< 系统异常-硬盘故障
	LOGSUBTYPE_SYSEXEPTION_RECSPACEFULL                      = 0x40000080, ///< 系统异常-录像空间满
	LOGSUBTYPE_SYSEXEPTION_SNAPSPACEFULL                     = 0x40000100, ///< 系统异常-抓拍空间满
	LOGSUBTYPE_SYSEXEPTION_ILLEGAACCESS                      = 0x40000200, ///< 系统异常-非法访问
	LOGSUBTYPE_SYSEXEPTION_HOTBACKUP                         = 0x40000400, ///< 系统异常-热备异常    ***** 需求没有
	LOGSUBTYPE_SYSEXEPTION_MEDIASTREAMLOSTALL                = 0x40001000, ///< 系统异常-流媒体丢失
	LOGSUBTYPE_SYSEXEPTION_EXCEPTIONREBOOTALL                = 0x40002000, ///< 系统异常-掉电关机
	LOGSUBTYPE_SYSEXEPTION_RECALL							 = 0x40004000, ///< 系统异常-录像异常
	LOGSUBTYPE_SYSEXEPTION_NO_REC_RECORD_ALL				 = 0x40008000, ///< 系统异常-录像时长不足

	LOGSUBTYPE_USEROPRATE_ALL                                = 0x80000001, ///< 用户操作-全部
	LOGSUBTYPE_USEROPRATE_BOOTDEV                            = 0x80000002, ///< 用户操作-重启和开关设备
	LOGSUBTYPE_USEROPRATE_LOGIN                              = 0x80000004, ///< 用户操作-登录注销
	LOGSUBTYPE_USEROPRATE_BROWSE                             = 0x80000008, ///< 用户操作-开始停止浏览
	LOGSUBTYPE_USEROPRATE_PTZ                                = 0x80000010, ///< 用户操作-云台操作
	LOGSUBTYPE_USEROPRATE_RECIMAGEOPERATE                    = 0x80000020, ///< 用户操作-录像图片操作
	LOGSUBTYPE_USEROPRATE_RECIMAGEREPLAY                     = 0x80000040, ///< 用户操作-录像图片回放
	LOGSUBTYPE_USEROPRATE_LABEL                              = 0x80000080, ///< 用户操作-标签操作
	LOGSUBTYPE_USEROPRATE_BACKUPORDL                         = 0x80000100, ///< 用户操作-备份下载
	LOGSUBTYPE_USEROPRATE_CHN                                = 0x80000200, ///< 用户操作-通道操作
	LOGSUBTYPE_USEROPRATE_CFGOPERATE                         = 0x80000400, ///< 用户操作-导入导出配置
	LOGSUBTYPE_USEROPRATE_PARAMCONFIG                        = 0x80000800, ///< 用户操作-参数配置
	LOGSUBTYPE_USEROPRATE_STORAGE                            = 0x80001000, ///< 用户操作-硬盘操作
	LOGSUBTYPE_USEROPRATE_UPDATE                             = 0x80002000, ///< 用户操作-升级操作
	LOGSUBTYPE_USEROPRATE_RECOVERY                           = 0x80004000, ///< 用户操作-恢复出厂
	LOGSUBTYPE_USEROPRATE_GUIOUTPUTCUT                       = 0x80008000, ///< 用户操作-GUI输出切换
	LOGSUBTYPE_USEROPRATE_VOICECALLORBROADCAST               = 0x80010000, ///< 用户操作-语音呼叫广播
	LOGSUBTYPE_USEROPRATE_EXTSTORAGE                         = 0x80020000, ///< 用户操作-外接存储设备操作
	LOGSUBTYPE_USEROPRATE_AIANLS							 = 0x80040000, ///< 用户操作-智能分析
	LOGSUBTYPE_USEROPRATE_CTRLLIB							 = 0x80080000, ///< 用户操作-布控库

	LOGSUBTYPE_SYSINFO_ALL                                   = 0x01000001, ///< 系统信息-全部
	LOGSUBTYPE_SYSINFO_LINE_CONNECT                          = 0x01000002, ///< 系统信息-网络接入
	LOGSUBTYPE_SYSINFO_USB_INFO								 = 0x01000004, ///< 系统信息-U盘插入与拔出
	LOGSUBTYPE_SYSINFO_SATA_DISK							 = 0x01000008, ///< 系统信息-硬盘插入与拔出
	LOGSUBTYPE_SYSINFO_SDCARD_INFO                           = 0x01000010, ///< 系统信息-存储卡插入与拔出
	LOGSUBTYPE_SYSINFO_SIMCARD_INFO                          = 0x01000020, ///< 系统信息-sim插入与拔出
	LOGSUBTYPE_SYSINFO_SRV_CENTER_REGIST                     = 0x01000040, ///< 系统信息-服务中心注册
	LOGSUBTYPE_SYSINFO_IPC_ONLINE							 = 0x01000080, ///< 系统信息-摄像机上线
	LOGSUBTYPE_SYSINFO_REC_INFO								 = 0x01000100, ///< 系统信息-录像开始与停止
};

enum ELogSrcType
{
	LOGSRCTYPE_ALL                                           = 0x01, ///< 全部
	LOGSRCTYPE_NVR                                           = 0x02, ///< NVR
	LOGSRCTYPE_CHN                                           = 0x04, ///< 各通道
	LOGSRCTYPE_USER                                          = 0x08, ///< 各用户
};

enum EHealthLogTimeType
{
	HEALTHLOGTIMETYPE_ONE_MONTH                              = 1, ///< 一个月
	HEALTHLOGTIMETYPE_TWO_MONTH                              = 2, ///< 两个月
	HEALTHLOGTIMETYPE_THREE_MONTH                            = 3, ///< 三个月
};

enum EHealthInfoType
{
	HEALTHINFOTYPE_POWER_OFF                                 = 0x01, ///< 掉电关机
	HEALTHINFOTYPE_ABNORMAL_REBOOT                           = 0x02, ///< 异常重启
	HEALTHINFOTYPE_NET_ABNORMAL                              = 0x04, ///< 网络异常
	HEALTHINFOTYPE_SHUTDOWN                                  = 0x08, ///< 正常关机
	HEALTHINFOTYPE_REBOOT                                    = 0x10, ///< 正常重启
};

enum ESummerTimeOffset
{
	SUMMERTIMEOFFSET_30MIN = 0x01,    ///< 30分钟
	SUMMERTIMEOFFSET_60MIN  = 0x02,   ///< 60分钟
	SUMMERTIMEOFFSET_90MIN  = 0x04,   ///< 90分钟
	SUMMERTIMEOFFSET_120MIN  = 0x08,  ///< 120分钟
};

enum EDiskInitState
{
	DISKINITSTATE_IDLE        = 0x01,      ///< 空闲
	DISKINITSTATE_RUNNING     = 0x02,      ///< 进行中
	DISKINITSTATE_DONE        = 0x04,      ///< 完成
	DISKINITSTATE_FAILED      = 0x08,      ///< 失败
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
	SYSTIMESYNCTYPE_NTP				= 0x0001,   ///< NTP同步
	SYSTIMESYNCTYPE_VSIP			= 0x0002,   ///< VSIP平台同步
	SYSTIMESYNCTYPE_ONVIF			= 0x0004,	///< ONVIF平台同步
	SYSTIMESYNCTYPE_GB1				= 0x0008,   ///< GB28181-1平台同步
	SYSTIMESYNCTYPE_GB2				= 0x0010,   ///< GB28181-2平台同步
	SYSTIMESYNCTYPE_ANDROID			= 0x0020,   ///< ANDROID系统同步
	SYSTIMESYNCTYPE_GPS				= 0x0040,   ///< GPS同步
	SYSTIMESYNCTYPE_WORKSTATION		= 0x0080,   ///< 采集工作站同步
	SYSTIMESYNCTYPE_AUTO			= 0x0100,   ///< 自适应
};

enum EAlarmInDayType
{
	ALARMINDAYTYPE_MONDAY       = 0, ///< 周一
	ALARMINDAYTYPE_TUESDAY      = 1, ///< 周二
	ALARMINDAYTYPE_WEDNESDAY    = 2, ///< 周三
	ALARMINDAYTYPE_THURSDAY     = 3, ///< 周四
	ALARMINDAYTYPE_FRIDAY       = 4, ///< 周五
	ALARMINDAYTYPE_SATURDAY     = 5, ///< 周六
	ALARMINDAYTYPE_SUNDAY       = 6, ///< 周日
	ALARMINDAYTYPE_HOLIDAY      = 7, ///< 假日
};

enum EAlarmNumType
{
	ALARMNUM_SMART_WARNINGLINE                     = 1010, ///< 告警-警戒线
	ALARMNUM_SMART_DETECT                          = 1011, ///< 告警-区域入侵
	ALARMNUM_SMART_DETECTIN                        = 1012, ///< 告警-进入区域
	ALARMNUM_SMART_DETECTOUT                       = 1013, ///< 告警-离开区域
	ALARMNUM_SMART_PROPERTYLOST                    = 1014, ///< 告警-物品遗留
	ALARMNUM_SMART_PROPERTYTAKE                    = 1015, ///< 告警-物品拿取
	ALARMNUM_SMART_PEOPLEGATHERING                 = 1016, ///< 告警-人员聚集
	ALARMNUM_SMART_FACEDETECTION                   = 1017, ///< 告警-人脸检测
	ALARMNUM_SMART_SHADE                           = 1018, ///< 告警-遮挡
	ALARMNUM_SMART_OUTOFFOCUS                      = 1019, ///< 告警-虚焦
	ALARMNUM_SMART_SCENECHANGE                     = 1020, ///< 告警-场景变化
	ALARMNUM_SMART_AUDIOEXCEPTION                  = 1021, ///< 告警-音频异响

	ALARMNUM_SYS_NETFAULT                          = 1030, ///< 系统异常-网络故障
	ALARMNUM_SYS_IPCONFLIT                         = 1031, ///< 系统异常-IP冲突
	ALARMNUM_SYS_MACCONFLIT                        = 1032, ///< 系统异常-MAC冲突
	ALARMNUM_SYS_MONITORDROPPED                    = 1033, ///< 系统异常-监控点掉线
	ALARMNUM_SYS_NORECDISK                         = 1034, ///< 系统异常-无硬盘
	ALARMNUM_SYS_DISKFAULT                         = 1035, ///< 系统异常-硬盘故障
	ALARMNUM_SYS_RECSPACEFULL                      = 1036, ///< 系统异常-录像空间满
	ALARMNUM_SYS_SNAPSPACEFULL                     = 1037, ///< 系统异常-抓拍空间满
	ALARMNUM_SYS_ILLEGAACCESS                      = 1038, ///< 系统异常-非法访问
	ALARMNUM_SYS_HOTBACKUP                         = 1039, ///< 系统异常-热备异常    ***** 需求没有
};

enum ERecoverFactoryType
{
	RECOVERFACTORYTYPE_NONE	= 0, ///< 不恢复
	RECOVERFACTORYTYPE_ALL	= 1, ///< 完全恢复
};

enum EAutoServiceType
{
	AUTOSERVICETYPE_NONE     = 0, ///< 不维护
	AUTOSERVICETYPE_PERDAY   = 1, ///< 每日
	AUTOSERVICETYPE_PERWEEK  = 2, ///< 每周
	AUTOSERVICETYPE_PERMONTH = 3, ///< 每月
};

enum ESimStatus
{
	SIMSTATUS_INSERT	= 1, ///< 插入
	SIMSTATUS_UNINSERT	= 2, ///< 未插入
};

enum ENetType
{
	NETTYPE_UNKNOWN	= 0, ///< 未知
	NETTYPE_2G		= 1, ///< 2G
	NETTYPE_3G		= 2, ///< 3G
	NETTYPE_4G		= 3, ///< 4G
	NETTYPE_5G		= 4, ///< 5G
};

enum EGB28181RegPlatStatus
{
	GB28181REGPLATSTATUS_SUCCESS	= 1, ///< 注册成功
	GB28181REGPLATSTATUS_FAILED		= 2, ///< 注册失败
};

enum EPubSecRegPlatStatus
{
	PUBSECREGPLATSTATUS_SUCCESS	= 1, ///< 注册成功
	PUBSECREGPLATSTATUS_FAILED	= 2, ///< 注册失败
};

enum EAiuSnapType
{
	AIUSNAPTYPE_MANUAL		= 1, ///< 手动抓拍
};

enum EAiuPicType
{
	AIUPICTYPE_FACE	= 1, ///< 人脸
	AIUPICTYPE_CAR	= 2, ///< 车牌
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
	PICSIZETYPE_ORIGINAL	= 1, ///< 原图
	PICSIZETYPE_SMALL		= 2, ///< 小图
};

enum ESeriesSnapPicType
{
	SNAPPICTYPE_FACE_ORIG					= 0x0001,   ///< 人脸原图
	SNAPPICTYPE_FACE_PART					= 0x0002,   ///< 人脸局部对应
	SNAPPICTYPE_CAR_ORIG					= 0x0004,	///< 车牌图片
	SNAPPICTYPE_CAR_NO						= 0x0008,   ///< 车牌
	SNAPPICTYPE_REC_THUMBNAIL				= 0x0010,   ///< 录像缩略图
	SNAPPICTYPE_REC_CLIP_THUMBNAIL			= 0x0020,   ///< 录像片段缩略图
	SNAPPICTYPE_MANUAL_SNAP					= 0x0040,   ///< 手动抓拍
	SNAPPICTYPE_EVENT_REC					= 0x0080,   ///< 事件录像
	SNAPPICTYPE_EVENT_REC_THUMBNAIL			= 0x0100,   ///< 事件录像缩略图
};

enum EAiuAlgDetectType
{
	AIUALGDETECTTYPE_MANUAL	= 1, ///< 算法检测类型，手动
	AIUALGDETECTTYPE_AUTO	= 2, ///< 算法检测类型，自动
};

enum EPicUploadPubSecStatus
{
	PICUPLOADPUBSECSTATUS_NOUPLOAD	= 1, ///< 未上传
	PICUPLOADPUBSECSTATUS_UPLOADING	= 2, ///< 上传中
	PICUPLOADPUBSECSTATUS_UPLOADED	= 3, ///< 已上传
};

enum EPicUploadStatus
{
	PICUPLOADSTATUS_NOUPLOAD		= 1, ///< 未上传
	PICUPLOADSTATUS_UPLOADING		= 2, ///< 上传中
	PICUPLOADSTATUS_UPLOADED		= 3, ///< 已上传
	PICUPLOADSTATUS_UPLOADFAILED	= 4, ///< 上传失败
};

enum EAiuType
{
	AIUTYPE_FACE	= 1, ///< 智能类型，人脸检测
	AIUTYPE_CAR 	= 2, ///< 智能类型，车牌检测
};

enum ESexType
{
	SEXTYPE_MALE		= 0x0001, ///< 男性
	SEXTYPE_FEMALE		= 0x0002, ///< 女性
	SEXTYPE_UNLIMITED	= 0x0004, ///< 不限
};

enum EIdType
{
	IDTYPE_ID		= 0x0001, ///< 身份证
	IDTYPE_OFFICER	= 0x0002, ///< 军官证
	IDTYPE_PASSPORT = 0x0004, ///< 护照
	IDTYPE_OTHER	= 0x0008, ///< 其他
	IDTYPE_UNLIMITED= 0x0010, ///< 不限
};

enum ECtrlLibAddType
{
	CTRLLIBADDTYPE_NORMAL		= 0, ///< 普通添加
	CTRLLIBADDTYPE_FORCE		= 1, ///< 强制添加
	CTRLLIBADDTYPE_DED_UPDATE	= 2, ///< 增加去重判断，重复时更新该成员
	CTRLLIBADDTYPE_DED_NO_ADD	= 3, ///< 增加去重判断，重复时返回对应错误码
};

enum ECtrlLibExportType
{
	CTRLLIBEXPORTTYPE_WHOLE		= 0x0001, ///< 整个布控库
	CTRLLIBEXPORTTYPE_PART		= 0x0002, ///< 部分成员
};

enum EEigenvalueType
{
	EIGENVALUETYPE_SUCCESS			= 0x0001, ///< 建模成功
	EIGENVALUETYPE_FAIL				= 0x0002, ///< 建模失败
	EIGENVALUETYPE_UNLIMITED		= 0x0004, ///< 全部
};

enum ECtrlLibType
{
	CTRLLIBTYPE_FACE = 0x0001, ///< 人脸库
	CTRLLIBTYPE_CAR  = 0x0002, ///< 车辆库
};

enum ECtrlLibExportState
{
	CTRLLIBEXPORTSTATE_IDLE				= 0x0001, ///< 空闲
	CTRLLIBEXPORTSTATE_CREAT_FILE		= 0x0002, ///< 正在生成导出文件
	CTRLLIBEXPORTSTATE_CREAT_FILE_DONE	= 0x0004, ///< 生成导出文件完成
	CTRLLIBEXPORTSTATE_EXPORTING		= 0x0008, ///< 导出中
	CTRLLIBEXPORTSTATE_DONE				= 0x0010, ///< 完成
	CTRLLIBEXPORTSTATE_ERR				= 0x0020, ///< 出错
};

enum ECtrlLibImportState
{
	CTRLLIBIMPORTSTATE_IDLE				= 0x0001, ///< 空闲
	CTRLLIBIMPORTSTATE_PREPARING		= 0x0002, ///< 准备完成
	CTRLLIBIMPORTSTATE_SENDING			= 0x0004, ///< 传输文件
	CTRLLIBIMPORTSTATE_PROCESSING		= 0x0008, ///< 导入中
	CTRLLIBIMPORTSTATE_DONE				= 0x0010, ///< 完成
	CTRLLIBIMPORTSTATE_ERR_DONE			= 0x0020, ///< 完成,有提取特征值等添加成员错误情况
	CTRLLIBIMPORTSTATE_ERR				= 0x0040, ///< 出错
	CTRLLIBIMPORTSTATE_OVER_SUP_MAX		= 0x0080, ///< 超出最大数量
	CTRLLIBIMPORTSTATE_NO_CSV			= 0x0100, ///< 未找到索引文件
	CTRLLIBIMPORTSTATE_CHECK_FAIL		= 0x0200, ///< 文件校验失败
};

enum ECtrlLibBatchType
{
	CTRLLIBBATCHTYPE_PIC_AND_CSV		= 0, ///< 图片和.csv表格文件
	CTRLLIBBATCHTYPE_ONLY_PIC			= 1, ///< 仅有图片
};

enum EClientType
{
	CLIENTTYPE_GUI			= 0, ///< gui
	CLIENTTYPE_WEB			= 1, ///< web
};

enum EAlgType
{
	ALGTYPE_FACE_DETECT			= 0x0001, ///< 人脸检测
	ALGTYPE_FACE_COMPARE		= 0x0002, ///< 人脸比对
	ALGTYPE_CAR_DETECT			= 0x0004, ///< 车辆检测
	ALGTYPE_PERSON_DETECT		= 0x0008, ///< 行人检测
	ALGTYPE_ALL_DETECT			= 0xffff, ///< 全部
};

enum EAiuUploadType
{
	AIUUPLOADTYPE_PUBSEC1 	= 0x0001,		///< 视图库1
	AIUUPLOADTYPE_PUBSEC2 	= 0x0002,		///< 视图库2
	AIUUPLOADTYPE_PAD 		= 0x0004,		///< pad
	AIUUPLOADTYPE_FTP 		= 0x0008,		///< ftp
};

enum EAiuUploadCaps
{
	AIUUPLOADCAPS_MDPIC 	= 0x0001,		///< 人脸图片
	AIUUPLOADCAPS_CARPIC 	= 0x0002,		///< 车牌图片
	AIUUPLOADCAPS_PIC 		= 0x0004,		///< 常规图片
	AIUUPLOADCAPS_AUD 		= 0x0008,		///< 音频
	AIUUPLOADCAPS_VID 		= 0x0010,		///< 视频
	AIUUPLOADCAPS_EVENTVID 	= 0x0020,		///< 事件视频
};

enum EAipDetectMode
{
	AIPDETECTMODE_LOCAL 	= 0x0001,		///< 本地检测
	AIPDETECTMODE_REMOTE 	= 0x0002,		///< 本身不检测,IPC检测
};

enum EAipRmRepeatMode
{
	AIPRMREPEATMODE_H_SCORE 	= 0x0001,		///< 评分最高
	AIPRMREPEATMODE_FIRST_OK 	= 0x0002,		///< 首张合格
};

enum EAipUploadMode
{
	AIPUPLOADMODE_ALL 				= 0x0001,		///< 全部
	AIPUPLOADMODE_SMALL 			= 0x0002,		///< 小图
	AIPUPLOADMODE_BODY 				= 0x0004,		///< 全身照
	AIPUPLOADMODE_BG 				= 0x0008,		///< 原图
	AIPUPLOADMODE_SBG 				= 0x0010,		///< 小图加原图
	AIPUPLOADMODE_SMALL_THUMBNAIL	= 0x0020,		///< 小图加缩略图
	AIPUPLOADMODE_SMALL_GPS			= 0x0040,		///< 小图加地理位置
};

enum EAipCompareMode
{
	AIPCOMPAREMODE_BLACK 	= 0x0001,		///< 黑名单
	AIPCOMPAREMODE_WHITE 	= 0x0002,		///< 白名单
};

enum ELogAlarmType
{
	LOGALARMTYPE_BLACKLIST 		= 0x0001,		///< 黑名单报警
	LOGALARMTYPE_WHITELIST 		= 0x0002,		///< 白名单报警
	LOGALARMTYPE_WHITELOG 		= 0x0004,		///< 白名单日志
};

enum EAlarmLinkUploadMode
{
	ALARMLINKUPLOADMODE_ALARM_ONLY 		= 0x0001,		///< 仅报警人员
	ALARMLINKUPLOADMODE_ALL 			= 0x0002,		///< 全部人员
};

enum EAlarmLinkUploadType
{
	ALARMLINKUPLOADTYPE_FACE 				= 0x0001,		///< 人脸照
	ALARMLINKUPLOADMODE_BACKGROUND 			= 0x0002,		///< 全景照
	ALARMLINKUPLOADMODE_FACE_BACKGROUND 	= 0x0004,		///< 人脸+全景照
};

enum ESnapPicQuailty
{
	SNAPPICQUAILTY_HIGH 			= 0x0001,		///< 人脸照
	SNAPPICQUAILTY_MIDIUM 			= 0x0002,		///< 全景照
	SNAPPICQUAILTY_LOW 				= 0x0004,		///< 人脸+全景照
};

/**
 * 结构体定义
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
	s32 nX;	///< x坐标
	s32 nY;	///< y坐标
}NETPACKED;

struct TNetNvrCap
{
	s8 szDevModel[NET_MAXLEN_64];	///< NVR型号
	BOOL32 bActive;					///< NVR是否激活
	s32 nMaxChnNum;					///< NVR最大通道数
	s32 nMaxChnGroupNum;			///< NVR最大通道分组数
	s32 nNicNum;					///< 网卡数量
	s32 nSerialNum;					///< 串口数量
	s32 nMaxAlarmInNum;				///< 最大告警输入数量
	s32 nGB28181InPlatNum;			///< 国标上联平台数量，0表示不支持GB28181上联协议
	BOOL32 bSupVsip;				///< 是否支持vsip上联协议
	BOOL32 bSupGB28181Ctl;          ///< 是否支持国标下联控制
	BOOL32 bSupPubSec;				///< 是否支持视图库上联协议
	BOOL32 bSupPubSecCtl;			///< 是否支持视图库下联协议
	BOOL32 bSupOnvif;               ///< 是否支持onvif上联协议
	BOOL32 bSupCloudServer;         ///< 是否支持云服务
	BOOL32 bSupZeroChnEnc;			///< 是否支持零通道编码功能
	BOOL32 bSupAI;                  ///< 是否支持AI
	BOOL32 bSupSnmp;			    ///< 是否支持snmp功能
	BOOL32 bSupPcap;                ///< 是否支持网络抓包
	BOOL32 bSupBroadcast;           ///< 是否支持广播
	BOOL32 bSupIpFilter;            ///< 是否支持ip过滤
	BOOL32 bSupSXTServer;           ///< 是否支持视讯通服务(目前pad客户端使用)
	BOOL32 bDisableListenStateCtrl;	///< 是否禁用监听状态控制
	BOOL32 bWirelessVeh;            ///< 是否是车载设备
	BOOL32 bSupAisCtrlLib;          ///< 是否支持布控库
	BOOL32 bSupSnap;                ///< 是否支持抓拍
	BOOL32 bSupAiSearch;			///< 是否支持智能检索
	BOOL32 bSupWebsocket;			///< 是否支持websocket
	BOOL32 bSupDDNS;				///< 是否支持Ddns功能
	BOOL32 bSupDragCusCanvas;		///< 是否支持自定义画布
	BOOL32 bSupSysHealth;			///< 是否支持健康状态
	BOOL32 bSupIsp;					///< 是否支持Isp
	BOOL32 bSupMulticast;			///< 是否支持多播
	BOOL32 bSupGpSecurity;			///< 是否支持GP安全
}NETPACKED;

struct TNetGuiLangInfo
{
	u32 dwLangMask;			///< gui支持的语言，EGuiLangType组合值
	EGuiLangType eCurLang;	///< 当前语言
}NETPACKED;

struct TNetLoginInfo
{
	u32 dwNvrIp;									///< NVR IP，网络序
	u16 wPort;										///< 端口，网络序
	s8  szUserName[NET_NVR_USER_NAME_MAX_LEN + 1];	///< 用户名
	s8  szPwd[NET_NVR_USER_PWD_MAX_LEN + 1];		///< 密码
	s8  szEmail[NET_MAXLEN_32 + 1];					///< 邮箱
	EGuiLangType eGuiLangType;						///< gui语言类型（仅供gui使用）
	BOOL32 bEnableWebsocket;						///< 是否启用websocket
}NETPACKED;

struct TNetLoginErrInfo
{
	s32 nLoginRetryTimes;       ///< 剩余登录重试次数
	s32 nLoginLockedTime;       ///< 剩余登陆锁定时间
}NETPACKED;

struct TNetCheckInfo
{
	s8 szUserName[NET_NVR_USER_NAME_MAX_LEN + 1];	///< 用户名
	s8 szPwd[NET_NVR_USER_PWD_MAX_LEN + 1];			///< 密码
}NETPACKED;

struct TNetNvrState
{
	u32 dwNvrStateMask;					///< NVR状态，ENvrState组合值
	u32 dwRealLogTimeStamp;				///< 实时日志时间戳
	u32 dwRealLogNum;					///< 实时日志数量
	u32 dwRealStateNum;					///< 实时状态数量
	s32 nNvrNtyEventNum;				///< NVR通知事件数量
	BOOL32 bFaceSnapChanged;			///< 人脸抓拍变化，NVRSTATE_AIUSTATUS时有效
	s8 szFaceSnapChnId[NET_MAXLEN_128];	///< 人脸抓拍变化的通道id，NVRSTATE_AIUSTATUS时有效
	BOOL32 bCarSnapChanged;				///< 车辆抓拍变化，NVRSTATE_AIUSTATUS时有效
	s8 szCarSnapChnId[NET_MAXLEN_128];	///< 车辆抓拍变化的通道id，NVRSTATE_AIUSTATUS时有效
	BOOL32 bAiuCfgChanged;				///< 智能配置变化，NVRSTATE_AIUSTATUS时有效
	s8 szAiuCfgChnId[NET_MAXLEN_128];	///< 智能配置变化的通道id，NVRSTATE_AIUSTATUS时有效
	BOOL32 bNvrAlarmChanged;			///< nvr告警变化，NVRSTATE_ALARMSTATE时有效
	BOOL32 bChnAlarmChanged;			///< 通道告警变化，NVRSTATE_ALARMSTATE时有效
	s8 szChnAlarmChnId[NET_MAXLEN_128];	///< 通道告警变化的通道id，NVRSTATE_ALARMSTATE时有效
	s8 szChnListChnId[NET_MAXLEN_512];	///< 通道列表变化的通道id，NVRSTATE_CHNLIST时有效
}NETPACKED;

struct TNetCallBackInfo
{
	u32 dwHandle;			///< 句柄
	s32 nErrNo;				///< 错误码
	void *pParam;			///< 回调函数参数
	void *pResult;			///< 异步函数执行结果
	s32 nResultLen;			///< pResult长度
	EInterType eInterType;	///< 接口类型
}NETPACKED;

struct TNetMsgItem
{
	u32 dwHandle;		///< 句柄
	EMsgType eMsgType;	///< 消息类型
	void *pData;		///< 数据
	s32 nDataLen;		///< 数据长度
	u32 dwData;			///< eMsgType = MSGTYPE_FACE_DETECT_SNAP：当天抓拍人脸总数
						///< eMsgType = MSGTYPE_CAR_DETECT_SNAP：当天抓拍车辆总数
						///< eMsgType = MSGTYPE_PERSON_DETECT_SNAP：当天行人检测总数
	// void *pParam;	///< 回调函数参数
}NETPACKED;

struct TNetSubscribeMsgItem
{
	EMsgType eMsgType;	///< 消息类型
}NETPACKED;

struct TNetSubscribeMsgList
{
	s32 nNum;															///< 订阅消息数量
	TNetSubscribeMsgItem atSubscribeMsgItem[NET_SUBSCRIBE_MSG_MAX_NUM];	///< 订阅消息信息
}NETPACKED;

struct TNetProtoItem
{
	EProtoType eProtoType;	///< 协议类型
	BOOL32 bSupportSearch;	///< 协议是否支持设备搜索
	u32 dwTransProtoMask;	///< 媒体传输协议，ETransProto组合值
	u32 dwTcpConnectModeMask;///< TCP连接方向，ETcpMode组合值
}NETPACKED;

struct TNetProtoList
{
	u8 byProtoNum;                                 ///< 协议数量
	TNetProtoItem atProtoItem[NET_PROTO_MAX_NUM];  ///< 协议内容
}NETPACKED;

struct TNetSearchDevParam
{
	u32 dwProtoMask;		///< 搜索指定协议类型的设备，EProtoType组合值
	BOOL32 bEnableIpFilter;	///< 是否启用IP过滤
	u32 dwStartIp;			///< 起始IP，网络序
	u32 dwEndIp;			///< 结束IP，网络序
}NETPACKED;

struct TNetSearchDevTask
{
	u32 dwTaskId;		///< 搜索任务id
	u32 dwTaskTimes;	///< 搜索任务持续时长，单位s
}NETPACKED;

struct TNetDevExtInfo
{
	BOOL32 bSupActive;                     ///< 是否支持激活
	BOOL32 bActive;                        ///< 是否激活
	BOOL32 bSupSetAddr;                    ///< 是否支持设置地址
	u32 dwSubnetMask;	                   ///< 子网掩码，网络序
	u32 dwDefGateway;	                   ///< 默认网关，网络序
	s8 szExtProtoVer[NET_MAXLEN_64 + 1];   ///< 获取扩展信息使用的协议版本
	s8 szMac[NET_MAXLEN_32 + 1];           ///< MAC地址
	s8 szSecureCode[NET_MAXLEN_64 + 1];    ///< 安全码
	EActiveState eActiveState;             ///< 激活状态
}NETPACKED;

struct TNetDevInfo
{
	u32 dwIp;								///< 设备IP地址，网络序
	EProtoType eProtoType;					///< 协议类型
	u16 wProtoPort;							///< 协议端口
	u16 wVideoEncChnNum;					///< 视频编码通道数量
	u16 wAudioEncNum;						///< 音频编码数量
	u16 wAudioDecNum;						///< 音频解码数量
	s8 szDevModel[NET_MAXLEN_32 + 1];		///< 设备型号
	s8 szManufacturer[NET_MAXLEN_32 + 1];	///< 厂商信息
	s8 szUuid[NET_MAXLEN_128 + 1];          ///< 设备对应uuid
	EDevType eDevType;						///< 设备类型
	s8 szSoftVer[NET_MAXLEN_64];		    ///< 软件版本号
	TNetDevExtInfo tDevExtInfo;             ///< 设备扩展信息
}NETPACKED;

struct TNetSearchedDevList
{
	BOOL32 bFinished;	                                     ///< 搜索是否结束
	u16 wDevNum;                                             ///< 设备数量
	TNetDevInfo atDevInfo[NET_PER_GET_SEARCHED_DEV_MAX_NUM]; ///< 设备信息
}NETPACKED;

struct TNetAddDev
{
	s32 nChnId;					    ///< 通道id，1~最大通道数，NET_CHNID_AUTO表示不指定通道，自动分配
	s8  szDevInfo[NET_MAXLEN_512];	///< 设备信息，xml格式
}NETPACKED;

struct TNetAddDevList
{
	u16 wDevNum;                                  ///< 设备数量        
	TNetAddDev atAddDev[NET_PER_ADD_DEV_MAX_NUM]; ///< 设备信息
}NETPACKED;

struct TNetDelDevList
{ 
	u16 wDevNum;                            ///< 设备数量
	s32 anChnId[NET_PER_DEL_DEV_MAX_NUM];   ///< 通道id数组
}NETPACKED;

struct TNetDevActiveInfo
{ 
	u32 dwIp;								///< 设备IP地址，网络序
	EProtoType eProtoType;					///< 协议类型
	u16 wProtoPort;							///< 协议端口
	s8 szExtProtoVer[NET_MAXLEN_64 + 1];    ///< 获取扩展信息使用的协议版本
	s8 szMac[NET_MAXLEN_32 + 1];            ///< MAC地址
	s8 szSecureCode[NET_MAXLEN_64 + 1];     ///< 安全码
}NETPACKED;

struct TNetCreateDevActiveTaskInfo
{ 
	BOOL32 bAdminPwd;                                              ///< 是否使用NVR管理员密码
	BOOL32 bAdminEmail;                                            ///< 是否使用NVR管理员邮箱
	s8 szPassword[NET_MAXLEN_64];                                  ///< 自定义激活密码(bAdminPwd为false时有效)
	s8 szEmail[NET_MAXLEN_64];                                     ///< 自定义管理邮箱(bAdminEmail为false时有效)
	u16 wDevNum;                                                   ///< 设备数量
	TNetDevActiveInfo atDevActiveInfo[NET_PER_DEV_ACTIVE_MAX_NUM]; ///< 设备信息
}NETPACKED;

struct TNetAddDevActiveInfo
{ 
	s32 nTaskId;                                                   ///< 激活任务ID
	u16 wDevNum;                                                   ///< 设备数量
	TNetDevActiveInfo atDevActiveInfo[NET_PER_DEV_ACTIVE_MAX_NUM]; ///< 设备信息
}NETPACKED;

struct TNetQueryDevActiveTaskState
{ 
	s32 nTaskId;                                                   ///< 激活任务ID
	u16 wDevNum;                                                   ///< 设备数量
	TNetDevActiveInfo atDevActiveInfo[NET_PER_DEV_ACTIVE_MAX_NUM]; ///< 设备信息
}NETPACKED;

struct TNetDevActiveState
{ 
	EDevActiveState eDevActiveState;	       ///< 激活状态
	EDevActiveErrReason eDevActiveErrReason;   ///< 激活出错原因(可选项)
	TNetDevActiveInfo tDevActiveInfo;          ///< 设备信息
}NETPACKED;

struct TNetDevActiveTaskState
{ 
	BOOL32 bFinished;                                                ///< 是否完成
	u16 wDevNum;                                                     ///< 设备数量
	TNetDevActiveState atDevActiveState[NET_PER_DEV_ACTIVE_MAX_NUM]; ///< 设备信息
}NETPACKED;

struct TNetModifyDevIpInfo
{ 
	s8  szUserName[NET_MAXLEN_64];	                ///< 用户名
	s8  szPwd[NET_MAXLEN_64 + 1];		            ///< 密码
	u32 dwIp;		    							///< 修改后的IP地址
	u32 dwSubnetMask;		    					///< 修改后的子网掩码
	u32 dwDefaultGateway;		    				///< 修改后的默认网关
	TNetDevActiveInfo tDevInfo;                     ///< 设备信息
}NETPACKED;

struct TNetCreateDevUpgradeTaskInfo
{ 
	u16 wDevNum;                                ///< 设备数量
	s32 anChnId[NET_PER_UPGRADE_DEV_MAX_NUM];   ///< 通道id数组
	s8 szPkgHead[NET_MAXLEN_512 + 1];           ///< pkg包头信息
	s32 nPkgHeadSize;                           ///< pkg包头大小
	s32 nPkgTotalSize;                          ///< pkg包总大小
}NETPACKED;

struct TNetDevUpgradeStateInfo
{ 
	s32 nChnId;                                  ///< 通道id
	EDevUpgradeState eDevUpgradeState;           ///< 当前升级状态
	EDevUpgradeErrReason eDevUpgradeErrReason;   ///< 升级出错原因,只在升级状态为DEVUPGRADESTATE_ERROR时有效
}NETPACKED;

struct TNetDevUpgradeStateList
{ 
	BOOL32 bAllFinished;                                                         ///< 是否全部升级结束
	s32 nDevStateNum;                                                            ///< 升级设备数量
	TNetDevUpgradeStateInfo atDevUpgradeStateInfo[NET_PER_UPGRADE_DEV_MAX_NUM];  ///< 升级设备状态列表
}NETPACKED;

struct TNetDevUpgradePkgData
{ 
	s32 nDataStartPos;                                    ///< 数据起始偏移
	s32 nDataLen;                                         ///< 数据buffer长度
	s8 *pszDataBuf;                                       ///< 数据buffer内容
}NETPACKED;

struct TNetGetNvrChnList
{
	u32 dwChnMask;				///< 获取通道列表指定信息，EChnMask组合值
	s32 nChnIdStart;			///< 起始通道id
	s32 nChnIdEnd;				///< 结束通道id
	s8 szChnId[NET_MAXLEN_256];	///< 通道id，与nChnIdStart和nChnIdEnd互斥，多个通道使用逗号分割，例如：1,3,9
}NETPACKED;

struct TNetDevItem
{
	TNetDevInfo tDevInfo;	///< 基本信息
}NETPACKED;

struct TNetChnState
{
	BOOL32 bDevOnline;		///< 设备在线
	BOOL32 bIsRecording;	///< 是否在录像
	BOOL32 bMoveDetect;     ///< 是否触发移动侦测告警
	BOOL32 bVideoLost;		///< 是否触发视频源丢失告警
	BOOL32 bAlmIn;			///< 是否触发告警输入告警
	BOOL32 bSmart;			///< 是否触发智能告警
	BOOL32 bAccessProtect;	///< 是否触发防拆报警
	EDevErrNo eDevErrNo;	///< 设备下线情况下的错误码
}NETPACKED;

struct TNetChnInfo
{
	s32 nId;						///< 通道id
	s32 nDevId;						///< 设备id
	s8  szAlias[NET_MAXLEN_64 + 1];	///< 通道别名，0-32个字符
	s32 nVideoSourceId;				///< 视频源id
	BOOL32 bIdle;					///< 通道是否空闲
	BOOL32 bNewAdd;					///< 通道是否新添加
	TNetChnState tChnState;			///< 通道状态
}NETPACKED;

struct TNetChnItem
{
	TNetChnInfo tChnInfo;            ///< 通道信息
	TNetDevItem tDevItem;            ///< 设备信息
}NETPACKED;

struct TNetNvrChnList
{
	u16 wChnNum;				///< [in]-数组长度，[out]-实际获取的通道数量
	TNetChnItem *patChnItem;	///< 指向TNetChnItem数组
}NETPACKED;

struct TNetCompositeChnItem
{
	s32 nChnId;                              ///< 通道ID
	ECompositeChnType eCompositeChnType;     ///< 合成通道类型
	BOOL32 bDisplayFirst;                    ///< 是否展示在列表首位
	BOOL32 bSupportRec;                      ///< 是否支持录像
	BOOL32 bSupportEditName;                 ///< 是否支持编辑名字
}NETPACKED;

struct TNetNvrCompositeChnCap
{
	u16 wChnNum;				                                        ///< 获取的通道数量
	TNetCompositeChnItem atCompositeChnItem[NET_COMPOSITE_CHN_MAX_NUM];	///< 指向TNetChnItem数组
}NETPACKED;

struct TNetOnvifChnCfg
{
	u32 dwIp;								///< 设备IP地址，网络序
	u16 wProtoPort;							///< 协议端口
	EDevAddMode eDevAddMode;                ///< 设备添加模式
	u16 wSrcId;		            			///< 视频源id
	u16 wSrcNum;                            ///< 视频源数量(只有当eDevAddMode取值为DEVADDMODE_KEDAFISHEYE时有效)
	ETransProto eTransProto;    	        ///< 传输协议类型
	s8 szAuthName[NET_MAXLEN_64 + 1];       ///< 认证用户名
	s8 szAuthPwd[NET_MAXLEN_64 + 1];        ///< 认证密码
}NETPACKED;

struct TNetRtspChnCfg
{
	u32 dwIp;								///< 设备IP地址，网络序
	ETransProto eTransProto;    	        ///< 传输协议类型
	s8 szMainStreamUrl[NET_MAXLEN_128 + 1]; ///< 主码流地址
	s8 szSecStreamUrl[NET_MAXLEN_128 + 1];  ///< 辅码流地址
	s8 szAuthName[NET_MAXLEN_64 + 1];       ///< 认证用户名
	s8 szAuthPwd[NET_MAXLEN_64 + 1];        ///< 认证密码
	BOOL32 bSendRtspKplvForTcp;             ///< tcp模式下是否发送rtsp心跳
}NETPACKED;

struct TNetGb28181EncChnCfg
{
	s8 szEncId[NET_MAXLEN_32];                                               ///< 编码通道ID
}NETPACKED;

struct TNetGb28181ChnCfg
{
	s32 nEncChnNum;                                                          ///< 编码通道数量
	TNetGb28181EncChnCfg atGb28181EncChnCfg[NET_GB28181_PRECHN_ENC_MAX_NUM]; ///< 编码通道ID
}NETPACKED;

struct TNetGb28181AlarmInCfg
{
	s8 szAlarmInId[NET_MAXLEN_32];                                           ///< 编码通道ID
}NETPACKED;

struct TNetGb28181DevCfg
{
	s8 szDevId[NET_MAXLEN_32];                                               ///< 设备ID
	s8 szDevPwd[NET_MAXLEN_64];                                              ///< 密码
	ETransProto eTransProto;    	                                         ///< 传输协议类型
	ETcpMode eTcpMode;														 ///< 连接方向	
	s32 nChnNum;                                                             ///< 通道数量
	TNetGb28181ChnCfg atGb28181ChnCfg[NET_GB28181_CHN_MAX_NUM];              ///< 通道信息
	s32 nAlarmInNum;                                                         ///< 告警输入数量
	TNetGb28181AlarmInCfg atGb28181AlarmInCfg[NET_GB28181_CHN_MAX_NUM];       ///< 告警输入信息
}NETPACKED;

struct TNetChnCfg
{
	s32 nChnId;					            ///< 通道id
	TNetOnvifChnCfg tOnvifChnCfg;           ///< Onvif设备配置
	TNetRtspChnCfg tRtspChnCfg;             ///< Rtsp设备配置
	TNetGb28181DevCfg tGb28181DevCfg;       ///< GB28181设备配置
}NETPACKED;

struct TNetChnAlias
{
	s32 nChnId;                         ///< 通道id
    s8 szChnAlias[NET_MAXLEN_64 + 1];	///< 通道别名，0~32个字符
	BOOL32 bSetDevOsd;                  ///< 通道扩展配置，是否设置为OSD台标，只在通道别名设置及获取通道扩展配置时使用
	BOOL32 bSetDevName;                 ///< 通道扩展配置，是否设置为通道设备名称，只在通道别名设置及获取通道扩展配置时使用
}NETPACKED;

struct TNetChnAliasList 
{
	u16 wNum;                                              ///< 通道别名列表数量
	TNetChnAlias atChnAlias[NET_CHN_ALIAS_LIST_MAX_NUM];   ///< 通道别名列表
}NETPACKED;

struct TNetChnGroupItem
{
	s32 nId;									///< 通道组Id
	s8  szAlias[NET_MAXLEN_64 + 1];	            ///< 通道组别名，0(分组内无通道)或者1~32个字符
	u16 wChnNum;                                ///< 通道数量
	s32 anChnId[NET_CHNGROUP_CHNITEM_MAX_NUM];	///< 通道id列表
}NETPACKED;

struct TNetChnGroupList
{
	u16 wChnGroupNum;                                      ///< 通道分组数量
	TNetChnGroupItem atChnGroupItem[NET_CHNGROUP_MAX_NUM]; ///< 通道分组信息
}NETPACKED;

struct TNetTimeSyncCfg
{
	s32 nChnId;                           ///< 通道ID
	u32 dwTimeSyncTypeMask;               ///< 时间同步类型掩码，ETimeSyncType组合值
	ETimeSyncType eCurTimeSyncType;       ///< 当前时间同步类型
	BOOL32 bSyncTimeZone;                 ///< 是否与nvr时区同步，TRUE-同步，FALSE-不同步
}NETPACKED;

struct TNetOsdMargin
{
	EOsdMarginType eOsdMarginTypeTop;        ///< 当前顶部边距
	EOsdMarginType eOsdMarginTypeLeft;       ///< 当前左侧边距
	EOsdMarginType eOsdMarginTypeRight;      ///< 当前右侧边距
	EOsdMarginType eOsdMarginTypeBottom;     ///< 当前底部边距
	u32 dwOsdMarginMask;                     ///< 支持的边距类型，EOsdMarginType类型组合值
}NETPACKED;

struct TNetOsdPic
{
	BOOL32 bCustomAreaOnePic;                             ///< 用户自定义区域是否只生成一张图片
	EOsdPicType eCurOsdPicType;                           ///< 当前OSD图片类型
	u32 dwSupOsdPicType;                                  ///< 支持的图片类型，EOsdPicType类型组合值
	s32 nPicBits;                                         ///< 图片位数
}NETPACKED;

struct TNetOsdTime
{
	EOsdTimeFormat eOsdTimeFormat;                 ///< 时间格式
	u32 dwOsdTimeFormatMask;                       ///< 支持的时间格式，EOsdTimeFormat类型组合值
	BOOL32 bSingleRow;                             ///< 是否单行显示（true:单行,false:分行）
}NETPACKED;

struct TNetOsdFontSize
{
	EOsdResolutionType eOsdResolutionType;                ///< 分辨率类型
	EOsdFontSizeType eOsdFontSizeType;                    ///< 字体大小
	u32 dwOsdFontSizeTypeMask;                            ///< 支持的字体大小，EOsdFontSizeType类型组合值
	s32 nOsdFontSize;                                     ///< 字体大小(仅在超出能力范围时有效)
}NETPACKED;

struct TNetOsdFont
{
	EOsdFontType OsdFontType;                             ///< 字体名称
	u32 dwOsdFontTypeMask;                                ///< 支持的字体类型，EOsdFontType类型组合值
	s8 szOsdTextFont[NET_MAXLEN_64 + 1];                  ///< 字体名称（PicGenByPic取值为false时，上传者自定义字体名）
	EOsdTextColor eOsdTextColor;                          ///< 文字颜色
	u32 dwOsdTextColorMask;                               ///< 支持的文字颜色，EOsdTextColor类型组合值              
	EOsdTextColor eOsdTextBkColor;                        ///< 文字背景颜色
	u32 dwOsdTextBkColorMask;                             ///< 支持的文字背景颜色，EOsdTextColor类型组合值
	BOOL32 bTextBkTransparent;                            ///< 文字背景是否透明
	BOOL32 bTextEdge;                                     ///< 文字是否描边
	s32 nOsdFontSizeNums;                                 ///< osd字体大小数量
	TNetOsdFontSize atOsdFontSize[NET_DEVOSD_MAX_NUM];    ///< osd字体大小
}NETPACKED;

struct TNetSingleOsdCfg
{
	EOsdType eOsdType;                       ///< OSD类型
	BOOL32 bVisible;                         ///< 是否可见
	s32 nUserOsdSN;                          ///< 用户自定义OSD标识id，取值范围为: 1 - UserMaxNum值,其他OSD取0
	TNetRect tOsdRect;                       ///< osd 位置
	EOsdContentType eOsdContentType;         ///< osd 内容类型
	u32 dwOsdContentTypeMask;                ///< osd 内容类型掩码，EOsdContentType类型组合值
	s8 szContent[NET_MAXLEN_128];            ///< osd 内容
	s32 nMaxChar;                            ///< osd 最大字符数
	s8 szDevLineId[NET_MAXLEN_128];          ///< 内容类型为dev时,对应的外设字符串的行id
}NETPACKED;

struct TNetOsdCap
{
	BOOL32 bOsdMoveFree;                                      ///< 是否支持每个osd字幕随意拖动
	u32 dwSupOsdLanguageType;                                 ///< 前端支持配置的osd语言类型掩码，EOsdLanguageType类型组合值
	u32 dwSupFontAttr;                                        ///< 支持的字体属性，EOsdFontAttrType类型组合值
	u32 dwSupOsdType;                                         ///< 支持的osd类型，EOsdType类型组合值
	s32 nUserOsdMaxNum;                                       ///< 用户自定义字幕支持的最大个数
	s32 nUserAreaMaxSize;                                     ///< 用户字幕支持的最大面积
	s32 nOsdArea2MaxSize;                                     ///< 包含时间、速度、信号柱和REC字幕的OSD区域2最大面积(1080p分辨率下,界面使用)，
	                                                          ///< 若支持每个osd字幕随意拖动,则该值无效
}NETPACKED;

struct TNetDevOsdCfg
{
	BOOL32 bMultiLine;                                    ///< 是否OSD1~OSD5支持多行（编辑时当作一个osd，同时支持回车换行）（GUI和web默认false）
	BOOL32 bCgiSetOsd;                                    ///< 是否是cgi对osd配置
	BOOL32 bCreateByDev;                                  ///< 是否由前端生成图片
	BOOL32 bSupCreateByDev;                               ///< 是否支持由前端生成图片
	BOOL32 bSupChangeCreateByDev;                         ///< 是否支持修改由前端生成图片
	s32 nResolutionWidth;                                 ///< 视频宽（与视频编码里的分辨率不是同一概念）
	s32 nResolutionHeight;                                ///< 视频高（与视频编码里的分辨率不是同一概念）
	EOsdResolutionType eOsdResolutionType;                ///< 分辨率类型
	s8 szConfigType[NET_MAXLEN_128];                      ///< 方案类型
	TNetOsdMargin tOsdMargin;                             ///< OSD区域边距
	TNetOsdPic tOsdPic;                                   ///< OSD图片
	TNetRect tOsdCustomArea;                              ///< 用户自定义区域
	TNetOsdTime tOsdTime;                                 ///< 时间配置
	TNetOsdFont tOsdFont;                                 ///< 字体配置
	s32 nOsdNums;                                         ///< osd数量
	TNetSingleOsdCfg atSingleOsdCfg[NET_DEVOSD_MAX_NUM];  ///< 具体osd信息
	TNetOsdCap tOsdCap;                                   ///< osd能力
}NETPACKED;

struct TNetOsdPicFile
{
	EOsdType eOsdType;                       ///< OSD类型
	s8 szFilePath[NET_MAXLEN_128];           ///< osd 图片文件位置
}NETPACKED;

struct TNetOsdPicFileList
{
	s32 nOsdPicNums;                         ///< osd 图片数量
	TNetOsdPicFile atOsdPicFile[NET_DEVOSD_MAX_NUM]; ///< osd 图片信息数组
}NETPACKED;

struct TNetRcModeItem 
{
	ERcModeType eRcModeType;
	u32 dwImgQualityMask;               ///< 编码质量类型掩码，EImgQualityType组合值
}NETPACKED;

struct TNetEncResolution
{
	s16 nEncResolutionWidth;            ///< 编码分辨率宽
	s16 nEncResolutionHeight;           ///< 编码分辨率高
}NETPACKED;

struct TNetResFrameRate
{
	TNetEncResolution tEncResolution;            ///< 编码分辨率
	s16 nFrameRateMin;                           ///< 此分辨率下的最大最小帧率
	s16 nFrameRateMax;
}NETPACKED;

struct TNetVideoEncCapItem 
{
	EVideoEncType eEncType;											///< 编码类型
	TNetEncResolution atEncResolution[NET_VIDEOENC_RESOLUTION_MAX_NUM]; ///< 编码分辨率能力数组
	u16 wEncResNum;  
	TNetResFrameRate atResFrameRate[NET_VIDEOENC_RESOLUTION_MAX_NUM];	///< 此分辨率下的最大最小帧率
	u32 dwRcModeMask;												///< 码率控制模式掩码，ERcModeType组合值
	u16 wRcModeNum;													///< 码率控制模式数量
	TNetRcModeItem atRcModeItem[NET_VIDEOENC_RECMODE_MAX_NUM];		///< 码率控制信息列表
	u32 dwImgQualityMask;											///< 编码质量类型掩码，EImgQualityType组合值
	u32 dwEncProfileMask;											///< 编码复杂度类型掩码，EVideoEncProfileType组合值
	s32 nMaxKeyRateMin;												///< 关键帧间隔范围
	s32 nMaxKeyRateMax;
	s32 nBitRateMin;												///< 码率上限范围
	s32 nBitRateMax;
	BOOL32 bSupSmartEnc;										    ///< 是否支持smart编码
	BOOL32 bSupBitSmooth;											///< 是否支持码流平滑
	s32 nBitSmoothMin;												///< 码流平滑范围
	s32 nBitSmoothMax;
}NETPACKED;

struct TNetVideoEncParamItem 
{
	u16 wEncId;                                                            ///< 编码通道id
	EVideoEncType eEncType;                                                ///< 编码类型
	u32 dwEncTypeMask;                                                     ///< 编码类型掩码，EVideoEncType组合值
	u16 wEncTypeNums;                                                      ///< 支持的编码类型数量
	TNetVideoEncCapItem atVideoEncCapItem[NET_VIDEOENC_ENCTYPE_MAX_NUM];   ///< 编码类型能力信息列表
	ERcModeType eRcModeType;                                               ///< 码率控制模式
	s32 nFrameRate;                                                        ///< 视频帧率
	TNetEncResolution tEncResolution;                                      ///< 编码分辨率
	s32 nMaxBitRate;                                                       ///< 编码码率上限
	s32 nMaxKeyRate;                                                       ///< I帧间隔
	EImgQualityType eImgQualityType;                                       ///< 图像质量
	EVideoEncProfileType eEncProfileType;                                  ///< 编码复杂度
	BOOL32 bEnableSmartEnc;												   ///< 是否开启smart编码
	s32 nBitSmoothLevel;                                                   ///< 码流平滑
}NETPACKED;

struct TNetChnVideoEncParam
{
	s32 nChnId;                                                            ///< 通道id
	u16 wEncChnNum;                                                        ///< 编码通道数量
	TNetVideoEncParamItem atVideoEncParamItem[NET_PER_CHN_ENCCHN_MAX_NUM]; ///< 编码通道信息
}NETPACKED;

struct TNetAudioEncCapItem
{
	EAudioEncType eAudioEncType;                                            ///< 音频编码类型
	u32 dwAudioSampleRateTypeMask;                                          ///< 采样率类型掩码，EAudioSampleRateType组合值
	u16 wEncVolMin;                                                         ///< 编码音量最小值
	u16 wEncVolMax;                                                         ///< 编码音量最大值
}NETPACKED;

struct TNetAudioEncParamItem 
{
	u16 wEncId;                                                             ///< 音频编码通道ID
	EAudioInType eAudioInType;                                              ///< 音频源类型
	EAudioEncType eAudioEncType;                                            ///< 音频编码类型
	EAudioSampleRateType eAudioSampleRateType;                              ///< 编码采样率
	u16 wEncVol;                                                            ///< 编码音量
	u32 dwAudioEncTypeMask;                                                 ///< 音频编码类型掩码，EAudioEncType组合值
	u16 wEncTypeNum;                                                        ///< 编码类型数量
	TNetAudioEncCapItem atAudioEncCapItem[NET_AUDIOENC_ENCTYPE_MAX_NUM];    ///< 编码类型能力信息列表	
}NETPACKED;

struct TNetChnAudioEncParam
{
	s32 nChnId;                                                             ///< 通道id
	u16 wEncChnNum;                                                         ///< 编码通道数量
	u32 dwAudioInTypeMask;                                                  ///< 当前支持音频源类型掩码，EAudioInType组合值
	EAudioInType eCurAudioInType;											///< 当前选中的音频值
	TNetAudioEncParamItem atAudioEncParamItem[NET_PER_CHN_ENCCHN_MAX_NUM];  ///< 编码通道信息
}NETPACKED;

struct TNetChnAudioDecParam 
{
	s32 nChnId;                                                             ///< 通道id
	BOOL32 bEnableAudMixRec;												///< 是否支持混音录像
	BOOL32 bAudMixRec;                                                      ///< 是否混音录像
	u16 wAudDecVol;                                                         ///< 解码音量
	u16 wAudDecVolMin;                                                      ///< 解码音量最小值
	u16 wAudDecVolMax;                                                      ///< 解码音量最大值
}NETPACKED;

struct TNetCordonCfg
{
	s32 nErrNo;					///< 错误号 NET_OK：获取成功
	BOOL32 bEnable;				///< 是否启用
	s32 nCurSchemeId;			///< 当前方案，从1开始
	s32 nSchemeMaxNum;			///< 方案最大数量
	s32 nVgaWidth;				///< 画布宽度
	s32 nVgaHeight;				///< 画布高度
	BOOL32 bSupPreset;			///< 是否支持关联预置位
	s32 nPresetId;				///< 预置位id，从0开始，0：表示内置预置位
	s32 nPresetMaxNum;			///< 预置位最大数量，不包含预置位0
	ECordonType eCordonType;	///< 警戒线方向
	u32 dwCordonTypeMask;		///< 警戒线方向掩码，ECordonType组合值
	s32 nTargetRatio;			///< 目标比例
	s32 nTargetRatioMin;		///< 目标比例最小值
	s32 nTargetRatioMax;		///< 目标比例最大值
	s32 nSensi;					///< 灵敏度
	s32 nSensiMin;				///< 灵敏度最小值
	s32 nSensiMax;				///< 灵敏度最大值
	TNetRect tTargetFilterRect;	///< 目标过滤区域
	TNetPoint tStartPoint;		///< 警戒线起点
	TNetPoint tEndPoint;		///< 警戒线终点
	TNetPoint tAPoint;			///< A区域点坐标
	TNetPoint tBPoint;			///< B区域点坐标
}NETPACKED;

struct TNetAreaCfg
{
	s32 nErrNo;									///< 错误号 NET_OK：获取成功
	BOOL32 bEnable;								///< 是否启用
	s32 nCurSchemeId;							///< 当前方案，从1开始
	s32 nSchemeMaxNum;							///< 方案最大数量
	s32 nVgaWidth;								///< 画布宽度
	s32 nVgaHeight;								///< 画布高度
	BOOL32 bSupPreset;							///< 是否支持关联预置位
	s32 nPresetId;								///< 预置位id，从0开始，0：表示内置预置位
	s32 nPresetMaxNum;							///< 预置位最大数量，不包含预置位0
	s32 nTargetRatio;							///< 目标比例
	s32 nTargetRatioMin;						///< 目标比例最小值
	s32 nTargetRatioMax;						///< 目标比例最大值
	s32 nSensi;									///< 灵敏度
	s32 nSensiMin;								///< 灵敏度最小值
	s32 nSensiMax;								///< 灵敏度最大值
	BOOL32 bSupTimeThreshold;					///< 是否支持时间阈值，区域进入、区域离开无效
	s32 nTimeThreshold;							///< 时间阈值，区域进入、区域离开无效
	s32 nTimeThresholdMin;						///< 时间阈值最小值，区域进入、区域离开无效
	s32 nTimeThresholdMax;						///< 时间阈值最大值，区域进入、区域离开无效
	TNetRect tTargetFilterRect;					///< 目标过滤区域
	s32 nPointMaxNum;							///< 点的最大数量
	s32 nPointNum;								///< 点的数量
	TNetPoint atPoint[NET_AREA_POINT_MAX_NUM];	///< point
}NETPACKED;

struct TNetVirFocusDetectCfg
{
	s32 nErrNo;     ///< 错误号 NET_OK：获取成功
	BOOL32 bEnable;	///< 是否启用
	s32 nSensi;		///< 灵敏度
	s32 nSensiMin;	///< 灵敏度最小值
	s32 nSensiMax;	///< 灵敏度最大值
}NETPACKED;

struct TNetSceneChangeCfg
{
	s32 nErrNo;				///< 错误号 NET_OK：获取成功
	BOOL32 bEnable;			///< 是否启用
	ECheckMode eCheckMode;	///< 检测模式
	u32 dwCheckModeMask;	///< 检测模式掩码，ECheckMode组合值
	s32 nSensi;				///< 灵敏度
	s32 nSensiMin;			///< 灵敏度最小值
	s32 nSensiMax;			///< 灵敏度最大值
}NETPACKED;

struct TNetPeopleGatherCfg
{
	s32 nErrNo;									///< 错误号 NET_OK：获取成功
	BOOL32 bEnable;								///< 是否启用
	s32 nCurSchemeId;							///< 当前方案，从1开始
	s32 nSchemeMaxNum;							///< 方案最大数量
	s32 nVgaWidth;								///< 画布宽度
	s32 nVgaHeight;								///< 画布高度
	BOOL32 bSupPreset;							///< 是否支持关联预置位
	s32 nPresetId;								///< 预置位id，从0开始，0：表示内置预置位
	s32 nPresetMaxNum;							///< 预置位最大数量，不包含预置位0
	s32 nRatio;									///< 占比
	s32 nRatioMin;								///< 占比最小值
	s32 nRatioMax;								///< 占比最大值
	s32 nPointMaxNum;							///< 点的最大数量
	s32 nPointNum;								///< 点的数量
	TNetPoint atPoint[NET_AREA_POINT_MAX_NUM];	///< point
}NETPACKED;

struct TNetAudioAbnormalCfg
{
	s32 nErrNo;						///< 错误号 NET_OK：获取成功
	BOOL32 bEnable;					///< 是否启用
	s32 nSensi;						///< 灵敏度
	s32 nSensiMin;					///< 灵敏度最小值
	s32 nSensiMax;					///< 灵敏度最大值
	s32 nSoundIntenThreshold;		///< 声音强度阈值
	s32 nSoundIntenThresholdMin;	///< 声音强度阈值最小值
	s32 nSoundIntenThresholdMax;	///< 声音强度阈值最大值
}NETPACKED;

struct TNetAudioMeter
{
	s32 nErrNo;						///< 错误号 NET_OK：获取成功
	s32 nAudMeter;					///< 音量波形图实时数值
}NETPACKED;

struct TNetFaceDetectCfg
{
	s32 nErrNo;						///< 错误号 NET_OK：获取成功
	BOOL32 bEnable;					///< 是否启用
}NETPACKED;

struct TNetAccessProtectCfg
{
	s32 nErrNo;						///< 错误号 NET_OK：获取成功
	BOOL32 bEnable;					///< 是否启用
}NETPACKED;

struct TNetRecMgrCap 
{
	BOOL32 bRecAudioCap;            ///< 是否支持录音频
	BOOL32 bStartAnrCap;            ///< 是否支持转录
	BOOL32 bHolidayCap;             ///< 是否支持假日
	s32 nHolidayNum;                ///< 假日个数
	u32 dwRecModeTypeMask;          ///< 录像模式类型掩码，ERecModeType组合值
	u32 dwRecStreamTypeMask;        ///< 录像码流类型掩码，ERecStreamType组合值
	BOOL32 bSupThumbnail;           ///< 是否支持缩略图
	BOOL32 bSupSlowRecover;         ///< 是否支持淡忘式
	s32 nMaxSegPlayNum;             ///< 分段回放最大分段数
}NETPACKED;

struct TNetRecTimePlanItem
{
	s32 nStartTime;                       ///< 时间段开始时间
	s32 nEndTime;                         ///< 时间段结束时间
}NETPACKED;

struct TNetRecDayTimePlan
{
	ERecDayType eRecDayType;                                                  ///< 日期类型
	u16 wItemNum;                                                             ///< 当天时间段数
	TNetRecTimePlanItem atRecTimePlanItem[NET_PRE_DAY_RECTIMEPLAN_MAX_NUM];   ///< 时间段信息
}NETPACKED;

struct TNetChnRecPlan
{
	s32 nChnId;                                                   ///< 通道ID
	ERecModeType eRecModeType;                                    ///< 录像模式类型
	ERecStreamType eRecStreamType;                                ///< 录像码流类型
	u32 dwRecStreamTypeMask;                                      ///< 录像码流类型掩码, ERecStreamType类型组合值
	BOOL32 bRecAudio;                                             ///< 是否记录音频
	s32 nRecOverTime;                                             ///< 录像过期时间,单位小时
	TNetRecDayTimePlan atRecDayTimePlan[NET_RECTIMEPLAN_MAX_NUM]; ///< 录像时间计划
}NETPACKED;

struct TNetCopyChnRecPlan
{
	s32 nSrcChnId;                                                ///< 源通道ID
	s32 nDstChnNum;                                               ///< 目的通道ID数量
	s32 anDstChnId[NET_COPYCHNRECPLAN_MAX_NUM];                   ///< 目的通道ID列表
}NETPACKED;

struct TNetSnapTimePlanItem
{
	s32 nStartTime;                       ///< 时间段开始时间
	s32 nEndTime;                         ///< 时间段结束时间
}NETPACKED;

struct TNetSnapDayTimePlan
{
	ERecDayType eSnapDayType;                                                   ///< 日期类型
	u16 wItemNum;                                                               ///< 当天时间段数
	TNetSnapTimePlanItem atSnapTimePlanItem[NET_PRE_DAY_RECTIMEPLAN_MAX_NUM];   ///< 时间段信息
}NETPACKED;

struct TNetChnSnapPlan
{
	s32 nChnId;                                                     ///< 通道ID
	ESnapModeType eSnapModeType;                                    ///< 录像模式类型
	s32 nAlarmInterval;                                             ///< 告警抓拍间隔,单位秒
	s32 nMinAlarmInterval;                                          ///< 告警抓拍最小间隔,单位秒
	s32 nMaxAlarmInterval;                                          ///< 告警抓拍最大间隔,单位秒
	BOOL32 bSupTimerInterval;										///< 是否支持定时抓拍
	s32 nTimerInterval;                                             ///< 定时抓拍间隔,单位秒
	s32 nMinTimerInterval;                                          ///< 定时抓拍最小间隔,单位秒
	s32 nMaxTimerInterval;                                          ///< 定时抓拍最大间隔,单位秒
	TNetSnapDayTimePlan atSnapDayTimePlan[NET_RECTIMEPLAN_MAX_NUM]; ///< 录像时间计划
}NETPACKED;

struct TNetCopyChnSnapPlan
{
	s32 nSrcChnId;                                                ///< 源通道ID
	s32 nDstChnNum;                                               ///< 目的通道ID数量
	s32 anDstChnId[NET_COPYCHNRECPLAN_MAX_NUM];                   ///< 目的通道ID列表
}NETPACKED;

struct TNetTime
{
	s32 nHour;                ///< 时
	s32 nMin;                 ///< 分
	s32 nSec;                 ///< 秒
}NETPACKED;

struct TNetRecAdvanceOption
{
	BOOL32 bStopRecSpaceFull;                                     ///< 录像空间满后是否停止
	BOOL32 bStopPicSpaceFull;                                     ///< 图片空间满后是否停止
	s32 nPreRecTime;                                              ///< 预录时长
	s32 nMinPreRecTime;                                           ///< 预录时长最小值
	s32 nMaxPreRecTime;                                           ///< 预录时长最大值
	s32 nRecDelay;                                                ///< 录像延时
	s32 nMinRecDelay;                                             ///< 录像延时最小值
	s32 nMaxRecDelay;                                             ///< 录像延时最大值
	ERecDownloadSpeed eRecDownloadSpeed;                          ///< 录像下载速度
	u32 dwRecDownloadSpeed;                                       ///< 录像下载速度掩码，ERecDownloadSpeed组合值
	BOOL32 bEnableAnr;                                            ///< 是否开启转录
	BOOL32 bEnableLongTimeRecCfg;                                 ///< 是否启用超长时间录像配置
	s32 nFullRecTime;                                             ///< 完整录像时长
	s32 nEventRecTime;                                            ///< 时间录像时长
	s32 nSmallVideoPreTime;										  ///< 小视频长度前多少秒
	s32 nMinSmallVideoPreTime;									  ///< 小视频长度前多少秒最小值
	s32 nMaxSmallVideoPreTime;									  ///< 小视频长度前多少秒最大值
	s32 nSmallVideoOverTime;									  ///< 小视频长度后多少秒
	s32 nMinSmallVideoOverTime;									  ///< 小视频长度后多少秒最小值
	s32 nMaxSmallVideoOverTime;									  ///< 小视频长度后多少秒最大值
	BOOL32 bSupNoRecCheck;                                        ///< 是否支持无录像配置参数
	s32 nNoRecDays;												  ///< 录像保存时长（天）
	s32 nNoRecDaysMin;									          ///< 录像保存时长最小值
	s32 nNoRecDaysMax;									          ///< 录像保存时长最大值
	TNetTime tNoRecTime;										  ///< 录像保存时间
}NETPACKED;

struct TNetHolidayTime
{
	s32 nYear;                   ///< 年，按日方式生效
	s32 nMonth;                  ///< 月，按日方式，按月方式，按周方式生效
	s32 nDay;                    ///< 天，按日方式，按月方式生效
	s32 nWeek;                   ///< 第几周，按周方式生效
	s32 nWeekDay;                ///< 周几，按周方式生效
}NETPACKED;

struct TNetHolidayCfg
{
	s32 nId;                                  ///< 假日ID
	BOOL32 bEnable;                           ///< 是否启用
	s8 szName[NET_MAXLEN_64 + 1];             ///< 假日名称
	EHolidaySelMode eHolidaySelMode;          ///< 假日选择模式
	TNetHolidayTime tHolidayStartTime;        ///< 假日开始时间
	TNetHolidayTime tHolidayEndTime;          ///< 假日结束时间
}NETPACKED;

struct  TNetHolidayCfgList
{
	s32 nHolidayNum;                                                ///< 假日数量
	TNetHolidayCfg atHolidayCfg[NET_HOLIDAY_MAX_NUM];               ///< 假日配置数组
}NETPACKED;

struct  TNetRecBackupItem
{
	s32 nChnId;                             ///< 通道ID;
	s8 szStartTime[NET_MAXLEN_32];          ///< 开始时间(格式为:2016-03-15T12:48:01.330)
	s8 szEndTime[NET_MAXLEN_32];            ///< 结束时间(格式为:2016-03-15T12:48:01.330)
	s32 nStartTime;                         ///< 开始时间(可选项，可不初始化)
	s32 nDur;                               ///< 录像持续时间(可选项，可不初始化)
}NETPACKED;

struct  TNetRecBackupData
{
	s32 nRecBackupItemNum;                                                          ///< 备份数据数量数量
	TNetRecBackupItem atRecBackupItem[NET_PER_CHNGETRECLIST_MAX_NUM];               ///< 备份数据数组
}NETPACKED;

struct  TNetRecBackupStateItem
{
	u64 ullCurrTime;                                       ///< 当前拷贝时间（为当前天的偏移时间）
	s8 szCurrTime[NET_MAXLEN_32];                          ///< 当前拷贝时间(格式为:2016-03-15T12:48:01.330)
	EBackupState eRecBackupState;                          ///< 备份状态
}NETPACKED;

struct  TNetRecBackupState
{
	s32 nRecBackupStateItemNum;                                                          ///< 备份数据数量数量
	TNetRecBackupStateItem atRecBackupStateItem[NET_PER_CHNGETRECLIST_MAX_NUM];          ///< 备份数据数组
}NETPACKED;

struct  TNetLockRecItem
{
	s32 nChnId;                            ///< 通道id
	s8 szStartTime[NET_MAXLEN_32];         ///< 开始时间
    s8 szEndTime[NET_MAXLEN_32];           ///< 结束时间
}NETPACKED;

struct  TNetLockRecList
{
	s32 nLockRecNum;                                                       ///< 锁定录像数量
	TNetLockRecItem atLockRecItem[NET_PER_LOCK_REC_MAX_NUM];               ///< 锁定录像数据数组
}NETPACKED;

struct  TNetDataTime
{
	s32 nYear;                ///< 年
	s32 nMonth;               ///< 月
	s32 nDay;                 ///< 日
	s32 nHour;                ///< 时
	s32 nMin;                 ///< 分
	s32 nSec;                 ///< 秒
	s32 nMSec;                ///< 毫秒
	s8 szTime[NET_MAXLEN_32]; ///< 时间
}NETPACKED;

struct TNetDate
{
	s32 nYear;  ///< 年
	s32 nMonth; ///< 月
	s32 nDay;   ///< 日
}NETPACKED;

struct  TNetLockRecDataItem
{
	s32 nChnId;                     ///< 通道ID
	u64 ullLockRecSize;             ///< 录像数据大小
	TNetDataTime tStartTime;        ///< 开始时间
	TNetDataTime tEndTime;          ///< 结束时间
}NETPACKED;

struct  TNetLockRecData
{
	s32 nLockRecNum;                                                               ///< 锁定录像数量
	TNetLockRecDataItem atLockRecDataItem[NET_PER_LOCK_REC_MAX_NUM];               ///< 录像数据大小数组
}NETPACKED;

struct  TNetPicBackupItem
{
	s32 nChnId;                             ///< 通道ID;
	s8 szPicUri[NET_MAXLEN_256];            ///< 图片uri
}NETPACKED;

struct  TNetPicBackupData
{
	s32 nPicBackupItemNum;                                                          ///< 备份数据数量数量
	TNetPicBackupItem atPicBackupItem[NET_PER_CHNGETRECLIST_MAX_NUM];               ///< 备份数据数组
}NETPACKED;

struct  TNetPicBackupStateItem
{
	EBackupState ePicBackupState;                       ///< 备份状态
}NETPACKED;

struct  TNetPicBackupState
{
	s32 nPicBackupStateItemNum;                                                          ///< 备份数据数量数量
	TNetPicBackupStateItem atPicBackupStateItem[NET_PER_CHNGETRECLIST_MAX_NUM];          ///< 备份数据数组
}NETPACKED;

struct TNetGetChnMonthRec
{
	s32 nChnNum;                                       ///< 通道数量
	s32 anChnId[NET_PER_CHNGETMONTHREC_MAX_NUM];       ///< 通道id数组
	s32 nYear;                                         ///< 年份
	s32 nMonth;                                        ///< 月份
}NETPACKED;

struct TNetChnMonthRecItem
{
	s32 nChnId;                                        ///< 通道id
	u32 dwMonthRec;                                    ///< 月视图信息
}NETPACKED;

struct TNetChnMonthRecList
{
	s32 nChnNum;                                                             ///< 通道数量
	TNetChnMonthRecItem atChnMonthRecItem[NET_PER_CHNGETMONTHREC_MAX_NUM];   ///< 月视图信息
}NETPACKED;

struct TNetCreatQueryRecTask 
{
	s32 nChnId;                             ///< 通道id
	BOOL32 bQueryRecord;                    ///< 是否记录搜索
	s8 szStartTime[NET_MAXLEN_32];          ///< 开始时间(格式为:2016-03-15T12:48:01.330)
	s8 szEndTime[NET_MAXLEN_32];            ///< 结束时间(格式为:2016-03-15T12:48:01.330)
	u32 dwRecEventTypeMask;                 ///< 录像事件类型掩码，ERecEventType组合值
	BOOL32 bMergedRec;                      ///< 是否合并录像记录
}NETPACKED;

struct TNetGetRecTaskResult 
{
	s32 nTaskId;                            ///< 任务id
	s32 nStartIndex;                        ///< 起始记录索引
	s32 nNeedNum;                           ///< 希望获取的最大记录条数
}NETPACKED;

struct TNetChnRecItem
{
	u64 ullRecSize;                          ///< 录像大小,单位BYTE
	u64 ullStartTime;                        ///< 起始时间(单位：毫秒)
	s8 szStartTime[NET_MAXLEN_32];           ///< 开始时间(格式为:2016-03-15T12:48:01.330)
	u32 dwRecDur;                            ///< 录像时长(单位：毫秒)
	ERecEventType eRecEventType;             ///< 录像事件类型
	BOOL32 bLocked;                          ///< 录像是否锁定
	s8 szThumbnailUrl[NET_MAXLEN_512 + 1];   ///< 录像记录对应缩略图URL
}NETPACKED;

struct TNetChnRecList
{
	BOOL32 bFinished;                                            ///< 是否完成
	s32 nTotalRecNum;                                            ///< 总的录像数量
	s32 nCurrRecNum;                                             ///< 当前录像数量
	s32 nRealRecNum;                                             ///< 合并前录像数量
	TNetChnRecItem atChnRecItem[NET_PER_CHNGETRECLIST_MAX_NUM];  ///< 通道录像数组
}NETPACKED;

struct TNetPlaybackItem
{
	s32 nWinId;                              ///< 窗格Id
	s32 nChnId;                              ///< 通道Id
	s8 szStartTime[NET_MAXLEN_32];           ///< 开始时间(格式为:2016-03-15T12:48:01.330)
	s8 szEndTime[NET_MAXLEN_32];             ///< 结束时间(格式为:2016-03-15T12:48:01.330)
	s8 szResStartTime[NET_MAXLEN_32];        ///< 资源开始时间(格式为:2016-03-15T12:48:01.330)
	s8 szResEndTime[NET_MAXLEN_32];          ///< 资源结束时间(格式为:2016-03-15T12:48:01.330)
	s8 szUri[NET_MAXLEN_512];            ///< 文件路径（asf，mp4回放） 
}NETPACKED;

struct TNetPlaybackList
{
	s32 nItemNums;                                                      ///< item数量
	TNetPlaybackItem atPlaybackItem[NET_PER_CHNPLAYBACKLIST_MAX_NUM];   ///< item信息
}NETPACKED;

struct TNetRecFileInfo
{
	s8 szUri[NET_MAXLEN_512];                ///< 文件路径（asf，mp4回放）
	u64 ullFileSize;                         ///< 文件大小
	u64 ullStartTime;                        ///< 录像开始时间(单位：毫秒)
	s32 nDuration;                           ///< 录像时长
}NETPACKED;

struct TNetPlaybackVcrCtrl
{
	s32 nTaskId;                              ///< 任务Id
	s32 nChnId;                               ///< 通道Id
	BOOL32 bBackwards;                        ///< 是否倒放
	EPlaybackVcrCmdType ePlaybackVcrCmdType;  ///< vcr操作类型
	EPlaybackFrameMode ePlaybackFrameMode;    ///< 回放帧模式选择（非调整速度的vcr控制不生效）
	s8 szSeekTime[NET_MAXLEN_32];             ///< 要拖拽到的本地时间,精确到毫秒(格式为:2016-03-15T12:48:01.330)
	s32 nSkipTime;                            ///< 前跳后跳时间间隔
}NETPACKED;

struct TNetPlaybackState
{
	u64 ullPlaybackTime;                      ///< 回显时间(单位：毫秒)
	EPlaybackVcrState ePlaybackState;         ///< 回放回显状态
	s8 szCurrPlayTime[NET_MAXLEN_32];         ///< 当前播放位置,精确到毫秒(格式为:2016-03-15T12:48:01.330)
	s32 nCurrRtpTime;                         ///< rtp时间
}NETPACKED;

struct TNetEventCyclePlaybackInfo
{
	s32 nTaskId;                              ///< 任务Id
	s32 nChnId;                               ///< 通道Id(同步方式该参数无效)
	s8 szStartTime[NET_MAXLEN_32];            ///< 开始时间(格式为:2016-03-15T12:48:01.330)
	s8 szEndTime[NET_MAXLEN_32];              ///< 结束时间(格式为:2016-03-15T12:48:01.330)
	BOOL32 bFilePlayback;                         ///< 是否文件回放
}NETPACKED;

struct TNetQuerySnapPicInfo
{
	s32 nChnId;                               ///< 通道Id
	s8 szStartTime[NET_MAXLEN_32];            ///< 开始时间(格式为:2016-03-15T12:48:01.330)
	s8 szEndTime[NET_MAXLEN_32];              ///< 结束时间(格式为:2016-03-15T12:48:01.330)
	ESnapPicEventType eSnapPicEventType;      ///< 查询的抓拍图片类型
	s32 nStartIndex;                          ///< 要查询的起始索引
	s32 nNeedNum;                             ///< 本次需要查询的数量   
}NETPACKED;

struct TNetSnapPicInfo 
{
	s8 szUri[NET_MAXLEN_512 + 1];              ///< 图片唯一标识
	s8 szSnapPicTime[NET_MAXLEN_32];           ///< 图片抓拍时间
	u32 dwSnapPicEventMask;                    ///< 抓拍图片事件类型，ESnapPicEventType组合值
	s32 nPicSize;                              ///< 图片大小，单位字节
	s32 nPicWidth;                             ///< 图片宽度
	s32 nPicHeight;                            ///< 图片高度
}NETPACKED;

struct TNetSnapPicList
{
	BOOL32 bFinished;                                             ///< 查询是否完成
	s32 nTotalNum;                                                ///< 总的图片数量
	s32 nCurrNum;                                                 ///< 本次图片数量
	TNetSnapPicInfo atSnapPicInfo[NET_PER_QUERYSNAPPIC_MAX_NUM];  ///< 图片信息数组
}NETPACKED;

struct TNetGetSnapPic
{
	s32 nChnId;									///< 通道Id
	s8 szUri[NET_MAXLEN_512 + 1];               ///< 图片唯一标识
}NETPACKED;

struct TNetRecTagInfo 
{
	s32 nChnId;                         ///< 通道ID
	s8 szUri[NET_MAXLEN_256];           ///< 标签唯一标识
	s8 szTime[NET_MAXLEN_32];           ///< 标签时间
	s32 nTime;                          ///< 当天时间偏移
	s8 szTagName[NET_MAXLEN_64 + 1];    ///< 标签名字（内容）
}NETPACKED;

struct  TNetRecTagList
{
	s32 nNum;                                                   ///< 数量
	TNetRecTagInfo atRecTagInfo[NET_PER_QUERYRECTAG_MAX_NUM];   ///< 标签信息数组
}NETPACKED;

struct  TNetRecTagOperateParam
{
	ERecTagOperaType eRecTagOperaType;                ///< 标签操作类型
	TNetRecTagInfo tRecTagInfo;                       ///< 录像标签信息
	TNetRecTagInfo tOldRecTagInfo;                    ///< 旧的录像标签信息（TAGOPERATE_UPDATE操作时生效）
}NETPACKED;

struct  TNetCreateQueryRecTagTaskParam
{
	s32 nChnId;                              ///< 通道ID
	s8 szStartTime[NET_MAXLEN_32];           ///< 开始时间
	s8 szEndTime[NET_MAXLEN_32];             ///< 结束时间
	s8 szTagKey[NET_MAXLEN_64 + 1];          ///< 标签检索关键字
}NETPACKED;

struct TNetQueryRecTagResultParam
{
	s32 nTaskId;                         ///< 通道ID
	s32 nBeginIndex;                     ///< 起始索引
	s32 nQueryNum;                       ///< 查询数量
}NETPACKED;

struct  TNetQueryRecTagResult
{
	BOOL32 bFinished;                        ///< 查询是否完成
	TNetRecTagList tRecTagList;              ///< 录像标签列表            
}NETPACKED;

struct TNetDstVideoItem
{
	u16 wDstRtpPort;	///< 目的rtp端口
	u16 wDstRtcpPort;	///< 目的rtcp端口
	u8 byPayload;		///< payload
}NETPACKED;

struct TNetDstAudioItem
{
	u16 wDstRtpPort;	///< 目的rtp端口
	u16 wDstRtcpPort;	///< 目的rtcp端口
	u8 byPayload;		///< payload
}NETPACKED;

struct TNetPlaybackDstChnItem
{
	s32 nChnId;													///< 通道id
	s8 szStartTime[NET_MAXLEN_32];								///< 开始时间
	s8 szEndTime[NET_MAXLEN_32];								///< 结束时间
	s8 szResStartTime[NET_MAXLEN_32];							///< 资源开始时间
	s8 szResEndTime[NET_MAXLEN_32];								///< 资源结束时间
	u32 dwDstIp;												///< 码流传输目的地址，网络序
	s32 nDstVideoItemNum;										///< 视频数量
	TNetDstVideoItem atDstVideoItem[NET_PLAYBACK_ITEM_MAX_NUM];	///< 视频信息
	s32 nDstAudioItemNum;										///< 音频数量
	TNetDstAudioItem atDstAudioItem[NET_PLAYBACK_ITEM_MAX_NUM];	///< 音频信息
}NETPACKED;

struct TNetPlaybackDstChnList
{
	s32 nNum;															///< 数量
	TNetPlaybackDstChnItem atPlaybackDstChnItem[NET_PLAYBACK_MAX_NUM];	///< 录像回放通道信息
}NETPACKED;

struct TNetSrcVideoItem
{
	u16 wSrcRtpPort;	///< 源rtp端口
	u16 wSrcRtcpPort;	///< 源rtcp端口
}NETPACKED;

struct TNetSrcAudioItem
{
	u16 wSrcRtpPort;	///< 源rtp端口
	u16 wSrcRtcpPort;	///< 源rtcp端口
}NETPACKED;

struct TNetPlaybackSrcChnItem
{
	s32 nChnId;													///< 通道id
	s32 nSrcVideoItemNum;										///< 视频数量
	TNetSrcVideoItem atSrcVideoItem[NET_PLAYBACK_ITEM_MAX_NUM];	///< 视频信息
	s32 nSrcAudioItemNum;										///< 音频数量
	TNetSrcAudioItem atSrcAudioItem[NET_PLAYBACK_ITEM_MAX_NUM];	///< 音频信息
}NETPACKED;

struct TNetPlaybackSrcChnList
{
	s32 nNum;															///< 数量
	TNetPlaybackSrcChnItem atPlaybackSrcChnItem[NET_PLAYBACK_MAX_NUM];	///< 录像回放通道信息
}NETPACKED;

struct TNetDiskMgrCap
{
	u32 dwNetDiskType;	          ///< 支持的网络硬盘类型，ENetDiskType组合值
	BOOL32 bSupRaid;              ///< 是否支持raid
	u32 dwRaidLevel;	          ///< 支持的RAID的级别，ERaidLevel组合值
	u32 dwSmartTestDiskType;      ///< 支持SMART检测的硬盘类型，EDiskType组合值
	u32 dwBadSectorCheckDiskType; ///< 支持坏道检测检测的硬盘类型，EDiskType组合值
	BOOL32 bSupDiskCust;		  ///< 是否支持自定义分区
	BOOL32 bSupSmartStorage;	  ///< 是否支持智能存储
}NETPACKED;

struct TNetDiskPartCap
{
	u8 byId;					///< 分区id
	EDiskPartType ePartType;	///< 分区类型
}NETPACKED;

struct TNetDiskPartType
{
	u8  byDiskId;														///< 硬盘id
	s32 nNum;															///< 分区个数
	TNetDiskPartCap atDiskPartCap[NET_DISK_PART_MAX_NUM];				///< 分区类型
}NETPACKED;

struct TNetDiskPartTypeList
{
	s32 nNum;															///< 磁盘个数
	TNetDiskPartType atDiskPartType[NET_NVR_DISK_MAX_NUM];				///< 分区用途
}NETPACKED;

struct TNetDiskPartTypeParam
{
	BOOL32 bAll;									///< 表示获取全部ID
	s32 nNum;										///< 磁盘个数
	u8  abyDiskId[NET_NVR_DISK_MAX_NUM];			///< 硬盘id 
}NETPACKED;

struct TNetDiskPartInfo
{
	u8 byId;					///< 分区id
	s32 nPartSize;				///< 分区大小
	s32 nFreeSize;				///< 剩余大小
}NETPACKED;

struct TNetDiskInfo
{
	u8  byDiskId;					///< 硬盘id
	s32 nDiskCapacity;				///< 硬盘容量，单位MB
	EDiskState eDiskState;			///< 硬盘状态
	EDiskAttribute eDiskAttribute;	///< 硬盘属性
	BOOL32 bAttrCanModify;          ///< 硬盘属性是否可编辑
	EDiskType eDiskType;			///< 硬盘类型
	s32	nDiskFreeCapacity;			///< 硬盘剩余容量，单位MB
	u8	byDiskGroupId;				///< 盘组id
	EDiskSmartTestState eDiskSmartTestState; ///< 硬盘smart检测状态
	s32 nPartNum;					///< 分区个数
	s32 nPartMax;					///< 最大分区数
	TNetDiskPartInfo atNetDiskPartInfo[NET_DISK_PART_MAX_NUM];	///< 分区信息
	EDiskUsingType eDiskUsingType;	///< 硬盘存储用途
	s8 szNetDiskName[NET_MAXLEN_64 + 1];    ///< 网络磁盘名称
	s8 szNetDiskIpAddr[NET_MAXLEN_32];    ///< 网络磁盘IP地址
}NETPACKED;

struct TNetDiskList
{
	s16 swDiskNum;                                 ///< 硬盘数量
	TNetDiskInfo atDiskInfo[NET_NVR_DISK_MAX_NUM]; ///< 硬盘信息
}NETPACKED;

struct TNetDiskAttributeInfo
{
	u8  byDiskId;					///< 硬盘id
	EDiskAttribute eDiskAttribute;	///< 硬盘属性
}NETPACKED;

struct TNetDiskAttributeList
{
	s16 swDiskNum;                                          ///< 硬盘数量
	TNetDiskAttributeInfo atDiskInfo[NET_NVR_DISK_MAX_NUM]; ///< 硬盘信息
}NETPACKED;

struct TNetDiskUsingTypeParam
{
	u8  byDiskId;					///< 硬盘id
	EDiskUsingType eDiskUsingType;	///< 硬盘存储用途
}NETPACKED;

struct TNetDiskUsingTypeList
{
	s32 nDiskNum;                                          ///< 硬盘数量
	TNetDiskUsingTypeParam atDiskUsingTypeParam[NET_NVR_DISK_MAX_NUM];	///< 硬盘存储用途
}NETPACKED;

struct TNetDiskInitParam
{
	u8  byDiskId;					///< 硬盘id
	s32 nPartNum;					///< 分区个数
	TNetDiskPartInfo atNetDiskPartInfo[NET_DISK_PART_MAX_NUM];	///< 分区信息
}NETPACKED;

struct TNetNetDiskParam
{
	ENetDiskType eNetDiskType;				            ///< 网络硬盘类型
	s8  szDiskIp[NET_MAXLEN_32 + 1];		            ///< 硬盘IP
	u16 wDiskPort;							            ///< 硬盘端口
	s8  szUserName[NET_NVR_USER_NAME_MAX_LEN + 1];		///< 用户名
	s8  szPwd[NET_MAXLEN_32 + 1];			            ///< 密码
	s8  szNetDiskName[NET_MAXLEN_128 + 1];	            ///< 网络硬盘名称，仅用于添加网络硬盘
}NETPACKED;

struct TNetNetDiskInfo
{
	s8 szNetDiskName[NET_MAXLEN_128 + 1];	///< 网络硬盘名称
}NETPACKED;

struct TNetNetDiskList
{
	BOOL32 bFinished;	                                         ///< 搜索是否完成
	u8 byNetDiskNum;                                             ///< 网络硬盘数量
	TNetNetDiskInfo atNetDiskInfo[NET_PER_GET_NETDISK_MAX_NUM];  ///< 网络硬盘信息
}NETPACKED;

struct TNetDiskQuotaCfg
{
	s32 nChnId;                     ///< 通道ID
	s32 nRecQuota;					///< 录像配额
	s32 nRecUsedQuota;				///< 录像已用配额
	s32 nPicQuota;					///< 图片配额
	s32 nPicUsedQuota;				///< 图片已用配额
	s32 nDigestQuota;			    ///< 摘要配额
	s32 nDigestUsedQuota;			///< 摘要已用配额
	u64 ullTotalFreeQuotaSize;      ///< 剩余配额总容量(单位MB)
	u64 ullTotalDiskSize;           ///< 磁盘总容量(单位MB)
}NETPACKED;

struct TNetChnDiskQuotaCopyInfo 
{
	s32 nSrcChn;                              ///< 源通道ID
	s32 nDstNum;                              ///< 目的通道数目
	s32 anDstChn[NET_COPYDISKQUOTA_MAX_NUM];  ///< 目的通道
}NETPACKED;

struct TNetChnDiskGroupCfg
{
	s32 nDiskGroupId;                           ///< 硬盘分组ID
	s32 nChnNum;                                ///< 通道数量
	s32 anChnId[NET_DISKGROUP_CHN_MAX_NUM];     ///< 通道id数组
}NETPACKED;

struct TNetChnDiskGroupList
{
	s32 nNum;                                                             ///< 数量
	TNetChnDiskGroupCfg atChnDiskGroupCfg[NET_GROUP_MAX_NUM];             ///< 通道id数组
}NETPACKED;

struct  TNetDiskAdvanceCfg
{
	BOOL32 bEnableDiskSleep;   ///< 是否启用硬盘休眠
	s32 nDiskSleepTime;        ///< 空闲多少时间开启硬盘休眠
	s32 nDiskSleepTimeMax;     ///< 空闲最长时间开启硬盘休眠
	s32 nDiskSleepTimeMin;     ///< 空闲最短时间开启硬盘休眠
}NETPACKED;

struct TNetRaidDiskInfo
{
	s32 nDiskId;              ///< 硬盘id
	BOOL32 bHotBackupDisk;    ///< 是否为热备盘
}NETPACKED;

struct TNetCreateRaidParam
{
	ERaidLevel eRaidLevel;                                      ///< Raid等级
	ERaidCreateType eRaidCreateType;                            ///< 创建方式
	s8 szRaidName[NET_MAXLEN_64 + 1];                           ///< Raid名字
	s32 nDiskNum;                                               ///< 硬盘数量
	TNetRaidDiskInfo atRaidDiskInfo[NET_INTERNALDISK_MAX_NUM];  ///< 硬盘信息数组
}NETPACKED;

struct TNetRaidInfo
{
	ERaidLevel eRaidLevel;                                      ///< Raid等级
	s8 szRaidName[NET_MAXLEN_64 + 1];                           ///< Raid名字
	u64 ullRaidSize;                                            ///< raid空间大小
	ERaidStatus eRaidStatus;                                    ///< raid状态
	s32 nDiskNum;                                               ///< 硬盘数量
	TNetRaidDiskInfo atRaidDiskInfo[NET_INTERNALDISK_MAX_NUM];  ///< 硬盘信息数组
}NETPACKED;

struct TNetFrameCap
{
	u32 dwOutputTypeMask;				///< 输出类型掩码，EOutputType组合值
	s32 nGridItemMaxNum;				///< 画面风格窗格最大数量
	s8 szFrameStyleMask[NET_MAXLEN_64];	///< 画面风格掩码，例如：1,4,6,8,9
}NETPACKED;

struct TNetGridItem
{
	BOOL32 bShow;						///< 格子是否显示
	u16 wGridId;                        ///< grid id
	s32 nX;                             ///< grid left
	s32 nY;                             ///< grid top
	s32 nWidth;                         ///< grid width
	s32 nHeight;                        ///< grid height
}NETPACKED;

struct  TNetGridList
{
	s32 nFrameX;                                           ///< 画面风格区域左上角X坐标
	s32 nFrameY;                                           ///< 画面风格区域左上角Y坐标
	s32 nFrameWidth;                                       ///< 画面风格区域宽
	s32 nFrameHeight;                                      ///< 画面风格区域高
	EFrameStyle eFrameStyle;                               ///< 画面风格
	BOOL32 bSaveCfg;                                       ///< 是否保存到配置
	BOOL32 bStartAllMonitor;							   ///< 是否开启全部浏览
	BOOL32 bNotDestroyDec;			    				   ///< 是否不销毁解码器
	u16 wGridNum;                                          ///< 格子数
	TNetGridItem atGridItem[NET_FRAME_STYLE_GRID_MAX_NUM]; ///< 格子信息
}NETPACKED;

struct TNetBindItem
{
	u16 wGridId;		///< grid id
	s32 nChnId;			///< chn id
	BOOL32 bNotSave;	///< 是否不保存
}NETPACKED;

struct TNetBindList
{
	u16 wBindNum;                                ///< 绑定数量
	TNetBindItem atBindItem[NET_BIND_MAX_NUM];   ///< 绑定信息数组
}NETPACKED;

struct TNetPipItem
{
	BOOL32 bSetPip;		///< 是否设置画中画
	EPipType ePipType;  ///< 画中画类型
	s32 nTaskId;        ///< 回放任务ID
	s32 nChnId;			///< 通道id
	s32 nX;				///< 左上角坐标(偶数)
	s32 nY;				///< 左上角坐标(偶数)
	s32 nWidth;			///< 宽(偶数)
	s32 nHeight;		///< 高(偶数)
}NETPACKED;

struct TNetPipList
{
	u16 wPipNum;							///< 画中画数量
	TNetPipItem atPipItem[NET_PIP_MAX_NUM];	///< 画中画数组
}NETPACKED;

struct TNetChnSnap
{
	s32 nChnId;						///< 通道id
	EAiuSnapType eAiuSnapType;		///< 抓拍类型
	s32 nSnapNum;                   ///< 抓拍张数
}NETPACKED;

struct TNetChnSnapFileInfo
{
	u64 ullFileId;        ///< 抓拍的文件id
	s32 nErrCode;         ///< 抓拍错误码
}NETPACKED;

struct TNetVolume
{
	BOOL32 bMute; ///< 是否静音
	s32 nVolume;  ///< 音量
}NETPACKED;

struct TNetAudioDec
{
	BOOL32 bMonitor;			///< TRUE：浏览音频解码，FALSE：回放音频解码
	s32 nChnId;					///< 通道ID
	EAudioListen eAudioListen;	///< 音频通道
	s32 nPlaybackTaskId;		///< 回放任务id
}NETPACKED;

struct TNetCallCap
{
	u32 dwAudioEncTypeMask;                                                 ///< 音频编码类型掩码，EAudioEncType组合值
	u16 wEncTypeNum;                                                        ///< 编码类型数量
	TNetAudioEncCapItem atAudioEncCapItem[NET_AUDIOENC_ENCTYPE_MAX_NUM];    ///< 编码类型能力信息列表，wEncVolMin、wEncVolMin字段无效
	u32 dwTransTypeMask;													///< 传输方式掩码，ETransType组合值
	u32 dwStreamPackTypeMask;												///< 码流打包方式掩码，EStreamPackType组合值
}NETPACKED;

struct TNetCall
{
	s32 nCallId;         ///< 呼叫id，开始呼叫返回
	ECallMode eCallMode; ///< 呼叫方式：本地呼叫、本地广播
	ECallType eCallType; ///< 呼叫类型：呼叫通道
	s32 nChnId;			 ///< 呼叫通道id
}NETPACKED;

struct TNetRemoteCallParam
{
	EAudioEncType eAudioEncType;				///< 音频编码类型
	EAudioSampleRateType eAudioSampleRateType;	///< 编码采样率
	ETransType eTransType;						///< 传输方式
	EStreamPackType eStreamPackType;			///< 码流打包方式
	u32 dwIp;									///< 呼叫端的IP，主机序
	u16 wRtpPort;								///< 呼叫端的rtp端口
	u16 wRtcpPort;								///< 呼叫端的rtcp端口
}NETPACKED;

struct TNetCallParam
{
	ECallMode eCallMode;					///< 呼叫方式
	ECallType eCallType;					///< 呼叫类型
	s32 nChnId;								///< 呼叫通道id
	TNetRemoteCallParam tRemoteCallParam;	///< 远端呼叫参数，远端呼叫时有效
}NETPACKED;

struct TNetRemoteCallResult
{
	u16 wRtpPort;	///< nvr接收音频的rtp端口
	u16 wRtcpPort;	///< nvr接收音频的rtcp端口
}NETPACKED;

struct TNetCallResult
{
	s32 nCallId;							///< 呼叫id
	TNetRemoteCallResult tRemoteCallResult;	///< 远端呼叫参数，远端呼叫时有效
}NETPACKED;

struct TNetPtzCtrl
{
	s32 nChnId;			///< 通道id
	EPTZCMD ePtzCmd;	///< 云台控制命令
	s32 nIspSpeed;		///< ISP控制相关速度(0~100)，相关命令：PTZCMD_ZOOMTELE、PTZCMD_ZOOMWIDE、PTZCMD_FOCUSFAR、PTZCMD_FOCUSNEAR
	s32 nPanSpeed;		///< 水平转动速度(0~100)，相关命令：PTZCMD_MOVELEFT、PTZCMD_MOVERIGHT、PTZCMD_MOVELEFTUP、PTZCMD_MOVELEFTDOWN、PTZCMD_MOVERIGHTUP、PTZCMD_MOVERIGHTDOWN
	s32 nTilSpeed;		///< 垂直转动速度(0~100)，相关命令：PTZCMD_MOVEUP、PTZCMD_MOVEDOWN、PTZCMD_MOVELEFTUP、PTZCMD_MOVELEFTDOWN、PTZCMD_MOVERIGHTUP、PTZCMD_MOVERIGHTDOWN
	s32 nNumber;		///< 预置位(0~255)、巡航路径(1~32)，相关命令：PTZCMD_PRESET_CALL、PTZCMD_PRESET_SAVE、PTZCMD_PRESET_DEL、PTZCMD_PATHCRUISE_CALL、PTZCMD_PATHCRUISE_STOP
	s32 nX;				///< 框选缩放水平坐标(取值范围：0~255 相关命令：PTZCMD_GOTO_POINT、PTZCMD_ZOOM_PART、PTZCMD_ZOOM_WHOLE)
	s32 nY;				///< 框选缩放垂直坐标(取值范围：0~255 相关命令：PTZCMD_GOTO_POINT、PTZCMD_ZOOM_PART、PTZCMD_ZOOM_WHOLE)
	s32 nWidth;			///< 框选缩放宽(取值范围：0~255 相关命令：PTZCMD_GOTO_POINT、PTZCMD_ZOOM_PART、PTZCMD_ZOOM_WHOLE)
	s32 nHeight;		///< 框选缩放高(取值范围：0~255 相关命令：PTZCMD_GOTO_POINT、PTZCMD_ZOOM_PART、PTZCMD_ZOOM_WHOLE)
	s32 nWinWidth;		///< 自定义框选缩放宽，相关命令：PTZCMD_GOTO_POINT、PTZCMD_ZOOM_PART、PTZCMD_ZOOM_WHOLE
	s32 nWinHeight;		///< 自定义框选缩放高，相关命令：PTZCMD_GOTO_POINT、PTZCMD_ZOOM_PART、PTZCMD_ZOOM_WHOLE
}NETPACKED;

struct TNetPtzPreset
{
	s32 nPresetId;									///< 预置位ID
	BOOL32 bEnable;									///< 预置点使能
	s8 szName[NET_MAXLEN_64 + 1];					///< 预置位名称
	BOOL32 bSpecial;								///< 是否为特殊预置位(设置时不带此节点)
}NETPACKED;

struct TNetPtzPresetList
{
	s32 nMax;											///< 支持的最大预置位个数
	s32 nNum;											///< 预置位个数	
	TNetPtzPreset atPtzPreset[NET_PTZ_PRESET_MAX_NUM];  ///< 预置位列表
}NETPACKED;

struct TNetFishEyeModeCfg
{
	s32 nChnId;							///< 通道id
	EFishInstallType eFishInstallType;	///< 鱼眼安装方式
	u32 dwFishInstallTypeMask;			///< 鱼眼安装方式取值范围，EFishInstallType组合值
	BOOL32 bSupDevHardInfo;				///< 是否支持鱼眼硬解
	u32 dwFishDisplayTypeMask;			///< 显示方式取值范围，EFishDisplayType组合值(吸顶，墙面，桌面共用)
	EFishDisplayType eCeilDisplayType;	///< 吸顶显示方式
	EFishDisplayType eMtDisplayType;	///< 墙面显示方式
	EFishDisplayType eDtDisplayType;	///< 桌面显示方式
}NETPACKED;

struct TNetFishEyePtzRect
{
	s32 nIndex;				///< 索引
	s32 nX;					///< 区域中心点的x坐标
	s32 nY;					///< 区域中心点的y坐标
}NETPACKED;

struct TNetFishEyePtzRectCfg
{
	s32 nChnId;													///< 通道id
	s32 nRectNum;											    ///< 鱼眼的ptz区域个数
	TNetFishEyePtzRect atNetFishEyePtzRect[NET_FISH_EYE_RECT_MAX_NUM];		///< 鱼眼的ptz区域
	s32 nCanavsWidth;											///< 画布宽
	s32 nCanavsHeight;											///< 画布高
}NETPACKED;

struct TNetIspCaps
{
	s32 nChnId;								///< 通道id
	BOOL32 bSupImageAdjustment;				///< 是否支持图像效果
	u32 dwIspModeMask;						///< 支持的ISP模式(EIspNeedMask组合值)
}NETPACKED;

struct TNetIspCfg
{
	s32 nChnId;			///< 通道id
	s32 nBright;		///< 亮度
	s32 nContrast;		///< 对比度
	s32 nSaturation;	///< 饱和度
	s32 nSharpness;		///< 锐度
}NETPACKED;

struct TNetIspGainCfg
{
	s32 nChnId;					///< 通道id
	u32 dwGainModeMask;			///< 支持的增益模式(EIspGainMode组合值)
	EIspGainMode eGainMode;		///< 增益模式
	s32 nGainLevel;				///< 增益等级,手动模式下有效(设置时可选)(取值范围[0,100])
	s32 nGainMax;				///< 增益上限,自动模式下有效(设置时可选)(取值范围[0,100])
}NETPACKED;

struct TNetIspIrisCfg
{
	s32 nChnId;					///< 通道id
	u32 dwIrisModeMask;			///< 支持的光圈模式(EIspIrisMode组合值)
	EIspIrisMode eIrisMode;		///< 光圈模式
	s32 nIrisSize;				///< 光圈大小(设置时可选)(取值范围[0,100])
	s32 nIrisLevel;				///< 光圈灵敏度(设置时可选)(取值范围[0,100])
}NETPACKED;

struct TNetIspFocusCfg
{
	s32 nChnId;					///< 通道id
	u32 dwFocusModeMask;		///< 支持的聚焦模式(EIspFocusMode组合值)
	EIspFocusMode eFocusMode;	///< 聚焦模式
	u32 dwFocusMinMask;			///< 支持的最小聚焦距离(EIspFocusMin组合值)
	EIspFocusMin eFocusMin;		///< 最小聚焦距离(设置时可选)
}NETPACKED;

struct TNetIspShutterCfg
{
	s32 nChnId;						///< 通道id
	u32 dwShutterModeMask;			///< 支持的快门模式(EIspShutterMode组合值)
	EIspShutterMode eShutterMode;	///< 快门模式
	u32 dwShutterMinMask;			///< 支持的最小聚焦距离(EIspShutterOpt组合值)
	EIspShutterOpt eShutterMin;		///< 快门下限,自动模式下有效(设置时可选)
	u32 dwShutterLevelMask;			///< 支持的最小聚焦距离(EIspShutterOpt组合值)
	EIspShutterOpt eShutterLevel;	///< 快门等级,手动模式下有效(设置时可选)
}NETPACKED;

struct TNetIspFreqModeCfg
{
	s32 nChnId;					///< 通道id
	u32 dwFreqModeMask;			///< 支持的防闪烁模式(EIspFreqMode组合值)
	EIspFreqMode eFreqMode;		///< 防闪烁模式
}NETPACKED;

struct TNetIspWhiteBalanceCfg
{
	s32 nChnId;									///< 通道id
	u32 dwWhiteBalanceMask;						///< 支持的防闪烁模式(EIspWhiteBalance组合值)
	EIspWhiteBalance eWhiteBalanceMode;			///< 防闪烁模式
	s32 nWBalanceRed;							///< 红增益,手动模式下有效(设置时可选)(取值范围[0,100])
	s32 nWBalanceBlue;							///< 蓝增益,手动模式下有效(设置时可选)(取值范围[0,100])
}NETPACKED;

struct TNetIspFixedtimeDay
{
	EDayType eDayType;							///< 星期几，不支持假日
	BOOL32 bEnable;								///< 是否开启
	s8 szStartTime[NET_MAXLEN_32];				///< 开始时间
	s8 szEndTime[NET_MAXLEN_32];				///< 结束时间
}NETPACKED;

struct TNetIspFixedtimeAllDay
{
	BOOL32 bEnable;							 ///< 是否每天
	s8 szStartTime[NET_MAXLEN_32];           ///< 开始时间
	s8 szEndTime[NET_MAXLEN_32];             ///< 结束时间
}NETPACKED;

struct TNetIspDayNightSwitchFixedtime
{
	TNetIspFixedtimeAllDay	tIspFixedtimeEveryDay;								///< 每天
	s32 nNum;																	///< 数量
	TNetIspFixedtimeDay atIspFixedtimeDay[NET_ALARM_GUARD_TIME_SEG_MAX_NUM];	///< 按周(与每天互斥)
}NETPACKED;

struct TNetIspDayNightSwitchCfg
{
	s32 nChnId;									///< 通道id
	u32 dwDayNightSwitchMask;					///< 支持的日夜切换模式(EIspDayNightSwitch组合值)
	EIspDayNightSwitch eDayNightSwitchMode;		///< 日夜切换模式
	u32 dwTriggerMask;							///< 支持的告警触发状态(EIspDayNightSwitch组合值)
	EIspDayNightSwitch eTrigger;				///< 告警触发状态(告警触发模式下有效,设置时可选)	
	s32 nSensLevel;								///< 日夜转换灵敏度(增益自动模式下有效,设置时可选)
	s32 nSensLevelMin;							///< 日夜转换灵敏度最小值
	s32 nSensLevelMax;							///< 日夜转换灵敏度最大值
	s32 nGainThreshold;							///< 白天到夜晚转换增益阈值(增益自动模式下有效,设置时可选)
	s32 nGainThresholdMin;						///< 白天到夜晚转换增益阈值最小值
	s32 nGainThresholdMax;						///< 白天到夜晚转换增益阈值最大值
	s32 nDelayTime;								///< 转换时间(增益自动模式下有效,设置时可选)
	s32 nDelayTimeMin;							///< 转换时间最小值
	s32 nDelayTimeMax;							///< 转换时间最大值
	s32 nLightSens;								///< 日夜转换光敏灵敏度(光敏自动模式下有效,设置时可选)
	s32 nLightSensMin;							///< 日夜转换光敏灵敏度最小值
	s32 nLightSensMax;							///< 日夜转换光敏灵敏度最大值
	s32 nLightThr;								///< 日夜转换光敏阈值(光敏自动模式下有效,设置时可选)
	s32 nLightThrMin;							///< 日夜转换光敏阈值最小值
	s32 nLightThrMax;							///< 日夜转换光敏阈值最大值
	TNetIspDayNightSwitchFixedtime tFixedtime;	///< 定时参数(定时模式下有效,设置时可选)
}NETPACKED;

struct TNetIspDenoiseCfg
{
	s32 nChnId;									///< 通道id
	u32 dwDenoiseModeMask;						///< 支持的降噪模式(EIspDenoiseMode组合值)
	EIspDenoiseMode eDenoiseMode;				///< 降噪模式
	s32 nLevel;									///< 降噪等级(取值范围[0,100])
}NETPACKED;

struct TNetIspDynamicRegulateCfg
{
	s32 nChnId;									///< 通道id
	u32 dwModeMask;								///< 支持的背光补偿模式(EIspDynamicRegulateMode组合值)
	EIspDynamicRegulateMode eMode;				///< 背光补偿模式
	s32 nBlcLevel;								///< 背光补偿等级(设置时可选)(取值范围[0,100])
	s32 nSlcLevel;								///< 强光抑制等级(设置时可选)(取值范围[0,100])
	s32 nWdrLevel;								///< 宽动态等级(设置时可选)(取值范围[0,100])
	s32 nSmartIrLevel;							///< SmartIr等级(设置时可选)(取值范围[0,100])
	EIspBlcRegionType eBlcRegionType;			///< 手动背光补偿自定义区域类型
	TNetRect tBlcRegion;						///< 自定义区域(设置时可选)
}NETPACKED;

struct TNetIspImageEnhanceCfg
{
	s32 nChnId;									///< 通道id
	u32 dwModeMask;								///< 支持的图像增强模式(EIspImageEnhanceMode组合值)
	EIspImageEnhanceMode eMode;					///< 图像增强模式
	s32 nDigDynRgLevel;							///< 数字宽动态等级(设置时可选)(取值范围[0,100])
	s32 nAdapGammaLevel;						///< 自适应gamma等级(设置时可选)(取值范围[0,100])
	s32 nDefogLevel;							///< 数字去雾等级(设置时可选)(取值范围[0,100])
}NETPACKED;

struct TNetIspStablizerCfg
{
	s32 nChnId;									///< 通道id
	u32 dwModeMask;								///< 支持的防抖模式(EIspBasicMode组合值)
	EIspBasicMode eMode;						///< 防抖模式
	s32 nStablLevel;							///< 防抖等级(设置时可选)(取值范围[0,100])
}NETPACKED;

struct TNetIspCombinHdrCfg
{
	s32 nChnId;									///< 通道id
	u32 dwModeMask;								///< 支持的合成宽动态模式(EIspCombinHdrMode组合值)
	EIspCombinHdrMode eMode;					///< 合成宽动态模式
	s32 nCombinHdrLevel;						///< 等级(取值范围[0,100])
}NETPACKED;

struct TNetIspDoubleLensCfg
{
	s32 nChnId;									///< 通道id
	u32 dwModeMask;								///< 支持的二倍镜模式参数(EIspBasicMode组合值)
	EIspBasicMode eMode;						///< 二倍镜模式参数
}NETPACKED;

struct TNetIspExpAllAutoCfg
{
	s32 nChnId;									///< 通道id
	u32 dwModeMask;								///< 支持的二倍镜模式参数(EIspBasicMode组合值)
	EIspBasicMode eMode;						///< 二倍镜模式参数
}NETPACKED;

struct TNetIspProExposureCfg
{
	s32 nChnId;									///< 通道id
	u32 dwModeMask;								///< 支持的曝光控制模式(EIspBasicMode组合值)
	EIspBasicMode eMode;						///< 曝光控制模式
	s32 nExposureLevel;							///< 曝光目标亮度
	s32 nExposureSpeed;							///< 曝光调整速度
	TNetRect tExposureWindow;					///< 曝光参考区域
}NETPACKED;

struct TNetIspSceneCustomItem
{
	s32 nIndex;									///< 自定义场景索引
	s8 szName[NET_MAXLEN_64 + 1];				///< 自定义场景名称
}NETPACKED;

struct TNetIspSceneCustomList
{
	s32 nMaxNum;																	///< 自定义场景最大数量
	s32 nNum;																		///< 数量
	TNetIspSceneCustomItem atSceneCustomItem[NET_ISP_SCENE_CUSTOMLIST_MAX_NUM];		///< 自定义场景
}NETPACKED;

struct TNetIspSceneDynamicDaynight
{
	BOOL32 bCustom;								///< 是否自定义模式 (自定义模式则自定义场景名称生效，反之操作模式生效)
	s8 szCustomName[NET_MAXLEN_64 + 1];			///< 自定义场景名称
	EIspSceneMode eDynamicType;					///< 操作模式
}NETPACKED;

struct TNetIspSceneDynamicTime
{
	BOOL32 bEnble;										///< 该时间段是否启用	
	s8 szStartTime[NET_MAXLEN_32];						///< 时间段开始时间
	s8 szEndTime[NET_MAXLEN_32];						///< 时间段结束时间
	TNetIspSceneDynamicDaynight tSceneDynamicDaynight;	///< 动态场景模式
}NETPACKED;

struct TNetIspSceneDynamicWeek
{
	EDayType eDayType;															///< 日期类型
	u16 wItemNum;																///< 当天时间段数
	TNetIspSceneDynamicTime atSceneDynamicTime[NET_ISP_SCENE_DAY_DYNAMIC_MAX_NUM]; ///< 时间段信息
}NETPACKED;

struct TNetIspSceneDynamicTimelist
{
	s32 nNum;																///< 数量							
	TNetIspSceneDynamicWeek atSceneDynamicPlan[NET_RECTIMEPLAN_MAX_NUM];	///< 录像时间计划
}NETPACKED;

struct TNetIspSceneModeDynamic
{
	u32 dwDynamicTypeMask;								///< 支持的动态模式类型(EIspSceneDynamicType组合值)
	EIspSceneDynamicType eDynamicType;					///< 动态模式类型
	TNetIspSceneDynamicDaynight tSceneDynamicDayMode;	///< 日场景模式
	TNetIspSceneDynamicDaynight tSceneDynamicNightMode;	///< 夜场景模式
	TNetIspSceneDynamicTimelist tSceneDynamicTimelist;	///< 时间表
}NETPACKED;

struct TNetIspSceneModeCfg
{
	s32 nChnId;										///< 通道id
	u32 dwModeMask;									///< 支持的操作模式(EIspSceneMode组合值)
	EIspSceneMode eMode;							///< 操作模式
	TNetIspSceneCustomList tSceneCustomList;		///< 用户自定义列表
	TNetIspSceneModeDynamic tSceneModeDynamic;		///< 动态模式
}NETPACKED;

struct TNetIspFlipAndPlayBackCfg
{
	s32 nChnId;									///< 通道id
	BOOL32 bSupportFlip;						///< 是否支持翻转模式
	u32 dwFlipModeMask;							///< 支持的翻转模式(EIspFlipMode组合值)
	EIspFlipMode eFlipMode;						///< 翻转模式
	BOOL32 bSupportMirror;						///< 是否支持镜像模式
	u32 dwMirrorModeMask;						///< 支持的镜像模式(EIspMirrorMode组合值)
	EIspMirrorMode eMirrorMode;					///< 翻转模式
	BOOL32 bSupportRotate;						///< 是否支持走廊模式
	u32 dwRotateModeMask;						///< 支持的走廊模式(EIspRotateMode组合值)
	EIspRotateMode eRotateMode;					///< 走廊模式
	BOOL32 bSupportPlayBack;					///< 是否支持本地回显模式
	u32 dwPlayBackModeMask;						///< 支持的本地回显模式(EIspPlayBackMode组合值)
	EIspPlayBackMode ePlayBackMode;				///< 本地回显模式
}NETPACKED;

struct TNetIspInfaredLevel
{
	BOOL32 bLevelMode;							///< 是否红外强度模式，反之远近光强度模式
	s32 nLevel;									///< 红外强度
	s32 nLevelMax;								///< 红外强度最大值
	s32 nLevelMin;								///< 红外强度最小值
	s32 nFar;									///< 远光强度
	s32 nFarMax;								///< 远光强度最大值
	s32 nFarMin;								///< 远光强度最小值
	s32 nNear;									///< 近光强度
	s32 nNearMax;								///< 近光强度最大值
	s32 nNearMin;								///< 近光强度最小值
}NETPACKED;

struct TNetIspInfraredCfg
{
	s32 nChnId;									///< 通道id
	u32 dwModeMask;								///< 支持的红外灯模式(EIspInfaredMode组合值)
	EIspInfaredMode eMode;						///< 红外灯模式
	TNetIspInfaredLevel tIspInfaredLevel;		///< 红外灯强度(红外强度模式、远近光强度模式二选一)
}NETPACKED;

struct TNetIspLaserCfg
{
	s32 nChnId;									///< 通道id
	u32 dwSwitchMask;							///< 支持的开关模式(EIspBasicMode组合值)
	EIspBasicMode eSwitch;						///< 开关
	s32 nIntensity;								///< 激光强度
	s32 nIntensityMin;							///< 激光强度最小值
	s32 nIntensityMax;							///< 激光强度最大值
	u32 dwModeMask;								///< 支持的聚光模式(EIspLaserMode组合值)
	EIspLaserMode eMode;						///< 聚光模式
	BOOL32 bSupCentradMode;						///< 是否支持激光中心度模式
	u32 dwCentradModeMask;						///< 支持的聚光模式(EIspLaserCentradMode组合值)
	EIspLaserCentradMode eCentradMode;			///< 激光中心度模式
	BOOL32 bSupCentradModeSpeed;				///< 是否支持激光中心度调节速度
	s32 nCentradModeSpeedMin;					///< 激光中心度调节速度最小值
	s32 nCentradModeSpeedMax;					///< 激光中心度调节速度最大值
	s32 nCentradModeSpeed;						///< 激光中心度调节速度
}NETPACKED;

struct TNetIspLdcCfg
{
	s32 nChnId;									///< 通道id
	u32 dwSwitchMask;							///< 支持的开关模式(EIspBasicMode组合值)
	EIspBasicMode eSwitch;						///< 开关
	BOOL32 bSupMainParam;						///< 是否支持主矫正参数
	s32 nMainParamMin;							///< 主矫正参数最小值
	s32 nMainParamMax;							///< 主矫正参数最大值
	s32 nMainParam;								///< 主矫正参数
	BOOL32 bSupAssistParam;						///< 是否支持辅矫正参数
	s32 nAssistParamMin;						///< 辅矫正参数最小值
	s32 nAssistParamMax;						///< 辅矫正参数最大值
	s32 nAssistParam;							///< 辅矫正参数
	BOOL32 bSupSharpLevel;						///< 是否支持锐化参数
	s32 nSharpLevelMin;							///< 锐化参数最小值
	s32 nSharpLevelMax;							///< 锐化参数最大值
	s32 nSharpLevelParam;						///< 锐化参数
}NETPACKED;

struct TNetDigitalZoom
{
	u16 wGridId;			///< grid id
	BOOL32 bDigitalZoom;	///< 是否数字放大
	s32 nX;					///< 起始坐标
	s32 nY;					///< 起始坐标
	s32 nWidth;				///< 宽
	s32 nHeight;			///< 高
}NETPACKED;

struct TNetDecMode
{
	u16 wGridId;        ///< 窗格id
	EDecMode eDecMode;  ///< 解码模式
}NETPACKED;

struct TNetVideoStreamInfo
{
	s32 nFrameRate;				///< 帧率
	s32 nBitRate;				///< 码率
	EVideoEncType eEncType;		///< 编码格式
	TNetEncResolution tEncResolution; ///< 分辨率
}NETPACKED;

struct TNetDisModeItem
{
	EDisMode eDisMode;	///< 显示模式
	s32 nBright;		///< 亮度
	s32 nContrast;		///< 对比度
	s32 nSaturation;	///< 饱和度
	s32 nHue;			///< 色度
}NETPACKED;

struct TNetDisModeList
{
	EDisMode eCurDisMode;							///< 当前显示模式
	TNetDisModeItem atDisModeItem[DISMODE_COUNT];	///< 显示模式信息
}NETPACKED;

struct TNetGetRtspRealStreamUrlParam
{
	s32 nChnId;                                    ///< 通道ID
	ERtspTransType eRtspTransType;                 ///< rtsp码流传输类型
	s32 nVideoEncNum;                              ///< 需要申请的视频编码通道数量
	s32 anVideoEncId[NET_NVR_VIDEO_ENC_MAX_NUM];   ///< 视频编码通道ID数组
	s32 nAudioEncNum;                              ///< 需要申请的音频编码通道数量
	s32 anAudioSrcId[NET_NVR_AUDIO_ENC_MAX_NUM];   ///< 音频源通道ID数组
}NETPACKED;

struct TNetRtspRealStreamUrl
{
	s8 szRtspUrl[NET_MAXLEN_256 + 1];        ///< 生成的rtsp url
	s32 nRtspPort;                           ///< rtsp url中的端口
}NETPACKED;

struct TNetGetRtspPlaybackUrlParam
{
	s32 nTaskID;									///< 已创建的放像任务ID
	s32 nChnID;                                  	///< 通道ID
	s8 szStartTime[NET_MAXLEN_32];					///< 开始时间(格式为:2016-03-15T12:48:01.330)
	s8 szEndTime[NET_MAXLEN_32];					///< 结束时间(格式为:2016-03-15T12:48:01.330)
	s8 szResStartTime[NET_MAXLEN_32];				///< 录像资源起始时间(格式为:2016-03-15T12:48:01.330)(此节点是可选项,不填写则表示可以向前seek)
	s8 szResEndTime[NET_MAXLEN_32];					///< 录像资源结束时间(格式为:2016-03-15T12:48:01.330)(此节点是可选项,不填写则表示一直向后播放)
	s32 nVideoEncNum;								///< 需要申请的视频编码通道数量
	s32 anVideoEncId[NET_NVR_VIDEO_ENC_MAX_NUM];	///< 视频编码通道ID数组
	s32 nAudioEncNum;								///< 需要申请的音频编码通道数量
	s32 anAudioSrcId[NET_NVR_AUDIO_ENC_MAX_NUM];	///< 音频源通道ID数组
}NETPACKED;

struct TNetRtspPlaybackUrl
{
	s8 szRtspUrl[NET_MAXLEN_256 + 1];        ///< 生成的rtsp url
	s32 nRtspPort;                           ///< rtsp url中的端口
}NETPACKED;

struct TNetGetDecState
{
	EOutputType eOutputType;	///< 显示类型
	u16 wGridIdStart;			///< 起始格子id
	u16 wGridIdEnd;				///< 结束格子id
}NETPACKED;

struct TNetDecState
{
	EDecState eDecState;	///< 解码状态
}NETPACKED;

struct TNetDecStateList
{
	s32 nNum;										///< 数量
	TNetDecState atDecState[NET_DEC_STATE_MAX_NUM];	///< 解码状态
}NETPACKED;

struct TNetEthMode
{
	ENetMode eNetMode;        ///< 网卡工作模式
	u32 dwNetModeMask;        ///< 网卡工作模式能力，ENetMode组合值
	s32 nDefRouting;          ///< 默认路由 
}NETPACKED;

struct TNetEthCap
{
	BOOL32 bEthReadOnly;                 ///< 网卡配置是否只读
	BOOL32 bDnsReadOnly;                 ///< dns配置是否只读
	BOOL32 bDefRoutingReadOnly;			 ///< 默认路由配置是否只读
}NETPACKED;

struct TNetEthParam
{
	s32 nEthId;                  ///< 网卡ID
	s8 szName[NET_MAXLEN_128];   ///< 网卡名
	BOOL32 bSupRouter;           ///< 是否支持设置为路由
	BOOL32 bDhcp;		         ///< IP获得方式，TRUE-自动获得，FALSE-手动设定
	u32 dwIp;			         ///< IP，网络序
	u32 dwSubnetMask;	         ///< 子网掩码，网络序
	u32 dwDefGateway;	         ///< 默认网关，网络序
	BOOL32 bDnsAuto;	         ///< DNS获得方式，TRUE-自动获得，FALSE-手动设定
	u32 dwFirstDns;		         ///< 首选DNS，网络序
	u32 dwSecondDns;	         ///< 备选DNS，网络序
	u32 dwNetCardSpeedMask;      ///< 网卡速度掩码，ENetCardSpeed组合值
	ENetCardSpeed eNetCardSpeed; ///< 网卡速度
	ENetCardSpeed eActualSpeed;  ///< 网卡速度为自适应(NETCARDSPEED_AUTO)时，实际网卡速度
	ENetLinkSate eNetLinkState;	 ///< 是否连接
	s8 szMac[NET_MAXLEN_32];	 ///< Mac地址
	BOOL32 bDefRouting;			 ///< 默认路由
	TNetEthCap tEthCap;          ///< 网卡能力
}NETPACKED;

struct TNetEthInfo
{
	u8 byEthNum;                                  ///< 网卡数量
	TNetEthParam atEthParam[NET_NVR_ETH_MAX_NUM]; ///< 网卡信息
}NETPACKED;

struct TNetServerPort
{
	u16 wHttpPort;   ///< HTTP端口
	u16 wRtspPort;   ///< RTSP端口
	u16 wServerPort; ///< 服务端口
}NETPACKED;

struct TNetWifiMode
{
	EWifiMode eWifiMode;			///< wifi模式
}NETPACKED;

struct TNetMobileNetworkStatus
{
	ESimStatus eSimStatus;				///< sim卡状态
	s8 szNetOperators[NET_MAXLEN_64];	///< 网络运营商
	ENetType  eModeType;				///< 网络类型
	BOOL32 bConnect;					///< 是否连接
	s32 nSignal;						///< 信号强度
	s8 szIpAddr[NET_MAXLEN_32];			///< ip地址
	s8 szDnsAddr[NET_MAXLEN_32];		///< dns地址
	s8 szIMEI[NET_MAXLEN_64];			///< IMEI号
}NETPACKED;

struct TNetMulticastCfg
{
	s8 szMultiAddr[NET_MAXLEN_32];	///< 多播地址
	u16 wStartPort;					///< 起始端口
	s32 nTtl;						///< ttl值
	s32 nTtlMin;					///< ttl最小值
	s32 nTtlMax;					///< ttl最大值
}NETPACKED;

struct TNetSysBaseParam
{
	s8 szDevName[NET_MAXLEN_64 + 1];        ///< 设备名称
	u16 wDevNo;                             ///< 设备编号
	u16 wDevNoMin;                          ///< 设备编号最小值
	u16 wDevNoMax;                          ///< 设备编号最大值
	u32 dwLangMask;			                ///< gui支持的语言，EGuiLangType组合值
	EGuiLangType eCurLang;	                ///< 当前语言
	u32 dwAutoLogoutTimeMask;			    ///< 支持的自动注销时间列表，EAutoLogoutTime组合值
	EAutoLogoutTime eCurAutoLogoutTime ;	///< 当前自动注销时间
	u8 byMenuTran;                          ///< 菜单透明度
	u8 byMenuTranMin;                       ///< 菜单透明度
	u8 byMenuTranMax;                       ///< 菜单透明度
	EAudioListen eDefAudioListen;			///< 默认音频收听
	BOOL32 bBootGuideEnable;                ///< 是否开启开机向导，TRUE-启用，FALSE-不启用
	BOOL32 bOptPwdEnable;                   ///< 是否启用操作密码，TRUE-启用，FALSE-不启用
}NETPACKED;

struct TNetZeroChnEncParam
{
	BOOL32 bEnable;                                ///< 是否启用零通道编码，TRUE-启用，FALSE-不启用
	u16 wEncResNum;                                ///< 编码分辨率数量
	TNetEncResolution atEncResolution[NET_VIDEOENC_RESOLUTION_MAX_NUM]; ///< 编码分辨率列表
	TNetEncResolution tEncResolution;              ///< 当前编码分辨率
	u32 dwFrameRateMask;                           ///< 编码帧率列表，EFrameRateType组合值
	EFrameRateType eCurFrameRateType;              ///< 当前编码帧率
	u32 dwBitRateMask;                             ///< 编码码率列表，EBitRateType组合值
	EBitRateType eCurBitRateType;                  ///< 当前编码码率
	u32 dwZeroChnVideoSrcMask;                     ///< 零通道编码图像来源列表，EZeroChnVideoSrcType组合值
	EZeroChnVideoSrcType eCurZeroChnVideoSrcType;  ///< 零通道编码当前图像来源
	u32 dwEncTypeMask;							   ///< 编码格式列表，EVideoEncType组合值
	EVideoEncType eCurEncType;					   ///< 当前编码格式
}NETPACKED;

struct TNetUserInfo
{
	s8 szUserName[NET_NVR_USER_NAME_MAX_LEN + 1];	///< 用户名
	BOOL32 bDisable;								///< 用户是否停用，TRUE-停用，FALSE-启用
	u16 wLoginTimes;								///< 登录次数
	EUserLevel eUserLevel;							///< 用户级别
}NETPACKED;

struct TNetUserList
{
	u16 wUserNum;                                   ///< 用户数量
	TNetUserInfo atUserInfo[NET_NVR_USER_MAX_NUM];  ///< 用户信息数组
}NETPACKED;

struct TNetAddUser
{
	s8 szUserName[NET_NVR_USER_NAME_MAX_LEN + 1];	///< 用户名，1~32个字符
	s8 szPwd[NET_NVR_USER_PWD_MAX_LEN + 1];			///< 密码，密码长度8~16
	EUserLevel eUserLevel;							///< 用户级别
}NETPACKED;

struct TNetEditUser
{
	s8 szOldUserName[NET_NVR_USER_NAME_MAX_LEN + 1];	///< 旧用户名
	s8 szNewUserName[NET_NVR_USER_NAME_MAX_LEN + 1];	///< 新用户名，1~32个字符，没有修改用户名该字段清零
	s8 szPwd[NET_NVR_USER_PWD_MAX_LEN + 1];			    ///< 密码，密码长度8~16
	EUserLevel eUserLevel;								///< 用户级别
	BOOL32 bDisable;									///< 用户是否停用，TRUE-停用，FALSE-启用
	BOOL32 bPwdReset;									///< 密码重置
}NETPACKED;

struct TNetModifyUserPwd
{
	s8 szOldPwd[NET_NVR_USER_PWD_MAX_LEN + 1];		///< 旧密码
	s8 szNewPwd[NET_NVR_USER_PWD_MAX_LEN + 1];		///< 新密码，密码长度8~16
}NETPACKED;

struct TNetUserLevelPermInfo 
{
	EUserLevel eUserLevel;                              ///< 用户等级
	u32 dwSysPermMask;                                  ///< 系统权限掩码，ESysPermType组合值
	u32 dwChnPermMask;                                  ///< 通道权限掩码，EChnPermType组合值
}NETPACKED;

struct TNetUserPermCapInfo
{
	s32 nUserMaxNum;	                                               ///< 用户最大数量
	s32 nUserNameMaxLen;                                               ///< 用户名最大长度
	s32 nPasswordMinLen;                                               ///< 用户密码最小长度
	s32 nPasswordMaxLen;                                               ///< 用户密码最大长度
	BOOL32 bDisableUserSup;                                            ///< 是否支持用户停用功能
	u32 dwUserLevelMask;                                               ///< 用户级别掩码，EUserLevel类型组合值
	BOOL32 bPermSup;                                                   ///< 是否支持权限配置
	BOOL32 bSysPermSup;                                                ///< 是否支持系统权限配置
	u32 dwSysPermMask;                                                 ///< 系统权限掩码，ESysPermType组合值
	BOOL32 bChnPermSup;                                                ///< 是否支持通道权限配置
	u32 dwChnPermMask;                                                 ///< 通道权限掩码，EChnPermType组合值
	BOOL32 bRmtCtrlSup;                                                ///< 远程访问控制权限功能是否支持
	s32 nRmtIpStrMaxLen;                                               ///< 远程访问控制字符串最大长度
	u16 wUserLevelNum;                                                 ///< 用户级别数量
	TNetUserLevelPermInfo atUserLevelPermInfo[NET_USERLEVEL_MAX_NUM];  ///< 用户级别权限
}NETPACKED;

struct TNetChnPermInfo
{
	s32 nId;                  ///< 通道Id
	u32 dwChnPermMask;        ///< 通道权限
}NETPACKED;

struct TNetUserPermInfo
{
	s8 szUserName[NET_NVR_USER_NAME_MAX_LEN + 1];	             ///< 用户名
	BOOL32 bSetRemoteInfo;                                       ///< 是否设置远程信息
	BOOL32 bEnRemoteCtrl;                                        ///< 是否开启远程控制，TRUE-开启，FALSE-不启用
	s8 szRemoteIpInfo[NET_MAXLEN_256 + 1];                       ///< 远程访问控制ip列表,启用远程控制后生效，默认空，范围7-255字符;ip地址间以分号分割，支持“*”通配符
	u32 dwSysPermMask;                                           ///< 用户系统权限，ESysPermType组合值
	u16 wChnNums;                                                ///< 通道数量
	TNetChnPermInfo atChnPermInfo[NET_PER_GET_CHNPERM_MAX_NUM];  ///< 通道权限数组
}NETPACKED;

struct TNetRtspAuthInfo
{
	ERtspAuthType eRtspAuthType;             ///< RTSP认证方式
	u32 dwRtspAuthTypeMask;                  ///< RTSP认证方式掩码，ERtspAuthType类型组合值
}NETPACKED;

struct TNetSecurityServiceInfo
{
	BOOL32 bEnableSsh;                          ///< 是否启用SSH连接
	BOOL32 bEnableIllLoginLock;					///< 是否启用非法登录锁定
	s8 szManageMailbox[NET_MAXLEN_64 + 1];      ///< 管理邮箱地址
}NETPACKED;

struct TNetIpAddrFilterCfg
{
	BOOL32 bEnable;                                                 ///< 是否启用Ip地址过滤
	EIpAddrFilterType eCurrIpAddrFilterType;                        ///< 当前的ip地址过滤类型
	u32 dwIpAddrFilterTypeMask;                                     ///< 支持的ip地址过滤类型掩码，EIpAddrFilterType类型组合值
	s32 nBlackIpAddrMaxNum;                                         ///< 黑名单ip地址最大数量
	s32 nBlackIpAddrNum;                                            ///< 黑名单ip地址数量
	s8 aszBlackIpAddr[NET_IPADDRFILTERLIST_MAXNUM][NET_MAXLEN_16];  ///< 黑名单ip地址数组
	s32 nWhiteIpAddrMaxNum;                                         ///< 白名单ip地址最大数量
	s32 nWhiteIpAddrNum;                                            ///< 白名单ip地址数量
	s8 aszWhiteIpAddr[NET_IPADDRFILTERLIST_MAXNUM][NET_MAXLEN_16];  ///< 白名单ip地址数组
}NETPACKED;

struct  TNetDisplayPortFreshRateCap
{
	EDisplayResolution eDisplayResolution;       ///< 显示分辨率
	u32 dwDisplayFreshRateMask;                  ///< 显示刷新率掩码，EDisplayFreshRate组合值
}NETPACKED;

struct TNetDisplayPort
{
	u16 wPortId;                                                                                  ///< 端口Id
	BOOL32 bEnable;                                                                               ///< 是否可用
	EDisplayResolution eDisplayResolution;                                                        ///< 显示分辨率
	u32 dwDisplayResolutionMask;                                                                  ///< 显示分辨率掩码，EDisplayResolution组合值
	EDisplayFreshRate eDisplayFreshRate;                                                          ///< 显示刷新率
	u16 wFreshRateCapNum;                                                                         ///< 显示刷新率能力数量
	TNetDisplayPortFreshRateCap tDisplayPortFreshRateCap[NET_DISPLAY_PORT_RESOLUTION_MAX_NUM];    ///< 显示刷新率能力
}NETPACKED;

struct TNetDisplayPortVgaList 
{
	BOOL32 bValid;                                              ///< 是否生效
	u16 wPortNum;                                               ///< 显示接口数量
	TNetDisplayPort atDisplayPort[NET_DISPLAY_PORT_MAX_NUM];    ///< 显示接口信息
}NETPACKED;

struct TNetDisplayPortHdmiList 
{
	BOOL32 bValid;                                              ///< 是否生效
	u16 wPortNum;                                               ///< 显示接口数量
	TNetDisplayPort atDisplayPort[NET_DISPLAY_PORT_MAX_NUM];    ///< 显示接口信息
}NETPACKED;

struct TNetDisplayPortCvbsList 
{
	BOOL32 bValid;                                              ///< 是否生效
	u16 wPortNum;                                               ///< 显示接口数量
	TNetDisplayPort atDisplayPort[NET_DISPLAY_PORT_MAX_NUM];    ///< 显示接口信息
}NETPACKED;

struct TNetSerialPortInfo 
{
	ESerialType eSerialType;                 ///< 串口类型
	u32 dwSerialWorkTypeMask;                ///< 串口功能掩码, ESerialWorkType类型组合值
	ESerialWorkType eSerialWorkType;         ///< 串口功能
	u32 dwSerialBaudRateMask;                ///< 波特率掩码, ESerialBaudRate类型组合值
	ESerialBaudRate eSerialBaudRate;         ///< 波特率
	u32 dwSerialDataBitMask;                 ///< 数据位掩码, ESerialDataBit类型组合值
	ESerialDataBit eSerialDataBit;           ///< 数据位
	u32 dwSerialStopBitMask;                 ///< 停止位掩码, ESerialStopBit类型组合值
	ESerialStopBit eSerialStopBit;           ///< 停止位
	u32 dwSerialParityBitMask;               ///< 校验位掩码, ESerialParityBit类型组合值
	ESerialParityBit eSerialParityBit;       ///< 校验位
	u32 dwSerialFlowCtrlMask;                ///< 流控掩码, ESerialFlowCtrl类型组合值
	ESerialFlowCtrl eSerialFlowCtrl;         ///< 流控
	s8 szSerialPortName[NET_MAXLEN_64 + 1];  ///< 串口名称
}NETPACKED;

struct TNetKtsmActiveStatus
{
	s8 szPinCode[NET_MAXLEN_64 + 1];		///< PIN码
	BOOL32 bActive;                         ///< 是否激活
}NETPACKED;

struct TNetActiveKtsmModule
{
	BOOL32 bError;                         ///< 是否出错
	s8 szErrCode[NET_MAXLEN_64 + 1];	   ///< 错误码
}NETPACKED;

struct TNetKtsmStatus
{
	s32 nStatus;						   ///< 自检状态,1为正常可工作
	s32 nSigSpeed;						   ///< 签名速度,单位:次/秒
	s32 nVerifySpeed;					   ///< 验签速度,单位:次/秒
	s32 nHashSpeed;					       ///< hash速度,单位:kb/s
	s32 nEncSpeed;					       ///< 加密速度,单位:kb/s
	s32 nDecSpeed;					       ///< 解密速度,单位:kb/s
	s8 szVer[NET_MAXLEN_64];			   ///< 模块版本号
	s8 szModInfo[NET_MAXLEN_128];		   ///< 模块信息字符串
}NETPACKED;

struct TNetKtsmMgrSrvInfo
{
	s8 szIpAddr[NET_MAXLEN_32];				///< 密管服务ip
	s32 nPort;								///< 密管服务端口
}NETPACKED;

struct TNetKtsmUserInfo
{
	s8  szUserName[NET_MAXLEN_64];	                ///< 用户名
	s8  szPwd[NET_MAXLEN_64 + 1];		            ///< 密码
}NETPACKED;

struct TNetNvrDevInfo
{
	s8 szDevType[NET_MAXLEN_64 + 1];                            ///< 设备类型
	s8 szDevSerialNum[NET_MAXLEN_64 + 1];                       ///< 设备序列号
	s8 szDevHwVer[NET_MAXLEN_64 + 1];                           ///< 设备硬件版本号
	s8 szDevProductionDate[NET_MAXLEN_64 + 1];                  ///< 设备生产日期
	s8 szDevSoftVer[NET_MAXLEN_64 + 1];                         ///< 设备软件版本号
	s32 nCpuUsed;                                               ///< cpu占用率
	s32 nMemUsed;                                               ///< 内存占用率
	BOOL32 bSupDevRomVer;										///< 是否支持设备ROM版本号
	s8 szDevRomVer[NET_MAXLEN_64 + 1];                          ///< 设备ROM版本号
	s8 szDevImei[NET_MAXLEN_32 + 1];							///< 设备编号(IMEI号)
}NETPACKED;

struct TNetChnRecState
{
	s32 nChnId;                              ///< 通道ID
	EChnRecState eChnRecState;               ///< 录像状态
	ERecStreamType eRecStreamType;           ///< 码流类型
	s32 nFrameRate;                          ///< 视频帧率
	s32 nResolutionW;                        ///< 分辨率宽
	s32 nResolutionH;                        ///< 分辨率高
	s32 nBitRate;                            ///< 码率
	BOOL32 bAudioRec;                        ///< 是否录制音频
}NETPACKED;

struct TNetChnRecStateList
{
	s32 nChnNum;                                                    ///< 通道数
	TNetChnRecState atChnRecState[NET_PER_GET_CHNRECSTATE_MAX_NUM]; ///< 通道录像状态信息
}NETPACKED;

struct TNetLogSubTypeItem
{
	ELogSubType eLogSubType;                                          ///< 日志类型
	u32 dwLogSrcTypeMask;                                             ///< 日志来源类型掩码， ELogSrcType组合值
}NETPACKED;

struct TNetLogMainTypeItem
{
	ELogMainType eLogMainType;                                        ///< 日志类别
	u16 wSubTypeNums;                                                 ///< 日志类型数量
	TNetLogSubTypeItem atLogSubTypeItem[NET_LOG_SUBTYPR_MAX_NUM];     ///< 日志类型数组
}NETPACKED;

struct TNetCreateSearchLogTaskParam 
{
	s8 szStartTime[NET_MAXLEN_32];                                    ///< 开始时间
	s8 szEndTime[NET_MAXLEN_32];                                      ///< 结束时间
	ELogMainType eLogMainType;                                        ///< 日志类别
	ELogSubType eLogSubType;                                          ///< 日志类型
	ELogSrcType eLogSrcType;                                          ///< 日志来源
	s32 nChnId;                                                       ///< 日志来源-通道ID
	s8 szUserName[NET_NVR_USER_NAME_MAX_LEN + 1];                     ///< 日志来源-用户名
	EGuiLangType eGuiLangType;                                        ///< 搜索的语言
}NETPACKED;

struct TNetSearchLogTaskInfo
{
	u32 dwTaskId;                                                     ///< 任务id
	u32 dwLogTotalNums;                                               ///< 日志总条数
}NETPACKED;

struct TNetSearchLogItem 
{
	u32 dwTimeStamp;                                                  ///< 时间戳
	s8 szCreateTime[NET_MAXLEN_32];                                   ///< 创建时间
	s8 szLogSrc[NET_MAXLEN_128];                                      ///< 日志来源
	s8 szLogType[NET_MAXLEN_64];                                      ///< 日志类型
	s8 szDetail[NET_MAXLEN_512 + 1];                                  ///< 日志详情
}NETPACKED;

struct TNetHealthLogItem 
{
	EHealthInfoType eHealthInfoType;							///< 记录类型
	s32 nRecordNum;												///< 记录次数
}NETPACKED;

struct TNetHealthLogList 
{
	s32 nNum;														///< 数量
	TNetHealthLogItem atHealthLogItem[NET_HEALTHLOG_TYPE_MAX_NUM];	///< 健康状态信息列表
}NETPACKED;

struct TNetHealthDetailParam
{
	EHealthLogTimeType eHealthLogTimeType;							///< 时间段类型
	EHealthInfoType eHealthInfoType;								///< 记录类型
	s32 nStartIndex;												///< 起始索引
	s32 nNeedNum;													///< 需要数量
}NETPACKED;

struct TNetHealthDetail
{
	s8 szSource[NET_MAXLEN_128];									///< 来源
	EHealthInfoType eHealthInfoType;								///< 记录类型
	s8 szDetail[NET_MAXLEN_512 + 1];								///< 详情描述
	s8 szTime[NET_MAXLEN_32];										///< 时间
}NETPACKED;

struct TNetHealthDetailList 
{
	s32 nNum;															///< 数量
	TNetHealthDetail atHealthDetail[NET_GET_HEALTHLOG_DETAIL_MAX_NUM];	///< 健康状态详情列表
}NETPACKED;

struct TNetGetRealLog
{
	u32 dwStartTimeStamp;	///< 起始时间戳
	u32 dwEndTimeStamp;		///< 终止时间戳
}NETPACKED;

struct TNetRealLogItem
{
	u32 dwTimeStamp;                ///< 实时日志时间戳
	s8 szLogTime[NET_MAXLEN_32];    ///< 实时日志时间
	ERealLogMainType eMainType;     ///< 实时日志类别
	s8 szType[NET_MAXLEN_64];       ///< 实时日志类型
}NETPACKED;

struct TNetRealLogList
{
	u16 wLogNum;                                          ///< 日志条数
	TNetRealLogItem atRealLogItem[NET_REAL_LOG_MAX_NUM];  ///< 日志信息
}NETPACKED;

struct TNetGetRealLogDetail
{
	u32 dwTimeStamp;	         ///< 实时日志时间戳
	s8 szType[NET_MAXLEN_64];    ///< 实时日志类型
	EGuiLangType eLang;	         ///< 实时日志语言
}NETPACKED;

struct TNetRealLogDetail
{
	s8 szTime[NET_MAXLEN_32];			///< 实时日志时间
	s8 szSource[NET_MAXLEN_128];        ///< 实时日志来源
	s8 szType[NET_MAXLEN_64];           ///< 实时日志类型
	s8 szDetail[NET_MAXLEN_512 + 1];    ///< 实时日志详情
}NETPACKED;

struct TNetRealStateItem
{
	s8 szType[NET_MAXLEN_32];    ///< 实时状态类型
}NETPACKED;

struct TNetRealStateList
{
	u16 wRealStateNum;                                         ///< 实时状态信息数量
	TNetRealStateItem atRealStateItem[NET_REAL_STATE_MAX_NUM]; ///< 实时状态信息
}NETPACKED;

struct TNetSystemTimeInfo
{
	BOOL32 bTimeZone;	///< 是否获取时区
	BOOL32 bManualSync; ///< 是否获取手动设置参数
	BOOL32 bSummer;		///< 是否获取夏令时配置
	BOOL32 bAutoSync;	///< 是否获取时间自动同步参数
}NETPACKED;

struct TNetSystemTimeParam
{
	ESummerTimeType eSummerTimeZone; ///< 时区
	BOOL32 bSyncEnable;			     ///< 本次设置是否有效
	s8 szTime[NET_MAXLEN_32];	     ///< 设置的时间字符串
	BOOL32 bSummerEnable;		     ///< 是否启用夏令时
	u32	dwSummerOffset;			     ///< 夏令时偏移时间列表 ESummerTimeOffset
	ESummerTimeOffset eSummerOffset; ///< 当前夏令时时间
	s32 nBeginMonth;			     ///< 开始时间月
	s32 nBeginWeek;				     ///< 开始时间周
	s32 nBeginDay;				     ///< 开始时间日
	s32 nBeginHour;				     ///< 开始时间小时
	s32 nEndMonth;				     ///< 结束时间月
	s32 nEndWeek;				     ///< 结束时间周
	s32 nEndDay;				     ///< 结束时间日
	s32 nEndHour;				     ///< 结束时间小时
	BOOL32 bAutoSyncEnable;		     ///< 是否启用自动校时
	s8 szType[NET_MAXLEN_16];	     ///< 自动校时类型 ntp 或者proto
	s8 szServerIP[NET_MAXLEN_32 + 1];    ///< IP或者域名
	s32 nServerPort;			     ///< 端口
	s32 nInterval;				     ///< 内部时间间隔
}NETPACKED;

struct TNetSysTimePriority
{
	ESysTimeSyncType eAutoSyncType;		///< 自适应类型
	BOOL32 bEnable;						///< 是否启用
}NETPACKED;

struct TNetSysTimeAutoSyncParam
{
	BOOL32 bEnable;									///< 是否启用自动校时
	ESysTimeSyncType eAutoSyncType;					///< 自动校时类型
	u32 dwAutoSyncTypeMask;							///< 支持自动校时类型,EAutoSyncType组合值
	ESysTimeSyncType eSyncCurIndex;					///< 当前生效类型
	s32 nAdaptLockTime;								///< 校时保护时间
	s32 nAdaptLockTimeMin;							///< 校时保护时间最小值
	s32 nAdaptLockTimeMax;							///< 校时保护时间最大值
	s32 nAutoAdaptNum;								///< 自适应同步源个数
	TNetSysTimePriority atNetSysTimePriority[NET_SYSTIME_SYNCTYPE_MAX_NUM]; ///< 优先级列表	
}NETPACKED;

struct TNetSystemTimeParamEx
{
	ESummerTimeType eSummerTimeZone;				///< 时区
	s8 szTime[NET_MAXLEN_32];						///< 时间字符串(格式为:2016-03-15T12:48:01.330)
	TNetSysTimeAutoSyncParam tNetAutoSyncParam;		///< 自动校时参数
	TNetSysTimeAutoSyncParam tNetDefSyncParam;		///< 默认自动校时参数
	BOOL32 bNtpEnable;								///< 是否启用NTP协议校时
	s8 szNtpIP[NET_MAXLEN_32 + 1];					///< IP或者域名
	s32 nNtpPort;									///< 端口
	s32 nNtpInterval;								///< 内部时间间隔
	BOOL32 bSummerEnable;							///< 是否启用夏令时
	u32	dwSummerOffset;								///< 夏令时偏移时间列表 ESummerTimeOffset
	ESummerTimeOffset eSummerOffset;				///< 当前夏令时时间
	s32 nBeginMonth;								///< 开始时间月
	s32 nBeginWeek;									///< 开始时间周
	s32 nBeginDay;									///< 开始时间日
	s32 nBeginHour;									///< 开始时间小时
	s32 nEndMonth;									///< 结束时间月
	s32 nEndWeek;									///< 结束时间周
	s32 nEndDay;									///< 结束时间日
	s32 nEndHour;									///< 结束时间小时
}NETPACKED;

struct TNetDiskNumInfo
{
	s32 nGroupID;							///< GroupID值
	s32 nDiskIDNum;							///< 一个GROUP下ID
	s32 nDiskID[NET_GROUP_DISKS_MAX_NUM];	///< 同一个盘组设置硬盘ID
}NETPACKED;

struct TNetGroupDiskList
{
	s32 nGroupNum;		                                ///< 盘组数目
	TNetDiskNumInfo tNetDiskNumInfo[NET_GROUP_MAX_NUM]; ///< 盘组信息数组
}NETPACKED;

struct TNetSetGroupDiskErrDisk
{
	s32 nDiskId;         ///< 出错磁盘ID
	s32 nGroupId;        ///< 出错磁盘加入的盘组ID
	s32 nErrCode;        ///< 业务错误码
}NETPACKED;

struct TNetSetGroupDiskErrInfo
{
	BOOL32 bSuccess;                                                        ///< 操作是否成功
	s32 nErrNum;                                                            ///< 出错磁盘数量
	TNetSetGroupDiskErrDisk atSetGroupDiskErrDisk[NET_GROUP_DISKS_MAX_NUM]; ///< 出错磁盘信息
}NETPACKED;

struct TNetUpgradeExDisk
{
	s32 nCapacity;					///< 硬盘容量 MB
	EDiskType eDiskType;			///< 硬盘类型
	s8  szPartition[NET_MAXLEN_32]; ///< 分区名字
	s8	szMount[NET_MAXLEN_64];		///< 挂在路径
	s32 nDiskFree;					///< 硬盘剩余空间 MB
	s8	szFSType[NET_MAXLEN_16];	///< 文件系统类型
}NETPACKED;

struct TNetExDiskList
{
	s32 nExDiskNum;                                          ///< 扩展硬盘数量
	TNetUpgradeExDisk tNetUpgradeExDisk[NET_EXDISK_MAX_NUM]; ///< 扩展硬盘信息
}NETPACKED;

struct TNetUpgradePrepareParam
{
	s8  szSecurityInfo[NET_MAXLEN_256];	///< 鉴权信息
	s8	szPartition[NET_MAXLEN_32];		///< 分区名字
	s8	szMount[NET_MAXLEN_64];			///< 外置硬盘挂载点
	s8  szFilePath[NET_MAXLEN_512];		///< 文件相对路径
	s8  szFSType[NET_MAXLEN_16];		///< 文件系统类型
}NETPACKED;

struct TNetSystemFactoryDef
{
	s8 szType[NET_MAXLEN_16];			///< Part部分恢复 All完全恢复
	BOOL32 bChnCfgEnable;				///< 通道
	BOOL32 bRecCfgEnable;				///< 录像
	BOOL32 bNetCfgEnable;				///< 网络
	BOOL32 bBaseCfgEnable;				///< 基本配置
	BOOL32 bEventCfgEnable;				///< 事件
	BOOL32 bIntellCfgEnable;			///< 智能
}NETPACKED;

struct TNetRecoverFactoryInfo
{
	ERecoverFactoryType eRecoverFactoryType;	///< 恢复出厂类型
}NETPACKED;

struct TNetGetChnCap
{
	s8 szChnId[NET_MAXLEN_512]; ///< 通道id，多个通道使用逗号分割，例如：1,2,3,4
	BOOL32 bGetHardwareCap;		///< 是否获取硬件能力
	BOOL32 bGetMoveDetectCap;	///< 是否获取移动侦测能力
	BOOL32 bGetVideoLostCap;	///< 是否获取视频丢失能力
	BOOL32 bGetOsdCap;          ///< 是否获取OSD能力
	BOOL32 bGetDevNameCap;      ///< 是否获取设备名称能力
	BOOL32 bGetUpgradeCap;      ///< 是否获取设备升级能力
	BOOL32 bGetOverlayAreaCap;  ///< 是否需要遮挡告警能力
}NETPACKED;

struct TNetChnCap
{
	s32 nChnId;						///< 通道id
	s32 nAlarmInNum;				///< 告警输入数量
	s32 nAlarmOutNum;				///< 告警输出数量
	BOOL32 bSupMoveDetectCfg;		///< 是否支持移动侦测配置
	BOOL32 bSupOsdCfg;              ///< 是否支持OSD配置
	BOOL32 bSupDevNameCfg;          ///< 是否支持设备名称配置
	BOOL32 bSupChangeDevNameCfg;	///< 是否可更改设备名称配置
	BOOL32 bSupUpgrade;	            ///< 是否支持升级
	BOOL32 bSupOverlayArea;	        ///< 是支持否遮挡告警能力
}NETPACKED;

struct TNetChnCapList
{
	s32 nNum;											///< 数量
	TNetChnCap atChnCap[NET_PER_GET_CHN_CAP_MAX_NUM];	///< 通道能力
}NETPACKED;

struct TNetChnInteDetectCap
{
	s32 nErrNo;					///< 错误号 NET_OK：获取成功
	BOOL32 bSupCordon;			///< 是否支持警戒线配置
	BOOL32 bSupAreaInvasion;	///< 是否支持区域入侵配置
	BOOL32 bSupAreaEnter;		///< 是否支持区域进入配置
	BOOL32 bSupAreaLeave;		///< 是否支持区域离开配置
	BOOL32 bSupObjectPick;		///< 是否支持物品拿取配置
	BOOL32 bSupObjectLeft;		///< 是否支持物品遗留配置
	BOOL32 bSupVirtualFocus;	///< 是否支持虚焦检测配置
	BOOL32 bSupPeopleGather;	///< 是否支持人员聚集配置
	BOOL32 bSupSceneChange;		///< 是否支持场景变更配置
	BOOL32 bSupAudAbnormal;		///< 是否支持声音异常配置
	BOOL32 bSupFaceDetect;		///< 是否支持人脸检测配置
	BOOL32 bSupPlateDetect;		///< 是否支持车牌抓拍配置
	BOOL32 bSupAccessProtect;	///< 是否支持防拆报警配置
}NETPACKED;

struct TNetAlarmInCfg
{
	s32 nAlarmInId;						///< 告警输入号
	EAlarmInType eAlarmInType;			///< 告警输入类型
	s8 szAlarmName[NET_MAXLEN_64 + 1];	///< 告警名称
	BOOL32 bEnable;						///< 是否启用，设置时无效
}NETPACKED;

struct TNetAlarmInCfgList
{
	s32 nNum;												///< 数量
	TNetAlarmInCfg atAlarmInCfg[NET_ALARM_IN_ID_MAX_NUM];	///< 告警输入配置
}NETPACKED;

struct TNetAlarmSource
{
	s8 szAlarmSource[NET_MAXLEN_16];	///< 告警来源(可取值：nvr：表示NVR 数字：表示某个通道)
	EAlarmType eAlarmType;				///< 告警类型
	s32 nAlarmInId;						///< 告警输入号，eAlarmType为ALARMTYPE_ALARMIN时有效
	ESmartAlarmType eSmartAlarmType;	///< 智能告警类型，eAlarmType为ALARMTYPE_SMARTDETECT时有效
}NETPACKED;

struct TNetTimeSeg
{
	s32 nStartTime;	///< 开始时间
	s32 nEndTime;	///< 结束时间
}NETPACKED;

struct TNetAlarmGuardTimeItem
{
	EDayType eDayType;											///< 某天的布防时间
	s32 nNum;													///< 数量
	TNetTimeSeg atTimeSeg[NET_ALARM_GUARD_TIME_SEG_MAX_NUM];	///< 布防时间段
}NETPACKED;

struct TNetAlarmGuardTime
{
	TNetAlarmSource tAlarmSource;												///< 告警源
	s32 nNum;																	///< 数量
	TNetAlarmGuardTimeItem atAlarmGuardTimeItem[NET_ALARM_GUARD_TIME_MAX_NUM];	///< 布防时间
}NETPACKED;

struct TNetSysAlarmCfgItem
{
	ESysAlarmType eSysAlarmType;	///< 系统告警类型
	BOOL32 bAudioAlarm;				///< 是否声音告警
	BOOL32 bSendMail;				///< 是否发送邮件
	BOOL32 bLightAlm;				///< 是否告警灯闪烁
}NETPACKED;

struct TNetSysAlarmCfg
{
	s32 nNum;															///< 数量
	TNetSysAlarmCfgItem atSysAlarmCfgItem[NET_SYS_ALARM_CFG_MAX_NUM];	///< 系统告警配置项
}NETPACKED;

struct TNetAlarmInCopy
{
	s8 szAlarmSource[NET_MAXLEN_16];	///< 告警来源(可取值：nvr：表示NVR 数字：表示某个通道)
	s32 nAlarmInId;						///< 告警输入号
}NETPACKED;

struct TNetAlarmInCopyParam
{
	TNetAlarmInCopy tCopySrc;								///< 告警输入复制源
	s32 nNum;												///< 复制目的数量
	TNetAlarmInCopy atCopyDst[NET_ALARM_IN_COPY_MAX_NUM];	///< 告警输入复制目的
}NETPACKED;

struct TNetAlarmLinkCap
{
	BOOL32 bSupLinkNvrAudioAlarm;			///< 是否支持联动到nvr声音报警
	BOOL32 bSupLinkNvrSendMail;				///< 是否支持联动到nvr发送邮件
	BOOL32 bSupLinkNvrUpCenter;				///< 是否支持联动到nvr上报中心
	s8 szLinkNvrAlarmOutId[NET_MAXLEN_64];	///< 联动到nvr的告警输出号，多项使用逗号分隔
	s8 szLinkHdmi[NET_MAXLEN_32];			///< 联动显示到HDMI接口号，多项使用逗号分隔
	s8 szLinkVga[NET_MAXLEN_32];			///< 联动显示到VGA接口号，多项使用逗号分隔
	BOOL32 bSupLinkChnRec;					///< 是否支持联动到通道录象
	BOOL32 bSupLinkChnSnap;					///< 是否支持联动到通道抓拍
	BOOL32 bSupLinkChnPreset;				///< 是否支持联动到通道预置位
	s32 nLinkChnPresetMin;					///< 预置位编号最小值
	s32 nLinkChnPresetMax;					///< 预置位编号最大值
	BOOL32 bSupLinkChnCruise;				///< 是否支持联动到通道巡航
	s32 nLinkChnCruiseMin;					///< 巡航编号最小值
	s32 nLinkChnCruiseMax;					///< 巡航编号最大值
	BOOL32 bSysAudAlm;						///< 是否支持声音报警
	BOOL32 bSysMail;						///< 是否支持发送邮件
	BOOL32 bSysLight;						///< 是否支持告警灯闪烁
	BOOL32 bSupLinkNvrCloudSrv;				///< 是否支持联动云服务
	u32 dwSupUploadModeMask;				///< 支持联动上传方式EAiuUploadType组合值
}NETPACKED;

struct TNetAlarmLinkOutItem
{
	s8 szLinkOutObj[NET_MAXLEN_8];	///< 联动输出对象(可取值：nvr：表示NVR 数字：表示某个通道)
	s8 szAlarmOutId[NET_MAXLEN_64];	///< 告警输出号，多项使用逗号分隔
}NETPACKED;

struct TNetAlarmLinkPtzItem
{
	s32 nChnId;								///< 通道id
	EAlarmLinkPtzType eAlarmLinkPtzType;	///< 联动PTZ类型
	s32 nNumber;							///< 预置位、巡航编号
}NETPACKED;

struct TNetAlarmLinkCfg
{
	TNetAlarmSource tAlarmSource;											///< 告警源
	u32 dwAlarmBasicLinkMask;												///< 常规联动，EAlarmBasicLink组合值
	s32 nAlarmLinkOutNum;													///< 联动输出数量
	TNetAlarmLinkOutItem atAlarmLinkOutItem[NET_ALARM_LINK_OUT_MAX_NUM];	///< 联动输出
	s8 szLinkChnRec[NET_MAXLEN_512];										///< 联动通道录像，通道id，多项使用逗号分隔
	s8 szLinkChnSnap[NET_MAXLEN_512];										///< 联动通道抓拍，通道id，多项使用逗号分隔
	s32 nAlarmLinkPtzNum;													///< 联动PTZ数量
	TNetAlarmLinkPtzItem atAlarmLinkPtzItem[NET_ALARM_LINK_PTZ_MAX_NUM];	///< 联动PTZ
}NETPACKED;

struct TNetSmtpLimitLen
{
	s32 nServerMin;						 ///< 服务器最小长度
	s32 nServerMax;						 ///< 服务器最大长度
	u16 nPortMin;						 ///< 端口最小值
	u16 nPortMax;						 ///< 端口最大值
	s32 nUserNameMin;					 ///< 用户名最小长度
	s32 nUserNameMax;					 ///< 用户名最大长度
	s32 nPassWordMin;					 ///< 密码最小长度
	s32 nPassWordMax;					 ///< 密码最大长度
	s32 nSenderNameMin;					 ///< 发送人名称最小长度
	s32 nSenderNameMax;					 ///< 发送人名称最大长度
	s32 nSenderEmailMin;				 ///< 发送人地址最小长度
	s32 nSenderEmailMax;				 ///< 发送人地址最大长度
	s32 nRecverNameMin;					 ///< 收件人名字最小长度
	s32 nRecverNameMax;					 ///< 收件人名字最大长度
	s32 nRecverEmailMin;				 ///< 收件人地址最小长度
	s32 nRecverEmailMax;				 ///< 收件人地址最大长度
	s32 nRecverMaxNum;					 ///< 收件人数目最大长度
}NETPACKED;

struct TNetRecver
{
	s8 szRecverName[NET_MAXLEN_64 + 1];		 ///< 收件人名称
	s8 szRecverEmail[NET_MAXLEN_64 + 1];	 ///< 收件人地址
}NETPACKED;

struct TNetSmtpCfg
{
	s8 szServer[NET_MAXLEN_64 + 1];			///< 服务器
	u16 nPort;								///< 端口
	s8 szUserName[NET_MAXLEN_64 + 1];		///< 用户名
	s8 szPassWord[NET_MAXLEN_64];			///< 密码
	s8 szSenderName[NET_MAXLEN_64 + 1];		///< 发送人名字
	s8 szSenderEmail[NET_MAXLEN_64 + 1];	///< 发送人地址
	BOOL32 bSsl;							///< 是否启用SSL
	BOOL32 bAttachPic;						///< 是否增加附件
	s32 nRecverNum;							///< 收人人个数
	TNetRecver tRecver[NET_MAXLEN_8];
}NETPACKED;

struct TNetSmtpCfgAll
{
	TNetSmtpLimitLen tSmtpLimitLen;
	TNetSmtpCfg	tSmtpCfg;
}NETPACKED;

struct TNetPortMap
{
	EPORTTYPE ePortType;	///< 端口类型
	u16 wPort;				///< 端口
	u16 wMapPort;			///< 映射端口
	BOOL32 bUpnpState;		///< UPnP状态 TRUE：已生效 FALSE：无效
}NETPACKED;

struct TNetPppoeCfg
{
	BOOL32 bEnable;					///< 是否启用PPPoE
	s32 nNetCardId;					///< 网卡id，在哪张网卡上生效
	s8 szUserName[NET_MAXLEN_128];	///< PPPoE服务用户名
	s8 szPwd[NET_MAXLEN_64];		///< PPPoE服务密码
}NETPACKED;

struct TNetPppoeStatus
{
	EPppoeStatus ePppoeStatus;	///< PPPoE状态
	s8 szIp[NET_MAXLEN_32];		///< 拨号成功后的IP
}NETPACKED;

struct TNetUpnpCfg
{
	BOOL32 bEnable;										///< 是否启用UPnP
	s8 szAlias[NET_MAXLEN_64 + 1];						///< 别名
	EUPNPMODE eUpnpMode;								///< UPnP模式
	s8 szMapIP[NET_MAXLEN_32];							///< 映射IP
	s32 nPortMapNum;									///< 端口映射数量
	TNetPortMap atPortMap[NET_UPNP_PORT_MAP_MAX_NUM];	///< 端口映射信息
}NETPACKED;

struct TNetDDNSCfg
{
	BOOL32 bEnable;										///< 是否启用
	s8 szUsername[NET_MAXLEN_64 + 1];					///< DDNS服务用户名
	s8 szPassword[NET_MAXLEN_64];						///< DDNS服务密码(获取密码时,此节点内容为空)
	u32 dwDDNSModeSupMask;								///< 支持的DDNS服务提供商，EDDNSType组合值
	EDDNSType eDDNSMode;								///< DDNS服务提供商
	s8 szDomainName[NET_MAXLEN_256 + 1];				///< DDNS域名,ip或域名
	s32 nPort;											///< DDNS端口
	s8 szSrvAddr[NET_MAXLEN_256 + 1];					///< 服务器地址,ip或域名
	s8 szDevMac[NET_MAXLEN_32 + 1];						///< 设备Mac(设置时不带此节点)
}NETPACKED;

struct TNetDDNSState
{
	s8 szStatusMsg[NET_MAXLEN_512];						///< 状态消息
}NETPACKED;

struct TNetSnmp3ReadParam
{
	s8 szUserName[NET_MAXLEN_64 + 1];						///< 用户名称
	s32 nUserNameMin;										///< 用户名称长度最小值
	s32 nUserNameMax;										///< 用户名称长度最大值
	EAuthenType eAuthenType;								///< 鉴权方式
	u32 dwAuthenTypeOpt;									///< 支持鉴权方式种类,EAuthenType组合值
	s8 szAuthentPass[NET_MAXLEN_64 + 1];					///< 鉴权密码
	s32 nAuthentPassMin;									///< 鉴权密码长度最小值
	s32 nAuthentPassMax;									///< 鉴权密码长度最大值
	EKeyAlgType eKeyAlgType;								///< 加密方式
	u32 dwKeyAlgTypeOpt;									///< 支持加密方式种类,EKeyAlgType组合值
	s8 szPrivateKeyPass[NET_MAXLEN_64 + 1];					///< 私钥密码
	s32 nPrivateKeyPassMin;									///< 私钥密码长度最小值
	s32 nPrivateKeyPassMax;									///< 私钥密码长度最大值
};

struct TNetSnmp3WriteParam
{
	s8 szUserName[NET_MAXLEN_64 + 1];						///< 用户名称
	s32 nUserNameMin;										///< 用户名称长度最小值
	s32 nUserNameMax;										///< 用户名称长度最大值
	EAuthenType eAuthenType;								///< 鉴权方式
	u32 dwAuthenTypeOpt;									///< 支持鉴权方式种类,EAuthenType组合值
	s8 szAuthentPass[NET_MAXLEN_64 + 1];					///< 鉴权密码
	s32 nAuthentPassMin;									///< 鉴权密码长度最小值
	s32 nAuthentPassMax;									///< 鉴权密码长度最大值
	EKeyAlgType eKeyAlgType;								///< 加密方式
	u32 dwKeyAlgTypeOpt;									///< 支持加密方式种类,EKeyAlgType组合值
	s8 szPrivateKeyPass[NET_MAXLEN_64 + 1];					///< 私钥密码
	s32 nPrivateKeyPassMin;									///< 私钥密码长度最小值
	s32 nPrivateKeyPassMax;									///< 私钥密码长度最大值
};

struct TNetSnmp3Param
{
	TNetSnmp3ReadParam tNetSnmp3ReadParam;
	TNetSnmp3WriteParam tNetSnmp3WriteParam;
};

struct TNetSnmpCfg
{
	BOOL32 bSnmpEnable;									///< snmp启动开关
	ESnmpVersion eSnmpVersion;							///< snmp版本
	u32 dwSnmpVersionOpt;								///< 支持snmp版本种类,ESnmpVersion组合值
	s8 szReadUnionName[NET_MAXLEN_64 + 1];				///< 读共同体名称
	s32 nReadUnionNameMin;								///< 读共同体名称长度最小值
	s32 nReadUnionNameMax;								///< 读共同体名称长度最大值
	s8 szWriteUnionName[NET_MAXLEN_64 + 1];				///< 写共同体名称
	s32 nWriteUnionNameMin;								///< 写共同体名称长度最小值
	s32 nWriteUnionNameMax;								///< 写共同体名称长度最大值
	s32 nSnmpPort;										///< snmp端口
	s32 nSnmpPortMin;									///< snmp端口长度最小值
	s32 nSnmpPortMax;									///< snmp端口长度最大值
	s8  szTrapAddr[NET_MAXLEN_32];						///< Trap地址
	s32 nTrapPort;										///< Trap端口
	s32 nTrapPortMin;									///< Trap端口长度最小值
	s32 nTrapPortMax;									///< Trap端口长度最大值
	s8  szTrapName[NET_MAXLEN_64 + 1];					///< Trap团体名
	s32 nTrapNameMin;									///< Trap团体名长度最小值
	s32 nTrapNameMax;									///< Trap团体名长度最大值
	s8  szPuId[NET_MAXLEN_64 + 1];						///< 设备入网ID
	TNetSnmp3Param tNetSnmp3Param;						///< snmp3参数
};

struct TNetGpsInfo
{
	s32 nSatNum;				///< 使用的卫星数量
	BOOL32 bAvailable;			///< 是否有效
	s8 szTime[NET_MAXLEN_32];	///< 时间(格式为:2016-03-15T12:48:01.330)
	double dLatitude;			///< 纬度
	double dLongitude;			///< 经度
	double dDirection;			///< 地面方向，正北方为0度，东方为90度，南方为180度，西方为270度
	double dAltitude;			///< 海拔高度，单位:m
	double dSpeed;				///< 地面速度，单位:千米/小时
}NETPACKED;

struct TNetCaptureStart
{
	s8 szNetID[NET_MAXLEN_8];				///< 网口ID
	ENetPackFilter eCaptureFilter;			///< 过滤类型
	s8 szFilePath[NET_MAXLEN_256 + 1];		///< 抓包目录
	u32 dwIpFilter;							///< IP过滤，网络序
	s8 szPortFilter[NET_MAXLEN_32 + 1];		///< 端口过滤
}NETPACKED;

struct TNetCaptureStop
{
	s32 nNetId;								///< 网口ID
	s8 szFileName[NET_MAXLEN_256 + 1];		///< 文件名字
}NETPACKED;

struct TNetCaptureState
{
	BOOL32 bCapture;                        ///< 是否正在抓包
	BOOL32 bFileExist;						///< 抓包文件是否存在
	s32 nNetIdNum;							///< 网口ID数量
	s8 szNetID[NET_MAXLEN_8];				///< 网口ID
	s8 szFileName[NET_MAXLEN_256 + 1];		///< 文件名字
	ENetPackFilter eCaptureFilter;			///< 过滤类型
	u32 dwIpFilter;							///< IP过滤，网络序
	s8 szPortFilter[NET_MAXLEN_32 + 1];		///< 端口过滤
}NETPACKED;

struct TNetPingStart
{
	s32 nChnID;							///< 通道ID
	s8 szDestIP[NET_MAXLEN_32+1];		///< 前端IP
	s32 nMtu;							///< MTU
	s32 nTimes;							///< PING 次数
	s32 nTimeOut;						///< 超时时间，秒
}NETPACKED;

struct TNetPingResult
{
	EPingReply ePingReply;				///< PING类型
	s32 nTTL;							///< TTL
	s32 nPackage;						///< 包
	s32 nDelay;							///< 延迟
}NETPACKED;

struct TNetPingResultList
{
	BOOL32 bFinish;						///< 是否完成
	s32 nResultNum;						///< 个数
	TNetPingResult tPingResult[NET_MAXLEN_8];
}NETPACKED;

struct TNetGetChnAlarmCfg
{
	s8 szChnID[NET_MAXLEN_256];    ///< 通道id，多个通道使用','分隔，例如：1,2,3,4
	BOOL32 bMoveDetect;            ///< 是否获取通道移动侦测配置
	BOOL32 bVideoLost;			   ///< 是否获取通道视频丢失配置
}NETPACKED;

struct TNetMdReg
{
	s32 nX;			///< 左上点坐标
	s32 nY; 		///< 右上点坐标
	s32 nWidth;		///< 区域宽度
	s32 nHeight;	///< 区域高度
}NETPACKED;

struct TNetChnAlarmCfg
{
	s32 nChnID;                         ///< 通道ID
	s32 nErrNo;							///< 错误号 NET_OK：获取成功
	BOOL32 bMdEnable;	                ///< 是否启用移动侦测
	s32 nSensiValue;                    ///< 灵敏度
	s32 nSensiMin;						///< 灵敏度最小值
	s32 nSensiMax;						///< 灵敏度最大值
	s32 nRegNum;                        ///< 区域数目
	s32 nRegMax;						///< 区域数目最大值
	s32 nRegWidthMax;                   ///< 区域最大宽度
	s32 nRegHeightMax;					///< 区域最大高度
	TNetMdReg tMdReg[NET_MAXLEN_8];     ///< 移动侦测区域信息
}NETPACKED;

struct TNetChnAlarmCfgList
{
	s32 nNum;															///< 数量
	TNetChnAlarmCfg atChnAlarmCfg[NET_PER_GET_CHN_ALARMCFG_MAX_NUM];	///< 通道告警配置(移动侦测)
}NETPACKED;

struct TNetGetChnAlarmState
{
	s32 nChnIdStart;	///< 起始通道id
	s32 nChnIdEnd;		///< 结束通道id
}NETPACKED;

struct TNetAlarmState
{
	s8 szAlarmSource[NET_MAXLEN_16];	///< 告警来源(可取值：nvr：表示NVR 数字：表示某个通道)
	EAlarmType eAlarmType;				///< 告警类型
	s32 nAlarmInId;						///< 告警输入号，eAlarmType为ALARMTYPE_ALARMIN时有效
	ESmartAlarmType eSmartAlarmType;	///< 智能告警类型，eAlarmType为ALARMTYPE_SMARTDETECT时有效
	ESysAlarmType eSysAlarmType;		///< 系统告警类型，eAlarmType为ALARMTYPE_SYSALARM时有效
	s32 nSysAlarmParam;					///< eAlarmType为ALARMTYPE_SYSALARM时有效，(1)硬盘故障时表示硬盘ID (2)录像满、抓拍满、前端掉线时表示通道ID (3)网络故障、ip冲突、mac冲突时表示网卡ID
	s8 szEthName[NET_MAXLEN_32];		///< eAlarmType为ALARMTYPE_SYSALARM时有效，网络故障、ip冲突、mac冲突时表示网卡名称
	s8 szAlarmTime[NET_MAXLEN_32];		///< 告警时间
}NETPACKED;

struct TNetAlarmStateList
{
	s32 nNum;														///< 数量
	TNetAlarmState atAlarmState[NET_PER_GET_ALARM_STATE_MAX_NUM];	///< 告警状态
}NETPACKED;

struct TNetDiskSmartTestAttr
{
	s32 nAttrID;							    ///< 属性ID
	s8 szAttrName[NET_MAXLEN_64];			    ///< 属性名
	s32 nFlag;				                    ///< Flag字段
	s32 nValue;					                ///< Value字段
	s32 nWorst;				                    ///< Worst字段
	s32 nThresh;				                ///< Thresh字段
	s8 szRawValue[NET_MAXLEN_16];				///< RawValue字段
	ESmartTestAttrStatus eSmartTestAttrStatus;  ///< 健康状态
}NETPACKED;

struct TNetDiskSmartTestInfo
{
	s8 szModelNo[NET_MAXLEN_64];		            ///< 硬盘型号
	s8 szSerialNo[NET_MAXLEN_64];					///< 硬盘序列号
	s32 nDiskTemperature;				            ///< 温度
	s32 nDiskUseTime;					            ///< 使用时间
	ESmartTestResultType eSmartTestResultType;	    ///< 检测结果
	s32 nAttrNum;                                   ///< 属性数量
	TNetDiskSmartTestAttr atDiskSmartTestAttr[NET_MAX_SMART_ATTR_ITEM_NUM]; ///< smart检测属性列表
}NETPACKED;

struct TNetDiskGeometryInfo
{
	s32 nCylindeNum;              ///< 柱面总数
	s32 nHeadNum;                 ///< 磁头总数
	s32 nSectorNum;               ///< 扇区数
	s32 nSectorSize;              ///< 扇区大小
	u64 ullLBATotalSize;          ///< LBA总大小
	u64 ullGeoStart;              ///< 分区起始位置
}NETPACKED;

struct TNetBadSectorCheckTaskInfo
{
	s32 nTaskId;                                 ///< 坏道检测任务id
	s32 nNvrDiskID;                              ///< NVR磁盘id
	EBadSectorCheckType eBadSectorCheckType;     ///< 坏道检测类型
	EBadSectorCheckState eBadSectorCheckState;   ///< 坏道检测状态
}NETPACKED;

struct TNetErrLbaInfo
{
	u64 ullLBA;                   ///< 当前逻辑块地址
	s32 nCylindeId;               ///< 当前柱面号
	s32 nHeadId;                  ///< 当前磁头号
	s32 nSectorId;                ///< 当前扇区号
	s32 nSectorNum;               ///< 扇区数目
	s32 nTimeCount;               ///< 耗时
	s8 szErrInfo[NET_MAXLEN_32];  ///< 错误信息
}NETPACKED;

struct TNetBadSectorCheckInfo
{
	EBadSectorCheckState eBadSectorCheckState;                ///< 坏道检测状态
	u64 ullCurCheckLBA;                                       ///< 当前正在检测的lba
	s32 nErrLbaNum;                                           ///< 出错的lba数量
	TNetErrLbaInfo atErrLbaInfo[NET_MAX_ERROR_SECTOR_NUM];    ///< 出错的lba信息
}NETPACKED;

struct TNetBandInfo
{
	s32 nNetID;							///< 网口ID
	u32 dwChnLinkBand;					///< 通道连接带宽
	u32 dwChnRemainBand;				///< 通道剩余带宽
	u32 dwCurNetPost;					///< 当前发送带宽
	u32 dwRemainNetPostCap;				///< 剩余带宽能力
}NETPACKED;

struct TNetBandParam
{
	s32 nNum;
	TNetBandInfo tNetBand[NET_MAXLEN_8];
}NETPACKED;

struct TNetVsipParam
{
	BOOL32 bEnable;				   ///< 是否启用
	BOOL32 bSendNat;			   ///< 是否发送Nat探测包
	s8  szRegMethod[NET_MAXLEN_8]; ///< 地址类型(url或ip)
	s8  szIpAddr[NET_MAXLEN_32];   ///< Ip地址
	s32 nIpAddrMax;				   ///< Ip地址最大字符
	s8  szDomain[NET_MAXLEN_32];   ///< 域名
	s32 nPlatPort;				   ///< 平台端口
	s32 nPlatPortMax;			   ///< 平台端口最大值
	s32 nPlatPortMin;			   ///< 平台端口最小值
	s8  szUUID[NET_MAXLEN_64];     ///< 入网UUID
	s32 nUUIDMax;                  ///< 入网UUID最大值
	s8  szPassWord[NET_MAXLEN_64]; ///< 入网设备密码
	s32 nPassWordMax;			   ///< 入网设备密码最大字符
	BOOL32 bReportSecStream;       ///< 是否上报辅流
	BOOL32 bReportZeroChn;         ///< 是否上报零通道
}NETPACKED;

struct TNetGB28181Info
{
	s32 nPlatIndex;					///< 平台索引
}NETPACKED;

struct TNetGBEncChns
{
	s8  szChnID[NET_MAXLEN_32];    ///< 编码通道国标id
	s8  szChnName[NET_MAXLEN_64 + 1];  ///< 编码通道的名称
	s32 nChnID;                    ///< 当前国标编码通道对应的通道号
	s32 nEncID;                    ///< 当前国标编码通道对应的编码通道ID(可选值: 1表示主流 2表示辅流)
}NETPACKED;

struct TNetGBAlmChns
{
	s8  szAlmID[NET_MAXLEN_32];		///< 告警通道国标id
	s8  szAlmName[NET_MAXLEN_64 + 1];	///< 告警通道的名称
	s8  szAlmSource[NET_MAXLEN_32];	///< 告警通道来源
	s32 nAlmInID;					///< 告警输入号
}NETPACKED;

struct TNetGB28181Param
{
	BOOL32 bEnable;                          ///< 是否启用
	s32 nLocalPort;							 ///< 本地端口
	s8  szDevID[NET_MAXLEN_32 + 1];          ///< 入网id
	s32 nDevIDMax;							 ///< 入网id最大值
	s8  szDevName[NET_MAXLEN_64 + 1];        ///< 设备名称
	s32 nDevNameMax;						 ///< 设备名称最大字符数
	s8 szPlatID[NET_MAXLEN_32 + 1];          ///< 平台id
	s32 nPlatIDMax;							 ///< 平台id最大值
	s8 szPlatIp[NET_MAXLEN_64 + 1];          ///< 平台ip
	s32 nPlatPort;							 ///< 平台端口
	s8 szUsrName[NET_MAXLEN_128 + 1];		 ///< 用户名
	s32 nUsrNameMax;						 ///< 用户名最大字符数
	s8 szPassWord[NET_MAXLEN_64 + 1];		 ///< 密码
	s32 nPassWordMax;						 ///< 密码最大字符数
	s32 nEncChnNum;                          ///< 视频编码通道数量
	s32 nEncChnNumMin;                       ///< 视频编码通道数量最小值
	s32 nEncChnNumMax;                       ///< 视频编码通道数量最大值
	TNetGBEncChns tNetGBEnc[NET_GB28181_ENC_CHN_MAX_NUM]; ///< 视频编码通道信息
	s32 nAlarmChnNum;                        ///< 告警通道数量
	s32 nAlarmChnNumMin;					 ///< 告警通道数量最小值
	s32 nAlarmChnNumMax;					 ///< 告警通道数量最大值
	TNetGBAlmChns tNetGBAlm[NET_GB28181_ALARM_CHN_MAX_NUM]; ///< 告警通道信息
	s32 nReletSpan;							 ///< 续租时间
	s32 nReletSpanMin;						 ///< 续租时间最小值
	s32 nReletSpanMax;						 ///< 续租时间最大值
	s32 nKplvSpan;							 ///< 心跳间隔
	s32 nKplvSpanMin;						 ///< 心跳间隔最小值
	s32 nKplvSpanMax;						 ///< 心跳间隔最大值
	s32 nKplvTimeOutMaxTimes;				 ///< 超时次数
	s32 nKplvTimeOutMaxTimesMin;			 ///< 超时次数最小值
	s32 nKplvTimeOutMaxTimesMax;			 ///< 超时次数最大值
	s8 szOwner[NET_MAXLEN_256 + 1];			 ///< 设备归属
	s32 nOwnerMax;							 ///< 设备归属最大字符
	s8 szCivilCode[NET_MAXLEN_256 + 1];		 ///< 行政区域
	s32 nCivilCodeMax;						 ///< 行政区域最大字符
	s8 szPoliceRgn[NET_MAXLEN_256 + 1];		 ///< 警区信息
	s32 nPoliceRgnMax;						 ///< 警区信息最大字符
	s8 szSetupAddress[NET_MAXLEN_256 + 1];	 ///< 安装地址
	s32 nSetupAddrMax;						 ///< 安装地址最大字符
	s8 szExtendCfgOpt[NET_MAXLEN_512];		 ///< 扩展配置项
	s8 szExtendCfg[NET_MAXLEN_256 * 4];		 ///< 国标高级扩展配置
	s32 nExtendCfgMax;						 ///< 国标高级扩展配置最大字符数量
	s8 szGbOrderOpt[NET_MAXLEN_64];          ///< opt表示可选的国标兼容次序模块名
	s8 szGbOrder[NET_MAXLEN_64];			 ///< 国标兼容次序
	BOOL32 bSupSecurityLev;					 ///< 是否支持安全等级配置
	u32 dwSecurityLevMask;				     ///< 支持的安全等级
	ESipSecurityLev eSipSecurityLev;		 ///< 安全等级	
}NETPACKED;

struct TNetOnvifParam
{
	BOOL32 bEnable;	                 ///< 是否启用
	s8 szUrl[NET_MAXLEN_256 + 1];    ///< 服务器地址(获取时有效,不可修改)
	EOnvifAuthType eOnvifAuthType;   ///< 鉴权方式
}NETPACKED;

struct TNetPubSecCfg
{
	BOOL32 bEnable;                          ///< 是否启用
	s8 szServerIp[NET_MAXLEN_64 + 1];        ///< 平台ip
	s32 nServerPort;						 ///< 服务器端口
	s8 szUsrName[NET_MAXLEN_128 + 1];		 ///< 用户名
	s32 nUsrNameMax;						 ///< 用户名长度最大字符数
	s8 szPassWord[NET_MAXLEN_64 + 1];		 ///< 密码
	s32 nPassWordMax;						 ///< 密码长度最大字符数
	s32 nKeepAlive;							 ///< 心跳间隔
	s32 nKeepAliveMax;						 ///< 心跳间隔最大值
	s32 nKeepAliveMin;						 ///< 心跳间隔最小值
	s8  szDevID[NET_MAXLEN_32 + 1];          ///< 设备ID
	s32 nDevIDMax;							 ///< 设备ID长度最大值
	s8  szSetupAddress[NET_MAXLEN_32 + 1];   ///< 安装地址
	s32 nSetupAddressMax;					 ///< 安装地址长度最大值
	s8  szCivilCode[NET_MAXLEN_32 + 1];      ///< 行政区域
	s32 nCivilCodeMax;						 ///< 行政区域长度最大值
}NETPACKED;

struct TNetQRCode
{
	EQRBmpType eQRBmpType;	///< 二维码图片类型
	s32 nQRBmpSize;			///< 二维码图片大小
	s8 szQRBmpData[8192];	///< 二维码图片数据
}NETPACKED;

struct TNetCloudCfg
{
	BOOL32 bEnable;									///< 是否启用云服务
	s8 szUmid[NET_MAXLEN_128];						///< 设备序列号
	s32 nQRCodeNum;									///< 二维码数量
	TNetQRCode atQRCode[NET_CLOUD_QR_CODE_MAX_NUM];	///< 二维码信息
}NETPACKED;

struct TNetGB28181CtlInfo
{
	s8 szGbId[NET_MAXLEN_64];			///< 国标ID
	s32 nLocalPort;						///< 服务端口
	s32 nKplvSpan;						///< 心跳间隔(单位秒)
	s32 nKplvSpanMin;					///< 心跳间隔最小值
	s32 nKplvSpanMax;					///< 心跳间隔最大值
	s32 nKplvTimeOutMaxTimes;           ///< 心跳最大次数
	s32 nKplvTimeOutMaxTimesMin;        ///< 心跳最大次数最小值  
	s32 nKplvTimeOutMaxTimesMax;        ///< 心跳最大次数最大值
}NETPACKED;

struct TNetPubSecCltCfg
{
	u16 wServerPort;						///< 服务端口
	s8 szUserName[NET_MAXLEN_32 * 3 + 1];	///< 用户名
	s8 szPwd[NET_MAXLEN_32 + 1];			///< 密码
}NETPACKED;

struct TNetAutoServiceCfg
{
	EAutoServiceType eAutoServiceType;	///< 自动维护类型
	s32 nDay;							///< 周几或几号，eAutoServiceType为AUTOSERVICETYPE_PERWEEK或AUTOSERVICETYPE_PERMONTH时有效，为AUTOSERVICETYPE_PERWEEK时，值为[1,7]，分别表示周一到周日，为AUTOSERVICETYPE_PERMONTH时，值为[1,31]
	s32 nHour;							///< 时
	s32 nMinute;						///< 分
	s32 nSecond;						///< 秒
}NETPACKED;

struct TNetNetAdvancedParam
{
	BOOL32 bEnable;					///< 是否启用
	s32 nCheckPoint1;               ///< 第一重传检测点
	s32 nCheckPoint2;				///< 第二重传检测点
	s32 nCheckPoint3;				///< 第三重传检测点
	s32 nOverdueDiscard;            ///< 过期丢弃时间
}NETPACKED;

struct TNetSysAdvancedCfg
{
	BOOL32 bEnableDiskPreRec;		///< 是否启用硬盘预录
	BOOL32 bEnableThirdStreamSup;	///< 是否启用三码流支持
	BOOL32 bSupDoubleAudio;			///< 是否支持双路音频支持
	BOOL32 bEnableDoubleAudioSup;   ///< 是否启用双路音频支持
	EAudioEncType eCurAudioCallEncType; ///< 当前的呼叫编码格式
	u32 dwSupAudioCallEncTypeMask;  ///< 支持的音频呼叫编码格式掩码，EAudioEncType组合值
	BOOL32 bSupRecBlockSize;		///< 是否支持设置录像块大小
	u32 dwSupRecBlockSizeMask;		///< 支持设置录像块大小的类型，ERecBlockSize组合值
	ERecBlockSize eCurRecBlockSize;	///< 当前录像块大小
	BOOL32 bSupPtzCtrlTime;			///< 是否支持PTZ独占释放时间
	s32 nPtzCtrlTimeMax;			///< PTZ独占释放时间最大值
	s32 nPtzCtrlTimeMin;			///< PTZ独占释放时间最小值
	s32 nPtzCtrlTime;				///< PTZ独占释放时间
	BOOL32 bSupAlarmDelayTime;		///< 是否支持报警合并间隔时间
	s32 nAlarmDelayTimeMax;			///< 报警合并间隔时间最大值
	s32 nAlarmDelayTimeMin;			///< 报警合并间隔时间最小值
	s32 nAlarmDelayTime;			///< 报警合并间隔时间
}NETPACKED;

struct TNetNvrNtyEventItem
{
	s8 szEventType[NET_MAXLEN_64];	///< 事件类型
}NETPACKED;

struct TNetNvrNtyEvent
{
	s32 nNum;													///< NVR通知事件数量
	TNetNvrNtyEventItem atEventItem[NET_NVR_NTY_EVENT_MAX_NUM];	///< NVR通知事件信息
}NETPACKED;

struct TNetSimInfo
{
	s32 nId;				///< SIM卡id，从1开始
	ESimStatus eSimStatus;	///< SIM卡状态
	ENetType eNetType;		///< 网络类型
	s32 nSignal;			///< 信号
}NETPACKED;

struct TNetGB28181RegPlatStatus
{
	s32 nPlatId;									///< 平台id，从1开始
	EGB28181RegPlatStatus eGB28181RegPlatStatus;	///< 注册状态
}NETPACKED;

struct TNetPubSecRegPlatStatus
{
	s32 nPlatId;								///< 平台id，从1开始
	EPubSecRegPlatStatus ePubSecRegPlatStatus;	///< 注册状态
}NETPACKED;

struct TNetNetRealTimeStatus
{
	BOOL32 bEnableGps;																///< 是否开启GPS
	BOOL32 bEnableWifiProbe;														///< 是否开启WIFI探针
	s32 nSimNum;																	///< SIM卡数量
	TNetSimInfo atSimInfo[NET_SIM_CARD_MAX_NUM];									///< SIM卡状态
	s32 nGB28181RegPlatNum;															///< 国标注册平台数量
	TNetGB28181RegPlatStatus atGB28181RegPlatStatus[NET_GB28181_REG_PLAT_MAX_NUM];	///< 国标注册平台状态
	s32 nPubSecRegPlatNum;															///< 视图库注册平台数量
	TNetPubSecRegPlatStatus atPubSecRegPlatStatus[NET_PUBSEC_REG_PLAT_MAX_NUM];		///< 视图库注册平台状态
}NETPACKED;

struct TNetAiuChnCap
{
	s32 nChnId;						///< 通道id
	BOOL32 bSupFaceDetect;			///< 是否支持人脸检测
	BOOL32 bSupCarPlateRecognise;	///< 是否支持车牌识别
}NETPACKED;

struct TNetAiuChnCapList
{
	s32 nNum;													///< 数量
	TNetAiuChnCap atAiuChnCap[NET_PER_GET_AIU_CHN_CAP_MAX_NUM];	///< AIU通道能力
}NETPACKED;

struct TNetRegion
{
	s32 nX;	///< x坐标
	s32 nY;	///< y坐标
	s32 nW;	///< 宽
	s32 nH;	///< 高
}NETPACKED;

struct TNetGetAiuRecentSnapPicList
{
	s32 nChnId;					///< 通道id
	EAiuPicType eAiuPicType;	///< Aiu图片类型
}NETPACKED;

struct TNetAiuFaceInfoItem
{
	TNetRegion tRegion;	///< 人脸坐标
}NETPACKED;

struct TNetAiuCarPlateInfoItem
{
	s8 szCarPlateNo[NET_MAXLEN_16];		///< 车牌号
	s8 szCarPlateColor[NET_MAXLEN_32];	///< 车牌颜色
	TNetRegion tRegion;					///< 车牌坐标
}NETPACKED;

struct TNetAiuAlgResult
{
	EPicSizeType ePicSizeType;												///< 图片大小类型
	EAiuAlgDetectType eAiuAlgDetectType;									///< 算法检测类型
	s32 nScore;																///< 得分
	s32 nAiuFaceInfoItemNum;												///< 人脸个数，人脸抓拍时有效
	TNetAiuFaceInfoItem atAiuFaceInfoItem[NET_FACE_MAX_NUM];				///< 人脸信息，人脸抓拍时有效
	s32 nAiuCarPlateInfoItemNum;											///< 车牌个数，车牌抓拍时有效
	TNetAiuCarPlateInfoItem atAiuCarPlateInfoItem[NET_CAR_PLATE_MAX_NUM];	///< 车牌信息，车牌抓拍时有效
}NETPACKED;

struct TNetPicUploadPubSecStatusItem
{
	s32 nPlatId;									///< 平台id
	EPicUploadPubSecStatus ePicUploadPubSecStatus;	///< 上传状态
}NETPACKED;

struct TNetAiuSnapPicItem
{
	u64 ullFileId;																					///< 文件id
	s8 szFilePath[NET_MAXLEN_512];																	///< 文件绝对路径
	EPicType ePicType;																				///< 图片类型
	s8 szSnapTime[NET_MAXLEN_32];																	///< 抓拍时间
	TNetAiuAlgResult tAiuAlgResult;																	///< 算法识别结果
	s32 nItemNum;																					///< 数量
	TNetPicUploadPubSecStatusItem atPicUploadPubSecStatusItem[NET_PIC_UPLOAD_PUBSEC_PLAT_MAX_NUM];	///< 上传状态
}NETPACKED;

struct TNetAiuSnapPicList
{
	s32 nSnapPicTotalNum;													///< 抓拍图片总数
	s32 nGroupNum;															///< 获取到的图片组数
	s32 nItemNum;															///< 获取到的每组图片数量
	TNetAiuSnapPicItem atAiuSnapPicItem[NET_AIU_RECENT_SNAP_PIC_GROUP_MAX_NUM][NET_AIU_RECENT_SNAP_PIC_ITEM_MAX_NUM];	///< 抓拍图片信息
}NETPACKED;

struct TNetStartEventRec
{
	s32 nChnId;							///< 通道id
	s32 nEncId;							///< 编码通道id，从1开始
	s8 szEventName[NET_MAXLEN_32 + 1];	///< 事件名称
	s8 szEventDesc[NET_MAXLEN_128 + 1];	///< 事件描述
	s32 nPreRecTime;					///< 预录时长，单位秒
	s32 nRecDelay;						///< 录像延时，单位秒
	BOOL32 bSnapPic;					///< 是否抓拍图片
}NETPACKED;

struct TNetEventRecInfo
{
	s32 nRecId;							///< 录像id
	s8 szEventName[NET_MAXLEN_32 + 1];	///< 事件名称
	s8 szEventDesc[NET_MAXLEN_128 + 1];	///< 事件描述
}NETPACKED;

struct TNetManualEventInfo
{
	s8 szEventName[NET_MAXLEN_16 * 3 + 1];	///< 事件名称
	s8 szEventDesc[NET_MAXLEN_64 * 3 + 1];	///< 事件描述
	BOOL32 bDefault;						///< 是否默认
}NETPACKED;

struct TNetManualEventList
{
	BOOL32 bEventSnap;                                              ///< 是否触发事件抓拍
	s32 nEventNum;                                                  ///< 手动事件数量
	TNetManualEventInfo atManualEventInfo[NET_MANUALEVENT_MAXNUM];  ///< 手动事件信息
}NETPACKED;

struct TNetManualEventCap
{
	BOOL32 bSup;       ///< 是否支持手动事件配置
	s32 nEventMaxNum;  ///< 手动事件最大数量
}NETPACKED;

struct TNetAiuManualTrigParam
{
	s32 nNvrChnID;        ///< nvr通道号
	EAiuType eAiuType;    ///< 智能类型
}NETPACKED;

struct TNetCtrlLibCap
{
	s32 nCtrlLibMaxNum;		///< 布控库最大数量 
	s32 nCustomLabelMaxNum;	///< 自定义标签最大数量
	u32 dwCtrlLibTypeMask;	///< 支持的布控库类型，ECtrlLibType组合值
	u32 dwIdTypeMask;		///< 支持的证件类型，EIdType组合值
	u32 dwSupMemNum;		///< 布控库支持的总人数
}NETPACKED;

struct TNetCustomLabelItem
{
	BOOL32 bEnable;							///< 是否启用
	s8 szLabelName[NET_MAXLEN_64 + 1];		///< 标签名称
	s8 szLabelContent[NET_MAXLEN_64 + 1];	///< 标签内容
}NETPACKED;

struct TNetCustomLabelList
{
	s32 nNum;																	///< 自定义标签数量
	TNetCustomLabelItem atCustomLabelItem[NET_CTRLLIB_CUSTOM_LABEL_MAX_NUM];	///< 自定义标签信息
}NETPACKED;

struct TNetCtrlLibItem
{
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];	///< 布控库名称
	ECtrlLibType eCtrlLibType;				///< 布控库类型
	u32 dwNum;								///< 布控库成员数量
	s8 szRecogEngine[NET_MAXLEN_32 + 1];	///< 识别引擎
	TNetCustomLabelList tCustomLabelList;	///< 自定义标签列表
	s8 szCreater[NET_MAXLEN_64 + 1];		///< 创建人
	s8 szCreateTime[NET_MAXLEN_32];			///< 创建时间(格式为:2016-03-15T12:48:01.330)
	u32 dwEigenvalueErr;					///< 布控库中是否含有特征值提取错误的成员数量(设置时不带此节点)
}NETPACKED;

struct TNetCtrlLibList
{
	s32 nNum;											///< 布控库数量
	TNetCtrlLibItem atCtrlLibItem[NET_CTRLLIB_MAX_NUM];	///< 布控库信息
}NETPACKED;

struct TNetEditCtrlLibState
{
	ECtrlLibImportState eState;	///< 编辑状态
	s32 nCurIndex;				///< 当前正在编辑的布控库成员索引
	s32 nTotalNum;				///< 需要编辑的布控库成员总数
}NETPACKED;

struct TNetSearchPersonLib
{
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];	///< 布控库名称
	u32 dwBeginIndex;						///< 搜索起始索引
	u32 dwNeedNum;							///< 需要搜索的数量
	s8 szName[NET_MAXLEN_64 + 1];			///< 姓名
	ESexType eSexType;						///< 性别
	EIdType eIdType;						///< 证件类型
	s8 szIdNumber[NET_MAXLEN_32 + 1];		///< 证件号码
	s8 szAddr[NET_MAXLEN_64 + 1];			///< 联系地址
	s8 szPhone[NET_MAXLEN_32 + 1];			///< 联系电话
	TNetDate tBirthDateStart;				///< 出生日期搜索的起始日期
	TNetDate tBirthDateEnd;					///< 出生日期搜索的结束日期
	s8 szAddTimeStart[NET_MAXLEN_32];		///< 入库时间搜索的起始时间
	s8 szAddTimeEnd[NET_MAXLEN_32];			///< 入库时间搜索的结束时间
	EEigenvalueType eEigenvalueType;		///< 特征值是否错误类型
	BOOL32 bGetTotalNum;					///< 是否需要总数
}NETPACKED;

struct TNetPersonItem
{
	u32 dwPrimarykey;						///< 主键
	s8 szName[NET_MAXLEN_64 + 1];			///< 姓名
	ESexType eSexType;						///< 性别
	EIdType eIdType;						///< 证件类型
	s8 szIdNumber[NET_MAXLEN_32 + 1];		///< 证件号码
	TNetDate tBirthDate;					///< 出生日期
	s8 szAddr[NET_MAXLEN_64 + 1];			///< 联系地址
	s8 szPhone[NET_MAXLEN_32 + 1];			///< 联系电话
	TNetCustomLabelList tCustomLabelList;	///< 自定义标签列表
	s8 szOperator[NET_MAXLEN_64 + 1];		///< 操作人
	s8 szAddTime[NET_MAXLEN_32];			///< 入库时间
	s8 szPicPath[NET_MAXLEN_128];			///< 图片路径
	s8 *pszPicData;							///< 图片信息
	s32 nPicDataLen;						///< 图片长度
	BOOL32 bEgiErr;							///< 该人员特征值提取是否错误
	ECtrlLibAddType eAddType;				///< 添加类型
}NETPACKED;

struct TNetPersonList
{
	BOOL32 bFinish;			///< 是否完成
	u32 dwTotalNum;			///< 搜索到的总数量
	u32 dwPersonItemNum;	///< 本次返回的人员数量
	TNetPersonItem atPersonItem[NET_PERSON_ITEM_MAX_NUM];	///< 人员信息
}NETPACKED;

struct TNetDelPerson
{
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];			///< 布控库名称
	s32 nNum;										///< 主键数量
	u32 adwPrimarykey[NET_PER_DEL_PERSON_MAX_NUM];	///< 主键
}NETPACKED;

struct TNetPicData
{
	s8 szPicPath[NET_MAXLEN_128];	///< 图片路径
	s8 *pszPicData;					///< 图片数据
	s32 nPicDataLen;				///< 图片数据长度
}NETPACKED;

struct TNetCtrlLibExport
{
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];			///< 布控库名称
	s8 szExportPath[NET_MAXLEN_512];				///< GUI导出路径
	ECtrlLibExportType eExportType;					///< 导出类型
	s32 nNum;										///< 主键数量
	u32 adwPrimarykey[NET_PER_DEL_PERSON_MAX_NUM];	///< 主键
	EClientType	eClientType;						///< 客户端类型
}NETPACKED;

struct TNetCtrlLibExportFlieList
{
	s32 nNum;												///< 文件数量
	s8 aszFileName[NET_CTRLLIB_EXPORT_FILE_MAX_NUM][512];	///< 文件名
}NETPACKED;

struct TNetCtrlLibExportFlieData
{
	ECtrlLibExportState eState;		///< 导出状态
	s8 *pszData;					///< 文件数据
	s32 nDataLen;					///< 数据长度
}NETPACKED;

struct TNetCtrlLibImportState
{
	ECtrlLibImportState eImportState;					///< 导入状态
	u32 dwTotalNum;										///< 导入总数量
	u32 dwCurIndex;										///< 当前导入位置
}NETPACKED;

struct TNetCtrlLibImportSend
{ 
	s8 szFileName[NET_MAXLEN_512];						  ///< 导入文件名
	s32 nDataStartPos;                                    ///< 数据起始偏移
	s32 nDataLen;                                         ///< 数据buffer长度
	s8 *pszDataBuf;                                       ///< 数据buffer内容
}NETPACKED;

struct TNetCtrlLibImportParam
{ 
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];			///< 布控库名称
	EClientType	eClientType;						///< 客户端类型
	s8 szImportPath[NET_MAXLEN_512];				///< GUI导出路径
}NETPACKED;

struct TNetCtrlLibCopy
{
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];						///< 复制的源布控库名称
	s32 nNum;													///< 主键数量
	u32 adwPrimarykey[NET_PER_DEL_PERSON_MAX_NUM];				///< 主键
	s32 nDstNum;												///< 目标布控库数量
	s8 aszDstCtrlLib[NET_CTRLLIB_MAX_NUM][NET_MAXLEN_64 + 1];	///< 目标布控库名称
}NETPACKED;

struct TNetCopyErrLibInfo
{
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];							///< 提取特征值错误对应的布控库索引
	s32 nNum;														///< 成员名称数量
	s8 aszDstCtrlLib[NET_PERSON_ITEM_MAX_NUM][NET_MAXLEN_64 + 1];	///< 成员名称
}NETPACKED;

struct TNetCtrlLibCopyErrList
{
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];						///< 复制的源布控库名称
	s32 nNum;													///< 布控库提取特征值错误信息列表数量
	TNetCopyErrLibInfo atCopyErrLibInfo[NET_CTRLLIB_MAX_NUM];	///< 布控库提取特征值错误信息列表
}NETPACKED;

struct TNetCtrlLibBatch
{
	ECtrlLibBatchType eBatchType;								///< 批量添加模式
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];						///< 布控库名称
	s8 szFilePath[NET_MAXLEN_512];								///< GUI文件路径
}NETPACKED;

struct TNetCtrlLibBatchPrepare
{
	BOOL32 bContinue;											///< 是否续传任务
	s8 szCtrlLibName[NET_MAXLEN_64 + 1];						///< 布控库名称
	EClientType	eClientType;									///< 客户端类型
	s8 szMac[NET_MAXLEN_64];									///< 批量添加对端的mac地址
	ECtrlLibBatchType eBatchType;								///< 批量添加模式
	s8 szFilePath[NET_MAXLEN_512];								///< GUI文件路径
}NETPACKED;

struct TNetCtrlLibBatchState
{
	ECtrlLibImportState eBatchState;	///< 布控库批量添加状态
	u32 dwTotalNum;						///< 添加总数
	u32 dwErrNum;						///< 添加出错的数量
	u32 dwSucNum;						///< 添加成功的数量
}NETPACKED;

struct TNetCtrlLibErrInfo
{
	u32 dwBeginIndex;					///< 搜索起始索引
	u32 dwNeedNum;						///< 当前需要数量
}NETPACKED;

struct TNetCtrlLibErrList
{
	u32 dwTotalNum;															///< 搜索总数
	u32 dwReturnNum;														///< 当前返回总数目
	s8 aszMemberName[NET_GET_ERR_PERSON_ITEM_MAX_NUM][NET_MAXLEN_64 + 1];	///< 成员名称
}NETPACKED;

struct TNetAlgEngineCap
{
	BOOL32 bSupAlgDownload;				///< 是否支持算法引擎加载和卸载
	u32 dwSupType;						///< 支持的算法引擎(EAlgType组合值)
	u32 dwAlgTotalSize;					///< 允许最大算法引擎总大小(单位字节)
}NETPACKED;

struct TNetAlgEngine
{
	s8 szName[NET_MAXLEN_64 + 1];			///< 算法引擎名称
	u32 dwSize;								///< 算法引擎大小(单位字节)
	u32 dwSupType;							///< 支持的功能类型(EAlgType组合值)
}NETPACKED;

struct TNetAlgEngineList
{
	s32 nNum;														///< 数量
	TNetAlgEngine atNetAlgEngine[NET_ALGENGINE_MAX_NUM];			///< 算法引擎列表
}NETPACKED;

struct TNetAlgEngineUpload
{
	s32 nTaskId;					///< 智能算法加载任务ID
	s8* pszAlgEngineData;			///< 数据buffer内容(base64加密)
	s32 nAlgEngineDataLen;			///< 数据buffer长度
}NETPACKED;

struct TNetAiuUploadCaps
{
	EAiuUploadType eAiuUploadTypes;			///< 上传数据支持的类型(EAiuUploadType组合值)
	u32 dwAiuUploadCaps;					///< 内容可选值,支持多种类型时用 "," 分隔(EAiuUploadCaps组合值)
}NETPACKED;

struct TNetNvrAiuCap
{
	s32 nAlgMaxChnNum;					///< 支持智能算法分析最大路数
	s32 nManDetectMaxNum;				///< 支持人脸检测算法最大路数
	s32 nCarPlateRegMaxNum;				///< 支持车牌识别算法最大路数
	BOOL32 bVAPData;					///< 是否支持视音图大数据	
	BOOL32 bSupGuardTime;				///< 是否支持智能布防时间	
	BOOL32 bSupBackUp;					///< 智能数据和音视图是否支持备份功能	
	BOOL32 bFace;						///< 是否支持接收ipc智能人员图片	
	BOOL32 bCarPlate;					///< 是否支持接收ipc智能车辆图片
	s32 nAlgUploadCapsNum;				///< 上传模式的支持类型数量
	TNetAiuUploadCaps atNetAiuUploadCaps[NET_AIUUPLOADCAPS_MAX_NUM]; ///< 上传模式的支持类型
}NETPACKED;

struct TNetNvrVAPDataCap
{
	BOOL32 bVAPData;					///< 是否支持视音图大数据	
	BOOL32 bSupGuardTime;				///< 是否支持智能布防时间	
	s32 nAlgUploadCapsNum;				///< 上传模式的支持类型数量
	TNetAiuUploadCaps atNetAiuUploadCaps[NET_AIUUPLOADCAPS_MAX_NUM]; ///< 上传模式的支持类型
}NETPACKED;

struct TNetAisChnCap
{
	s32 nChnId;							///< 通道号
	BOOL32 bEvtData;					///< 通道是否支持事件视频	
	BOOL32 bAudData;					///< 通道是否支持音频大数据	
	BOOL32 bVidData;					///< 通道是否支持视频大数据	
	BOOL32 bPicData;					///< 通道是否支持图片大数据	
}NETPACKED;

struct TNetGetAiuBigDataCfg
{
	s32 nChnNum;									///< NVR通道ID数量
	s32 anChnId[NET_AIU_GETBIGDATACFG_MAX_NUM];		///< 通道ID
	BOOL32 bGuardTime;								///< 是否需要携带布防时间
}NETPACKED;

struct TNetAiuBigDataUpload
{
	BOOL32 bEnable;					///< 是否启用上传
	EAiuUploadType eAiuUploadType;	///< 上传模式
}NETPACKED;

struct TNetAiuBigDataEvent
{
	s8 szName[NET_MAXLEN_64 + 1];		///< 事件名称	
}NETPACKED;

struct TNetAiuBigDataEventVideo
{
	BOOL32 bNotUseMobileNet;					///< 是否不使用2g/3g/4g移动网络同步
	s32 nUploadNum;								///< 上传列表数量
	TNetAiuBigDataUpload atNetAiuBigDataUpload[NET_AIU_BIGDATA_UPLOAD_MAX_NUM];///< 上传列表
	s32 nEventNum;								///< 事件名称列表数量
	TNetAiuBigDataEvent atNetAiuBigDataEvent[NET_AIU_BIGDATA_UPLOAD_MAX_NUM];///< 事件名称列表
}NETPACKED;

struct TNetAiuBigDataCommonVideo
{
	BOOL32 bNotUseMobileNet;					///< 是否不使用2g/3g/4g移动网络同步
	s32 nUploadNum;								///< 上传列表数量
	TNetAiuBigDataUpload atNetAiuBigDataUpload[NET_AIU_BIGDATA_UPLOAD_MAX_NUM];///< 上传列表
}NETPACKED;

struct TNetAiuBigDataCfg
{
	s32 nChnId;												///< 通道ID
	TNetAiuBigDataEventVideo tNetAiuBigDataEventVideo;		///< 事件视频
	TNetAiuBigDataCommonVideo tNetAiuBigDataCommonVideo;	///< 常规视频
	TNetAiuBigDataCommonVideo tNetAiuBigDataCommonAudio;	///< 常规音频
	TNetAiuBigDataCommonVideo tNetAiuBigDataCommonPic;		///< 常规图片
	s32 nGuardTimeNum;															///< 数量
	TNetAlarmGuardTimeItem atAlarmGuardTimeItem[NET_ALARM_GUARD_TIME_MAX_NUM];	///< 布防时间
}NETPACKED;

struct TNetAiuBigDataCfgList
{
	s32 nChnNum;									///< 通道数量
	TNetAiuBigDataCfg atNetAiuBigDataCfg[NET_AIU_GETBIGDATACFG_MAX_NUM];///< 通道配置信息
}NETPACKED;

struct TNetAiChnDetect
{
	s32 nChnId;							///< 通道号
	u32 dwDetectType;					///< 通道支持检测算法类型(EAlgType组合值)
}NETPACKED;

struct TNetAiProcessUploadMode
{
	EAiuUploadType eAiuUploadType;		///< 上传方式
	u32 dwUploadMode;					///< 支持的上传类型(EAipUploadMode组合值)
}NETPACKED;

struct TNetAiProcessUploadAlgType
{
	EAlgType eAlgType;														///< 算法类型
	s32 nNum;																///< 数量						
	TNetAiProcessUploadMode atAiProcessUpload[NET_AIP_UPLOAD_TYPE_MAX_NUM];	///< 上传方式			
}NETPACKED;

struct TNetAiProcessUpload
{
	s32 nNum;															///< 数量
	TNetAiProcessUploadAlgType atUploadAlgType[NET_ALG_TYPE_MAX_NUM];	///< 算法对应上传方式能力			
}NETPACKED;

struct TNetAiProcessCap
{
	BOOL32 bFixChnDetect;				///< 是否固定那几个通道做智能检测
	s32 nFaceMaxNum;					///< 支持人脸最大智能检测数
	s32 nCarMaxNum;						///< 支持车辆最大智能检测数
	s32 nPersonMaxNum;					///< 支持行人最大智能检测数
	s32 nDetectNumPerChn;				///< 每个通道支持的检测数量
	s32 nDetectChnNum;					///< 支持最大智能检测通道数(非固定)
	u32 dwDetectType;					///< 通道支持检测算法类型(EAlgType组合值)(非固定)
	s32 nNum;							///< 支持检测的通道列表数量(固定)
	TNetAiChnDetect atNetAiChnDetect[NET_AIP_DETECT_CHN_MAX_NUM];	///< 支持检测的通道列表(固定)
	s32 nCompareRuleNum;				///< 支持的最大比对规则个数
	s32 nCompareChnNum;					///< 支持的最大比对通道个数
	u32 dwDetectMode;					///< 支持的检测模式(EAipDetectMode组合值)
	u32 dwRmRepeatMode;					///< 支持的去重模式(EAipRmRepeatMode组合值)	
	TNetAiProcessUpload	tAiProcessUpload; ///< 算法支持的上传模式/上传方式
	u32 dwCompareMode;					///< 支持的比对模式(EAipCompareMode组合值)
	BOOL32 bSupDetectArea;				///< 是否支持绘制区域
	s32 nWidthMin;						///< 绘制的矩形宽度最小值
	s32 nWidthMax;						///< 绘制的矩形宽度最大值
	s32 nHeightMin;						///< 绘制的矩形高度最小值
	s32 nHeightMax;						///< 绘制的矩形高度最大值
	s32 nSenMin;						///< 检测灵敏度最小值
	s32 nSenMax;						///< 检测灵敏度最大值
	s32 nSimilarityMin;					///< 相似度范围最小值
	s32 nSimilarityMax;					///< 相似度范围最大值
	u32 dwDetectIntervalTypeMask;		///< 检测间隔时间支持算法类型,(EAlgType组合值)
	s32 nDetectIntervalMin;				///< 检测间隔时间范围最小值，单位s
	s32 nDetectIntervalMax;				///< 检测间隔时间范围最大值，单位s
	BOOL32 bSimCard;					///< 是否支持移动网络
	u32	dwAlarmDedupIntervalSupMask;	///< 告警去重时间间隔是否支持(EAipCompareMode组合值)
	s32 nAlarmDedupIntervalMin;			///< 告警去重时间间隔范围最小值
	s32 nAlarmDedupIntervalMax;			///< 告警去重时间间隔范围最大值
	u32	dwAlarmLinkUploadModeMask;		///< 智能告警联动上传模式支持(EAlarmLinkUploadMode组合值)
	u32	dwAlarmLinkUploadTypeMask;		///< 智能告警联动上传类型支持(EAlarmLinkUploadType组合值)
	s32 nAlgDetectTotalNum;				///< 支持的算法检测总数
	BOOL32 bSupSnapPicQuailty;			///< 抓拍图片质量功能是否支持
	u32 dwSnapPicQuailtyMask;			///< 支持的抓拍图片质量(ESnapPicQuailty组合值)
}NETPACKED;

struct TNetIntellChnParam
{
	s32 nChnIDStart;					///< 起始通道号
	s32 nMaxNum;						///< 要获取的最大个数
}NETPACKED;

struct TNetIntellChn
{
	s32 nChnID;								///< 通道ID
	s32 nDevId;								///< 该通道对应设备编号
	s32 nDevSrcID;						    ///< 设备的视频源ID
	s8 szChnAlias[NET_MAXLEN_64 + 1];		///< NVR通道别名
	BOOL32 bNormalAdd;						///< 是否已作为普通通道添加(设置不带此节点)
	s8 szIpAddr[NET_MAXLEN_32 + 1];			///< 智能通道IP
	BOOL32 bOnline;							///< 是否在线(设置不带此节点)
}NETPACKED;

struct TNetIntellChnList
{
	s32 nNum;												///< 智能通道列表数量
	TNetIntellChn atIntellChn[NET_AIP_DETECT_CHN_MAX_NUM];	///< 智能通道列表
}NETPACKED;

struct TNetAiDetectAlgParam
{
	BOOL32 bEnable;							///< 是否启用
	EAlgType eAlgType;						///< 检测类型
	EAipDetectMode eDetectMode;				///< 检测模式
}NETPACKED;

struct TNetAiDetectChn
{
	s32 nChnId;								///< 通道号
	s32 nNum;								///< 智能通道列表数量
	TNetAiDetectAlgParam atNetAiDetectAlgParam[NET_AIP_AI_CHN_ALG_PARAM_MAX_NUM];	///< 智能通道列表
}NETPACKED;

struct TNetChnDetectAlgList
{
	s32 nNum;								///< 通道检测算法列表数量
	TNetAiDetectChn atNetAiDetectChn[NET_AIP_DETECT_CHN_MAX_NUM];	///< 通道检测算法列表
}NETPACKED;

struct TNetDelChnDetectAlg
{
	s32 nChnId;								///< 通道号
	EAlgType eAlgType;						///< 检测类型
}NETPACKED;

struct TNetGetChnDetectAlgParam
{
	s32 nChnId;								///< 通道号
	EAlgType eAlgType;						///< 检测类型
}NETPACKED;

struct TNetChnDetectAlgParamSet
{
	BOOL32 bEnable;							///< 是否启用
	EAlgType eAlgType;						///< 检测类型
	EAipDetectMode eDetectMode;				///< 检测模式
	s8 szEngineName[NET_MAXLEN_64 + 1];		///< 引擎名称
	TNetRegion tNetRectRegion;				///< 长方形面积，10000*10000
	s32 nSensitivity;						///< 灵敏度
	EAipRmRepeatMode eAipRmRepeatMode;		///< 去重模式
	BOOL32 bOnlyDetectPic;					///< 仅检测抓拍图片,FALSE表示视频流和手动图片都检测,TRUE表示仅检测抓拍图片
	s32 nDetectInterval;					///< 检测间隔时间
	ESnapPicQuailty eSnapPicQuailty;		///< 抓拍图片质量
}NETPACKED;

struct TNetAipUpload
{
	EAiuUploadType eAiuUploadType;	///< 上传方式
	BOOL32 bEnable;					///< 是否启用上传
	EAipUploadMode eAipUploadMode;	///< 上传模式
}NETPACKED;

struct TNetChnDetectAlgProcessParam
{
	EAlgType eAlgType;						///< 检测类型
	s32 nNum;								///< 上传列表数量
	TNetAipUpload atNetAipUpload[NET_AIU_BIGDATA_UPLOAD_MAX_NUM];			///< 上传列表
	BOOL32 bNotUseMobileNet;				///< 是否不使用移动网络上传
}NETPACKED;

struct TNetChnDetectAlgParam
{
	s32 nChnId;														///< 通道号
	TNetChnDetectAlgParamSet tNetChnDetectAlgParamSet;				///< 参数设置
	TNetChnDetectAlgProcessParam tNetChnDetectAlgProcessParam;		///< 处理方式
}NETPACKED;

struct TNetDetectAlgParam
{
	TNetChnDetectAlgParamSet tNetChnDetectAlgParamSet;				///< 参数设置
	TNetChnDetectAlgProcessParam tNetChnDetectAlgProcessParam;		///< 处理方式
}NETPACKED;

struct TNetDetectAlgDefaultParam
{
	TNetDetectAlgParam tFaceDetectAlgParam;				///< 人脸
	TNetDetectAlgParam tCarDetectAlgParam;				///< 车辆
}NETPACKED;

struct TNetCompareRule
{
	s32 nIndex;									///< 比对规则索引
	s8 szRuleName[NET_MAXLEN_64 + 1];			///< 规则名称
	s8 szLibName[NET_MAXLEN_64 + 1];			///< 布控库名称
	EAipCompareMode eAipCompareMode;				///< 比对模式
	s32 nSimilarity;							///< 相似度
	s32 nChnNum;								///< 通道数量
	s32 anChnId[NET_AIP_COMPARE_RULE_CHN_MAX_NUM];///< 通道号
	s32 nAlarmDedupInterval;					///< 告警去重时间间隔
}NETPACKED;

struct TNetCompareRuleList
{
	s32 nNum;									///< 列表数量
	TNetCompareRule atCompareRule[NET_AIP_COMPARE_RULE_MAX_NUM];///< 比对规则信息列表
}NETPACKED;

struct TNetCompareRuleTime
{
	s32 nNum;															///< 数量
	TNetAlarmGuardTimeItem atTimeItem[NET_ALARM_GUARD_TIME_MAX_NUM];	///< 布防时间
}NETPACKED;

struct TNetRuleLinkUpload
{
	BOOL32 bEnable;									///< 是否启用上传
	EAiuUploadType eUploadType;						///< 上传模式
	EAlarmLinkUploadMode eUploadPicMode;			///< 上传图片模式
	EAlarmLinkUploadType eUploadPicType;			///< 上传图片类型
}NETPACKED;

struct TNetRuleLinkCfg
{
	s32 nIndex;																///< 比对规则索引
	u32 dwAlarmBasicLinkMask;												///< 常规联动，EAlarmBasicLink组合值
	s32 nAlarmLinkOutNum;													///< 联动输出数量
	TNetAlarmLinkOutItem atAlarmLinkOutItem[NET_ALARM_LINK_OUT_MAX_NUM];	///< 联动输出
	s32 nRuleLinkUploadNum;													///< 上传视图库数量
	TNetRuleLinkUpload atRuleLinkUpload[NET_AI_RULELINK_UPLOAD_MAX_NUM];	///< 上传视图库列表
	s8 szLinkChnRec[NET_MAXLEN_512];										///< 联动通道录像，通道id，多项使用逗号分隔
	s8 szLinkChnSnap[NET_MAXLEN_512];										///< 联动通道抓拍，通道id，多项使用逗号分隔
	s32 nAlarmLinkPtzNum;													///< 联动PTZ数量
	TNetAlarmLinkPtzItem atAlarmLinkPtzItem[NET_ALARM_LINK_PTZ_MAX_NUM];	///< 联动PTZ
}NETPACKED;

struct TNetFaceSnapSearch
{
	s32 nChnNum;															///< 通道数量
	s32 anChnId[NET_AISEARCH_CHN_MAX_NUM];									///< 通道ID
	s32 nStartIndex;														///< 开始索引
	s32 nNeedNum;															///< 当前需要的数量
	s8 szStartTime[NET_MAXLEN_32];											///< 搜索的起始时间
	s8 szEndTime[NET_MAXLEN_32];											///< 搜索的结束时间
	s32 nSimilar;															///< 相似度
	s8 *pszPicData;															///< 图片信息
	s32 nPicDataLen;														///< 图片长度
}NETPACKED;

struct TNetFaceSnapItem
{
	s32 nChnId;															///< 通道号 
	s8 szTime[NET_MAXLEN_32];											///< 抓拍时间
	s32 nAge;															///< 年龄
	ESexType eSexType;													///< 性别
	s8 szPicPath[NET_MAXLEN_128];										///< 图片路径
	s32 nSimilar;														///< 相似度
}NETPACKED;

struct TNetFaceSnapList
{
	BOOL32 bFinish;															///< 是否完成
	u32 dwTotalNum;															///< 搜索总数
	u32 dwItemNum;															///< 当前返回的数量
	TNetFaceSnapItem atFaceSnapItem[NET_FACE_SNAP_ITEM_MAX_NUM];			///< 搜索结果列表
	s8 szLastItemTime[NET_MAXLEN_32];										///< 当前搜索条件最后一条时间
}NETPACKED;

struct TNetLogAlarmSearch
{															
	s8 szLibName[NET_MAXLEN_64 + 1];										///< 布控库名称
	s32 nChnNum;															///< 通道数量
	s32 anChnId[NET_AISEARCH_CHN_MAX_NUM];									///< 通道ID
	ELogAlarmType eLogAlarmType;											///< 日志类型
	s32 nStartIndex;														///< 开始索引
	s32 nNeedNum;															///< 当前需要的数量
	s8 szStartTime[NET_MAXLEN_32];											///< 搜索的起始时间
	s8 szEndTime[NET_MAXLEN_32];											///< 搜索的结束时间
	s8 szName[NET_MAXLEN_64 + 1];											///< 姓名
	ESexType eSexType;														///< 性别
	EIdType eIdType;														///< 证件类型
	s8 szIdNumber[NET_MAXLEN_32 + 1];										///< 证件号码
	s8 szAddr[NET_MAXLEN_64 + 1];											///< 联系地址
	s8 szPhone[NET_MAXLEN_32 + 1];											///< 联系电话
	TNetDate tBirthDateStart;												///< 出生日期搜索的起始日期
	TNetDate tBirthDateEnd;													///< 出生日期搜索的结束日期
}NETPACKED;

struct TNetPersonInfo
{
	s8 szName[NET_MAXLEN_64 + 1];			///< 姓名
	ESexType eSexType;						///< 性别
	EIdType eIdType;						///< 证件类型
	s8 szIdNumber[NET_MAXLEN_32 + 1];		///< 证件号码
	TNetDate tBirthDate;					///< 出生日期
	s8 szAddr[NET_MAXLEN_64 + 1];			///< 联系地址
	s8 szPhone[NET_MAXLEN_32 + 1];			///< 联系电话
	TNetCustomLabelList tCustomLabelList;	///< 自定义标签列表
	s8 szOperator[NET_MAXLEN_64 + 1];		///< 操作人
	s8 szAddTime[NET_MAXLEN_32];			///< 入库时间
	s8 szPicPath[NET_MAXLEN_128];			///< 图片路径
}NETPACKED;

struct TNetLogAlarmItem
{
	s32 nChnId;															///< 通道号 
	s8 szTime[NET_MAXLEN_32];											///< 抓拍时间
	s32 nSimilar;														///< 相似度
	s8 szPicPath[NET_MAXLEN_128];										///< 图片路径
	s8 szLibPicPath[NET_MAXLEN_128];									///< 视图库中成员图片路径
	TNetPersonInfo tPersonInfo;											///< 人员信息
}NETPACKED;

struct TNetLogAlarmList
{
	BOOL32 bFinish;															///< 是否完成
	u32 dwTotalNum;															///< 搜索总数
	u32 dwItemNum;															///< 当前返回的数量
	TNetLogAlarmItem atAlarmLogItem[NET_FACE_SNAP_ITEM_MAX_NUM];			///< 搜索结果列表
	s8 szLastItemTime[NET_MAXLEN_32];										///< 当前搜索条件最后一条时间
}NETPACKED;

struct TNetPersonSnapSearch
{
	s32 nChnNum;															///< 通道数量
	s32 anChnId[NET_AISEARCH_CHN_MAX_NUM];									///< 通道ID
	s32 nStartIndex;														///< 开始索引
	s32 nNeedNum;															///< 当前需要的数量
	s8 szStartTime[NET_MAXLEN_32];											///< 搜索的起始时间
	s8 szEndTime[NET_MAXLEN_32];											///< 搜索的结束时间
	s8 *pszPicData;															///< 图片信息
	s32 nPicDataLen;														///< 图片长度
}NETPACKED;

struct TNetPersonSnapItem
{
	s32 nChnId;															///< 通道号 
	s8 szTime[NET_MAXLEN_32];											///< 抓拍时间
	s8 szPicPath[NET_MAXLEN_128];										///< 图片路径
}NETPACKED;

struct TNetPersonSnapList
{
	BOOL32 bFinish;															///< 是否完成
	u32 dwTotalNum;															///< 搜索总数
	u32 dwItemNum;															///< 当前返回的数量
	TNetPersonSnapItem atPersonSnapItem[NET_FACE_SNAP_ITEM_MAX_NUM];		///< 搜索结果列表
	s8 szLastItemTime[NET_MAXLEN_32];										///< 当前搜索条件最后一条时间
}NETPACKED;

struct TNetSmallVideoParam
{
	s32 nPreStartTime;	///< 短视频提前播放时间，默认提前5s
	s32 nDuration;		///< 时长，默认60s
}NETPACKED;

struct TNetSnapCfg
{
	s32 nTimerInterval;		///< 抓拍时间间隔(秒)
	s32 nSnapNum;			///< 抓拍张数
}NETPACKED;

struct TNetManualSnap
{
	s8 szEventName[NET_MAXLEN_32 + 1];			///< 事件名称
	s8 szDesc[NET_MAXLEN_128 + 1];				///< 描述信息
}NETPACKED;

struct TNetSeriesSnap
{
	s32 nChnID;							///< 通道ID
	s32 nEncID;							///< 编码ID
	ESeriesSnapPicType ePicType;		///< 图片类型
	TNetManualSnap tManualSnap;			///< 手动抓拍图片信息(当ePicType为SNAPPICTYPE_MANUAL_SNAP时生效)
}NETPACKED;

struct TNetPicUploadStatus
{
	BOOL32 bUploadPubSec1;					///< 是否上传视图库1
	EPicUploadStatus ePubSec1UploadStatus;	///< 视图库1上传状态
	BOOL32 bUploadPubSec2;					///< 是否上传视图库2
	EPicUploadStatus ePubSec2UploadStatus;	///< 视图库2上传状态
	BOOL32 bUploadFtp;						///< 是否上传FTP
	EPicUploadStatus eFtpUploadStatus;		///< FTP上传状态
	BOOL32 bUploadPad;						///< 是否上传PAD
	EPicUploadStatus ePadUploadStatus;		///< PAD上传状态
	s8 szPicPath[NET_MAXLEN_128];			///< 图片路径，推送图片状态消息时有效
}NETPACKED;

struct TNetDetectSnapItem
{
	s32 nChnId;																///< 通道id
	s8 szSnapTime[NET_MAXLEN_32];											///< 抓拍时间(格式为:2016-03-15T12:48:01.330)
	EAlgType eAlgType;														///< 检测类型
	EPicType ePicType;														///< 图片类型
	TNetPicData tSmallPic;													///< 小图
	TNetPicData tOrigPic;													///< 原图
	TNetPicUploadStatus tSmallPicUploadStatus;								///< 小图上传状态
	TNetPicUploadStatus tOrigPicUploadStatus;								///< 原图上传状态
	s32 nAiuCarPlateInfoItemNum;											///< 车牌个数，车牌抓拍时有效
	TNetAiuCarPlateInfoItem atAiuCarPlateInfoItem[NET_CAR_PLATE_MAX_NUM];	///< 车牌信息，车牌抓拍时有效
}NETPACKED;

struct TNetDetectSnapList
{
	s32 nSnapFaceTotalNum;												///< 当天抓拍人脸总数
	s32 nSnapCarTotalNum;												///< 当天抓拍车辆总数
	s32 nPersonDetectTotalNum;											///< 当天行人检测总数
	s32 nNum;															///< 检测抓拍数量
	TNetDetectSnapItem atDetectSnapItem[NET_DETECT_SNAP_LIST_MAX_NUM];	///< 检测抓拍信息
}NETPACKED;

struct TNetCmpAlarmItem
{
	s32 nChnId;								///< 通道id
	s8 szAlarmTime[NET_MAXLEN_32];			///< 报警时间(格式为:2016-03-15T12:48:01.330)
	ELogAlarmType eLogAlarmType;			///< 报警类型
	EPicType ePicType;						///< 图片类型
	s32 nSimilar;							///< 相似度，白名单报警无效
	s8 szName[NET_MAXLEN_64 + 1];			///< 姓名，白名单报警无效
	ESexType eSexType;						///< 性别，白名单报警无效
	EIdType eIdType;						///< 证件类型，白名单报警无效
	s8 szIdNumber[NET_MAXLEN_32 + 1];		///< 证件号码，白名单报警无效
	TNetDate tBirthDate;					///< 出生日期，白名单报警无效
	s8 szAddr[NET_MAXLEN_64 + 1];			///< 联系地址，白名单报警无效
	s8 szPhone[NET_MAXLEN_32 + 1];			///< 联系电话，白名单报警无效
	TNetCustomLabelList tCustomLabelList;	///< 自定义标签列表，白名单报警无效
	s8 szOperator[NET_MAXLEN_64 + 1];		///< 操作人，白名单报警无效
	s8 szAddTime[NET_MAXLEN_32];			///< 入库时间(格式为:2016-03-15T12:48:01.330)，白名单报警无效
	TNetPicData tSmallPic;					///< 小图
	TNetPicData tCtrlLicPic;				///< 布控库中的图片，白名单报警无效
}NETPACKED;

struct TNetCmpAlarmList
{
	s32 nNum;														///< 比较报警数量
	TNetCmpAlarmItem atCmpAlarmItem[NET_CMP_ALARM_LIST_MAX_NUM];	///< 比对报警信息
}NETPACKED;


#ifdef WIN32
#pragma pack(pop)
#endif


/**
 * 接口定义
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
 * @brief  初始化kdnetsdk
 * @param  无
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_Init();

/**
 * @brief  释放kdnetsdk资源
 * @param  无
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_Cleanup();

/**
 * @brief  获取设备能力
 * @param  dwNvrIp  [in]NVR IP，网络序
 *         wPort    [in]端口，网络序
 *         ptNvrCap [out]设备能力
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNvrCap(u32 dwNvrIp, u16 wPort, TNetNvrCap *ptNvrCap);

/**
 * @brief  获取gui语言信息，（仅供gui使用）
 * @param  dwNvrIp       [in]NVR IP，网络序
 *         wPort         [in]端口，网络序
 *         ptGuiLangInfo [out]gui语言信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetGuiLangInfo(u32 dwNvrIp, u16 wPort, TNetGuiLangInfo *ptGuiLangInfo);

/**
 * @brief  获取轻量用户列表，（仅供gui使用）
 * @param  dwNvrIp    [in]NVR IP，网络序
 *         wPort      [in]端口，网络序
 *         ptUserList [out]用户列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetLightUserList(u32 dwNvrIp, u16 wPort, TNetUserList *ptUserList);

/**
 * @brief  激活
 * @param  tLoginInfo [in]激活信息
 *         pdwHandle  [out]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_Active(TNetLoginInfo tLoginInfo, u32 *pdwHandle);

/**
 * @brief  登录
 * @param  tLoginInfo [in]登录信息
 *         pdwHandle  [out]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_Login(TNetLoginInfo tLoginInfo, u32 *pdwHandle);

/**
 * @brief  登录
 * @param  tLoginInfo     [in]登录信息
 *         pdwHandle      [out]句柄
 *         ptLoginErrInfo [out]扩展回复信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_LoginEx(TNetLoginInfo tLoginInfo, u32 *pdwHandle, TNetLoginErrInfo *ptLoginErrInfo);

/**
 * @brief  注销
 * @param  dwHandle  [in]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_Logout(u32 dwHandle);

/**
 * @brief  重启
 * @param  dwHandle  [in]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_Reboot(u32 dwHandle);

/**
 * @brief  关机
 * @param  dwHandle  [in]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_Shutdown(u32 dwHandle);

/**
 * @brief  校验用户信息
 * @param  dwHandle   [in]句柄
 *         tCheckInfo [in]校验信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CheckUserInfo(u32 dwHandle, TNetCheckInfo tCheckInfo);

/**
 * @brief  设置登录端口
 * @param  dwHandle [in]句柄
 *         wPort    [in]端口，网络序
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetLoginPort(u32 dwHandle, u16 wPort);

/**
 * @brief  注册nvr状态通知
 * @param  dwHandle [in]句柄
 *         pfFun    [in]nvr状态通知回调函数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_RegNvrStateNty(u32 dwHandle, pfNvrStateCallBack pfFun);

/**
 * @brief  注销nvr状态通知
 * @param  dwHandle [in]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_UnregNvrStateNty(u32 dwHandle);

/**
 * @brief  注册消息通知
 * @param  pfFun [in]消息通知回调函数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_RegMsgNty(pfMsgCallBack pfFun);

/**
 * @brief  订阅消息
 * @param  dwHandle          [in]句柄
 *         tSubscribeMsgList [in]订阅消息列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SubscribeMsg(u32 dwHandle, TNetSubscribeMsgList tSubscribeMsgList);

/**
 * @brief  设置心跳间隔
 * @param  dwHandle [in]句柄
 *         nValue   [in]心跳间隔，范围(1~30)秒
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetHeartbeatInterval(u32 dwHandle, s32 nValue);

/**
 * @brief  获取实时日志起始时间戳
 * @param  dwHandle          [in]句柄
 *         pdwStartTimeStamp [out]起始时间戳
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRealLogTimeStamp(u32 dwHandle, u32 *pdwStartTimeStamp);

/**
 * @brief  设置实时日志起始时间戳
 * @param  dwHandle    [in]句柄
 *         dwTimeStamp [in]时间戳
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetRealLogStartTimeStamp(u32 dwHandle, u32 dwTimeStamp);

/**
 * @brief  获取协议列表
 * @param  dwHandle    [in]句柄
 *         ptProtoList [out]协议列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetProtoList(u32 dwHandle, TNetProtoList *ptProtoList);

/**
 * @brief  创建搜索设备任务
 * @param  dwHandle        [in]句柄
 *         dwProtoMask     [in]搜索指定协议类型的设备，EProtoType组合值
 *         ptSearchDevTask [out]搜索任务信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreateSearchDevTask(u32 dwHandle, u32 dwProtoMask, TNetSearchDevTask *ptSearchDevTask);

/**
 * @brief  创建搜索设备任务，支持更多搜索条件
 * @param  dwHandle        [in]句柄
 *         tSearchDevParam [in]搜索条件
 *         ptSearchDevTask [out]搜索任务信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreateSearchDevTaskEx(u32 dwHandle, TNetSearchDevParam tSearchDevParam, TNetSearchDevTask *ptSearchDevTask);

/**
 * @brief  销毁搜索设备任务
 * @param  dwHandle [in]句柄
 *         dwTaskId [in]搜索任务id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroySearchDevTask(u32 dwHandle, u32 dwTaskId);

/**
 * @brief  获取搜索到的设备
 * @param  dwHandle          [in]句柄
 *         dwTaskId          [in]搜索任务id
 *         ptSearchedDevList [out]搜索到的设备列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSearchedDevList(u32 dwHandle, u32 dwTaskId, TNetSearchedDevList *ptSearchedDevList);

/**
 * @brief  获取nvr通道列表，一次最多可获取NET_PER_GET_CHNLIST_MAX_NUM
 * @param  dwHandle       [in]句柄
 *         tGetNvrChnList [in]指定的获取参数
 *         ptNvrChnList   [out]获取结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNvrChnList(u32 dwHandle, TNetGetNvrChnList tGetNvrChnList, TNetNvrChnList *ptNvrChnList);

/**
 * @brief  获取nvr合成通道列表，最多可获取NET_COMPOSITE_CHN_MAX_NUM
 * @param  dwHandle               [in]句柄
 *         ptNvrChnCompositeCap   [out]获取结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNvrCompositeChnCap(u32 dwHandle, TNetNvrCompositeChnCap *ptNvrChnCompositeCap);

/**
 * @brief  获取通道配置
 * @param  dwHandle       [in]句柄
 *         ptNvrChnCfg    [out]获取结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnCfg(u32 dwHandle, TNetChnCfg *ptNvrChnCfg);

/**
 * @brief  设置通道配置
 * @param  dwHandle       [in]句柄
 *         dwChnId        [in]通道id
 *         pszChnCfgInfo  [in]通道配置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetChnCfg(u32 dwHandle, s32 nChnId, const s8 *pszChnCfgInfo);

/**
 * @brief  添加设备，一次最多可添加NET_PER_ADD_DEV_MAX_NUM
 * @param  dwHandle    [in]句柄
 *         tAddDevList [in]设备列表
 *         ptChnCfg    [out]设备信息, 国标设备时生效, 值为NULL时表示不关注
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_AddDev(u32 dwHandle, TNetAddDevList tAddDevList, TNetChnCfg *ptChnCfg);

/**
 * @brief  添加GB28181设备
 * @param  dwHandle      [in]句柄
 *         pszChnCfgInfo [in]国标设备信息
 *         ptChnCfg      [out]设备信息, 国标设备时生效, 值为NULL时表示不关注
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_AddGB28181Dev(u32 dwHandle, const s8 *pszChnCfgInfo, TNetChnCfg *ptChnCfg);

/**
 * @brief  删除设备
 * @param  dwHandle [in]句柄
 *         dwChnId  [in]通道id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DelDev(u32 dwHandle, s32 nChnId);

/**
 * @brief  删除设备
 * @param  dwHandle    [in]句柄
 *         tDelDevList [in]删除设备列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DelDevEx(u32 dwHandle, TNetDelDevList tDelDevList);

/**
 * @brief  创建前端批量激活任务
 * @param  dwHandle                  [in]句柄
 *         tCreateDevActiveTaskInfo  [in]激活设备信息
 *         pnTaskId                  [out]激活任务任务ID
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreateDevActiveTask(u32 dwHandle, TNetCreateDevActiveTaskInfo tCreateDevActiveTaskInfo, s32 *pnTaskId);

/**
 * @brief  销毁前端批量激活任务
 * @param  dwHandle                  [in]句柄
 *         nTaskId                   [in]激活任务ID
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroyDevActiveTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  添加前端到批量激活任务
 * @param  dwHandle                  [in]句柄
 *         tAddDevActiveInfo         [in]要添加的前端信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_AddDevToActiveTask(u32 dwHandle, TNetAddDevActiveInfo tAddDevActiveInfo);

/**
 * @brief  查询前端批量激活任务状态
 * @param  dwHandle                  [in]句柄
 *         tQueryDevActiveTaskState  [in]查询相关信息
 *         ptDevActiveTaskState      [out]激活状态信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_QueryDevActiveState(u32 dwHandle, TNetQueryDevActiveTaskState tQueryDevActiveTaskState, TNetDevActiveTaskState *ptDevActiveTaskState);

/**
 * @brief  修改搜索到的前端网络参数
 * @param  dwHandle                  [in]句柄
 *         tModifyDevIpInfo          [in]修改信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_ModifyDevIp(u32 dwHandle, TNetModifyDevIpInfo tModifyDevIpInfo);

/**
 * @brief  创建前端批量升级任务
 * @param  dwHandle                  [in]句柄
 *         tCreateDevUpgradeTaskInfo [in]升级设备及升级包信息
 *         pnTaskId                  [out]升级任务ID
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreateDevUpgradeTask(u32 dwHandle, TNetCreateDevUpgradeTaskInfo tCreateDevUpgradeTaskInfo, s32 *pnTaskId);

/**
 * @brief  销毁前端批量升级任务
 * @param  dwHandle                  [in]句柄
 *         nTaskId                   [in]升级任务ID
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroyDevUpgradeTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  查询前端批量升级任务状态
 * @param  dwHandle                  [in]句柄
 *         nTaskId                   [in]升级任务ID
 *         ptDevUpgradeStateList     [out]升级状态信息列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_QueryDevUpgradeState(u32 dwHandle, s32 nTaskId, TNetDevUpgradeStateList *ptDevUpgradeStateList);

/**
 * @brief  上传前端批量升级包
 * @param  dwHandle                  [in]句柄
 *         nTaskId                   [in]升级任务ID
 *         tDevUpgradePkgData        [in]升级包数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_UploadDevUpgradePkg(u32 dwHandle, s32 nTaskId, TNetDevUpgradePkgData tDevUpgradePkgData);

/**
 * @brief  获取通道别名
 * @param  dwHandle         [in]句柄
 *         nChnIdStart      [in]起始通道id
 *         nChnIdEnd        [in]结束通道id
 *         ptChnAliasList   [out]获取的结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnAlias(u32 dwHandle, s32 nChnIdStart, s32 nChnIdEnd, TNetChnAliasList *ptChnAliasList);

/**
 * @brief  设置通道别名
 * @param  dwHandle       [in]句柄
 *         tChnAliasList  [in]通道别名列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetChnAlias(u32 dwHandle, TNetChnAliasList tChnAliasList);

/**
 * @brief  设置单个通道别名
 * @param  dwHandle       [in]句柄
 *         nChnId         [in]通道id
 *         pszChnAlias    [in]通道别名
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetChnAliasEx(u32 dwHandle, s32 nChnId,  const char* pszChnAlias);

/**
 * @brief  获取通道分组列表
 * @param  dwHandle         [in]句柄
 *         dwStartId        [in]通道组起始id，从1开始
 *         dwEndId          [in]通道组结束id，最大NET_CHNGROUP_MAX_NUM
 *         ptChnGroupList   [out]获取结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnGroupList(u32 dwHandle, s32 nStartId, s32 nEndId, TNetChnGroupList *ptChnGroupList);

/**
 * @brief  设置通道分组列表
 * @param  dwHandle         [in]句柄
 *         tChnGroupList    [in]通道组信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetChnGroupList(u32 dwHandle, TNetChnGroupList tChnGroupList);

/**
 * @brief  获取通道时间同步配置
 * @param  dwHandle            [in]句柄
 *         ptTimeSyncCfg       [out]获取结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetTimeSyncCfg(u32 dwHandle, TNetTimeSyncCfg *ptTimeSyncCfg);

/**
 * @brief  设置通道时间同步配置
 * @param  dwHandle            [in]句柄
 *         tTimeSyncCfg        [in]设置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetTimeSyncCfg(u32 dwHandle, TNetTimeSyncCfg tTimeSyncCfg);

/**
 * @brief  获取前端OSD配置
 * @param  dwHandle            [in]句柄
 *         nChnId              [in]通道id
 *         ptDevOsdCfg         [out]获取结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDevOsdCfg(u32 dwHandle, s32 nChnId, TNetDevOsdCfg *ptDevOsdCfg);

/**
 * @brief  获取前端OSD配置
 * @param  dwHandle            [in]句柄
 *         nChnId              [in]通道id
 *         ptDevOsdCfg         [in]配置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetDevOsdCfg(u32 dwHandle, s32 nChnId, TNetDevOsdCfg tDevOsdCfg);

/**
 * @brief  上传前端OSD图片
 * @param  dwHandle            [in]句柄
 *         nChnId              [in]通道id
 *         tOsdPicList         [in]图片信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_UploadDevOsdPic(u32 dwHandle, s32 nChnId, TNetOsdPicFileList tOsdPicList);

/**
 * @brief  获取通道视频编码配置
 * @param  dwHandle            [in]句柄
 *         ptChnVideoEncParam  [out]获取结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnVideoEnc(u32 dwHandle, TNetChnVideoEncParam *ptChnVideoEncParam);

/**
 * @brief  获取通道视频编码配置(异步)
 * @param  dwHandle   [in]句柄
 *         nChnId     [in]通道id
 *         pfCallBack [in]回调函数
 *         pParam     [in]回调函数参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnVideoEncAsync(u32 dwHandle, s32 nChnId, pfAPICallBack pfCallBack, void *pParam);

/**
 * @brief  设置通道视频编码配置
 * @param  dwHandle            [in]句柄
 *         tChnVideoEncParam   [in]设置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetChnVideoEnc(u32 dwHandle, TNetChnVideoEncParam tChnVideoEncParam);

/**
 * @brief  获取通道音频编码配置
 * @param  dwHandle            [in]句柄
 *         ptChnAudioEncParam  [out]获取结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnAudioEnc(u32 dwHandle, TNetChnAudioEncParam *ptChnAudioEncParam);

/**
 * @brief  获取通道音频编码配置(异步)
 * @param  dwHandle   [in]句柄
 *         nChnId     [in]通道id
 *         pfCallBack [in]回调函数
 *         pParam     [in]回调函数参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnAudioEncAsync(u32 dwHandle, s32 nChnId, pfAPICallBack pfCallBack, void *pParam);

/**
 * @brief  设置通道音频编码配置
 * @param  dwHandle            [in]句柄
 *         tChnAudioEncParam   [in]设置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetChnAudioEnc(u32 dwHandle, TNetChnAudioEncParam tChnAudioEncParam);

/**
 * @brief  获取通道音频解码配置
 * @param  dwHandle            [in]句柄
 *         ptChnAudioDecParam  [out]获取结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnAudioDec(u32 dwHandle, TNetChnAudioDecParam *ptChnAudioDecParam);

/**
 * @brief  设置通道音频解码配置
 * @param  dwHandle            [in]句柄
 *         tChnAudioDecParam   [in]设置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetChnAudioDec(u32 dwHandle, TNetChnAudioDecParam tChnAudioDecParam);

/**
 * @brief  通道字符串标识转换为通道id
 * @param  dwHandle  [in]句柄
 *         pszChnStr [in]通道字符串标识
 *         pnChnId   [out]通道id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_ChnStr2ChnId(u32 dwHandle, const s8 *pszChnStr, s32 *pnChnId);

/**
 * @brief  获取警戒线配置
 * @param  dwHandle    [in]句柄
 *         nChnId      [in]通道id
 *         nSchemeId   [in]方案id，从1开始，可以为0，0：表示获取当前方案
 *         ptCordonCfg [out]警戒线配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCordonCfg(u32 dwHandle, s32 nChnId, s32 nSchemeId, TNetCordonCfg *ptCordonCfg);

/**
 * @brief  设置警戒线配置
 * @param  dwHandle   [in]句柄
 *         nChnId     [in]通道id
 *         tCordonCfg [in]警戒线配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetCordonCfg(u32 dwHandle, s32 nChnId, TNetCordonCfg tCordonCfg);

/**
 * @brief  获取区域配置
 * @param  dwHandle        [in]句柄
 *         nChnId          [in]通道id
 *         nSchemeId       [in]方案id，从1开始，可以为0，0：表示获取当前方案
 *         eSmartAlarmType [in]基础智能类型，支持区域进入、区域离开、区域入侵、物品拿取、物品遗留
 *         ptAreaCfg       [out]区域配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAreaCfg(u32 dwHandle, s32 nChnId, s32 nSchemeId, ESmartAlarmType eSmartAlarmType, TNetAreaCfg *ptAreaCfg);

/**
 * @brief  设置区域配置
 * @param  dwHandle        [in]句柄
 *         nChnId          [in]通道id
 *         eSmartAlarmType [in]基础智能类型，支持区域进入、区域离开、区域入侵、物品拿取、物品遗留
 *         tAreaCfg        [in]区域配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetAreaCfg(u32 dwHandle, s32 nChnId, ESmartAlarmType eSmartAlarmType, TNetAreaCfg tAreaCfg);

/**
 * @brief  获取虚焦检测配置
 * @param  dwHandle            [in]句柄
 *         nChnId              [in]通道id
 *         ptVirFocusDetectCfg [out]虚焦检测配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetVirFocusDetectCfg(u32 dwHandle, s32 nChnId, TNetVirFocusDetectCfg *ptVirFocusDetectCfg);

/**
 * @brief  设置虚焦检测配置
 * @param  dwHandle           [in]句柄
 *         nChnId             [in]通道id
 *         tVirFocusDetectCfg [in]虚焦检测配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetVirFocusDetectCfg(u32 dwHandle, s32 nChnId, TNetVirFocusDetectCfg tVirFocusDetectCfg);

/**
 * @brief  获取场景变更配置
 * @param  dwHandle         [in]句柄
 *         nChnId           [in]通道id
 *         ptSceneChangeCfg [out]场景变更配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSceneChangeCfg(u32 dwHandle, s32 nChnId, TNetSceneChangeCfg *ptSceneChangeCfg);

/**
 * @brief  设置场景变更配置
 * @param  dwHandle        [in]句柄
 *         nChnId          [in]通道id
 *         tSceneChangeCfg [in]场景变更配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetSceneChangeCfg(u32 dwHandle, s32 nChnId, TNetSceneChangeCfg tSceneChangeCfg);

/**
 * @brief  获取人员聚集配置
 * @param  dwHandle          [in]句柄
 *         nChnId            [in]通道id
 *         nSchemeId         [in]方案id，从1开始，可以为0，0：表示获取当前方案
 *         ptPeopleGatherCfg [out]人员聚集配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetPeopleGatherCfg(u32 dwHandle, s32 nChnId, s32 nSchemeId, TNetPeopleGatherCfg *ptPeopleGatherCfg);

/**
 * @brief  设置人员聚集配置
 * @param  dwHandle         [in]句柄
 *         nChnId           [in]通道id
 *         tPeopleGatherCfg [in]人员聚集配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetPeopleGatherCfg(u32 dwHandle, s32 nChnId, TNetPeopleGatherCfg tPeopleGatherCfg);

/**
 * @brief  获取声音异常配置
 * @param  dwHandle           [in]句柄
 *         nChnId             [in]通道id
 *         ptAudioAbnormalCfg [out]声音异常配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAudioAbnormalCfg(u32 dwHandle, s32 nChnId, TNetAudioAbnormalCfg *ptAudioAbnormalCfg);

/**
 * @brief  设置声音异常配置
 * @param  dwHandle          [in]句柄
 *         nChnId            [in]通道id
 *         tAudioAbnormalCfg [in]声音异常配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetAudioAbnormalCfg(u32 dwHandle, s32 nChnId, TNetAudioAbnormalCfg tAudioAbnormalCfg);

/**
 * @brief  获取音量波形图实时数值
 * @param  dwHandle           [in]句柄
 *         nChnId             [in]通道id
 *         ptAudioAbnormalCfg [out]声音异常配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAudioMeter(u32 dwHandle, s32 nChnId, TNetAudioMeter *ptAudioMeter);

/**
 * @brief  获取人脸检测配置
 * @param  dwHandle        [in]句柄
 *         nChnId          [in]通道id
 *         ptFaceDetectCfg [out]人脸检测配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetFaceDetectCfg(u32 dwHandle, s32 nChnId, TNetFaceDetectCfg *ptFaceDetectCfg);

/**
 * @brief  设置人脸检测配置
 * @param  dwHandle       [in]句柄
 *         nChnId         [in]通道id
 *         tFaceDetectCfg [in]人脸检测配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetFaceDetectCfg(u32 dwHandle, s32 nChnId, TNetFaceDetectCfg tFaceDetectCfg);

/**
 * @brief  获取防拆报警配置
 * @param  dwHandle				[in]句柄
 *         nChnId				[in]通道id
 *         ptAccessProtectCfg	[out]防拆报警配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAccessProtectCfg(u32 dwHandle, s32 nChnId, TNetAccessProtectCfg *ptAccessProtectCfg);

/**
 * @brief  设置防拆报警配置
 * @param  dwHandle       [in]句柄
 *         nChnId         [in]通道id
 *         tFaceDetectCfg [in]人脸检测配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetAccessProtectCfg(u32 dwHandle, s32 nChnId, TNetAccessProtectCfg tAccessProtectCfg);

/**
 * @brief  获取录像管理模块能力
 * @param  dwHandle            [in]句柄
 *         ptRecMgrCap         [out]获取结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRecMgrCap(u32 dwHandle, TNetRecMgrCap *ptRecMgrCap);

/**
 * @brief  获取录像事件类型
 * @param  dwHandle          [in]句柄
 *         pdwRecEventMask   [out]支持的录像事件类型掩码，ERecEventType组合值
 *  	   pdwInteDetectMask [out]支持的智能侦测类型掩码，ERecEventType组合值
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRecEventType(u32 dwHandle, u32 *pdwRecEventMask, u32 *pdwInteDetectMask);

/**
 * @brief  获取抓拍图片事件类型
 * @param  dwHandle            [in]句柄
 *         pdwSnapPicEventMask [out]支持的抓拍图片事件类型掩码，ESnapPicEventType组合值
 *  	   pdwInteDetectMask   [out]支持的智能侦测类型掩码，ESnapPicEventType组合值
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSnapPicEventType(u32 dwHandle, u32 *pdwSnapPicEventMask, u32 *pdwInteDetectMask);

/**
 * @brief  获取通道录像计划
 * @param  dwHandle            [in]句柄
 *         ptChnRecPlan        [in/out]传入id，传出结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnRecPlan(u32 dwHandle, TNetChnRecPlan *ptChnRecPlan);

/**
 * @brief  设置通道录像计划
 * @param  dwHandle            [in]句柄
 *         tChnRecPlan         [in]传入数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetChnRecPlan(u32 dwHandle, TNetChnRecPlan tChnRecPlan);

/**
 * @brief  拷贝通道录像计划
 * @param  dwHandle            [in]句柄
 *         tCopyChnRecPlan     [in]传入数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CopyChnRecPlan(u32 dwHandle, TNetCopyChnRecPlan tCopyChnRecPlan);

/**
 * @brief  获取通道抓拍计划
 * @param  dwHandle            [in]句柄
 *         ptChnSnapPlan       [in/out]传入id，传出结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnSnapPlan(u32 dwHandle, TNetChnSnapPlan *ptChnSnapPlan);

/**
 * @brief  设置通道抓拍计划
 * @param  dwHandle            [in]句柄
 *         tChnSnapPlan        [in]传入数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetChnSnapPlan(u32 dwHandle, TNetChnSnapPlan tChnSnapPlan);

/**
 * @brief  拷贝通道抓拍计划
 * @param  dwHandle            [in]句柄
 *         tCopyChnSnapPlan    [in]传入数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CopyChnSnapPlan(u32 dwHandle, TNetCopyChnSnapPlan tCopyChnSnapPlan);

/**
 * @brief  获取录像高级配置
 * @param  dwHandle            [in]句柄
 *         ptRecAdvanceOption  [out]传出数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRecAdvanceOption(u32 dwHandle, TNetRecAdvanceOption *ptRecAdvanceOption);

/**
 * @brief  设置录像高级配置
 * @param  dwHandle            [in]句柄
 *         ptRecAdvanceOption  [in]传入数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetRecAdvanceOption(u32 dwHandle, TNetRecAdvanceOption tRecAdvanceOption);

/**
 * @brief  获取假日配置
 * @param  dwHandle            [in]句柄
 *         ptHolidayCfgList    [out]传出数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetHolidayCfg(u32 dwHandle, TNetHolidayCfgList *ptHolidayCfgList);

/**
 * @brief  设置假日配置
 * @param  dwHandle            [in]句柄
 *         tHolidayCfgLis      [in]传入数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetHolidayCfg(u32 dwHandle, TNetHolidayCfgList tHolidayCfgLis);

/**
 * @brief  创建录像备份任务
 * @param  dwHandle            [in]句柄
 *         tRecBackupData      [in]传入数据
 *         pnTaskId            [out]创建的任务ID
 *         pullDataSize        [out]本次传入的数据大小
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreateRecBackupTask(u32 dwHandle, TNetRecBackupData tRecBackupData, s32 *pnTaskId, u64 *pullDataSize);

/**
 * @brief  添加录像备份数据
 * @param  dwHandle            [in]句柄
 *         nTaskId             [in]任务ID
 *         tRecBackupData      [in]传入数据
 *         pullDataSize        [out]本次传入的数据大小
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_AddRecBackupData(u32 dwHandle, s32 nTaskId, TNetRecBackupData tRecBackupData, u64 *pullDataSize);

/**
 * @brief  开启录像备份任务
 * @param  dwHandle            [in]句柄
 *         nTaskId             [in]任务ID
 *         eRecBackupType      [in]任务类型
 *         pszBackupPath       [in]备份路径
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StartRecBackupTask(u32 dwHandle, s32 nTaskId, ERecBackupType eRecBackupType, s8 *pszBackupPath);

/**
 * @brief  查询录像备份状态
 * @param  dwHandle            [in]句柄
 *         nTaskId             [in]任务ID
 *         tRecBackupData      [in]查询数据
 *         ptRecBackupState    [out]反馈结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_QueryRecBackupState(u32 dwHandle, s32 nTaskId, TNetRecBackupData tRecBackupData, TNetRecBackupState *ptRecBackupState);

/**
 * @brief  销毁录像备份任务
 * @param  dwHandle            [in]句柄
 *         nTaskId             [in]任务ID
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroyRecBackupTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  web下载开始
 * @param  dwHandle            [in]句柄
 * @param  wPort			   [in]下载端口
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_WebRecDownloadStart(u32 dwHandle, u16 wPort);

/**
 * @brief  web下载保活
 * @param  dwHandle            [in]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_WebRecDownloadKeep(u32 dwHandle);

/**
 * @brief  web下载结束
 * @param  dwHandle            [in]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_WebRecDownloadStop(u32 dwHandle);

/**
 * @brief  锁定录像前获取录像信息
 * @param  dwHandle            [in]句柄
 *         tLockRecList        [in]传入数据
 *         ptLockRecData       [out]传出录像信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRecInfoBeforeLock(u32 dwHandle, TNetLockRecList tLockRecList, TNetLockRecData *ptLockRecData);

/**
 * @brief  锁定录像
 * @param  dwHandle            [in]句柄
 *         tLockRecList        [in]传入数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_LockRec(u32 dwHandle, TNetLockRecList tLockRecList);

/**
 * @brief  解锁录像
 * @param  dwHandle            [in]句柄
 *         tLockRecList        [in]传入数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_UnLockRec(u32 dwHandle, TNetLockRecList tLockRecList);

/**
 * @brief  创建图片备份任务
 * @param  dwHandle            [in]句柄
 *         ptPicBackupData     [in]传入数据
 *         pnTaskId            [out]创建的任务ID
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreatePicBackupTask(u32 dwHandle, TNetPicBackupData *ptPicBackupData, s32 *pnTaskId);

/**
 * @brief  添加图片备份数据
 * @param  dwHandle            [in]句柄
 *         nTaskId             [in]任务ID
 *         ptPicBackupData     [in]传入数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_AddPicBackupData(u32 dwHandle, s32 nTaskId, TNetPicBackupData *ptPicBackupData);

/**
 * @brief  开启图片备份任务
 * @param  dwHandle            [in]句柄
 *         nTaskId             [in]任务ID
 *         pszBackupPath       [in]备份路径
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StartPicBackupTask(u32 dwHandle, s32 nTaskId, s8 *pszBackupPath);

/**
 * @brief  查询图片备份状态
 * @param  dwHandle            [in]句柄
 *         nTaskId             [in]任务ID
 *         ptPicBackupData     [in]查询数据
 *         ptRecBackupState    [out]反馈结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_QueryPicBackupState(u32 dwHandle, s32 nTaskId, TNetPicBackupData *ptPicBackupData, TNetPicBackupState *ptPicBackupState);

/**
 * @brief  销毁图片备份任务
 * @param  dwHandle            [in]句柄
 *         nTaskId             [in]任务ID
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroyPicBackupTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  获取通道录像月视图
 * @param  dwHandle            [in]句柄
 *         tChnMonthRec        [in]获取参数
 *         pdwMonthRec         [out]传出结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnMonthRec(u32 dwHandle, TNetGetChnMonthRec tGetChnMonthRec, TNetChnMonthRecList *ptChnMonthRecList);

/**
 * @brief  创建查询录像记录（事件）任务
 * @param  dwHandle                   [in]句柄
 *         tCreateQueryRecTask        [in]获取参数
 *         pnTaskId                   [out]传出结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreatQueryRecTask(u32 dwHandle, TNetCreatQueryRecTask tCreateQueryRecTask, s32 *pnTaskId);

/**
 * @brief  查询录像记录结果
 * @param  dwHandle                      [in]句柄
 *         tGetQueryRecTaskResult        [in]获取参数
 *         tChnRecList                   [out]传出结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRecTaskResult(u32 dwHandle, TNetGetRecTaskResult tGetRecTaskResult, TNetChnRecList *ptChnRecList);

/**
 * @brief  销毁查询录像记录（事件）任务
 * @param  dwHandle       [in]句柄
 *         nTaskId        [in]任务id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroyQueryRecTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  创建回放任务
 * @param  dwHandle       [in]句柄
 *         ePlaybackType  [in]回放类型
 *         pnTaskId       [out]任务id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreatePlaybackTask(u32 dwHandle, EPlaybackType ePlaybackType, s32 *pnTaskId);

/**
 * @brief  开始回放
 * @param  dwHandle        [in]句柄
 *         tPlaybackList   [in]回放信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StartPlayback(u32 dwHandle, EOutputType eOutputType, s32 nTaskId, TNetPlaybackList tPlaybackList);

/**
 * @brief  停止回放
 * @param  dwHandle        [in]句柄
 *         tPlaybackList   [in]回放信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StopPlayback(u32 dwHandle, EOutputType eOutputType, s32 nTaskId, TNetPlaybackList tPlaybackList);

/**
 * @brief  向回显回放流任务添加通道
 * @param  dwHandle        [in]句柄
 *         nTaskId         [in]回放任务ID
 *         tPlaybackList   [in]回放信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_AddPlaybackChn(u32 dwHandle, s32 nTaskId, TNetPlaybackList tPlaybackList);

/**
 * @brief  从回显回放流任务删除通道
 * @param  dwHandle        [in]句柄
 *         nTaskId         [in]回放任务ID
 *         tPlaybackList   [in]回放信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DelPlaybackChn(u32 dwHandle, s32 nTaskId, TNetPlaybackList tPlaybackList);

/**
 * @brief  获取录像文件信息（mp4，asf）
 * @param  dwHandle        [in]句柄
 *         pszUri          [in]文件路径
 *         ptRecFileInfo   [out]文件信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRecFileInfo(u32 dwHandle, s8 *pszUri, TNetRecFileInfo *ptRecFileInfo);

/**
 * @brief  回放Vcr控制
 * @param  dwHandle           [in]句柄
 *         tPlaybackVcrCtrl   [in]设置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_PlaybackVcrCtrl(u32 dwHandle, TNetPlaybackVcrCtrl tPlaybackVcrCtrl);

/**
 * @brief  回放进度查询
 * @param  dwHandle        [in]句柄
 *         nTaskId         [in]任务id
 *         nChnId          [in]通道id
 *         ptPlaybackState [out]回放状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_QueryPlaybackProgress(u32 dwHandle, s32 nTaskId, s32 nChnId, TNetPlaybackState *ptPlaybackState);

/**
 * @brief  设置回放同步属性
 * @param  dwHandle       [in]句柄
 *         nTaskId        [in]任务id
 *         bSync          [in]是否同步
 *         nRefChnID      [in]参考通道id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetPlaybackSyncProp(u32 dwHandle, s32 nTaskId, BOOL32 bSync, s32 nRefChnID);

/**
 * @brief  销毁回放流任务
 * @param  dwHandle       [in]句柄
 *         nTaskId        [in]任务id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroyPlaybackTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  开始事件循环回放
 * @param  dwHandle                       [in]句柄
 *         tEventCyclePlaybackInfo        [in]事件循环相关信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StartEventCyclePlayback(u32 dwHandle, TNetEventCyclePlaybackInfo tEventCyclePlaybackInfo);

/**
 * @brief  停止事件循环回放
 * @param  dwHandle       [in]句柄
 *         nTaskId        [in]任务id
 *         nChnId         [in]通道id(同步方式该参数无效)
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StopEventCyclePlayback(u32 dwHandle, s32 nTaskId, s32 nChnId);

/**
 * @brief  查询抓拍图片
 * @param  dwHandle                 [in]句柄
 *         tQuerySnapPicInfo        [in]要查询的通道信息
 *         ptSnapPicList            [out]返回图片信息列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_QuerySnapPic(u32 dwHandle, TNetQuerySnapPicInfo tQuerySnapPicInfo, TNetSnapPicList *ptSnapPicList);

/**
 * @brief  获取抓拍图片
 * @param  dwHandle			[in]句柄
 *         tGetSnapPic		[in]图片参数
 *         ptGetPicData		[out]图片数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSnapPic(u32 dwHandle, TNetGetSnapPic tGetSnapPic, TNetPicData *ptGetPicData);

/**
 * @brief  查询历史标签
 * @param  dwHandle                 [in]句柄
 *         ptSnapPicList            [out]返回标签信息列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_QueryHistroyRecTag(u32 dwHandle, TNetRecTagList *ptRecTagList);

/**
 * @brief  录像标签操作
 * @param  dwHandle                 [in]句柄
 *         tRecTagOperateParam      [in]标签操作参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_RecTagOperate(u32 dwHandle, TNetRecTagOperateParam tRecTagOperateParam);

/**
 * @brief  创建录像标签查询任务
 * @param  dwHandle                         [in]句柄
 *         tCreateQueryRecTagTaskParam      [in]创建任务参数
 *         pnTaskId                         [out]查询任务ID
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreateQueryRecTagTask(u32 dwHandle, TNetCreateQueryRecTagTaskParam tCreateQueryRecTagTaskParam, s32 *pnTaskId);

/**
 * @brief  获取录像标签查询结果
 * @param  dwHandle                         [in]句柄
 *         tQueryRecTagResultParam          [in]查询参数
 *         ptQueryRecTagResult              [out]录像标签查询结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetQueryRecTagResult(u32 dwHandle, TNetQueryRecTagResultParam tQueryRecTagResultParam, TNetQueryRecTagResult *ptQueryRecTagResult);

/**
 * @brief  销毁录像标签查询任务
 * @param  dwHandle                         [in]句柄
 *         nTaskId                          [in]任务ID
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroyQueryRecTagTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  创建回放任务
 * @param  dwHandle             [in]句柄
 *         ePlaybackType        [in]回放类型
 *		   bCreateRtspUrl		[in]是否生成 rtsp url
 *         tPlaybackDstChnList  [in]回放通道信息
 *         pnTaskId             [out]任务id
 *         ptPlaybackSrcChnList [out]回放通道信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreatePlaybackTaskEx(u32 dwHandle, EPlaybackType ePlaybackType, BOOL32 bCreateRtspUrl, TNetPlaybackDstChnList tPlaybackDstChnList, s32 *pnTaskId, TNetPlaybackSrcChnList *ptPlaybackSrcChnList);

/**
 * @brief  销毁回放任务
 * @param  dwHandle       [in]句柄
 *         nTaskId        [in]任务id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroyPlaybackTaskEx(u32 dwHandle, s32 nTaskId);

/**
 * @brief  获取用于拉取回放码流的rtsp url
 * @param  dwHandle		               [in]句柄
 *         tNetGetRtspPlaybackUrlParam   [in]请求参数
 *         ptNetRtspPlaybackUrl       [out]返回数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRtspPlaybackUrl(u32 dwHandle, TNetGetRtspPlaybackUrlParam tNetGetRtspPlaybackUrlParam, TNetRtspPlaybackUrl *ptNetRtspPlaybackUrl);

/**
 * @brief  开始回放
 * @param  dwHandle [in]句柄
 *         nTaskId  [in]任务id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StartPlaybackEx(u32 dwHandle, s32 nTaskId);

/**
 * @brief  停止回放
 * @param  dwHandle [in]句柄
 *         nTaskId  [in]任务id
 *         nChnId   [in]通道id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StopPlaybackEx(u32 dwHandle, s32 nTaskId, s32 nChnId);

/**
 * @brief  向回放任务添加通道
 * @param  dwHandle             [in]句柄
 *         nTaskId              [in]任务id
 *         tPlaybackDstChnList  [in]回放通道信息
 *         ptPlaybackSrcChnList [out]回放通道信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_AddPlaybackChnEx(u32 dwHandle, s32 nTaskId, TNetPlaybackDstChnList tPlaybackDstChnList, TNetPlaybackSrcChnList *ptPlaybackSrcChnList);

/**
 * @brief  从回放任务删除通道
 * @param  dwHandle [in]句柄
 *         nTaskId  [in]任务ID
 *         panChnId [in]通道数组
 *         nNum     [in]数组数量
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DelPlaybackChnEx(u32 dwHandle, s32 nTaskId, const s32 *panChnId, s32 nNum);

/**
 * @brief  获取硬盘管理能力
 * @param  dwHandle     [in]句柄
 *         ptDiskMgrCap [out]硬盘管理能力
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDiskMgrCap(u32 dwHandle, TNetDiskMgrCap *ptDiskMgrCap);

/**
 * @brief  获取硬盘分区用途信息
 * @param  dwHandle				[in]句柄
 *         ptDiskPartTypeList	[out]硬盘分区用途信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDiskPartUsingInfo(u32 dwHandle, TNetDiskPartTypeParam tDiskPartTypeParam, TNetDiskPartTypeList *ptDiskPartTypeList);

/**
 * @brief  获取硬盘列表
 * @param  dwHandle   [in]句柄
 *         ptDiskList [out]硬盘列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDiskList(u32 dwHandle, TNetDiskList *ptDiskList);

/**
 * @brief  搜索网络硬盘
 * @param  dwHandle      [in]句柄
 *         tNetDiskParam [in]搜索参数
 *         pdwTaskId     [out]搜索任务id，用于获取网络硬盘
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SearchNetDisk(u32 dwHandle, TNetNetDiskParam tNetDiskParam, u32 *pdwTaskId);

/**
 * @brief  获取网络硬盘
 * @param  dwHandle      [in]句柄
 *         dwTaskId      [in]搜索任务id
 *         ptNetDiskList [out]网络硬盘列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNetDisk(u32 dwHandle, u32 dwTaskId, TNetNetDiskList *ptNetDiskList);

/**
 * @brief  添加网络硬盘
 * @param  dwHandle      [in]句柄
 *         tNetDiskParam [in]添加参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_AddNetDisk(u32 dwHandle, TNetNetDiskParam tNetDiskParam);

/**
 * @brief  卸载硬盘，一次可卸载多个硬盘
 * @param  dwHandle  [in]句柄
 *         tDiskList [in]卸载硬盘列表，只需填写字段TNetDiskList::swDiskNum TNetDiskInfo::byDiskId
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_UnmountDisk(u32 dwHandle, TNetDiskList tDiskList);

/**
 * @brief  设置硬盘存储用途
 * @param  dwHandle				[in]句柄
 *         tDiskUsingTypeParam	[in]存储用途参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetDiskUsingType(u32 dwHandle, TNetDiskUsingTypeList tDiskUsingTypeList);

/**
 * @brief  初始化硬盘
 * @param  dwHandle				[in]句柄
 *         tNetDiskInitParam	[in]初始化参数
 *         pwTaskId				[out]返回初始化task id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_InitDisk(u32 dwHandle, TNetDiskInitParam tNetDiskInitParam, u16 *pwTaskId);

/**
 * @brief  初始化硬盘(r2b2及以前版本)
 * @param  dwHandle  [in]句柄
 *         byDeskId  [in]硬盘id
 *         pwTaskId  [out]返回初始化task id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_InitDiskEx(u32 dwHandle, u8 byDeskId, u16 *pwTaskId);

/**
 * @brief  初始化USB外接硬盘(格式化为NTFS)
 * @param  dwHandle     [in]句柄
 *         pszPartName  [in]需要格式化的分区名
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_InitUsbExtDisk(u32 dwHandle, s8 *pszPartName);

/**
 * @brief  设置硬盘读写属性
 * @param  dwHandle           [in]句柄
 *         tDiskAttributeList [in]硬盘属性信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_EditDiskAttribute(u32 dwHandle, TNetDiskAttributeList tDiskAttributeList);

/**
 * @brief  获取当前硬盘的管理模式
 * @param  dwHandle        [in]句柄
 *         peStoreMode     [out]传出当前生效硬盘的管理模式信息
 *         peCfgStoreMode  [out]传出配置硬盘的管理模式信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDiskStoreMode(u32 dwHandle, EStoreMode *peStoreMode, EStoreMode *peCfgStoreMode, u32 *pdwStoreModeCap);

/**
 * @brief  设置当前硬盘的管理模式
 * @param  dwHandle        [in]句柄
 *         eStoreMode      [in]要设置的银盘管理模式
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetDiskStoreMode(u32 dwHandle, EStoreMode eStoreMode);

/**
 * @brief  获取硬盘配额配置
 * @param  dwHandle        [in]句柄
 *         ptDiskQuotaCfg  [in/out]传入通道id，传出配额信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDiskQuotaCfg(u32 dwHandle, TNetDiskQuotaCfg *ptDiskQuotaCfg);

/**
 * @brief  设置硬盘配额配置
 * @param  dwHandle        [in]句柄
 *         tDiskQuotaCfg   [in]配额信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetDiskQuotaCfg(u32 dwHandle, TNetDiskQuotaCfg tDiskQuotaCfg);

/**
 * @brief  拷贝通道硬盘配额配置
 * @param  dwHandle                [in]句柄
 *         tChnDiskQuotaCopyInfo   [in]拷贝配额信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CopyChnDiskQuotaCfg(u32 dwHandle, TNetChnDiskQuotaCopyInfo tChnDiskQuotaCopyInfo);

/**
 * @brief  获取通道盘组配置
 * @param  dwHandle            [in]句柄
 *         ptChnDiskGroupList  [out]传出通道盘组配置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnDiskGroupCfg(u32 dwHandle, TNetChnDiskGroupList *ptChnDiskGroupList);

/**
 * @brief  设置通道盘组配置
 * @param  dwHandle            [in]句柄
 *         tChnDiskGroupList   [in]通道盘组配置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetChnDiskGroupCfg(u32 dwHandle, TNetChnDiskGroupList tChnDiskGroupList);

/**
 * @brief  获取硬盘高级配置
 * @param  dwHandle            [in]句柄
 *         ptDiskAdvanceCfg    [out]硬盘高级配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDiskAdvanceCfg(u32 dwHandle, TNetDiskAdvanceCfg *ptDiskAdvanceCfg);

/**
 * @brief  设置硬盘高级配置
 * @param  dwHandle            [in]句柄
 *         tDiskAdvanceCfg     [in]硬盘高级配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetDiskAdvanceCfg(u32 dwHandle, TNetDiskAdvanceCfg tDiskAdvanceCfg);

/**
 * @brief  创建RAID
 * @param  dwHandle         [in]句柄
 *         tCreateRaidParam [in]创建raid参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreateRaid(u32 dwHandle, TNetCreateRaidParam tCreateRaidParam);

/**
 * @brief  获取创建RAID状态
 * @param  dwHandle         [in]句柄
 * @return 创建完成返回NET_OK，创建中中返回ERR_NET_DM_DISK_RAID_CREATING，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCreateRaidStatus(u32 dwHandle);

/**
 * @brief  获取RAID信息
 * @param  dwHandle         [in]句柄
 *         tRaidInfo        [out]raid信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRaidInfo(u32 dwHandle, TNetRaidInfo *ptRaidInfo);

/**
 * @brief  添加热备盘
 * @param  dwHandle         [in]句柄
 *         nDiskId          [in]磁盘ID
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_AddHotBackupDisk(u32 dwHandle, s32 nDiskId);

/**
 * @brief  删除热备盘
 * @param  dwHandle         [in]句柄
 *         nDiskId          [in]磁盘ID
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DelHotBackupDisk(u32 dwHandle, s32 nDiskId);

/**
 * @brief  删除RAID
 * @param  dwHandle         [in]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DelRaid(u32 dwHandle);

/**
 * @brief  获取删除RAID状态
 * @param  dwHandle         [in]句柄
 * @return 删除完成返回NET_OK，删除中返回ERR_NET_DM_DISK_RAID_DELING，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDelRaidStatus(u32 dwHandle);

/**
 * @brief  恢复Raid
 * @param  dwHandle         [in]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_RecoveryRaid(u32 dwHandle);

/**
 * @brief  获取浏览回放相关能力
 * @param  dwNvrIp    [in]NVR IP，网络序
 *         wPort      [in]端口，网络序
 *         ptFrameCap [out]获取的相关能力
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetFrameCap(u32 dwNvrIp, u16 wPort, TNetFrameCap *ptFrameCap);

/**
 * @brief  浏览初始化
 * @param  dwNvrIp [in]NVR IP，网络序
 *         wPort   [in]端口，网络序
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_MonitorInit(u32 dwNvrIp, u16 wPort);

/**
 * @brief  获取绑定列表
 * @param  dwNvrIp     [in]NVR IP，网络序
 *         wPort       [in]端口，网络序
 *         eOutputType [in]显示类型
 *         ptBindList  [out]绑定列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetBindList(u32 dwNvrIp, u16 wPort, EOutputType eOutputType, TNetBindList *ptBindList);

/**
 * @brief  获取画面风格
 * @param  dwNvrIp     [in]NVR IP，网络序
 *         wPort       [in]端口，网络序
 *         eOutputType [in]显示类型
 *         ptGridList  [out]画面风格信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetFrameStyle(u32 dwNvrIp, u16 wPort, EOutputType eOutputType, TNetGridList *ptGridList);

/**
 * @brief  设置画面风格
 * @param  dwNvrIp     [in]NVR IP，网络序
 *         wPort       [in]端口，网络序
 *         eOutputType [in]显示类型
 *         tGridList   [in]画面风格信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetFrameStyle(u32 dwNvrIp, u16 wPort, EOutputType eOutputType, TNetGridList tGridList);

/**
 * @brief  绑定
 * @param  dwHandle    [in]句柄
 *         eOutputType [in]显示类型
 *         tBindList   [in]绑定列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_Bind(u32 dwHandle, EOutputType eOutputType, TNetBindList tBindList);

/**
 * @brief  交换绑定
 * @param  dwHandle    [in]句柄
 *         eOutputType [in]显示类型
 *         wGridId1    [in]交换格子1
 *         wGridId2    [in]交换格子2
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SwitchBind(u32 dwHandle, EOutputType eOutputType, u16 wGridId1, u16 wGridId2);

/**
 * @brief  解绑
 * @param  dwHandle    [in]句柄
 *         eOutputType [in]显示类型
 *         wGridId     [in]grid id
 *		   nChnId      [in]chn id
 *         bNotSave    [in]是否不保存
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_Unbind(u32 dwHandle, EOutputType eOutputType, u16 wGridId, s32 nChnId, BOOL32 bNotSave);

/**
 * @brief  批量解绑
 * @param  dwHandle    [in]句柄
 *         eOutputType [in]显示类型
 *         tUnbindList [in]解绑列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_UnbindEx(u32 dwHandle, EOutputType eOutputType, TNetBindList tUnbindList);

/**
 * @brief  设置画中画
 * @param  dwHandle    [in]句柄
 *         eOutputType [in]显示类型
 *         tPipList    [in]画中画列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetPip(u32 dwHandle, EOutputType eOutputType, TNetPipList tPipList);

/**
 * @brief  抓拍
 * @param  dwHandle    [in]句柄
 *         eOutputType [in]显示类型
 *         wGridId     [in]格子id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_Snap(u32 dwHandle, EOutputType eOutputType, u16 wGridId);

/**
 * @brief  通道抓拍
 * @param  dwHandle    [in]句柄
 *         tChnSnap    [in]抓拍参数
 *         paChnSnapFileInfo [out]抓拍的文件信息数组
 *         pnFileNum         [in/out]输入paChnSnapFileInfo指向的数组大小，输出文件信息数量
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_ChnSnap(u32 dwHandle, TNetChnSnap tChnSnap, TNetChnSnapFileInfo *paChnSnapFileInfo, s32 *pnFileNum);

/**
 * @brief  获取音量及静音信息
 * @param  dwHandle    [in]句柄
 *         eOutputType [in]显示类型
 *         ptVolume    [out]音量信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetVolume(u32 dwHandle, EOutputType eOutputType, TNetVolume *ptVolume);

/**
 * @brief  设置音量及静音
 * @param  dwHandle    [in]句柄
 *         eOutputType [in]显示类型
 *         tVolume     [in]音量信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetVolume(u32 dwHandle, EOutputType eOutputType, TNetVolume tVolume);

/**
 * @brief  开启音频解码
 * @param  dwHandle  [in]句柄
 *         tAudioDec [in]音频解码参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StartAudioDec(u32 dwHandle, TNetAudioDec tAudioDec);

/**
 * @brief  停止音频解码
 * @param  dwHandle  [in]句柄
 *         tAudioDec [in]音频解码参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StopAudioDec(u32 dwHandle, TNetAudioDec tAudioDec);

/**
 * @brief  获取呼叫能力
 * @param  dwHandle  [in]句柄
 *         ptCallCap [out]呼叫能力
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCallCap(u32 dwHandle, TNetCallCap *ptCallCap);

/**
 * @brief  开始呼叫，仅支持本地呼叫
 * @param  dwHandle [in]句柄
 *         tCall    [in/out]呼叫参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StartCall(u32 dwHandle, TNetCall *ptCall);

/**
 * @brief  开始呼叫，支持本地呼叫、远端呼叫
 * @param  dwHandle [in]句柄
 *         tCall    [in/out]呼叫参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StartCalleEx(u32 dwHandle, TNetCallParam tCallParam, TNetCallResult *ptCallResult);

/**
 * @brief  停止呼叫
 * @param  dwHandle [in]句柄
 *         nCallId  [in]呼叫id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StopCall(u32 dwHandle, s32 nCallId);

/**
 * @brief  获取呼叫音量
 * @param  dwHandle     [in]句柄
 *         pnCallVolume [in]呼叫音量
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCallVolume(u32 dwHandle, s32 *pnCallVolume);

/**
 * @brief  设置呼叫音量
 * @param  dwHandle    [in]句柄
 *         nCallVolume [in]呼叫音量
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetCallVolume(u32 dwHandle, s32 nCallVolume);

/**
 * @brief  发送PTZ命令
 * @param  dwHandle [in]句柄
 *         tPtzCtrl [in]PTZ命令
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SendPtzCmd(u32 dwHandle, TNetPtzCtrl tPtzCtrl);

/**
 * @brief  获取通道的云台所有预置位信息
 * @param  dwHandle			[in]句柄
 *         nChnId			[in]通道号
 *         ptPtzPresetList	[out]云台所有预置位信息			
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetPtzPresetAll(u32 dwHandle, s32 nChnId, TNetPtzPresetList *ptPtzPresetList);

/**
 * @brief  获取前端的鱼眼模式配置
 * @param  dwHandle				[in]句柄
 *         ptNetFishEyeModeCfg	[in/out]配置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetFishEyeModeCfg(u32 dwHandle, TNetFishEyeModeCfg *ptNetFishEyeModeCfg);

/**
 * @brief  设置前端的鱼眼模式配置
 * @param  dwHandle				[in]句柄
 *         tNetFishEyeModeCfg	[in]配置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetFishEyeModeCfg(u32 dwHandle, TNetFishEyeModeCfg tNetFishEyeModeCfg);

/**
 * @brief  获取前端的鱼眼坐标ptz配置
 * @param  dwHandle					[in]句柄
 *         ptNetFishEyePtzRectCfg	[in/out]配置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetFishEyePtzRectCfg(u32 dwHandle, TNetFishEyePtzRectCfg *ptNetFishEyePtzRectCfg);

/**
 * @brief  设置前端的鱼眼坐标ptz配置
 * @param  dwHandle					[in]句柄
 *         tNetFishEyePtzRectCfg	[in]配置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetFishEyePtzRectCfg(u32 dwHandle, TNetFishEyePtzRectCfg tNetFishEyePtzRectCfg);

/**
 * @brief  获取ISP能力
 * @param  dwHandle		[in]句柄
 *         ptIspCaps	[out]能力信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspCap(u32 dwHandle, TNetIspCaps *ptIspCaps);

/**
 * @brief  获取ISP配置
 * @param  dwHandle [in]句柄
 *         ptIspCfg [in/out]配置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspCfg(u32 dwHandle, TNetIspCfg *ptIspCfg);

/**
 * @brief  设置ISP配置
 * @param  dwHandle [in]句柄
 *         ptIspCfg [in]配置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspCfg(u32 dwHandle, TNetIspCfg tIspCfg);

/**
 * @brief  获取通道的isp 增益参数相关配置
 * @param  dwHandle     [in]句柄
 *		   tIspGainCfg  [in/out]增益参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspGainCfg(u32 dwHandle, TNetIspGainCfg *ptIspGainCfg);

/**
 * @brief  设置通道的isp 增益参数相关配置
 * @param  dwHandle     [in]句柄
 *		   tIspGainCfg  [in]增益参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspGainCfg(u32 dwHandle, TNetIspGainCfg tIspGainCfg);

/**
 * @brief  获取通道的isp 光圈参数相关配置
 * @param  dwHandle     [in]句柄
 *		   ptIspIrisCfg  [in/out]光圈参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspIrisCfg(u32 dwHandle, TNetIspIrisCfg *ptIspIrisCfg);

/**
 * @brief  设置通道的isp 光圈参数相关配置
 * @param  dwHandle     [in]句柄
 *		   tIspIrisCfg  [in]光圈参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspIrisCfg(u32 dwHandle, TNetIspIrisCfg tIspIrisCfg);

/**
 * @brief  获取通道的isp 聚焦参数相关配置
 * @param  dwHandle			[in]句柄
 *		   ptIspFocusCfg	[in/out]聚焦参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspFocusCfg(u32 dwHandle, TNetIspFocusCfg *ptIspFocusCfg);

/**
 * @brief  设置通道的isp 聚焦参数相关配置
 * @param  dwHandle      [in]句柄
 *		   tIspFocusCfg  [in]聚焦参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspFocusCfg(u32 dwHandle, TNetIspFocusCfg tIspFocusCfg);

/**
 * @brief  获取通道的isp 快门参数相关配置
 * @param  dwHandle			[in]句柄
 *		   ptIspFocusCfg	[in/out]快门参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspShutterCfg(u32 dwHandle, TNetIspShutterCfg *ptIspShutterCfg);

/**
 * @brief  设置通道的isp 快门参数相关配置
 * @param  dwHandle			[in]句柄
 *		   tIspShutterCfg	[in]快门参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspShutterCfg(u32 dwHandle, TNetIspShutterCfg tIspShutterCfg);

/**
 * @brief  获取通道的isp 防闪烁模式参数
 * @param  dwHandle			[in]句柄
 *		   ptIspFreqModeCfg	[in/out]防闪烁模式参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspFreqModeCfg(u32 dwHandle, TNetIspFreqModeCfg *ptIspFreqModeCfg);

/**
 * @brief  设置通道的isp 防闪烁模式参数
 * @param  dwHandle			[in]句柄
 *		   tIspFreqModeCfg	[in]防闪烁模式参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspFreqModeCfg(u32 dwHandle, TNetIspFreqModeCfg tIspFreqModeCfg);

/**
 * @brief  获取通道的isp 白平衡参数参数
 * @param  dwHandle			[in]句柄
 *		   ptIspFreqModeCfg	[in/out]白平衡参数参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspWhiteBalanceCfg(u32 dwHandle, TNetIspWhiteBalanceCfg *ptIspWhiteBalanceCfg);

/**
 * @brief  设置通道的isp 白平衡参数参数
 * @param  dwHandle				[in]句柄
 *		   tIspWhiteBalanceCfg	[in]白平衡参数参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspWhiteBalanceCfg(u32 dwHandle, TNetIspWhiteBalanceCfg tIspWhiteBalanceCfg);

/**
 * @brief  获取通道的isp 日夜转换参数
 * @param  dwHandle					[in]句柄
 *		   ptIspDayNightSwitchCfg	[in/out]日夜转换参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspDayNightSwitchCfg(u32 dwHandle, TNetIspDayNightSwitchCfg *ptIspDayNightSwitchCfg);

/**
 * @brief  设置通道的isp 日夜转换参数
 * @param  dwHandle					[in]句柄
 *		   tIspDayNightSwitchCfg	[in]日夜转换参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspDayNightSwitchCfg(u32 dwHandle, TNetIspDayNightSwitchCfg tIspDayNightSwitchCfg);

/**
 * @brief  获取通道的isp 2D降噪参数
 * @param  dwHandle			[in]句柄
 *		   ptIspDenoiseCfg	[in/out]2D降噪参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspDenoise2DCfg(u32 dwHandle, TNetIspDenoiseCfg *ptIspDenoiseCfg);

/**
 * @brief  设置通道的isp 2D降噪参数
 * @param  dwHandle			[in]句柄
 *		   tIspDenoiseCfg	[in]2D降噪参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspDenoise2DCfg(u32 dwHandle, TNetIspDenoiseCfg tIspDenoiseCfg);

/**
 * @brief  获取通道的isp 3D降噪参数
 * @param  dwHandle			[in]句柄
 *		   ptIspDenoiseCfg	[in/out]3D降噪参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspDenoise3DCfg(u32 dwHandle, TNetIspDenoiseCfg *ptIspDenoiseCfg);

/**
 * @brief  设置通道的isp 3D降噪参数
 * @param  dwHandle			[in]句柄
 *		   tIspDenoiseCfg	[in]3D降噪参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspDenoise3DCfg(u32 dwHandle, TNetIspDenoiseCfg tIspDenoiseCfg);

/**
 * @brief  获取通道的isp 动态调节参数
 * @param  dwHandle					[in]句柄
 *		   ptIspDynamicRegulateCfg	[in/out]动态调节参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspDynamicRegulateCfg(u32 dwHandle, TNetIspDynamicRegulateCfg *ptIspDynamicRegulateCfg);

/**
 * @brief  设置通道的isp 动态调节参数
 * @param  dwHandle					[in]句柄
 *		   tIspDynamicRegulateCfg	[in]动态调节参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspDynamicRegulateCfg(u32 dwHandle, TNetIspDynamicRegulateCfg tIspDynamicRegulateCfg);

/**
 * @brief  获取通道的isp 图像增强参数
 * @param  dwHandle					[in]句柄
 *		   ptIspImageEnhanceCfg		[in/out]图像增强参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspImageEnhanceCfg(u32 dwHandle, TNetIspImageEnhanceCfg *ptIspImageEnhanceCfg);

/**
 * @brief  设置通道的isp 图像增强参数
 * @param  dwHandle					[in]句柄
 *		   tIspImageEnhanceCfg		[in]图像增强参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspImageEnhanceCfg(u32 dwHandle, TNetIspImageEnhanceCfg tIspImageEnhanceCfg);

/**
 * @brief  获取通道的isp 防抖参数
 * @param  dwHandle					[in]句柄
 *		   ptIspStablizerCfg		[in/out]防抖参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspStablizerCfg(u32 dwHandle, TNetIspStablizerCfg *ptIspStablizerCfg);

/**
 * @brief  设置通道的isp 防抖参数
 * @param  dwHandle				[in]句柄
 *		   tIspStablizerCfg		[in]防抖参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspStablizerCfg(u32 dwHandle, TNetIspStablizerCfg tIspStablizerCfg);

/**
 * @brief  获取通道的isp 合成宽动态模式参数
 * @param  dwHandle					[in]句柄
 *		   ptIspCombinHdrCfg		[in/out]合成宽动态模式参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspCombinHdrCfg(u32 dwHandle, TNetIspCombinHdrCfg *ptIspCombinHdrCfg);

/**
 * @brief  设置通道的isp 合成宽动态模式参数
 * @param  dwHandle				[in]句柄
 *		   tIspCombinHdrCfg		[in]合成宽动态模式参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspCombinHdrCfg(u32 dwHandle, TNetIspCombinHdrCfg tIspCombinHdrCfg);

/**
 * @brief  获取通道的isp 二倍镜模式参数
 * @param  dwHandle					[in]句柄
 *		   ptIspCombinHdrCfg		[in/out]二倍镜模式参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspDoubleLensModeCfg(u32 dwHandle, TNetIspDoubleLensCfg *ptIspDoubleLensModeCfg);

/**
 * @brief  设置通道的isp 二倍镜模式参数
 * @param  dwHandle				[in]句柄
 *		   tIspCombinHdrCfg		[in]二倍镜模式参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspDoubleLensModeCfg(u32 dwHandle, TNetIspDoubleLensCfg tIspDoubleLensModeCfg);

/**
 * @brief  获取通道的isp 曝光全自动模式参数
 * @param  dwHandle					[in]句柄
 *		   ptIspCombinHdrCfg		[in/out]曝光全自动模式参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspExpAllAutoCfg(u32 dwHandle, TNetIspExpAllAutoCfg *ptIspExpAllAutoModeCfg);

/**
 * @brief  设置通道的isp 曝光全自动模式参数
 * @param  dwHandle				[in]句柄
 *		   tIspCombinHdrCfg		[in]曝光全自动模式参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspExpAllAutoCfg(u32 dwHandle, TNetIspExpAllAutoCfg tIspExpAllAutoModeCfg);

/**
 * @brief  获取通道的isp 高级自动曝光参数
 * @param  dwHandle					[in]句柄
 *		   ptProExposureCfg			[in/out]高级自动曝光参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspProExposureCfg(u32 dwHandle, TNetIspProExposureCfg *ptProExposureCfg);

/**
 * @brief  设置通道的isp 高级自动曝光参数
 * @param  dwHandle					[in]句柄
 *		   tProExposureCfg			[in]高级自动曝光参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspProExposureCfg(u32 dwHandle, TNetIspProExposureCfg tProExposureCfg);

/**
 * @brief  获取通道的isp 场景模式参数
 * @param  dwHandle					[in]句柄
 *		   ptSceneModeCfg			[in/out]场景模式参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspSceneModeCfg(u32 dwHandle, TNetIspSceneModeCfg *ptSceneModeCfg);

/**
 * @brief  设置通道的isp 场景模式参数
 * @param  dwHandle					[in]句柄
 *		   tSceneModeCfg			[in]场景模式参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspSceneModeCfg(u32 dwHandle, TNetIspSceneModeCfg tSceneModeCfg);

/**
 * @brief  获取通道的isp 翻转和回显参数
 * @param  dwHandle					[in]句柄
 *		   ptFlipAndPlayBackCfg		[in/out]翻转和回显参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspFlipAndPlayBackCfg(u32 dwHandle, TNetIspFlipAndPlayBackCfg *ptFlipAndPlayBackCfg);

/**
 * @brief  设置通道的isp 翻转和回显参数
 * @param  dwHandle					[in]句柄
 *		   tFlipAndPlayBackCfg		[in]翻转和回显参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspFlipAndPlayBackCfg(u32 dwHandle, TNetIspFlipAndPlayBackCfg tFlipAndPlayBackCfg);

/**
 * @brief  获取通道的isp 红外灯配置参数
 * @param  dwHandle				[in]句柄
 *		   ptIspInfraredCfg		[in/out]红外灯配置参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspInfraredCfg(u32 dwHandle, TNetIspInfraredCfg *ptIspInfraredCfg);

/**
 * @brief  设置通道的isp 红外灯配置参数
 * @param  dwHandle				[in]句柄
 *		   tIspInfraredCfg		[in]红外灯配置参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspInfraredCfg(u32 dwHandle, TNetIspInfraredCfg tIspInfraredCfg);

/**
 * @brief  获取通道的isp 激光配置参数
 * @param  dwHandle				[in]句柄
 *		   tIspLaserCfg			[in/out]激光配置参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspLaserCfg(u32 dwHandle, TNetIspLaserCfg *ptIspLaserCfg);

/**
 * @brief  设置通道的isp 激光配置参数
 * @param  dwHandle				[in]句柄
 *		   tIspLaserCfg			[in]激光配置参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspLaserCfg(u32 dwHandle, TNetIspLaserCfg tIspLaserCfg);

/**
 * @brief  获取通道的isp 畸变矫正参数
 * @param  dwHandle				[in]句柄
 *		   ptIspLdcCfg			[in/out]激光配置参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIspLdcCfg(u32 dwHandle, TNetIspLdcCfg *ptIspLdcCfg);

/**
 * @brief  设置通道的isp 畸变矫正参数
 * @param  dwHandle				[in]句柄
 *		   tIspLdcCfg			[in]激光配置参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIspLdcCfg(u32 dwHandle, TNetIspLdcCfg tIspLdcCfg);

/**
 * @brief  数字放大
 * @param  dwHandle     [in]句柄
 *         eOutputType  [in]显示类型
 *         tDigitalZoom [in]数字放大参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DigitalZoom(u32 dwHandle, EOutputType eOutputType, TNetDigitalZoom tDigitalZoom);

/**
 * @brief  获取解码模式
 * @param  dwHandle    [in]句柄
 *         eOutputType [in]显示类型
 *         tDecMode    [in/out]解码模式
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDecMode(u32 dwHandle, EOutputType eOutputType, TNetDecMode *ptDecMode);

/**
 * @brief  设置解码模式
 * @param  dwHandle    [in]句柄
 *         eOutputType [in]显示类型
 *         tDecMode    [in]解码模式
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetDecMode(u32 dwHandle, EOutputType eOutputType, TNetDecMode tDecMode);

/**
 * @brief  获取码流信息
 * @param  dwHandle			 [in]句柄
 *         eOutputType       [in]显示类型
 *         wGridId           [in]grid id
 *         ptVideoStreamInfo [out]码流信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetVideoStreamInfo(u32 dwHandle, EOutputType eOutputType, u16 wGridId, TNetVideoStreamInfo *ptVideoStreamInfo);

/**
 * @brief  获取显示模式
 * @param  dwHandle		 [in]句柄
 *         eOutputType   [in]显示类型
 *         ptDisModeList [out]显示模式信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDisMode(u32 dwHandle, EOutputType eOutputType, TNetDisModeList *ptDisModeList);

/**
 * @brief  设置显示模式
 * @param  dwHandle		[in]句柄
 *         eOutputType  [in]显示类型
 *         tDisModeItem [in]显示模式信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetDisMode(u32 dwHandle, EOutputType eOutputType, TNetDisModeItem tDisModeItem);

/**
 * @brief  获取用于拉取实时码流的rtsp url
 * @param  dwHandle		               [in]句柄
 *         tGetRtspRealStreamUrlParam  [in]请求参数
 *         ptRtspRealStreamUrl         [out]rtsp url
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRtspRealStreamUrl(u32 dwHandle, TNetGetRtspRealStreamUrlParam tGetRtspRealStreamUrlParam, TNetRtspRealStreamUrl *ptRtspRealStreamUrl);

/**
 * @brief  实时码流强制关键帧
 * @param  dwHandle	 [in]句柄
 *         nChnId    [in]通道id
 *         nVidEncId [in]视频编码通道id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_RealStreamForceKeyFrame(u32 dwHandle, s32 nChnId, s32 nVidEncId);

/**
 * @brief  获取解码状态
 * @param  dwHandle	      [in]句柄
 *         tGetDecState   [in]获取参数
 *         ptDecStateList [out]解码状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDecState(u32 dwHandle, TNetGetDecState tGetDecState, TNetDecStateList *ptDecStateList);

/**
 * @brief  获取网卡模式
 * @param  dwHandle  [in]句柄
 *         ptEthMode [out]网卡模式信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetEthMode(u32 dwHandle, TNetEthMode *ptEthMode);

/**
 * @brief  设置网卡模式
 * @param  dwHandle [in]句柄
 *         tEthMode [in]网卡模式信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetEthMode(u32 dwHandle, TNetEthMode tEthMode);

/**
 * @brief  获取网卡信息
 * @param  dwHandle  [in]句柄
 *         ptEthInfo [out]网卡信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetEthInfo(u32 dwHandle, TNetEthInfo *ptEthInfo);

/**
 * @brief  设置网卡信息
 * @param  dwHandle [in]句柄
 *         tEthInfo [in]网卡信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetEthInfo(u32 dwHandle, TNetEthInfo tEthInfo);

/**
 * @brief  获取虚拟网卡信息
 * @param  dwHandle   [in]句柄
 *         eNetMode   [in]网卡工作模式，仅NETMODE_FAULTTOL、NETMODE_LOADBAL有效
 *         ptEthParam [out]虚拟网卡信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetVirEthInfo(u32 dwHandle, ENetMode eNetMode, TNetEthParam *ptEthParam);

/**
 * @brief  设置虚拟网卡信息
 * @param  dwHandle  [in]句柄
 *         eNetMode  [in]网卡工作模式，仅NETMODE_FAULTTOL、NETMODE_LOADBAL有效
 *         tEthParam [in]虚拟网卡信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetVirEthInfo(u32 dwHandle, ENetMode eNetMode, TNetEthParam tEthParam);

/**
 * @brief  获取服务端口
 * @param  dwHandle     [in]句柄
 *         ptServerPort [out]服务端口
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetServerPort(u32 dwHandle, TNetServerPort *ptServerPort);

/**
 * @brief  设置服务端口
 * @param  dwHandle    [in]句柄
 *         tServerPort [in]服务端口
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetServerPort(u32 dwHandle, TNetServerPort tServerPort);

/**
 * @brief  获取wifi使能
 * @param  dwHandle    [in]句柄
 *         bEnable	   [out]wifi是否开启
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetWifiEnable(u32 dwHandle, BOOL32 *pbEnable);

/**
 * @brief  设置wifi使能
 * @param  dwHandle    [in]句柄
 *         bEnable	   [in]wifi是否开启
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetWifiEnable(u32 dwHandle, BOOL32 bEnable);

/**
 * @brief  获取wifi模式
 * @param  dwHandle		[in]句柄
 *         ptWifiMode	[out]wifi模式
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetWifiMode(u32 dwHandle, TNetWifiMode *ptWifiMode);

/**
 * @brief  设置wifi模式
 * @param  dwHandle		[in]句柄
 *         tWifiMode	[in]wifi模式
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetWifiMode(u32 dwHandle, TNetWifiMode tWifiMode);

/**
 * @brief  获取移动网络状态
 * @param  dwHandle				[in]句柄
 *         nSimCardId			[in]simId，从1开始
 *         ptMobileNetStatus	[out]移动网络状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetMobileNetworkStatus(u32 dwHandle, s32 nSimCardId, TNetMobileNetworkStatus *ptMobileNetStatus);

/**
 * @brief  获取多播配置
 * @param  dwHandle	      [in]句柄
 *         ptMulticastCfg [out]多播配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetMulticastCfg(u32 dwHandle, TNetMulticastCfg *ptMulticastCfg);

/**
 * @brief  设置多播配置
 * @param  dwHandle		 [in]句柄
 *         tMulticastCfg [in]多播配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetMulticastCfg(u32 dwHandle, TNetMulticastCfg tMulticastCfg);

/**
 * @brief  获取系统基本参数
 * @param  dwHandle       [in]句柄
 *         ptSysBaseParam [out]系统基本参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSysBaseParam(u32 dwNvrIp, u16 wPort, TNetSysBaseParam *ptSysBaseParam);

/**
 * @brief  设置系统基本参数
 * @param  dwHandle      [in]句柄
 *         tSysBaseParam [in]系统基本参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetSysBaseParam(u32 dwHandle, TNetSysBaseParam tSysBaseParam);

/**
 * @brief  获取零通道编码参数
 * @param  dwHandle          [in]句柄
 *         ptZeroChnEncParam [out]零通道编码参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSysZeroChnEncParam(u32 dwHandle, TNetZeroChnEncParam *ptZeroChnEncParam);

/**
 * @brief  获取零通道编码参数
 * @param  dwHandle         [in]句柄
 *         tZeroChnEncParam [in]零通道编码参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetSysZeroChnEncParam(u32 dwHandle, TNetZeroChnEncParam tZeroChnEncParam);

/**
 * @brief  获取用户列表
 * @param  dwHandle   [in]句柄
 *         ptUserList [out]用户列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetUserList(u32 dwHandle, TNetUserList *ptUserList);

/**
 * @brief  添加用户
 * @param  dwHandle [in]句柄
 *         tAddUser [in]添加的用户
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_AddUser(u32 dwHandle, TNetAddUser tAddUser);

/**
 * @brief  删除用户
 * @param  dwHandle    [in]句柄
 *         pszUserName [in]删除的用户
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DelUser(u32 dwHandle, const s8 *pszUserName);

/**
 * @brief  编辑用户
 * @param  dwHandle  [in]句柄
 *         tEditUser [in]编辑的用户
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_EditUser(u32 dwHandle, TNetEditUser tEditUser);

/**
 * @brief  修改用户密码
 * @param  dwHandle       [in]句柄
 *         tModifyUserPwd [in]用户新密码
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_ModifyUserPwd(u32 dwHandle, TNetModifyUserPwd tModifyUserPwd);

/**
 * @brief  获取用户权限能力
 * @param  dwHandle          [in]句柄
 *         ptUserPermCapInfo [out]获取的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetUserPermCapInfo(u32 dwHandle, TNetUserPermCapInfo *ptUserPermCapInfo);

/**
 * @brief  获取用户权限
 * @param  dwHandle       [in]句柄
 *         ptUserPermInfo [out]获取的信息,需要传入用户名
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetUserPermInfo(u32 dwHandle, TNetUserPermInfo *ptUserPermInfo);

/**
 * @brief  设置用户权限
 * @param  dwHandle       [in]句柄
 *         tUserPermInfo  [in]要设置的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetUserPermInfo(u32 dwHandle, TNetUserPermInfo tUserPermInfo);

/**
 * @brief  获取rtsp认证方式
 * @param  dwHandle       [in]句柄
 *         ptRtspAuthInfo [out]获取的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRtspAuthInfo(u32 dwHandle, TNetRtspAuthInfo *ptRtspAuthInfo);

/**
 * @brief  设置rtsp认证方式
 * @param  dwHandle       [in]句柄
 *         tRtspAuthInfo  [in]要设置的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetRtspAuthInfo(u32 dwHandle, TNetRtspAuthInfo tRtspAuthInfo);

/**
 * @brief  获取安全服务信息
 * @param  dwHandle              [in]句柄
 *         ptSecurityServiceInfo [out]获取的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSecurityServiceInfo(u32 dwHandle, TNetSecurityServiceInfo *ptSecurityServiceInfo);

/**
 * @brief  设置安全服务信息
 * @param  dwHandle              [in]句柄
 *         tSecurityServiceInfo  [in]要设置的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetSecurityServiceInfo(u32 dwHandle, TNetSecurityServiceInfo tSecurityServiceInfo);

/**
 * @brief  获取ip地址过滤配置
 * @param  dwHandle              [in]句柄
 *         ptIpAddrFilterCfg     [out]获取的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIpAddrFilterCfg(u32 dwHandle, TNetIpAddrFilterCfg *ptIpAddrFilterCfg);

/**
 * @brief  设置ip地址过滤配置
 * @param  dwHandle              [in]句柄
 *         tIpAddrFilterCfg      [out]获取的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIpAddrFilterCfg(u32 dwHandle, TNetIpAddrFilterCfg tIpAddrFilterCfg);

/**
 * @brief  获取VGA显示接口参数
 * @param  dwNvrIp               [in]设备ip
 *         wPort                 [in]设备端口号
 *         ptDisplayPortVgaList  [out]获取的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDisplayPortVgaParam(u32 dwNvrIp, u16 wPort, TNetDisplayPortVgaList *ptDisplayPortVgaList);

/**
 * @brief  设置VGA显示接口参数
 * @param  dwHandle              [in]句柄
 *         tDisplayPortVgaList   [in]要设置的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetDisplayPortVgaParam(u32 dwHandle, TNetDisplayPortVgaList tDisplayPortVgaList);

/**
 * @brief  获取HDMI显示接口参数
 * @param  dwNvrIp               [in]设备ip
 *         wPort                 [in]设备端口号
 *         ptDisplayPortHdmiList [out]获取的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDisplayPortHdmiParam(u32 dwNvrIp, u16 wPort, TNetDisplayPortHdmiList *ptDisplayPortHdmiList);

/**
 * @brief  设置HDMI显示接口参数
 * @param  dwHandle             [in]句柄
 *         tDisplayPortHdmiList [in]要设置的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetDisplayPortHdmiParam(u32 dwHandle, TNetDisplayPortHdmiList tDisplayPortHdmiList);

/**
 * @brief  获取CVBS显示接口参数
 * @param  dwNvrIp               [in]设备ip
 *         wPort                 [in]设备端口号
 *         ptDisplayPortCvbsList [out]获取的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDisplayPortCvbsParam(u32 dwNvrIp, u16 wPort, TNetDisplayPortCvbsList *ptDisplayPortCvbsList);

/**
 * @brief  设置CVBS显示接口参数
 * @param  dwHandle             [in]句柄
 *         tDisplayPortCvbsList [in]要设置的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetDisplayPortCvbsParam(u32 dwHandle, TNetDisplayPortCvbsList tDisplayPortCvbsList);

/**
 * @brief  获取输出类型
 * @param  dwNvrIp      [in]设备ip
 *         wPort        [in]设备端口号
 *         peOutputType [out]获取的输出类型
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetOutputType(u32 dwNvrIp, u16 wPort, EOutputType *peOutputType);

/**
 * @brief  设置输出类型
 * @param  dwHandle    [in]句柄
 *         eOutputType [out]输出类型
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetOutputType(u32 dwHandle, BOOL32 bValid, EOutputType eOutputType);

/**
 * @brief  获取串口配置信息
 * @param  dwHandle            [in] 句柄
 *         nSerialPortIndex    [in] 串口索引
 *         ptSerialPortInfo    [out]串口信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSerialPortInfo(u32 dwHandle, s32 nSerialPortIndex, TNetSerialPortInfo *ptSerialPortInfo);

/**
 * @brief  设置串口配置信息
 * @param  dwHandle            [in] 句柄
 *         nSerialPortIndex    [in] 串口索引
 *         tSerialPortInfo    [in]串口信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetSerialPortInfo(u32 dwHandle, s32 nSerialPortIndex, TNetSerialPortInfo tSerialPortInfo);

/**
 * @brief  获取ktsm模块是否激活
 * @param  dwHandle					[in] 句柄
 *         ptKtsmActiveStatus		[out]激活信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetKtsmActiveStatus(u32 dwHandle, TNetKtsmActiveStatus *ptKtsmActiveStatus);

/**
 * @brief  激活ktsm模块
 * @param  dwHandle			[in]句柄
 *         pszPinCode		[in]PIN码
 *         ptActiveKtsmModule [out]激活信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_ActiveKtsmModule(u32 dwHandle, s8 *pszPinCode, TNetActiveKtsmModule *ptActiveKtsmModule);

/**
 * @brief  检测ktsm模块状态
 * @param  dwHandle			[in] 句柄
 *         ptKtsmStatus		[out]ktsm模块状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetKtsmStatus(u32 dwHandle, TNetKtsmStatus *ptKtsmStatus);

/**
 * @brief  获取ktsm密管服务ip和端口
 * @param  dwHandle				[in] 句柄
 *         ptKtsmMgrSrvInfo		[out]ktsm密管服务信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetKtsmPwdMgrSrvInfo(u32 dwHandle, TNetKtsmMgrSrvInfo *ptKtsmMgrSrvInfo);

/**
 * @brief  设置ktsm密管服务ip和端口
 * @param  dwHandle				[in] 句柄
 *         tKtsmMgrSrvInfo		[in]ktsm密管服务信息
 *		   ptActiveKtsmModule	[out]错误信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetKtsmPwdMgrSrvInfo(u32 dwHandle, TNetKtsmMgrSrvInfo tKtsmMgrSrvInfo, TNetActiveKtsmModule *ptActiveKtsmModule);

/**
 * @brief  重置ktsm网络序列号
 * @param  dwHandle				[in] 句柄

 *		   ptActiveKtsmModule	[out]错误信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_ResetKtsmNetLicense(u32 dwHandle, TNetActiveKtsmModule *ptActiveKtsmModule);

/**
 * @brief  获取模块用户名密码
 * @param  dwHandle				[in] 句柄
 *         ptKtsmUserInfo		[out]用户名密码
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetKtsmUserInfo(u32 dwHandle, TNetKtsmUserInfo *ptKtsmUserInfo);

/**
 * @brief  设置模块用户名密码
 * @param  dwHandle				[in] 句柄
 *         tKtsmMgrSrvInfo		[in]用户名密码
 *		   ptActiveKtsmModule	[out]错误信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetKtsmUserInfo(u32 dwHandle, TNetKtsmUserInfo tKtsmUserInfo, TNetActiveKtsmModule *ptActiveKtsmModule);

/**
 * @brief  获取设备信息
 * @param  dwHandle                [in]句柄
 *         ptNvrDevInfo            [out]设备信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNvrDevInfo(u32 dwHandle, TNetNvrDevInfo *ptNvrDevInfo);

/**
 * @brief  获取通道录像状态
 * @param  dwHandle               [in]句柄
 *         nStartChnId            [in]起始通道ID
 *         nEndChnId              [in]结束通道ID
 *         ptChnRecStateList      [out]通道录像状态列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnRecState(u32 dwHandle, s32 nStartChnId, s32 nEndChnId, TNetChnRecStateList *ptChnRecStateList);

/**
 * @brief  获取日志能力
 * @param  dwHandle                [in]句柄
 *         pdwLangCap              [out]日志语言能力,指针为空不获取
 *         ptLogMainTypeItem       [out]获取的信息
 *         ptLogMainTypeItem       [in/out]传入最大数量返回实际数量
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetLogCap(u32 dwHandle, u32 *pdwLangCap, TNetLogMainTypeItem *patLogMainTypeItem, s32 *pLen);

/**
 * @brief  创建搜索日志任务
 * @param  dwHandle                       [in]句柄
 *         tCreateSearchLogTaskParam      [in]创建task的参数
 *         ptSearchLogTaskInfo            [out]获取的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreateSearchLogTask(u32 dwHandle, TNetCreateSearchLogTaskParam tCreateSearchLogTaskParam, TNetSearchLogTaskInfo* ptSearchLogTaskInfo);

/**
 * @brief  销毁搜索日志任务
 * @param  dwHandle                       [in]句柄
 *         dwTaskId                       [in]任务id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroySearchLogTask(u32 dwHandle, u32 dwTaskId);

/**
 * @brief  获取搜索日志结果
 * @param  dwHandle                       [in]句柄
 *         dwTaskId                       [in]任务id
 *         ptSearchLogItem                [out]获取的信息
 *         pNums                          [in/out]传入最大数量返回实际数量
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSearchLogResult(u32 dwHandle, u32 dwTaskId, u32 dwStart, TNetSearchLogItem *patSearchLogItem, u32 *pNums);

/**
 * @brief  导出搜索日志结果
 * @param  dwHandle                       [in]句柄
 *         dwTaskId                       [in]任务id
 *         pszPath                        [in]导出路径
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_ExportSearchLogResult(u32 dwHandle, u32 dwTaskId, s8 *pszPath);

/**
 * @brief  写操作日志
 * @param  dwHandle                       [in]句柄
 *         pszLogType                     [in]日志类型
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_WriteOperationLog(u32 dwHandle, s8 *pszLogType);

/**
 * @brief  获取健康状态信息
 * @param  dwHandle               [in]句柄
 		   eHealthLogTimeType     [in]时间段类型
		   ptHealthLogList        [out]健康状态信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetHealthState(u32 dwHandle, EHealthLogTimeType eHealthLogTimeType, TNetHealthLogList *ptHealthLogList);

/**
 * @brief  获取健康状态详情
 * @param  dwHandle               [in]句柄
 		   eHealthLogTimeType     [in]时间段类型
		   ptHealthLogList        [out]健康状态信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetHealthStateDetail(u32 dwHandle, TNetHealthDetailParam tHealthDetailParam, TNetHealthDetailList *ptHealthDetailList);

/**
 * @brief  获取实时日志
 * @param  dwHandle      [in]句柄
 *         tGetRealLog   [in]获取实时日志参数
 *         ptRealLogList [out]获取的实时日志列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRealLog(u32 dwHandle, TNetGetRealLog tGetRealLog, TNetRealLogList *ptRealLogList);

/**
 * @brief  获取实时日志详情
 * @param  dwHandle          [in]句柄
 *         tGetRealLogDetail [in]获取实时日志详情参数
 *         ptRealLogDetail   [out]实时日志详情
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRealLogDetail(u32 dwHandle, TNetGetRealLogDetail tGetRealLogDetail, TNetRealLogDetail *ptRealLogDetail);

/**
 * @brief  获取实时状态
 * @param  dwHandle        [in]句柄
 *         ptRealStateList [out]获取的实时状态列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRealState(u32 dwHandle, TNetRealStateList *ptRealStateList);

/**
 * @brief  创建获取系统时间参数
 * @param  dwHandle                       [in]句柄
 *         tSystemTimeInfo			      [in]创建system time的参数
 *         ptSystemTimeParam              [out]获取的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSystemTimeParam(u32 dwHandle, TNetSystemTimeInfo tSystemTimeInfo, TNetSystemTimeParam* ptSystemTimeParam);

/**
 * @brief  创建设置系统时间参数
 * @param  dwHandle                       [in]句柄
 *         tSystemTimeParam			      [in]设置的参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetSystemTimeParam(u32 dwHandle, TNetSystemTimeParam tSystemTimeParam);

/**
 * @brief  创建获取系统时间参数
 * @param  dwHandle                       [in]句柄
 *         ptSystemTimeParam              [out]获取的信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSystemTimeParamEx(u32 dwHandle, TNetSystemTimeParamEx* ptSystemTimeParam);

/**
 * @brief  创建设置系统时间参数
 * @param  dwHandle                       [in]句柄
 *         tSystemTimeParam			      [in]设置的参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetSystemTimeParamEx(u32 dwHandle, TNetSystemTimeParamEx tSystemTimeParam);

/**
 * @brief  创建设置盘组的硬盘ID
 * @param  dwHandle                       [in]句柄
 *         tGroupDiskList			      [in]创建盘组硬盘ID的参数
 *         ptSetGroupDiskErrInfo          [out]错误反馈信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetGroupDiskList(u32 dwHandle, TNetGroupDiskList tGroupDiskList, TNetSetGroupDiskErrInfo *ptSetGroupDiskErrInfo);

/**
 * @brief  查询硬盘初始化进度
 * @param  dwHandle                       [in]句柄
 *         dwTaskId					      [in]task id
 *  	   pwProgress					  [out]进度条
 *  	   peDiskInitState                [out]状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_QueryDiskInitProgress(u32 dwHandle, u16 dwTaskId, s32* pwProgress, EDiskInitState *peDiskInitState);

/**
 * @brief  获取扩展磁盘列表
 * @param  dwHandle                       [in]句柄
 *         ptExDiskList         		  [out]外部挂载盘参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetExDiskList(u32 dwHandle, TNetExDiskList *ptExDiskList);

/**
 * @brief  配置导出
 * @param  dwHandle                       [in]句柄
 *         pszFolderPath        		  [in]要导出到的文件夹路径
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_ExportCfg(u32 dwHandle, s8 *pszFolderPath);

/**
 * @brief  配置导入
 * @param  dwHandle                       [in]句柄
 *         pszFilePath           		  [in]要导入的文件全路径，包含文件名
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_ImportCfg(u32 dwHandle, s8 *pszFilePath);

/**
 * @brief  升级准备
 * @param  dwHandle                       [in]句柄
 *         TNetUpgradePrepareParam		  [in]升级准备参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_UpgradePrepare(u32 dwHandle, TNetUpgradePrepareParam tUpgradePrepareParam);

/**
 * @brief  查询升级状态
 * @param  dwHandle                       [in]句柄
 *  	   state						  [out]状态 Idel(空闲，当前设备没有升级)|Upgrade(升级中)|UpgradeSuccess(升级成功)|UpgradeFailure(升级失败)
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_QueryUpgradeState(u32 dwHandle, s8* pState);

/**
 * @brief  恢复出厂状态
 * @param  dwHandle          [in]句柄
 *  	   tSystemFactoryDef [in]恢复出厂参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetSystemFactoryDef(u32 dwHandle, TNetSystemFactoryDef tSystemFactoryDef);

/**
 * @brief  获取恢复出厂信息
 * @param  dwHandle             [in]句柄
 *  	   ptRecoverFactoryInfo	[out]恢复出厂信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRecoverFactoryInfo(u32 dwHandle, TNetRecoverFactoryInfo *ptRecoverFactoryInfo);

/**
 * @brief  获取通道能力
 * @param  dwHandle     [in]句柄
 *  	   tGetChnCap	[in]输入参数
 *  	   ptChnCapList [out]通道能力
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnCap(u32 dwHandle, TNetGetChnCap tGetChnCap, TNetChnCapList *ptChnCapList);

/**
 * @brief  获取通道智能侦测能力
 * @param  dwHandle           [in]句柄
 *  	   nChnId	          [in]通道id
 *  	   ptChnInteDetectCap [out]通道智能侦测能力
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnInteDetectCap(u32 dwHandle, s32 nChnId, TNetChnInteDetectCap *ptChnInteDetectCap);

/**
 * @brief  获取NVR告警输入配置
 * @param  dwHandle         [in]句柄
 *  	   pszAlarmInId   	[in]告警输入号，多个告警输入号使用逗号分割
 *		   ptAlarmInCfgList [out]告警输入配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNvrAlarmInCfg(u32 dwHandle, const s8 *pszAlarmInId, TNetAlarmInCfgList *ptAlarmInCfgList);

/**
 * @brief  设置NVR告警输入配置
 * @param  dwHandle        [in]句柄
 *		   tAlarmInCfgList [in]告警输入配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetNvrAlarmInCfg(u32 dwHandle, TNetAlarmInCfgList tAlarmInCfgList);

/**
 * @brief  获取通道告警输入配置
 * @param  dwHandle         [in]句柄
 *         nChnId           [in]通道id
 *  	   pszAlarmInId 	[in]告警输入号，多个告警输入号使用逗号分割
 *		   ptAlarmInCfgList [out]告警输入配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnAlarmInCfg(u32 dwHandle, s32 nChnId, const s8 *pszAlarmInId, TNetAlarmInCfgList *ptAlarmInCfgList);

/**
 * @brief  设置通道告警输入配置
 * @param  dwHandle        [in]句柄
 *         nChnId          [in]通道id
 *		   tAlarmInCfgList [in]告警输入配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetChnAlarmInCfg(u32 dwHandle, s32 nChnId, TNetAlarmInCfgList tAlarmInCfgList);

/**
 * @brief  获取告警布防时间
 * @param  dwHandle         [in]句柄
 *  	   tAlarmSource		[in]告警源
 *  	   ptAlarmGuardTime [out]告警布防时间
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAlarmGuardTime(u32 dwHandle, TNetAlarmSource tAlarmSource, TNetAlarmGuardTime *ptAlarmGuardTime);

/**
 * @brief  设置告警布防时间
 * @param  dwHandle        [in]句柄
 *  	   tAlarmGuardTime [in]告警布防时间
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetAlarmGuardTime(u32 dwHandle, TNetAlarmGuardTime tAlarmGuardTime);

/**
 * @brief  获取系统告警配置
 * @param  dwHandle      [in]句柄
 *         ptSysAlarmCfg [out]系统告警配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSysAlarmCfg(u32 dwHandle, TNetSysAlarmCfg *ptSysAlarmCfg);

/**
 * @brief  设置系统告警配置
 * @param  dwHandle     [in]句柄
 *  	   tSysAlarmCfg [in]系统告警配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetSysAlarmCfg(u32 dwHandle, TNetSysAlarmCfg tSysAlarmCfg);

/**
 * @brief  告警输入复制
 * @param  dwHandle          [in]句柄
 *  	   tAlarmInCopyParam [in]复制参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_AlarmInCopy(u32 dwHandle, TNetAlarmInCopyParam tAlarmInCopyParam);

/**
 * @brief  获取告警联动配置
 * @param  dwHandle       [in]句柄
 *  	   ptAlarmLinkCfg [in/out]告警联动配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAlarmLinkCfg(u32 dwHandle, TNetAlarmLinkCfg *ptAlarmLinkCfg);

/**
 * @brief  设置告警联动配置
 * @param  dwHandle      [in]句柄
 *  	   tAlarmLinkCfg [in]告警联动配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetAlarmLinkCfg(u32 dwHandle, TNetAlarmLinkCfg tAlarmLinkCfg);

/**
 * @brief  获取告警联动能力
 * @param  dwHandle       [in]句柄
 *  	   ptAlarmLinkCap [out]告警联动能力
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAlarmLinkCap(u32 dwHandle, TNetAlarmLinkCap *ptAlarmLinkCap);

/**    
 * @brief  gbk转utf8
 * @param  pGbk							[in]Gbk字符串
 *  	   pUtf8						[out]Utf8字符串
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GBK2UTF8(const s8 *pGbk, s8 *pUtf8);

/**
 * @brief  utf8转gbk
 * @param  pUtf8						[in]Utg8字符串
 *  	   pGbk							[out]Gbk字符串
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_UTF82GBK(const s8 *pUtf8, s8 *pGbk);

/**
 * @brief  导出通道列表
 * @param  dwHandle            [in]句柄
 *         pDevList			   [out]通道输入列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_ExportDevList(u32 dwHandle, s8* pDevList);

/**
 * @brief  导入通道列表
 * @param  dwHandle            [in]句柄
 *         pDevList			   [in]通道输入列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_ImportDevList(u32 dwHandle, s8* pDevList, s32 *pnErrLine);

/**
 * @brief  获取smtp配置
 * @param  dwHandle     [in]句柄
 *         ptSmtpCfgAll	[out]smtp配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSmtpCfg(u32 dwHandle, TNetSmtpCfgAll *ptSmtpCfgAll);

/**
 * @brief  设置smtp配置
 * @param  dwHandle [in]句柄
 *         tSmtpCfg [in]smtp配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetSmtpCfg(u32 dwHandle, TNetSmtpCfg tSmtpCfg);

/**
 * @brief  开始邮箱测试
 * @param  dwHandle            [in]句柄
 *         tSmtpCfg            [in]smtp服务信息
 *         pnTaskId            [out]邮箱测试任务id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StartSmtpTest(u32 dwHandle, TNetSmtpCfg tSmtpCfg, s32 *pnTaskId);

/**
 * @brief  获取邮箱测试状态
 * @param  dwHandle            [in]句柄
 *         nTaskId             [in]邮箱测试任务id
 *         peSmtpTestStatus    [out]邮箱测试任务状态
 *         pnResult            [out]邮箱测试任务结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSmtpTestStatus(u32 dwHandle, s32 nTaskId, ESmtpTestStatus *peSmtpTestStatus, s32 *pnResult);

/**
 * @brief  停止邮箱测试
 * @param  dwHandle            [in]句柄
 *         nTaskId             [in]邮箱测试任务id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StopSmtpTest(u32 dwHandle, s32 nTaskId);

/**
 * @brief  获取PPPoE配置
 * @param  dwHandle   [in]句柄
 *         ptPppoeCfg [out]PPPoE配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetPppoeCfg(u32 dwHandle, TNetPppoeCfg *ptPppoeCfg);

/**
 * @brief  设置PPPoE配置
 * @param  dwHandle  [in]句柄
 *         tPppoeCfg [out]PPPoE配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetPppoeCfg(u32 dwHandle, TNetPppoeCfg tPppoeCfg);

/**
 * @brief  获取PPPoE状态
 * @param  dwHandle      [in]句柄
 *         ptPppoeStatus [out]PPPoE状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetPppoeStatus(u32 dwHandle, TNetPppoeStatus *ptPppoeStatus);

/**
 * @brief  获取UPnP配置
 * @param  dwHandle  [in]句柄
 *         ptUpnpCfg [out]UPnP配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetUpnpCfg(u32 dwHandle, TNetUpnpCfg *ptUpnpCfg);

/**
 * @brief  设置UPnP配置
 * @param  dwHandle [in]句柄
 *         tUpnpCfg	[in]UPnP配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetUpnpCfg(u32 dwHandle, TNetUpnpCfg tUpnpCfg);

/**
 * @brief  获取DDNS配置
 * @param  dwHandle  [in]句柄
 *         ptDDNSCfg [out]DDNS配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDDNSCfg(u32 dwHandle, TNetDDNSCfg *ptDDNSCfg);

/**
 * @brief  设置DDNS配置
 * @param  dwHandle [in]句柄
 *         tDDNSCfg	[in]DDNS配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetDDNSCfg(u32 dwHandle, TNetDDNSCfg tDDNSCfg);

/**
 * @brief  获取DDNS服务状态
 * @param  dwHandle		[in]句柄
 *         ptDDNSState	[out]服务状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDDNSState(u32 dwHandle, TNetDDNSState *ptDDNSState);

/**
 * @brief  获取SNMP参数
 * @param  dwHandle		[in]句柄
 *         ptNetSnmpCfg [out]Snmp配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSnmpCfg(u32 dwHandle, TNetSnmpCfg *ptNetSnmpCfg);

/**
 * @brief  获取GPS信息
 * @param  dwHandle  [in]句柄
 *         ptGpsInfo [out]GPS信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetGpsInfo(u32 dwHandle, TNetGpsInfo *ptGpsInfo);

/**
 * @brief  设置SNMP参数
 * @param  dwHandle		[in]句柄
 *         tNetSnmpCfg  [in]Snmp配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetSnmpCfg(u32 dwHandle, TNetSnmpCfg tNetSnmpCfg);

/**
 * @brief  消警
 * @param  dwHandle        [in]句柄
 *         tAlarmStateList [in]消警信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_AlarmRemove(u32 dwHandle, TNetAlarmStateList tAlarmStateList);

/**
 * @brief  开始抓包
 * @param  dwHandle            [in]句柄
 *         TNetCaptureStart	   [in]输入参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StartCapturePack(u32 dwHandle, TNetCaptureStart tCapureStart);

/**
 * @brief  停止抓包
 * @param  dwHandle            [in]句柄
 *         TNetCaptureStop	   [out]输出参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StopCapturePack(u32 dwHandle, TNetCaptureStop *ptCaptureStop);

/**
 * @brief  查询抓包状态
 * @param  dwHandle            [in]句柄
 *         ptCaptureState 	   [out]抓包状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCaptureState(u32 dwHandle, TNetCaptureState *ptCaptureState);

/**
 * @brief  开始Ping
 * @param  dwHandle            [in]句柄
 *         TNetPingStart	   [in]ping参数
 *         TaskID			   [out]ptTaskID
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StartPing(u32 dwHandle, TNetPingStart tPingStart, s32 *ptTaskID);

/**
 * @brief  获取Ping结果
 * @param  dwHandle            [in]句柄
 *         nTaskID			   [in]ID
 *         TNetPingStart	   [out]结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetPingResult(u32 dwHandle, s32 nTaskID, TNetPingResultList *ptPingResultList);

/**
 * @brief  停止Ping
 * @param  dwHandle            [in]句柄
 *   	   TaskID			   [in]ID
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StopPing(u32 dwHandle, s32 nTaskID);

/**
 * @brief  获取vsip协议上联配置
 * @param  dwHandle            [in]句柄
 *         TNetBandParam	   [out]输出参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNetVsip(u32 dwHandle, TNetVsipParam *ptNetVsipParam);

/**
 * @brief  设置vsip协议上联配置
 * @param  dwHandle            [in]句柄
 *         TNetBandParam	   [out]输出参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetNetVsip(u32 dwHandle, TNetVsipParam ptNetVsipParam);

/**
 * @brief  获取vsip协议注册平台状态
 * @param  dwHandle    [in]句柄
 *         pbRegStatus [out]注册状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetVsipRegStatus(u32 dwHandle, BOOL32 *pbRegStatus);

/**
 * @brief  获取gb28181协议配置
 * @param  dwHandle           [in]句柄
 *	       TNetGBInfo		  [in]输入参数
 *	       TNetGBParam		  [out]输出参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNetGB28181(u32 dwHandle, TNetGB28181Info tNetGBInfo, TNetGB28181Param *ptNetGBParam);

/**
 * @brief  设置gb28181协议配置
 * @param  dwHandle           [in]句柄
 *	       nPlatIndex		  [in]平台索引
 *	       TNetGBParam		  [out]输出参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetNetGB28181(u32 dwHandle, s32 nPlatIndex, TNetGB28181Param tNetGBParam);

/**
 * @brief  获取gb28181协议注册平台状态
 * @param  dwHandle    [in]句柄
 *	       nPlatIndex  [in]平台索引
 *	       pbRegStatus [out]注册状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetGB28181RegStatus(u32 dwHandle, s32 nPlatIndex, BOOL32 *pbRegStatus);

/**
 * @brief  获取Onvif协议配置
 * @param  dwHandle           [in]句柄
 *	       ptOnvifParam	  [out]输出参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNetOnvif(u32 dwHandle, TNetOnvifParam *ptOnvifParam);

/**
 * @brief  设置Onvif协议配置
 * @param  dwHandle           [in]句柄
 *	       tOnvifParam	  [in]参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetNetOnvif(u32 dwHandle, TNetOnvifParam tOnvifParam);

/**
 * @brief  获取视图库协议注册平台状态
 * @param  dwHandle    [in]句柄
 *	       nPlatIndex  [in]平台索引
 *	       pbRegStatus [out]注册状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetPubSecRegStatus(u32 dwHandle, s32 nPlatIndex, BOOL32 *pbRegStatus);

/**
 * @brief  获取视图库协议上联配置
 * @param  dwHandle           [in]句柄
 *	       nPlatIndex		  [in]视图库平台索引
 *	       ptPubSecAppCfg	  [out]视图库协议上联配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetPubSecCfg(u32 dwHandle, s32 nPlatIndex, TNetPubSecCfg *ptPubSecAppCfg);

/**
 * @brief  设置视图库协议上联配置
 * @param  dwHandle           [in]句柄
 *	       nPlatIndex		  [in]视图库平台索引
 *	       tPubSecAppCfg	  [in]视图库协议上联配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetPubSecCfg(u32 dwHandle, s32 nPlatIndex, TNetPubSecCfg tPubSecAppCfg);

/**
 * @brief  获取云服务配置
 * @param  dwHandle   [in]句柄
 *	       ptCloudCfg [out]云服务配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCloudCfg(u32 dwHandle, TNetCloudCfg *ptCloudCfg);

/**
 * @brief  设置云服务配置
 * @param  dwHandle [in]句柄
 *	       bEnable  [in]是否启用云服务
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetCloudCfg(u32 dwHandle, BOOL32 bEnable);

/**
 * @brief  获取云服务状态
 * @param  dwHandle      [in]句柄
 *	       peCloudStatus [out]云服务状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCloudStatus(u32 dwHandle, ECloudStatus *peCloudStatus);

/**
 * @brief  获取国标下联控制信息
 * @param  dwHandle        [in]句柄
 *	       pGB28181CtlInfo [out]国标下联控制信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetGB28181CtlInfo(u32 dwHandle, TNetGB28181CtlInfo *ptGB28181CtlInfo);

/**
 * @brief  设置国标下联控制信息
 * @param  dwHandle        [in]句柄
 *	       tGB28181CtlInfo [out]国标下联控制信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetGB28181CtlInfo(u32 dwHandle, TNetGB28181CtlInfo tGB28181CtlInfo);

/**
 * @brief  获取视图库下联配置
 * @param  dwHandle       [in]句柄
 *	       ptPubSecCltCfg [out]视图库下联配置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetPubSecCltCfg(u32 dwHandle, TNetPubSecCltCfg *ptPubSecCltCfg);

/**
 * @brief  设置视图库下联配置
 * @param  dwHandle      [in]句柄
 *	       tPubSecCltCfg [in]视图库下联配置信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetPubSecCltCfg(u32 dwHandle, TNetPubSecCltCfg tPubSecCltCfg);

/**
 * @brief  获取通道告警配置(移动侦测)
 * @param  dwHandle          [in]句柄
 *		   tGetChnAlarmCfg   [in]获取参数
 *		   ptChnAlarmCfgList [out]通道告警配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnAlarmCfg(u32 dwHandle, TNetGetChnAlarmCfg tGetChnAlarmCfg, TNetChnAlarmCfgList *ptChnAlarmCfgList);

/**
 * @brief  设置通道告警配置(移动侦测)
 * @param  dwHandle     [in]句柄
 *		   tChnAlarmCfg [in]通道告警配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetChnAlarmCfg(u32 dwHandle, TNetChnAlarmCfg tChnAlarmCfg);

/**
 * @brief  获取通道视频源丢失告警配置
 * @param  dwHandle          [in]句柄
 *		   nChnId            [in]通道
 *		   pbEnable          [out]是否启用
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnVideoLostAlarmCfg(u32 dwHandle, s32 nChnId, BOOL32 *pbEnable);

/**
 * @brief  设置通道视频源丢失告警配置
 * @param  dwHandle          [in]句柄
 *		   nChnId            [in]通道
 *		   pbEnable          [in]是否启用
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetChnVideoLostAlarmCfg(u32 dwHandle, s32 nChnId, BOOL32 bEnable);

/**
 * @brief  获取智能侦测-遮挡检测配置
 * @param  dwHandle          [in]句柄
 *		   ptNetChnAlarmCfg	 [in/out]遮挡检测配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnShiledDetectCfg(u32 dwHandle, TNetChnAlarmCfg *ptNetChnAlarmCfg);

/**
 * @brief  设置智能侦测-遮挡检测配置
 * @param  dwHandle     [in]句柄
 *		   tChnAlarmCfg [in]通道告警配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetChnShiledDetectCfg(u32 dwHandle, TNetChnAlarmCfg tChnAlarmCfg);

/**
 * @brief  获取NVR告警状态
 * @param  dwHandle         [in]句柄
 *         ptAlarmStateList	[out]告警状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNvrAlarmState(u32 dwHandle, TNetAlarmStateList *ptAlarmStateList);

/**
 * @brief  获取通道告警状态
 * @param  dwHandle          [in]句柄
 *         tGetChnAlarmState [in]输入参数
 *         ptAlarmStateList	 [out]告警状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnAlarmState(u32 dwHandle, TNetGetChnAlarmState tGetChnAlarmState, TNetAlarmStateList *ptAlarmStateList);

/**
 * @brief  硬盘SMART检测
 * @param  dwHandle               [in]句柄
 *         nDiskId                [in]硬盘ID
 *         ptDiskSmartTestInfo	  [out]输出参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DiskSmartTest(u32 dwHandle, s32 nDiskId, TNetDiskSmartTestInfo *ptDiskSmartTestInfo);

/**
 * @brief  获取硬盘结构信息
 * @param  dwHandle               [in]句柄
 *         nDiskId                [in]硬盘ID
 *         ptDiskGeometryInfo	  [out]输出参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDiskGeometryInfo(u32 dwHandle, s32 nDiskId, TNetDiskGeometryInfo *ptDiskGeometryInfo);

/**
 * @brief  获取坏道检测任务信息
 * @param  dwHandle                   [in]句柄
 *         ptBadSectorCheckTaskInfo	  [out]输出参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetBadSectorCheckTaskInfo(u32 dwHandle, TNetBadSectorCheckTaskInfo *ptBadSectorCheckTaskInfo);

/**
 * @brief  创建坏道检测任务信息
 * @param  dwHandle               [in]句柄
 *         nDiskId                [in]硬盘ID
 *         eBadSectorCheckType    [in]坏道检测类型
 *         pTaskId          	  [out]任务ID
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreateBadSectorCheckTask(u32 dwHandle, s32 nDiskId, EBadSectorCheckType eBadSectorCheckType, s32 *pnTaskId);

/**
 * @brief  坏道检测任务控制
 * @param  dwHandle                   [in]句柄
 *         nTaskId                    [in]任务ID
 *         eBadSectorCheckCtrlType    [in]坏道检测任务控制类型
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_BadSectorCheckTaskCtrl(u32 dwHandle, s32 nTaskId, EBadSectorCheckCtrlType eBadSectorCheckCtrlType);

/**
 * @brief  停止坏道检测任务
 * @param  dwHandle               [in]句柄
 *         nTaskId                [in]任务ID
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StopBadSectorCheckTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  获取坏道检测信息
 * @param  dwHandle               [in]句柄
 *         nTaskId                [in]任务ID
 *         ullStartPos            [in]起始位置
 *         ptBadSectorCheckInfo   [out]返回数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetBadSectorCheckResult(u32 dwHandle, s32 nTaskId, u64 ullStartPos, TNetBadSectorCheckInfo *ptBadSectorCheckInfo);

/**
 * @brief  获取网络带宽
 * @param  dwHandle               [in]句柄
 *         TNetBandParam		  [out]输出参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNetBand(u32 dwHandle, TNetBandParam *ptNetBandParam);

/**
 * @brief  获取自动维护配置
 * @param  dwHandle         [in]句柄
 *         ptAutoServiceCfg [out]自动维护配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAutoServiceCfg(u32 dwHandle, TNetAutoServiceCfg *ptAutoServiceCfg);

/**
 * @brief  设置自动维护配置
 * @param  dwHandle        [in]句柄
 *         tAutoServiceCfg [in]自动维护配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetAutoServiceCfg(u32 dwHandle, TNetAutoServiceCfg tAutoServiceCfg);

/**
 * @brief  获取高级配置
 * @param  dwHandle           [in]句柄
 *	       TNetAdvancedInfo	  [in]输入参数
 *	       TNetAdvancedParam  [out]输出参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNetAdvancedCfg(u32 dwHandle, TNetNetAdvancedParam *ptAdvancedParam);

/**
 * @brief  设置高级配置
 * @param  dwHandle           [in]句柄
 *	       TNetAdvancedParam  [out]输出参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetNetAdvancedCfg(u32 dwHandle, TNetNetAdvancedParam tAdvancedParam);

/**
 * @brief  获取系统高级配置
 * @param  dwHandle         [in]句柄
 *	       ptSysAdvancedCfg [out]系统高级配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSysAdvancedCfg(u32 dwHandle, TNetSysAdvancedCfg *ptSysAdvancedCfg);

/**
 * @brief  设置系统高级配置
 * @param  dwHandle        [in]句柄
 *	       tSysAdvancedCfg [in]系统高级配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetSysAdvancedCfg(u32 dwHandle, TNetSysAdvancedCfg tSysAdvancedCfg);

/**
 * @brief  获取NVR通知事件
 * @param  dwHandle      [in]句柄
 *	       ptNvrNtyEvent [out]NVR通知事件信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNvrNtyEvent(u32 dwHandle, TNetNvrNtyEvent *ptNvrNtyEvent);

/**
 * @brief  透明通道
 * @param  dwHandle           [in]句柄
 *	       pInBuf             [in]输入缓冲区
 *	       nInBufLen          [in]输入缓冲区大小
 *	       pOutBuf            [out]输出缓冲区
 *	       pnOutLen           [in/out]传入输出缓冲区大小，传出实际接收大小
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_TransparentChn(u32 dwHandle, s8 *pInBuf, s32 nInBufLen, s8 *pOutBuf, s32 *pnOutLen);

/**
 * @brief  获取自定义配置
 * @param  dwHandle [in]句柄
 *	       pszKey   [in]配置项名称
 *	       pszVal   [out]配置项值
 *	       nSize    [in]pszVal缓冲区大小
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCustomCfg(u32 dwHandle, const s8 *pszKey, s8 *pszVal, s32 nSize);

/**
 * @brief  设置自定义配置
 * @param  dwHandle [in]句柄
 *	       pszKey   [in]配置项名称
 *	       pszVal   [in]配置项值
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetCustomCfg(u32 dwHandle, const s8 *pszKey, const s8 *pszVal);

/**
 * @brief  获取网络实时状态
 * @param  dwHandle            [in]句柄
 *	       ptNetRealTimeStatus [out]网络实时状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNetRealTimeStatus(u32 dwHandle, TNetNetRealTimeStatus *ptNetRealTimeStatus);

/**
 * @brief  获取AIU通道能力
 * @param  dwHandle        [in]句柄
 *         pszChnId        [in]通道id，多个通道使用逗号分割，例如：1,2,3,4，最多32个通道
 *	       ptAiuChnCapList [out]AIU通道能力
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAiuChnCap(u32 dwHandle, const s8 *pszChnId, TNetAiuChnCapList *ptAiuChnCapList);

/**
 * @brief  获取近期智能抓拍图片列表
 * @param  dwHandle                 [in]句柄
 *         tGetAiuRecentSnapPicList [in]获取参数
 *	       ptAiuSnapPicList         [out]抓拍图片列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAiuRecentSnapPicList(u32 dwHandle, TNetGetAiuRecentSnapPicList tGetAiuRecentSnapPicList, TNetAiuSnapPicList *ptAiuSnapPicList);

/**
 * @brief  开启事件录像
 * @param  dwHandle       [in]句柄
 *         tStartEventRec [in]开启事件录像参数
 *	       pnRecId        [out]录像id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StartEventRec(u32 dwHandle, TNetStartEventRec tStartEventRec, s32 *pnRecId);

/**
 * @brief  设置事件录像信息
 * @param  dwHandle      [in]句柄
 *         tEventRecInfo [in]事件录像信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetEventRecInfo(u32 dwHandle, TNetEventRecInfo tEventRecInfo);

/**
 * @brief  获取手动事件配置
 * @param  dwHandle          [in]句柄
 *         ptManualEventList [out]手动事件信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetManualEventInfo(u32 dwHandle, TNetManualEventList *ptManualEventList);

/**
 * @brief  设置手动事件配置
 * @param  dwHandle          [in]句柄
 *         tManualEventList  [in]手动事件信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetManualEventInfo(u32 dwHandle, TNetManualEventList tManualEventList);

/**
 * @brief  获取手动事件能力
 * @param  dwHandle          [in]句柄
 *         ptManualEventCap  [out]手动事件能力
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetManualEventCap(u32 dwHandle, TNetManualEventCap *ptManualEventCap);

/**
 * @brief  手动触发通道智能处理
 * @param  dwHandle             [in]句柄
 *         tAiuManualTrigParam  [in]触发参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_AiuManualTrig(u32 dwHandle, TNetAiuManualTrigParam tAiuManualTrigParam);

/**
 * @brief  获取布控库能力
 * @param  dwHandle     [in]句柄
 *         ptCtrlLibCap [out]布控库能力
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCtrlLibCap(u32 dwHandle, TNetCtrlLibCap *ptCtrlLibCap);

/**
 * @brief  获取布控库列表
 * @param  dwHandle      [in]句柄
 *         ptCtrlLibList [out]布控库列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCtrlLibList(u32 dwHandle, TNetCtrlLibList *ptCtrlLibList);

/**
 * @brief  新建布控库
 * @param  dwHandle     [in]句柄
 *         tCtrlLibItem [in]布控库信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreateCtrlLib(u32 dwHandle, TNetCtrlLibItem tCtrlLibItem);

/**
 * @brief  删除布控库
 * @param  dwHandle       [in]句柄
 *         pszCtrlLibName [in]布控库名称
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DelCtrlLib(u32 dwHandle, const s8 *pszCtrlLibName);

/**
 * @brief  创建编辑布控库任务
 * @param  dwHandle          [in]句柄
 *         pszOldCtrlLibName [in]原布控库名称
 *         tCtrlLibItem      [in]新布控库信息
 *         pnTaskId          [out]任务id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreateEditCtrlLibTask(u32 dwHandle, const s8 *pszOldCtrlLibName, TNetCtrlLibItem tCtrlLibItem, s32 *pnTaskId);

/**
 * @brief  获取编辑布控库状态
 * @param  dwHandle           [in]句柄
 *         nTaskId            [in]任务id
 *         ptEditCtrlLibState [out]编辑布控库状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetEditCtrlLibState(u32 dwHandle, s32 nTaskId, TNetEditCtrlLibState *ptEditCtrlLibState);

/**
 * @brief  销毁编辑布控库任务
 * @param  dwHandle [in]句柄
 *         nTaskId  [in]任务id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroyEditCtrlLibTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  搜索人脸库
 * @param  dwHandle         [in]句柄
 *         tSearchPersonLib [in]搜索条件
 *         ptPersonList     [out]搜索结果
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SearchPersonLib(u32 dwHandle, TNetSearchPersonLib tSearchPersonLib, TNetPersonList *ptPersonList);

/**
 * @brief  添加人员
 * @param  dwHandle       [in]句柄
 *         pszCtrlLibName [in]布控库名称
 *         tPersonItem    [in]人员信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_AddPerson(u32 dwHandle, const s8 *pszCtrlLibName, TNetPersonItem tPersonItem);

/**
 * @brief  编辑人员
 * @param  dwHandle       [in]句柄
 *         pszCtrlLibName [in]布控库名称
 *         tPersonItem    [in]人员信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_EditPerson(u32 dwHandle, const s8 *pszCtrlLibName, TNetPersonItem tPersonItem);

/**
 * @brief  删除人员
 * @param  dwHandle   [in]句柄
 *         tDelPerson [in]删除的人员
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DelPerson(u32 dwHandle, TNetDelPerson tDelPerson);

/**
 * @brief  删除全部人员
 * @param  dwHandle			[in]句柄
 *         pszCtrlLibName	[in]布控库名
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DelAllPerson(u32 dwHandle, s8 *pszCtrlLibName);

/**
 * @brief  根据路径获取图片数据
 * @param  dwHandle   [in]句柄
 *         pPicPath	  [in]图片路径
 *         ptNetGetPicData [out]图片数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetPicData(u32 dwHandle, const s8* pszPicPath, TNetPicData *ptNetGetPicData);

/**
 * @brief  布控库导出准备
 * @param  dwHandle				[in]句柄
 *         tNetCtrlLibExport	[in]导出参数
 *		   pdwNeedSpace			[out]导出所需的大小
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CtrlLibExportPrepare(u32 dwHandle, TNetCtrlLibExport tNetCtrlLibExport, u32 *pdwNeedSpace);

/**
 * @brief  布控库导出生成导出文件
 * @param  dwHandle				[in]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CtrlLibExportCreateFile(u32 dwHandle, TNetCtrlLibExportFlieList *ptCtrlLibExportFlieList);

/**
 * @brief  获取布控库导出文件(异步)
 * @param  dwHandle   [in]句柄
 *         pszFileName[in]文件名
 *         pfCallBack [in]回调函数
 *         pParam     [in]回调函数参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CtrlLibExportGetFileAsync(u32 dwHandle, s8 *pszFileName, pfAPICallBack pfCallBack, void *pParam);

/**
 * @brief  获取布控库导出文件
 * @param  dwHandle   [in]句柄
 *         pszFileName[in]文件名
 *         pfCallBack [in]回调函数
 *         pParam     [in]回调函数参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CtrlLibExportGetFile(u32 dwHandle, s8 *pszFileName, pfAPICallBack pfCallBack, void *pParam);

/**
 * @brief 布控库导出获取状态
 * @param  dwHandle				[in]句柄
 *         peCtrlLibExportState	[out]导出状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCtrlLibExportState(u32 dwHandle, ECtrlLibExportState *peCtrlLibExportState);

/**
 * @brief  销毁布控库导出任务
 * @param  dwHandle				[in]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroyCtrlLibExportTask(u32 dwHandle);

/**
 * @brief  布控库导入准备
 * @param  dwHandle				[in]句柄
 *         dwTotalSize			[in]导入文件大小
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CtrlLibImportPrepare(u32 dwHandle, u32 dwTotalSize);

/**
 * @brief  布控库导入获取状态
 * @param  dwHandle				[in]句柄
 *         ptCtrlLibExportState	[out]导入状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCtrlLibImportState(u32 dwHandle, TNetCtrlLibImportState *ptCtrlLibExportState);

/**
 * @brief  布控库导入上传数据
 * @param  dwHandle				[in]句柄
 *         tNetCtrlLibImportSend[in]数据参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CtrlLibImportSend(u32 dwHandle, TNetCtrlLibImportSend tNetCtrlLibImportSend);

/**
 * @brief  开始导入布控库
 * @param  dwHandle				[in]句柄
 *         tCtrlLibImportParam	[in]导入参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StartCtrlLibImport(u32 dwHandle, TNetCtrlLibImportParam tCtrlLibImportParam);

/**
 * @brief  布控库获取导入错误信息
 * @param  dwHandle				[in]句柄
 * @param  tCtrlLibErrInfo		[in]参数
 * @param  ptCtrlLibErrList		[out]错误列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCtrlLibImportErrInfo(u32 dwHandle, TNetCtrlLibErrInfo tCtrlLibErrInfo, TNetCtrlLibErrList *ptCtrlLibErrList);

/**
 * @brief  销毁布控库导入任务
 * @param  dwHandle				[in]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroyCtrlLibImportTask(u32 dwHandle);

/**
 * @brief  布控库复制
 * @param  dwHandle				[in]句柄
 *         tCtrlLibCopy			[in]复制参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CtrlLibCopy(u32 dwHandle, TNetCtrlLibCopy tCtrlLibCopy);

/**
 * @brief  布控库复制获取状态
 * @param  dwHandle				[in]句柄
 *         peState				[out]状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCtrlLibCopyState(u32 dwHandle, ECtrlLibImportState *peState);

/**
 * @brief  布控库复制获取错误信息
 * @param  dwHandle				[in]句柄
 *         ptCtrlLibCopyErrList	[out]错误信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCtrlLibCopyErrInfo(u32 dwHandle, TNetCtrlLibCopyErrList *ptCtrlLibCopyErrList);

/**
 * @brief  销毁布控库复制
 * @param  dwHandle				[in]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroyCtrlLibCopyTask(u32 dwHandle);

/**
 * @brief  获取布控库批量剩余添加人数
 * @param  dwHandle				[in]句柄
 * @param  pdwFreeMemNum		[out]剩余添加人数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetBatchCtrlLibFreeMemNum(u32 dwHandle, u32 *pdwFreeMemNum);

/**
 * @brief  布控库批量添加获取是否有续传任务
 * @param  dwHandle						[in]句柄
 * @param  ptCtrlLibBatchPrepare		[in]续传任务参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetBatchCtrlLibContinueTask(u32 dwHandle, TNetCtrlLibBatchPrepare *ptCtrlLibBatchPrepare);

/**
 * @brief  布控库批量添加准备
 * @param  dwHandle					[in]句柄
 * @param  tCtrlLibBatchPrepare		[in]准备参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_BatchCtrlLibPrepare(u32 dwHandle, TNetCtrlLibBatchPrepare tCtrlLibBatchPrepare);

/**
 * @brief  布控库批量添加开始
 * @param  dwHandle				[in]句柄
 * @param  tCtrlLibBatch		[in]任务参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_StartCtrlLibBatch(u32 dwHandle, TNetCtrlLibBatch tCtrlLibBatch);

/**
 * @brief  布控库批量添加获取状态
 * @param  dwHandle				[in]句柄
 * @param  ptCtrlLibBatchState	[out]状态
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCtrlLibBatchState(u32 dwHandle, TNetCtrlLibBatchState *ptCtrlLibBatchState);

/**
 * @brief  布控库批量添加获取错误信息
 * @param  dwHandle				[in]句柄
 * @param  tCtrlLibErrInfo		[in]参数
 * @param  ptCtrlLibErrList		[out]错误列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCtrlLibBatchErrInfo(u32 dwHandle, TNetCtrlLibErrInfo tCtrlLibErrInfo, TNetCtrlLibErrList *ptCtrlLibErrList);

/**
 * @brief  批量添加取消上次的续传任务
 * @param  dwHandle				[in]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CancelContinueCtrlLibBatchTask(u32 dwHandle);

/**
 * @brief  销毁布控库批量添加任务
 * @param  dwHandle				[in]句柄
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroyCtrlLibBatchTask(u32 dwHandle);

/**
 * @brief  获取算法引擎能力
 * @param  dwHandle				[in]句柄
 *         ptNetAlgEngineCap	[out]算法引擎能力
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAlgEngineCap(u32 dwHandle, TNetAlgEngineCap *ptNetAlgEngineCap);

/**
 * @brief  获取算法引擎列表
 * @param  dwHandle      [in]句柄
 *         ptCtrlLibList [out]算法引擎列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAlgEngineList(u32 dwHandle, TNetAlgEngineList *ptNetAlgEngineList);

/**
 * @brief  创建智能算法加载任务
 * @param  dwHandle				[in]句柄
 *         dwAlgEngineSize		[in]算法引擎大小
 *		   pnTaskId				[out]任务id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreateLoadAlgEngineTask(u32 dwHandle, u32 dwAlgEngineSize, s32 *pnTaskId);

/**
 * @brief  销毁智能算法加载任务
 * @param  dwHandle				[in]句柄
 *         nTaskId				[in]任务id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroyLoadAlgEngineTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  上传智能算法加载数据
 * @param  dwHandle				[in]句柄
 *         tNetAlgEngineUpload	[in]智能算法加载数据
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_UploadAlgEngine(u32 dwHandle, TNetAlgEngineUpload tNetAlgEngineUpload);

/**
 * @brief  创建智能算法卸载任务
 * @param  dwHandle				[in]句柄
 *         pszAlgEngineName		[in]算法引擎名
 *		   pnTaskId				[out]任务id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreateUnloadAlgEngineTask(u32 dwHandle, s8 *pszAlgEngineName, s32 *pnTaskId);

/**
 * @brief  获取智能算法卸载进度
 * @param  dwHandle				[in]句柄
 *         nTaskId				[in]任务id
 *		   pbFinish				[out]是否结束
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetUnloadAlgEngineProgress(u32 dwHandle, s32 nTaskId, BOOL32 *pbFinish);

/**
 * @brief  销毁智能算法卸载任务
 * @param  dwHandle				[in]句柄
 *         nTaskId				[in]任务id
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DestroyUnloadAlgEngineTask(u32 dwHandle, s32 nTaskId);

/**
 * @brief  获取NVR的AIU能力
 * @param  dwHandle				[in]句柄
 *         ptNetNvrAiuCap		[out]AIU能力
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNvrAiuCap(u32 dwHandle, TNetNvrAiuCap *ptNetNvrAiuCap);

/**
 * @brief  获取NVR的VAPData能力
 * @param  dwHandle					[in]句柄
 *         ptNetNvrVAPDataCap		[out]VAPData能力
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetNvrVAPDataCap(u32 dwHandle, TNetNvrVAPDataCap *ptNetNvrVAPDataCap);

/**
 * @brief  获取AIS通道能力
 * @param  dwHandle					[in]句柄
 *         ptNetAisChnCap			[in/out]通道能力
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAisChnCap(u32 dwHandle, TNetAisChnCap *ptNetAisChnCap);

/**
 * @brief  获取通道视音图大数据配置
 * @param  dwHandle				[in]句柄
 *         tNetGetAiuBigDataCfg	[in]获取参数
 *		   ptNetAiuBigDataCfgList[out]配置参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAisVAPDataCfg(u32 dwHandle, TNetGetAiuBigDataCfg tNetGetAiuBigDataCfg, TNetAiuBigDataCfgList *ptNetAiuBigDataCfgList);

/**
 * @brief  设置通道视音图大数据配置
 * @param  dwHandle				[in]句柄
 *         tNetAiuBigDataCfg	[in]设置参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetAisVAPDataCfg(u32 dwHandle, TNetAiuBigDataCfg tNetAiuBigDataCfg);

/**
 * @brief  获取智能处理相关能力
 * @param  dwHandle				[in]句柄
 *         ptNetAiProcessCap	[out]智能处理能力
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAiProcessCap(u32 dwHandle, TNetAiProcessCap *ptNetAiProcessCap);

/**
 * @brief  获取通道检测算法默认参数
 * @param  dwHandle					[in]句柄
 *         ptDetectAlgDefaultParam	[out]通道检测算法默认参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnDetectAlgDefParam(u32 dwHandle, TNetDetectAlgDefaultParam *ptDetectAlgDefaultParam);

/**
 * @brief  获取智能通道列表
 * @param  dwHandle					[in]句柄
 *         tIntellChnParam			[in]参数
 *         ptIntellChnParam			[out]智能通道列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetIntellChnList(u32 dwHandle, TNetIntellChnParam tIntellChnParam, TNetIntellChnList *ptIntellChnList);

/**
 * @brief  设置智能通道列表
 * @param  dwHandle					[in]句柄
 *         tIntellChnList			[in]智能通道列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetIntellChnList(u32 dwHandle, TNetIntellChnList tIntellChnList);

/**
 * @brief  获取智能通道检测算法配置列表
 * @param  dwHandle					[in]句柄
 *         ptNetChnDetectAlgList	[out]配置列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetAipDetectChnAlgList(u32 dwHandle, TNetChnDetectAlgList *ptNetChnDetectAlgList);

/**
 * @brief  删除智能通道对应算法配置信息
 * @param  dwHandle					[in]句柄
 *         tNetDelChnDetectAlg		[in]通道对应算法配置
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DelChnDetectAlgCfg(u32 dwHandle, TNetDelChnDetectAlg tNetDelChnDetectAlg);

/**
 * @brief  获取通道检测算法参数
 * @param  dwHandle					[in]句柄
 *         tNetDelChnDetectAlg		[in]通道对应算法配置
 *		   ptNetChnDetectAlgParam   [out]通道检测算法参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetChnDetectAlgParam(u32 dwHandle, TNetGetChnDetectAlgParam tNetGetChnDetectAlg, TNetChnDetectAlgParam *ptNetChnDetectAlgParam);

/**
 * @brief  设置通道检测算法参数
 * @param  dwHandle					[in]句柄
 *         tNetChnDetectAlgParam	[in]通道检测算法参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetChnDetectAlgParam(u32 dwHandle, TNetChnDetectAlgParam tNetChnDetectAlgParam);

/**
 * @brief  获取通道行人检测布防时间时间
 * @param  dwHandle					[in]句柄
 *		   nChnId					[in]通道号
 *		   ptNetCompareRuleTime     [out]布防时间
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetPersonDetectTime(u32 dwHandle, s32 nChnId, TNetCompareRuleTime *ptNetCompareRuleTime);

/**
 * @brief  设置通道行人检测布防时间时间
 * @param  dwHandle					[in]句柄
 *		   nChnId					[in]通道号
 *		   ptNetCompareRuleTime     [in]布防时间
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetPersonDetectTime(u32 dwHandle, s32 nChnId, TNetCompareRuleTime tNetCompareRuleTime);

/**
 * @brief  获取通道行人检测联动方式
 * @param  dwHandle					[in]句柄
 *		   nChnId					[in]通道号
 *		   ptNetRuleLinkCfg			[out]联动方式
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetPersonDetectLinkMode(u32 dwHandle,  s32 nChnId, TNetRuleLinkCfg *ptNetRuleLinkCfg);

/**
 * @brief  设置通道行人检测联动方式
 * @param  dwHandle					[in]句柄
 *		   nChnId					[in]通道号
 *		   tNetRuleLinkCfg			[in]联动方式
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetPersonDetectLinkMode(u32 dwHandle,  s32 nChnId, TNetRuleLinkCfg tNetRuleLinkCfg);

/**
 * @brief  获取比对已经创建比对规则信息
 * @param  dwHandle					[in]句柄
 *		   ptNetCompareRuleList		[out]比对规则信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCompareRuleList(u32 dwHandle, TNetCompareRuleList *ptNetCompareRuleList);

/**
 * @brief  获取默认比对规则配置
 * @param  dwHandle					[in]句柄
 *		   ptCompareRule			[out]比对规则信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetDefaultCompareRule(u32 dwHandle, TNetCompareRule *ptCompareRule);

/**
 * @brief  新建比对规则
 * @param  dwHandle					[in]句柄
 *		   tNetCompareRule			[in/out]比对规则信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_CreateCompareRule(u32 dwHandle, TNetCompareRule *ptNetCompareRule);

/**
 * @brief  删除比对规则
 * @param  dwHandle					[in]句柄
 *		   nIndex					[in]比对规则索引
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_DelCompareRule(u32 dwHandle, s32 nIndex);

/**
 * @brief  编辑比对规则
 * @param  dwHandle					[in]句柄
 *		   tNetCompareRule			[in]比对规则信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_EditCompareRule(u32 dwHandle, TNetCompareRule *ptNetCompareRule);

/**
 * @brief  获取比对规则布防时间
 * @param  dwHandle					[in]句柄
 *		   nIndex					[in]比对规则索引
 *		   ptNetCompareRuleTime     [out]布防时间
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCompareRuleTime(u32 dwHandle, s32 nIndex, TNetCompareRuleTime *ptNetCompareRuleTime);

/**
 * @brief  设置比对规则布防时间
 * @param  dwHandle					[in]句柄
 *		   nIndex					[in]比对规则索引
 *		   ptNetCompareRuleTime     [in]布防时间
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetCompareRuleTime(u32 dwHandle, s32 nIndex, TNetCompareRuleTime tNetCompareRuleTime);

/**
 * @brief  获取比对规则联动方式
 * @param  dwHandle					[in]句柄
 *		   ptNetRuleLinkCfg			[in/out]比对规则索引/联动方式
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetCompareRuleLinkMode(u32 dwHandle, TNetRuleLinkCfg *ptNetRuleLinkCfg);

/**
 * @brief  设置比对规则联动方式
 * @param  dwHandle					[in]句柄
 *		   tNetRuleLinkCfg			[out]联动方式
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetCompareRuleLinkMode(u32 dwHandle, TNetRuleLinkCfg tNetRuleLinkCfg);

/**
 * @brief  人员抓拍检索
 * @param  dwHandle					[in]句柄
 *		   tFaceSnapCfg				[in]搜索条件
 *		   ptFaceSnapList			[out]人员抓拍列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_FaceSnapSearch(u32 dwHandle, TNetFaceSnapSearch tFaceSnapCfg, TNetFaceSnapList *ptFaceSnapList);

/**
 * @brief  日志报警检索
 * @param  dwHandle					[in]句柄
 *		   tLogSearch				[in]搜索条件
 *		   ptAlarmLogList			[out]日志报警检索列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_LogAlarmSearch(u32 dwHandle, TNetLogAlarmSearch tLogSearch, TNetLogAlarmList *ptAlarmLogList);

/**
 * @brief  行人抓拍检索
 * @param  dwHandle					[in]句柄
 *		   tPersonSnapCfg			[in]搜索条件
 *		   ptFacePersonList			[out]人员抓拍列表
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_PersonSnapSearch(u32 dwHandle, TNetPersonSnapSearch tPersonSnapCfg, TNetPersonSnapList *ptFacePersonList);

/**
 * @brief  获取短视频参数
 * @param  dwHandle          [in]句柄
 *		   ptSmallVideoParam [out]短视频参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSmallVideoParam(u32 dwHandle, TNetSmallVideoParam *ptSmallVideoParam);

/**
 * @brief  获取抓拍配置参数
 * @param  dwHandle			[in]句柄
 * @param  ptNetSnapCfg     [out]抓拍配置参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetSnapCfg(u32 dwHandle, TNetSnapCfg *ptSnapCfg);

/**
 * @brief  设置抓拍配置参数
 * @param  dwHandle			[in]句柄
 * @param  tSnapCfg			[in]配置参数
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SetSnapCfg(u32 dwHandle, TNetSnapCfg tSnapCfg);

/**
 * @brief  采集连续抓拍
 * @param  dwHandle			[in]句柄
 * @param  tSeriesSnap		[in]采集连续抓拍参数
 * @param  pnId				[out]标识索引
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_SeriesSnap(u32 dwHandle, TNetSeriesSnap tSeriesSnap, s32 *pnId);

/**
 * @brief  获取近期检测抓拍列表
 * @param  dwHandle		    [in]句柄
 *         pszChnId         [in]通道id，多个通道使用逗号分割，例如：1,2,3,4
 *         eAlgType         [in]检测类型
 *         ptDetectSnapList [out]检测抓拍信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRecentDetectSnapList(u32 dwHandle, const s8 *pszChnId, EAlgType eAlgType, TNetDetectSnapList *ptDetectSnapList);

/**
 * @brief  获取近期比对报警列表
 * @param  dwHandle		  [in]句柄
 *         ptCmpAlarmList [out]比对报警信息
 * @return 成功返回NET_OK，失败返回错误码
 */
NVR_API s32 STDCALL NET_GetRecentCmpAlarmList(u32 dwHandle, TNetCmpAlarmList *ptCmpAlarmList);

#ifdef __cplusplus
}
#endif

#endif