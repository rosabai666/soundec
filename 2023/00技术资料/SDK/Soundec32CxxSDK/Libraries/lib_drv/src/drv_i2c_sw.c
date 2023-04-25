#if 1//def SUPPORT_I2C_DRV
#include "types.h"
#include "common.h"
#include "hal_gpio.h"

static bool sw_i2c_getSda(void);

#define IO_OUT	GPIO_DIRECTION_OUTPUT
#define IO_IN	GPIO_DIRECTION_INPUT
#define IO_HIGH	GPIO_DATA_HIGH
#define IO_LOW	GPIO_DATA_LOW

#define I2C_CLK_PIN			GPIO_10
#define I2C_SDA_PIN			GPIO_11

#define __I2C_CFG_CLK(OorI)	       	{hal_gpio_set_direction(I2C_CLK_PIN, OorI);}
#define __I2C_CFG_DAT(OorI)  	   	{hal_gpio_set_direction(I2C_SDA_PIN, OorI);}
#define __I2C_SET_CLK(HiLo)	       	{hal_gpio_set_output(I2C_CLK_PIN, HiLo);}
#define __I2C_SET_DAT(HiLo)  	   	{hal_gpio_set_output(I2C_SDA_PIN, HiLo);}
#define __I2C_GET_DAT_HIGH          (sw_i2c_getSda() == IO_HIGH)

#define __I2C_DELAY(x)				{delay_us(x);}

static bool sw_i2c_getSda(void)
{
    return hal_gpio_get_input(I2C_SDA_PIN);
}

void sw_i2c_init(void)
{
    hal_gpio_set_pinmux_function(GPIO_10, GPIO_10_FUNC_GPIO);
    hal_gpio_set_pinmux_function(GPIO_11, GPIO_11_FUNC_GPIO);	//sda

    __I2C_CFG_DAT(IO_OUT);
    __I2C_CFG_CLK(IO_OUT);
    __I2C_SET_DAT(IO_HIGH);
    __I2C_SET_CLK(IO_HIGH);
}

// -\___
// ---\_
static void sw_i2c_start(void)
{
    __I2C_SET_DAT(IO_HIGH);
    __I2C_SET_CLK(IO_HIGH);
    __I2C_DELAY(1);
    __I2C_SET_DAT(IO_LOW);
    __I2C_DELAY(1);
    __I2C_SET_CLK(IO_LOW);
}

// _/-
// ---
static void sw_i2c_stop(void)
{
    __I2C_SET_CLK(IO_LOW);	         __I2C_DELAY(1);
    __I2C_CFG_DAT(IO_OUT);
    __I2C_SET_DAT(IO_LOW);	         __I2C_DELAY(1);
    __I2C_SET_CLK(IO_HIGH);            __I2C_DELAY(1);
    __I2C_SET_DAT(IO_HIGH);            __I2C_DELAY(1);
    __I2C_SET_CLK(IO_LOW);             __I2C_DELAY(1);
    __I2C_SET_CLK(IO_HIGH);            __I2C_DELAY(1);
}

static U8 sw_i2c_getAck(void)
{
    U16 i=0;

    __I2C_SET_DAT(IO_HIGH);
    __I2C_CFG_DAT(IO_IN);
    __I2C_DELAY(1);
    __I2C_SET_CLK(IO_HIGH);
    __I2C_DELAY(1);

    while((__I2C_GET_DAT_HIGH)&&(i<0xFFFF))
    {
        __I2C_DELAY(1);
        i++;
    }
    __I2C_SET_CLK(IO_LOW);

    __I2C_DELAY(1);
    __I2C_CFG_DAT(IO_OUT);

    if(i>=0xFFFF)
        return 0;//ack fail
    return 1;  //ack ok
}

static void sw_i2c_setNak(void)
{
    __I2C_SET_CLK(IO_HIGH);
    __I2C_DELAY(1);
    __I2C_SET_CLK(IO_LOW);
    __I2C_DELAY(1);
}

static void sw_i2c_setAck (void)
{
    __I2C_SET_DAT(IO_LOW);
    __I2C_DELAY(1);
    __I2C_SET_CLK(IO_HIGH);
    __I2C_DELAY(1);
    __I2C_SET_CLK(IO_LOW);
    __I2C_DELAY(1);
}

void sw_i2c_setData(U8 senddata)
{
    U8 i;
    U8 bMask[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

    /* MSB is sent first */
    for(i=0;i<8;i++)
    {
        if (senddata&bMask[i])
        {
            __I2C_SET_DAT(IO_HIGH);
        }
        else
        {
            __I2C_SET_DAT(IO_LOW);
        }
        __I2C_DELAY(1);
        __I2C_SET_CLK(IO_HIGH);
        __I2C_DELAY(1);
        __I2C_SET_CLK(IO_LOW);
    }
}

U8 sw_i2c_getData(void)
{
    U8 i,tmpdata=0;
    U8 bMask[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

    __I2C_SET_DAT(IO_HIGH);
    __I2C_CFG_DAT(IO_IN);
    for(i=0;i<8;i++)
    {
        __I2C_SET_CLK(IO_HIGH);
        __I2C_DELAY(1);
        if(__I2C_GET_DAT_HIGH)
            tmpdata+=bMask[i];
        __I2C_DELAY(1);
        __I2C_SET_CLK(IO_LOW);
        __I2C_DELAY(1);
    }
    __I2C_CFG_DAT(IO_OUT);
    __I2C_SET_DAT(IO_HIGH);
    return tmpdata;
}


// START,Write dAddr,ACK,iAddr,ACK,DATA0 ACK,...DATAn,noACK,STOP
U8 sw_i2c_txStr(U8 iDeviceAddr,U8 iAddr,U8 *iDataBuf,U8 iLen,U8 iLen_x)
{
    U8 i,j=0,retry=0;

    sw_i2c_init();
    if(iLen<1) iLen=1;
    j=iLen;//-1;
I2C_ST_POS:
    retry++;
    if(retry>100)   return 0;
    sw_i2c_start();

    sw_i2c_setData(iDeviceAddr);
    if(!sw_i2c_getAck())    goto I2C_ST_POS;

    sw_i2c_setData(iAddr);
    if(!sw_i2c_getAck())    goto I2C_ST_POS;

    for(i=iLen_x;i<iLen;i++)
    {
        sw_i2c_setData(iDataBuf[i]);
        if(i<j)
        {
            if(!sw_i2c_getAck())    goto I2C_ST_POS;
        }
    }
//    sw_i2c_setNak();
    sw_i2c_stop();
    return 1;
}

U8 sw_i2c_rxStr(U8 iDeviceAddr,U8 iAddr,U8 *Rdata_buf,U8 len,U8 noregaddr)
{
    U8 retry=0;
    U8 i=0, j=0;

    sw_i2c_init();
I2C_ST_POS:
    retry++;
    if(retry>100)   return 0;
    //---------------------START----------------------------------
    sw_i2c_start();
    if (noregaddr==0)
    {
        //---------------------dAddr----------------------------------
        sw_i2c_setData(iDeviceAddr);
        if(!sw_i2c_getAck())    goto I2C_ST_POS;

        delay_us(50);

        //----------------------iAddr---------------------------------
        sw_i2c_setData(iAddr);
        if(!sw_i2c_getAck())    goto I2C_ST_POS;

        sw_i2c_start();
    }
    sw_i2c_setData(iDeviceAddr | 0x01);
    if(!sw_i2c_getAck())    goto I2C_ST_POS;

    j= len-1;
    for (i = 0; i < j; i++)
    {
        __I2C_DELAY(50);
        *Rdata_buf++ = sw_i2c_getData();
        sw_i2c_setAck();
    }
    __I2C_DELAY(50);

    *Rdata_buf = sw_i2c_getData();
    sw_i2c_setNak();

    sw_i2c_stop();
    return   1;
}


#endif   //I2C_DRV

