
// netsdkdemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "netsdkdemo.h"
#include "netsdkdemodlg.h"
#ifdef _P2P_MAP_
#include "NPC_MPI_MON_DevNetPortServer.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CnetsdkdemoDlg 对话框

#pragma comment(lib, "kdnetsdk.lib")
#pragma comment(lib, "kddecsdk.lib")
#ifdef _P2P_MAP_
#pragma comment(lib, "NpcMpiMonTsp.lib")
#endif

#define NET_SDK_ADDR		"127.0.0.1"
#define NET_SDK_PORT		"80"
#define NET_SDK_USER_NAME	"admin"
#define NET_SDK_PASSWROD	"admin123"

#define CSTRING2CHAR(cstr) cstr.GetBuffer(cstr.GetLength())

void PlayCallBack(TDecCallBackInfo tCallBackInfo);
void PlayDownLoadCallBack(TDecCallBackInfo tCallBackInfo);
void NvrStateCallBack(u32 dwHandle, TNetNvrState tNvrState);

s8 g_achLocalPath[NET_MAXLEN_512] = { 0 };
s8 g_achLogPath[NET_MAXLEN_512] = { 0 };
#define  P2P_LOG_BUF_LEN  (8*1024)
CRITICAL_SECTION cs;//定义临界区全局变量
BOOL32 GetCurLocalTime(s8 *pszTime,u32 dwLen)  //2016-03-15T12:48:01.330
{
	time_t t = time(0);
	strftime(pszTime, dwLen, "%FT%T", localtime(&t));
	return TRUE;
}
s32 LogWriteToFile(s8 *sData,s8 *pFilePATH)
{
	if (NULL == sData ||
		NULL == pFilePATH)
	{
		printf("Param Error!!!!\n");
		return FALSE;
	}
	s32 dwLen = strlen(sData);
	if (0 == dwLen)
	{
		printf("Param Error!!!!\n");
		return FALSE;
	}
	EnterCriticalSection(&cs);
	FILE *g_sFd = fopen(pFilePATH, "ab+");
	if (NULL == g_sFd)
	{
		printf("   Error!  NULL\n");
		LeaveCriticalSection(&cs);
		return FALSE;
	}
	//fseek(g_sFd,0,SEEK_END); 
	//fseek(g_sFd,0,SEEK_END);
	//u32 dwFileLen = ftell(g_sFd);

	time_t t = time(0);
	char tmp[128];
	strftime(tmp, sizeof(tmp), "%F %T", localtime(&t));
	char tmpData[P2P_LOG_BUF_LEN+128] = {0};
	snprintf(tmpData, sizeof(tmpData), "[%s]   %s", tmp, sData);
	dwLen = strlen(tmpData);
	u32 dwWriteLen = fwrite(tmpData, dwLen, 1, g_sFd);
	fflush(g_sFd);
	fclose(g_sFd);
	LeaveCriticalSection(&cs);
	printf("   dbgsaveData,File:%s ,DataLen:%u [%u]\n", pFilePATH, dwLen, dwWriteLen);
	return TRUE;
}

DWORD WINAPI  QueryPlayBackProgressThread(LPVOID lpParam)
{
	CnetsdkdemoDlg *ptDemoDlg = (CnetsdkdemoDlg *)lpParam;
	while(1)
	{
		ptDemoDlg->QueryPlayProgress();
		Sleep(1000);
	}
}
BOOL32 CnetsdkdemoDlg::QueryPlayProgress()
{
	TNetPlaybackState tPlaybackState = { 0 };
	if (0 != m_tPlayBackParm.nTaskId)
	{
		memset(&tPlaybackState, 0, sizeof(tPlaybackState));
		s32 nRet = NET_QueryPlaybackProgress(m_dwHandle, m_tPlayBackParm.nTaskId, m_tPlayBackParm.nChnId, &tPlaybackState);
		if (nRet != NET_OK)
		{
			return FALSE;
		}
		switch (tPlaybackState.ePlaybackState)
		{
		case PLAYBACKSTATE_PLAY:
		{
			//进度
			//float fProgress = (float)((ConvertTime(tPlaybackState.szCurrPlayTime) - m_ulRecStartTime) * 100) / m_dwRecTotalTime;
		}
		break;
		case PLAYBACKSTATE_OVER:
		{
			//End 
			CString strTmp;
			strTmp.Format("PlayBack Task  Finish!!!\n");
			AfxMessageBox(strTmp);
			StopPlayBack();
			m_btnPlayBack.SetWindowText("playback");
		}
		break;
		default:
		{
			//异常
		}
		break;
		}
	}
	if (0 != m_tPlayBackParm.nTaskRecDownLoadId)
	{
		memset(&tPlaybackState, 0, sizeof(tPlaybackState));
		s32 nRet = NET_QueryPlaybackProgress(m_dwHandle, m_tPlayBackParm.nTaskRecDownLoadId, m_tPlayBackParm.nRecDownLoadChnId, &tPlaybackState);
		if (nRet != NET_OK)
		{
			
			return FALSE;
		}
		switch (tPlaybackState.ePlaybackState)
		{
		case PLAYBACKSTATE_PLAY:
		{
			//进度
			//float fProgress = (float)((ConvertTime(tPlaybackState.szCurrPlayTime) - m_ulRecStartTime) * 100) / m_dwRecTotalTime;
			
		}
		break;
		case PLAYBACKSTATE_OVER:
		{
			//End 
			CString strTmp;
			strTmp.Format("DownLoad Task  Finish!!!\n");
			AfxMessageBox(strTmp);
			StopPlayBack();
			m_btnPlayBack.SetWindowText("playback");
		}
		break;

		default:
		{
			//异常
		}
		break;
		}
	}
	return TRUE;
}
#ifdef _P2P_MAP_
//线程函数：获取p2pmap日志
DWORD WINAPI DemoThread(LPVOID lpParam)
{
	int n = (int)lpParam;
	s8 achData[P2P_LOG_BUF_LEN] = { 0 };
	u32 dwLen = strlen(achData);
	snprintf(achData,sizeof(achData),"====================  P2P Log  =================\n");
	LogWriteToFile(achData,  g_achLogPath);
	s8 achTemp[P2P_LOG_BUF_LEN] = { 0 };
	while (1)
	{
		s32 nLogType = 0;
		memset(achTemp, 0, sizeof(achTemp));
		BOOL32 bGetLog = NPC_F_MPI_MON_DNP_GetLogData(
			sizeof(achTemp),				//[IN]Log information buffer size
			&nLogType,			//[OUT]Log type, see NPC_D_MON_LOG_INFO_TYPE_ * definition
			achTemp,			//[OUT]Log information buffer, caller allocates memory
			1					//[IN]Timeout (ms)
			);
		if (bGetLog && (strlen(achTemp)>0))
		{
			snprintf(achData, sizeof(achData), "%s  [type:%d]\n", achTemp, nLogType);
			LogWriteToFile(achData, g_achLogPath);
		}
		else
		{
			Sleep(50);
		}
	}
	return 0;
}
#endif
CnetsdkdemoDlg::CnetsdkdemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CnetsdkdemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset(&m_tNvrCap, 0, sizeof(TNetNvrCap));
	memset(&m_tPlayBackParm, 0, sizeof(m_tPlayBackParm));
	m_dwDecViewHandle = 0;
	m_dwDecPlayBackHandle = 0;
	m_bStartView = FALSE;
	m_bStartPlayBack = FALSE;
	memset(&m_tNvrChnList, 0, sizeof(m_tNvrChnList));
}

void CnetsdkdemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipaddrIp);
	DDX_Control(pDX, IDC_LIST1, m_listChnList);
	DDX_Control(pDX, IDC_EDIT1, m_editPort);
	DDX_Control(pDX, IDC_EDIT2, m_editUser);
	DDX_Control(pDX, IDC_EDIT3, m_editPwd);
	DDX_Control(pDX, IDC_RADIO_TCP, m_radioTcp);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_STATIC_PLAYBACK, m_staticPlayBack);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_btnView);
	DDX_Control(pDX, IDC_BUTTON_PLAYBACK, m_btnPlayBack);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_datetimePlayBack);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_datetimeTime);
}

BEGIN_MESSAGE_MAP(CnetsdkdemoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CnetsdkdemoDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_GET_CHN_LIST, &CnetsdkdemoDlg::OnBnClickedButtonGetChnList)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, &CnetsdkdemoDlg::OnBnClickedButtonView)
	ON_BN_CLICKED(IDC_BUTTON_PLAYBACK, &CnetsdkdemoDlg::OnBnClickedButtonPlayback)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CnetsdkdemoDlg 消息处理程序
BOOL CnetsdkdemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_listChnList.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);		//设置列表控件样式为详细信息
	DWORD dwStyle = m_listChnList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;							//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;								//网格线（只适用与report风格的listctrl）
	m_listChnList.SetExtendedStyle(dwStyle);					//设置扩展风格

	m_listChnList.InsertColumn(COL_CHNID, "CHN ID", LVCFMT_LEFT, 60);
	m_listChnList.InsertColumn(COL_CHNNAME, "Channel Name", LVCFMT_LEFT, 120);
	m_listChnList.InsertColumn(COL_STATE, "Status", LVCFMT_LEFT, 60);
	m_listChnList.InsertColumn(COL_IP, "IP", LVCFMT_LEFT, 120);
	m_listChnList.InsertColumn(COL_DEV_TYPE, "Mode", LVCFMT_LEFT, 80);
	m_listChnList.InsertColumn(COL_PROTO, "Protocol", LVCFMT_LEFT, 80);
	m_listChnList.InsertColumn(COL_PORT, "Port", LVCFMT_LEFT, 60);
	m_listChnList.InsertColumn(COL_DEV_MODEL, "Mode", LVCFMT_LEFT, 140);

	m_radioTcp.SetCheck(TRUE);

	m_ipaddrIp.SetAddress(ntohl(inet_addr(NET_SDK_ADDR)));  
	m_editPort.SetWindowText(NET_SDK_PORT);
	m_editUser.SetWindowText(NET_SDK_USER_NAME);
	m_editPwd.SetWindowText(NET_SDK_PASSWROD);

	s32 nRet = NET_Init();
	if (nRet != NET_OK)
	{
		CString strTmp;
		strTmp.Format("NET_Init fail, errcode = %d\n", nRet);
		AfxMessageBox(strTmp);
		return FALSE;
	}

	nRet = DEC_Init();
	if (nRet != NET_OK)
	{
		CString strTmp;
		strTmp.Format("DEC_Init fail, errcode = %d\n", nRet);
		AfxMessageBox(strTmp);
		return FALSE;
	}
	m_dwHandle = 0;
	m_bIsUmidUser = FALSE;
	m_pNetSerHandle = NULL;
	memset(&m_tConfig,0,sizeof(m_tConfig));
	NPC_CHAR		sUstIpAddr[32] = { 0 };
	NPC_INT			nUstPort = 0;
#ifdef _P2P_MAP_
	ReadConfig();
	if(m_bIsUmidUser)
	{
		//查询最优服务器(0=成功，其他失败)
		if (NPC_F_MPI_MON_DNP_GetBestSrv(
			"v0.api.umeye.com",				//[IN]服务器IP地址或域名
			8888,			//[IN]服务器端口号
			sUstIpAddr,				//[OUT]最优服务器IP
			&nUstPort			//[OUT]最优服务器端口
			) == 0)
		{
			snprintf(m_tConfig.sServerIpAddr,sizeof(m_tConfig.sServerIpAddr),"%s", sUstIpAddr);
			m_tConfig.usServerPort = nUstPort;
		}
		//debug 
		//snprintf(m_tConfig.sServerIpAddr,
		//		sizeof(m_tConfig.sServerIpAddr), "%s", "92.223.103.68");
		//m_tConfig.usServerPort = 8300;

		//创建设备网络端口映射服务
		m_pNetSerHandle = NPC_F_MPI_MON_DNP_CreateDevNetPortServer(
			m_tConfig.sServerIpAddr,			//[IN]服务器IP地址或域名
			m_tConfig.usServerPort,				//[IN]服务器端口号
			m_tConfig.sAuthId,					//[IN]认证ID
			m_tConfig.sAuthPwd					//[IN]认证密码
			);
		if (m_pNetSerHandle == NULL)
		{
			CString strTmp;
			strTmp.Format("NPC_F_MPI_MON_DNP_CreateDevNetPortServer    Error!!!\n");
			AfxMessageBox(strTmp);
			return FALSE;
		}
		DWORD ThreadID;
		HANDLE hThread = CreateThread(NULL, 0, DemoThread, (LPVOID)(1), 0, &ThreadID);
	}
#endif
	DWORD ThreadID_DownLoad;
	HANDLE hThread_DownLoad = CreateThread(NULL, 0, QueryPlayBackProgressThread, (LPVOID)(this), 0, &ThreadID_DownLoad);
	//初始化临界区
	InitializeCriticalSection(&cs);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CnetsdkdemoDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	NET_Logout(m_dwHandle);
	#ifdef _P2P_MAP_
	//p2p 清理 [多NVR时 只是NVR 退出登录如下处理]
	if (m_bIsUmidUser)
	{
		if (m_pNetSerHandle)
		{
			int i = 0;
			for (i = 0; i < NPC_NETPORT_TYPE_MAX; i++)
			{
				NPC_F_MPI_MON_DNP_DelPort(
						m_pNetSerHandle,					//[IN]设备网络端口
						m_tConfig.tDevCfg.awLocalPort[i]	//[IN]本地端口号
					);
			}
			memset(&m_tConfig.tDevCfg, 0, sizeof(m_tConfig.tDevCfg));
		}
		m_bIsUmidUser = FALSE;
	}
   #endif

	//整个进程退出处理
	NET_Cleanup();
	DEC_Cleanup();

#ifdef _P2P_MAP_
	if (m_pNetSerHandle)
	{
		NPC_F_MPI_MON_DNP_DestroyDevNetPortServer(
			m_pNetSerHandle				//[IN]设备网络端口
		);
		m_pNetSerHandle = NULL;
	}
#endif
	memset(&m_tConfig,0,sizeof(m_tConfig));
}

NPC_BOOL CnetsdkdemoDlg::Umid2ConnId(s8 *pUmid,s8 *pConnId,u32 dwConnIdLen )
{
	if (!pUmid || !pConnId)
	{
		return FALSE;
	}
	int nOffset = 0;
	u32 dwUmidLen = strlen(pUmid);
	//KANR7T38030486011B
	if (dwUmidLen < 18)
	{
		CString strTmp;
		strTmp.Format(" Cfg Umid Error!!   %s \n", pUmid);
		AfxMessageBox(strTmp);
		return FALSE;
	}
	char achChnNum[32] = { 0 };
	memset(achChnNum,0,sizeof(achChnNum));
	//achChnNum[0] = '0';
	//achChnNum[1] = 'x';
	achChnNum[0] = pUmid[dwUmidLen-1-2];
	achChnNum[1] = pUmid[dwUmidLen-1-1];
	nOffset = -strtol(achChnNum, NULL, 16);;

	char achConnId[32] = { 0 };
	int i = 0;
	for (i = 0; i < 12; i++)
	{
		achConnId[i] = pUmid[3 + i];
	}
	char* p = achConnId;
	const int nNumOfDigital = '9' - '0' + 1;
	const int nNumOfLetter = 'Z' - 'A' + 1;
	for (; p != NULL && *p != '\0'; p++) {
		if (*p >= '0' && *p <= '9') {
			*p = *p + nOffset % nNumOfDigital;
			if (*p > '9')
				*p -= nNumOfDigital;
			else if (*p < '0')
				*p += nNumOfDigital;
		}
		else if (*p >= 'A' && *p <= 'Z') {
			*p = *p + nOffset % nNumOfLetter;
			if (*p > 'Z')
				*p -= nNumOfLetter;
			else if (*p < 'A')
				*p += nNumOfLetter;
		}
		else {
			printf("StrOffset, error!!!\n");
			break;
		}
	}
	snprintf(pConnId, dwConnIdLen, "%s", achConnId);

	return TRUE;
}
NPC_BOOL CnetsdkdemoDlg::ReadConfig()
{
	NPC_CHAR sCfgFileName[256] = {0};
	NPC_BOOL bIfGetCfg;

	bIfGetCfg = TRUE;
	char achBuf[256];
	GetModuleFileName(NULL, achBuf, sizeof(achBuf));
	//删除文件名，只获得路径
	char *pChr = _tcsrchr(achBuf, _T('\\'));
	pChr++;
	*pChr = _T('\0');
	snprintf(g_achLocalPath, sizeof(g_achLocalPath), "%s", achBuf);
	snprintf(sCfgFileName, sizeof(sCfgFileName), "%s\\nts_dev_port_map_config.ini", g_achLocalPath);
	snprintf(g_achLogPath,sizeof(g_achLogPath), "%s\\p2plog.txt", g_achLocalPath);
	//NPC_CHAR							sServerIpAddr[512];						//服务器IP地址
	GetPrivateProfileString("NET", "ServerIpAddr", "", m_tConfig.sServerIpAddr, sizeof(m_tConfig.sServerIpAddr), sCfgFileName);
	if (m_tConfig.sServerIpAddr[0] == 0)
	{
		bIfGetCfg = FALSE;
		strcpy(m_tConfig.sServerIpAddr, "app.umeye.cn");
		CString strTmp;
		strTmp.Format(" Cfg File Not Exist!!  %s \n", sCfgFileName);
		AfxMessageBox(strTmp);
	}
	if (bIfGetCfg)
	{
		//NPC_USHORT						usServerPort;							//服务器端口号
		m_tConfig.usServerPort = GetPrivateProfileInt("NET", "ServerPort", 8300, sCfgFileName);
		if (m_tConfig.usServerPort == 0)
		{
			return FALSE;
		}

		//NPC_CHAR							sAuthId[64];							//认证ID
		GetPrivateProfileString("NET", "AuthId", "sdktest", m_tConfig.sAuthId, sizeof(m_tConfig.sAuthId), sCfgFileName);
		if (m_tConfig.sAuthId[0] == 0)
		{
			return FALSE;
		}

		//NPC_CHAR							sAuthPwd[64];							//认证密码
		GetPrivateProfileString("NET", "AuthPwd", "sdktest", m_tConfig.sAuthPwd, sizeof(m_tConfig.sAuthPwd), sCfgFileName);
		if (m_tConfig.sAuthPwd[0] == 0)
		{
			return FALSE;
		}
		//KANR7T38030486011B
		GetPrivateProfileString("NET", "Umid", "KANR7T38030486011B", m_tConfig.tDevCfg.sUMID, sizeof(m_tConfig.tDevCfg.sUMID), sCfgFileName);
		//A0C613637193
		Umid2ConnId(m_tConfig.tDevCfg.sUMID, m_tConfig.tDevCfg.sConnID, sizeof(m_tConfig.tDevCfg.sConnID));
		m_bIsUmidUser = GetPrivateProfileInt("NET", "IsUmidUser", 0, sCfgFileName);
	}
	else
	{
		snprintf(m_tConfig.sServerIpAddr, sizeof(m_tConfig.sServerIpAddr), "%s", "app.umeye.cn");
		m_tConfig.usServerPort = 8300;
		snprintf(m_tConfig.sAuthId, sizeof(m_tConfig.sAuthId), "%s", "sdktest");
		snprintf(m_tConfig.sAuthPwd, sizeof(m_tConfig.sAuthPwd), "%s", "sdktest");
		snprintf(m_tConfig.tDevCfg.sUMID, sizeof(m_tConfig.tDevCfg.sUMID), "%s", "KANR7T38030486011B");
		//A0C613637193
		Umid2ConnId(m_tConfig.tDevCfg.sUMID, m_tConfig.tDevCfg.sConnID, sizeof(m_tConfig.tDevCfg.sConnID));
		m_bIsUmidUser = 0;
	}

	return TRUE;
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CnetsdkdemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CnetsdkdemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CnetsdkdemoDlg::OnBnClickedButtonLogin()
{
	CString strIpAddr, srtPort;
	m_ipaddrIp.GetWindowText(strIpAddr);
	m_editPort.GetWindowText(srtPort);
	u16 wPort_NvrLocal = atoi(srtPort);

	CString strUserName, srtPasswd;
	m_editUser.GetWindowText(strUserName);
	m_editPwd.GetWindowText(srtPasswd);

	NPC_CHAR achUmIp[64] = { 0 };
	//m_bIsUmidUser = TRUE; //使用umeye方式登录  ReadConfig()
	if (0 != m_dwHandle)
	{
		StopPlayBack();
		StopView();
		s32 nLogOutRet = NET_Logout(m_dwHandle);
		m_dwHandle = 0;
	}
	snprintf(achUmIp, sizeof(achUmIp), "%s", strIpAddr);
	NPC_USHORT usLocalMapPort = 0;
	TRACE( "isUmid:%d\n",m_bIsUmidUser);
	if (m_bIsUmidUser && m_pNetSerHandle != NULL)
	{
#ifdef _P2P_MAP_
		//1.检查链接服务器状态
		//检测服务器连接状态，返回：0：未连接，1：正在连接，2：已连接，返回9表示错误
		NPC_INT	  nRet_NetConn = 0;
		u32 dwConnCnt = 0;
		nRet_NetConn = NPC_F_MPI_MON_DNP_CheckSrvConnState(
			m_pNetSerHandle				//[IN]设备网络端口
		);
		if (2 != nRet_NetConn)
		{
			CString strTmp;
			strTmp.Format("Connect Net Ser Faild! Waiting...   ServerIp:%s \n", m_tConfig.sServerIpAddr);
			AfxMessageBox(strTmp);
			//return;
		}
		do {
			nRet_NetConn = NPC_F_MPI_MON_DNP_CheckSrvConnState(
				m_pNetSerHandle				//[IN]设备网络端口
				);
			if (2 == nRet_NetConn)
			{
				//connect ok
				break;
			}
			Sleep(1000);
			dwConnCnt++;
		} while (dwConnCnt < 10);
		
		if (2 != nRet_NetConn)
		{
			CString strTmp;
			strTmp.Format("[Error]  Connect Net Ser TimeOut!!  ServerIp:%s \n", m_tConfig.sServerIpAddr);
			AfxMessageBox(strTmp);
			return;
		}

		//snprintf(m_tConfig.tDevCfg.sUMID, sizeof(m_tConfig.tDevCfg.sUMID), "%s","A0C613637193");
		NPC_USHORT usDestPort = wPort_NvrLocal;
		NPC_INT iRet = NPC_FALSE;
		NPC_INT	out_pQueryDevSpaceTime = 0;
		if (m_tConfig.tDevCfg.awLocalPort[NPC_NETPORT_TYPE_LOGIN] != 0)
		{
			NPC_F_MPI_MON_DNP_DelPort(
				m_pNetSerHandle,					//[IN]设备网络端口
				m_tConfig.tDevCfg.awLocalPort[NPC_NETPORT_TYPE_LOGIN]	//[IN]本地端口号
			);
			m_tConfig.tDevCfg.awLocalPort[NPC_NETPORT_TYPE_LOGIN] = 0;
		}
		if (m_tConfig.tDevCfg.awLocalPort[NPC_NETPORT_TYPE_RTSP] != 0)
		{
			NPC_F_MPI_MON_DNP_DelPort(
				m_pNetSerHandle,					//[IN]设备网络端口
				m_tConfig.tDevCfg.awLocalPort[NPC_NETPORT_TYPE_RTSP]	//[IN]本地端口号
			);
			m_tConfig.tDevCfg.awLocalPort[NPC_NETPORT_TYPE_RTSP] = 0;
		}
		//添加映射端口（连接目标地址），返回值：0：成功，其它失败
		if ((iRet = NPC_F_MPI_MON_DNP_AddPortByDestAddr(
			m_pNetSerHandle,			//[IN]设备网络端口
			m_tConfig.tDevCfg.sConnID,					//[IN]UMID
			achUmIp,			//[IN]目标IP地址（设备端连接的IP地址）
			usDestPort,				//[IN]目标端口号
			&usLocalMapPort			//[INOUT]本地端口号，如果值为0，则由系统分配端口号
			)) != 0)
		{
			m_tConfig.tDevCfg.awLocalPort[NPC_NETPORT_TYPE_LOGIN] = 0;
			CString strTmp;
			strTmp.Format("Add map port by dest addr fail, iRet=%d. DevPort:%d",iRet, usDestPort);
			AfxMessageBox(strTmp);
		}
		else
		{
			m_tConfig.tDevCfg.awLocalPort[NPC_NETPORT_TYPE_LOGIN] = usLocalMapPort;
		}
		usDestPort = 554;
		usLocalMapPort = 0;
		if ((iRet = NPC_F_MPI_MON_DNP_AddPortByDestAddr(
			m_pNetSerHandle,			//[IN]设备网络端口
			m_tConfig.tDevCfg.sConnID,					//[IN]UMID
			achUmIp,			//[IN]目标IP地址（设备端连接的IP地址）
			usDestPort,				//[IN]目标端口号
			&usLocalMapPort			//[INOUT]本地端口号，如果值为0，则由系统分配端口号
			)) != 0)
		{
			m_tConfig.tDevCfg.awLocalPort[NPC_NETPORT_TYPE_RTSP] = 0;
			CString strTmp;
			strTmp.Format("Add map port by dest addr fail, iRet=%d. DevPort:%d", iRet, usDestPort);
			AfxMessageBox(strTmp);
		}
		else
		{
			m_tConfig.tDevCfg.awLocalPort[NPC_NETPORT_TYPE_RTSP] = usLocalMapPort;
		}
#endif
	}
	

	TNetLoginInfo tLoginInfo = { 0 };
	
	if (m_bIsUmidUser)
	{
		tLoginInfo.dwNvrIp = inet_addr(achUmIp);
		tLoginInfo.wPort = htons(m_tConfig.tDevCfg.awLocalPort[NPC_NETPORT_TYPE_LOGIN]);
	}
	else
	{
		tLoginInfo.wPort = htons(wPort_NvrLocal);
		tLoginInfo.dwNvrIp = inet_addr(strIpAddr);
	}
	strcpy_s(tLoginInfo.szPwd, srtPasswd);
	strcpy_s(tLoginInfo.szUserName,strUserName);
	
	s32 nLoginRet = NET_Login(tLoginInfo, &m_dwHandle);
	if ( nLoginRet != NET_OK && ERR_NET_ALREADY_LOGIN != nLoginRet)
	{
		CString strTmp ;
		strTmp.Format("NET_Login fail, errno = %d\n", nLoginRet);
		AfxMessageBox(strTmp);
		return;
	}
	s32 nRet = NET_SetHeartbeatInterval(m_dwHandle, 29);
	nRet = NET_GetNvrCap(tLoginInfo.dwNvrIp, tLoginInfo.wPort, &m_tNvrCap);
	if (nRet != NET_OK)
	{
		CString strTmp;
		strTmp.Format("NET_GetNvrCap fail, errno = %d\n", nRet);
		AfxMessageBox(strTmp);
		return;
	}
	NET_RegNvrStateNty(m_dwHandle, NvrStateCallBack);
	UpdateChnList();

}

void CnetsdkdemoDlg::OnBnClickedButtonGetChnList()
{
	UpdateChnList();
}

void CnetsdkdemoDlg::OnBnClickedButtonView()
{
	if (m_bStartView)
	{
		StopView();
		m_btnView.SetWindowText("view");
	}
	else
	{
		s32 nChnId = 0;
		POSITION pos = m_listChnList.GetFirstSelectedItemPosition();       
		if (pos == NULL) 
		{
			AfxMessageBox("No items were selected!\n"); 
			return;
		}
		else       
		{            
			while (pos)            
			{                 
				int nItem = m_listChnList.GetNextSelectedItem(pos);                 

				nChnId = (s32)m_listChnList.GetItemData(nItem);
				break;
			} 
		}

		StartView(nChnId);
		m_bStartView = TRUE;
		m_btnView.SetWindowText("stop");
	}
}

void CnetsdkdemoDlg::OnBnClickedButtonPlayback()
{
	if(m_bStartPlayBack)
	{
		StopPlayBack();
		m_btnPlayBack.SetWindowText("playback");
	}
	else
	{
		s32 nChnId = 0;
		POSITION pos = m_listChnList.GetFirstSelectedItemPosition();       
		if (pos == NULL) 
		{
			AfxMessageBox("No items were selected!\n"); 
			return;
		}
		else       
		{            
			while (pos)            
			{                 
				int nItem = m_listChnList.GetNextSelectedItem(pos);                 

				nChnId = (s32)m_listChnList.GetItemData(nItem);
				break;
			} 
		}

		CTime c_Date;
		CTime c_Time;
		m_datetimePlayBack.GetTime(c_Date);
		m_datetimeTime.GetTime(c_Time);
		CString strDate = c_Date.Format("%Y-%m-%d");
		CString strTime = c_Time.Format("%H:%M:%S");

		CString strDateTime = strDate + "T" + strTime;

		if(m_radioTcp.GetCheck())
		{
			if(StartPlayBackByTCP(nChnId, CSTRING2CHAR(strDateTime)))
			{
				m_bStartPlayBack = TRUE;
				m_btnPlayBack.SetWindowText("stop");
			}
		}
		else{
			if(StartPlayBackByUDP(nChnId, CSTRING2CHAR(strDateTime)))
			{
				m_bStartPlayBack = TRUE;
				m_btnPlayBack.SetWindowText("stop");
			}
		}
	}

}

#define IP2STR(ip) (IP2string(ip).c_str())

void CnetsdkdemoDlg::UpdateChnList()
{
	if (m_tNvrChnList.patChnItem != NULL)
	{
		delete m_tNvrChnList.patChnItem;
		m_tNvrChnList.patChnItem = NULL;
	}

	//清空内容 
	m_listChnList.DeleteAllItems();

	//获取通道数据
	s32 nMaxChnNum = m_tNvrCap.nMaxChnNum;
	m_tNvrChnList.patChnItem = new TNetChnItem[nMaxChnNum];
	s32 nGetCount = 0;

	if ( m_tNvrChnList.patChnItem == NULL )
	{
		return;
	}
	memset(m_tNvrChnList.patChnItem, 0, sizeof(TNetChnItem) * nMaxChnNum);
	while ( nGetCount < nMaxChnNum )
	{
		TNetGetNvrChnList tGetNvrChnList = {0};
		TNetNvrChnList ptNvrChnList = {0};

		ptNvrChnList.patChnItem = &m_tNvrChnList.patChnItem[nGetCount];

		tGetNvrChnList.dwChnMask = CHNMASK_ALL;
		tGetNvrChnList.nChnIdStart = nGetCount + 1;
		tGetNvrChnList.nChnIdEnd = tGetNvrChnList.nChnIdStart + 31;
		tGetNvrChnList.nChnIdEnd = min(tGetNvrChnList.nChnIdEnd, nMaxChnNum);
		ptNvrChnList.wChnNum = tGetNvrChnList.nChnIdEnd - tGetNvrChnList.nChnIdStart + 1;
		s8 achLogData[NET_MAXLEN_512] = { 0 };
		u32 dwLen = strlen(achLogData);
		snprintf(achLogData, sizeof(achLogData), "NET_GetNvrChnList   Start !!! \n");
		LogWriteToFile(achLogData, g_achLogPath);

		s32 nRet = NET_GetNvrChnList(m_dwHandle, tGetNvrChnList, &ptNvrChnList);
		if (nRet != NET_OK)
		{	
			snprintf(achLogData, sizeof(achLogData), "NET_GetNvrChnList fail ,errno = %d\n", nRet);
			LogWriteToFile(achLogData, g_achLogPath);

			CString strTmp ;
			strTmp.Format("NET_GetNvrChnList fail ,errno = %d\n",nRet);
			AfxMessageBox(strTmp);
			if (m_tNvrChnList.patChnItem != NULL)
			{
				delete []m_tNvrChnList.patChnItem;
				m_tNvrChnList.patChnItem = NULL;
			}
			return;
		}
		else
		{
			nGetCount += ptNvrChnList.wChnNum;
			snprintf(achLogData, sizeof(achLogData), "NET_GetNvrChnList   Success !!! \n");
			LogWriteToFile(achLogData, g_achLogPath);
		}
	}

	s32 nItem = 0;
	s8 buffer[256] = {0};

	//更新显示列表
	for (s32 nChnItem = 0; nChnItem < nMaxChnNum; nChnItem++)
	{
		if(m_tNvrChnList.patChnItem[nChnItem].tChnInfo.bIdle)
			continue;

		int nCount = m_listChnList.GetItemCount();
		nItem = m_listChnList.InsertItem(nCount,0);		

		sprintf_s(buffer, ("D%d"), m_tNvrChnList.patChnItem[nChnItem].tChnInfo.nId);
		m_listChnList.SetItemText(nItem, COL_CHNID, buffer);	//通道id
		m_listChnList.SetItemData(nItem, m_tNvrChnList.patChnItem[nChnItem].tChnInfo.nId);

		m_listChnList.SetItemText(nItem, COL_CHNNAME, m_tNvrChnList.patChnItem[nChnItem].tChnInfo.szAlias);
		m_listChnList.SetItemText(nItem, COL_STATE, GetChnState(m_tNvrChnList.patChnItem[nChnItem].tChnInfo.tChnState));//通道别名
		m_listChnList.SetItemText(nItem, COL_IP, IP2STR(m_tNvrChnList.patChnItem[nChnItem].tDevItem.tDevInfo.dwIp));    //设备IP地址
		m_listChnList.SetItemText(nItem, COL_DEV_TYPE, GetDevType(m_tNvrChnList.patChnItem[nChnItem].tDevItem.tDevInfo.eDevType));    //设备类型IP地址
		m_listChnList.SetItemText(nItem, COL_PROTO, GetProtoType(m_tNvrChnList.patChnItem[nChnItem].tDevItem.tDevInfo.eProtoType));    //协议

		memset(buffer, 0, sizeof(buffer));
		sprintf_s(buffer, ("%d"), m_tNvrChnList.patChnItem[nChnItem].tDevItem.tDevInfo.wProtoPort);
		m_listChnList.SetItemText(nItem, COL_PORT, buffer);    //端口
		m_listChnList.SetItemText(nItem, COL_DEV_MODEL, m_tNvrChnList.patChnItem[nChnItem].tDevItem.tDevInfo.szDevModel);    //设备型号
	}
}

BOOL32 CnetsdkdemoDlg::StartView(s32 nChnId)
{
	if(nChnId < 1)
		return FALSE;

	TNetGetRtspRealStreamUrlParam tGetRtspRealStreamUrlParam = {0};
	TNetRtspRealStreamUrl tRtspRealStreamUrl = {0};

	tGetRtspRealStreamUrlParam.nChnId = nChnId;
	tGetRtspRealStreamUrlParam.eRtspTransType = RTSPTRANSTYPE_TCP;
	tGetRtspRealStreamUrlParam.nAudioEncNum = GetAudioEncNum(nChnId);
	for(u32 i = 0; i < tGetRtspRealStreamUrlParam.nAudioEncNum; i++)
	{
		tGetRtspRealStreamUrlParam.anAudioSrcId[i] = 1;
	}
	tGetRtspRealStreamUrlParam.nVideoEncNum = 1;
	for(u32 i = 0; i < tGetRtspRealStreamUrlParam.nVideoEncNum; i++)
	{
		tGetRtspRealStreamUrlParam.anVideoEncId[i] = 1;
	}

	s32 nRet = NET_GetRtspRealStreamUrl(m_dwHandle, tGetRtspRealStreamUrlParam, &tRtspRealStreamUrl);
	if(NET_OK != nRet)
	{
		return FALSE;
	}

	CString strUserName,srtPasswd,strRemoteIpAddr,strLocalIpAddr;
	m_editUser.GetWindowText(strUserName);
	m_editPwd.GetWindowText(srtPasswd);
	m_ipaddrIp.GetWindowText(strRemoteIpAddr);

	u32 dwNvrIp = inet_addr(strRemoteIpAddr);
	u32 dwLocalIp = 0;
	GetLocalIPEx(dwLocalIp, dwNvrIp);

	strLocalIpAddr.Format("%d.%d.%d.%d", dwLocalIp>>24, (dwLocalIp<<8)>>24, 
		(dwLocalIp<<16)>>24, (dwLocalIp<<24)>>24);

	//rtsp://172.16.192.100:554/realtime?chnid=2;vid=0;aid=0;agent=cgi
	//0x00daddc8 "rtsp://172.16.196.51:554/realtime?chnid=1;vid=0;aid=0;agent=cgi"
	CString strUrl = "rtsp://";
	if (m_bIsUmidUser)
	{
		char *pTemp = strstr(tRtspRealStreamUrl.szRtspUrl,":");
		if (pTemp != NULL)
		{
			pTemp++;
			pTemp = strstr(pTemp, ":");
			if (pTemp)
			{
				pTemp = strstr(pTemp, "/");
			}
		}
		strUrl += strRemoteIpAddr;
		char achLocalUmPort[32] = { 0 };
		snprintf(achLocalUmPort, sizeof(achLocalUmPort), ":%d", m_tConfig.tDevCfg.awLocalPort[NPC_NETPORT_TYPE_RTSP]);
		//strUrl += ":";
		strUrl += achLocalUmPort;
		strUrl += pTemp;
		//"rtsp://127.0.0.1:38762/realtime?chnid=5;vid=0;aid=0;agent=cgi"
	}
	else
	{
		//"rtsp://192.168.71.222:554/realtime?chnid=5;vid=0;aid=0;agent=cgi"
		char *pTemp = strstr(tRtspRealStreamUrl.szRtspUrl, ":");
		if (pTemp != NULL)
		{
			pTemp++;
			pTemp = strstr(pTemp, ":");
		}
		strUrl += strRemoteIpAddr;
		strUrl += pTemp;
	}
	

	if(m_radioTcp.GetCheck())
	{
		TDecPlayRtspTcpParam tDecPlayRtspTcpParam = {0};
		tDecPlayRtspTcpParam.nChnId = nChnId;
		tDecPlayRtspTcpParam.nAudioChnNum = 1;
		tDecPlayRtspTcpParam.tDecodeParam.hWnd = (void *)m_staticView.GetSafeHwnd();
		tDecPlayRtspTcpParam.tDecodeParam.eDecodeMode = DECODEMODE_REALTIME;
		tDecPlayRtspTcpParam.tDecodeParam.bHardwareAccelerate = FALSE;
		tDecPlayRtspTcpParam.tDecodeParam.tSyncAudio.bSyncToAudio = FALSE;
		tDecPlayRtspTcpParam.tDecodeParam.tSyncAudio.eSyncAudioType = SYNCAUDIOTYPE_RTP;
		strcpy(tDecPlayRtspTcpParam.tRtspParam.szUserName, strUserName.GetBuffer(strUserName.GetLength()) );
		strcpy(tDecPlayRtspTcpParam.tRtspParam.szPassword, srtPasswd.GetBuffer(srtPasswd.GetLength()));
		strcpy(tDecPlayRtspTcpParam.tRtspParam.szRtspURL, strUrl.GetBuffer(strUrl.GetLength()));

		nRet = DEC_StartPlayRtspTcp(tDecPlayRtspTcpParam, &m_dwDecViewHandle);

		if(nRet != DEC_OK)
		{
			CString strTmp ;
			strTmp.Format("DEC_StartPlay fail, errno = %d\n", nRet);
			AfxMessageBox(strTmp);
		}

		nRet = DEC_PlayAudio(m_dwDecViewHandle, PLAYAUDIOTYPE_ALL);
		if (nRet != DEC_OK)
		{
			return FALSE;
		}

		DEC_SetAudioVolume(1, 0xffff);
	}
	else
	{
		TDecPlayRtspUdpParam tDecPlayRtspUdpParam = {0};
		tDecPlayRtspUdpParam.nChnId = nChnId;
		tDecPlayRtspUdpParam.nAudioChnNum = 1;
		tDecPlayRtspUdpParam.tDecodeParam.hWnd = (void *)m_staticView.GetSafeHwnd();
		tDecPlayRtspUdpParam.tDecodeParam.eDecodeMode = DECODEMODE_REALTIME;
		tDecPlayRtspUdpParam.tDecodeParam.bHardwareAccelerate = FALSE;
		tDecPlayRtspUdpParam.tDecodeParam.tSyncAudio.bSyncToAudio = FALSE;
		tDecPlayRtspUdpParam.tDecodeParam.tSyncAudio.eSyncAudioType = SYNCAUDIOTYPE_RTP;
		strcpy(tDecPlayRtspUdpParam.tRtspUdpParam.tRtspParam.szUserName,strUserName.GetBuffer(strUserName.GetLength()));
		strcpy(tDecPlayRtspUdpParam.tRtspUdpParam.tRtspParam.szPassword, srtPasswd.GetBuffer(srtPasswd.GetLength()));
		strcpy(tDecPlayRtspUdpParam.tRtspUdpParam.tRtspParam.szRtspURL, strUrl.GetBuffer(strUrl.GetLength()));
		tDecPlayRtspUdpParam.tRtspUdpParam.tRtpParam.tLocalRtpNetParam.dwIp = inet_addr(CSTRING2CHAR(strLocalIpAddr));
		tDecPlayRtspUdpParam.tRtspUdpParam.tRtpParam.tLocalRtpNetParam.tVideoRtpPort.wRtpPort= 60204;
		tDecPlayRtspUdpParam.tRtspUdpParam.tRtpParam.tLocalRtpNetParam.tVideoRtpPort.wRtcpPort = 60205;
		tDecPlayRtspUdpParam.tRtspUdpParam.tRtpParam.tLocalRtpNetParam.tAudioRtpPort.wRtpPort = 60206;
		tDecPlayRtspUdpParam.tRtspUdpParam.tRtpParam.tLocalRtpNetParam.tAudioRtpPort.wRtcpPort = 60207;
		tDecPlayRtspUdpParam.tRtspUdpParam.tRtpParam.tRemoteRtpNetParam.dwIp = inet_addr(CSTRING2CHAR(strRemoteIpAddr));

		nRet = DEC_StartPlayRtspUdp(tDecPlayRtspUdpParam, &m_dwDecViewHandle);

		if(nRet != DEC_OK)
		{
			CString strTmp ;
			strTmp.Format("DEC_StartPlay fail, errno = %d\n", nRet);
			AfxMessageBox(strTmp);
		}
		nRet = DEC_PlayAudio(m_dwDecViewHandle, PLAYAUDIOTYPE_AUDIO1);
		if (nRet != DEC_OK)
		{
			return FALSE;
		}
	}

	return TRUE;
}

HBRUSH CnetsdkdemoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	switch(pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_VIEW:
	case IDC_STATIC_PLAYBACK:
		{
			HBRUSH B = CreateSolidBrush(RGB(0, 0, 0));
			return (HBRUSH)B;
		}
	default:
		break;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL32 CnetsdkdemoDlg::StopView()
{
	if(m_bStartView)
	{
		m_bStartView = FALSE;

		if(m_dwDecViewHandle == 0)
			return FALSE;

		s32 nRet = DEC_StopPlay(m_dwDecViewHandle);
		if(nRet != DEC_OK)
		{
			CString strTmp ;
			strTmp.Format("DEC_StopPlay fail, errno = %d\n", nRet);
			AfxMessageBox(strTmp);
			return FALSE;
		}

		m_dwDecViewHandle = 0;
		Invalidate(FALSE);
	}

	return TRUE;
}

BOOL32 CnetsdkdemoDlg::StartPlayDownLoadByTCP(s32 nChnId, const s8 *szStartTime)
{
	if (NULL == szStartTime)
	{
		return FALSE;
	}
	//[StartPlayBackByTCP]  nChnId:2  szStartTime = "2020-02-26T22:00:39"
	m_tPlayBackParm.nRecDownLoadChnId = nChnId;

	TNetPlaybackSrcChnList tPlaybackSrcChnList = { 0 };
	TNetPlaybackDstChnList tPlaybackDstChnList = { 0 };

	s32 nRet = NET_CreatePlaybackTaskEx(m_dwHandle, PLAYBACKTYPE_BYTIME, TRUE, tPlaybackDstChnList, &m_tPlayBackParm.nTaskRecDownLoadId, &tPlaybackSrcChnList);
	if (NET_OK != nRet)
	{
		cout << "[MainWindow::StartPlayBackByTCP] NET_CreatePlaybackTask nRet = " << nRet << " nTaskId = " << m_tPlayBackParm.nTaskId << endl;
		return FALSE;
	}

	TNetGetRtspPlaybackUrlParam tNetGetRtspPlaybackUrlParam = { 0 };
	TNetRtspPlaybackUrl tNetRtspPlaybackUrl = { 0 };

	memset(&tNetGetRtspPlaybackUrlParam,0,sizeof(tNetGetRtspPlaybackUrlParam));
	tNetGetRtspPlaybackUrlParam.nTaskID = m_tPlayBackParm.nTaskRecDownLoadId;
	tNetGetRtspPlaybackUrlParam.nChnID = m_tPlayBackParm.nRecDownLoadChnId;
	//2020-02-20T00:13:30</StartTime><EndTime>2020-02-20T00:47:15
	strcpy(tNetGetRtspPlaybackUrlParam.szStartTime, szStartTime);
	GetCurLocalTime(tNetGetRtspPlaybackUrlParam.szEndTime, sizeof(tNetGetRtspPlaybackUrlParam.szEndTime));

	strcpy(tNetGetRtspPlaybackUrlParam.szResStartTime, tNetGetRtspPlaybackUrlParam.szStartTime);
	strcpy(tNetGetRtspPlaybackUrlParam.szResEndTime, tNetGetRtspPlaybackUrlParam.szEndTime);
	tNetGetRtspPlaybackUrlParam.nAudioEncNum = 1;
	for (u32 i = 0; i < tNetGetRtspPlaybackUrlParam.nAudioEncNum; i++)
	{
		tNetGetRtspPlaybackUrlParam.anAudioSrcId[i] = 1;
	}
	tNetGetRtspPlaybackUrlParam.nVideoEncNum = 1;
	for (u32 i = 0; i < tNetGetRtspPlaybackUrlParam.nVideoEncNum; i++)
	{
		tNetGetRtspPlaybackUrlParam.anVideoEncId[i] = 1;
	}

	nRet = NET_GetRtspPlaybackUrl(m_dwHandle, tNetGetRtspPlaybackUrlParam, &tNetRtspPlaybackUrl);
	if (NET_OK != nRet)
	{
		cout << "[MainWindow::StartPlayBackByTCP] NET_GetRtspReplayUrl nRet = " << nRet << " szRtspUrl = " << tNetRtspPlaybackUrl.szRtspUrl << endl;
		return FALSE;
	}

	CString strUserName, srtPasswd, strRemoteIpAddr, strLocalIpAddr;
	m_editUser.GetWindowText(strUserName);
	m_editPwd.GetWindowText(srtPasswd);
	m_ipaddrIp.GetWindowText(strRemoteIpAddr);

	u32 dwNvrIp = inet_addr(strRemoteIpAddr);
	u32 dwLocalIp = 0;
	GetLocalIPEx(dwLocalIp, dwNvrIp);

	//rtsp://172.16.192.100:554/realtime?chnid=2;vid=0;aid=0;agent=cgi
	CString strUrl = "rtsp://";
	if (m_bIsUmidUser)
	{
		char *pTemp = strstr(tNetRtspPlaybackUrl.szRtspUrl, ":");
		if (pTemp != NULL)
		{
			pTemp++;
			pTemp = strstr(pTemp, ":");
			if (pTemp)
			{
				pTemp = strstr(pTemp, "/");
			}
		}
		strUrl += strRemoteIpAddr;
		char achLocalUmPort[32] = { 0 };
		snprintf(achLocalUmPort, sizeof(achLocalUmPort), ":%d", m_tConfig.tDevCfg.awLocalPort[NPC_NETPORT_TYPE_RTSP]);

		strUrl += achLocalUmPort;
		strUrl += pTemp;
	}
	else
	{

		char *pTemp = strstr(tNetRtspPlaybackUrl.szRtspUrl, ":");
		if (pTemp != NULL)
		{
			pTemp++;
			pTemp = strstr(pTemp, ":");
		}
		strUrl += strRemoteIpAddr;
		strUrl += pTemp;
	}
#if 0
	nRet = NET_StartPlaybackEx(m_dwHandle, m_tPlayBackParm.nTaskRecDownLoadId);
	if (nRet != DEC_OK)
	{
		return FALSE;
	}
#endif

	TDecPlayRtspTcpParam tDecPlayRtspTcpParam = { 0 };
	tDecPlayRtspTcpParam.nChnId = m_tPlayBackParm.nRecDownLoadChnId;
	tDecPlayRtspTcpParam.nAudioChnNum = 1;
	tDecPlayRtspTcpParam.tDecodeParam.hWnd = (void *)m_staticPlayBack.GetSafeHwnd();
	tDecPlayRtspTcpParam.tDecodeParam.eDecodeMode = DECODEMODE_REALTIME;
	tDecPlayRtspTcpParam.tDecodeParam.bHardwareAccelerate = FALSE;
	tDecPlayRtspTcpParam.tDecodeParam.tSyncAudio.bSyncToAudio = FALSE;
	tDecPlayRtspTcpParam.tDecodeParam.tSyncAudio.eSyncAudioType = SYNCAUDIOTYPE_RTP;
	strcpy(tDecPlayRtspTcpParam.tRtspParam.szUserName, CSTRING2CHAR(strUserName));
	strcpy(tDecPlayRtspTcpParam.tRtspParam.szPassword, CSTRING2CHAR(srtPasswd));
	strcpy(tDecPlayRtspTcpParam.tRtspParam.szRtspURL, CSTRING2CHAR(strUrl));

	TDecRecDownload tRecDownload;
	u32 dwHandle = 0;

	tRecDownload.tRtspParam = tDecPlayRtspTcpParam.tRtspParam;
	tRecDownload.nChnId = m_tPlayBackParm.nRecDownLoadChnId;
	tRecDownload.nAudioChnNum = 1;
	snprintf(tRecDownload.szRecFileName, sizeof(tRecDownload.szRecFileName), "%s\\RecDownload_%d.mp4", g_achLocalPath, tRecDownload.nChnId);
	s32 nRet_Down = DEC_StartRecDownloadTcp(tRecDownload, &dwHandle);
	if (DEC_OK != nRet_Down)
	{
		return FALSE;
	}
	m_dwDecRecDownLoadHandle = dwHandle;
	nRet = DEC_PlayAudio(m_dwDecRecDownLoadHandle, PLAYAUDIOTYPE_AUDIO1);
	if (nRet != DEC_OK)
	{
		return FALSE;
	}
	nRet = DEC_SetPlayCallBack(m_dwDecRecDownLoadHandle, PlayDownLoadCallBack, this);
	if (nRet != DEC_OK)
	{
		return FALSE;
	}

	return TRUE;
}


BOOL32 CnetsdkdemoDlg::StartPlayBackByTCP(s32 nChnId, const s8 *szStartTime)
{
	if(NULL == szStartTime)
	{
		return FALSE;
	}

#if 1
	//rec download
	StartPlayDownLoadByTCP(nChnId, szStartTime);
	return TRUE;
#else
	//rec playback

	//[StartPlayBackByTCP]  nChnId:2  szStartTime = "2020-02-26T22:00:39"
	m_tPlayBackParm.nChnId = nChnId;

	TNetPlaybackSrcChnList tPlaybackSrcChnList = {0};
	TNetPlaybackDstChnList tPlaybackDstChnList = {0};

	s32 nRet = NET_CreatePlaybackTaskEx(m_dwHandle, PLAYBACKTYPE_BYTIME, TRUE, tPlaybackDstChnList, &m_tPlayBackParm.nTaskId, &tPlaybackSrcChnList);
	if(NET_OK != nRet)
	{
		cout << "[MainWindow::StartPlayBackByTCP] NET_CreatePlaybackTask nRet = " << nRet << " nTaskId = " << m_tPlayBackParm.nTaskId << endl;
		return FALSE;
	}

	TNetGetRtspPlaybackUrlParam tNetGetRtspPlaybackUrlParam = {0};
	TNetRtspPlaybackUrl tNetRtspPlaybackUrl = {0};

	tNetGetRtspPlaybackUrlParam.nTaskID = m_tPlayBackParm.nTaskId;
	tNetGetRtspPlaybackUrlParam.nChnID = m_tPlayBackParm.nChnId;
	//2020-02-20T00:13:30</StartTime><EndTime>2020-02-20T00:47:15
	strcpy(tNetGetRtspPlaybackUrlParam.szStartTime, szStartTime);


	GetCurLocalTime(tNetGetRtspPlaybackUrlParam.szEndTime, sizeof(tNetGetRtspPlaybackUrlParam.szEndTime));

	strcpy(tNetGetRtspPlaybackUrlParam.szResStartTime, tNetGetRtspPlaybackUrlParam.szStartTime);
	strcpy(tNetGetRtspPlaybackUrlParam.szResEndTime, tNetGetRtspPlaybackUrlParam.szEndTime);
	tNetGetRtspPlaybackUrlParam.nAudioEncNum = 1;
	for(u32 i = 0; i < tNetGetRtspPlaybackUrlParam.nAudioEncNum; i++)
	{
		tNetGetRtspPlaybackUrlParam.anAudioSrcId[i] = 1;
	}
	tNetGetRtspPlaybackUrlParam.nVideoEncNum = 1;
	for(u32 i = 0; i < tNetGetRtspPlaybackUrlParam.nVideoEncNum; i++)
	{
		tNetGetRtspPlaybackUrlParam.anVideoEncId[i] = 1;
	}

	nRet = NET_GetRtspPlaybackUrl(m_dwHandle, tNetGetRtspPlaybackUrlParam, &tNetRtspPlaybackUrl);
	if(NET_OK != nRet)
	{
		cout << "[MainWindow::StartPlayBackByTCP] NET_GetRtspReplayUrl nRet = " << nRet << " szRtspUrl = " << tNetRtspPlaybackUrl.szRtspUrl << endl;
		return FALSE;
	}

	CString strUserName,srtPasswd,strRemoteIpAddr,strLocalIpAddr;
	m_editUser.GetWindowText(strUserName);
	m_editPwd.GetWindowText(srtPasswd);
	m_ipaddrIp.GetWindowText(strRemoteIpAddr);

	u32 dwNvrIp = inet_addr(strRemoteIpAddr);
	u32 dwLocalIp = 0;
	GetLocalIPEx(dwLocalIp, dwNvrIp);

	//rtsp://172.16.192.100:554/realtime?chnid=2;vid=0;aid=0;agent=cgi
	CString strUrl = "rtsp://";
	if (m_bIsUmidUser)
	{
		char *pTemp = strstr(tNetRtspPlaybackUrl.szRtspUrl, ":");
		if (pTemp != NULL)
		{
			pTemp++;
			pTemp = strstr(pTemp, ":");
			if (pTemp)
			{
				pTemp = strstr(pTemp, "/");
			}
		}
		strUrl += strRemoteIpAddr;
		char achLocalUmPort[32] = { 0 };
		snprintf(achLocalUmPort, sizeof(achLocalUmPort), ":%d", m_tConfig.tDevCfg.awLocalPort[NPC_NETPORT_TYPE_RTSP]);

		strUrl += achLocalUmPort;
		strUrl += pTemp;
	}
	else
	{

		char *pTemp = strstr(tNetRtspPlaybackUrl.szRtspUrl, ":");
		if (pTemp != NULL)
		{
			pTemp++;
			pTemp = strstr(pTemp, ":");
		}
		strUrl += strRemoteIpAddr;
		strUrl += pTemp;
	}
#if 0
	nRet = NET_StartPlaybackEx(m_dwHandle, m_tPlayBackParm.nTaskId);
	if(nRet != DEC_OK)
	{
		return FALSE;
	}
#endif

	TDecPlayRtspTcpParam tDecPlayRtspTcpParam = { 0 };
	tDecPlayRtspTcpParam.nChnId = m_tPlayBackParm.nChnId;
	tDecPlayRtspTcpParam.nAudioChnNum = 1;
	tDecPlayRtspTcpParam.tDecodeParam.hWnd = (void *)m_staticPlayBack.GetSafeHwnd();
	tDecPlayRtspTcpParam.tDecodeParam.eDecodeMode = DECODEMODE_REALTIME;
	tDecPlayRtspTcpParam.tDecodeParam.bHardwareAccelerate = FALSE;
	tDecPlayRtspTcpParam.tDecodeParam.tSyncAudio.bSyncToAudio = FALSE;
	tDecPlayRtspTcpParam.tDecodeParam.tSyncAudio.eSyncAudioType = SYNCAUDIOTYPE_RTP;
	strcpy(tDecPlayRtspTcpParam.tRtspParam.szUserName, CSTRING2CHAR(strUserName));
	strcpy(tDecPlayRtspTcpParam.tRtspParam.szPassword, CSTRING2CHAR(srtPasswd));
	strcpy(tDecPlayRtspTcpParam.tRtspParam.szRtspURL, CSTRING2CHAR(strUrl));

	nRet = DEC_StartPlayRtspTcp(tDecPlayRtspTcpParam, &m_dwDecPlayBackHandle);
	if (nRet != DEC_OK)
	{
		return FALSE;
	}

	nRet = DEC_PlayAudio(m_dwDecPlayBackHandle, PLAYAUDIOTYPE_AUDIO1);
	if (nRet != DEC_OK)
	{
		return FALSE;
	}

	nRet = DEC_SetPlayCallBack(m_dwDecPlayBackHandle, PlayCallBack, this);
	if (nRet != DEC_OK)
	{
		return FALSE;
	}

	return TRUE;
#endif
}


BOOL32 CnetsdkdemoDlg::StartPlayBackByUDP(s32 nChnId, const s8 *szStartTime)
{
	if(NULL == szStartTime)
	{
		return FALSE;
	}

	CString strUserName,srtPasswd,strRemoteIpAddr,strLocalIpAddr;
	m_editUser.GetWindowText(strUserName);
	m_editPwd.GetWindowText(srtPasswd);
	m_ipaddrIp.GetWindowText(strRemoteIpAddr);

	u32 dwNvrIp = inet_addr(strRemoteIpAddr);
	u32 dwLocalIp = 0;
	GetLocalIPEx(dwLocalIp, dwNvrIp);

	strLocalIpAddr.Format("%d.%d.%d.%d", dwLocalIp>>24, (dwLocalIp<<8)>>24, 
		(dwLocalIp<<16)>>24, (dwLocalIp<<24)>>24);

	m_tPlayBackParm.nChnId = nChnId;

	TNetPlaybackSrcChnList tPlaybackSrcChnList = {0};
	TNetPlaybackDstChnList tPlaybackDstChnList = {0};
	tPlaybackDstChnList.nNum = 1;
	tPlaybackDstChnList.atPlaybackDstChnItem[0].nChnId = nChnId;
	strcpy(tPlaybackDstChnList.atPlaybackDstChnItem[0].szStartTime, szStartTime);
	//strcpy(tPlaybackDstChnList.atPlaybackDstChnItem[0].szEndTime, "2028-08-04T06:00:00");
	strcpy(tPlaybackDstChnList.atPlaybackDstChnItem[0].szResStartTime, szStartTime);
	strcpy(tPlaybackDstChnList.atPlaybackDstChnItem[0].szResEndTime, "");
	tPlaybackDstChnList.atPlaybackDstChnItem[0].dwDstIp = inet_addr(CSTRING2CHAR(strLocalIpAddr));
	tPlaybackDstChnList.atPlaybackDstChnItem[0].nDstAudioItemNum = 1;
	for(u32 i = 0; i < tPlaybackDstChnList.atPlaybackDstChnItem[0].nDstAudioItemNum; i++)
	{
		tPlaybackDstChnList.atPlaybackDstChnItem[0].atDstAudioItem[i].wDstRtcpPort = 60711;
		tPlaybackDstChnList.atPlaybackDstChnItem[0].atDstAudioItem[i].wDstRtpPort = 60710;
	}
	tPlaybackDstChnList.atPlaybackDstChnItem[0].nDstVideoItemNum = 1;
	for(u32 i = 0; i < tPlaybackDstChnList.atPlaybackDstChnItem[0].nDstVideoItemNum; i++)
	{
		tPlaybackDstChnList.atPlaybackDstChnItem[0].atDstVideoItem[i].wDstRtcpPort = 60709;
		tPlaybackDstChnList.atPlaybackDstChnItem[0].atDstVideoItem[i].wDstRtpPort = 60708;
	}

	s32 nRet = NET_CreatePlaybackTaskEx(m_dwHandle, PLAYBACKTYPE_BYTIME, FALSE, tPlaybackDstChnList, &m_tPlayBackParm.nTaskId, &tPlaybackSrcChnList);
	if(NET_OK != nRet)
	{
		cout << "[MainWindow::StartPlayBackByTCP] NET_CreatePlaybackTask nRet = " << nRet << " nTaskId = " << m_tPlayBackParm.nTaskId << endl;
		return FALSE;
	}

	TDecPlayRtpParam tDecPlayRtpParam = {0};
	tDecPlayRtpParam.nChnId = m_tPlayBackParm.nChnId;
	tDecPlayRtpParam.nAudioChnNum = 1;
	tDecPlayRtpParam.tDecodeParam.hWnd = (void *)m_staticPlayBack.GetSafeHwnd();
	tDecPlayRtpParam.tDecodeParam.eDecodeMode = DECODEMODE_REALTIME;
	tDecPlayRtpParam.tDecodeParam.bHardwareAccelerate = FALSE;
	tDecPlayRtpParam.tDecodeParam.tSyncAudio.bSyncToAudio = FALSE;
	tDecPlayRtpParam.tDecodeParam.tSyncAudio.eSyncAudioType = SYNCAUDIOTYPE_RTP;
	tDecPlayRtpParam.tRtpParam.tLocalRtpNetParam.dwIp = inet_addr(CSTRING2CHAR(strLocalIpAddr));
	tDecPlayRtpParam.tRtpParam.tLocalRtpNetParam.tVideoRtpPort.wRtpPort = 60708;
	tDecPlayRtpParam.tRtpParam.tLocalRtpNetParam.tVideoRtpPort.wRtcpPort = 60709;
	tDecPlayRtpParam.tRtpParam.tLocalRtpNetParam.tAudioRtpPort.wRtpPort = 60710;
	tDecPlayRtpParam.tRtpParam.tLocalRtpNetParam.tAudioRtpPort.wRtcpPort = 60711;
	tDecPlayRtpParam.tRtpParam.tRemoteRtpNetParam.tVideoRtpPort.wRtpPort = tPlaybackSrcChnList.atPlaybackSrcChnItem[0].atSrcVideoItem[0].wSrcRtpPort;
	tDecPlayRtpParam.tRtpParam.tRemoteRtpNetParam.tVideoRtpPort.wRtcpPort = tPlaybackSrcChnList.atPlaybackSrcChnItem[0].atSrcVideoItem[0].wSrcRtcpPort;
	tDecPlayRtpParam.tRtpParam.tRemoteRtpNetParam.tAudioRtpPort.wRtpPort = tPlaybackSrcChnList.atPlaybackSrcChnItem[0].atSrcAudioItem[0].wSrcRtpPort;
	tDecPlayRtpParam.tRtpParam.tRemoteRtpNetParam.tAudioRtpPort.wRtcpPort = tPlaybackSrcChnList.atPlaybackSrcChnItem[0].atSrcAudioItem[0].wSrcRtcpPort;
	tDecPlayRtpParam.tRtpParam.tRemoteRtpNetParam.dwIp = inet_addr(CSTRING2CHAR(strRemoteIpAddr));

	nRet = DEC_StartPlayRtp(tDecPlayRtpParam, &m_dwDecPlayBackHandle);
	if(nRet != DEC_OK)
	{
		return FALSE;
	}

	nRet = NET_StartPlaybackEx(m_dwHandle, m_tPlayBackParm.nTaskId);
	if(nRet != DEC_OK)
	{
		return FALSE;
	}

	return TRUE;
}
BOOL32 CnetsdkdemoDlg::NetStartPlayDownLoad(u32 dwHandle)
{
	if (dwHandle == m_dwDecRecDownLoadHandle)
	{
		s32 nRet = NET_StartPlaybackEx(m_dwHandle, m_tPlayBackParm.nTaskRecDownLoadId);
		cout << "[MainWindow::on_btn_playback_clicked] NET_StartPlaybackEx nRet = " << nRet << " nTaskId = " << m_tPlayBackParm.nTaskRecDownLoadId << endl;
		return nRet;
	}
	return TRUE;
}

BOOL32 CnetsdkdemoDlg::NetStartPlayBack(u32 dwHandle)
{
	if(dwHandle == m_dwDecPlayBackHandle)
	{
		s32 nRet = NET_StartPlaybackEx(m_dwHandle, m_tPlayBackParm.nTaskId);
		cout << "[MainWindow::on_btn_playback_clicked] NET_StartPlaybackEx nRet = " << nRet << " nTaskId = " << m_tPlayBackParm.nTaskId << endl;
		return nRet;
	}
	return TRUE;
}

BOOL32 CnetsdkdemoDlg::StopPlayBack()
{
	if (m_bStartPlayBack)
	{
		m_bStartPlayBack = FALSE;

		if (0 != m_dwDecPlayBackHandle)
		{
			NET_StopPlaybackEx(m_dwHandle, m_tPlayBackParm.nTaskId, m_tPlayBackParm.nChnId);
			NET_DestroyPlaybackTaskEx(m_dwHandle, m_tPlayBackParm.nTaskId);
			s32 nRet = DEC_StopPlay(m_dwDecPlayBackHandle);
			if (nRet != DEC_OK)
			{
				return FALSE;
			}
			m_tPlayBackParm.nTaskId = 0;
			m_dwDecPlayBackHandle = 0;
		}
		if (0 != m_dwDecRecDownLoadHandle)
		{
			 NET_StopPlaybackEx(m_dwHandle, m_tPlayBackParm.nTaskRecDownLoadId, m_tPlayBackParm.nRecDownLoadChnId);
			 NET_DestroyPlaybackTaskEx(m_dwHandle, m_tPlayBackParm.nTaskRecDownLoadId);
			s32 nRet_DownStop= DEC_StopRecDownload(m_dwDecRecDownLoadHandle);
			if (DEC_OK != nRet_DownStop)
			{
				return FALSE;
			}
			m_dwDecRecDownLoadHandle = 0;
			m_tPlayBackParm.nTaskRecDownLoadId = 0;
		}
		Invalidate(FALSE);
	}

	return TRUE;
}

s32 CnetsdkdemoDlg::GetAudioEncNum( s32 nChnId )
{
	if (nChnId >= 1 && nChnId <= m_tNvrCap.nMaxChnNum)
	{
		return m_tNvrChnList.patChnItem[nChnId - 1].tDevItem.tDevInfo.wAudioEncNum;
	}

	return 0;
}

///////////////////////////////////////////////

void PlayCallBack(TDecCallBackInfo tCallBackInfo)
{
	cout << "[PlayCallBack] NET_StartPlaybackEx tCallBackInfo.dwHandle = " << tCallBackInfo.dwHandle << " tCallBackInfo.ePlayState = " << tCallBackInfo.ePlayState << endl;

	if(PLAYSTATE_RTSP_READY == tCallBackInfo.ePlayState)
	{
		((CnetsdkdemoDlg*)tCallBackInfo.pContext)->NetStartPlayBack(tCallBackInfo.dwHandle);
	}
}
void PlayDownLoadCallBack(TDecCallBackInfo tCallBackInfo)
{
	cout << "[PlayCallBack] NET_StartPlaybackEx tCallBackInfo.dwHandle = " << tCallBackInfo.dwHandle << " tCallBackInfo.ePlayState = " << tCallBackInfo.ePlayState << endl;

	if (PLAYSTATE_RTSP_READY == tCallBackInfo.ePlayState)
	{
		((CnetsdkdemoDlg*)tCallBackInfo.pContext)->NetStartPlayDownLoad(tCallBackInfo.dwHandle);
	}
}

void NvrStateCallBack(u32 dwHandle, TNetNvrState tNvrState)
{

}

string IP2string( u32 dwIP )
{
	std::ostringstream os;
	os<<(int)(dwIP&0xFF)<<'.'<<(int)((dwIP>>8)&0xFF)<<'.'<<(int)((dwIP>>16)&0xFF)<<'.'<<(int)((dwIP>>24)&0xFF)<<'\0';
	string strIP(os.str());
	return strIP;
}

CString GetChnState(TNetChnState tNetChn)
{
	if ( !tNetChn.bDevOnline )
	{
		//switch(tNetChn.eDevErrNo)
		//{
		//case DEVERRNO_LINKING:
		//	return "上线中";
		//case DEVERRNO_CONNECTFAILED:
		//	return "连接失败";
		//case DEVERRNO_AUTHFAILED:
		//	return "认证失败";
		//case DEVERRNO_PARAMERR:
		//	return "参数错误";
		//case DEVERRNO_CHNABNORMAL:
		//	return "通信失败";
		//case DEVERRNO_NOSTREAM:
		//	return "取流失败";
		//case DEVERRNO_NOREGISTER:
		//	return "未注册";
		//default:
		//	return "offline";
		//}

		return "offline";
	}

	return "online";
}

CString GetDevType(EDevType eDevType)
{
	CString strType;
	switch(eDevType)
	{
	case DEVTYPE_IPC:
		strType = "IPC";
		break;
	case DEVTYPE_DVS:
		strType = "DVS";
		break;
	case DEVTYPE_DVR:
		strType = "DVR";
		break;
	case DEVTYPE_NVR:
		strType = "NVR";
		break;
	default:
		strType = "unkonw";
		break;
	}
	return strType;
}

CString GetProtoType(EProtoType eProtoType)
{
	CString strType;
	switch(eProtoType)
	{
	case PROTOTYPE_ONVIF:
		strType = "onvif";
		break;
	case PROTOTYPE_GB28181:
		strType = "GB28181";
		break;
	case PROTOTYPE_RTSP:
		strType = "RTSP";
		break;
	case PROTOTYPE_VSIP:
		strType = "VSIP";
		break;
	case PROTOTYPE_IPCSEARCH:
		strType = "ipcsearch";
		break;
	default:
		strType = "unkonw";
		break;
	}

	return strType.MakeUpper();
}

s32 GetLocalIPEx(u32 &dwLocalIP, u32 dwDestIP)
{	
	char szHostName[MAX_PATH] = {0};

	dwDestIP = ntohl(dwDestIP);

	gethostname(szHostName, MAX_PATH);
	hostent* tHost = gethostbyname(szHostName);

	if(tHost != NULL)
	{
		if (dwDestIP == 0)
		{
			if (tHost->h_addr_list[0] != NULL)
			{
				memcpy (&dwLocalIP, tHost->h_addr_list[0],tHost->h_length);
				dwLocalIP = ntohl(dwLocalIP);
				return TRUE;
			}
			else
			{
				dwLocalIP = 0;
				return FALSE;
			}
		}
		else
		{
			// 寻找与dwDestIP同网段的IP
			DWORD dwIP = 0;
			dwLocalIP = 0;

			int nSimilarity =0;
			for (int nAdapter=0; tHost->h_addr_list[nAdapter]; nAdapter++)
			{
				memcpy (&dwIP, tHost->h_addr_list[nAdapter],tHost->h_length);
				dwIP = ntohl(dwIP);
				BYTE a[4], b[4];
				a[0]= dwIP>> 24;a[1]= dwIP>> 16;a[2]= dwIP>> 8;a[3]= dwIP;
				b[0]= dwDestIP>> 24;b[1]= dwDestIP>> 16;b[2]= dwDestIP>> 8;b[3]= dwDestIP;

				int nSimilarity1 =0;
				for (int i=0; i < 4; i++)
					if(a[i] == b[i])
					{
						nSimilarity1++;
					}
					else
					{
						break;
					}

					if(nSimilarity1 > nSimilarity)
					{
						nSimilarity = nSimilarity1;
						dwLocalIP = dwIP;
					}
			}

			// 如果没有找到同网段的IP，使用第一个IP
			if (nSimilarity == 0)
			{
				memcpy (&dwIP, tHost->h_addr_list[0],tHost->h_length);
				dwLocalIP = ntohl(dwIP);
			}

			printf("dev ip:%d:%d:%d:%d\n", dwDestIP>>24, (dwDestIP<<8)>>24, 
				(dwDestIP<<16)>>24, (dwDestIP<<24)>>24 );	// 设备IP地址
			printf("local ip:%d:%d:%d:%d\n", dwLocalIP>>24, (dwLocalIP<<8)>>24, 
				(dwLocalIP<<16)>>24, (dwLocalIP<<24)>>24 );	// 本机IP地址
		}
	}
	else
	{
		dwLocalIP = 0;
		return FALSE;
	}

	return TRUE;
}
