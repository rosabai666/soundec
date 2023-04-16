<<<<<<< HEAD
/* 	CONFIDENTIAL
*********************************************************************************

	File:			dsp_control.h
	Version:		1.00.00
	Description:	DSP Control Processing Header File

	Copyright(c) 2012 Yamaha Corporation

*********************************************************************************/

/* proto type declaration */
void Mute(void);
void MuteRelease(void);
void SetVolume(UC bVolID);

void SetBalance(signed char bBalID);

void SetCrossover(UC bNetworkID, UC *pbData);

void SetS3D(UC *pbData);
void S3DOn(bool bOn);

void SetHXT(UC *pbData);
void HXTOn(bool bOn);

void SetAEQ(UC *pbData0, UC *pbData1);
void AEQOn(bool bOn);

void PresetLoudnessSetting(UC bEffectID);
void UserLoudnessSetting(UC *pbData);
void LoudnessOn(bool bOn);

void SetPowerLimit(UC *pbData);
void PowerLimitOn(bool bOn);

void SetSoftClip(UC *pbData);
void SoftClipOn(bool bOn);

void SmallSignalDetectionOn(UC *pbData);
void SmallSignalDetectionOff(void);

void PlayFM(UC bContentID);

/* end of file */
=======
/* 	CONFIDENTIAL
*********************************************************************************

	File:			dsp_control.h
	Version:		1.00.00
	Description:	DSP Control Processing Header File

	Copyright(c) 2012 Yamaha Corporation

*********************************************************************************/

/* proto type declaration */
void Mute(void);
void MuteRelease(void);
void SetVolume(UC bVolID);

void SetBalance(signed char bBalID);

void SetCrossover(UC bNetworkID, UC *pbData);

void SetS3D(UC *pbData);
void S3DOn(bool bOn);

void SetHXT(UC *pbData);
void HXTOn(bool bOn);

void SetAEQ(UC *pbData0, UC *pbData1);
void AEQOn(bool bOn);

void PresetLoudnessSetting(UC bEffectID);
void UserLoudnessSetting(UC *pbData);
void LoudnessOn(bool bOn);

void SetPowerLimit(UC *pbData);
void PowerLimitOn(bool bOn);

void SetSoftClip(UC *pbData);
void SoftClipOn(bool bOn);

void SmallSignalDetectionOn(UC *pbData);
void SmallSignalDetectionOff(void);

void PlayFM(UC bContentID);

/* end of file */
>>>>>>> origin
