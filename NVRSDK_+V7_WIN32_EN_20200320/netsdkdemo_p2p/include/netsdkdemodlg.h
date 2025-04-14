
// netsdkdemoDlg.h : 头文件
//

#pragma once
#include "netsdk.h"
#include "netsdkerrno.h"
#include "decsdk.h"
#include "decsdkerrno.h"
#include <string>
#include <iostream>
#include "NPC_MPI_MON_DevNetPortServer.h"
#include <sstream>  

using namespace std;

struct TPlayBackParm
{
	s32 nTaskId;                        ///< 回放任务Id
	s32 nChnId;                         ///< 当前回放通道
	s32 nTaskRecDownLoadId;				//<回放下载任务ID	
	s32 nRecDownLoadChnId;
};

//常量定义
#define NPC_D_NTS_DPMC_MAC_DEV_NUM							32					//最大设备数

enum 
{
	NPC_NETPORT_TYPE_LOGIN=0,
	NPC_NETPORT_TYPE_RTSP,
	NPC_NETPORT_TYPE_MAX
};
//设备数据
typedef struct _NPC_S_NTS_DPMC_DEV_DATA
{
	NPC_CHAR							sUMID[64];								//UMID
	NPC_CHAR							sConnID[64];							//连接服务器ID

	NPC_BOOL							bIfSetLocalPort;						//是否设置本地端口号
	NPC_USHORT							awLocalPort[NPC_NETPORT_TYPE_MAX];		//本地端口号  本地序
	NPC_USHORT							usState;								//状态

} NPC_S_NTS_DPMC_DEV_DATA, *PNPC_S_NTS_DPMC_DEV_DATA;

//配置数据
typedef struct _NPC_S_NTS_DPMC_CONFIG
{
	NPC_CHAR							sServerIpAddr[512];						//服务器IP地址
	NPC_USHORT							usServerPort;							//服务器端口号
	NPC_CHAR							sAuthId[64];							//认证ID
	NPC_CHAR							sAuthPwd[64];							//认证密码
//	NPC_BOOL							bIfSaveLog;								//是否保存日志

	//NPC_INT								iDevNum;								//设备数
	NPC_S_NTS_DPMC_DEV_DATA				tDevCfg;	//设备表

} NPC_S_NTS_DPMC_CONFIG, *PNPC_S_NTS_DPMC_CONFIG;

// CnetsdkdemoDlg 对话框
class CnetsdkdemoDlg : public CDialog
{
// 构造
public:
	CnetsdkdemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NETSDKDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg NPC_BOOL ReadConfig();
	afx_msg NPC_BOOL Umid2ConnId(s8 *pUmid, s8 *pConnId, u32 dwConnIdLen);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonGetChnList();
	afx_msg void OnBnClickedButtonView();
	afx_msg void OnBnClickedButtonPlayback();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

public:
	BOOL32 NetStartPlayBack(u32 dwHandle);
	BOOL32 NetStartPlayDownLoad(u32 dwHandle);
	BOOL32 QueryPlayProgress();

private:
	BOOL IsLogin()	{ return m_dwHandle > 0; }
	void UpdateChnList();	///< 更新通道列表
	BOOL32 StartView(s32 nChnId);
	BOOL32 StopView();
	BOOL32 StartPlayBackByTCP(s32 nChnId, const s8 *szStartTime);
	BOOL32 StartPlayBackByUDP(s32 nChnId, const s8 *szStartTime);
	BOOL32 StopPlayBack();

	BOOL32 StartPlayDownLoadByTCP(s32 nChnId, const s8 *szStartTime);


	s32 GetAudioEncNum( s32 nChnId );

private:
	u32 m_dwHandle;
	TNetNvrCap m_tNvrCap;				///< 设备信息
	TNetNvrChnList m_tNvrChnList;		///< 通道信息

	u32 m_dwDecViewHandle;
	u32 m_dwDecPlayBackHandle;
	BOOL32 m_bStartView;
	BOOL32 m_bStartPlayBack;

public:
	//p2p map data
	BOOL32 m_bIsUmidUser;
	NPC_S_NTS_DPMC_CONFIG m_tConfig;
	NPC_HANDLE m_pNetSerHandle;

	//rec DownLoad
	u32 m_dwDecRecDownLoadHandle;

	TPlayBackParm m_tPlayBackParm;

	/**
	 * 通道列表标题
	 */
	enum EChnListTitle
	{
		COL_CHNID        = 0, ///< 通道号
		COL_CHNNAME      = 1, ///< 通道名称
		COL_STATE        = 2, ///< 状态
		COL_IP           = 3, ///< IP地址
		COL_DEV_TYPE     = 4,  ///< 设备类型
		COL_PROTO        = 5, ///< 协议
		COL_PORT	     = 6, ///< 端口
		COL_DEV_MODEL    = 7, ///< 设备型号
		COL_MANU_INFO    = 8  ///< 厂商
	};

	CIPAddressCtrl m_ipaddrIp;
	CListCtrl m_listChnList;
	CEdit m_editPort;
	CEdit m_editUser;
	CEdit m_editPwd;
	CButton m_radioTcp;
	CStatic m_staticView;
	CStatic m_staticPlayBack;
	CButton m_btnView;
	CButton m_btnPlayBack;
	CDateTimeCtrl m_datetimePlayBack;
	CDateTimeCtrl m_datetimeTime;
};

string IP2string( u32 dwIP );
CString GetChnState(TNetChnState tNetChn);
CString GetDevType(EDevType eDevType);
CString GetProtoType(EProtoType eProtoType);
s32 GetLocalIPEx(u32 &dwLocalIP, u32 dwDestIP);
