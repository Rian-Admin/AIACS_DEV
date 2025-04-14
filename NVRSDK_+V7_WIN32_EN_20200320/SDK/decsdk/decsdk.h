#ifndef _DECSDK_H_
#define _DECSDK_H_

#include "decsdkerrno.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * decsdk类型定义
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

#define DEC_MAXLEN_8							8
#define DEC_MAXLEN_16							16
#define DEC_MAXLEN_32							32
#define DEC_MAXLEN_64							64
#define DEC_MAXLEN_128							128
#define DEC_MAXLEN_256							256
#define DEC_MAXLEN_512							512


/**
 * 枚举定义
 */

enum ESyncAudioType
{
	SYNCAUDIOTYPE_RTP = 1,			                   ///< 使用RTP同步
	SYNCAUDIOTYPE_NTP = 2,		                       ///< 使用NTP同步
};

enum EPLAYSTATE
{
	PLAYSTATE_RTSP_READY       = 1,                          ///< RTSP准备就绪
	PLAYSTATE_RTSP_CONNECTLOST = 2,                          ///< RTSP连接断开
	PLAYSTATE_DEC_INFO         = 3,							 ///< 解码信息回调
	PLAYSTATE_DEC_DRAW         = 4,                          ///< 解码描绘回调
};

enum EPlayAudioType
{
	PLAYAUDIOTYPE_NONE       = 1,                      ///< 不播放音频
	PLAYAUDIOTYPE_AUDIO1     = 2,                      ///< 播放音频1
	PLAYAUDIOTYPE_AUDIO2     = 3,                      ///< 播放音频2
	PLAYAUDIOTYPE_ALL        = 4,                      ///< 播放全部
};

enum EDecodeMode
{
	DECODEMODE_REALTIME            = 0,                ///< 实时模式
	DECODEMODE_STATISTICSFRAMERATE = 1,                ///< 统计帧率模式
	DECODEMODE_FIXEDFRAMERATE      = 2,                ///< 固定帧率模式
	DECODEMODE_TIMESTAMP           = 3,                ///< 时间戳模式
	DECODEMODE_REALTIMEBALANCED    = 4,                ///< 实时均衡模式
};

enum EMediaType
{
	MEDIATYPE_PCMU          = 0,                       ///< PCMU
	MEDIATYPE_ADPCM         = 5,                       ///< ADPCM
	MEDIATYPE_PCMA          = 8,                       ///< PCMA
	MEDIATYPE_G722          = 9,                       ///< G722
	MEDIATYPE_G7221C        = 98,                      ///< G7221C
	MEDIATYPE_AACLC         = 102,                     ///< AACLC
	MEDIATYPE_AMR           = 105,                     ///< AMR
	MEDIATYPE_OPUS          = 117,                     ///< OPUS
};

enum EFrameType
{
	FRAMETYPE_VIDEO = 0,
	FRAMETYPE_AUDIO,
	FRAMETYPE_DIRECT_VIDEO,
	FRAMETYPE_MEDIA_CODEC_VIDEO,
	FRAMETYPE_AUDIO_RENDER,			///< 音频扬声器，采集时指定该类型，则从扬声器采集，就WIN下支持
	FRAMETYPE_UNKNOWN = 255,
};

enum EDataType
{
	DATATYPE_VIDEO_I = 0,
	DATATYPE_VIDEO_P,
	DATATYPE_VIDEO_B,
};

enum EDataFormat
{
	DATAFORMAT_PCM  = 0,
	DATAFORMAT_YV12 = 100,
	DATAFORMAT_RGB24,
	DATAFORMAT_RGBA32,
	DATAFORMAT_ABGR,
	DATAFORMAT_ARGB4444,
	DATAFORMAT_RGB565,
	DATAFORMAT_ARGB1555,
	DATAFORMAT_BGRA,
	DATAFORMAT_YUY2,
	DATAFORMAT_UYVY,
	DATAFORMAT_NV21,
	DATAFORMAT_NV12,
	DATAFORMAT_I420,
	DATAFORMAT_SURF,
	DATAFORMAT_END
};

enum EDecBufType
{
	DECBUFTYPE_AUDIO_NOTDEC  = 0, ///< 音频未解码缓存
	DECBUFTYPE_VIDEO_NOTDEC  = 1, ///< 视频未解码缓存
	DECBUFTYPE_AUDIO_NOTPLAY = 2, ///< 音频未播放缓存
	DECBUFTYPE_VIDEO_NOTPLAY = 3, ///< 视频未播放缓存
};


/**
 * 结构体定义
 */

struct TDecDrawCallBackParam
{
	void *hdc;                                         ///< 描绘DC
	void *hwnd;                                        ///< 窗口句柄
	void *rendertype;                                  ///< 渲染类型，D3D直接显示，D3D显示，DDRAW显示，DRAWDIB显示
};

struct TDecCallBackInfo
{
	u32 dwHandle;			                           ///< 句柄
	EPLAYSTATE ePlayState;                             ///< 播放状态
	void *pContext;                                    ///< 回调上下文
	void *pParam;                                      ///< 回调参数
};

struct  TDecRtpPort
{
	u16 wRtpPort;                                      ///< RTP端口
	u16 wRtcpPort;                                     ///< RTCP端口
};

struct TDecRtpNetParam
{
	u32 dwIp;                                          ///< IP（网络序）
	TDecRtpPort tVideoRtpPort;                         ///< 视频端口信息
	TDecRtpPort tAudioRtpPort;                         ///< 音频端口信息
	TDecRtpPort tAudio2RtpPort;                        ///< 第二路音频端口信息
};

struct TDecRtspParam 
{
	s8 szUserName[DEC_MAXLEN_64 + 1];                  ///< 服务端用户名
	s8 szPassword[DEC_MAXLEN_64 + 1];                  ///< 服务端密码
	s8 szRtspURL[DEC_MAXLEN_512];                      ///< rtsp url
};

struct TDecRtpParam 
{
	TDecRtpNetParam tLocalRtpNetParam;                 ///< 本地rtp网络参数
	TDecRtpNetParam tRemoteRtpNetParam;                ///< 远端rtp网络参数
};

struct TDecRtspUdpParam 
{
	TDecRtspParam tRtspParam;                          ///< rtsp参数
	TDecRtpParam tRtpParam;                            ///< rtp参数
};

struct TDecSyncAudio
{
	BOOL32 bSyncToAudio;	                           ///< 是否开启音频同步 FALSE：关闭 TRUE：开启
	ESyncAudioType eSyncAudioType;                     ///< 音频同步类型
};

struct TDecDecodeParam 
{
	void *hWnd;                                        ///< 播放窗口句柄
	BOOL32 bHardwareAccelerate;                        ///< 是否开启硬件加速 FALSE：软解 TRUE：自适应硬件加速
	EDecodeMode eDecodeMode;                           ///< 解码模式
	TDecSyncAudio tSyncAudio;                          ///< 音频同步参数
	u32 dwDecDelay;                                    ///< 解码延时，单位ms，取值范围0-1200ms。默认值0ms.
};

struct TDecPlayRtspTcpParam
{
	s32 nChnId;                                        ///< 通道ID
	s32 nAudioChnNum;                                  ///< 音频通道数
	TDecRtspParam tRtspParam;                          ///< rtsp参数
	TDecDecodeParam tDecodeParam;                      ///< 解码参数
};

struct TDecPlayRtspUdpParam
{
	s32 nChnId;                                        ///< 通道ID
	s32 nAudioChnNum;                                  ///< 音频通道数
	TDecRtspUdpParam tRtspUdpParam;                    ///< rtsp udp参数
	TDecDecodeParam tDecodeParam;                      ///< 解码参数
};

struct TDecPlayRtpParam
{
	s32 nChnId;                                        ///< 通道ID
	s32 nAudioChnNum;                                  ///< 音频通道数
	TDecRtpParam tRtpParam;                            ///< rtp参数
	TDecDecodeParam tDecodeParam;                      ///< 解码参数
};

struct TDecRcvStatistics
{
	u32    dwPackNum;                                  ///< 已接收的包数
	u32    dwPackLose;                                 ///< 丟包数
	u32    dwPackIndexError;                           ///< 包乱序数
	u32    dwFrameNum;                                 ///< 已接收的帧数
};

struct TDecDecStatistics
{
	u8	byVideoFrameRate;                              ///< 视频解码帧率
	u16	wVideoBitRate;                                 ///< 视频解码码率
	u16	wAudioBitRate;                                 ///< 音频解码码率
	u32 dwVideoRecvFrame;                              ///< 收到的视频帧数
	u32 dwAudioRecvFrame;                              ///< 收到的音频帧数
	u32 dwVideoLoseFrame;                              ///< 丢失的视频帧数
	u32 dwAudioLoseFrame;                              ///< 丢失的音频帧数
	u16	wVideoLoseRatio;                               ///< 视频丢失率,单位是% 	
	u16	wAudioLoseRatio;                               ///< 音频丢失率,单位是% 	
	u32 dwPackError;                                   ///< 乱帧数
	u32 dwIndexLose;                                   ///< 序号丢帧
	BOOL32 bVidCompellingIFrm;                         ///< 视频强制请求I帧
	u32 dwDecdWidth;	                               ///< 码流的宽
	u32 dwDecdHeight;                                  ///< 码流的高
	s8 szVideoType[DEC_MAXLEN_32];                     ///< 视频格式
	s8 szAudioType[DEC_MAXLEN_32];                     ///< 音频格式
};

struct TDecDisplayRegionInfo
{
	s32 nLeft;                                         ///< 区域left
	s32 nRight;                                        ///< 区域right
	s32 nTop;                                          ///< 区域top
	s32 nBottom;                                       ///< 区域bottom
	void* hDestWnd;                                    ///< 显示视频区域句柄
	BOOL32 bEnable;                                    ///< 是否启用
};

struct TDecAudioEncParam 
{
	EMediaType eMediaType;                             ///< 音频编码类型
	s32 nEncSampleRate;                                ///< 编码采样率
};

struct TDecSendAudioNetParam
{
	u32 dwLocalIp;                                     ///< 本地IP（网络序）
	TDecRtpPort tLocalAudioRtpPort;                    ///< 本地音频端口信息
	u32 dwRemoteIp;                                    ///< 远端IP（网络序）
	TDecRtpPort tRemoteAudioRtpPort;                   ///< 远端音频端口信息
};

struct TDecAudioCaptureAndSendParam
{
	s32 nChnId;                                        ///< 通道id
	TDecAudioEncParam tAudioEncParam;                  ///< 音频编码参数
	TDecSendAudioNetParam tSendAudioNetParam;          ///< 发送音频网络参数
};

struct TDecFrameInfo
{
	u8 *pbyRawBuf;
	u32 dwRawlen;
	EFrameType eFrameType;		///< 数据类型
	EDataType eDataType;		///< 视频有效
	u32	dwFrameIndex;			///< 帧序号
	u64	ullNetTimeStamp;		///< 相对时间戳（RTP内部时间戳）
	u64	ullMediaTimeStamp;		///< 绝对时间戳，用于音视频同步
	EDataFormat eDataFormat;	///< EDataFormat
	union
	{
		struct
		{
			u16	wFrameRate;	
			u16	wWidth;			///< 画面宽
			u16	wHeight;		///< 画面高
			u16	strike[3];
		}tVideo;

		struct
		{
			u16	wChannels;		///< 声道数
			u16	wBitWidth;		///< 位宽
			u16	wSampleRate;	///< 采样率
		}tAudio;
	};
	u16	wBitRate;				///< 比特率
	u16	wReverse[3];			///< 采集侧，wReverse[0]保存音频采集DB值，wReverse[1]保存采集失败时的错误码，wReverse[2]参考源DB值；重编码回调时，wReverse[0]=0x00F0表示存在音频多路流，wReverse[1]的值为流ID，必须从1累加，目前最多支持2路音频流，比如wReverse[1]=1为流1，wReverse[1]=2为流2
};

struct TDecRawData
{
	u8 *pData;					///< 数据缓冲
	u32 dwPreBufSize;			///< pData缓冲前预留了多少空间，用于加
	u32	dwDataSize;				///< pData指向的实际缓冲大小
	EDataFormat eDataFormat;	///< EDataFormat
	u32 dwFrameID;				///< 帧标识，用于接收端
	u32 dwTimeStamp;			///< 时间戳，用于接收端
	u32 dwSSRC;					///< 同步源，用于接收端
	u64	ullRawTimeStamp;			///< 绝对时间戳，用于音视频同步
	union
	{
		struct
		{
			u8 byKeyFrame;		///< 低2位频帧类型（I or P），第8位为码流结束标志，为1时，内部将解码所有残留帧
			u8 byFrameRate;		///< 发送帧率，用于接收端
			u16 wVideoWidth;	///< 视频帧宽
			u16 wVideoHeight;	///< 视频帧宽
		}tVideo;

		struct
		{
			u16 wChannel;		///< 音频通道
			u16 wSample;		///< 音频采样率
			u16 wSamplebit;		///< 音频采样位数
		}tAudio;
	};
	u16 wReverse[4];			///< wReverse[0]=0x00F0表示存在音频多路流，wReverse[1]的值为流ID，必须从1累加，目前最多支持2路音频流，比如wReverse[1]=1为流1，wReverse[1]=2为流2
};


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
	#ifdef _DECSDK_
		#define DEC_API __declspec(dllexport)
	#else
		#define DEC_API __declspec(dllimport)
	#endif
	#define STDCALL _stdcall
#else
	#define DEC_API
	#define STDCALL
#endif

typedef void (*pfPlayCallBack)(TDecCallBackInfo tCallBackInfo);
typedef void (*pfYuvDataCallBack)(u32 dwHandle, TDecFrameInfo tFrameInfo, void *pContext);
typedef void (*pfRawDataCallBack)(u32 dwHandle, TDecRawData tRawData, void *pContext);

/**
 * @brief  初始化kddecsdk
 * @param  无
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_Init();

/**
 * @brief  释放kddecsdk资源
 * @param  无
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_Cleanup();

/**
 * @brief  开始播放rtsp tcp（默认不播放音频）
 * @param  tPlayRtspTcpParam      [in]播放参数
 *         pdwHandle              [out]播放器句柄，从1开始
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_StartPlayRtspTcp(TDecPlayRtspTcpParam tPlayRtspTcpParam, u32 *pdwHandle);

/**
 * @brief  开始播放rtsp udp（默认不播放音频）
 * @param  tPlayRtspUdpParam      [in]播放参数
 *         pdwHandle              [out]播放器句柄，从1开始
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_StartPlayRtspUdp(TDecPlayRtspUdpParam tPlayRtspUdpParam, u32 *pdwHandle);

/**
 * @brief  开始播放rtp udp（默认不播放音频）
 * @param  tPlayParam      [in]播放参数
 *         pdwHandle       [out]播放器句柄，从1开始
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_StartPlayRtp(TDecPlayRtpParam tPlayRtpParam, u32 *pdwHandle);

/**
 * @brief  停止播放
 * @param  dwHandle      [in]播放器句柄
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_StopPlay(u32 dwHandle);

/**
 * @brief  暂停播放
 * @param  dwHandle      [in]播放器句柄
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_PausePlay(u32 dwHandle);

/**
 * @brief  恢复播放
 * @param  dwHandle      [in]播放器句柄
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_ResumePlay(u32 dwHandle);

/**
 * @brief  重刷最后一帧图像
 * @param  dwHandle      [in]播放器句柄
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_RedrawLastFrame(u32 dwHandle);

/**
 * @brief  播放音频
 * @param  dwHandle       [in]播放器句柄
 *         ePlayAudioType [in]音频播放类型
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_PlayAudio(u32 dwHandle, EPlayAudioType ePlayAudioType);

/**
 * @brief  设置音频音量
 * @param  dwHandle       [in]播放器句柄
 *         dwVolume       [in]音频音量,范围[0,0xFFFF]
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_SetAudioVolume(u32 dwHandle, u32 dwVolume);

/**
 * @brief  获取音频音量
 * @param  dwHandle       [in]播放器句柄
 *         pdwVolume      [out]音频音量,范围[0,0xFFFF]
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_GetAudioVolume(u32 dwHandle, u32 *pdwVolume);

/**
 * @brief  设置播放回调
 * @param  dwHandle      [in]播放器句柄
 *         pPlayCB       [in]播放回调
 *         pContext      [in]回调上下文
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_SetPlayCallBack(u32 dwHandle, pfPlayCallBack pPlayCB, void *pContext);

/**
 * @brief  设置YUV数据回调
 * @param  dwHandle      [in]播放器句柄
 *         pYuvDataCB    [in]回调接口
 *         pContext      [in]回调上下文
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_SetYuvDataCallBack(u32 dwHandle, pfYuvDataCallBack pYuvDataCB, void *pContext);

/**
 * @brief  设置原始数据回调
 * @param  dwHandle      [in]播放器句柄
 *         pRawDataCB    [in]回调接口
 *         pContext      [in]回调上下文
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_SetRawDataCallBack(u32 dwHandle, pfRawDataCallBack pRawDataCB, void *pContext);

/**
 * @brief  获取接收统计数据
 * @param  dwHandle               [in]播放器句柄
 *         ptVideoRcvStatistics   [out]视频接收统计数据
 *         ptAudioRcvStatistics   [out]音频接收统计数据
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_GetRcvStatistics(u32 dwHandle, TDecRcvStatistics *ptVideoRcvStatistics, TDecRcvStatistics *ptAudioRcvStatistics);

/**
 * @brief  获取解码统计数据
 * @param  dwHandle               [in]播放器句柄
 *         ptDecStatistics        [out]解码统计数据
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_GetDecStatistics(u32 dwHandle, TDecDecStatistics *ptDecStatistics);

/**
 * @brief  获取解码统计数据
 * @param  dwHandle               [in]播放器句柄
 *         ePlayAudioType         [in]播放音频类型
 *         ptDecStatistics        [out]解码统计数据
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_GetDecStatisticsEx(u32 dwHandle, EPlayAudioType ePlayAudioType, TDecDecStatistics *ptDecStatistics);

/**
 * @brief  本地抓拍
 * @param  dwHandle               [in]播放器句柄
 *         szPicFullPath          [in]抓拍图片完整路径
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_LocalSnapshot(u32 dwHandle, const s8 *szPicFullPath);

/**
 * @brief  是否正在本地录像
 * @param  dwHandle               [in]播放器句柄
 * @return 正在本地录像返回TRUE
 */
DEC_API BOOL32 STDCALL DEC_IsLocalRecording(u32 dwHandle);

/**
 * @brief  开始本地录像
 * @param  dwHandle               [in]播放器句柄
 *         szFileFullPath         [in]录像文件完整路径
 *         dwFileSize             [in]录像文件切分大小,单位1K字节
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_StartLocalRecord(u32 dwHandle, const s8 *szFileFullPath, u32 dwFileSize);

/**
 * @brief  停止本地录像
 * @param  dwHandle               [in]播放器句柄
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_StopLocalRecord(u32 dwHandle);

/**
 * @brief  垂直同步设置
 * @param  dwHandle               [in]播放器句柄
 *         bEnable                [in]是否开启垂直同步
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_VerticalSyncEnable(u32 dwHandle, BOOL32 bEnable);

/**
 * @brief  设置数字放大区域
 * @param  dwHandle               [in]播放器句柄
 *         tDisplayRegionInfo     [in]显示区域信息
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_SetDisplayRegion(u32 dwHandle, TDecDisplayRegionInfo tDisplayRegionInfo);

/**
 * @brief  开始音频捕获和发送
 * @param  tAudioCaptureAndSendParam      [in]音频捕获和发送参数
 *         pdwHandle                      [out]播放器句柄，从1开始
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_StartAudioCaptureAndSend(TDecAudioCaptureAndSendParam tAudioCaptureAndSendParam, u32 *pdwHandle);

/**
 * @brief  停止音频捕获和发送
 * @param  tAudioCaptureAndSendParam      [in]音频捕获和发送参数
 *         dwHandle                      [out]播放器句柄，从1开始
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_StopAudioCaptureAndSend(u32 dwHandle);

/**
 * @brief  是否正在音频捕获
 * @param  dwHandle               [in]播放器句柄
 * @return 正在音频捕获返回TRUE
 */
DEC_API BOOL32 STDCALL DEC_IsAudioCapture(u32 dwHandle);

/**
 * @brief  清除解码器缓存
 * @param  dwHandle    [in]播放器句柄
 *         eDecBufType [in]缓存类型
 * @return 成功返回DEC_OK，失败返回错误码
 */
DEC_API s32 STDCALL DEC_ResetDecBuffer(u32 dwHandle, EDecBufType eDecBufType);

#ifdef __cplusplus
}
#endif

#endif