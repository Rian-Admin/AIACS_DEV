#ifndef _NETSDKERRNO_H_
#define _NETSDKERRNO_H_

/**
 * 说明：错误码定义文件，该文件将所有错误码合并，包括cgiapp错误码、业务错误码、netsdk内部错误码
 * 1.cgiapp错误码范围1~500
 * 2.业务错误码范围1000~3000
 * 3.netsdk内部错误码范围6000~6499
 */

#define NET_OK									0    ///< 操作成功

/**
 * cgiapp错误码定义
 * 错误码范围1~500
 */

#define ERR_NET_UPGRADE_PREPARE_OK				1    ///< 设备升级准备就绪
#define ERR_NET_UPGRADE_SUCCESS					2    ///< 设备升级成功
#define ERR_NET_SYSTEM_REBOOT					3    ///< 设备重启
#define ERR_NET_FACTORY_DEF						4    ///< 恢复出厂设置
#define ERR_NET_URL_CHANGE						5    ///< CGI服务地址变化
#define ERR_NET_PTZ_UPGRADE						6    ///< 云台升级
#define ERR_NET_ISUPGRADE						7    ///< 设备正在升级
#define ERR_NET_NO_SEC_MAIL						8    ///< 安全邮箱未设置
#define ERR_NET_ALREADY_ACT						9    ///< 设备已激活
#define ERR_NET_ALREADY_LOGIN					10   ///< 用户已登录
#define ERR_NET_SYSTEM_SHUTDOWN					11   ///< 设备关闭

#define ERR_NET_UNKNOW							201  ///< 协议错误
#define ERR_NET_USERNAME_ERR					202  ///< 用户不存在
#define ERR_NET_NOT_AUTH						203  ///< 鉴权失败
#define ERR_NET_PASS_ERR						204  ///< 密码错误
#define ERR_NET_AUTHID_ERR						205  ///< 鉴权ID错误
#define ERR_NET_NO_POWER						206  ///< 用户无权限
#define ERR_NET_IP_DENY							207  ///< IP被锁定
#define ERR_NET_OLDPASS_ERR						208  ///< 旧密码错误
#define ERR_NET_USER_EXISTED					209  ///< 用户已存在
#define ERR_NET_URL_NO_SUPPORT					210  ///< 此功能不支持或信令非法
#define ERR_NET_NEED_BODY						211  ///< 信令非法
#define ERR_NET_XML_ERR							212  ///< 信令非法
#define ERR_NET_XML_ROOT_ERR					213  ///< 信令非法
#define ERR_NET_PARAM_LOST						214  ///< 信令非法
#define ERR_NET_PARMM_TOOLONG					215  ///< 信令非法
#define ERR_NET_CHECK_ERR						216  ///< 升级信息校验失败
#define ERR_NET_FILE_ERR						217  ///< 升级文件校验失败
#define ERR_NET_UPGRADE_FAILURE					218  ///< 设备升级失败
#define ERR_NET_PARAM_ERR						219  ///< 信令非法
#define ERR_NET_IP_ERR							220  ///< IP非法
#define ERR_NET_MASK_ERR						221  ///< IP掩码错误
#define ERR_NET_GATEWAY_ERR						222  ///< IP网关错误
#define ERR_NET_DNS_ERR							223  ///< DNS地址错误
#define ERR_NET_MULTICAST_ERR					224  ///< 组播地址错误
#define ERR_NET_MTU_ERR							225  ///< MTU参数错误
#define ERR_NET_SEQ_ERR							226  ///< 信令时序错误
#define ERR_NET_URL_ERR							227  ///< URL格式错误
#define ERR_NET_INVALID_ARG						228  ///< 参数非法
#define ERR_NET_DEVICE_BUSY						229  ///< 设备正忙
#define ERR_NET_RECOVER							230  ///< 操作失败，参数恢复
#define ERR_NET_USER_BANNED						231  ///< 用户被锁定
#define ERR_NET_USER_ACTIVE						232  ///< 设备未激活
#define ERR_NET_NO_MEM							233  ///< 内存不足
#define ERR_NET_BUF_TOO_SMALL					234  ///< 数据缓冲区不足
#define ERR_NET_NOT_FOUND						235  ///< 信令非法
#define ERR_NET_NO_IMPLEMENTED					236  ///< 功能不支持
#define ERR_NET_ALREADY_EXIST					237  ///< 已存在
#define ERR_NET_NEED_LOGIN						238  ///< 未登录
#define ERR_NET_USER_DISABLED					239  ///< 用户被禁用
#define ERR_NET_USER_PORT_OCCUPIED				240  ///< 端口被占用
#define ERR_NET_USER_USERS_TOPLIMIT				241  ///< 登录用户已达上限
#define ERR_NET_IP_OCCUPIED						242  ///< IP被占用

/**
 * 业务错误码定义
 * 错误码范围1000~3000
 */

#define ERR_NET_ERROR							1001 ///< 操作失败，请稍后重试
#define ERR_NET_ASSERT							1002 ///< 参数非法
#define ERR_NET_SEM_TAKE_FAILED					1003 ///< 操作失败，请稍后重试
#define ERR_NET_SEM_GIVEE_FAILED				1004 ///< 操作失败，请稍后重试
#define ERR_NET_PARAM_INVALID					1005 ///< 参数非法
#define ERR_NET_WAIT_TIMEOUT					1006 ///< 操作超时
#define ERR_NET_MALLOC_FAILED					1007 ///< 分配内存失败
#define ERR_NET_STRING_ILLEGAL					1008 ///< 字符串非法
#define ERR_NET_STRING_TOO_SHORT				1009 ///< 字符串过短
#define ERR_NET_STRING_TOO_LONG					1010 ///< 字符串过长

#define ERR_NET_FILE_INEXIST					1020 ///< 文件不存在
#define ERR_NET_WRITE_FILE						1021 ///< 写文件失败
#define ERR_NET_EXPORTING_CFG					1022 ///< 正在导入或导出配置文件
#define ERR_NET_REGISTER_FULL					1023 ///< 注册个数达到上限
#define ERR_NET_ALREADY_REGISTERED				1024 ///< 已注册
#define ERR_NET_NO_REGISTERED					1025 ///< 未注册
#define ERR_NET_IO_ERROR						1026 ///< IO错误,硬盘不存在或者硬盘满

#define ERR_NET_CAP_NO_SUPPORT					1100 ///< 能力不支持

#define ERR_NET_USER_EXIST        				1200 ///< 用户已存在
#define ERR_NET_USER_INEXIST      				1201 ///< 用户不存在
#define ERR_NET_USER_FILE_BROKEN  				1202 ///< 文件损坏
#define ERR_NET_USER_NUM_MAX      				1203 ///< 用户数达到上限
#define ERR_NET_USER_DEL_DISALLOWED      		1204 ///< 该用户禁止删除
#define ERR_NET_USER_NAME_ILLEGAL 				1205 ///< 用户名包含非法字符
#define ERR_NET_USER_PWD_ILLEGAL  				1206 ///< 用户密码包含非法字符
#define ERR_NET_USER_NAME_LEN_TOO_LONG 			1207 ///< 用户名过长
#define ERR_NET_USER_NAME_LEN_TOO_SHORT 		1208 ///< 用户名过短
#define ERR_NET_USER_PWD_LEN_TOO_LONG 			1209 ///< 用户密码过长
#define ERR_NET_USER_PWD_LEN_TOO_SHORT 			1210 ///< 用户密码过短
#define ERR_NET_USER_PWD_STRENGTH_WEAK 			1211 ///< 用户密码强度太弱
#define ERR_NET_USER_MGR_EMAIL_ILLEGAL			1212 ///< 邮箱格式错误
#define ERR_NET_USER_REMOTE_IP_INFO_ILLEGAL		1213 ///< 远程登录IP非法
#define ERR_NET_USER_NAME_MDY_DISALLOWED		1214 ///< 用户名不允许修改
#define ERR_NET_USER_ADMIN_PERM_MDY_DISALLOWED	1215 ///< 无权限修改
#define ERR_NET_USER_DEV_SYS_NOACTIVE			1216 ///< 设备未激活
#define ERR_NET_USER_PASS_SAME_TO_BEFORE		1217 ///< 与旧密码相同
#define ERR_NET_USER_NAME_NULL           		1218 ///< 用户名不能为空
#define ERR_NET_USER_PASS_NULL          		1219 ///< 密码不能为空

#define ERR_NET_LOG								1300 ///< 日志相关
#define ERR_NET_USER_LOG_TASK_BUSY				1301 ///< 日志任务忙，请稍后再试
#define ERR_NET_USER_LOG_TASK_ID_UNAVAILABLE    1302 ///< 无效的任务ID

#define ERR_NET_DEV								1400 ///< 外设

#define ERR_NET_NET								1500 ///< 网络
#define ERR_NET_NET_REGISTER_FULL				1501 ///< 注册个数达到上限
#define ERR_NET_NET_ALREADY_REGISTERED			1502 ///< 已注册
#define ERR_NET_NET_NO_REGISTERED				1503 ///< 未注册
#define ERR_NET_NET_PING_NUM_MAX				1504 ///< 当前ping达到上限
#define ERR_NET_NET_DOMAIN_ANALY_FAILD			1505 ///< 域名解析失败
#define ERR_NET_NET_PORT_IS_USING    			1506 ///< 网络端口被占用
#define ERR_NET_NET_OTHER_IP_IN_SAME_NET    	1507 ///< 与其他IP在相同网段
#define ERR_NET_NET_IP_GW_NOTIN_SAME_NET    	1508 ///< IP和网关不在同一网段
#define ERR_NET_NET_OPERATE_TOO_FREQUENCY		1509 ///< 操作过于频繁，请稍后再试
#define ERR_NET_NET_PING_CHN_NO_IP				1510 ///< 无设备IP地址

#define ERR_NET_MPU								1600 ///< mpu相关错误
#define ERR_NET_MPU_DEC_CHN_NUM_OVER_MAX		1601 ///< 解码通道数已达上限
#define ERR_NET_MPU_DEC_ABILITY_OVER_MAX		1602 ///< 超出解码能力
#define ERR_NET_MPU_CHN_ID_IS_DECODING			1603 ///< 该通道正在解码
#define ERR_NET_MPU_MC_SET_LAYOUT_FAILED		1604 ///< 设置画面风格失败
#define ERR_NET_MPU_MC_SET_DEC_PARAM_FAILED		1605 ///< 创建解码器失败
#define ERR_NET_MPU_MC_SET_OPT_FAILED			1606 ///< 设置参数失败
#define ERR_NET_MPU_MC_GET_OPT_FAILED			1607 ///< 获取参数失败
#define ERR_NET_MPU_OVER_MC_DEV_ZOOM_CAP		1608 ///< 超出缩放能力
#define ERR_NET_MPU_BIND_FAILED					1609 ///< 绑定失败
#define ERR_NET_MPU_UNBIND_FAILED				1610 ///< 解绑失败

#define ERR_NET_MEDIA							1700 ///< media相关错误

#define ERR_NET_DM								1800 ///< 硬盘管理相关错误
#define ERR_NET_DM_DISK_ID_INVALID				1801 ///< 硬盘ID无效
#define ERR_NET_DM_DISK_IN_USE					1802 ///< 硬盘正在使用
#define ERR_NET_DM_NET_DISK_NAME_TOO_LONG		1803 ///< 硬盘名称过长
#define ERR_NET_DM_FUNCTION_NOT_SUPPORT			1804 ///< 功能不支持
#define ERR_NET_DM_DISK_USED_BY_RP				1805 ///< 硬盘正在使用
#define ERR_NET_DM_DISK_USED_BY_RP_PLY			1806 ///< 正在放像
#define ERR_NET_DM_DISK_USED_BY_RP_DLD			1807 ///< 正在下载
#define ERR_NET_DM_DISK_UMOUNT_PART_ERR			1808 ///< 卸载失败
#define ERR_NET_DM_DISK_FORMAT_PART_ERR			1809 ///< 格式化失败
#define ERR_NET_DM_DISK_CHG_CALLBACK_FULL		1810 ///< 硬盘满
#define ERR_NET_DM_DISK_EXPCPTION_FORBID_OPERATION 1811 ///< 硬盘异常禁止操作
#define ERR_NET_DM_DISK_EXTERNAL_DISK_LIMIT     1812 ///< 外置存储设备达到上限
#define ERR_NET_DM_DISK_BAD_SECTOR_CHECK_NO_TASKID 1813 ///< 坏道检测任务达到上限
#define ERR_NET_DM_DISK_DISK_FS_TYPE_FAILD      1814 ///< 硬盘格式错误
#define ERR_NET_DM_DISK_RAID_HOTBACKUP_DISK_SIZE_ERROR      1815 ///< 热备盘容量过小
#define ERR_NET_DM_DISK_SET_QUTOA_SIZE_OVER_ALL_DISK_SIZE      1816 ///< 设置配额的容量大于磁盘总容量
#define ERR_NET_DM_DISK_RAID_DELING             1817 ///< raid正在删除中
#define ERR_NET_DM_DISK_RAID_CREATING           1818 ///< raid正在构建中
#define ERR_NET_DM_DISK_JUST_SUP_ONE_SMART_DISK 1820 ///< 只支持一个智能盘

#define ERR_NET_REC								1900 ///< 录放像相关错误
#define ERR_NET_REC_CFG_DATA_NOT_EXIST			1901 ///< 录像配置不存在
#define ERR_NET_REC_NO_IDLE_PLY_TASK			1902 ///< 录像任务达到上限
#define ERR_NET_REC_MSIN_STOP_FAILED			1903 ///< 停止接收录像数据失败
#define ERR_NET_REC_MSIN_RELEASE_FAILED			1904 ///< 停止接收录像数据失败
#define ERR_NET_REC_REPEAT_TO_ADD_CHN			1905 ///< 通道重复添加
#define ERR_NET_REC_START_PLY_FAILED			1906 ///< 开启放像失败
#define ERR_NET_REC_BAKUP_TASK_FULL				1907 ///< 录像备份任务达到上限
#define ERR_NET_REC_IMG_BAK_TASK_FULL			1908 ///< 图片备份任务达到上限
#define ERR_NET_REC_CHN_NOT_START				1909 ///< 通道录像未开启
#define ERR_NET_REC_CMD_DEAL_THREAD_BUSY		1910 ///< 处理线程忙
#define ERR_NET_REC_PART_BUSY	            	1911 ///< 录像分区忙
#define ERR_NET_REC_COMPONENT_LIB_ERR   		1912 ///< 录像错误
#define ERR_NET_REC_DISK_STATUS_SLEEP   		1913 ///< 磁盘正在唤醒
#define ERR_NET_REC_PLAYER_FULL   				1914 ///< 当前放像任务已满，部分放像开启失败
#define ERR_NET_REC_AUD_INVALID   				1915 ///< 无有效音频

#define ERR_NET_CFG								2000 ///< 配置相关错误
#define ERR_NET_CFG_OPEN_DATABASE_FALID			2001 ///< 打开数据库失败
#define ERR_NET_CFG_CLOSE_DATABASE_FALID		2002 ///< 关闭数据库失败
#define ERR_NET_CFG_CREATE_TABLE_FALID			2003 ///< 创建表失败
#define ERR_NET_CFG_GET_PARAM_FALID				2004 ///< 获取参数失败
#define ERR_NET_CFG_SET_PARAM_FALID				2005 ///< 设置参数失败
#define ERR_NET_CFG_NO_THIS_DATA				2006 ///< 参数非法
#define ERR_NET_CFG_NO_TABLE					2007 ///< 参数非法
#define ERR_NET_CFG_INPORT_CFG_DEV_ERR			2008 ///< 导入配置与设备型号不匹配
#define ERR_NET_CFG_INPORT_CFG_CRC_FAILED		2009 ///< 导入配置文件校验失败

#define ERR_NET_PUI								2100 ///< pui模块相关错误
#define ERR_NET_PUI_CHNID_ADDED					2101 ///< 该通道已占用
#define ERR_NET_PUI_DEV_REPEAT_ADD				2102 ///< 设备重复添加
#define ERR_NET_PUI_DEV_ADD_FAILED				2103 ///< 设备添加失败
#define ERR_NET_PUI_CHNID_ADDED_FULL			2104 ///< 通道已满
#define ERR_NET_PUI_APPCLT_ERR					2105 ///< 操作失败
#define ERR_NET_PUI_DEV_DEL_FAILED				2106 ///< 删除设备失败
#define ERR_NET_PUI_OVER_MAX_USRNUM				2107 ///< 搜索任务达到上限
#define ERR_NET_PUI_OVER_MAX_GROUP_NUM			2108 ///< 分组达到上限
#define ERR_NET_PUI_OVER_MAX_CHN_NUM			2109 ///< 通道数达到上限
#define ERR_NET_PUI_LEN_NOT_ENOUGH				2110 ///< 参数非法
#define ERR_NET_PUI_OVER_MAX_ACPT_BANDWIDTH		2111 ///< 接入带宽达到上限
#define ERR_NET_PUI_PTZ_TASK_RUNING				2112 ///< PTZ任务被占用
#define ERR_NET_PUI_VALID_DEV_UPGRADE_TASK      2113 ///< 无可利用升级任务
#define ERR_NET_PUI_NO_DETECT_AREA				2114 ///< 移动侦测区域数量不能为0
#define ERR_NET_PUI_DEV_FORBIDDEN   			2115 ///< 设备被禁止
#define ERR_NET_PUI_AUTH_ID_ERR_FORBIDDEN		2116 ///< 鉴权错误

#define ERR_NET_VTDUCTRL						2200 ///< Nvrvtductrl模块相关错误码
#define ERR_NET_VTDU_APPCLT_STREAM_PREPARE_FAILED   2201 ///< 开启前端码流失败
#define ERR_NET_VTDU_APPCLT_STREAM_START_FAILED 2202 ///< 开启前端码流失败
#define ERR_NET_VTDU_SND_IS_FULL                2203 ///< 超出发送能力
#define ERR_NET_VTDU_SEN_RATE_OVER              2204 ///< 超出发送能力
#define ERR_NET_VTDU_DEV_OFFLINE                2205 ///< 设备下线
#define ERR_NET_VTDU_MSIN_NO_STREAM             2206 ///< 码流接收失败
#define ERR_NET_VTDU_MSIN_CREATE_FAILED         2207 ///< 创建码流接收对象失败
#define ERR_NET_VTDU_MSIN_SET_OPT_FAILED        2208 ///< 设置码流接收参数失败
#define ERR_NET_VTDU_MSIN_SET_TRANSPARAM_FAILED 2209 ///< 设置码流接收参数失败
#define ERR_NET_VTDU_MSIN_INPUT_DATA_FAILED     2210 ///< 码流接收失败
#define ERR_NET_VTDU_MSIN_START_FAILED          2211 ///< 开始码流接收失败
#define ERR_NET_VTDU_MSIN_STOP_FAILED           2212 ///< 停止码流接收失败
#define ERR_NET_VTDU_MSIN_RELEASE_FAILED        2213 ///< 停止码流接收失败
#define ERR_NET_VTDU_ADD_PIPELINE_FAILED        2215 ///< 创建码流输出对象失败
#define ERR_NET_VTDU_REMOVE_PIPELINE_FAILED     2216 ///< 停止发送失败
#define ERR_NET_VTDU_MSOUT_CREATE_FAILED        2217 ///< 创建码流输出对象失败
#define ERR_NET_VTDU_MSOUT_SET_OPT_FAILED       2218 ///< 设置码流输出属性失败
#define ERR_NET_VTDU_MSOUT_SET_TRANSPARAM_FAILED 2219 ///< 设置码流传输参数失败
#define ERR_NET_VTDU_MSOUT_SET_DATA_CB_FAILED   2220 ///< 设置码流输出属性失败
#define ERR_NET_VTDU_MSOUT_GET_DATA_POS_FAILED  2221 ///< 获取码流数据失败
#define ERR_NET_VTDU_MSOUT_GET_DATA_FAILED      2222 ///< 获取码流数据失败
#define ERR_NET_VTDU_MSOUT_RELEASE_DATA_FAILED  2223 ///< 释放码流数据失败
#define ERR_NET_VTDU_MSOUT_STRAT_FAILED         2224 ///< 开始发送失败
#define ERR_NET_VTDU_MSOUT_STOP_FAILED          2225 ///< 停止发送失败
#define ERR_NET_VTDU_MSOUT_RELEASE_FAILED       2226 ///< 停止发送失败
#define ERR_NET_VTDU_INPUT_VID_PARAM_INVALID	2227 ///< 输入视频参数非法
#define ERR_NET_VTDU_INPUT_AUD_PARAM_INVALID	2228 ///< 输入音频参数非法
#define ERR_NET_VTDU_IS_AUDCALLING				2230 ///< 正在呼叫
#define ERR_NET_VTDU_OVER_MAX_SND_BANDWIDTH		2231 ///< 超出发送带宽能力
#define ERR_NET_VTDU_BROADCASTING_NO_SUPPORT_CHN 2235 ///< 无支持呼叫的通道

#define ERR_NET_SMTP_ERR                        2301 ///< 邮件发送失败
#define ERR_NET_SMTP_FILE_LEN_ERR               2302 ///< 邮件附件过大
#define ERR_NET_SMTP_PARAM_INVALID              2303 ///< 邮件参数错误
#define ERR_NET_SMTP_CONNECT_SERVER_ERR         2304 ///< 连接服务器失败
#define ERR_NET_SMTP_LOGIN_ERR                  2305 ///< 用户认证失败
#define ERR_NET_SMTP_SEND_ERR                   2306 ///< 发送数据失败
#define ERR_NET_SMTP_RECV_ERR                   2307 ///< 接收数据失败
#define ERR_NET_SMTP_CONNECT_TIME_OUT           2308 ///< 连接SMTP服务器超时
#define ERR_NET_SMTP_RESPONSE_ERR               2309 ///< 返回码错误
#define ERR_NET_SMTP_CONNECT_SSL_ERR            2300 ///< SSL连接失败
#define ERR_NET_SMTP_STARTTLS_ERR               2311 ///< 邮件加密失败
#define ERR_NET_SMTP_ASSERT_ERR                 2312 ///< 参数非法
#define ERR_NET_SMTP_DOMAIN_ANALY_ERR           2313 ///< 邮件域名解析失败

#define ERR_NET_CTRLLIB_OVER_MAX_NUM			2701 ///< 超过布控库支持的数量
#define ERR_NET_CTRLLIB_OTHER_OPT_IS_DOING		2702 ///< 其他应用正在使用布控库
#define ERR_NET_CTRLLIB_SAME_NAME				2703 ///< 已存在同名布控库
#define ERR_NET_CTRLLIB_WRITE_DB_FAIL			2705 ///< 布控库写入数据库失败
#define ERR_NET_CTRLLIB_OTHER_USER_OPT_ALG		2706 ///< 其他用户正在操作算法引擎
#define ERR_NET_CTRLLIB_CREATE_FILE_FAIL		2707 ///< 创建文件表失败
#define ERR_NET_CTRLLIB_OPEN_DB_FAIL			2708 ///< 布控库打开数据库失败
#define ERR_NET_CTRLLIB_CREATE_TABLE_FAIL		2709 ///< 布控库创建文件表失败
#define ERR_NET_CTRLLIB_EXE_SQL_FAIL			2710 ///< 布控库执行SQL语句失败
#define ERR_NET_CTRLLIB_PIC_OVER_RAM_SIZE		2711 ///< 布控库图片大小超过分配的内存
#define ERR_NET_CTRLLIB_SYS_CMD_FAIL			2712 ///< 布控库执行系统命令失败
#define ERR_NET_CTRLLIB_OTHER_USER_IMPORT		2713 ///< 布控库导入功能已有其他用户正在执行
#define ERR_NET_CTRLLIB_CHECK_IMPORT_FAIL		2714 ///< 布控库导入文件校验失败
#define ERR_NET_CTRLLIB_CREATE_THREAD_FAIL		2715 ///< 布控库创建线程失败
#define ERR_NET_CTRLLIB_EGI_FAIL				2716 ///< 提取特征值失败
#define ERR_NET_CTRLLIB_COMPARE_USED			2717 ///< 布控库正在被比对规则使用
#define ERR_NET_CTRLLIB_OVER_SUP_MAX			2718 ///< 布控库添加的成员总数超过了支持的总数
#define ERR_NET_AIS_PIC_QUERY_RESULT_OVER_NUM	2719 ///< 以图搜图查询数量较多，建议缩小查询时间范围
#define ERR_NET_AIS_PIC_NO_FACE_PIC_FEATURE		2720 ///< 未检测到人脸
#define ERR_NET_CTRLLIB_ADD_NO_EGI				2721 ///< 布控库添加成员成功，但没有特征值
#define ERR_NET_AIS_OVER_SUP_DETECT_NUM			2722 ///< 超出最大检测能力
#define ERR_NET_AIS_ALG_INVALID					2723 ///< 算法无效，找不到此算法
#define ERR_NET_AIS_CREATE_DETECT_FAILED		2724 ///< 创建检测句柄失败
#define ERR_NET_AIS_OVER_CMP_CHN_NUM			2725 ///< 比对通道个数超过最大能力
#define ERR_NET_AIS_RULE_NAME_EXIST				2726 ///< 比对规则名已存在
#define ERR_NET_AIS_CMP_CTRLLIB_EXIST			2727 ///< 比对布控库已存在
#define ERR_NET_AIS_OVER_SUP_RULE_NUM			2728 ///< 比对规则个数超过最大能力
#define ERR_NET_AIS_CREATE_CMP_HANDLE_FAILED	2729 ///< 创建比对句柄失败
#define ERR_NET_CTRLLIB_MEM_ALREADY_DEL			2730 ///< 布控库对已删除的成员进行操作
#define ERR_NET_AIS_ALG_ERR_ALG_NOT_MATCH		2732 ///< 系统中存在部分模型数据与当前版本不匹配
#define ERR_NET_AIS_CTRLLIB_AGI_UPDATING		2733 ///< 算法模型更新，正在更新特征值

/**
 * netsdk内部错误码定义
 * 错误码范围6000~6499
 */

#define ERR_NET_INIT_FAILED						6000 ///< 初始化失败
#define ERR_NET_INVALID_HANDLE					6001 ///< 句柄无效
#define ERR_NET_INVALID_PARAM					6002 ///< 参数非法
#define ERR_NET_PARSE_FAILED					6003 ///< 解析失败
#define ERR_NET_CREATE_HANDLE_FAILED			6004 ///< 创建句柄失败
#define ERR_NET_ALLOC_MEM_FAILED				6005 ///< 申请内存失败
#define ERR_NET_SOCKET_OPT_FAILED				6006 ///< 设置网络属性失败
#define ERR_NET_CONNECT_FAILED					6007 ///< 连接失败
#define ERR_NET_CONNECT_CLOSED					6008 ///< 连接断开
#define ERR_NET_SEND_FAILED						6009 ///< 发送失败
#define ERR_NET_RECV_FAILED						6010 ///< 接收失败
#define ERR_NET_SEND_TIMEOUT					6011 ///< 发送超时
#define ERR_NET_RECV_TIMEOUT					6012 ///< 接收超时
#define ERR_NET_SEND_OUT_MEM					6013 ///< 发送缓冲不足
#define ERR_NET_RECV_OUT_MEM					6014 ///< 接收缓冲不足

#endif
