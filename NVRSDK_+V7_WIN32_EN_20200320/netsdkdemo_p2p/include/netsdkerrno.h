#ifndef _NETSDKERRNO_H_
#define _NETSDKERRNO_H_

/**
 * ˵���������붨���ļ������ļ������д�����ϲ�������cgiapp�����롢ҵ������롢netsdk�ڲ�������
 * 1.cgiapp�����뷶Χ1~500
 * 2.ҵ������뷶Χ1000~3000
 * 3.netsdk�ڲ������뷶Χ6000~6499
 */

#define NET_OK									0    ///< �����ɹ�

/**
 * cgiapp�����붨��
 * �����뷶Χ1~500
 */

#define ERR_NET_UPGRADE_PREPARE_OK				1    ///< �豸����׼������
#define ERR_NET_UPGRADE_SUCCESS					2    ///< �豸�����ɹ�
#define ERR_NET_SYSTEM_REBOOT					3    ///< �豸����
#define ERR_NET_FACTORY_DEF						4    ///< �ָ���������
#define ERR_NET_URL_CHANGE						5    ///< CGI�����ַ�仯
#define ERR_NET_PTZ_UPGRADE						6    ///< ��̨����
#define ERR_NET_ISUPGRADE						7    ///< �豸��������
#define ERR_NET_NO_SEC_MAIL						8    ///< ��ȫ����δ����
#define ERR_NET_ALREADY_ACT						9    ///< �豸�Ѽ���
#define ERR_NET_ALREADY_LOGIN					10   ///< �û��ѵ�¼
#define ERR_NET_SYSTEM_SHUTDOWN					11   ///< �豸�ر�

#define ERR_NET_UNKNOW							201  ///< Э�����
#define ERR_NET_USERNAME_ERR					202  ///< �û�������
#define ERR_NET_NOT_AUTH						203  ///< ��Ȩʧ��
#define ERR_NET_PASS_ERR						204  ///< �������
#define ERR_NET_AUTHID_ERR						205  ///< ��ȨID����
#define ERR_NET_NO_POWER						206  ///< �û���Ȩ��
#define ERR_NET_IP_DENY							207  ///< IP������
#define ERR_NET_OLDPASS_ERR						208  ///< ���������
#define ERR_NET_USER_EXISTED					209  ///< �û��Ѵ���
#define ERR_NET_URL_NO_SUPPORT					210  ///< �˹��ܲ�֧�ֻ�����Ƿ�
#define ERR_NET_NEED_BODY						211  ///< ����Ƿ�
#define ERR_NET_XML_ERR							212  ///< ����Ƿ�
#define ERR_NET_XML_ROOT_ERR					213  ///< ����Ƿ�
#define ERR_NET_PARAM_LOST						214  ///< ����Ƿ�
#define ERR_NET_PARMM_TOOLONG					215  ///< ����Ƿ�
#define ERR_NET_CHECK_ERR						216  ///< ������ϢУ��ʧ��
#define ERR_NET_FILE_ERR						217  ///< �����ļ�У��ʧ��
#define ERR_NET_UPGRADE_FAILURE					218  ///< �豸����ʧ��
#define ERR_NET_PARAM_ERR						219  ///< ����Ƿ�
#define ERR_NET_IP_ERR							220  ///< IP�Ƿ�
#define ERR_NET_MASK_ERR						221  ///< IP�������
#define ERR_NET_GATEWAY_ERR						222  ///< IP���ش���
#define ERR_NET_DNS_ERR							223  ///< DNS��ַ����
#define ERR_NET_MULTICAST_ERR					224  ///< �鲥��ַ����
#define ERR_NET_MTU_ERR							225  ///< MTU��������
#define ERR_NET_SEQ_ERR							226  ///< ����ʱ�����
#define ERR_NET_URL_ERR							227  ///< URL��ʽ����
#define ERR_NET_INVALID_ARG						228  ///< �����Ƿ�
#define ERR_NET_DEVICE_BUSY						229  ///< �豸��æ
#define ERR_NET_RECOVER							230  ///< ����ʧ�ܣ������ָ�
#define ERR_NET_USER_BANNED						231  ///< �û�������
#define ERR_NET_USER_ACTIVE						232  ///< �豸δ����
#define ERR_NET_NO_MEM							233  ///< �ڴ治��
#define ERR_NET_BUF_TOO_SMALL					234  ///< ���ݻ���������
#define ERR_NET_NOT_FOUND						235  ///< ����Ƿ�
#define ERR_NET_NO_IMPLEMENTED					236  ///< ���ܲ�֧��
#define ERR_NET_ALREADY_EXIST					237  ///< �Ѵ���
#define ERR_NET_NEED_LOGIN						238  ///< δ��¼
#define ERR_NET_USER_DISABLED					239  ///< �û�������
#define ERR_NET_USER_PORT_OCCUPIED				240  ///< �˿ڱ�ռ��
#define ERR_NET_USER_USERS_TOPLIMIT				241  ///< ��¼�û��Ѵ�����
#define ERR_NET_IP_OCCUPIED						242  ///< IP��ռ��

/**
 * ҵ������붨��
 * �����뷶Χ1000~3000
 */

#define ERR_NET_ERROR							1001 ///< ����ʧ�ܣ����Ժ�����
#define ERR_NET_ASSERT							1002 ///< �����Ƿ�
#define ERR_NET_SEM_TAKE_FAILED					1003 ///< ����ʧ�ܣ����Ժ�����
#define ERR_NET_SEM_GIVEE_FAILED				1004 ///< ����ʧ�ܣ����Ժ�����
#define ERR_NET_PARAM_INVALID					1005 ///< �����Ƿ�
#define ERR_NET_WAIT_TIMEOUT					1006 ///< ������ʱ
#define ERR_NET_MALLOC_FAILED					1007 ///< �����ڴ�ʧ��
#define ERR_NET_STRING_ILLEGAL					1008 ///< �ַ����Ƿ�
#define ERR_NET_STRING_TOO_SHORT				1009 ///< �ַ�������
#define ERR_NET_STRING_TOO_LONG					1010 ///< �ַ�������

#define ERR_NET_FILE_INEXIST					1020 ///< �ļ�������
#define ERR_NET_WRITE_FILE						1021 ///< д�ļ�ʧ��
#define ERR_NET_EXPORTING_CFG					1022 ///< ���ڵ���򵼳������ļ�
#define ERR_NET_REGISTER_FULL					1023 ///< ע������ﵽ����
#define ERR_NET_ALREADY_REGISTERED				1024 ///< ��ע��
#define ERR_NET_NO_REGISTERED					1025 ///< δע��
#define ERR_NET_IO_ERROR						1026 ///< IO����,Ӳ�̲����ڻ���Ӳ����

#define ERR_NET_CAP_NO_SUPPORT					1100 ///< ������֧��

#define ERR_NET_USER_EXIST        				1200 ///< �û��Ѵ���
#define ERR_NET_USER_INEXIST      				1201 ///< �û�������
#define ERR_NET_USER_FILE_BROKEN  				1202 ///< �ļ���
#define ERR_NET_USER_NUM_MAX      				1203 ///< �û����ﵽ����
#define ERR_NET_USER_DEL_DISALLOWED      		1204 ///< ���û���ֹɾ��
#define ERR_NET_USER_NAME_ILLEGAL 				1205 ///< �û��������Ƿ��ַ�
#define ERR_NET_USER_PWD_ILLEGAL  				1206 ///< �û���������Ƿ��ַ�
#define ERR_NET_USER_NAME_LEN_TOO_LONG 			1207 ///< �û�������
#define ERR_NET_USER_NAME_LEN_TOO_SHORT 		1208 ///< �û�������
#define ERR_NET_USER_PWD_LEN_TOO_LONG 			1209 ///< �û��������
#define ERR_NET_USER_PWD_LEN_TOO_SHORT 			1210 ///< �û��������
#define ERR_NET_USER_PWD_STRENGTH_WEAK 			1211 ///< �û�����ǿ��̫��
#define ERR_NET_USER_MGR_EMAIL_ILLEGAL			1212 ///< �����ʽ����
#define ERR_NET_USER_REMOTE_IP_INFO_ILLEGAL		1213 ///< Զ�̵�¼IP�Ƿ�
#define ERR_NET_USER_NAME_MDY_DISALLOWED		1214 ///< �û����������޸�
#define ERR_NET_USER_ADMIN_PERM_MDY_DISALLOWED	1215 ///< ��Ȩ���޸�
#define ERR_NET_USER_DEV_SYS_NOACTIVE			1216 ///< �豸δ����
#define ERR_NET_USER_PASS_SAME_TO_BEFORE		1217 ///< ���������ͬ
#define ERR_NET_USER_NAME_NULL           		1218 ///< �û�������Ϊ��
#define ERR_NET_USER_PASS_NULL          		1219 ///< ���벻��Ϊ��

#define ERR_NET_LOG								1300 ///< ��־���
#define ERR_NET_USER_LOG_TASK_BUSY				1301 ///< ��־����æ�����Ժ�����
#define ERR_NET_USER_LOG_TASK_ID_UNAVAILABLE    1302 ///< ��Ч������ID

#define ERR_NET_DEV								1400 ///< ����

#define ERR_NET_NET								1500 ///< ����
#define ERR_NET_NET_REGISTER_FULL				1501 ///< ע������ﵽ����
#define ERR_NET_NET_ALREADY_REGISTERED			1502 ///< ��ע��
#define ERR_NET_NET_NO_REGISTERED				1503 ///< δע��
#define ERR_NET_NET_PING_NUM_MAX				1504 ///< ��ǰping�ﵽ����
#define ERR_NET_NET_DOMAIN_ANALY_FAILD			1505 ///< ��������ʧ��
#define ERR_NET_NET_PORT_IS_USING    			1506 ///< ����˿ڱ�ռ��
#define ERR_NET_NET_OTHER_IP_IN_SAME_NET    	1507 ///< ������IP����ͬ����
#define ERR_NET_NET_IP_GW_NOTIN_SAME_NET    	1508 ///< IP�����ز���ͬһ����
#define ERR_NET_NET_OPERATE_TOO_FREQUENCY		1509 ///< ��������Ƶ�������Ժ�����
#define ERR_NET_NET_PING_CHN_NO_IP				1510 ///< ���豸IP��ַ

#define ERR_NET_MPU								1600 ///< mpu��ش���
#define ERR_NET_MPU_DEC_CHN_NUM_OVER_MAX		1601 ///< ����ͨ�����Ѵ�����
#define ERR_NET_MPU_DEC_ABILITY_OVER_MAX		1602 ///< ������������
#define ERR_NET_MPU_CHN_ID_IS_DECODING			1603 ///< ��ͨ�����ڽ���
#define ERR_NET_MPU_MC_SET_LAYOUT_FAILED		1604 ///< ���û�����ʧ��
#define ERR_NET_MPU_MC_SET_DEC_PARAM_FAILED		1605 ///< ����������ʧ��
#define ERR_NET_MPU_MC_SET_OPT_FAILED			1606 ///< ���ò���ʧ��
#define ERR_NET_MPU_MC_GET_OPT_FAILED			1607 ///< ��ȡ����ʧ��
#define ERR_NET_MPU_OVER_MC_DEV_ZOOM_CAP		1608 ///< ������������
#define ERR_NET_MPU_BIND_FAILED					1609 ///< ��ʧ��
#define ERR_NET_MPU_UNBIND_FAILED				1610 ///< ���ʧ��

#define ERR_NET_MEDIA							1700 ///< media��ش���

#define ERR_NET_DM								1800 ///< Ӳ�̹�����ش���
#define ERR_NET_DM_DISK_ID_INVALID				1801 ///< Ӳ��ID��Ч
#define ERR_NET_DM_DISK_IN_USE					1802 ///< Ӳ������ʹ��
#define ERR_NET_DM_NET_DISK_NAME_TOO_LONG		1803 ///< Ӳ�����ƹ���
#define ERR_NET_DM_FUNCTION_NOT_SUPPORT			1804 ///< ���ܲ�֧��
#define ERR_NET_DM_DISK_USED_BY_RP				1805 ///< Ӳ������ʹ��
#define ERR_NET_DM_DISK_USED_BY_RP_PLY			1806 ///< ���ڷ���
#define ERR_NET_DM_DISK_USED_BY_RP_DLD			1807 ///< ��������
#define ERR_NET_DM_DISK_UMOUNT_PART_ERR			1808 ///< ж��ʧ��
#define ERR_NET_DM_DISK_FORMAT_PART_ERR			1809 ///< ��ʽ��ʧ��
#define ERR_NET_DM_DISK_CHG_CALLBACK_FULL		1810 ///< Ӳ����
#define ERR_NET_DM_DISK_EXPCPTION_FORBID_OPERATION 1811 ///< Ӳ���쳣��ֹ����
#define ERR_NET_DM_DISK_EXTERNAL_DISK_LIMIT     1812 ///< ���ô洢�豸�ﵽ����
#define ERR_NET_DM_DISK_BAD_SECTOR_CHECK_NO_TASKID 1813 ///< �����������ﵽ����
#define ERR_NET_DM_DISK_DISK_FS_TYPE_FAILD      1814 ///< Ӳ�̸�ʽ����
#define ERR_NET_DM_DISK_RAID_HOTBACKUP_DISK_SIZE_ERROR      1815 ///< �ȱ���������С
#define ERR_NET_DM_DISK_SET_QUTOA_SIZE_OVER_ALL_DISK_SIZE      1816 ///< ���������������ڴ���������
#define ERR_NET_DM_DISK_RAID_DELING             1817 ///< raid����ɾ����
#define ERR_NET_DM_DISK_RAID_CREATING           1818 ///< raid���ڹ�����
#define ERR_NET_DM_DISK_JUST_SUP_ONE_SMART_DISK 1820 ///< ֻ֧��һ��������

#define ERR_NET_REC								1900 ///< ¼������ش���
#define ERR_NET_REC_CFG_DATA_NOT_EXIST			1901 ///< ¼�����ò�����
#define ERR_NET_REC_NO_IDLE_PLY_TASK			1902 ///< ¼������ﵽ����
#define ERR_NET_REC_MSIN_STOP_FAILED			1903 ///< ֹͣ����¼������ʧ��
#define ERR_NET_REC_MSIN_RELEASE_FAILED			1904 ///< ֹͣ����¼������ʧ��
#define ERR_NET_REC_REPEAT_TO_ADD_CHN			1905 ///< ͨ���ظ����
#define ERR_NET_REC_START_PLY_FAILED			1906 ///< ��������ʧ��
#define ERR_NET_REC_BAKUP_TASK_FULL				1907 ///< ¼�񱸷�����ﵽ����
#define ERR_NET_REC_IMG_BAK_TASK_FULL			1908 ///< ͼƬ��������ﵽ����
#define ERR_NET_REC_CHN_NOT_START				1909 ///< ͨ��¼��δ����
#define ERR_NET_REC_CMD_DEAL_THREAD_BUSY		1910 ///< �����߳�æ
#define ERR_NET_REC_PART_BUSY	            	1911 ///< ¼�����æ
#define ERR_NET_REC_COMPONENT_LIB_ERR   		1912 ///< ¼�����
#define ERR_NET_REC_DISK_STATUS_SLEEP   		1913 ///< �������ڻ���
#define ERR_NET_REC_PLAYER_FULL   				1914 ///< ��ǰ�����������������ַ�����ʧ��
#define ERR_NET_REC_AUD_INVALID   				1915 ///< ����Ч��Ƶ

#define ERR_NET_CFG								2000 ///< ������ش���
#define ERR_NET_CFG_OPEN_DATABASE_FALID			2001 ///< �����ݿ�ʧ��
#define ERR_NET_CFG_CLOSE_DATABASE_FALID		2002 ///< �ر����ݿ�ʧ��
#define ERR_NET_CFG_CREATE_TABLE_FALID			2003 ///< ������ʧ��
#define ERR_NET_CFG_GET_PARAM_FALID				2004 ///< ��ȡ����ʧ��
#define ERR_NET_CFG_SET_PARAM_FALID				2005 ///< ���ò���ʧ��
#define ERR_NET_CFG_NO_THIS_DATA				2006 ///< �����Ƿ�
#define ERR_NET_CFG_NO_TABLE					2007 ///< �����Ƿ�
#define ERR_NET_CFG_INPORT_CFG_DEV_ERR			2008 ///< �����������豸�ͺŲ�ƥ��
#define ERR_NET_CFG_INPORT_CFG_CRC_FAILED		2009 ///< ���������ļ�У��ʧ��

#define ERR_NET_PUI								2100 ///< puiģ����ش���
#define ERR_NET_PUI_CHNID_ADDED					2101 ///< ��ͨ����ռ��
#define ERR_NET_PUI_DEV_REPEAT_ADD				2102 ///< �豸�ظ����
#define ERR_NET_PUI_DEV_ADD_FAILED				2103 ///< �豸���ʧ��
#define ERR_NET_PUI_CHNID_ADDED_FULL			2104 ///< ͨ������
#define ERR_NET_PUI_APPCLT_ERR					2105 ///< ����ʧ��
#define ERR_NET_PUI_DEV_DEL_FAILED				2106 ///< ɾ���豸ʧ��
#define ERR_NET_PUI_OVER_MAX_USRNUM				2107 ///< ��������ﵽ����
#define ERR_NET_PUI_OVER_MAX_GROUP_NUM			2108 ///< ����ﵽ����
#define ERR_NET_PUI_OVER_MAX_CHN_NUM			2109 ///< ͨ�����ﵽ����
#define ERR_NET_PUI_LEN_NOT_ENOUGH				2110 ///< �����Ƿ�
#define ERR_NET_PUI_OVER_MAX_ACPT_BANDWIDTH		2111 ///< �������ﵽ����
#define ERR_NET_PUI_PTZ_TASK_RUNING				2112 ///< PTZ����ռ��
#define ERR_NET_PUI_VALID_DEV_UPGRADE_TASK      2113 ///< �޿�������������
#define ERR_NET_PUI_NO_DETECT_AREA				2114 ///< �ƶ����������������Ϊ0
#define ERR_NET_PUI_DEV_FORBIDDEN   			2115 ///< �豸����ֹ
#define ERR_NET_PUI_AUTH_ID_ERR_FORBIDDEN		2116 ///< ��Ȩ����

#define ERR_NET_VTDUCTRL						2200 ///< Nvrvtductrlģ����ش�����
#define ERR_NET_VTDU_APPCLT_STREAM_PREPARE_FAILED   2201 ///< ����ǰ������ʧ��
#define ERR_NET_VTDU_APPCLT_STREAM_START_FAILED 2202 ///< ����ǰ������ʧ��
#define ERR_NET_VTDU_SND_IS_FULL                2203 ///< ������������
#define ERR_NET_VTDU_SEN_RATE_OVER              2204 ///< ������������
#define ERR_NET_VTDU_DEV_OFFLINE                2205 ///< �豸����
#define ERR_NET_VTDU_MSIN_NO_STREAM             2206 ///< ��������ʧ��
#define ERR_NET_VTDU_MSIN_CREATE_FAILED         2207 ///< �����������ն���ʧ��
#define ERR_NET_VTDU_MSIN_SET_OPT_FAILED        2208 ///< �����������ղ���ʧ��
#define ERR_NET_VTDU_MSIN_SET_TRANSPARAM_FAILED 2209 ///< �����������ղ���ʧ��
#define ERR_NET_VTDU_MSIN_INPUT_DATA_FAILED     2210 ///< ��������ʧ��
#define ERR_NET_VTDU_MSIN_START_FAILED          2211 ///< ��ʼ��������ʧ��
#define ERR_NET_VTDU_MSIN_STOP_FAILED           2212 ///< ֹͣ��������ʧ��
#define ERR_NET_VTDU_MSIN_RELEASE_FAILED        2213 ///< ֹͣ��������ʧ��
#define ERR_NET_VTDU_ADD_PIPELINE_FAILED        2215 ///< ���������������ʧ��
#define ERR_NET_VTDU_REMOVE_PIPELINE_FAILED     2216 ///< ֹͣ����ʧ��
#define ERR_NET_VTDU_MSOUT_CREATE_FAILED        2217 ///< ���������������ʧ��
#define ERR_NET_VTDU_MSOUT_SET_OPT_FAILED       2218 ///< ���������������ʧ��
#define ERR_NET_VTDU_MSOUT_SET_TRANSPARAM_FAILED 2219 ///< ���������������ʧ��
#define ERR_NET_VTDU_MSOUT_SET_DATA_CB_FAILED   2220 ///< ���������������ʧ��
#define ERR_NET_VTDU_MSOUT_GET_DATA_POS_FAILED  2221 ///< ��ȡ��������ʧ��
#define ERR_NET_VTDU_MSOUT_GET_DATA_FAILED      2222 ///< ��ȡ��������ʧ��
#define ERR_NET_VTDU_MSOUT_RELEASE_DATA_FAILED  2223 ///< �ͷ���������ʧ��
#define ERR_NET_VTDU_MSOUT_STRAT_FAILED         2224 ///< ��ʼ����ʧ��
#define ERR_NET_VTDU_MSOUT_STOP_FAILED          2225 ///< ֹͣ����ʧ��
#define ERR_NET_VTDU_MSOUT_RELEASE_FAILED       2226 ///< ֹͣ����ʧ��
#define ERR_NET_VTDU_INPUT_VID_PARAM_INVALID	2227 ///< ������Ƶ�����Ƿ�
#define ERR_NET_VTDU_INPUT_AUD_PARAM_INVALID	2228 ///< ������Ƶ�����Ƿ�
#define ERR_NET_VTDU_IS_AUDCALLING				2230 ///< ���ں���
#define ERR_NET_VTDU_OVER_MAX_SND_BANDWIDTH		2231 ///< �������ʹ�������
#define ERR_NET_VTDU_BROADCASTING_NO_SUPPORT_CHN 2235 ///< ��֧�ֺ��е�ͨ��

#define ERR_NET_SMTP_ERR                        2301 ///< �ʼ�����ʧ��
#define ERR_NET_SMTP_FILE_LEN_ERR               2302 ///< �ʼ���������
#define ERR_NET_SMTP_PARAM_INVALID              2303 ///< �ʼ���������
#define ERR_NET_SMTP_CONNECT_SERVER_ERR         2304 ///< ���ӷ�����ʧ��
#define ERR_NET_SMTP_LOGIN_ERR                  2305 ///< �û���֤ʧ��
#define ERR_NET_SMTP_SEND_ERR                   2306 ///< ��������ʧ��
#define ERR_NET_SMTP_RECV_ERR                   2307 ///< ��������ʧ��
#define ERR_NET_SMTP_CONNECT_TIME_OUT           2308 ///< ����SMTP��������ʱ
#define ERR_NET_SMTP_RESPONSE_ERR               2309 ///< ���������
#define ERR_NET_SMTP_CONNECT_SSL_ERR            2300 ///< SSL����ʧ��
#define ERR_NET_SMTP_STARTTLS_ERR               2311 ///< �ʼ�����ʧ��
#define ERR_NET_SMTP_ASSERT_ERR                 2312 ///< �����Ƿ�
#define ERR_NET_SMTP_DOMAIN_ANALY_ERR           2313 ///< �ʼ���������ʧ��

#define ERR_NET_CTRLLIB_OVER_MAX_NUM			2701 ///< �������ؿ�֧�ֵ�����
#define ERR_NET_CTRLLIB_OTHER_OPT_IS_DOING		2702 ///< ����Ӧ������ʹ�ò��ؿ�
#define ERR_NET_CTRLLIB_SAME_NAME				2703 ///< �Ѵ���ͬ�����ؿ�
#define ERR_NET_CTRLLIB_WRITE_DB_FAIL			2705 ///< ���ؿ�д�����ݿ�ʧ��
#define ERR_NET_CTRLLIB_OTHER_USER_OPT_ALG		2706 ///< �����û����ڲ����㷨����
#define ERR_NET_CTRLLIB_CREATE_FILE_FAIL		2707 ///< �����ļ���ʧ��
#define ERR_NET_CTRLLIB_OPEN_DB_FAIL			2708 ///< ���ؿ�����ݿ�ʧ��
#define ERR_NET_CTRLLIB_CREATE_TABLE_FAIL		2709 ///< ���ؿⴴ���ļ���ʧ��
#define ERR_NET_CTRLLIB_EXE_SQL_FAIL			2710 ///< ���ؿ�ִ��SQL���ʧ��
#define ERR_NET_CTRLLIB_PIC_OVER_RAM_SIZE		2711 ///< ���ؿ�ͼƬ��С����������ڴ�
#define ERR_NET_CTRLLIB_SYS_CMD_FAIL			2712 ///< ���ؿ�ִ��ϵͳ����ʧ��
#define ERR_NET_CTRLLIB_OTHER_USER_IMPORT		2713 ///< ���ؿ⵼�빦�����������û�����ִ��
#define ERR_NET_CTRLLIB_CHECK_IMPORT_FAIL		2714 ///< ���ؿ⵼���ļ�У��ʧ��
#define ERR_NET_CTRLLIB_CREATE_THREAD_FAIL		2715 ///< ���ؿⴴ���߳�ʧ��
#define ERR_NET_CTRLLIB_EGI_FAIL				2716 ///< ��ȡ����ֵʧ��
#define ERR_NET_CTRLLIB_COMPARE_USED			2717 ///< ���ؿ����ڱ��ȶԹ���ʹ��
#define ERR_NET_CTRLLIB_OVER_SUP_MAX			2718 ///< ���ؿ���ӵĳ�Ա����������֧�ֵ�����
#define ERR_NET_AIS_PIC_QUERY_RESULT_OVER_NUM	2719 ///< ��ͼ��ͼ��ѯ�����϶࣬������С��ѯʱ�䷶Χ
#define ERR_NET_AIS_PIC_NO_FACE_PIC_FEATURE		2720 ///< δ��⵽����
#define ERR_NET_CTRLLIB_ADD_NO_EGI				2721 ///< ���ؿ���ӳ�Ա�ɹ�����û������ֵ
#define ERR_NET_AIS_OVER_SUP_DETECT_NUM			2722 ///< �������������
#define ERR_NET_AIS_ALG_INVALID					2723 ///< �㷨��Ч���Ҳ������㷨
#define ERR_NET_AIS_CREATE_DETECT_FAILED		2724 ///< ���������ʧ��
#define ERR_NET_AIS_OVER_CMP_CHN_NUM			2725 ///< �ȶ�ͨ�����������������
#define ERR_NET_AIS_RULE_NAME_EXIST				2726 ///< �ȶԹ������Ѵ���
#define ERR_NET_AIS_CMP_CTRLLIB_EXIST			2727 ///< �ȶԲ��ؿ��Ѵ���
#define ERR_NET_AIS_OVER_SUP_RULE_NUM			2728 ///< �ȶԹ�����������������
#define ERR_NET_AIS_CREATE_CMP_HANDLE_FAILED	2729 ///< �����ȶԾ��ʧ��
#define ERR_NET_CTRLLIB_MEM_ALREADY_DEL			2730 ///< ���ؿ����ɾ���ĳ�Ա���в���
#define ERR_NET_AIS_ALG_ERR_ALG_NOT_MATCH		2732 ///< ϵͳ�д��ڲ���ģ�������뵱ǰ�汾��ƥ��
#define ERR_NET_AIS_CTRLLIB_AGI_UPDATING		2733 ///< �㷨ģ�͸��£����ڸ�������ֵ

/**
 * netsdk�ڲ������붨��
 * �����뷶Χ6000~6499
 */

#define ERR_NET_INIT_FAILED						6000 ///< ��ʼ��ʧ��
#define ERR_NET_INVALID_HANDLE					6001 ///< �����Ч
#define ERR_NET_INVALID_PARAM					6002 ///< �����Ƿ�
#define ERR_NET_PARSE_FAILED					6003 ///< ����ʧ��
#define ERR_NET_CREATE_HANDLE_FAILED			6004 ///< �������ʧ��
#define ERR_NET_ALLOC_MEM_FAILED				6005 ///< �����ڴ�ʧ��
#define ERR_NET_SOCKET_OPT_FAILED				6006 ///< ������������ʧ��
#define ERR_NET_CONNECT_FAILED					6007 ///< ����ʧ��
#define ERR_NET_CONNECT_CLOSED					6008 ///< ���ӶϿ�
#define ERR_NET_SEND_FAILED						6009 ///< ����ʧ��
#define ERR_NET_RECV_FAILED						6010 ///< ����ʧ��
#define ERR_NET_SEND_TIMEOUT					6011 ///< ���ͳ�ʱ
#define ERR_NET_RECV_TIMEOUT					6012 ///< ���ճ�ʱ
#define ERR_NET_SEND_OUT_MEM					6013 ///< ���ͻ��岻��
#define ERR_NET_RECV_OUT_MEM					6014 ///< ���ջ��岻��

#endif
