
Date: 2020-02-19
This SDK version number:NVRSDK_V7_WIN32_EN_20200220
Precautions:
1. When using umid account, browsing and playing audio and video does not support udp transmission
2. Added the p2pmapsdk module. Refer to the netsdkdemo_p2p_prj directory for how to use the module.

The following functions are followed in the demo:
  NPC_F_MPI_MON_DNP_CreateDevNetPortServer
  NPC_F_MPI_MON_DNP_AddPortByDestAddr
 
Follow the logic of the variable as follows:
m_bIsUmidUser
m_tConfig.tDevCfg.awLocalPort [NPC_NETPORT_TYPE_LOGIN]
m_tConfig.tDevCfg.awLocalPort [NPC_NETPORT_TYPE_RTSP]