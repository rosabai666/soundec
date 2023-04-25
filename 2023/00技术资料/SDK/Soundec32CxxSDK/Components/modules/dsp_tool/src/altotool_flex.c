#include "drv_codec.h"
#include "audio_session.h"
#include "altotool.h"

#if SUPPORT_ALTOTOOL
extern codec_adc12_config_t g_adc12_config;
extern codec_adc34_config_t g_adc34_config;
extern codec_adc56_config_t g_adc56_config;
extern codec_adc3456_config_t g_adc3456_config;
extern codec_adc789a_config_t g_adc789a_config;
extern codec_dac_config_t g_dac_config;
extern auSessionDesc_t auSessionDesc[AUDIO_SESSION_MAX];

void overrideAdc12Cfg(void)
{
#if (SUPPORT_ALTOTOOL_CODEDC)
	codec_adc12_config_t *config = &g_adc12_config;
	config->sample_rate = gAdcCfg.gCfg[ADC_12].sr;
	config->amic.gid1 = gAdcCfg.chxCfg[ADC_1].gid;
	config->amic.gid2 = gAdcCfg.chxCfg[ADC_2].gid;
	config->amic.gim1 = gAdcCfg.chxCfg[ADC_1].gim;
	config->amic.gim2 = gAdcCfg.chxCfg[ADC_2].gim;
	config->aiadc1_sel = gAdcCfg.chxCfg[ADC_1].aiadc;
	config->aiadc2_sel = gAdcCfg.chxCfg[ADC_2].aiadc;
	config->hpf_enable = gAdcCfg.pairCfg[ADC_12].hpf;
	config->wnf = gAdcCfg.pairCfg[ADC_12].wnf;
	config->mix_rec = gAdcCfg.pairCfg[ADC_12].mixrec;

	config->agc_enable = gAdcCfg.pairCfg[ADC_12].agcCfg.agcEn;
	config->agc_config->stereo = gAdcCfg.pairCfg[ADC_12].agcCfg.stereo;
	config->agc_config->target = gAdcCfg.pairCfg[ADC_12].agcCfg.target;
	config->agc_config->snr_optimisation_enable = gAdcCfg.pairCfg[ADC_12].agcCfg.snrOptEn;
	config->agc_config->noise_gate_enable = gAdcCfg.pairCfg[ADC_12].agcCfg.ngEn;
	config->agc_config->noise_gate_threshold = gAdcCfg.pairCfg[ADC_12].agcCfg.ngThr;
	config->agc_config->hold_time = gAdcCfg.pairCfg[ADC_12].agcCfg.holdTime;
	config->agc_config->attack_time = gAdcCfg.pairCfg[ADC_12].agcCfg.atkTime;
	config->agc_config->decay_time = gAdcCfg.pairCfg[ADC_12].agcCfg.dcyTime;
	config->agc_config->max_gain = gAdcCfg.pairCfg[ADC_12].agcCfg.maxGain;
	config->agc_config->min_gain = gAdcCfg.pairCfg[ADC_12].agcCfg.minGain;
#endif
}

void overrideAdc34Cfg(void)
{
#if (SUPPORT_ALTOTOOL_CODEDC)
	codec_adc3456_config_t *groupConfig = &g_adc3456_config;
	groupConfig->sample_rate = gAdcCfg.gCfg[ADC_3456].sr;
	codec_adc34_config_t *config = &g_adc34_config;
	config->gid3 = gAdcCfg.chxCfg[ADC_3].gid;
	config->gid4 = gAdcCfg.chxCfg[ADC_4].gid;
	config->aiadc3_sel = gAdcCfg.chxCfg[ADC_3].aiadc;
	config->aiadc4_sel = gAdcCfg.chxCfg[ADC_4].aiadc;
	config->hpf_enable = gAdcCfg.pairCfg[ADC_34].hpf;
	config->wnf = gAdcCfg.pairCfg[ADC_34].wnf;
	config->mix_rec = gAdcCfg.pairCfg[ADC_34].mixrec;

	config->agc_enable = gAdcCfg.pairCfg[ADC_34].agcCfg.agcEn;
	config->agc_config->stereo = gAdcCfg.pairCfg[ADC_34].agcCfg.stereo;	
	config->agc_config->target = gAdcCfg.pairCfg[ADC_34].agcCfg.target;	
	config->agc_config->snr_optimisation_enable = gAdcCfg.pairCfg[ADC_34].agcCfg.snrOptEn;	
	config->agc_config->noise_gate_enable = gAdcCfg.pairCfg[ADC_34].agcCfg.ngEn;	
	config->agc_config->noise_gate_threshold = gAdcCfg.pairCfg[ADC_34].agcCfg.ngThr;	
	config->agc_config->hold_time = gAdcCfg.pairCfg[ADC_34].agcCfg.holdTime;	
	config->agc_config->attack_time = gAdcCfg.pairCfg[ADC_34].agcCfg.atkTime;	
	config->agc_config->decay_time = gAdcCfg.pairCfg[ADC_34].agcCfg.dcyTime;	
	config->agc_config->max_gain = gAdcCfg.pairCfg[ADC_34].agcCfg.maxGain;
	config->agc_config->min_gain = gAdcCfg.pairCfg[ADC_34].agcCfg.minGain;	
#endif

}

void overrideAdc56Cfg(void)
{
#if (SUPPORT_ALTOTOOL_CODEDC)
	codec_adc3456_config_t *groupConfig = &g_adc3456_config;
	groupConfig->sample_rate = gAdcCfg.gCfg[ADC_3456].sr;
	codec_adc56_config_t *config = &g_adc56_config;
	config->gid5 = gAdcCfg.chxCfg[ADC_5].gid;
	config->gid6 = gAdcCfg.chxCfg[ADC_6].gid;
	config->aiadc5_sel = gAdcCfg.chxCfg[ADC_5].aiadc;
	config->aiadc6_sel = gAdcCfg.chxCfg[ADC_6].aiadc;
	config->hpf_enable = gAdcCfg.pairCfg[ADC_56].hpf;
	config->wnf = gAdcCfg.pairCfg[ADC_56].wnf;
	config->mix_rec = gAdcCfg.pairCfg[ADC_56].mixrec;

	config->agc_enable = gAdcCfg.pairCfg[ADC_56].agcCfg.agcEn;
	config->agc_config->stereo = gAdcCfg.pairCfg[ADC_56].agcCfg.stereo;	
	config->agc_config->target = gAdcCfg.pairCfg[ADC_56].agcCfg.target;	
	config->agc_config->snr_optimisation_enable = gAdcCfg.pairCfg[ADC_56].agcCfg.snrOptEn;	
	config->agc_config->noise_gate_enable = gAdcCfg.pairCfg[ADC_56].agcCfg.ngEn;	
	config->agc_config->noise_gate_threshold = gAdcCfg.pairCfg[ADC_56].agcCfg.ngThr;	
	config->agc_config->hold_time = gAdcCfg.pairCfg[ADC_56].agcCfg.holdTime;	
	config->agc_config->attack_time = gAdcCfg.pairCfg[ADC_56].agcCfg.atkTime;	
	config->agc_config->decay_time = gAdcCfg.pairCfg[ADC_56].agcCfg.dcyTime;	
	config->agc_config->max_gain = gAdcCfg.pairCfg[ADC_56].agcCfg.maxGain;
	config->agc_config->min_gain = gAdcCfg.pairCfg[ADC_56].agcCfg.minGain;	
#endif

}

void overrideAdcUser(void)
{
	auSessionDesc_t *descriptor;
	auCaptureIf_t *capture;
	uart_printf("start override adc user\n");
	for(int i=0; i<AUDIO_SESSION_MAX;i++)
	{
	     descriptor = &auSessionDesc[i];
	     int captureNum = descriptor->captureNum;
	     for(int index = 0; index <captureNum; index++)
	     {
	          capture = descriptor->capture[index];
	          switch ((uint32_t)(capture->capture.devId))
	          {
	               case AUDIO_DEVICE_MEMS12_IN:
			           uart_printf("override adc12:%x, sr:%d\n",capture,capture->format.sampleRateHz );
	                   capture->format.sampleRateHz = g_adc12_config.sample_rate;
	                   break;
#if (CODEC_ADC34_ENABLE)
	               case AUDIO_DEVICE_MEMS34_IN:
	                   capture->format.sampleRateHz = g_adc3456_config.sample_rate;
	                   break;
#endif
#if (CODEC_ADC56_ENABLE)
	               case AUDIO_DEVICE_MEMS56_IN:
	                   capture->format.sampleRateHz = g_adc3456_config.sample_rate;			
	                   break;
#endif
#if (CODEC_ADC78_ENABLE)
	               case AUDIO_DEVICE_MEMS78_IN:
	                   capture->format.sampleRateHz = g_adc789a_config.sample_rate;		
	                   break;
#endif
#if (CODEC_ADC9A_ENABLE)
	               case AUDIO_DEVICE_MEMS9A_IN:
	                   capture->format.sampleRateHz = g_adc789a_config.sample_rate;
	                   break;
#endif
	          }
	     }
	}		 
}

void populateAdcParameters(void)
{
        uart_printf("restore adc, gim1:%d, gim2:%d,gid1:%d,gid2:%d\n", gAdcCfg.chxCfg[0].gim,gAdcCfg.chxCfg[1].gim,
			gAdcCfg.chxCfg[0].gid,gAdcCfg.chxCfg[1].gid);
	overrideAdc12Cfg();
	overrideAdc34Cfg();
	overrideAdc56Cfg();
	overrideAdcUser();
}

void resetAdcParameters()
{
	gAdcCfg.chxCfg[ADC_1].en = true;	
	gAdcCfg.chxCfg[ADC_2].en = true;
	for(int i = ADC_3; i<=ADC_6; i++)
	{
		gAdcCfg.chxCfg[i].en = false;
	}
#if D_CONFIG_2MIC_MEETING_BOX
	gAdcCfg.chxCfg[ADC_3].en = true;
	gAdcCfg.chxCfg[ADC_4].en = true;
	uart_printf("chxCfg[2]:%d, chxCfg[3]:%d\n",gAdcCfg.chxCfg[ADC_3].en,gAdcCfg.chxCfg[ADC_4].en);
#endif	
}
void overrideDacCfg(void)
{
#if (SUPPORT_ALTOTOOL_CODEDC)
	codec_dac_config_t *config = &g_dac_config;
	config->sample_rate = gDacCfg.sr;
	config->aidacl_sel = gDacCfg.chxCfg[DAC_L].aidac;
	config->aidacr_sel = gDacCfg.chxCfg[DAC_R].aidac;
	config->gol = gDacCfg.chxCfg[DAC_L].gom;
	config->gor = gDacCfg.chxCfg[DAC_R].gom;
	config->godl = gDacCfg.chxCfg[DAC_L].god;
	config->godr = gDacCfg.chxCfg[DAC_R].god;
	
	config->drc_enable = gDacCfg.drcEn;
	config->lcomp= gDacCfg.chxCfg[DAC_L].drcComp;
	config->rcomp= gDacCfg.chxCfg[DAC_R].drcComp;
	config->rthres = gDacCfg.chxCfg[DAC_R].drcThr;
	config->lthres = gDacCfg.chxCfg[DAC_L].drcThr;
	
	config->dac_mix = gDacCfg.mix;
	uart_printf("dac mix is :%d\n", config->dac_mix);
	config->gimixl = gDacCfg.chxCfg[DAC_L].gimix;
	config->gimixr = gDacCfg.chxCfg[DAC_R].gimix;
	config->gomixl = gDacCfg.chxCfg[DAC_L].gomix;
	config->gomixr = gDacCfg.chxCfg[DAC_R].gomix;

	config->mixdacl_sel = gDacCfg.chxCfg[DAC_L].mixdac;
	config->mixdacl_sel = gDacCfg.chxCfg[DAC_R].mixdac;
#endif
}

//no need to override dac sampling rate as it can not be changed right now
void overrideDacUser(void)
{
}

void populateDacParameters(void)
{
	overrideDacCfg();
	overrideDacUser();
}

void overrideI2SCfg(i2s_cfg_t *i2s_cfg, int index){
#if (SUPPORT_ALTOTOOL_I2S)
	i2s_cfg->mode 	 	 = gI2sCfg.chxCfg[index].isMaster;
	i2s_cfg->standard	 = gI2sCfg.chxCfg[index].format;
	i2s_cfg->audioFreq 	 = gI2sCfg.chxCfg[index].sr;
	i2s_cfg->txDataLength = gI2sCfg.chxCfg[index].dataWidth;
	i2s_cfg->rxDataLength = gI2sCfg.chxCfg[index].dataWidth;
	i2s_cfg->frameLength  = gI2sCfg.chxCfg[index].frameLength;
	i2s_cfg->txEnable 	 = gI2sCfg.chxCfg[index].txEn;
	i2s_cfg->rxEnable 	 = gI2sCfg.chxCfg[index].rxEn;
#endif
}

void populateI2SParameters(void){
#if (SUPPORT_ALTOTOOL_I2S)
	auSessionDesc_t *descriptor;
	auCaptureIf_t *capture;
	auPlaybackIf_t *playback;
	auProcIf_t *procIf;
	for(int i=0; i<AUDIO_SESSION_MAX;i++)
	{
		descriptor = &auSessionDesc[i];
		procIf = descriptor->procIf;
		int captureNum = descriptor->captureNum;
		for(int index = 0; index <captureNum; index++)
		{
			capture = descriptor->capture[index];
#if (I2S1_ENABLE)
			 if(capture->capture.devId == AUDIO_DEVICE_I2S1) 
			 {
			         capture->format.sampleRateHz = gI2sCfg.chxCfg[index].sr;
			 }
#endif
#if (I2S2_ENABLE)
			if (capture->capture.devId == AUDIO_DEVICE_I2S2)
			{
			 capture->format.sampleRateHz = gI2sCfg.chxCfg[index].sr;
			}
#endif
#if (I2S3_ENABLE)
			if (capture->capture.devId == AUDIO_DEVICE_I2S3)
			{
			 capture->format.sampleRateHz = gI2sCfg.chxCfg[index].sr;
			}
#endif
		}
		int playbackNum = descriptor->playbackNum;
		for(int index = 0; index <playbackNum; index++)
		{
			playback = descriptor->playback[index];
#if (I2S1_ENABLE)
			if(playback->playback.devId == AUDIO_DEVICE_I2S1) 
			{
			     playback->format.sampleRateHz = gI2sCfg.chxCfg[index].sr;
				if(procIf != NULL)
				{
		 			procIf->outputFormat.sampleRateHz = gI2sCfg.chxCfg[index].sr;
				}
			}
#endif
#if (I2S2_ENABLE)
			if(playback->playback.devId == AUDIO_DEVICE_I2S2) 
			{
			     playback->format.sampleRateHz = gI2sCfg.chxCfg[index].sr;
				if(procIf != NULL)
				{
		 			procIf->outputFormat.sampleRateHz = gI2sCfg.chxCfg[index].sr;
				}
			}
#endif
#if (I2S3_ENABLE)
			if(playback->playback.devId == AUDIO_DEVICE_I2S3) 
			{
			     playback->format.sampleRateHz = gI2sCfg.chxCfg[index].sr;
				if(procIf != NULL)
				{
		 			procIf->outputFormat.sampleRateHz = gI2sCfg.chxCfg[index].sr;
				}
			}
#endif
		}
	}
#endif
}

void resetI2SParameters(void)
{
	gI2sCfg.chxCfg[I2S_1].en = gI2sCfg.chxCfg[I2S_2].en = gI2sCfg.chxCfg[I2S_3].en = false;
#if D_CONFIG_2MIC_BT_HEADPHONE
	gI2sCfg.chxCfg[I2S_3].en = true;
#endif
}
void populateAlgoParameters(void)
{
}
#endif

