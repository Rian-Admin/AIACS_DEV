
// netsdkdemoDlg.h : ͷ�ļ�
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
	s32 nTaskId;                        ///< �ط�����Id
	s32 nChnId;                         ///< ��ǰ�ط�ͨ��
	s32 nTaskRecDownLoadId;				//<�ط���������ID	
	s32 nRecDownLoadChnId;
};

//��������
#define NPC_D_NTS_DPMC_MAC_DEV_NUM							32					//����豸��

enum 
{
	NPC_NETPORT_TYPE_LOGIN=0,
	NPC_NETPORT_TYPE_RTSP,
	NPC_NETPORT_TYPE_MAX
};
//�豸����
typedef struct _NPC_S_NTS_DPMC_DEV_DATA
{
	NPC_CHAR							sUMID[64];								//UMID
	NPC_CHAR							sConnID[64];							//���ӷ�����ID

	NPC_BOOL							bIfSetLocalPort;						//�Ƿ����ñ��ض˿ں�
	NPC_USHORT							awLocalPort[NPC_NETPORT_TYPE_MAX];		//���ض˿ں�  ������
	NPC_USHORT							usState;								//״̬

} NPC_S_NTS_DPMC_DEV_DATA, *PNPC_S_NTS_DPMC_DEV_DATA;

//��������
typedef struct _NPC_S_NTS_DPMC_CONFIG
{
	NPC_CHAR							sServerIpAddr[512];						//������IP��ַ
	NPC_USHORT							usServerPort;							//�������˿ں�
	NPC_CHAR							sAuthId[64];							//��֤ID
	NPC_CHAR							sAuthPwd[64];							//��֤����
//	NPC_BOOL							bIfSaveLog;								//�Ƿ񱣴���־

	//NPC_INT								iDevNum;								//�豸��
	NPC_S_NTS_DPMC_DEV_DATA				tDevCfg;	//�豸��

} NPC_S_NTS_DPMC_CONFIG, *PNPC_S_NTS_DPMC_CONFIG;

// CnetsdkdemoDlg �Ի���
class CnetsdkdemoDlg : public CDialog
{
// ����
public:
	CnetsdkdemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_NETSDKDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	void UpdateChnList();	///< ����ͨ���б�
	BOOL32 StartView(s32 nChnId);
	BOOL32 StopView();
	BOOL32 StartPlayBackByTCP(s32 nChnId, const s8 *szStartTime);
	BOOL32 StartPlayBackByUDP(s32 nChnId, const s8 *szStartTime);
	BOOL32 StopPlayBack();

	BOOL32 StartPlayDownLoadByTCP(s32 nChnId, const s8 *szStartTime);


	s32 GetAudioEncNum( s32 nChnId );

private:
	u32 m_dwHandle;
	TNetNvrCap m_tNvrCap;				///< �豸��Ϣ
	TNetNvrChnList m_tNvrChnList;		///< ͨ����Ϣ

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
	 * ͨ���б����
	 */
	enum EChnListTitle
	{
		COL_CHNID        = 0, ///< ͨ����
		COL_CHNNAME      = 1, ///< ͨ������
		COL_STATE        = 2, ///< ״̬
		COL_IP           = 3, ///< IP��ַ
		COL_DEV_TYPE     = 4,  ///< �豸����
		COL_PROTO        = 5, ///< Э��
		COL_PORT	     = 6, ///< �˿�
		COL_DEV_MODEL    = 7, ///< �豸�ͺ�
		COL_MANU_INFO    = 8  ///< ����
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
