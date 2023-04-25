/** 
***********************************************************
 * 
 * @file name	: hal_i2s.h
 * @author		: zhangkun
 * @version		: V01.01
 * @date		: 2022-02-12
 * @brief		: I2S HAL MODULE
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
 * and Soundec Co. Ltd. (the "License"). You may not use this file except
 * in compliance with the License.
 * 
***********************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_I2S_H__
#define __HAL_I2S_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "types.h"
#include "snc8600.h"
#include "hal.h"
#include "hal_interrupt.h"
#include "hal_dmac.h"
//#include "common.h"

#if 1//(I2S_ENABLE == ENABLE)

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/*
32bit word size support  8K-96K
16/24bit word size support 8K-192K (193.5K)
*/

/** I2S1 - Register Layout Typedef        offset and reset value **/  


typedef struct
{
	/*Transmission and reception  channels 0*/
	__IO uint32_t I2S_LRBTHR;	    /*0x20:0x0:Left Receive Buffer 0 /Left Transmit Holding Register 0*/
	__IO uint32_t I2S_RRBTHR;	    /*0x24:0x0:Right Receive Buffer 0 /Right Transmit Holding Register 0*/
	__IO uint32_t I2S_RER;		    /*0x28:0x1:Receive channel 0 enable*/	
	__IO uint32_t I2S_TER;		    /*0x2c:0x1:Transmit channel 0 enable*/	
	__IO uint32_t I2S_RCR;		    /*0x30:0x4:Receive channel 0  word_length configuration*/
	__IO uint32_t I2S_TCR;		    /*0x34:0x5:Transmit channel 0 word_length configuration*/
	__I  uint32_t I2S_ISR;		    /*0x38:0x0:Receive/Transmit channel 0 interrupt status*/
	__IO uint32_t I2S_IMR;		    /*0x3c:0x33:Receive / transmit channel 0 interrupt mask*/ 	
	__I  uint32_t I2S_ROR;		    /*0x40:0x0:Receive channel 0 FIFO overload interruptRead register to clear interrupt status*/ 	
	__I  uint32_t I2S_TOR;		    /*0x44:0x0:Transmit channel 0 FIFO overload interruptRead register to clear interrupt status*/ 	
	__IO uint32_t I2S_RFCR;	        /*0x48:0x3:Receive channel 0 FIFO configuration*/ 	
	__IO uint32_t I2S_TFCR;		    /*0x4c:0x0:Transmit channel 0 FIFO configuration*/
	__O  uint32_t I2S_RFF;		    /*0x50:0x0:Receive channel 0 FIFO reset*/ 	
	__O  uint32_t I2S_TFF;	        /*0x54:0x0:Transmit channel 0 FIFO reset*/
}i2s_ch_t;

typedef struct
{
	__IO uint32_t I2S_IER;			/*0x00:0x0:I2S enable, I2S enable*/
	__IO uint32_t I2S_IRER;		    /*0x04:0x0:Receiver block enable*/	
	__IO uint32_t I2S_ITER;		    /*0x08:0x0:Transmitter block enable and Fast mode enable*/	
	__IO uint32_t I2S_CER;			/*0x0C:0x1:Clock generate enable*/    	
	__IO uint32_t I2S_CCR;			/*0x10:0xAC4414:Master mode clock configuration*/    	
	__O  uint32_t I2S_RXFFR;		/*0x14:0x0:Receiver block reset*/	
	__O  uint32_t I2S_TXFFR;	    /*0x18:0x0:Transmitter block reset*/
	uint32_t     RESERVED0; 	    /*RESERVED*/

	/*Transmission and reception  channels 0*/
	i2s_ch_t ch0;
	uint32_t     RESERVED1[2];	    

	/*Transmission and reception  channels 1*/
	i2s_ch_t ch1;
	uint32_t     RESERVED2[2];	    /**/
	
	/*Transmission and reception  channels 2*/
	i2s_ch_t ch2;
	uint32_t     RESERVED3[2];

	/*Transmission and reception  channels 3*/
	i2s_ch_t ch3;

	__IO uint32_t I2S_SCR;		     /*0x118:0x22220:0xSoftware configuration SCLK signal*/   	
	__IO uint32_t I2S_MFR;		     /*0x11c:0x1:I2S clock selection register 0:48mhz 1:12mhz*/
	__IO uint32_t I2S_CR;		     /*0x120:0x0:I2S format selection 0: Philips I2S format 1: MSB format 3: LSB format*/	
}i2s_reg_t;

typedef struct
{
	__I  uint32_t I2S_RXDMA;			/*0x1C0:0x0:Receiver Block DMA*/ 	
	__O  uint32_t I2S_RRXDMA;			/*0x1C4:0x0:reset Receiver Block DMA*/		
	__I  uint32_t I2S_TXDMA;			/*0x1C8:0x0:Transmitter Block DMA*/		
	__O  uint32_t I2S_RTXDMA;           /*0x1CC:0x0:reset Transmitter Block DMA*/
}i2s_dma_t;

typedef struct{	
	__I  uint32_t I2S_COMP_PARAM_2;		/*0x1f0:0x48c:Parameter 2*/	
	__I  uint32_t I2S_COMP_PARAM_1;		/*0x1f4:0x924067E:Parameter 2*/	
	__I  uint32_t I2S_COMP_VERSION;		/*0x1f8:0x3130332A:Component Version*/	
	__I  uint32_t I2S_COMP_TYPE;        /*0x1fc:0x445701A0:Component Type*/
}i2s_comp_t;

/** 
 * @defgroup  I2S_Mode I2S Mode
 */
#define I2S_MODE_MASTER 	(1U)
#define I2S_MODE_SLAVE		(0U)
/** 
 * @}
 */

/** 
 * @defgroup  I2S_Standard I2S Standard
 */
#define I2S_STANDARD_PHILIPS  	   (0U)
#define I2S_STANDARD_MSB 	  	   (1U)
#define I2S_STANDARD_LSB 	  	   (3U)


/** 
 * @defgroup  FIFO_Data_Length FIFO Data Length
 */
#define I2S_FIFO_DATALENGTH_12B   (1U) 
#define I2S_FIFO_DATALENGTH_16B   (2U)  
#define I2S_FIFO_DATALENGTH_20B   (3U)  
#define I2S_FIFO_DATALENGTH_24B   (4U) 
#define I2S_FIFO_DATALENGTH_32B   (5U) 
/** 
 * @}
 */


/** 
 * @defgroup  Frame_Length Frame Length
 */ 
#define I2S_FRAMELENGTH_16B   (0U)   
#define I2S_FRAMELENGTH_24B   (1U) 
#define I2S_FRAMELENGTH_32B   (2U) 
/** 
 * @}
 */

/** 
 * @defgroup  Frame_Length Frame Length
 */ 
#define I2S_FIFO_LEVEL0    (0U) 
#define I2S_FIFO_LEVEL1    (1U)   
#define I2S_FIFO_LEVEL2    (2U)   
#define I2S_FIFO_LEVEL3    (3U) 
#define I2S_FIFO_LEVEL4    (4U) 
#define I2S_FIFO_LEVEL5    (5U) 
#define I2S_FIFO_LEVEL6    (6U) 
#define I2S_FIFO_LEVEL7    (7U) 
#define I2S_FIFO_LEVEL8    (8U) 
#define I2S_FIFO_LEVEL9    (9U) 
#define I2S_FIFO_LEVEL10   (10U) 
#define I2S_FIFO_LEVEL11   (11U) 
#define I2S_FIFO_LEVEL12   (12U) 
#define I2S_FIFO_LEVEL13   (13U) 
#define I2S_FIFO_LEVEL14   (14U) 
#define I2S_FIFO_LEVEL15   (15U) 
/** 
 * @}
 */

/** 
 * @defgroup  Frame_Length Frame Length
 */ 
#define	I2S_INDEX_1 	0
#define	I2S_INDEX_2 	1
#define	I2S_INDEX_3 	2
#define	I2S_INDEX_MAX	3
/** 
 * @}
 */

typedef struct
{
	uint32_t	audioFreq;    			/*!< I2S  frequency>*/
    uint8_t 	mode;                  	/*!< Master/slave configuration>*/
    uint8_t 	standard;               /*!< I2S standard>*/           
	uint8_t     txDataLength;   		/*!< Data length>*/
    uint8_t     rxDataLength;           /*!< Data length>*/
    uint8_t     frameLength;            /*!< Frame width>*/
	uint8_t 	txFifolevel;            /*!< I2S FIFO LEVEL: 0-15>*/
	uint8_t 	rxFifoLevel;            /*!< I2S FIFO LEVEL: 0-15>*/
	uint8_t     txEnable;				/*!< I2S TX ENABLE>*/
	uint8_t     rxEnable;				/*!< I2S TX DISABLE>*/
}i2s_cfg_t;

typedef struct 
{
  i2s_reg_t	*instance;        /*!< Register base address*/
  i2s_dma_t *Dma;
  i2s_cfg_t configs;
}i2s_handle_t;

#define HAL_I2S_ASSERT_INSTANCE(i2sInatance)	(i2sInatance == I2S1 || i2sInatance == I2S2 || i2sInatance == I2S3)
//#define HAL_I2C_DISABL


/* Private macro -------------------------------------------------------------*/
#define HAL_I2S_CH_REG_SIZE             0x40
#define HAL_I2S_CH_REG_BASE             0x20

/* I2S - Peripheral instance base addresses */

/** Peripheral I2S1 base address */
#define		I2S1_BASE		 0x77000000
/** Peripheral I2S1 DMA1 base address */
#define     I2S1_DMA1_BASE   0x770001C0
#define     I2S1_COMP_BASE   0x770001F0
/** Peripheral I2S1 base pointer */
#define I2S1                  ((i2s_reg_t *)I2S1_BASE)
#define I2S1_DMA              ((i2s_dma_t *)I2S1_DMA1_BASE)
#define I2S1_COMP             ((i2s_comp_t *)I2S1_COMP_BASE)


/** Peripheral I2S2 base address */
#define     I2S2_BASE        0x77001000
/** Peripheral I2S2 DMA2 base address */
#define     I2S2_DMA2_BASE   0x770011C0
#define     I2S2_COMP_BASE   0x770011F0
/** Peripheral I2S2 base pointer */
#define I2S2                    ((i2s_reg_t *)I2S2_BASE)
#define I2S2_DMA                ((i2s_dma_t *)I2S2_DMA2_BASE)
#define I2S2_COMP               ((i2s_comp_t *)I2S2_COMP_BASE)


/** Peripheral I2S3 base address */
#define     I2S3_BASE        0x77002000
/** Peripheral I2S3 DMA3 base address */
#define     I2S3_DMA3_BASE   0x770021C0
#define     I2S3_COMP_BASE   0x770021F0
/** Peripheral I2S3 base pointer */
#define I2S3                    ((i2s_reg_t *)I2S3_BASE)
#define I2S3_DMA                ((i2s_dma_t *)I2S3_DMA3_BASE)
#define I2S3_COMP               ((i2s_comp_t *)I2S3_COMP_BASE)

/** Array initializer of I2S peripheral base addresses */
//#define I2S_BASE_ADDRS                           { I2S0_BASE, I2S1_BASE ,I2S1_BASE}
/** Array initializer of I2S peripheral base pointers */
//#define I2S_BASE_PTRS                            { I2S0, I2S1, I2S2}

/* IER: i2s enable ; 1: enable DW_apb_i2s/ 0: disable DW_apb_i2s */

#define I2S_IER_MASK                     (0x1) 
#define I2S_IER_SHIFT                    (0U)
#define I2S_IER_EN(x)                       (((uint32_t)(((uint32_t)(x))<<I2S_IER_SHIFT))&I2S_IER_MASK)

/*IRER(Receiver Block Enable Register): Receiver Block enable; 1: enable receiver / 0: disable receiver*/
#define I2S_IRER_RECEIVE_MASK            (0x1) 
#define I2S_IRER_RECEIVE_SHIFT           (0U)
#define I2S_IRER_RECEIVE(x)              (((uint32_t)(((uint32_t)(x))<<I2S_IRER_RECEIVE_SHIFT))&I2S_IRER_RECEIVE_MASK)
                 

/*ITER(Transmitter Block and fastmode Enable Register): Transmitter Block enable; 1: enable transmitter / 0: disable transmitter*/
#define I2S_ITER_FASTMODE_MASK           (0x2) 
#define I2S_ITER_FASTMODE_SHIFT          (1U)
#define I2S_ITER_FASTMODE(x)             (((uint32_t)(((uint32_t)(x))<<I2S_ITER_FASTMODE_SHIFT))&I2S_ITER_FASTMODE_MASK)

#define I2S_ITER_TRANSMITTER_MASK        (0x1) 
#define I2S_ITER_TRANSMITTER_SHIFT       (0U)
#define I2S_ITER_TRANSMITTER(x)          (((uint32_t)(((uint32_t)(x))<<I2S_ITER_TRANSMITTER_SHIFT))&I2S_ITER_TRANSMITTER_MASK)

/*CER: Clock Enable Register; 1: enable  master, 0: disable*/
#define I2S_CER_CLOCK_MASK                (0x1) 
#define I2S_CER_CLOCK_SHIFT               (0U)
#define I2S_CER_CLOCK(x)                  (((uint32_t)(((uint32_t)(x))<<I2S_CER_CLOCK_SHIFT))&I2S_CER_CLOCK_MASK)

/*CCR : Clock Configuration Register ;*/  

/*Frequency of WS clock*/
#define I2S_CRR_WS_FREQ_MASK              (0x3FFFF00)
#define I2S_CRR_WS_FREQ_SHIFT             (8U)
#define I2S_CRR_WS_FREQ(x)                (((uint32_t)(((uint32_t)(x))<<I2S_CRR_WS_FREQ_SHIFT))&I2S_CRR_WS_FREQ_MASK)
/*clk_word_size*/
#define I2S_CRR_WORD_SIZE_MASK            (0x18)
#define I2S_CRR_WORD_SIZE_SHIFT           (3U)
#define I2S_CRR_WORD_SIZE(x)              (((uint32_t)(((uint32_t)(x))<<I2S_CRR_WORD_SIZE_SHIFT))&I2S_CRR_WORD_SIZE_MASK)
/*sclk_gate*/
#define I2S_CRR_SCLK_GATE_MASK             (0x7)
#define I2S_CRR_SCLK_GATE_SHIFT            (0U)
#define I2S_CRR_SCLK_GATE(x)               (((uint32_t)(((uint32_t)(x))<<I2S_CRR_SCLK_GATE_SHIFT))&I2S_CRR_SCLK_GATE_MASK)

/*RXFFR : Receiver Block FIFO Reset Register*/
/*1:Reset 0:  NOT  Reset*/
#define I2S_RXFFR_RESET_MASK               (0x1)
#define I2S_RXFFR_RESET_SHIFT              (0U)
#define I2S_RXFFR_RESET(x)                 (((uint32_t)(((uint32_t)(x))<<I2S_RXFFR_RESET_SHIFT))&I2S_RXFFR_RESET_MASK)

/*TXFFR:Transmitter Block FIFO Reset Register*/
/*1:Reset 0:  NOT  Reset*/
#define I2S_TXFFR_RESET_MASK               (0x1)
#define I2S_TXFFR_RESET_SHIFT              (0U)
#define I2S_TXFFR_RESET(x)                 (((uint32_t)(((uint32_t)(x))<<I2S_TXFFR_RESET_SHIFT))&I2S_TXFFR_RESET_MASK)


/*LRBR0/LRBR1/LRBR2/LRBR3(Left Receive Buffer Register):Left channel receive buffer*/
#define I2S_LRBRn_MASK                    (0x3)
#define I2S_LRBRn_SHIFT                   (01)
#define I2S_LRBRn(x)                     (((uint32_t)(((uint32_t)(x))<<I2S_LRBRn_SHIFT))&I2S_LRBRn_MASK)

/*LTHR0/LTHR1/LTHR2/LTHR3(Left Transmit Holding Register) : Left channel send buffer*/
#define I2S_LTHRn_MASK                    (0x3)
#define I2S_LTHRn_SHIFT                   (1U)
#define I2S_LTHRn(x)                     (((uint32_t)(((uint32_t)(x))<<I2S_LTHRn_SHIFT))&I2S_LTHRn_MASK)


/*RRBR0/RRBR1/RRBR2/RRBR3(Right Receive Buffer Register):Right channel receive buffer*/
#define I2S_RRBRn_MASK                    (0x3)
#define I2S_RRBRn_SHIFT                   (1U)
#define I2S_RRBRn(x)                      (((uint32_t)(((uint32_t)(x))<<I2S_RRBRn_SHIFT))&I2S_RRBRn_MASK)

/*RTHR0/RTHR1/RTHR2/RTHR3(Right Transmit Holding Register):Right channel send buffer*/
#define I2S_RTHRn_MASK                    (0x3)
#define I2S_RTHRn_SHIFT                   (1U)
#define I2S_RTHRn(x)                      (((uint32_t)(((uint32_t)(x))<<I2S_RTHRn_SHIFT))&I2S_RTHRn_MASK)

/*RER0/RER1/RER2/RER3(Receive Enable Register)*/
#define I2S_RERn_RXCHEN_MASK              (0x1)
#define I2S_RERn_RXCHEN_SHIFT             (0U)
#define I2S_RERn_RXCHEN(x)                (((uint32_t)(((uint32_t)(x))<<I2S_RERn_RXCHEN_SHIFT))&I2S_RERn_RXCHEN_MASK)

/*TER0/TER1/TER2/TER3(Transmit Enable Register)*/
/*fast mode enable,1== enable , 0 == disable*/
#define I2S_TERn_FASEMODE_MASK            (0x2)
#define I2S_TERn_FASTMODE_SHIFT           (1U)
#define I2S_TERn_FAST_MODE_RESET(x)       (((uint32_t)(((uint32_t)(x))<<I2S_TERn_FASTMODE_SHIFT))&I2S_TERn_FASEMODE_MASK)
/*Transmit channel_n ,1 == enable ,0 == disable*/
#define I2S_TERn_TXCHEN_MASK              (0x1)
#define I2S_TERn_TXCHEN_SHIFT             (0U)
#define I2S_TERn_TXCHEN(x)                 (((uint32_t)(((uint32_t)(x))<<I2S_TERn_TXCHEN_SHIFT))&I2S_TERn_TXCHEN_MASK)

/*RCR0/RCR1/RCR2/RCR3(Receive Configuration Register)*/
/*
WORD_LENGTH
000 = Ignore word length
001 = 12 bit resolution
010 = 16 bit resolution
011 = 20 bit resolution
100 = 24 bit resolution
101 = 32 bit resolution
*/
#define I2S_RCRn_MASK                     (0x7)
#define I2S_RCRn_SHIFT                    (0U)
#define I2S_RCRn(x)                       (((uint32_t)(((uint32_t)(x))<<I2S_RCRn_SHIFT))&I2S_RCRn_MASK)

/*TCR0/TCR1/TCR2/TCR3(Transmit Configuration Register)*/
/*
WORD_LENGTH
000 = Ignore word length
001 = 12 bit resolution
010 = 16 bit resolution
011 = 20 bit resolution
100 = 24 bit resolution
101 = 32 bit resolution
*/
#define I2S_TCRn_MASK                     (0x7)
#define I2S_TCRn_SHIFT                    (0U)
#define I2S_TCRn(x)                       (((uint32_t)(((uint32_t)(x))<<I2S_TCRn_SHIFT))&I2S_TCRn_MASK)

/******ISR0/ISR1/ISR2/ISR3(Interrupt Status Register)******/

/*Transmit channel FIFO overload interrupt; 0: TX FIFO write valid , 1: TX FIFO write overrun .*/
#define I2S_ISRn_TXFO_MASK                (0x1)
#define I2S_ISRn_TXFO_SHIFT               (5U)
#define I2S_ISRn_TXFO(x)                  (((uint32_t)(((uint32_t)(x))<<I2S_ISRn_TXFO_SHIFT))&I2S_ISRn_TXFO_MASK)
#define I2S_ISRn_TXFO_POS                  (I2S_ISRn_TXFO_MASK << I2S_ISRn_TXFO_SHIFT )


/*Transmit channel FIFO null interrupt; 1: trigger level reached , 0: trigger level not reached */
#define I2S_ISRn_TXFE_MASK                (0x1)
#define I2S_ISRn_TXFE_SHIFT               (4U)
#define I2S_ISRn_TXFE(x)                  (((uint32_t)(((uint32_t)(x))<<I2S_ISRn_TXFE_SHIFT))&I2S_ISRn_TXFE_MASK)
#define I2S_ISRn_TXFE_POS                 (I2S_ISRn_TXFE_MASK << I2S_ISRn_TXFE_SHIFT )

/*Receive channel FIFO overload interrupt; 0: RX FIFO write valid , 1: RX FIFO write overrun */
#define I2S_ISRn_RXFO_MASK                (0x1)
#define I2S_ISRn_RXFO_SHIFT               (1U)
#define I2S_ISRn_RXFO(x)                  (((uint32_t)(((uint32_t)(x))<<I2S_ISRn_RXFO_SHIFT))&I2S_ISRn_RXFO_MASK)
#define I2S_ISRn_RXFO_POS                  (I2S_ISRn_RXFO_MASK << I2S_ISRn_RXFO_SHIFT )

/*Receive channel FIFO full interrupt; 1: trigger level reached , 0: trigger level not reached*/
#define I2S_ISRn_RXFA_MASK                (0x1)
#define I2S_ISRn_RXFA_SHIFT               (0U)
#define I2S_ISRn_RXFA(x)                 (((uint32_t)(((uint32_t)(x))<<I2S_ISRn_RXFA_SHIFT))&I2S_ISRn_RXFA_MASK)
#define I2S_ISRn_RXFA_POS                 (I2S_ISRn_RXFA_MASK << I2S_ISRn_RXFA_SHIFT)
/******ISR0/ISR1/ISR2/ISR3(Interrupt Status Register)******/

/******IMR0/IMR1/IMR2/IMR3(Interrupt Mask Register)******/
/*Shielding transmission channel FIFO overload interrupt ; 1: masks interrupt , 0: unmasks interrupt */
#define I2S_IMRn_TXFOM_MASK                (0x20)
#define I2S_IMRn_TXFOM_SHIFT               (5U)
#define I2S_IMRn_TXFOM(x)                  (((uint32_t)(((uint32_t)(x))<<I2S_IMRn_TXFOM_SHIFT))&I2S_IMRn_TXFOM_MASK)

/*Mask transmit channel FIFO null interrupt; 1: masks interrupt ,0: unmasks interrupt */
#define I2S_IMRn_TXFEM_MASK                (0x10)
#define I2S_IMRn_TXFEM_SHIFT               (4U)
#define I2S_IMRn_TXFEM(x)                  (((uint32_t)(((uint32_t)(x))<<I2S_IMRn_TXFEM_SHIFT))&I2S_IMRn_TXFEM_MASK)

/*Mask receive channel FIFO overload interrupt;1: masks interrupt ,0: unmasks interrupt */
#define I2S_IMRn_RXFOM_MASK                (0x2)
#define I2S_IMRn_RXFOM_SHIFT               (1U)
#define I2S_IMRn_RXFOM(x)                  (((uint32_t)(((uint32_t)(x))<<I2S_IMRn_RXFOM_SHIFT))&I2S_IMRn_RXFOM_MASK)

/*Mask receive channel FIFO full interrupt;1: masks interrupt ,0: unmasks interrupt */
#define I2S_IMRn_RXDAM_MASK                (0x1)
#define I2S_IMRn_RXDAM_SHIFT               (0U)
#define I2S_IMRn_RXDAM(x)                  (((uint32_t)(((uint32_t)(x))<<I2S_IMRn_RXDAM_SHIFT))&I2S_IMRn_RXDAM_MASK)
/******IMR0/IMR1/IMR2/IMR3(Interrupt Mask Register)******/

/*TOR0/TOR1/TOR2/TOR3(Transmit Overrun Register) ; Transmit channel FIFO overload , 0: TX FIFO write valid , 1: TX FIFO write overrun */
#define I2S_TORn_TXCHO_MASK                 (0x1)
#define I2S_TORn_TXCHO_SHIFT                (0U)
#define I2S_TORn_TXCHO(x)                   (((uint32_t)(((uint32_t)(x))<<I2S_TORn_TXCHO_SHIFT))&I2S_TORn_TXCHO_MASK)

/*ROR0/ROR1/ROR2/ROR3(Receive Overrun Register) ; 0: RX FIFO write valid , 1: RX FIFO write overrun */
#define I2S_RORn_RXCHO_MASK                 (0x1)
#define I2S_RORn_RXCHO_SHIFT                (0U)
#define I2S_RORn_RXCHO(x)                   (((uint32_t)(((uint32_t)(x))<<I2S_RORn_RXCHO_SHIFT))&I2S_RORn_RXCHO_MASK)

/*RFCR0/RFCR1/RFCR2/RFCR3(Receive FIFO Configuration Register)*/
#define I2S_RFCRn_RXFIFO_MASK               (0xF)
#define I2S_RFCRn_RXFIFO_SHIFT              (0U)
#define I2S_RFCRn_RXFIFO(x)                 (((uint32_t)(((uint32_t)(x))<<I2S_RFCRn_RXFIFO_SHIFT))&I2S_RFCRn_RXFIFO_MASK)

/*TFCR0/TFCR1/TFCR2/TFCR3(Transmit FIFO Configuration Register) */
#define I2S_TFCRn_TXFIFO_MASK               (0xF)
#define I2S_TFCRn_TXFIFO_SHIFT              (0U)
#define I2S_TFCRn_TXFIFO(x)                 (((uint32_t)(((uint32_t)(x))<<I2S_TFCRn_TXFIFO_SHIFT))&I2S_TFCRn_TXFIFO_MASK)

/*RFF0/RFF1/RFF2/RFF3(Receive FIFO Flush Register) ; 1:Reset 0: Not Reset*/
#define I2S_RFFn_FIFO_MASK                  (0x1)
#define I2S_RFFn_FIFO_SHIFT                 (0U)
#define I2S_RFFn_FIFO(x)                    (((uint32_t)(((uint32_t)(x))<<I2S_RFFn_FIFO_SHIFT))&I2S_RFFn_FIFO_MASK)

/*TFF0/TFF1/TFF2/TFF3(Transmit FIFO Flush Register); */
#define I2S_TFFn_FIFO_MASK                  (0x1)
#define I2S_TFFn_FIFO_SHIFT                 (0U)
#define I2S_TFFn_FIFO(x)                    (((uint32_t)(((uint32_t)(x))<<I2S_TFFn_FIFO_SHIFT))&I2S_TFFn_FIFO_MASK)

/*SCR(Software Configuration Register) ; */
/*The last SCLK contains several mclks */
#define I2S_SCR_LAST_SCLK_MASK              (0xFF000)
#define I2S_SCR_LAST_SCLK_SHIFT             (12U)
#define I2S_SCR_LAST_SCLK(x)                (((uint32_t)(((uint32_t)(x))<<I2S_SCR_LAST_SCLK_SHIFT))&I2S_SCR_LAST_SCLK_MASK)

/*The last SCLK contains several mclks */
#define I2S_SCR_DIV_SCLK_MASK               (0xFF0)
#define I2S_SCR_DIV_SCLK_SHIFT              (4U)
#define I2S_SCR_DIV_SCLK(x)                 (((uint32_t)(((uint32_t)(x))<<I2S_SCR_DIV_SCLK_SHIFT))&I2S_SCR_DIV_SCLK_MASK)

/*SW_DIV_SCLK_EN ; 1: enable 0: disable*/
#define I2S_SCR_SW_DIV_SCLK_EN_MASK          (0x1)
#define I2S_SCR_SW_DIV_SCLK_EN_SHIFT         (0U)
#define I2S_SCR_SW_DIV_SCLK_EN(x)            (((uint32_t)(((uint32_t)(x))<<I2S_SCR_SW_DIV_SCLK_EN_SHIFT))&I2S_SCR_SW_DIV_SCLK_EN_MASK)

/*MFR (Mclk Scect Register)  I2S clock selection register; 0:24Mhz*/
#define I2S_MCLK_SEL_MASK                   (0x1)
#define I2S_MCLK_SEL_SHIFT                  (0U)
#define I2S_MCLK_SEL(x)                     (((uint32_t)(((uint32_t)(x))<<I2S_MCLK_SEL_SHIFT))&I2S_MCLK_SEL_MASK)

/*RXDMA ;Receiver Block DMA Register*/
#define I2S_RXDMA_MASK                      (0x3)
#define I2S_RXDMA_SHIFT                     (0U)
#define I2S_RXDMA(x)                        (((uint32_t)(((uint32_t)(x))<<I2S_RXDMA_SHIFT))&I2S_RXDMA_MASK)

/*RRXDMA ;Reset Receiver Block DMA Register;1 reset , 0 not reset */
#define I2S_RRXDMA_MASK                      (0x1)
#define I2S_RRXDMA_SHIFT                     (0U)
//#define I2S_RRXDMA(x)                        (((uint32_t)(((uint32_t)(x))<<I2S_RRXDMA_SHIFT))&I2S_RRXDMA_MASK)

/*TXDMA; Transmit Block DMA Register*/
#define I2S_TXDMA_MASK                      (0x3)
#define I2S_TXDMA_SHIFT                     (0U)
#define I2S_TXDMA(x)                        (((uint32_t)(((uint32_t)(x))<<I2S_TXDMA_SHIFT))&I2S_TXDMA_MASK)

/*RTXDMA ;Reset Tramsmitter Block DMA Register;1 reset , 0 not reset */
#define I2S_RTXDMA_MASK                      (0x1)
#define I2S_RTXDMA_SHIFT                     (0U)
//#define I2S_RTXDMA(x)                        (((uint32_t)(((uint32_t)(x))<<I2S_RTXDMA_SHIFT))&I2S_RTXDMA_MASK)

/*I2S_COMP_PARAM_2 (Component Parameter Register 2)*/
#define I2S_RX_WORDSIZE_3_MASK           (0x1c00)
#define I2S_RX_WORDSIZE_3_SHIFT          (10)

#define I2S_RX_WORDSIZE_2_MASK           (0x380)
#define I2S_RX_WORDSIZE_2_SHIFT          (7)

#define I2S_RX_WORDSIZE_1_MASK           (0x38)
#define I2S_RX_WORDSIZE_1_SHIFT          (3)

#define I2S_RX_WORDSIZE_0_MASK           (0x7)
#define I2S_RX_WORDSIZE_0_SHIFT          (0)

/******I2S_COMP_PARAM_1(Component Parameter Register 1)******/
#define I2S_TX_WORDSIZE_3_MASK           (0xE000000)
#define I2S_TX_WORDSIZE_3_SHIFT          (25)

#define I2S_TX_WORDSIZE_2_MASK           (0x1C00000)
#define I2S_TX_WORDSIZE_2_SHIFT          (22)

#define I2S_TX_WORDSIZE_1_MASK           (0x380000)
#define I2S_TX_WORDSIZE_1_SHIFT          (19)

#define I2S_TX_WORDSIZE_0_MASK           (0x70000)
#define I2S_TX_WORDSIZE_0_SHIFT          (16)

#define I2S_TX_CHANNELS_MASK             (0x600)
#define I2S_TX_CHANNELS_SHIFT            (9)

#define I2S_RX_CHANNELS_MASK             (0x180)
#define I2S_RX_CHANNELS_SHIFT            (7)

/*1: enable , 0: disable*/
#define I2S_RECEIVER_BLOCK_MASK          (0x40)
#define I2S_RECEIVER_BLOCK_SHIFT         (6)

/*1: enable , 0: disable*/
#define I2S_TRANSMITTER_BLOCK_MASK       (0x20)
#define I2S_TRANSMITTER_BLOCK_SHIFT      (5)

/*1: enable , 0: disable*/
#define I2S_MODE_EN_MASK                 (0x10)
#define I2S_MODE_EN_SHIFT                (4)

#define I2S_FIFO_DEPTH_GLOBAL_MASK       (0xC)
#define I2S_FIFO_DEPTH_GLOBAL_SHIFT      (2)

#define I2S_APB_DATA_WIDTH_MASK          (0x3)
#define I2S_APB_DATA_WIDTH_SHIFT         (0)

/*I2S_COMP_VERSION(I2S Component Version Register);default value:0x3130332a*/
#define I2S_COMP_VERSION_MASK            (0xFFFFFFFF)
  
/*I2S_COMP_TYPE(I2S Component Type Register);default value : 0x445701a0*/
#define I2S_COMP_TYPE_MASK            (0xFFFFFFFF)

/*CR : Control Register*/
#define I2S_FORMAT_SEL_MASK           (0x3)
#define I2S_FORMAT_SEL_SHIFT          (0U)
#define I2S_FORMAT_SEL(x)             (((uint32_t)(((uint32_t)(x))<<I2S_FORMAT_SEL_SHIFT))&I2S_FORMAT_SEL_MASK)

/*RCR0 :Receive Configuration Register */
#define I2S_WORD_LENGTH_MASK           (0x3)
#define I2S_WORD_LENGTH_SHIFT          (0U)
#define I2S_WORD_LENGTH_SEL(x)         (((uint32_t)(((uint32_t)(x))<<I2S_WORD_LENGTH_SHIFT))&I2S_WORD_LENGTH_MASK)

#define I2S_ISR_MASK   (I2S_IMRn_TXFOM_MASK | I2S_IMRn_TXFEM_MASK | I2S_IMRn_RXFOM_MASK | I2S_IMRn_RXDAM_MASK)

//typedef void (*hal_i2s_isr_cb)(int32_t *data, uint8_t size);
//typedef void (*hal_i2s_isr_cb)(int32_t *data);



/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
hal_status_t hal_i2s_init(uint8_t i2sx, i2s_cfg_t *config);
hal_status_t hal_i2s_deinit(uint8_t i2sx);
hal_status_t hal_i2s_reinit(uint8_t i2sx);
hal_status_t hal_i2s_start(uint8_t i2sx);

bool hal_i2s_tx_ready(uint8_t i2sx);
bool hal_i2s_rx_ready(uint8_t i2sx);
bool hal_i2s_tx_overrun(uint8_t i2sx);
bool hal_i2s_rx_overrun(uint8_t i2sx);
uint32_t hal_i2s_clean_tx_overrun_status(uint8_t i2sx);
uint32_t hal_i2s_clean_rx_overrun_status(uint8_t i2sx);
void hal_i2s_tx_data(uint8_t i2sx , int32_t *data);
void hal_i2s_rx_data(uint8_t i2sx , int32_t* data);
hal_status_t hal_i2s_set_audio_freq(uint8_t i2sx, uint32_t audioFreq);

hal_status_t hal_i2s_tx_dma_init(uint8_t i2sx);
hal_status_t hal_i2s_rx_dma_init(uint8_t i2sx);
uint32_t hal_i2s_get_audio_freq(uint8_t i2sx);

hal_status_t hal_i2s_enable(uint8_t i2sx, uint8_t enable);
hal_status_t hal_i2s_set_mode(uint8_t i2sx, uint8_t mode);
hal_status_t hal_i2s_set_format(uint8_t i2sx, uint8_t format);
hal_status_t hal_i2s_set_sample_rate(uint8_t i2sx, uint32_t sampleRate);
hal_status_t hal_i2s_set_word_length(uint8_t i2sx, uint8_t wordLength);
hal_status_t hal_i2s_set_frame_length(uint8_t i2sx, uint8_t frameLength);
void hal_i2s_tx_enable(uint8_t i2sx, uint8_t enable);
void hal_i2s_rx_enable(uint8_t i2sx, uint8_t enable);


/* Private functions ---------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif

#endif  /* __HAL_I2S_H__ */
