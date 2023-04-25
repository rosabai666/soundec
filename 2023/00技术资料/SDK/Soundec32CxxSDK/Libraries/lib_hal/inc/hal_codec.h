/** 
***********************************************************
 * 
 * @file name	: hal_codec.h
 * @author		: Kevin
 * @version		: V00.10
 * @date		: 2020-02-24
 * @brief		: Dolphin Codec header file
 * 
***********************************************************
 * @attention
 * 
 * Copyright (c) 2020 Soundec Co., Ltd. 
 * All rights reserved.
 * 
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between the user
 * who uses the software, his/her employer (which must be your employer
 * and Soundec Co., Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 * 
***********************************************************/

#ifndef __HAL_CODEC_H__
#define __HAL_CODEC_H__

#include "types.h"
#include "hal_codec_reg.h"
//#include "boardConfig.h"

#ifdef	__cplusplus
extern "C"
{
#endif

/** 
 * @brief This function is used to transform the sampling rate to Codec register value.
 * @param[in] fs Sample rate
 * @return The Codec register value.
 */
int codec_samplerate_transform(uint32_t fs);

#ifdef CONFIG_CODEC_ENABLE_AIAS_IRQ
/** 
 * @brief This function is used to mask the AIAS Unlock interrupt
 */
void hal_codec_mask_aiasunlock_irq(void);

/** 
 * @brief This function is used to unmask the AIAS Unlock interrupt
 */
void hal_codec_unmask_aiasunlock_irq(void);

/** 
 * @brief This function is used to get AIAS-Unlock IRQ Flag
 */
bool hal_codec_get_aias_unlock_irq_flag(void);

/** 
 * @brief This function is used to clear AIAS-Unlock IRQ Flag
 */
void hal_codec_clear_aias_unlock_irq_flag(void);

/** 
 * @brief    This function is used to check there is at least one pending IRQ
 * @return   
 * 	@retval true Codec has generated at least one interrupt request on IRQ output that has not been handled yet
 * 	@retval false  Codec does not have any pending interrupt request on IRQ output
 */
bool hal_codec_get_pending_irq_status(void);

/** 
 * @brief    Get DAC AIAS lock state
 * @return   
 * 	@retval true AIAS is locked and operating normally.
 * 	@retval false AIAS is not locked. Data from the audio interface are automatically muted.
 */
bool hal_codec_get_dac_aias_lock_state(void);

/** 
 * @brief    Get ADC12 AIAS lock state
 * @return   
 * 	@retval true AIAS is locked and operating normally.
 * 	@retval false AIAS is not locked. Data from the audio interface are automatically muted.
 */
bool hal_codec_get_adc12_aias_lock_state(void);

/** 
 * @brief    Get ADC3456 AIAS lock state
 * @return   
 * 	@retval true AIAS is locked and operating normally.
 * 	@retval false AIAS is not locked. Data from the audio interface are automatically muted.
 */
bool hal_codec_get_adc3456_aias_lock_state(void);

/** 
 * @brief    Acknowledge status bit after DAC AIAS is locked
 * @return   
 * 	@retval true Measured mean frequency of incoming audio data is not within supported values.
 * 	@retval false Measured mean frequency of incoming audio data is within supported values.
 */
bool hal_codec_check_dac_aias_unknown_fs(void);

/** 
 * @brief    Acknowledge status bit after ADC12 AIAS is locked
 * @return   
 * 	@retval true Measured mean frequency of incoming audio data is not within supported values.
 * 	@retval false Measured mean frequency of incoming audio data is within supported values.
 */
bool hal_codec_check_adc12_aias_unknown_fs(void);

/** 
 * @brief    Acknowledge status bit after ADC3456 AIAS is locked
 * @return   
 * 	@retval true Measured mean frequency of incoming audio data is not within supported values.
 * 	@retval false Measured mean frequency of incoming audio data is within supported values.
 */

bool hal_codec_check_adc3456_aias_unknown_fs(void);
#endif

#ifdef CONFIG_CODEC_ENABLE_WT
/** 
 * @brief This function is used to mask the Whisper Trigger interrupt
 */
void hal_codec_mask_wt_irq(void);

/** 
 * @brief This function is used to unmask the Whisper Trigger interrupt
 */
void hal_codec_unmask_wt_irq(void);

/** 
 * @brief This function is used to clear Whisper Trigger IRQ Flag
 */
void hal_codec_clear_wt_irq_flag(void);

/** 
 * @brief    Get Pending IRQ_WT status
 * @return   
 * 	@retval true Codec has generated at least one interrupt request on IRQ_WT output that has not been handled yet.
 * 	@retval false Codec does not have any pending interrupt request on IRQ_WT output.
 */
bool hal_codec_get_irq_wt_pending_status(void);
#endif

#ifdef CONFIG_CODEC_ENABLE_HP_SC_IRQ
/** 
 * @brief This function is used to mask the HP Output Short Cut interrupt
 */
void hal_codec_mask_sc_irq(void);

/** 
 * @brief This function is used to unmask the HP Output Short Cut interrupt
 */
void hal_codec_unmask_sc_irq(void);

/** 
 * @brief This function is used to get Headphone Short Circuit IRQ Flag
 */
bool hal_codec_get_sc_irq_flag(void);

/** 
 * @brief This function is used to clear Headphone Short Circuit IRQ Flag
 */
void hal_codec_clear_sc_irq_flag(void);
#endif

/** 
 * @brief	This function is used to check the analog part of the Codec is ready or not
 * @return	
 * 	@retval true The analog part of the Codec is ready.
 * 	@retval false The analog part of the Codec is not ready.
 */
bool hal_codec_check_analog_ready(void);

/** 
 * @brief	Set Analog DAC sleep mode or not
 * @param[in]	sleep       Enter or exit sleep mode
 * 	@arg true	Analog DAC enter sleep mode
 *  @arg false	Analog DAC exit sleep mode
 */
void hal_codec_analog_dac_sleep(bool sleep);

/** 
 * @brief	Set Analog ADC sleep mode or not
 * @param[in]	sleep       Enter or exit sleep mode
 * 	@arg true	Analog ADC enter sleep mode
 *  @arg false	Analog ADC exit sleep mode
 */
void hal_codec_analog_adc_sleep(bool sleep);

/** 
 * @brief	Set Analog standby mode or not
 * @param[in]	standby		Enter or exit standby mode
 * 	@arg true	The analog part of codec enters standby mode
 *  @arg false	The analog part of codec exits standby mode
 */
void hal_codec_analog_standby(bool standby);

/** 
 * @brief	Set Digital standby mode or not
 * @param[in]	standby		Enter or exit standby mode
 * 	@arg true	The digital part of codec enters standby mode
 *  @arg false	The digital part of codec exits standby mode
 */
void hal_codec_digital_standby(bool standby);

/** 
 * @brief	Set MCLK frequency
 * @param[in]	crystal     Indication of MCLK frequency, this parameter can only be one of the type #hal_codec_crystal_t
 */
void hal_codec_set_crystal(hal_codec_crystal_t crystal);

/** 
 * @brief This function is used to set DAC audio interface to parallel mode
 */
void hal_codec_set_dac_audioif_parallel(void);

/** 
 * @brief	Set the DAC audio interface. (Master or Slave)
 * @param[in]	mode        The DAC audio interface mode is set. this parameter can only be one of the type #hal_codec_ai_mode_t
 */
void hal_codec_set_dac_master_slave_mode(hal_codec_ai_mode_t mode);

/** 
 * @brief	This function sets the activation of the DAC audio interface
 * @param[in]	active      DAC audio interface activation is set.
 * 	@arg true DAC audio interface active
 * 	@arg false DAC audio interface in power-down mode
 */
void hal_codec_activate_dac_audio_interface(bool active);

/** 
 * @brief	This function sets DAC digital filters audio Data Word Length selection for DSP protocol transmission
 * @param[in]	wordLength  DAC digital filters audio Data Word Length selection is set. this parameter can only be one of the type #hal_codec_adwl_t
 */
void hal_codec_set_dac_data_word_length(hal_codec_adwl_t wordLength);

/** 
 * @brief	Set the sampling rate of the DAC interface
 * @param[in]	fs          The sampling rate of the DAC interface is set, this parameter can only be one of the type #hal_codec_fs_t
 */
void hal_codec_set_dac_sample_rate(hal_codec_fs_t fs);

/** 
 * @brief	This function sets the validation of DAC Frequency Tuning Control Word
 * @note	In case DAC audio interface Frequency Tuning Control Word needs an update, set validation to be false, then call #hal_codec_tune_dac_sample_rate, then set validation to be true
 * @param[in]	valid       Validation of DAC Frequency Tuning Control Word is set
 * 	@arg true Validate DAC Frequency Tuning Control Word.
 *  @arg false Invalidate DAC Frequency Tuning Control Word.
 */
void hal_codec_tune_dac_sample_rate_valid(bool valid);

/** 
 * @brief	This function is used to fine tune the sampling rate of the master DAC interface
 * @note	@sa hal_codec_tune_dac_sample_rate_valid()
 * @param[in]	adj         The tuning control word is set. The equation is adj=128*Fmclk*(1/Fs - 1/Fs0), where Fs is the tuned sampling frequency, Fs0 is the normalized sampling frequency, Fmclk is MCLK frequency.
 */
void hal_codec_tune_dac_sample_rate(uint16_t adj);

/** 
 * @brief 	This function is used to mute the DAC interface
 */
void hal_codec_mute_dac(void);

/** 
 * @brief 	This function is used to unmute the DAC interface
 */
void hal_codec_unmute_dac(void);

/** 
 * @brief 	This function is used to activate or deactivate DAC
 * @param[in] active Indication of activation or deactivation to be set.
 *  @arg true DAC is active
 *  @arg false DAC in power-down mode
 */
void hal_codec_activate_dac(bool active);

/** 
 * @brief	This function is used to set the analog right channel(analog gain) of DAC be disabled or not.
 * @param[in]	leftOnly    Disable the analog right channel(analog gain) of DAC or not.
 * 	@arg true The analog right channel(analog gain) of DAC is disabled.
 *  @arg false The analog right channel(analog gain) of DAC is enabled.
 */
void hal_codec_set_dac_left_channel_only(bool leftOnly);

/** 
 * @brief This function is used to set the DAC left channel input selection
 * @note The digital mixer is AIDACL_SEL, the destination data interface is I_DAC1_INL, the inputs are DAC1_INL and DAC1_INR
 * @param[in] input The DAC left channel input selection is set, this parameter can only be one of the type #hal_codec_channel_sel_t
 *  @arg 0 Normal inputs, means that the input of I_DAC1_INL is DAC1_INL.
 *  @arg 1 Cross inputs, means that the input of I_DAC1_INL is DAC1_INR.
 *  @arg 2 Mixed inputs, means that the input of I_DAC1_INL is (DAC1_INL+DAC1_INR)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
void hal_codec_select_dac_aidacl_input(hal_codec_channel_sel_t input);

/** 
 * @brief This function is used to set the DAC left channel input selection
 * @note The digital mixer is AIDACR_SEL, the destination data interface is I_DAC1_INR, the inputs are DAC1_INL and DAC1_INR
 * @param[in] input The DAC left channel input selection is set, this parameter can only be one of the type #hal_codec_channel_sel_t
 *  @arg 0 Normal inputs, means that the input of I_DAC1_INR is DAC1_INR.
 *  @arg 1 Cross inputs, means that the input of I_DAC1_INR is DAC1_INL.
 *  @arg 2 Mixed inputs, means that the input of I_DAC1_INR is (DAC1_INL+DAC1_INR)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
void hal_codec_select_dac_aidacr_input(hal_codec_channel_sel_t input);

/** 
 * @brief This function is used to set the mixer mode on DAC Path
 * @note The digital mixer is DAC_MIX.
 * @param[in] input The mixer mode on DAC Path is set, this parameter can only be one of the type #hal_codec_dac_mixer_mode_t
 * 	@arg 0 Playback DAC only.
 *  @arg 1 Playback DAC + ADC.
 */
void hal_codec_select_dac_mixer_mode(hal_codec_dac_mixer_mode_t mode);

/** 
 * @brief This function is used to set the mixer right channel input selection on DAC path
 * @note The digital mixer is MIXDACL_SEL, the destination data interface is I_DILMIX, the inputs are DIL and DIR
 * @param[in] input The mixer right channel input selection on DAC path is set. This parameter can only be one of the type #hal_codec_channel_sel_t
 *  @arg 0 Normal inputs, means that the input of I_DILMIX is DIL.
 *  @arg 1 Cross inputs, means that the input of I_DILMIX is DIR.
 *  @arg 2 Mixed inputs, means that the input of I_DILMIX is (DIL+DIR)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
void hal_codec_select_dac_mixdacl_input(hal_codec_channel_sel_t input);

/** 
 * @brief This function is used to set the mixer right channel input selection on DAC path
 * @note The digital mixer is MIXDACR_SEL, the destination data interface is I_DIRMIX, the inputs are DIL and DIR
 * @param[in] input The mixer right channel input selection on DAC path is set. This parameter can only be one of the type #hal_codec_channel_sel_t
 *  @arg 0 Normal inputs, means that the input of I_DIRMIX is DIR.
 *  @arg 1 Cross inputs, means that the input of I_DIRMIX is DIL.
 *  @arg 2 Mixed inputs, means that the input of I_DIRMIX is (DIL+DIR)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
void hal_codec_select_dac_mixdacr_input(hal_codec_channel_sel_t input);

/** 
 * @brief This function is used to set the mixer channel 1 input selection on ADC path
 * @note The digital mixer is MIXADCL_SEL, the destination data interface is I_ADC_INL, the inputs are ADC_INL and ADC_INR
 * @param[in] input The mixer channel 1 input selection is set. This parameter can only be one of the type #hal_codec_channel_sel_t
 *  @arg 0 Normal inputs, means that the input of I_ADC_INL is ADC_INL.
 *  @arg 1 Cross inputs, means that the input of I_ADC_INL is ADC_INR.
 *  @arg 2 Mixed inputs, means that the input of I_ADC_INL is (ADC_INL+ADC_INR)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
void hal_codec_select_dac_mixadc1_input(hal_codec_channel_sel_t input);

/** 
 * @brief This function is used to set the mixer channel 2 input selection on ADC path
 * @note The digital mixer is MIXADCR_SEL, the destination data interface is I_ADC_INR, the inputs are ADC_INL and ADC_INR
 * @param[in] input The mixer channel 1 input selection is set. This parameter can only be one of the type #hal_codec_channel_sel_t
 *  @arg 0 Normal inputs, means that the input of I_ADC_INR is ADC_INR.
 *  @arg 1 Cross inputs, means that the input of I_ADC_INR is ADC_INL.
 *  @arg 2 Mixed inputs, means that the input of I_ADC_INR is (ADC_INL+ADC_INR)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
void hal_codec_select_dac_mixadc2_input(hal_codec_channel_sel_t input);

/** 
 * @brief This function is used to set the left channel DAC digital gain
 * @param[in] gain The left channel DAC digital gain is set. The range is -31 dB to 32 dB, by step of 1 dB
 */
void hal_codec_set_dac_godl(int8_t gain);

/** 
 * @brief This function is used to set the right channel DAC digital gain
 * @param[in] gain The right channel DAC digital gain is set. The range is -31 dB to 32 dB, by step of 1 dB
 */
void hal_codec_set_dac_godr(int8_t gain);

/** 
 * @brief This function is used to set the mixer gain for DAC left path
 * @param[in] gain The mixer gain for DAC left path is set. The range is -31 dB to 0 dB, by step of 1 dB
 */
void hal_codec_set_dac_gomixl(int8_t gain);
/** 
 * @brief This function is used to set the mixer gain for DAC right path
 * @param[in] gain The mixer gain for DAC right path is set. The range is -31 dB to 0 dB, by step of 1 dB
 */
void hal_codec_set_dac_gomixr(int8_t gain);

/** 
 * @brief 	This function is used to set the mixer gain for left input path
 * @param[in] gain The mixer gain for left input path is set. The range is -31 dB to 0 dB, by step of 1 dB
 */
void hal_codec_set_dac_gimixl(int8_t gain);

/** 
 * @brief 	This function is used to set the mixer gain for right input path
 * @param[in] gain The mixer gain for right input path is set. The range is -31 dB to 0 dB, by step of 1 dB
 */
void hal_codec_set_dac_gimixr(int8_t gain);

#if (CODEC_SPK_DRC_ENABLE)
/** 
 * @brief This function is used to enable or disable the DAC soft clipping DRC
 * @param[in] enable The operation is set.
 *  @arg true Enable DRC
 *  @arg falsen Disable DRC
 */
void hal_codec_enable_dac_drc(bool enable);

/** 
 * @brief This function is used to set the DRC compression threshold for left channel DAC
 * @param[in] threshold The DRC compression threshold is set, the range is [-31dB, 0dB] by step of 1 dB
 */
void hal_codec_set_dac_drc_lthres(int8_t threshold);

/** 
 * @brief This function is used to set the DRC compression threshold for right channel DAC
 * @param[in] threshold The DRC compression threshold is set, the range is [-31dB, 0dB] by step of 1 dB
 */
void hal_codec_set_dac_drc_rthres(int8_t threshold);

/** 
 * @brief This functions is used to set the DRC compression rate for left channel DAC
 * @param[in] rate The DRC compression rate is set. This parameter can only be one of the type #hal_codec_dac_drc_comp_t
 */
void hal_codec_set_dac_drc_lcomp(hal_codec_dac_drc_comp_t rate);

/** 
 * @brief This functions is used to set the DRC compression rate for right channel DAC
 * @param[in] rate The DRC compression rate is set. This parameter can only be one of the type #hal_codec_dac_drc_comp_t
 */
void hal_codec_set_dac_drc_rcomp(hal_codec_dac_drc_comp_t rate);
#endif

/** 
 * @brief This function is used to mute the headphone interface
 */
void hal_codec_mute_hp(void);

/** 
 * @brief This function is used to unmute the headphone interface
 */
void hal_codec_unmute_hp(void);

/** 
 * @brief 	This function is used to activate or power-down headphone
 * @param[in] active The operation is set.
 *  @arg ture Active headphone
 *	@arg false Power-down headphone
 */
void hal_codec_activate_hp(bool active);

/** 
 * @brief This function is used to select Headphone Output Amplifier input
 * @param[in] input TBD
 */
void hal_codec_select_hp_amp_input(uint8_t input);

/** 
 * @brief This function is used to set Left channel HP amplifier gain
 * @param[in] gain The amplifier gain is set. The range is [-19, 12] by step of 1 dB.
 */
void hal_codec_set_hp_gol(int8_t gain);

/** 
 * @brief This function is used to set Right channel HP amplifier gain
 * @param[in] gain The amplifier gain is set. The range is [-19, 12] by step of 1 dB.
 */
void hal_codec_set_hp_gor(int8_t gain);

/** 
 * @brief This function is used to set ADC12 audio interface to parallel mode
 */
void hal_codec_set_adc12_audioif_parallel(void);

/** 
 * @brief 	This function is used to set ADC12 audio interface (master/slaveness selection).
 * @param[in] mode The selection of ADC12 Audio Interface 
 *	@arg 0 Master mode
 *	@arg 1 Slave mode
 */
void hal_codec_set_adc12_master_slave_mode(hal_codec_ai_mode_t mode);

/** 
 * @brief This function is used to set ADC12 digital filters audio Data Word Length selection for DSP protocol transmission
 * @param[in] wordLength   
 *	@arg 0 16-bit word length data
 *	@arg 1 18-bit word length data
 *	@arg 2 20-bit word length data
 *	@arg 3 24-bit word length data (default)
 */
void hal_codec_set_adc12_data_word_length(hal_codec_adwl_t wordLength);

/** 
 * @brief This function is used to set the sampling rate of the ADC12 interface
 * @param[in] fs The sampling rate of ADC12 interface is set. This parameter can only be one of the type #hal_codec_fs_t
 */
void hal_codec_set_adc12_sample_rate(hal_codec_fs_t fs);

/** 
 * @brief 	This function is used to set ADC12 digital filters Wind Noise Filter mode
 * @param[in] mode The Wind Noise Filter mode is set.
 *	@arg 0 Wind Noise filter disabled.
 *	@arg 1 Enables Wind Noise Filter mode 1
 *	@arg 2 Enables Wind Noise Filter mode 2
 *	@arg 3 Enables Wind Noise Filter mode 3
 */
void hal_codec_set_adc12_wnf_mode(hal_codec_wnf_mode_t mode);

/** 
 * @brief This function is used to enable the High Pass Filter of the ADC12 interface
 * @param[in] enable The operation is set.    
 *  @arg true Enable HFP
 *  @arg false Disable HFP
 */
void hal_codec_enable_adc12_hpf(bool enable);

/** 
 * @brief	This function sets the validation of ADC12 Frequency Tuning Control Word
 * @note	In case DAC audio interface Frequency Tuning Control Word needs an update, set validation to be false, then call #hal_codec_tune_adc12_sample_rate(), then set validation to be true
 * @param[in]	valid       Validation of ADC12 Frequency Tuning Control Word is set
 * 	@arg true Validate ADC12 Frequency Tuning Control Word.
 *  @arg false Invalidate ADC12 Frequency Tuning Control Word.
 */
int hal_codec_tune_adc12_sample_rate_valid(bool valid);

/** 
 * @brief This function is used to fine tune the sampling rate of the master ADC12 interface
 * @note @sa hal_codec_tune_adc12_sample_rate_valid()
 * @param[in] adj The tuning control word is set. The equation is adj=128*Fmclk*(1/Fs - 1/Fs0), where Fs is the tuned sampling frequency, Fs0 is the normalized sampling frequency, Fmclk is MCLK frequency.
 */
int hal_codec_tune_adc12_sample_rate(uint16_t adj);

/** 
 * @brief	This function sets the activation of the ADC12 audio interface
 * @param[in]	active      ADC12 audio interface activation is set.
 * 	@arg true ADC12 audio interface active
 * 	@arg false ADC12 audio interface in power-down mode
 */
void hal_codec_activate_adc12_audio_interface(bool active);

/** 
 * @brief This function is used to set the clock of DMIC12
 * @param[in] ratio The DMIC clock ratio is set. This parameter can only be one of the type #hal_codec_dmic_clock_t
 */
void hal_codec_set_dmic12_clock(hal_codec_dmic_clock_t ratio);

/** 
 * @brief This function is used to activate DMIC1 or not
 * @param[in] active The operation is set.
 * 	@arg true Activate DMIC1
 *  @arg false Power down DMIC1
 */
void hal_codec_activate_dmic1(bool active);

/** 
 * @brief This function is used to activate DMIC2 or not
 * @param[in] active The operation is set.
 * 	@arg true Activate DMIC2
 *  @arg false Power down DMIC2
 */
void hal_codec_activate_dmic2(bool active);

/** 
 * @brief This function is used to select the data provided of the ADC1
 * @param[in] sel The data provided is selected. This parameter can only be one of the type #hal_codec_adc_data_provided_t
 *  @arg 0 Data provided from DMIC1,  
 *  @arg 1 Data provided from the average of DMIC1 and DIMC2, (DMIC1+DMIC2)/2
 */
void hal_codec_select_adc1_data_provided(hal_codec_adc_data_provided_t sel);

/** 
 * @brief This function is used to select the data provided of the ADC2
 * @param[in] sel The data provided is selected. This parameter can only be one of the type #hal_codec_adc_data_provided_t
 *  @arg 0 Data provided from DMIC2,  
 *  @arg 1 Data provided from the average of DMIC1 and DIMC2, (DMIC1+DMIC2)/2
 */
void hal_codec_select_adc2_data_provided(hal_codec_adc_data_provided_t sel);

/** 
 * @brief This function is used to set the analog microphone 1 input mode
 * @param[in] mode The analog microphone input mode is set. This parameter can only be one of the type #hal_codec_amic_input_mode_t.
 *	@arg 0 Single-ended input
 *	@arg 1 Differential input
 */
void hal_codec_set_amic1_input_mode(hal_codec_amic_input_mode_t mode);

/** 
 * @brief This function is used to set the analog microphone 2 input mode
 * @param[in] mode The analog microphone input mode is set. This parameter can only be one of the type #hal_codec_amic_input_mode_t.
 *	@arg 0 Single-ended input
 *	@arg 1 Differential input
 */
void hal_codec_set_amic2_input_mode(hal_codec_amic_input_mode_t mode);

/** 
 * @brief This function is used to set MICBIAS1 output state when SB_MICBIAS1 = 1
 * @param[in] state The MICBIAS1 output state is set. This parameter can only be one of the type #hal_codec_amic_bias_output_state_t
 *	@arg 0 MICBIAS1 output pulled down to 0
 *	@arg 1 MICBIAS1 output in high impedance state
 */
void hal_codec_set_amic1_bias_state_in_power_down(hal_codec_amic_bias_output_state_t state);

/** 
 * @brief This function is used to set MICBIAS2 output state when SB_MICBIAS2 = 1
 * @param[in] state The MICBIAS2 output state is set. This parameter can only be one of the type #hal_codec_amic_bias_output_state_t
 *	@arg 0 MICBIAS2 output pulled down to 0
 *	@arg 1 MICBIAS2 output in high impedance state
 */
void hal_codec_set_amic2_bias_state_in_power_down(hal_codec_amic_bias_output_state_t state);

void hal_codec_set_amic1_capcouple(bool cap);

void hal_codec_set_amic2_capcouple(bool cap);

/** 
 * @brief This function is used to activate or power down MICBIAS1 
 * @param[in] active The operation is set.
 *	@arg true Microphone biasing active to 2.5V
 *	@arg false Microphone biasing in power-down
 */
void hal_codec_activate_amic1_bias(bool active);

/** 
 * @brief This function is used to activate or power down MICBIAS2
 * @param[in] active The operation is set.
 *	@arg true Microphone biasing active to 2.5V
 *	@arg false Microphone biasing in power-down
 */
void hal_codec_activate_amic2_bias(bool active);

/** 
 * @brief This function is used to set analog microphone 1 boost stage gain(analog gain)
 * @param[in] gain The boost stage gain is set. This parameter can only be one of the type #hal_codec_gim_t
 */
void hal_codec_set_amic1_analog_gain(hal_codec_gim_t gain);

/** 
 * @brief This function is used to set analog microphone 1 boost stage gain(analog gain)
 * @param[in] gain The boost stage gain is set. This parameter can only be one of the type #hal_codec_gim_t
 */
void hal_codec_set_amic2_analog_gain(hal_codec_gim_t gain);

/** 
 * @brief This function is used to mute ADC12 audio interface
 */
void hal_codec_mute_adc12(void);


/** 
 * @brief This function is used to unmute ADC12 audio interface
 */
void hal_codec_unmute_adc12(void);

/** 
 * @brief This function is used to set ADC12 power mode.
 * @param[in] mode The power mode is set.
 *	@arg 0 Set ADC12 to normal mode
 *	@arg 1 Set ADC12 to low power mode
 * @return None
 */
void hal_codec_set_adc12_power_mode(hal_codec_adc12_power_mode_t mode);

/** 
 * @brief This function is used to enable or disable ADC1
 * @param[in] activate The operation is set.
 *  @arg true Enable ADC1
 *  @arg false Disable ADC1
 */
void hal_codec_activate_adc1(bool activate);

/** 
 * @brief This function is used to enable or disable ADC2
 * @param[in] activate The operation is set.
 *  @arg true Enable ADC2
 *  @arg false Disable ADC2
 */
void hal_codec_activate_adc2(bool activate);

/** 
 * @brief This function is used to set mixer channel 1 output selection on ADC path
 * @note The digital mixer is AIADC1_SEL, the destination data interface is DOL, the inputs are I_DOL and I_DOR
 * @param[in] sel The mixer channel 1 output selection is set
 *  @arg 0 Normal inputs, means that the input of DOL is I_DOL.
 *  @arg 1 Cross inputs, means that the input of DOL is I_DOR.
 *  @arg 2 Mixed inputs, means that the input of DOL is (I_DOL+I_DOR)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
void hal_codec_select_adc1_output(hal_codec_channel_sel_t sel);

/** 
 * @brief This function is used to set mixer channel 2 output selection on ADC path
 * @note The digital mixer is AIADC2_SEL, the destination data interface is DOR, the inputs are I_DOL and I_DOR
 * @param[in] sel The mixer channel 2 output selection is set. This parameter can only be one of the type #hal_codec_channel_sel_t
 *  @arg 0 Normal inputs, means that the input of DOR is I_DOR.
 *  @arg 1 Cross inputs, means that the input of DOR is I_DOL.
 *  @arg 2 Mixed inputs, means that the input of DOR is (I_DOL+I_DOR)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
void hal_codec_select_adc2_output(hal_codec_channel_sel_t input);

/** 
 * @brief This function is set mixer mode on ADC12 digital filters path
 * @param[in] mode The mixer mode on ADC12 digital filters path is set. This parameter can only be one of the type #hal_codec_adc_mixer_mode_t
 *	@arg 0 Record input only
 *	@arg 1 Record input + DAC
 */
void hal_codec_select_adc12_mixer_mode(hal_codec_adc_mixer_mode_t mode);


/** 
 * @brief This function is used to set the digital gain of ADC1
 * @param[in] gain The digital gain of ADC1 is set. The range is [-64, 63] by step of 1dB
 */
void hal_codec_set_adc1_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set the digital gain of ADC2
 * @param[in] gain The digital gain of ADC2 is set. The range is [-64, 63] by step of 1dB
 */
int hal_codec_set_adc2_digital_gain(int8_t gain);

/** 
 * @brief 	This function is used to enable/disable ADC12 AGC feature
 * @param[in] enable The operation is set. 
 *	@arg true Enable AGC
 *	@arg false Disable AGC
 */
void hal_codec_enable_adc12_agc(bool enable);

/** 
 * @brief This function is used to configure the AGC of ADC12
 * @param[in] agc The configuration information of the AGC is set.
 */
void hal_codec_config_adc12_agc(hal_codec_adc_agc_config_t *agc);

/** 
 * @brief This function is used to set ADC3456 audio interface to parallel mode
 */
void hal_codec_set_adc3456_audioif_parallel(void);

/** 
 * @brief 	This function is used to set ADC3456 audio interface (master/slaveness selection).
 * @param[in] mode The selection of ADC3456 Audio Interface 
 *	@arg 0 Master mode
 *	@arg 1 Slave mode
 */
void hal_codec_set_adc3456_master_slave_mode(hal_codec_ai_mode_t mode);

/** 
 * @brief This function is used to set ADC3456 digital filters audio Data Word Length selection for DSP protocol transmission
 * @param[in] wordLength   
 *	@arg 0 16-bit word length data
 *	@arg 1 18-bit word length data
 *	@arg 2 20-bit word length data
 *	@arg 3 24-bit word length data (default)
 */
void hal_codec_set_adc3456_data_word_length(hal_codec_adwl_t wordLength);

/** 
 * @brief This function is used to set the clock of DMIC3456
 * @param[in] ratio The DMIC clock ratio is set. This parameter can only be one of the type #hal_codec_dmic_clock_t
 */
void hal_codec_set_dmic3456_clock(hal_codec_dmic_clock_t ratio);

/** 
 * @brief This function is used to set the sampling rate of the ADC3456 interface
 * @param[in] fs The sampling rate of ADC3456 interface is set. This parameter can only be one of the type #hal_codec_fs_t
 */
int hal_codec_set_adc3456_sample_rate(hal_codec_fs_t fs);

/** 
 * @brief This function sets the validation of ADC3456 Frequency Tuning Control Word
 * @note In case DAC audio interface Frequency Tuning Control Word needs an update, set validation to be false, then call #hal_codec_tune_adc3456_sample_rate(), then set validation to be true
 * @param[in] valid Validation of ADC3456 Frequency Tuning Control Word is set
 * 	@arg true Validate ADC3456 Frequency Tuning Control Word.
 *  @arg false Invalidate ADC3456 Frequency Tuning Control Word.
 */
int hal_codec_tune_adc3456_sample_rate_valid(bool valid);

/** 
 * @brief This function is used to fine tune the sampling rate of the master ADC3456 interface
 * @note @sa hal_codec_tune_adc3456_sample_rate_valid()
 * @param[in] adj The tuning control word is set. The equation is adj=128*Fmclk*(1/Fs - 1/Fs0), where Fs is the tuned sampling frequency, Fs0 is the normalized sampling frequency, Fmclk is MCLK frequency.
 */
int hal_codec_tune_adc3456_sample_rate(uint16_t adj);


/** 
 * @brief This function sets the activation of the ADC34 audio interface
 * @param[in] active ADC34 audio interface activation is set.
 * 	@arg true ADC34 audio interface active
 * 	@arg false ADC34 audio interface in power-down mode
 */
void hal_codec_activate_adc34_audio_interface(bool active);

/** 
 * @brief 	This function is used to set ADC34 digital filters Wind Noise Filter mode
 * @param[in] mode The Wind Noise Filter mode is set.
 *	@arg 0 Wind Noise filter disabled.
 *	@arg 1 Enables Wind Noise Filter mode 1
 *	@arg 2 Enables Wind Noise Filter mode 2
 *	@arg 3 Enables Wind Noise Filter mode 3
 */
void hal_codec_set_adc34_wnf_mode(hal_codec_wnf_mode_t mode);

/** 
 * @brief This function is used to enable the High Pass Filter of the ADC34 interface
 * @param[in] enable The operation is set.    
 *  @arg true Enable HFP
 *  @arg false Disable HFP
 */
void hal_codec_enable_adc34_hpf(bool enable);

/** 
 * @brief This function is used to activate DMIC3 or not
 * @param[in] active The operation is set.
 * 	@arg true Activate DMIC3
 *  @arg false Power down DMIC3
 */
void hal_codec_activate_dmic3(bool active);

/** 
 * @brief This function is used to activate DMIC4 or not
 * @param[in] active The operation is set.
 * 	@arg true Activate DMIC4
 *  @arg false Power down DMIC4
 */
void hal_codec_activate_dmic4(bool active);

/** 
 * @brief This function is used to select the data provided of the ADC3
 * @param sel The data provided of ADC3 is selected. This parameter can only be one of the type #hal_codec_dmic_data_provided_t
 *  @arg 1 Data provided from DMIC3 self,  
 *  @arg 2 Data provided from the average of DMIC3 and DIMC4
 */
void hal_codec_select_adc3_data_provided(hal_codec_dmic_data_provided_t sel);

/** 
 * @brief This function is used to select the data provided of the ADC4
 * @param sel The data provided of ADC4 is selected. This parameter can only be one of the type #hal_codec_dmic_data_provided_t
 *  @arg 1 Data provided from DMIC4 self,  
 *  @arg 2 Data provided from the average of DMIC3 and DIMC4
 */
void hal_codec_select_adc4_data_provided(hal_codec_dmic_data_provided_t sel);

/** 
 * @brief This function is used to mute ADC34 audio interface
 */
void hal_codec_mute_adc34(void);

/** 
 * @brief This function is used to unmute ADC34 audio interface
 */
void hal_codec_unmute_adc34(void);

/** 
 * @brief This function is used to set mixer channel 3 output selection on ADC path
 * @note The digital mixer is AIADC3_SEL, the destination data interface is DO2L, the inputs are I_DO2L and I_DO2R
 * @param[in] sel The mixer channel 3 output selection is set
 *  @arg 0 Normal inputs, means that the input of DO2L is I_DO2L.
 *  @arg 1 Cross inputs, means that the input of DO2L is I_DO2R.
 *  @arg 2 Mixed inputs, means that the input of DO2L is (I_DO2L+I_DO2R)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
void hal_codec_select_adc3_output(hal_codec_channel_sel_t input);

/** 
 * @brief This function is used to set mixer channel 4 output selection on ADC path
 * @note The digital mixer is AIADC4_SEL, the destination data interface is DO2L, the inputs are I_DO2L and I_DO2R
 * @param[in] sel The mixer channel 4 output selection is set
 *  @arg 0 Normal inputs, means that the input of DO2R is I_DO2R.
 *  @arg 1 Cross inputs, means that the input of DO2R is I_DO2L.
 *  @arg 2 Mixed inputs, means that the input of DO2R is (I_DO2L+I_DO2R)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
void hal_codec_select_adc4_output(hal_codec_channel_sel_t input);

/** 
 * @brief This function is used to set the digital gain of ADC3
 * @param[in] gain The digital gain of ADC3 is set. The range is [-64, 63] by step of 1dB
 */
void hal_codec_set_adc3_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set the digital gain of ADC4
 * @param[in] gain The digital gain of ADC4 is set. The range is [-64, 63] by step of 1dB
 */
void hal_codec_set_adc4_digital_gain(int8_t gain);

/** 
 * @brief 	This function is used to enable/disable ADC34 AGC feature
 * @param[in] enable The operation is set. 
 *	@arg true Enable AGC
 *	@arg false Disable AGC
 */
void hal_codec_enable_adc34_agc(bool enable);

/** 
 * @brief This function is used to configure the AGC of ADC34
 * @param[in] agc The configuration information of the AGC is set.
 */
void hal_codec_config_adc34_agc(hal_codec_adc_agc_config_t *agc);

/** 
 * @brief This function sets the activation of the ADC56 audio interface
 * @param[in] active ADC56 audio interface activation is set.
 * 	@arg true ADC56 audio interface active
 * 	@arg false ADC56 audio interface in power-down mode
 */
void hal_codec_activate_adc56_audio_interface(bool active);

/** 
 * @brief 	This function is used to set ADC56 digital filters Wind Noise Filter mode
 * @param[in] mode The Wind Noise Filter mode is set.
 *	@arg 0 Wind Noise filter disabled.
 *	@arg 1 Enables Wind Noise Filter mode 1
 *	@arg 2 Enables Wind Noise Filter mode 2
 *	@arg 3 Enables Wind Noise Filter mode 3
 */
void hal_codec_set_adc56_wnf_mode(hal_codec_wnf_mode_t mode);

/** 
 * @brief This function is used to enable the High Pass Filter of the ADC56 interface
 * @param[in] enable The operation is set.    
 *  @arg true Enable HFP
 *  @arg false Disable HFP
 */
void hal_codec_enable_adc56_hpf(bool enable);

/** 
 * @brief This function is used to activate DMIC5 or not
 * @param[in] active The operation is set.
 * 	@arg true Activate DMIC5
 *  @arg false Power down DMIC5
 */
void hal_codec_activate_dmic5(bool active);

/** 
 * @brief This function is used to activate DMIC6 or not
 * @param[in] active The operation is set.
 * 	@arg true Activate DMIC6
 *  @arg false Power down DMIC6
 */
void hal_codec_activate_dmic6(bool active);

/** 
 * @brief This function is used to select the data provided of the ADC5
 * @param sel The data provided of ADC5 is selected. This parameter can only be one of the type #hal_codec_dmic_data_provided_t
 *  @arg 1 Data provided from DMIC5 self,  
 *  @arg 2 Data provided from the average of DMIC5 and DIMC6
 */
void hal_codec_select_adc5_data_provided(hal_codec_dmic_data_provided_t sel);

/** 
 * @brief This function is used to select the data provided of the ADC6
 * @param sel The data provided of ADC3 is selected. This parameter can only be one of the type #hal_codec_dmic_data_provided_t
 *  @arg 1 Data provided from DMIC6 self,  
 *  @arg 2 Data provided from the average of DMIC5 and DIMC6
 */
void hal_codec_select_adc6_data_provided(hal_codec_dmic_data_provided_t sel);

/** 
 * @brief This function is used to mute ADC34 audio interface
 */
void hal_codec_mute_adc56(void);

/** 
 * @brief This function is used to unmute ADC34 audio interface
 */
void hal_codec_unmute_adc56(void);

/** 
 * @brief This function is used to set mixer channel 5 output selection on ADC path
 * @note The digital mixer is AIADC5_SEL, the destination data interface is DO3L, the inputs are I_DO3L and I_DO3R
 * @param[in] sel The mixer channel 5 output selection is set
 *  @arg 0 Normal inputs, means that the input of DO3L is I_DO3L.
 *  @arg 1 Cross inputs, means that the input of DO3L is I_DO3R.
 *  @arg 2 Mixed inputs, means that the input of DO3L is (I_DO3L+I_DO3R)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
int hal_codec_select_adc5_output(hal_codec_channel_sel_t input);

/** 
 * @brief This function is used to set mixer channel 6 output selection on ADC path
 * @note The digital mixer is AIADC6_SEL, the destination data interface is DO3R, the inputs are I_DO3L and I_DO3R
 * @param[in] sel The mixer channel 6 output selection is set
 *  @arg 0 Normal inputs, means that the input of DO3R is I_DO3R.
 *  @arg 1 Cross inputs, means that the input of DO3R is I_DO3L.
 *  @arg 2 Mixed inputs, means that the input of DO3R is (I_DO3L+I_DO3R)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
int hal_codec_select_adc6_output(hal_codec_channel_sel_t input);


/** 
 * @brief This function is used to set the digital gain of ADC5
 * @param[in] gain The digital gain of ADC5 is set. The range is [-64, 63] by step of 1dB
 */
void hal_codec_set_adc5_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set the digital gain of ADC6
 * @param[in] gain The digital gain of ADC6 is set. The range is [-64, 63] by step of 1dB
 */
void hal_codec_set_adc6_digital_gain(int8_t gain);

/** 
 * @brief 	This function is used to enable/disable ADC56 AGC feature
 * @param[in] enable The operation is set. 
 *	@arg true Enable AGC
 *	@arg false Disable AGC
 */
void hal_codec_enable_adc56_agc(bool enable);

/** 
 * @brief This function is used to configure the AGC of ADC56
 * @param[in] agc The configuration information of the AGC is set.
 */
void hal_codec_config_adc56_agc(hal_codec_adc_agc_config_t *agc);

/** 
 * @brief 	This function is used to set ADC789a audio interface (master/slaveness selection).
 * @param[in] mode The selection of ADC789a Audio Interface 
 *	@arg 0 Master mode
 *	@arg 1 Slave mode
 */
void hal_codec_set_adc789a_master_slave_mode(hal_codec_ai_mode_t mode);

/** 
 * @brief This function is used to set ADC789a audio interface to parallel mode
 */
void hal_codec_set_adc789a_audioif_parallel(void);

/** 
 * @brief This function is used to set ADC789a digital filters audio Data Word Length selection for DSP protocol transmission
 * @param[in] wordLength   
 *	@arg 0 16-bit word length data
 *	@arg 1 18-bit word length data
 *	@arg 2 20-bit word length data
 *	@arg 3 24-bit word length data (default)
 */
void hal_codec_set_adc789a_data_word_length(hal_codec_adwl_t wordLength);

/** 
 * @brief This function is used to set the clock of DMIC789a
 * @param[in] ratio The DMIC clock ratio is set. This parameter can only be one of the type #hal_codec_dmic_clock_t
 */
void hal_codec_set_dmic789a_clock(hal_codec_dmic_clock_t ratio);

/** 
 * @brief This function is used to set the sampling rate of the ADC789a interface
 * @param[in] fs The sampling rate of ADC789a interface is set. This parameter can only be one of the type #hal_codec_fs_t
 */
int hal_codec_set_adc789a_sample_rate(hal_codec_fs_t fs);

/** 
 * @brief This function sets the validation of ADC3456 Frequency Tuning Control Word
 * @note In case DAC audio interface Frequency Tuning Control Word needs an update, set validation to be false, then call #hal_codec_tune_adc789a_sample_rate(), then set validation to be true
 * @param[in] valid Validation of ADC789a Frequency Tuning Control Word is set
 * 	@arg true Validate ADC789a Frequency Tuning Control Word.
 *  @arg false Invalidate ADC789a Frequency Tuning Control Word.
 */
int hal_codec_tune_adc789a_sample_rate_valid(bool valid);

/** 
 * @brief This function is used to fine tune the sampling rate of the master ADC789a interface
 * @note @sa hal_codec_tune_adc789a_sample_rate_valid()
 * @param[in] adj The tuning control word is set. The equation is adj=128*Fmclk*(1/Fs - 1/Fs0), where Fs is the tuned sampling frequency, Fs0 is the normalized sampling frequency, Fmclk is MCLK frequency.
 */
int hal_codec_tune_adc789a_sample_rate(uint16_t adj);


/** 
 * @brief This function sets the activation of the ADC78 audio interface
 * @param[in] active ADC78 audio interface activation is set.
 * 	@arg true ADC78 audio interface active
 * 	@arg false ADC78 audio interface in power-down mode
 */
void hal_codec_activate_adc78_audio_interface(bool active);

/** 
 * @brief 	This function is used to set ADC78 digital filters Wind Noise Filter mode
 * @param[in] mode The Wind Noise Filter mode is set.
 *	@arg 0 Wind Noise filter disabled.
 *	@arg 1 Enables Wind Noise Filter mode 1
 *	@arg 2 Enables Wind Noise Filter mode 2
 *	@arg 3 Enables Wind Noise Filter mode 3
 */
void hal_codec_set_adc78_wnf_mode(hal_codec_wnf_mode_t mode);

/** 
 * @brief This function is used to enable the High Pass Filter of the ADC78 interface
 * @param[in] enable The operation is set.    
 *  @arg true Enable HFP
 *  @arg false Disable HFP
 */
void hal_codec_enable_adc78_hpf(bool enable);

/** 
 * @brief This function is used to activate DMIC7 or not
 * @param[in] active The operation is set.
 * 	@arg true Activate DMIC7
 *  @arg false Power down DMIC7
 */
void hal_codec_activate_dmic7(bool active);

/** 
 * @brief This function is used to activate DMIC8 or not
 * @param[in] active The operation is set.
 * 	@arg true Activate DMIC8
 *  @arg false Power down DMIC8
 */
void hal_codec_activate_dmic8(bool active);

/** 
 * @brief This function is used to select the data provided of the ADC7
 * @param sel The data provided of ADC7 is selected. This parameter can only be one of the type #hal_codec_dmic_data_provided_t
 *  @arg 1 Data provided from DMIC7 self,  
 *  @arg 2 Data provided from the average of DMIC7 and DIMC8
 */
void hal_codec_select_adc7_data_provided(hal_codec_dmic_data_provided_t sel);

/** 
 * @brief This function is used to select the data provided of the ADC8
 * @param sel The data provided of ADC4 is selected. This parameter can only be one of the type #hal_codec_dmic_data_provided_t
 *  @arg 1 Data provided from DMIC8 self,  
 *  @arg 2 Data provided from the average of DMIC7 and DIMC8
 */
void hal_codec_select_adc8_data_provided(hal_codec_dmic_data_provided_t sel);

/** 
 * @brief This function is used to mute ADC78 audio interface
 */
void hal_codec_mute_adc78(void);

/** 
 * @brief This function is used to unmute ADC78 audio interface
 */
void hal_codec_unmute_adc78(void);

/** 
 * @brief This function is used to set mixer channel 7 output selection on ADC path
 * @note The digital mixer is AIADC3_SEL, the destination data interface is DO2L, the inputs are I_DO2L and I_DO2R
 * @param[in] sel The mixer channel 7 output selection is set
 *  @arg 0 Normal inputs, means that the input of DO2L is I_DO2L.
 *  @arg 1 Cross inputs, means that the input of DO2L is I_DO2R.
 *  @arg 2 Mixed inputs, means that the input of DO2L is (I_DO2L+I_DO2R)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
void hal_codec_select_adc7_output(hal_codec_channel_sel_t input);

/** 
 * @brief This function is used to set mixer channel 8 output selection on ADC path
 * @note The digital mixer is AIADC4_SEL, the destination data interface is DO2L, the inputs are I_DO2L and I_DO2R
 * @param[in] sel The mixer channel 8 output selection is set
 *  @arg 0 Normal inputs, means that the input of DO2R is I_DO2R.
 *  @arg 1 Cross inputs, means that the input of DO2R is I_DO2L.
 *  @arg 2 Mixed inputs, means that the input of DO2R is (I_DO2L+I_DO2R)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
void hal_codec_select_adc8_output(hal_codec_channel_sel_t input);

/** 
 * @brief This function is used to set the digital gain of ADC7
 * @param[in] gain The digital gain of ADC7 is set. The range is [-64, 63] by step of 1dB
 */
void hal_codec_set_adc7_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set the digital gain of ADC8
 * @param[in] gain The digital gain of ADC8 is set. The range is [-64, 63] by step of 1dB
 */
void hal_codec_set_adc8_digital_gain(int8_t gain);

/** 
 * @brief 	This function is used to enable/disable ADC78 AGC feature
 * @param[in] enable The operation is set. 
 *	@arg true Enable AGC
 *	@arg false Disable AGC
 */
void hal_codec_enable_adc78_agc(bool enable);

/** 
 * @brief This function is used to configure the AGC of ADC78
 * @param[in] agc The configuration information of the AGC is set.
 */
void hal_codec_config_adc78_agc(hal_codec_adc_agc_config_t *agc);

/** 
 * @brief This function sets the activation of the ADC9a audio interface
 * @param[in] active ADC9a audio interface activation is set.
 * 	@arg true ADC9a audio interface active
 * 	@arg false ADC9a audio interface in power-down mode
 */
void hal_codec_activate_adc9a_audio_interface(bool active);

/** 
 * @brief 	This function is used to set ADC9a digital filters Wind Noise Filter mode
 * @param[in] mode The Wind Noise Filter mode is set.
 *	@arg 0 Wind Noise filter disabled.
 *	@arg 1 Enables Wind Noise Filter mode 1
 *	@arg 2 Enables Wind Noise Filter mode 2
 *	@arg 3 Enables Wind Noise Filter mode 3
 */
void hal_codec_set_adc9a_wnf_mode(hal_codec_wnf_mode_t mode);

/** 
 * @brief This function is used to enable the High Pass Filter of the ADC9a interface
 * @param[in] enable The operation is set.    
 *  @arg true Enable HFP
 *  @arg false Disable HFP
 */
void hal_codec_enable_adc9a_hpf(bool enable);

/** 
 * @brief This function is used to activate DMIC9 or not
 * @param[in] active The operation is set.
 * 	@arg true Activate DMIC9
 *  @arg false Power down DMIC9
 */
void hal_codec_activate_dmic9(bool active);

/** 
 * @brief This function is used to activate DMIC10 or not
 * @param[in] active The operation is set.
 * 	@arg true Activate DMIC10
 *  @arg false Power down DMIC10
 */
void hal_codec_activate_dmic10(bool active);

/** 
 * @brief This function is used to select the data provided of the ADC9
 * @param sel The data provided of ADC9 is selected. This parameter can only be one of the type #hal_codec_dmic_data_provided_t
 *  @arg 1 Data provided from DMIC9 self,  
 *  @arg 2 Data provided from the average of DMIC9 and DIMC10
 */
void hal_codec_select_adc9_data_provided(hal_codec_dmic_data_provided_t sel);

/** 
 * @brief This function is used to select the data provided of the ADC10
 * @param sel The data provided of ADC9 is selected. This parameter can only be one of the type #hal_codec_dmic_data_provided_t
 *  @arg 1 Data provided from DMIC10 self,  
 *  @arg 2 Data provided from the average of DMIC9 and DIMC10
 */
void hal_codec_select_adc10_data_provided(hal_codec_dmic_data_provided_t sel);

/** 
 * @brief This function is used to mute ADC9a audio interface
 */
void hal_codec_mute_adc9a(void);

/** 
 * @brief This function is used to unmute ADC9a audio interface
 */
void hal_codec_unmute_adc9a(void);

/** 
 * @brief This function is used to set mixer channel 9 output selection on ADC path
 * @note The digital mixer is AIADC5_SEL, the destination data interface is DO3L, the inputs are I_DO3L and I_DO3R
 * @param[in] sel The mixer channel 9 output selection is set
 *  @arg 0 Normal inputs, means that the input of DO3L is I_DO3L.
 *  @arg 1 Cross inputs, means that the input of DO3L is I_DO3R.
 *  @arg 2 Mixed inputs, means that the input of DO3L is (I_DO3L+I_DO3R)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
int hal_codec_select_adc9_output(hal_codec_channel_sel_t input);

/** 
 * @brief This function is used to set mixer channel 10 output selection on ADC path
 * @note The digital mixer is AIADC6_SEL, the destination data interface is DO3R, the inputs are I_DO3L and I_DO3R
 * @param[in] sel The mixer channel 10 output selection is set
 *  @arg 0 Normal inputs, means that the input of DO3R is I_DO3R.
 *  @arg 1 Cross inputs, means that the input of DO3R is I_DO3L.
 *  @arg 2 Mixed inputs, means that the input of DO3R is (I_DO3L+I_DO3R)/2.
 *  @arg 3 0 inputs, means there is no inputs
 */
int hal_codec_select_adc10_output(hal_codec_channel_sel_t input);


/** 
 * @brief This function is used to set the digital gain of ADC9
 * @param[in] gain The digital gain of ADC9 is set. The range is [-64, 63] by step of 1dB
 */
void hal_codec_set_adc9_digital_gain(int8_t gain);

/** 
 * @brief This function is used to set the digital gain of ADC10
 * @param[in] gain The digital gain of ADC10 is set. The range is [-64, 63] by step of 1dB
 */
void hal_codec_set_adc10_digital_gain(int8_t gain);

/** 
 * @brief 	This function is used to enable/disable ADC9a AGC feature
 * @param[in] enable The operation is set. 
 *	@arg true Enable AGC
 *	@arg false Disable AGC
 */
void hal_codec_enable_adc9a_agc(bool enable);

/** 
 * @brief This function is used to configure the AGC of ADC9a
 * @param[in] agc The configuration information of the AGC is set.
 */
void hal_codec_config_adc9a_agc(hal_codec_adc_agc_config_t *agc);

#ifdef CONFIG_CODEC_ENABLE_WT
/** 
 * @brief This function is used to enable/disable the Wisper Trigger feature
 * @param[in] enable The operation is set.
 *  @arg true Enable Wisper Trigger
 *  @arg false Disable Wisper Trigger
 */
void hal_codec_enable_wt(bool enable);

/** 
 * @brief This function is used to set the codec state when Whisper Trigger feature is activated
 * @param[in] wt_only The operation is set.
 *	@arg true Codec is disabled when Whisper Trigger is activated
 *	@arg false Codec is not affected by Whisper Trigger
 */
void hal_codec_wt_only(bool wt_only);

/** 
 * @brief This function is used to restart Whisper Trigger after Whisper Trigger detection
 * @note After Whisper Trigger detection, can call this function to restart Whisper Trigger.
 */
void hal_codec_wt_restart(void);

/** 
 * @brief This function is used to set Whisper Triger Processed channel selection
 * @param[in] chan The Whisper Trigger channel is selected.
 *	@arg 0 Left channel
 *	@arg 1 Right channel
 *	@arg 2 Average between left channel and right channel.
 */
void hal_codec_select_wt_channel(hal_codec_wt_channel_sel_t chan);

/** 
 * @brief This function is used to enable or disable Whisper Trigger ZCD functionality
 * @param[in] enable The operation is set
 *	@arg false Disable ZCD functionality
 *	@arg true Enable ZCD functionality
 */
void hal_codec_enalbe_wt_zcd(bool enable);

/** 
 * @brief This function is used to set the digital microphone interface clock frequency selection, when Whisper Trigger is enabled and no voice is detected
 * @param[in] clock The digital microphone interface clock frequency is set. This parameter can only be one of the type #hal_codec_wt_dmic_clock_t
 *	@arg 0 3.0 to 3.25 MHz
 *	@arg 1 2.4 to 2.6 MHz
 *	@arg 2 1.0 to 1.08 MHz(default)
 *	@arg 3 0.75 to 0.8125 MHz (TBC)
 */
void hal_codec_set_wt_dmic_clock(hal_codec_wt_dmic_clock_t clock);

/** 
 * @brief This function is used to set Whipser Triger Power level sensitivity
 * @param[in] sens The power level sensitivity is set. The range of this parameter is [0, 31] by steps of 1dB.
 */
void hal_codec_set_wt_sensitivity(uint8_t sens);
#endif //CONFIG_CODEC_ENABLE_WT


/** 
 * @brief This function bypass the slave AIAS function, just used in test mode
 * @note This function should be removed
 */
void hal_codec_bypass_slave_aias(void);

/** 
 * @brief This function is used to disable the low frequency drift of the PGA
 */
void hal_codec_disable_pga_lf_drift(void);

/** 
 * @brief This function is used to write DAC audio parallel input data.
 * @param[in] dil DAC left channel audio parallel input data is writen.
 * @param[in] dir DAC right channel audio parallel input data is writen.
 */
void hal_codec_write_dac_data(int32_t dil, int32_t dir);

//#if (CONFIG_CODEC_ADC_DATA_MODE == CODEC_ADC_DATA_MODE_24BIT)
/** 
 * @brief This function is used to read ADC12 audio parallel output data.
 * @param[out] adc1_do ADC1 audio parallel output data is read.
 * @param[out] adc2_do ADC2 audio parallel output data is read.
 */
void hal_codec_read_adc12_data_24bit(int32_t *adc1_do, int32_t *adc2_do);

/** 
 * @brief This function is used to read ADC34 audio parallel output data.
 * @param[out] adc3_do ADC3 audio parallel output data is read.
 * @param[out] adc4_do ADC4 audio parallel output data is read.
 */
void hal_codec_read_adc34_data_24bit(int32_t *adc3_do, int32_t *adc4_do);

/** 
 * @brief This function is used to read ADC56 audio parallel output data.
 * @param[out] adc5_do ADC5 audio parallel output data is read.
 * @param[out] adc6_do ADC6 audio parallel output data is read.
 */
void hal_codec_read_adc56_data_24bit(int32_t *adc5_do, int32_t *adc6_do);

/** 
 * @brief This function is used to read ADC78 audio parallel output data.
 * @param[out] adc7_do ADC7 audio parallel output data is read.
 * @param[out] adc8_do ADC8 audio parallel output data is read.
 */
void hal_codec_read_adc78_data_24bit(int32_t *adc7_do, int32_t *adc8_do);

/** 
 * @brief This function is used to read ADC9a audio parallel output data.
 * @param[out] adc9_do ADC9 audio parallel output data is read.
 * @param[out] adc10_do ADC10 audio parallel output data is read.
 */
void hal_codec_read_adc9a_data_24bit(int32_t *adc9_do, int32_t *adc10_do);

//#else
/** 
 * @brief This function is used to read ADC12 audio parallel output data.
 * @param[out] adc12_do ADC12 audio parallel output data is read.
 */
void hal_codec_read_adc12_data_16bit(int32_t *adc12_do);

/** 
 * @brief This function is used to read ADC34 audio parallel output data.
 * @param[out] adc34_do ADC34 audio parallel output data is read.
 */
void hal_codec_read_adc34_data_16bit(int32_t *adc34_do);

/** 
 * @brief This function is used to read ADC56 audio parallel output data.
 * @param[out] adc56_do ADC56 audio parallel output data is read.
 */
void hal_codec_read_adc56_data_16bit(int32_t *adc56_do);

/** 
 * @brief This function is used to read ADC78 audio parallel output data.
 * @param[out] adc78_do ADC78 audio parallel output data is read.
 */
void hal_codec_read_adc78_data_16bit(int32_t *adc78_do);

/** 
 * @brief This function is used to read ADC9a audio parallel output data.
 * @param[out] adc9a_do ADC9a audio parallel output data is read.
 */
void hal_codec_read_adc9a_data_16bit(int32_t *adc9a_do);

//#endif

uint8_t hal_codec_get_dac_mute(void);
uint8_t hal_codec_get_adc12_mute(void);
uint8_t hal_codec_get_adc34_mute(void);
uint8_t hal_codec_get_adc56_mute(void);
uint8_t hal_codec_get_adc78_mute(void);
uint8_t hal_codec_get_adc9a_mute(void);

int8_t hal_codec_get_dac_godr(void);
int8_t hal_codec_get_dac_godl(void);;
int8_t hal_codec_get_hp_gol(void);
int8_t hal_codec_get_hp_gor(void);
int8_t hal_codec_get_amic1_analog_gain(void);
int8_t hal_codec_get_amic2_analog_gain(void);
int8_t hal_codec_get_adc1_digital_gain(void);
int8_t hal_codec_get_adc2_digital_gain(void);
int8_t hal_codec_get_adc3_digital_gain(void);
int8_t hal_codec_get_adc4_digital_gain(void);
int8_t hal_codec_get_adc5_digital_gain(void);
int8_t hal_codec_get_adc6_digital_gain(void);
int8_t hal_codec_get_adc7_digital_gain(void);
int8_t hal_codec_get_adc8_digital_gain(void);
int8_t hal_codec_get_adc9_digital_gain(void);
int8_t hal_codec_get_adc10_digital_gain(void);

void hal_codec_set_adc12_agc_stereo(uint8_t enable);
void hal_codec_set_adc34_agc_stereo(uint8_t enable);
void hal_codec_set_adc56_agc_stereo(uint8_t enable);
void hal_codec_set_adc12_agc_target(uint8_t target);		
void hal_codec_set_adc34_agc_target(uint8_t target);
void hal_codec_set_adc56_agc_target(uint8_t target);
void hal_codec_adc12_agc_noise_gate_enable(uint8_t enable);
void hal_codec_adc34_agc_noise_gate_enable(uint8_t enable);
void hal_codec_adc56_agc_noise_gate_enable(uint8_t enable);
void hal_codec_set_adc12_agc_noise_gate_thres(uint8_t threshold);
void hal_codec_set_adc34_agc_noise_gate_thres(uint8_t threshold);
void hal_codec_set_adc56_agc_noise_gate_thres(uint8_t threshold);
void hal_codec_adc12_agc_snr_optimzation(uint8_t enable);
void hal_codec_set_adc12_agc_hold_time(uint8_t holdTime);
void hal_codec_set_adc34_agc_hold_time(uint8_t holdTime);
void hal_codec_set_adc56_agc_hold_time(uint8_t holdTime);
void hal_codec_set_adc12_agc_attack_time(uint8_t attackTime);
void hal_codec_set_adc34_agc_attack_time(uint8_t attackTime);
void hal_codec_set_adc56_agc_attack_time(uint8_t attackTime);
void hal_codec_set_adc12_agc_decay_time(uint8_t decayTime);
void hal_codec_set_adc34_agc_decay_time(uint8_t decayTime);
void hal_codec_set_adc56_agc_decay_time(uint8_t decayTime);
void hal_codec_set_adc12_agc_min_gain(uint8_t gain);
void hal_codec_set_adc34_agc_min_gain(uint8_t gain);
void hal_codec_set_adc56_agc_min_gain(uint8_t gain);
void hal_codec_set_adc12_agc_max_gain(uint8_t gain);
void hal_codec_set_adc34_agc_max_gain(uint8_t gain);
void hal_codec_set_adc56_agc_max_gain(uint8_t gain);
void hal_codec_enable_dac_drc(bool enable);
void hal_codec_set_dac_drc_lthres(int8_t threshold);
void hal_codec_set_dac_drc_rthres(int8_t threshold);
void hal_codec_set_dac_drc_lcomp(hal_codec_dac_drc_comp_t rate);
void hal_codec_set_dac_drc_rcomp(hal_codec_dac_drc_comp_t rate);

void hal_codec_select_adc34_mixer_mode(hal_codec_adc_mixer_mode_t mode);
void hal_codec_select_adc56_mixer_mode(hal_codec_adc_mixer_mode_t mode);

#ifdef	__cplusplus
}
#endif

#endif /* __HAL_CODEC_H__ */
