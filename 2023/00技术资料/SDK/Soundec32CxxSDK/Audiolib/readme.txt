示例说明
备注：所有的算法库在添加的时候顺序都要排在HiFi3_VFPU_Library前面。
D_CONFIG_2MIC_BT_HEADPHONE、D_CONFIG_2MIC_MEETING_BOX、D_CONFIG_2MIC_USB_HEADPHONE只能设置其中一个为1。

1. 2mic_bt_headphone
1) 将boardConfig.h中的宏定义D_CONFIG_2MIC_BT_HEADPHONE设置为1。
2) 在配置选项Libraries中添加算法库speech_2mic_headset。
3) 打开宏定义代表打开算法，关闭宏定义算法关闭，直通模式，传输麦克风原始数据。

1. 2mic_meetingbox
1) 将boardConfig.h中的宏定义D_CONFIG_2MIC_MEETING_BOX设置为1。
2) 在配置选项Libraries中添加算法库speech_2mic_meetingbox。
3) 在user_config_2mic_meetingbox.h文件中选择MIC采样率为16K，位宽为16bit，SPK采样率为48K，16bit或者24bit。
4) 打开宏定义代表打开算法，关闭宏定义算法关闭，直通模式，传输麦克风原始数据。

1. 2mic_usb_headphone
1) 将boardConfig.h中的宏定义D_CONFIG_2MIC_USB_HEADPHONE设置为1。
2) 在配置选项Libraries中添加算法库speech_2mic_headset。
3) 在user_config_2mic_usb_headphone.h文件中选择MIC采样率为48K，位宽为16bit，SPK采样率为48K，16bit或者24bit。
4) 打开宏定义代表打开算法，关闭宏定义算法关闭，直通模式，传输麦克风原始数据。
