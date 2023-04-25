/** 
***********************************************************
 * 
 * @file name	: playTone.c
 * @author		: yingen.yu
 * @version		: V0.1
 * @date		: 2021-10-28
 * @brief		: play the prompt tones
 * 
***********************************************************
 * @attention
 * 
 * Copyright (c) 2021 Soundec Co., Ltd. 
 * All rights reserved.
 * 
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user
 * who uses the software, his/her employer (which must be your employer
 * and Soundec Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 * 
***********************************************************/
/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "drv_flash.h"

#include "playTone.h"
#if SUPPORT_PLAY_TONE
/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	U32 offsetAddr;
	U32 fileSize;
}TONE_INFO_ST;

/* Private macro -------------------------------------------------------------*/
//#define TONE_CHECK_ID				0x3355 // don't modify,keep same with wav2bin tool(old) 
#define TONE_FLASH_SIZE_MAX			0x80000 //limit  max size 512K
#define ONE_TIME_READ_SIZE			((TONES_SAMPLERATE/1000) * (TONES_BPS/8) * 16)//0x400
#define TONE_CACHE_BUFFER_SIZE		(ONE_TIME_READ_SIZE*4)//0x1000L	
#define FADE_IN_OUT_POINTS			256//128

/* Private variables ---------------------------------------------------------*/
static TONE_INFO_ST fileAddrTab[TONES_TOTAL_NUM] ;
static U8 totalNum = 0;
static U8 playingId = 0xff;
#if (PREEMPTIVE_MODE == 1)
static U8 playingId_2nd = 0xff;
#endif
static float splMult[PLAY_TONE_MAX] = {0};
static float splCount[PLAY_TONE_MAX] = {0};

static U8 cacheBuf[TONE_CACHE_BUFFER_SIZE];//4K buffer cache the data read from flash
static U32 readOffset[PLAY_TONE_MAX] = {0};
static U32 writeOffset = 0;
#ifdef PLAY_TONE_FADE_IN_OUT
static U16 fadePointCount[PLAY_TONE_MAX] = {0};//fade in fade out points count
#endif
#if (PREEMPTIVE_MODE == 1)
static U16 playtonefadePointCount[PLAY_TONE_MAX] = {0};//fade in fade out points count
#endif

uint8_t *toneTab;
uint8_t strLen;
uint8_t playStrId = 0xff;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static U32 getFileOffest()
{
	return fileAddrTab[playingId].offsetAddr;
}
static U32 getFileSize()	
{
	if(playingId < 20)
		return fileAddrTab[playingId].fileSize;
	else
		return 0;
}

uint32_t get_read_offset(void)
{
	uint32_t maxOffset = readOffset[PLAY_TONE_DAC];

	if(maxOffset < readOffset[PLAY_TONE_I2S])
	{
		maxOffset = readOffset[PLAY_TONE_I2S];
	}
	if(maxOffset < readOffset[PLAY_TONE_USB])
	{
		maxOffset = readOffset[PLAY_TONE_USB];
	}

	return maxOffset;
}

void clear_read_offset(void)
{
	readOffset[PLAY_TONE_DAC] = 0;
	readOffset[PLAY_TONE_I2S] = 0;
	readOffset[PLAY_TONE_USB] = 0;
}
/* Public functions ---------------------------------------------------------*/

/** 
 * @fn		PlayToneInit
 * @brief	[load tones info from flash]
 * @author	yingen.yu
 * @param	[none]
 * @return	[none]
 */
void PlayToneInit()
{
	U32 readData[TONES_TOTAL_NUM*2] = {0};
	U16 tempBuf[TONES_TOTAL_NUM*4 + 2];

#if 1//support new wav tools
	/*****************************
	* BYTE0 bits
	* BYTE1 totalNum
	* BYTE2-BYTE5 offsetAddr0
	* BYTE6-BYTE9 fileSize0
	* BYTE10-BYTE13 offsetAddr1
	* BYTE14-BYTE17 fileSize1
	* ......
	*******************************/
	flash_read_buf(USER_FLASH_TONE_START, (U32*)tempBuf, TONES_TOTAL_NUM*2 + 1);
	memcpy(readData, tempBuf+1, TONES_TOTAL_NUM*2*4);
	for(totalNum = 0; totalNum < TONES_TOTAL_NUM; totalNum++)
	{
		if(readData[totalNum*2] > TONE_FLASH_SIZE_MAX || readData[1 + totalNum*2] > TONE_FLASH_SIZE_MAX)
		{
			uart_printf("PlayToneInit failed totalNum:%d!!! \n", totalNum);
			break;
		}
		fileAddrTab[totalNum].offsetAddr = readData[totalNum*2];
		fileAddrTab[totalNum].fileSize = readData[1 + totalNum*2];
	//	uart_printf("read tone suncess totalNum:%d, addr:0x%x, size:0x%x \n", totalNum, fileAddrTab[totalNum].offsetAddr, fileAddrTab[totalNum].fileSize);
	}
#else
	U32 offsetAddr = 0;
	U16 toneSize = 0;
	U16 checkID = 0;
	while(offsetAddr < TONE_FLASH_SIZE_MAX && totalNum < DC_TONES_MAX)
	{
		flash_read_buf(USER_FLASH_TONE_START + offsetAddr, (U32*)&readData, 1);
		checkID = readData&0xffff;
		toneSize = (readData>>16)&0xffff;
		if(checkID == TONE_CHECK_ID && toneSize > 0 && toneSize < 0x8000)
		{
			fileAddrTab[totalNum].offsetAddr = offsetAddr;
			fileAddrTab[totalNum].fileSize = toneSize*2;
			totalNum += 1;
		//	uart_printf("read tone suncess totalNum:%d, addr:0x%x, size:0x%x \n", totalNum, offsetAddr, toneSize);
			if(toneSize&0x1)
				offsetAddr = offsetAddr + (toneSize + 1)*2 + 4;
			else
				offsetAddr = offsetAddr + toneSize*2 + 4;
			readData = 0;
		}
		else
		{
			uart_printf("PlayToneInit read data from flash failed!!! \n");
			break;
		}
	}
#endif
/*
	int i;
	for(i = 0; i < totalNum; i++)
	{
		uart_printf("fileAddrTab[%d]->addr:0x%x, size:0x%x \n", i, fileAddrTab[i].offsetAddr, fileAddrTab[i].fileSize);
	}
*/
}

#if (PREEMPTIVE_MODE == 1)
int PlayToneResetFor2nd(void)
{

    if(playingId_2nd == 0xff)
    {
        uart_printf("playtonefadePointCount :error case !!!!!!!!");
    }
    else
    {
        playingId = 0xff;
    }

    return 1;
}

int PlayToneDetectFor2nd(void)
{
    if((playingId_2nd != 0xff) && (playingId == 0xff))
    {
        PlayToneSwitch(playingId_2nd);
    }

    return 1;
}
#endif

/** 
 * @fn		PlayToneSwitch
 * @brief	[description]
 * @author	yingen.yu
 * @param	[description]
 *		@arg	playId       [defined according to customer requirements, such as DC_TONES_ID]
 * @return	[description]
 *		@arg	0			[success]
 *				-1			[failed]
 */
int PlayToneSwitch(U8 playId)
{

	//test for play all tones
#if 0
	static U8 testID = 0;
	if(playId == DC_TONES_VOL_UP)
	{
	if(testID < totalNum)
		testID++;
	else
		testID = 0;
	playId = testID;
	}
#endif
	if(playId < totalNum && playingId == 0xff)
	{
		writeOffset = 0;
		clear_read_offset();
		playingId = playId;
#ifdef PLAY_TONE_FADE_IN_OUT
		fadePointCount[PLAY_TONE_DAC] = FADE_IN_OUT_POINTS;
		fadePointCount[PLAY_TONE_I2S] = FADE_IN_OUT_POINTS;
		fadePointCount[PLAY_TONE_USB] = FADE_IN_OUT_POINTS;
#endif
        #if (PREEMPTIVE_MODE == 1)
        playingId_2nd = 0xff;
        playtonefadePointCount[PLAY_TONE_DAC] = 0;
        playtonefadePointCount[PLAY_TONE_I2S] = 0;
        playtonefadePointCount[PLAY_TONE_USB] = 0;
		splCount[PLAY_TONE_DAC] = 0;
		splCount[PLAY_TONE_I2S] = 0;
		splCount[PLAY_TONE_USB] = 0;
        #endif
	//	uart_printf("PlayToneSwitch success playingId:%d,totalNum:%d getFileSize():%d \n\n", playingId, totalNum, fileAddrTab[playingId].fileSize);
		PlayToneLoadDataProc();
		return 0;
	}
    #if (PREEMPTIVE_MODE == 1)
    else if (playId < totalNum && playingId != 0xff)
    {
        playingId_2nd = playId;
        if(playtonefadePointCount[PLAY_TONE_DAC] == 0)
        {
            playtonefadePointCount[PLAY_TONE_DAC] = FADE_IN_OUT_POINTS;
        }
        if(playtonefadePointCount[PLAY_TONE_I2S] == 0)
        {
            playtonefadePointCount[PLAY_TONE_I2S] = FADE_IN_OUT_POINTS;
        }
        if(playtonefadePointCount[PLAY_TONE_USB] == 0)
        {
            playtonefadePointCount[PLAY_TONE_USB] = FADE_IN_OUT_POINTS;
        }
        uart_printf("PlayToneSwitch wait, current playingId:%d, playingId_2nd:%d playtonefadePointCount:%d, %d, %d\n\n", playingId, playingId_2nd, 
        playtonefadePointCount[PLAY_TONE_DAC], playtonefadePointCount[PLAY_TONE_I2S], playtonefadePointCount[PLAY_TONE_USB]);
        return 0;
    }
    #endif
	else
	{
		uart_printf("PlayToneSwitch fail playid:%d, totalnum:%d, playingId:%d!!!! \n", playId, totalNum, playingId);
		return -1;
	}
}
/** 
 * @brief	Play a combined tone
 * @note	New requests are returned while the previous tone is playing
 * @param[in]	strTab      Array of combined tone ID
 * @param[in]	len         The length of the strTab array 
 * @return	result
 * 	@retval 0 success
 * 	@retval -1 failed
 */
int PlayToneString(uint8_t* strTab, uint8_t len)
{
	if(/*playStrId != 0xff || */strTab == NULL || len < 2)
	{
		uart_printf("PlayToneString failed strLen:%d, playStrId:%d!!!\n", strLen, playStrId);
		return -1;
	}	
	toneTab = strTab;
	strLen = len;
	playStrId = 0;
	PlayToneSwitch(toneTab[playStrId]);
	uart_printf("PlayToneString strLen:%d, playStrId:%d\n", strLen, playStrId);
	return 0;
}

/** 
 * @fn		PlayToneGetData
 * @brief	[provide data to codec]
 * @author	yingen.yu
 * @param	[description]
 *		@arg	readBuf       [Read only one point at a time]
 				fadeScale	  [0~1 for fade in fade out,avoid pop noise]
 * @return	[description]
 *		@arg	-1			[no tone is playing]
 *				0			[play over]
 *				1			[get data failed when playing or fade_in_fade_out is doing]
 *				2			[play start]
 *				3			[playing]
 */
#if (PREEMPTIVE_MODE == 1)
int PlayToneGetData(eToneOutputSrc src, int32_t* readBuf, float* fadeScale, float* fadeScaleTone, uint32_t samplerate)
#else
int PlayToneGetData(eToneOutputSrc src, int32_t* readBuf, float* fadeScale, uint32_t samplerate)
#endif
{
	if(samplerate == 0)
		return -1;
	splMult[src] = (float)samplerate/TONES_SAMPLERATE;
	if(splCount[src] == 0)
		splCount[src] = splMult[src];
	if(playingId < totalNum && readOffset[src] < getFileSize())
	{
#ifdef PLAY_TONE_FADE_IN_OUT//fade in
		if(readOffset[src] == 0 && fadePointCount[src] > 0)
		{
			fadePointCount[src]--;
			*fadeScale = (float)fadePointCount[src]/(FADE_IN_OUT_POINTS*splMult[src]);
			*readBuf = 0;
            
            #if (PREEMPTIVE_MODE == 1)
            if ((fadePointCount[src] == 0) && (playingId_2nd != 0xff))
            {
                PlayToneResetFor2nd();
            }
            #endif
			return 1;
		}
#endif
		*fadeScale = 0;
		if(readOffset[src] < writeOffset)
		{
			int ret = 3;
			
			if(readOffset[src] == 0)
				ret = 2;
				
			if(TONES_BPS == 16)
			{
				*readBuf = (int32_t)(((S16*)cacheBuf)[(readOffset[src]%TONE_CACHE_BUFFER_SIZE)>>1]) << 16;
				if(splMult[src] < 1)//means samplerate < TONES_SAMPLERATE
				{
					splCount[src] = 0;
					readOffset[src] = readOffset[src] + 2/splMult[src];
					
				}
				else
				{
					if(--splCount[src] < 1)
					{
						readOffset[src] = readOffset[src] + 2;
						splCount[src] += splMult[src];
					}
				}
			}
			else if(TONES_BPS == 8)
			{
				if(((S8*)cacheBuf)[readOffset[src]%TONE_CACHE_BUFFER_SIZE] == 0xff)
				{
					*readBuf = 0;
				}
				else
					*readBuf = ((int32_t)(((S8*)cacheBuf)[readOffset[src]%TONE_CACHE_BUFFER_SIZE] + 0x80)<<24);
				if(splMult[src] < 1)
				{
					splCount[src] = 0;
					readOffset[src] = readOffset[src] + 1/splMult[src];
					
				}
				else
				{
					if(--splCount[src] < 1)
					{
						readOffset[src] = readOffset[src] + 1;
						splCount[src] += splMult[src];
					}
				}
			}
			else if(TONES_BPS == 32)
			{
				*readBuf = ((int32_t*)cacheBuf)[(readOffset[src]%TONE_CACHE_BUFFER_SIZE) >> 2];
				if(splMult[src] < 1)
				{
					splCount[src] = 0;
					readOffset[src] = readOffset[src] + 4/splMult[src];
					
				}
				else
				{
					if(--splCount[src] < 1)
					{
						readOffset[src] = readOffset[src] + 4;
						splCount[src] += splMult[src];
					}
				}
			}
			else if(TONES_BPS == 24)
			{
				*readBuf = ((int32_t)(((S8*)cacheBuf)[(readOffset[src]%TONE_CACHE_BUFFER_SIZE)]) << 8) + ((int32_t)(((S8*)cacheBuf)[(readOffset[src]%TONE_CACHE_BUFFER_SIZE) + 1]) << 16)
						+ ((int32_t)(((S8*)cacheBuf)[(readOffset[src]%TONE_CACHE_BUFFER_SIZE) + 2]) << 24);
				if(splMult[src] < 1)
				{
					splCount[src] = 0;
					readOffset[src] = readOffset[src] + 3/splMult[src];
					
				}
				else
				{
					if(--splCount[src] < 1)
					{
						readOffset[src] = readOffset[src] + 3;
						splCount[src] += splMult[src];
					}
				}
			}
			#if (PREEMPTIVE_MODE == 1)
            if (playtonefadePointCount[src] != 0)
            {
                *fadeScaleTone = (float)playtonefadePointCount[src]/(FADE_IN_OUT_POINTS * splMult[src]);
                playtonefadePointCount[src]--;
                if(playtonefadePointCount[src] == 0)
                {
                    PlayToneResetFor2nd();
                    *readBuf = 0;
                }
            }
            #endif
			return ret;
		}
		else
		{
			*readBuf = 0;
			return 1;//wait data
		}
	}
	else
	{
		if(readOffset[src] == getFileSize() && readOffset[src] > 0)
		{
#ifdef PLAY_TONE_FADE_IN_OUT//fade out
			if(fadePointCount[src] < (FADE_IN_OUT_POINTS * splMult[src]))
			{
				fadePointCount[src]++;
				*fadeScale = (float)fadePointCount[src]/(FADE_IN_OUT_POINTS * splMult[src]);
				*readBuf = 0;
				return 1;
			}
			else
#endif
			{
				playingId = 0xff;
				readOffset[src] = 0;
				writeOffset = 0;
				*fadeScale = 0;
				splCount[src] = 0;

				if(playStrId != 0xff && toneTab != NULL)
				{
					if(++playStrId < strLen)
					{
						PlayToneSwitch(toneTab[playStrId]);
					}
					else
					{
						playStrId = 0xff;
						toneTab = NULL;
						strLen = 0;
					}
				//	uart_printf("playstring strLen:%d, toneTab[%d]:%d\n", strLen, playStrId, toneTab[playStrId]);
				}
				return 0;//play end
			}
		}
		return -1;
	}
}

/** 
 * @fn		PlayToneLoadDataProc
 * @brief	[load data to cachebuf read from flash]
 * @author	yingen.yu
 * @param	[none]
 * @return	[description]
 *		@arg	-1			[no tone is playing]
 *				0			[load data success]
 * @note	[first time read full buffer size,then every time read 1K size or less]
 */
int PlayToneLoadDataProc(void)
{
	if(playingId < totalNum && writeOffset < getFileSize())
	{
		U32 readSize = (getFileSize() - writeOffset) > TONE_CACHE_BUFFER_SIZE ? TONE_CACHE_BUFFER_SIZE:(getFileSize() - writeOffset);
		U32 startAddr = USER_FLASH_TONE_START + getFileOffest() + 4;
		
		if(writeOffset == 0)
		{
			flash_read_buf(startAddr, (U32*)cacheBuf, readSize/4);
		}
		else if(writeOffset < getFileSize() && (writeOffset - get_read_offset()) < TONE_CACHE_BUFFER_SIZE
			&& ((TONE_CACHE_BUFFER_SIZE + get_read_offset() - writeOffset) > ONE_TIME_READ_SIZE))
		{
			startAddr += writeOffset;
			readSize = ONE_TIME_READ_SIZE;
			U32* readbuf = (U32*)(cacheBuf + (writeOffset%TONE_CACHE_BUFFER_SIZE));
			int readLen = readSize/4;
			if((readSize + writeOffset) > getFileSize() && getFileSize() > writeOffset)
			{
				readSize = getFileSize() - writeOffset;
				if(readSize&0x3)
					readLen = readSize/4 + 1;
				else
					readLen = readSize/4;
			}
			flash_read_buf(startAddr, readbuf, readLen);
		/*
			if(readSize < ONE_TIME_READ_SIZE)
			{
				int i;
				for(i=0; i < readSize/4; i++)
				{
					if(i%16 == 0)
						uart_printf("\n");
					uart_printf("%08x, ", readbuf[i]);
				}
			}
		*/
		}
		else
		{
			return -1;
		}

		writeOffset += readSize;
		
	}
	return 0;
}

#endif //SUPPORT_PLAY_TONE

