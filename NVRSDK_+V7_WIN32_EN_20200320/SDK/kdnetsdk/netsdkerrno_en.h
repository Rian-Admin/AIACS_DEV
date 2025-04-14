#ifndef _NETSDKERRNO_H_
#define _NETSDKERRNO_H_

/**
 * 说明：错误码定义文件，该文件将所有错误码合并，包括cgiapp错误码、业务错误码、netsdk内部错误码
 * 1.cgiapp错误码范围1~500
 * 2.业务错误码范围1000~3000
 * 3.netsdk内部错误码范围6000~6499
 */

#define NET_OK									0    ///< Operation Succeeded

/**
 * cgiapp错误码定义
 * 错误码范围1~500
 */

#define ERR_NET_UPGRADE_PREPARE_OK				1    ///< Preparation for Device Upgrade Completed
#define ERR_NET_UPGRADE_SUCCESS					2    ///< Device Upgraded
#define ERR_NET_SYSTEM_REBOOT					3    ///< Device Rebooted
#define ERR_NET_FACTORY_DEF						4    ///< Reset to Factory Defaults
#define ERR_NET_URL_CHANGE						5    ///< CGI Service Address Changed
#define ERR_NET_PTZ_UPGRADE						6    ///< PT Upgraded
#define ERR_NET_ISUPGRADE						7    ///< Device Being Upgraded
#define ERR_NET_NO_SEC_MAIL						8    ///< Safety Mail Not Configured
#define ERR_NET_ALREADY_ACT						9    ///< Device Activated
#define ERR_NET_ALREADY_LOGIN					10   ///< User Already Logged In
#define ERR_NET_SYSTEM_SHUTDOWN					11   ///< Device Shut Down

#define ERR_NET_UNKNOW							201  ///< Protocol Error
#define ERR_NET_USERNAME_ERR					202  ///< User Not Exists
#define ERR_NET_NOT_AUTH						203  ///< Authentication Failed
#define ERR_NET_PASS_ERR						204  ///< Incorrect Password
#define ERR_NET_AUTHID_ERR						205  ///< Incorrect Authentication ID
#define ERR_NET_NO_POWER						206  ///< No Authorization
#define ERR_NET_IP_DENY							207  ///< IP Locked
#define ERR_NET_OLDPASS_ERR						208  ///< Incorrect Old Password
#define ERR_NET_USER_EXISTED					209  ///< User Already Exists
#define ERR_NET_URL_NO_SUPPORT					210  ///< Function Not Supported or Illegal Signaling Message
#define ERR_NET_NEED_BODY						211  ///< Illegal Signaling Message
#define ERR_NET_XML_ERR							212  ///< Illegal Signaling Message
#define ERR_NET_XML_ROOT_ERR					213  ///< Illegal Signaling Message
#define ERR_NET_PARAM_LOST						214  ///< Illegal Signaling Message
#define ERR_NET_PARMM_TOOLONG					215  ///< Illegal Signaling Message
#define ERR_NET_CHECK_ERR						216  ///< Verification of Upgrade Information Failed
#define ERR_NET_FILE_ERR						217  ///< Verification of Upgrade Package Failed
#define ERR_NET_UPGRADE_FAILURE					218  ///< Upgrade Failed
#define ERR_NET_PARAM_ERR						219  ///< Illegal Signaling Message
#define ERR_NET_IP_ERR							220  ///< Illegal IP
#define ERR_NET_MASK_ERR						221  ///< Incorrect Subnet Mask
#define ERR_NET_GATEWAY_ERR						222  ///< Incorrect Default Gateway
#define ERR_NET_DNS_ERR							223  ///< Incorrect DNS Address
#define ERR_NET_MULTICAST_ERR					224  ///< Incorrect Multicast Address
#define ERR_NET_MTU_ERR							225  ///< MTU Error
#define ERR_NET_SEQ_ERR							226  ///< Signaling Message Sequence Error
#define ERR_NET_URL_ERR							227  ///< Incorrect URL Format
#define ERR_NET_INVALID_ARG						228  ///< Illegal Parameter
#define ERR_NET_DEVICE_BUSY						229  ///< Device Busy
#define ERR_NET_RECOVER							230  ///< Operation Failed and Parameter Values Recovered
#define ERR_NET_USER_BANNED						231  ///< User Locked
#define ERR_NET_USER_ACTIVE						232  ///< Device Deactivated
#define ERR_NET_NO_MEM							233  ///<Insufficient Memory
#define ERR_NET_BUF_TOO_SMALL					234  ///<Insufficient Data Buffer
#define ERR_NET_NOT_FOUND						235  ///< Illegal Signaling Message
#define ERR_NET_NO_IMPLEMENTED					236  ///< Function Not Supported
#define ERR_NET_ALREADY_EXIST					237  ///< Exists
#define ERR_NET_NEED_LOGIN						238  ///< Not Logged In
#define ERR_NET_USER_DISABLED					239  ///< User Disabled
#define ERR_NET_USER_PORT_OCCUPIED				240  ///< Port Occupied
#define ERR_NET_USER_USERS_TOPLIMIT				241  ///< Upper Limit for Number of User Logins Reached
#define ERR_NET_IP_OCCUPIED						242  ///< IP Occupied

/**
 * 业务错误码定义
 * 错误码范围1000~3000
 */

#define ERR_NET_ERROR							1001 ///< Operation failed. Please try again later.
#define ERR_NET_ASSERT							1002 ///< Illegal Parameter
#define ERR_NET_SEM_TAKE_FAILED					1003 ///< Operation failed. Please try again later.
#define ERR_NET_SEM_GIVEE_FAILED				1004 ///< Operation failed. Please try again later.
#define ERR_NET_PARAM_INVALID					1005 ///< Illegal Parameter
#define ERR_NET_WAIT_TIMEOUT					1006 ///< Operation Timed Out
#define ERR_NET_MALLOC_FAILED					1007 ///< Failed to Allocate Memory
#define ERR_NET_STRING_ILLEGAL					1008 ///< Illegal String
#define ERR_NET_STRING_TOO_SHORT				1009 ///< String Too Short
#define ERR_NET_STRING_TOO_LONG					1010 ///< String Too Long

#define ERR_NET_FILE_INEXIST					1020 ///< File Not Exists
#define ERR_NET_WRITE_FILE						1021 ///< Failed to Write Data into File
#define ERR_NET_EXPORTING_CFG					1022 ///< Importing or Exporting Configuration File
#define ERR_NET_REGISTER_FULL					1023 ///< Upper Registration Limit Reached
#define ERR_NET_ALREADY_REGISTERED				1024 ///< Registered
#define ERR_NET_NO_REGISTERED					1025 ///< Not Registered
#define ERR_NET_IO_ERROR						1026 ///< IO error. The HDD does not exist or becomes full.

#define ERR_NET_CAP_NO_SUPPORT					1100 ///< Capability Limited

#define ERR_NET_USER_EXIST        				1200 ///< User Already Exists
#define ERR_NET_USER_INEXIST      				1201 ///< User Not Exists
#define ERR_NET_USER_FILE_BROKEN  				1202 ///< File Damaged
#define ERR_NET_USER_NUM_MAX      				1203 ///< Upper Limit for Number of Users Reached
#define ERR_NET_USER_DEL_DISALLOWED      		1204 ///< Deleting User Prohibited
#define ERR_NET_USER_NAME_ILLEGAL 				1205 ///< Illegal Parameters Found in Username
#define ERR_NET_USER_PWD_ILLEGAL  				1206 ///< Illegal Parameters Found in Password
#define ERR_NET_USER_NAME_LEN_TOO_LONG 			1207 ///< Username Too Long
#define ERR_NET_USER_NAME_LEN_TOO_SHORT 		1208 ///< Username Too Short
#define ERR_NET_USER_PWD_LEN_TOO_LONG 			1209 ///< Password Too Long
#define ERR_NET_USER_PWD_LEN_TOO_SHORT 			1210 ///< Password Too Short
#define ERR_NET_USER_PWD_STRENGTH_WEAK 			1211 ///< Password Too Weak
#define ERR_NET_USER_MGR_EMAIL_ILLEGAL			1212 ///< Incorrect Mail Format
#define ERR_NET_USER_REMOTE_IP_INFO_ILLEGAL		1213 ///< Illegal Remote IP
#define ERR_NET_USER_NAME_MDY_DISALLOWED		1214 ///< Editing Username Prohibited
#define ERR_NET_USER_ADMIN_PERM_MDY_DISALLOWED	1215 ///< No Authorization
#define ERR_NET_USER_DEV_SYS_NOACTIVE			1216 ///< Device Deactivated
#define ERR_NET_USER_PASS_SAME_TO_BEFORE		1217 ///< Same as Old Password
#define ERR_NET_USER_NAME_NULL           		1218 ///< Username Not Specified
#define ERR_NET_USER_PASS_NULL          		1219 ///< Password Not Specified

#define ERR_NET_LOG								1300 ///< Log Related
#define ERR_NET_USER_LOG_TASK_BUSY				1301 ///< The log task is busy. Please try again later.
#define ERR_NET_USER_LOG_TASK_ID_UNAVAILABLE    1302 ///< Invalid Task ID

#define ERR_NET_DEV								1400 ///< Peripherals

#define ERR_NET_NET								1500 ///< Network
#define ERR_NET_NET_REGISTER_FULL				1501 ///< Upper Registration Limit Reached
#define ERR_NET_NET_ALREADY_REGISTERED			1502 ///< Registered
#define ERR_NET_NET_NO_REGISTERED				1503 ///< Not Registered
#define ERR_NET_NET_PING_NUM_MAX				1504 ///< Upper Ping Limit Reached
#define ERR_NET_NET_DOMAIN_ANALY_FAILD			1505 ///< Failed to Parse Domain
#define ERR_NET_NET_PORT_IS_USING    			1506 ///< Network Port Occupied
#define ERR_NET_NET_OTHER_IP_IN_SAME_NET    	1507 ///< NIC IP addresses must be located on different segments
#define ERR_NET_NET_IP_GW_NOTIN_SAME_NET    	1508 ///< The subnet mask and default gateway must be located on the same segment
#define ERR_NET_NET_OPERATE_TOO_FREQUENCY		1509 ///< Too Frequent Requests. Please try again later.
#define ERR_NET_NET_PING_CHN_NO_IP				1510 ///< The channel has no IP address.

#define ERR_NET_MPU								1600 ///< mpu-related Error
#define ERR_NET_MPU_DEC_CHN_NUM_OVER_MAX		1601 ///< Upper Limit for Number of Decoding Channels Reached
#define ERR_NET_MPU_DEC_ABILITY_OVER_MAX		1602 ///< Decoding Capability Exceeded
#define ERR_NET_MPU_CHN_ID_IS_DECODING			1603 ///< Channel Being Decoded
#define ERR_NET_MPU_MC_SET_LAYOUT_FAILED		1604 ///< Failed to Configure Screen Layout
#define ERR_NET_MPU_MC_SET_DEC_PARAM_FAILED		1605 ///< Failed to Create Decoder
#define ERR_NET_MPU_MC_SET_OPT_FAILED			1606 ///< Failed to Configure Parameters
#define ERR_NET_MPU_MC_GET_OPT_FAILED			1607 ///< Failed to Obtain Parameter Values
#define ERR_NET_MPU_OVER_MC_DEV_ZOOM_CAP		1608 ///< Scaling Capability Exceeded
#define ERR_NET_MPU_BIND_FAILED					1609 ///< Binding Failed
#define ERR_NET_MPU_UNBIND_FAILED				1610 ///< Unbinding Failed

#define ERR_NET_MEDIA							1700 ///< media-related Error

#define ERR_NET_DM								1800 ///< HDD-related Error
#define ERR_NET_DM_DISK_ID_INVALID				1801 ///< Invalid HDD ID
#define ERR_NET_DM_DISK_IN_USE					1802 ///< HDD in Use
#define ERR_NET_DM_NET_DISK_NAME_TOO_LONG		1803 ///< HDD Name Too Long
#define ERR_NET_DM_FUNCTION_NOT_SUPPORT			1804 ///< Function Not Supported
#define ERR_NET_DM_DISK_USED_BY_RP				1805 ///< HDD in Use
#define ERR_NET_DM_DISK_USED_BY_RP_PLY			1806 ///< Playing Back
#define ERR_NET_DM_DISK_USED_BY_RP_DLD			1807 ///< Downloading
#define ERR_NET_DM_DISK_UMOUNT_PART_ERR			1808 ///< Uninstalling Failed
#define ERR_NET_DM_DISK_FORMAT_PART_ERR			1809 ///< Formatting Failed
#define ERR_NET_DM_DISK_CHG_CALLBACK_FULL		1810 ///< Full HDD
#define ERR_NET_DM_DISK_EXPCPTION_FORBID_OPERATION 1811 ///< Operation Forbidden Due to HDD Exception
#define ERR_NET_DM_DISK_EXTERNAL_DISK_LIMIT     1812 ///< Upper Limit for Number of External Storage Units Reached
#define ERR_NET_DM_DISK_BAD_SECTOR_CHECK_NO_TASKID 1813 ///< Upper Limit for Number of Bad Sector Detection Tasks Reached
#define ERR_NET_DM_DISK_DISK_FS_TYPE_FAILD      1814 ///< Incorrect HDD Format
#define ERR_NET_DM_DISK_RAID_HOTBACKUP_DISK_SIZE_ERROR      1815 ///< Small Capacity of Hot Backup HDD
#define ERR_NET_DM_DISK_SET_QUTOA_SIZE_OVER_ALL_DISK_SIZE      1816 ///< Quota Size Greater than Total HDD Capacity
#define ERR_NET_DM_DISK_RAID_DELING             1817 ///< Deleting RAID
#define ERR_NET_DM_DISK_RAID_CREATING           1818 ///< Creating RAID
#define ERR_NET_DM_DISK_JUST_SUP_ONE_SMART_DISK 1820 ///< Only one smart HDD is supported.

#define ERR_NET_REC								1900 ///< Recording-related Error
#define ERR_NET_REC_CFG_DATA_NOT_EXIST			1901 ///< Recording Configurations Not Exist
#define ERR_NET_REC_NO_IDLE_PLY_TASK			1902 ///< Upper Limit for Number of Recordings Reached
#define ERR_NET_REC_MSIN_STOP_FAILED			1903 ///< Failed to Stop Receiving Recording Data
#define ERR_NET_REC_MSIN_RELEASE_FAILED			1904 ///< Failed to Stop Receiving Recording Data
#define ERR_NET_REC_REPEAT_TO_ADD_CHN			1905 ///< Channel Already Added
#define ERR_NET_REC_START_PLY_FAILED			1906 ///< Failed to Start the Playback
#define ERR_NET_REC_BAKUP_TASK_FULL				1907 ///< Upper Limit for Number Recording Backup Tasks Reached
#define ERR_NET_REC_IMG_BAK_TASK_FULL			1908 ///< Upper Limit for Number Snapshot Backup Tasks Reached
#define ERR_NET_REC_CHN_NOT_START				1909 ///< Recording for Channel Not Enabled
#define ERR_NET_REC_CMD_DEAL_THREAD_BUSY		1910 ///< Processing Thread Busy
#define ERR_NET_REC_PART_BUSY	            	1911 ///<Partition Busy
#define ERR_NET_REC_COMPONENT_LIB_ERR   		1912 ///< Recording Error
#define ERR_NET_REC_DISK_STATUS_SLEEP   		1913 ///< Waking HDD Up
#define ERR_NET_REC_PLAYER_FULL   				1914 ///< No more playback tasks can be created. Some playbacks already failed.
#define ERR_NET_REC_AUD_INVALID   				1915 ///< No Valid Audio

#define ERR_NET_CFG								2000 ///< Configuration-related Error
#define ERR_NET_CFG_OPEN_DATABASE_FALID			2001 ///< Failed to Open Database
#define ERR_NET_CFG_CLOSE_DATABASE_FALID		2002 ///< Failed to Close Database
#define ERR_NET_CFG_CREATE_TABLE_FALID			2003 ///< Failed to Create Table
#define ERR_NET_CFG_GET_PARAM_FALID				2004 ///< Failed to Obtain Parameter Values
#define ERR_NET_CFG_SET_PARAM_FALID				2005 ///< Failed to Configure Parameters
#define ERR_NET_CFG_NO_THIS_DATA				2006 ///< Illegal Parameter
#define ERR_NET_CFG_NO_TABLE					2007 ///< Illegal Parameter
#define ERR_NET_CFG_INPORT_CFG_DEV_ERR			2008 ///< Configurations and Model Do Not Match
#define ERR_NET_CFG_INPORT_CFG_CRC_FAILED		2009 ///< Verification of Configurations to Be Imported Failed

#define ERR_NET_PUI								2100 ///< pui-related Error
#define ERR_NET_PUI_CHNID_ADDED					2101 ///< Channel Occupied
#define ERR_NET_PUI_DEV_REPEAT_ADD				2102 ///< Device Added Already
#define ERR_NET_PUI_DEV_ADD_FAILED				2103 ///< Failed to Add Device
#define ERR_NET_PUI_CHNID_ADDED_FULL			2104 ///< No Channel Available
#define ERR_NET_PUI_APPCLT_ERR					2105 ///< Operation Failed
#define ERR_NET_PUI_DEV_DEL_FAILED				2106 ///< Failed to Delete Device
#define ERR_NET_PUI_OVER_MAX_USRNUM				2107 ///< Upper Limit for Number of Searching Tasks Reached
#define ERR_NET_PUI_OVER_MAX_GROUP_NUM			2108 ///< Upper Limit for Number of Groups Reached
#define ERR_NET_PUI_OVER_MAX_CHN_NUM			2109 ///< Upper Limit for Number of Channels Reached
#define ERR_NET_PUI_LEN_NOT_ENOUGH				2110 ///< Illegal Parameter
#define ERR_NET_PUI_OVER_MAX_ACPT_BANDWIDTH		2111 ///< Upper Limit for Access Bandwidth Reached
#define ERR_NET_PUI_PTZ_TASK_RUNING				2112 ///< PTZ Task Occupied
#define ERR_NET_PUI_VALID_DEV_UPGRADE_TASK      2113 ///< No Upgrade Task Available
#define ERR_NET_PUI_NO_DETECT_AREA				2114 ///< Number of Motion Detection Areas Cannot Be Zero
#define ERR_NET_PUI_DEV_FORBIDDEN   			2115 ///< Device Disabled
#define ERR_NET_PUI_AUTH_ID_ERR_FORBIDDEN		2116 ///< Authentication Error

#define ERR_NET_VTDUCTRL						2200 ///< Nvrvtductrl-related Error
#define ERR_NET_VTDU_APPCLT_STREAM_PREPARE_FAILED   2201 ///< Failed to Obtain PU Stream
#define ERR_NET_VTDU_APPCLT_STREAM_START_FAILED 2202 ///< Failed to Obtain PU Stream
#define ERR_NET_VTDU_SND_IS_FULL                2203 ///< Sending Capability Exceeded
#define ERR_NET_VTDU_SEN_RATE_OVER              2204 ///< Sending Capability Exceeded
#define ERR_NET_VTDU_DEV_OFFLINE                2205 ///< Device Offline
#define ERR_NET_VTDU_MSIN_NO_STREAM             2206 ///< Failed to Receive Stream
#define ERR_NET_VTDU_MSIN_CREATE_FAILED         2207 ///< Failed to Create Stream Receiving Object
#define ERR_NET_VTDU_MSIN_SET_OPT_FAILED        2208 ///< Failed to Configure Stream Receiving Settings
#define ERR_NET_VTDU_MSIN_SET_TRANSPARAM_FAILED 2209 ///< Failed to Configure Stream Receiving Settings
#define ERR_NET_VTDU_MSIN_INPUT_DATA_FAILED     2210 ///< Failed to Receive Stream
#define ERR_NET_VTDU_MSIN_START_FAILED          2211 ///< Failed to Start Receiving Stream
#define ERR_NET_VTDU_MSIN_STOP_FAILED           2212 ///< Failed to Stop Receiving Stream
#define ERR_NET_VTDU_MSIN_RELEASE_FAILED        2213 ///< Failed to Stop Receiving Stream
#define ERR_NET_VTDU_ADD_PIPELINE_FAILED        2215 ///< Failed to Create Stream Output Object
#define ERR_NET_VTDU_REMOVE_PIPELINE_FAILED     2216 ///< Failed to Stop Sending
#define ERR_NET_VTDU_MSOUT_CREATE_FAILED        2217 ///< Failed to Create Stream Output Object
#define ERR_NET_VTDU_MSOUT_SET_OPT_FAILED       2218 ///< Failed to Configure the Output Attribute of Stream
#define ERR_NET_VTDU_MSOUT_SET_TRANSPARAM_FAILED 2219 ///< Failed to Configure Stream Transmission Settings
#define ERR_NET_VTDU_MSOUT_SET_DATA_CB_FAILED   2220 ///< Failed to Configure the Output Attribute of Stream
#define ERR_NET_VTDU_MSOUT_GET_DATA_POS_FAILED  2221 ///< Failed to Obtain Position of Stream Data
#define ERR_NET_VTDU_MSOUT_GET_DATA_FAILED      2222 ///< Failed to Obtain Stream Data
#define ERR_NET_VTDU_MSOUT_RELEASE_DATA_FAILED  2223 ///< Failed to Release Stream Data
#define ERR_NET_VTDU_MSOUT_STRAT_FAILED         2224 ///< Failed to Start Sending
#define ERR_NET_VTDU_MSOUT_STOP_FAILED          2225 ///< Failed to Stop Sending
#define ERR_NET_VTDU_MSOUT_RELEASE_FAILED       2226 ///< Failed to Stop Sending
#define ERR_NET_VTDU_INPUT_VID_PARAM_INVALID	2227 ///< Illegal Value for Video Input
#define ERR_NET_VTDU_INPUT_AUD_PARAM_INVALID	2228 ///< Illegal Value for Audio Input
#define ERR_NET_VTDU_IS_AUDCALLING				2230 ///< Calling
#define ERR_NET_VTDU_OVER_MAX_SND_BANDWIDTH		2231 ///< Sending Bandwidth Capability Exceeded
#define ERR_NET_VTDU_BROADCASTING_NO_SUPPORT_CHN 2235 ///< No channels supporting voice calls are found.

#define ERR_NET_SMTP_ERR                        2301 ///< Failed to Send Mail
#define ERR_NET_SMTP_FILE_LEN_ERR               2302 ///< Mail Attachment Too Large
#define ERR_NET_SMTP_PARAM_INVALID              2303 ///< Mail Parameter Error
#define ERR_NET_SMTP_CONNECT_SERVER_ERR         2304 ///< Failed to Connect to Server
#define ERR_NET_SMTP_LOGIN_ERR                  2305 ///< User Authentication Failed
#define ERR_NET_SMTP_SEND_ERR                   2306 ///< Failed to Send Data
#define ERR_NET_SMTP_RECV_ERR                   2307 ///< Failed to Receive Data
#define ERR_NET_SMTP_CONNECT_TIME_OUT           2308 ///< Connecting to SMTP Server Timed Out
#define ERR_NET_SMTP_RESPONSE_ERR               2309 ///< Response Error
#define ERR_NET_SMTP_CONNECT_SSL_ERR            2300 ///< SSL Disconnected
#define ERR_NET_SMTP_STARTTLS_ERR               2311 ///< Failed to Encrypt Mail
#define ERR_NET_SMTP_ASSERT_ERR                 2312 ///< Illegal Parameter
#define ERR_NET_SMTP_DOMAIN_ANALY_ERR           2313 ///< Failed to Parse Mail Domain

#define ERR_NET_CTRLLIB_OVER_MAX_NUM			2701 ///< Archive: no more archives can be added.
#define ERR_NET_CTRLLIB_OTHER_OPT_IS_DOING		2702 ///< Archive: the archive is being occupied by another application.
#define ERR_NET_CTRLLIB_SAME_NAME				2703 ///< Archive: the name already exists.
#define ERR_NET_CTRLLIB_WRITE_DB_FAIL			2705 ///< Archive: writing the archive data into the database failed.
#define ERR_NET_CTRLLIB_OTHER_USER_OPT_ALG		2706 ///< Archive: another user is configuring the algorithm engine.
#define ERR_NET_CTRLLIB_CREATE_FILE_FAIL		2707 ///< Archive: failed to create a table
#define ERR_NET_CTRLLIB_OPEN_DB_FAIL			2708 ///< Archive: failed to open the database
#define ERR_NET_CTRLLIB_CREATE_TABLE_FAIL		2709 ///< Archive: failed to create a table
#define ERR_NET_CTRLLIB_EXE_SQL_FAIL			2710 ///< Archive: failed to execute SQL statements
#define ERR_NET_CTRLLIB_PIC_OVER_RAM_SIZE		2711 ///< Archive: the picture is too large
#define ERR_NET_CTRLLIB_SYS_CMD_FAIL			2712 ///< Archive: failed to execute system commands.
#define ERR_NET_CTRLLIB_OTHER_USER_IMPORT		2713 ///< Archive: the picture import function is occupied by another user.
#define ERR_NET_CTRLLIB_CHECK_IMPORT_FAIL		2714 ///< Archive: verification of imported files failed
#define ERR_NET_CTRLLIB_CREATE_THREAD_FAIL		2715 ///< Archive: failed to create threads
#define ERR_NET_CTRLLIB_EGI_FAIL				2716 ///< Failed to extract characteristics
#define ERR_NET_CTRLLIB_COMPARE_USED			2717 ///< The archive is being occupied by a compare rule
#define ERR_NET_CTRLLIB_OVER_SUP_MAX			2718 ///< The upper limit of an archive is reached.
#define ERR_NET_AIS_PIC_QUERY_RESULT_OVER_NUM	2719 ///< Excessive search results may be produced. You are advised to narrow down the time range.
#define ERR_NET_AIS_PIC_NO_FACE_PIC_FEATURE		2720 ///< No faces are detected.
#define ERR_NET_CTRLLIB_ADD_NO_EGI				2721 ///< Personnel information is added to the archive, but the data model is not created.
#define ERR_NET_AIS_OVER_SUP_DETECT_NUM			2722 ///< The detection capability is exceeded.
#define ERR_NET_AIS_ALG_INVALID					2723 ///< This algorithm cannot be found.
#define ERR_NET_AIS_CREATE_DETECT_FAILED		2724 ///< Failed to create detection handles.
#define ERR_NET_AIS_OVER_CMP_CHN_NUM			2725 ///< No more channels can be added.
#define ERR_NET_AIS_RULE_NAME_EXIST				2726 ///< The name already exists.
#define ERR_NET_AIS_CMP_CTRLLIB_EXIST			2727 ///< The archive already exists.
#define ERR_NET_AIS_OVER_SUP_RULE_NUM			2728 ///< No more rules can be created.
#define ERR_NET_AIS_CREATE_CMP_HANDLE_FAILED	2729 ///< Failed to create compare handles.
#define ERR_NET_CTRLLIB_MEM_ALREADY_DEL			2730 ///< Operation failed because the target had been deleted
#define ERR_NET_AIS_ALG_ERR_ALG_NOT_MATCH		2732 ///< Some model data does not match with the current system version. You can try another search way.

/**
 * netsdk内部错误码定义
 * 错误码范围6000~6499
 */

#define ERR_NET_INIT_FAILED						6000 ///< Initialization Failed
#define ERR_NET_INVALID_HANDLE					6001 ///< Invalid Handle
#define ERR_NET_INVALID_PARAM					6002 ///< Illegal Parameter
#define ERR_NET_PARSE_FAILED					6003 ///< Parsing Failed
#define ERR_NET_CREATE_HANDLE_FAILED			6004 ///< Failed to Create a Handle
#define ERR_NET_ALLOC_MEM_FAILED				6005 ///< Failed to Apply for Memory
#define ERR_NET_SOCKET_OPT_FAILED				6006 ///< Failed to Configure Network Attributes
#define ERR_NET_CONNECT_FAILED					6007 ///< Connection Failed
#define ERR_NET_CONNECT_CLOSED					6008 ///< Disconnected
#define ERR_NET_SEND_FAILED						6009 ///< Sending Failed
#define ERR_NET_RECV_FAILED						6010 ///< Receiving Failed
#define ERR_NET_SEND_TIMEOUT					6011 ///< Sending Timed Out
#define ERR_NET_RECV_TIMEOUT					6012 ///< Receiving Timed Out
#define ERR_NET_SEND_OUT_MEM					6013 ///< Insufficient Sending Buffer 
#define ERR_NET_RECV_OUT_MEM					6014 ///< Insufficient Receiving Buffer

#endif
