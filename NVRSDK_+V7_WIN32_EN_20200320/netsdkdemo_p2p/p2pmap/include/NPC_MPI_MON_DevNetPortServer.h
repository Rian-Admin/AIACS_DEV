/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_MPI_MON_DevNetPortServer.h
 * Version       :  1.0
 * Author        :  CCH
 * DateTime      :  2012-07-18
 * Description   :  Device network port mapping service module
 *----------------------------------------------------------------------------*/

#ifndef __NPC_MPI_MON_DEVNETPORTSERVER_H
#define __NPC_MPI_MON_DEVNETPORTSERVER_H

#include "NPC_TypeDef.h"

//Export import definition
#ifdef _WIN32
	#ifdef _NPC_MPI_MON_DNP_EXPORTS
		#define NPC_MPI_MON_DNP_API __declspec(dllexport)
	#else
		#ifdef _NPC_MPI_MON_DNP_IMPORT
			#define NPC_MPI_MON_DNP_API __declspec(dllimport)
		#else
			#define NPC_MPI_MON_DNP_API
		#endif
	#endif
#else
	#define NPC_MPI_MON_DNP_API
#endif

//Definition of callback function through transmission
typedef		void	(*PNPC_F_MPI_MON_CALLBACK_DevNetPortP2pPortData)(
IN		PNPC_VOID							in_pUserData,										//[IN]User data
IN		PNPC_VOID							in_pPortData,										//[IN]Port data
IN		PNPC_BYTE							in_pDataBuf,                                        //[IN]Data buffer
IN		NPC_INT								in_iDataLen                                         //[IN]Data length
);

// C++ support
#ifdef __cplusplus
extern "C"
{
#endif

#if 1
//----------------------------------------------------------------------------------------
//Query the optimal server (0 = success, others fail)
NPC_MPI_MON_DNP_API		NPC_INT	NPC_F_MPI_MON_DNP_GetBestSrv(
	IN		PNPC_CHAR							in_pServerAddr,				//[IN]Server IP address or domain name
	IN		NPC_USHORT							in_usServerPort,			//[IN]Server port number
	OUT		PNPC_CHAR							out_pBestSrvIp,				//[OUT]Optimal server IP
	OUT		PNPC_INT							out_pBestSrvPotr			//[OUT]Optimal server port
	);
#endif

//----------------------------------------------------------------------------------------
//Create device network port mapping service
NPC_MPI_MON_DNP_API		NPC_HANDLE	NPC_F_MPI_MON_DNP_CreateDevNetPortServer(
	IN		PNPC_CHAR							in_pServerAddr,				//[IN]Server IP address or domain name
	IN		NPC_USHORT							in_usServerPort,			//[IN]Server port number
	IN		PNPC_CHAR							in_pAuthId,					//[IN]Authentication ID
	IN		PNPC_CHAR							in_pAuthPwd,                //[IN]Authentication password
    IN      NPC_BOOL                            in_bIfConnSpecSrv=TRUE      //[IN]Whether to connect to the specified server
	);

//Destroy device network port mapping service
NPC_MPI_MON_DNP_API		NPC_VOID	NPC_F_MPI_MON_DNP_DestroyDevNetPortServer(
	IN		NPC_HANDLE							in_hDevNetPort				//[IN]Device network port
	);

//Add mapping port, return value: 0: success, others fail
//Error code:
//7:Waiting for login server timeout
//8:Failed to find free TCP listening port
//9:Parameter error
//600001:Failed to parse connection parameters
//600002:Object creation failed
//600004:Create mapping port failed
//600009:Parameter error
//630001:Protocol error or system call failed
//630002:Connect device failed
//630004:Allocate resources failed
//630006:Access to the server failed (access timed out or disconnected or server operation failed)
//630009:Failure for other reasons
//630101:Username error
//630102:Password error
//630104:Connecting to server
//630105:Not connected to Server
//630111:Insufficient authority
//630112:Device is offline
NPC_MPI_MON_DNP_API		NPC_INT		NPC_F_MPI_MON_DNP_AddPort(
	IN		NPC_HANDLE							in_hDevNetPort,				//[IN]Device network port
	IN		PNPC_CHAR							in_pUmid,					//[IN]UMID
	INOUT	PNPC_USHORT							inout_pLocalPort,			//[INOUT]Local port number. If the value is 0, the system assigns the port number.
	IN		NPC_INT								in_iTimeout=20000			//[IN]Timeout (ms)
	);

//Add mapping port (connection destination address), return value: 0: success, others fail
//Error code:
//7:Waiting for login server timeout
//8:Failed to find free TCP listening port
//9:Parameter error
//600001:Failed to parse connection parameters
//600002:Object creation failed
//600004:Create mapping port failed
//600009:Parameter error
//630001:Protocol error or system call failed
//630002:Connect device failed
//630004:Allocate resources failed
//630006:Access to the server failed (access timed out or disconnected or server operation failed)
//630009:Failure for other reasons
//630101:Username error
//630102:Password error
//630104:Connecting to server
//630105:Not connected to Server
//630111:Insufficient authority
//630112:Device is offline
NPC_MPI_MON_DNP_API		NPC_INT		NPC_F_MPI_MON_DNP_AddPortByDestAddr(
	IN		NPC_HANDLE							in_hDevNetPort,				//[IN]Device network port
	IN		PNPC_CHAR							in_pUmid,					//[IN]UMID
	IN		PNPC_CHAR							in_pDestIpaddr,				//[IN]Destination IP address (the IP address of the device)
	IN		NPC_USHORT							in_usDestPort,				//[IN]Destination port number
	INOUT	PNPC_USHORT							inout_pLocalPort,			//[INOUT]Local port number. If the value is 0, the system assigns the port number.
	IN		NPC_INT								in_iTimeout=20000			//[IN]Timeout (ms)
	);

//Add mapping port (connection destination address), return value: 0: success, others fail
//Error code:
//7:Waiting for login server timeout
//8:Failed to find free TCP listening port
//9:Parameter error
//600001:Failed to parse connection parameters
//600002:Object creation failed
//600004:Create mapping port failed
//600009:Parameter error
//630001:Protocol error or system call failed
//630002:Connect device failed
//630004:Allocate resources failed
//630006:Access to the server failed (access timed out or disconnected or server operation failed)
//630009:Failure for other reasons
//630101:Username error
//630102:Password error
//630104:Connecting to server
//630105:Not connected to Server
//630111:Insufficient authority
//630112:Device is offline
NPC_MPI_MON_DNP_API		NPC_INT		NPC_F_MPI_MON_DNP_AddPortByChNo(
	IN		NPC_HANDLE							in_hDevNetPort,				//[IN]Device network port
	IN		PNPC_CHAR							in_pUmid,					//[IN]UMID
	IN		NPC_INT								in_iDestChNo,				//[IN]Target channel number, 0 is the default channel number
	INOUT	PNPC_USHORT							inout_pLocalPort,			//[INOUT]Local port number. If the value is 0, the system assigns the port number.
	IN		NPC_INT								in_iTimeout=20000			//[IN]Timeout (ms)
	);

//Connect the device and return port mapping data: NULL indicates failure, otherwise success
NPC_MPI_MON_DNP_API		PNPC_HANDLE NPC_F_MPI_MON_DNP_ConnDevAtCloud(
	IN		NPC_HANDLE							in_hDevNetPort,				//[IN]Device network port
	IN		PNPC_CHAR							in_pUmid,					//[IN]UMID
	IN		NPC_INT								in_iTimeout=20000			//[IN]Timeout (ms)
    );
    
//Disconnect the device
NPC_MPI_MON_DNP_API		NPC_VOID		NPC_F_MPI_MON_DNP_DisconnectDevAtCloud(
    IN		NPC_HANDLE							in_hDevNetPort,				//[IN]Device network port
    IN		NPC_HANDLE							in_hPortMapData				//[IN]Port mapping data
    );
    
//Delete mapped port
NPC_MPI_MON_DNP_API		NPC_VOID	NPC_F_MPI_MON_DNP_DelPort(
	IN		NPC_HANDLE							in_hDevNetPort,				//[IN]Device network port
	IN		NPC_USHORT							in_usLocalPort				//[IN]Local port number
	);


//----------------------------------------------------------------------------------------
//Check the server connection status, return: 0: not connected, 1: connecting, 2: connected, returning 9 indicates an error
NPC_MPI_MON_DNP_API		NPC_INT		NPC_F_MPI_MON_DNP_CheckSrvConnState(
	IN		NPC_HANDLE							in_hDevNetPort				//[IN]Device network port
	);


//Set private forwarding server, return: 0: success
NPC_MPI_MON_DNP_API		NPC_INT		NPC_F_MPI_MON_DNP_SetPrivateTrans(
	IN		NPC_HANDLE							in_hDevNetPort,				//[IN]Device network port
	IN		NPC_BOOL							in_bIfUsePriTrans=FALSE,	//[IN]Whether to use a private forwarding server
	IN		NPC_DWORD							in_dwEnterId=0				//[IN]Enterprise ID
	);

//----------------------------------------------------------------------------------------
//Clear device status query list
NPC_MPI_MON_DNP_API		NPC_VOID	NPC_F_MPI_MON_DNP_DevState_ClearList(
	IN		NPC_HANDLE							in_hDevNetPort				//[IN]Device network port
	);

//Add query device
NPC_MPI_MON_DNP_API		NPC_BOOL	NPC_F_MPI_MON_DNP_DevState_AddDev(
	IN		NPC_HANDLE							in_hDevNetPort,				//[IN]Device network port
	IN		PNPC_CHAR							in_pDevUmid					//[IN]Device UMID
	);

//Request device status query (please use batch query device status to reduce server query load)
NPC_MPI_MON_DNP_API		NPC_BOOL	NPC_F_MPI_MON_DNP_DevState_RequQueryDevState(
	IN		NPC_HANDLE							in_hDevNetPort,				//[IN]Device network port
	OUT		PNPC_INT							out_pQueryDevSpaceTime,		//[OUT]Interval for querying device status (s)
	IN		NPC_INT								in_iTimeout					//[IN]Timeout (ms)
	);

//Get device status (get status in query list)
NPC_MPI_MON_DNP_API		NPC_BOOL	NPC_F_MPI_MON_DNP_DevState_GetDevState(
	IN		NPC_HANDLE							in_hDevNetPort,				//[IN]Device network port
	IN		PNPC_CHAR							in_pDevUmid,				//[IN]Device UMID
	OUT		PNPC_INT							out_pDevState				//[OUT]Device status, 0: offline, 1: online
	);


//Send data packet
NPC_MPI_MON_DNP_API		NPC_BOOL	NPC_F_MPI_MON_DNP_SendData(
    IN		NPC_HANDLE							in_hPortMapData,			//[IN]Port mapping data
    IN		PNPC_CHAR							in_pSendData,				//[IN]Send data buffer
    IN		NPC_INT                             in_iDataLen,				//[IN]Send data length
    IN		NPC_INT								in_iTimeout					//[IN]Timeout (ms)
    );
//Receive data packet
NPC_MPI_MON_DNP_API		NPC_BOOL	NPC_F_MPI_MON_DNP_GetData(
    IN		NPC_HANDLE							in_hPortMapData,			//[IN]Port mapping data
    OUT		PNPC_CHAR							out_iDataBuf,               //Data buffer
    OUT		PNPC_INT							out_iDataLen,               //Data length
    IN		NPC_INT								in_iTimeout					//[IN]Timeout (ms)
    );
//----------------------------------------------------------------------------------------
//Get logs or debugging information, the bottom layer maintains a log queue, saves the generated logs into the queue, calls this function to get the number of logs from this queue, and calls the function to delete the queue data;
//If it is not called for a long time, the bottom layer will delete the oldest log, and a maximum of 100 logs are saved by default.
NPC_MPI_MON_DNP_API		NPC_BOOL	NPC_F_MPI_MON_DNP_GetLogData(
	IN		NPC_INT								in_iLogBufSize,				//[IN]Log information buffer size
	OUT		PNPC_INT							out_pLogInfoType,			//[OUT]Log type, see NPC_D_MON_LOG_INFO_TYPE_ * definition
	OUT		PNPC_CHAR							out_pLogBuf,				//[OUT]Log information buffer, caller allocates memory
	IN		NPC_INT								in_iTimeout					//[IN]Timeout (ms)
	);


//Receive data callback. The user data in the Callback is the device port data handle returned by the NPC_F_MPI_MON_DNP_ConnDevAtCloud interface.
NPC_MPI_MON_DNP_API		NPC_VOID	NPC_F_MPI_MON_DNP_SetP2pPortDataCallback(
    IN      PNPC_F_MPI_MON_CALLBACK_DevNetPortP2pPortData   in_pCallback,       //[IN]Callback
    IN		PNPC_VOID                                   in_hUserData,		//[IN]User data
    IN		NPC_HANDLE                                  in_hPortMapData     //[IN]Port mapping data
    );
    
    
NPC_MPI_MON_DNP_API		NPC_VOID	NPC_F_MPI_MON_DNP_Dev_PublicIpAddr(
    IN		NPC_HANDLE							in_hPortMapData,			//[IN]Port mapping data
    OUT		PNPC_BYTE*							out_iDataBuf,               //Data buffer
    OUT		PNPC_INT							out_iDataLen                //Data length
    );
// C++ support
#ifdef __cplusplus
}
#endif

#endif
