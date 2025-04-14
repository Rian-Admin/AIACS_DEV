#ifndef _DECSDK_H_
#define _DECSDK_H_

#include "decsdkerrno.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * decsdk���Ͷ���
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

#define DEC_MAXLEN_8							8
#define DEC_MAXLEN_16							16
#define DEC_MAXLEN_32							32
#define DEC_MAXLEN_64							64
#define DEC_MAXLEN_128							128
#define DEC_MAXLEN_256							256
#define DEC_MAXLEN_512							512


/**
 * ö�ٶ���
 */

enum ESyncAudioType
{
	SYNCAUDIOTYPE_RTP = 1,			                   ///< ʹ��RTPͬ��
	SYNCAUDIOTYPE_NTP = 2,		                       ///< ʹ��NTPͬ��
};

enum EPLAYSTATE
{
	PLAYSTATE_RTSP_READY       = 1,                          ///< RTSP׼������
	PLAYSTATE_RTSP_CONNECTLOST = 2,                          ///< RTSP���ӶϿ�
	PLAYSTATE_DEC_INFO         = 3,							 ///< ������Ϣ�ص�
	PLAYSTATE_DEC_DRAW         = 4,                          ///< �������ص�
};

enum EPlayAudioType
{
	PLAYAUDIOTYPE_NONE       = 1,                      ///< ��������Ƶ
	PLAYAUDIOTYPE_AUDIO1     = 2,                      ///< ������Ƶ1
	PLAYAUDIOTYPE_AUDIO2     = 3,                      ///< ������Ƶ2
	PLAYAUDIOTYPE_ALL        = 4,                      ///< ����ȫ��
};

enum EDecodeMode
{
	DECODEMODE_REALTIME            = 0,                ///< ʵʱģʽ
	DECODEMODE_STATISTICSFRAMERATE = 1,                ///< ͳ��֡��ģʽ
	DECODEMODE_FIXEDFRAMERATE      = 2,                ///< �̶�֡��ģʽ
	DECODEMODE_TIMESTAMP           = 3,                ///< ʱ���ģʽ
	DECODEMODE_REALTIMEBALANCED    = 4,                ///< ʵʱ����ģʽ
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
	FRAMETYPE_AUDIO_RENDER,			///< ��Ƶ���������ɼ�ʱָ�������ͣ�����������ɼ�����WIN��֧��
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
	DECBUFTYPE_AUDIO_NOTDEC  = 0, ///< ��Ƶδ���뻺��
	DECBUFTYPE_VIDEO_NOTDEC  = 1, ///< ��Ƶδ���뻺��
	DECBUFTYPE_AUDIO_NOTPLAY = 2, ///< ��Ƶδ���Ż���
	DECBUFTYPE_VIDEO_NOTPLAY = 3, ///< ��Ƶδ���Ż���
};


/**
 * �ṹ�嶨��
 */

struct TDecDrawCallBackParam
{
	void *hdc;                                         ///< ���DC
	void *hwnd;                                        ///< ���ھ��
	void *rendertype;                                  ///< ��Ⱦ���ͣ�D3Dֱ����ʾ��D3D��ʾ��DDRAW��ʾ��DRAWDIB��ʾ
};

struct TDecCallBackInfo
{
	u32 dwHandle;			                           ///< ���
	EPLAYSTATE ePlayState;                             ///< ����״̬
	void *pContext;                                    ///< �ص�������
	void *pParam;                                      ///< �ص�����
};

struct  TDecRtpPort
{
	u16 wRtpPort;                                      ///< RTP�˿�
	u16 wRtcpPort;                                     ///< RTCP�˿�
};

struct TDecRtpNetParam
{
	u32 dwIp;                                          ///< IP��������
	TDecRtpPort tVideoRtpPort;                         ///< ��Ƶ�˿���Ϣ
	TDecRtpPort tAudioRtpPort;                         ///< ��Ƶ�˿���Ϣ
	TDecRtpPort tAudio2RtpPort;                        ///< �ڶ�·��Ƶ�˿���Ϣ
};

struct TDecRtspParam 
{
	s8 szUserName[DEC_MAXLEN_64 + 1];                  ///< ������û���
	s8 szPassword[DEC_MAXLEN_64 + 1];                  ///< ���������
	s8 szRtspURL[DEC_MAXLEN_512];                      ///< rtsp url
};

struct TDecRtpParam 
{
	TDecRtpNetParam tLocalRtpNetParam;                 ///< ����rtp�������
	TDecRtpNetParam tRemoteRtpNetParam;                ///< Զ��rtp�������
};

struct TDecRtspUdpParam 
{
	TDecRtspParam tRtspParam;                          ///< rtsp����
	TDecRtpParam tRtpParam;                            ///< rtp����
};

struct TDecSyncAudio
{
	BOOL32 bSyncToAudio;	                           ///< �Ƿ�����Ƶͬ�� FALSE���ر� TRUE������
	ESyncAudioType eSyncAudioType;                     ///< ��Ƶͬ������
};

struct TDecDecodeParam 
{
	void *hWnd;                                        ///< ���Ŵ��ھ��
	BOOL32 bHardwareAccelerate;                        ///< �Ƿ���Ӳ������ FALSE����� TRUE������ӦӲ������
	EDecodeMode eDecodeMode;                           ///< ����ģʽ
	TDecSyncAudio tSyncAudio;                          ///< ��Ƶͬ������
	u32 dwDecDelay;                                    ///< ������ʱ����λms��ȡֵ��Χ0-1200ms��Ĭ��ֵ0ms.
};

struct TDecPlayRtspTcpParam
{
	s32 nChnId;                                        ///< ͨ��ID
	s32 nAudioChnNum;                                  ///< ��Ƶͨ����
	TDecRtspParam tRtspParam;                          ///< rtsp����
	TDecDecodeParam tDecodeParam;                      ///< �������
};

struct TDecPlayRtspUdpParam
{
	s32 nChnId;                                        ///< ͨ��ID
	s32 nAudioChnNum;                                  ///< ��Ƶͨ����
	TDecRtspUdpParam tRtspUdpParam;                    ///< rtsp udp����
	TDecDecodeParam tDecodeParam;                      ///< �������
};

struct TDecPlayRtpParam
{
	s32 nChnId;                                        ///< ͨ��ID
	s32 nAudioChnNum;                                  ///< ��Ƶͨ����
	TDecRtpParam tRtpParam;                            ///< rtp����
	TDecDecodeParam tDecodeParam;                      ///< �������
};

struct TDecRcvStatistics
{
	u32    dwPackNum;                                  ///< �ѽ��յİ���
	u32    dwPackLose;                                 ///< �G����
	u32    dwPackIndexError;                           ///< ��������
	u32    dwFrameNum;                                 ///< �ѽ��յ�֡��
};

struct TDecDecStatistics
{
	u8	byVideoFrameRate;                              ///< ��Ƶ����֡��
	u16	wVideoBitRate;                                 ///< ��Ƶ��������
	u16	wAudioBitRate;                                 ///< ��Ƶ��������
	u32 dwVideoRecvFrame;                              ///< �յ�����Ƶ֡��
	u32 dwAudioRecvFrame;                              ///< �յ�����Ƶ֡��
	u32 dwVideoLoseFrame;                              ///< ��ʧ����Ƶ֡��
	u32 dwAudioLoseFrame;                              ///< ��ʧ����Ƶ֡��
	u16	wVideoLoseRatio;                               ///< ��Ƶ��ʧ��,��λ��% 	
	u16	wAudioLoseRatio;                               ///< ��Ƶ��ʧ��,��λ��% 	
	u32 dwPackError;                                   ///< ��֡��
	u32 dwIndexLose;                                   ///< ��Ŷ�֡
	BOOL32 bVidCompellingIFrm;                         ///< ��Ƶǿ������I֡
	u32 dwDecdWidth;	                               ///< �����Ŀ�
	u32 dwDecdHeight;                                  ///< �����ĸ�
	s8 szVideoType[DEC_MAXLEN_32];                     ///< ��Ƶ��ʽ
	s8 szAudioType[DEC_MAXLEN_32];                     ///< ��Ƶ��ʽ
};

struct TDecDisplayRegionInfo
{
	s32 nLeft;                                         ///< ����left
	s32 nRight;                                        ///< ����right
	s32 nTop;                                          ///< ����top
	s32 nBottom;                                       ///< ����bottom
	void* hDestWnd;                                    ///< ��ʾ��Ƶ������
	BOOL32 bEnable;                                    ///< �Ƿ�����
};

struct TDecAudioEncParam 
{
	EMediaType eMediaType;                             ///< ��Ƶ��������
	s32 nEncSampleRate;                                ///< ���������
};

struct TDecSendAudioNetParam
{
	u32 dwLocalIp;                                     ///< ����IP��������
	TDecRtpPort tLocalAudioRtpPort;                    ///< ������Ƶ�˿���Ϣ
	u32 dwRemoteIp;                                    ///< Զ��IP��������
	TDecRtpPort tRemoteAudioRtpPort;                   ///< Զ����Ƶ�˿���Ϣ
};

struct TDecAudioCaptureAndSendParam
{
	s32 nChnId;                                        ///< ͨ��id
	TDecAudioEncParam tAudioEncParam;                  ///< ��Ƶ�������
	TDecSendAudioNetParam tSendAudioNetParam;          ///< ������Ƶ�������
};

struct TDecFrameInfo
{
	u8 *pbyRawBuf;
	u32 dwRawlen;
	EFrameType eFrameType;		///< ��������
	EDataType eDataType;		///< ��Ƶ��Ч
	u32	dwFrameIndex;			///< ֡���
	u64	ullNetTimeStamp;		///< ���ʱ�����RTP�ڲ�ʱ�����
	u64	ullMediaTimeStamp;		///< ����ʱ�������������Ƶͬ��
	EDataFormat eDataFormat;	///< EDataFormat
	union
	{
		struct
		{
			u16	wFrameRate;	
			u16	wWidth;			///< �����
			u16	wHeight;		///< �����
			u16	strike[3];
		}tVideo;

		struct
		{
			u16	wChannels;		///< ������
			u16	wBitWidth;		///< λ��
			u16	wSampleRate;	///< ������
		}tAudio;
	};
	u16	wBitRate;				///< ������
	u16	wReverse[3];			///< �ɼ��࣬wReverse[0]������Ƶ�ɼ�DBֵ��wReverse[1]����ɼ�ʧ��ʱ�Ĵ����룬wReverse[2]�ο�ԴDBֵ���ر���ص�ʱ��wReverse[0]=0x00F0��ʾ������Ƶ��·����wReverse[1]��ֵΪ��ID�������1�ۼӣ�Ŀǰ���֧��2·��Ƶ��������wReverse[1]=1Ϊ��1��wReverse[1]=2Ϊ��2
};

struct TDecRawData
{
	u8 *pData;					///< ���ݻ���
	u32 dwPreBufSize;			///< pData����ǰԤ���˶��ٿռ䣬���ڼ�
	u32	dwDataSize;				///< pDataָ���ʵ�ʻ����С
	EDataFormat eDataFormat;	///< EDataFormat
	u32 dwFrameID;				///< ֡��ʶ�����ڽ��ն�
	u32 dwTimeStamp;			///< ʱ��������ڽ��ն�
	u32 dwSSRC;					///< ͬ��Դ�����ڽ��ն�
	u64	ullRawTimeStamp;			///< ����ʱ�������������Ƶͬ��
	union
	{
		struct
		{
			u8 byKeyFrame;		///< ��2λƵ֡���ͣ�I or P������8λΪ����������־��Ϊ1ʱ���ڲ����������в���֡
			u8 byFrameRate;		///< ����֡�ʣ����ڽ��ն�
			u16 wVideoWidth;	///< ��Ƶ֡��
			u16 wVideoHeight;	///< ��Ƶ֡��
		}tVideo;

		struct
		{
			u16 wChannel;		///< ��Ƶͨ��
			u16 wSample;		///< ��Ƶ������
			u16 wSamplebit;		///< ��Ƶ����λ��
		}tAudio;
	};
	u16 wReverse[4];			///< wReverse[0]=0x00F0��ʾ������Ƶ��·����wReverse[1]��ֵΪ��ID�������1�ۼӣ�Ŀǰ���֧��2·��Ƶ��������wReverse[1]=1Ϊ��1��wReverse[1]=2Ϊ��2
};


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
 * @brief  ��ʼ��kddecsdk
 * @param  ��
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_Init();

/**
 * @brief  �ͷ�kddecsdk��Դ
 * @param  ��
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_Cleanup();

/**
 * @brief  ��ʼ����rtsp tcp��Ĭ�ϲ�������Ƶ��
 * @param  tPlayRtspTcpParam      [in]���Ų���
 *         pdwHandle              [out]�������������1��ʼ
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_StartPlayRtspTcp(TDecPlayRtspTcpParam tPlayRtspTcpParam, u32 *pdwHandle);

/**
 * @brief  ��ʼ����rtsp udp��Ĭ�ϲ�������Ƶ��
 * @param  tPlayRtspUdpParam      [in]���Ų���
 *         pdwHandle              [out]�������������1��ʼ
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_StartPlayRtspUdp(TDecPlayRtspUdpParam tPlayRtspUdpParam, u32 *pdwHandle);

/**
 * @brief  ��ʼ����rtp udp��Ĭ�ϲ�������Ƶ��
 * @param  tPlayParam      [in]���Ų���
 *         pdwHandle       [out]�������������1��ʼ
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_StartPlayRtp(TDecPlayRtpParam tPlayRtpParam, u32 *pdwHandle);

/**
 * @brief  ֹͣ����
 * @param  dwHandle      [in]���������
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_StopPlay(u32 dwHandle);

/**
 * @brief  ��ͣ����
 * @param  dwHandle      [in]���������
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_PausePlay(u32 dwHandle);

/**
 * @brief  �ָ�����
 * @param  dwHandle      [in]���������
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_ResumePlay(u32 dwHandle);

/**
 * @brief  ��ˢ���һ֡ͼ��
 * @param  dwHandle      [in]���������
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_RedrawLastFrame(u32 dwHandle);

/**
 * @brief  ������Ƶ
 * @param  dwHandle       [in]���������
 *         ePlayAudioType [in]��Ƶ��������
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_PlayAudio(u32 dwHandle, EPlayAudioType ePlayAudioType);

/**
 * @brief  ������Ƶ����
 * @param  dwHandle       [in]���������
 *         dwVolume       [in]��Ƶ����,��Χ[0,0xFFFF]
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_SetAudioVolume(u32 dwHandle, u32 dwVolume);

/**
 * @brief  ��ȡ��Ƶ����
 * @param  dwHandle       [in]���������
 *         pdwVolume      [out]��Ƶ����,��Χ[0,0xFFFF]
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_GetAudioVolume(u32 dwHandle, u32 *pdwVolume);

/**
 * @brief  ���ò��Żص�
 * @param  dwHandle      [in]���������
 *         pPlayCB       [in]���Żص�
 *         pContext      [in]�ص�������
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_SetPlayCallBack(u32 dwHandle, pfPlayCallBack pPlayCB, void *pContext);

/**
 * @brief  ����YUV���ݻص�
 * @param  dwHandle      [in]���������
 *         pYuvDataCB    [in]�ص��ӿ�
 *         pContext      [in]�ص�������
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_SetYuvDataCallBack(u32 dwHandle, pfYuvDataCallBack pYuvDataCB, void *pContext);

/**
 * @brief  ����ԭʼ���ݻص�
 * @param  dwHandle      [in]���������
 *         pRawDataCB    [in]�ص��ӿ�
 *         pContext      [in]�ص�������
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_SetRawDataCallBack(u32 dwHandle, pfRawDataCallBack pRawDataCB, void *pContext);

/**
 * @brief  ��ȡ����ͳ������
 * @param  dwHandle               [in]���������
 *         ptVideoRcvStatistics   [out]��Ƶ����ͳ������
 *         ptAudioRcvStatistics   [out]��Ƶ����ͳ������
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_GetRcvStatistics(u32 dwHandle, TDecRcvStatistics *ptVideoRcvStatistics, TDecRcvStatistics *ptAudioRcvStatistics);

/**
 * @brief  ��ȡ����ͳ������
 * @param  dwHandle               [in]���������
 *         ptDecStatistics        [out]����ͳ������
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_GetDecStatistics(u32 dwHandle, TDecDecStatistics *ptDecStatistics);

/**
 * @brief  ��ȡ����ͳ������
 * @param  dwHandle               [in]���������
 *         ePlayAudioType         [in]������Ƶ����
 *         ptDecStatistics        [out]����ͳ������
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_GetDecStatisticsEx(u32 dwHandle, EPlayAudioType ePlayAudioType, TDecDecStatistics *ptDecStatistics);

/**
 * @brief  ����ץ��
 * @param  dwHandle               [in]���������
 *         szPicFullPath          [in]ץ��ͼƬ����·��
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_LocalSnapshot(u32 dwHandle, const s8 *szPicFullPath);

/**
 * @brief  �Ƿ����ڱ���¼��
 * @param  dwHandle               [in]���������
 * @return ���ڱ���¼�񷵻�TRUE
 */
DEC_API BOOL32 STDCALL DEC_IsLocalRecording(u32 dwHandle);

/**
 * @brief  ��ʼ����¼��
 * @param  dwHandle               [in]���������
 *         szFileFullPath         [in]¼���ļ�����·��
 *         dwFileSize             [in]¼���ļ��зִ�С,��λ1K�ֽ�
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_StartLocalRecord(u32 dwHandle, const s8 *szFileFullPath, u32 dwFileSize);

/**
 * @brief  ֹͣ����¼��
 * @param  dwHandle               [in]���������
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_StopLocalRecord(u32 dwHandle);

/**
 * @brief  ��ֱͬ������
 * @param  dwHandle               [in]���������
 *         bEnable                [in]�Ƿ�����ֱͬ��
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_VerticalSyncEnable(u32 dwHandle, BOOL32 bEnable);

/**
 * @brief  �������ַŴ�����
 * @param  dwHandle               [in]���������
 *         tDisplayRegionInfo     [in]��ʾ������Ϣ
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_SetDisplayRegion(u32 dwHandle, TDecDisplayRegionInfo tDisplayRegionInfo);

/**
 * @brief  ��ʼ��Ƶ����ͷ���
 * @param  tAudioCaptureAndSendParam      [in]��Ƶ����ͷ��Ͳ���
 *         pdwHandle                      [out]�������������1��ʼ
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_StartAudioCaptureAndSend(TDecAudioCaptureAndSendParam tAudioCaptureAndSendParam, u32 *pdwHandle);

/**
 * @brief  ֹͣ��Ƶ����ͷ���
 * @param  tAudioCaptureAndSendParam      [in]��Ƶ����ͷ��Ͳ���
 *         dwHandle                      [out]�������������1��ʼ
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_StopAudioCaptureAndSend(u32 dwHandle);

/**
 * @brief  �Ƿ�������Ƶ����
 * @param  dwHandle               [in]���������
 * @return ������Ƶ���񷵻�TRUE
 */
DEC_API BOOL32 STDCALL DEC_IsAudioCapture(u32 dwHandle);

/**
 * @brief  �������������
 * @param  dwHandle    [in]���������
 *         eDecBufType [in]��������
 * @return �ɹ�����DEC_OK��ʧ�ܷ��ش�����
 */
DEC_API s32 STDCALL DEC_ResetDecBuffer(u32 dwHandle, EDecBufType eDecBufType);

#ifdef __cplusplus
}
#endif

#endif