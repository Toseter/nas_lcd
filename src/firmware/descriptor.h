#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

/*
 * USB Device Descriptor Structure
 */
typedef struct _USB_DEV_DSC
{
    unsigned char            bLength;
    unsigned char            bDscType;
    unsigned int             bcdUSB;
    unsigned char            bDevCls;
    unsigned char            bDevSubCls;
    unsigned char            bDevProtocol;
    unsigned char            bMaxPktSize0;
    unsigned int             idVendor;
    unsigned int             idProduct;
    unsigned int             bcdDevice;
    unsigned char            iMFR;
    unsigned char            iProduct;
    unsigned char            iSerialNum;
    unsigned char            bNumCfg;
} USB_DEV_DSC;

/*
 * USB Configuration Descriptor Structure
 */
typedef struct _USB_CFG_DSC
{
    unsigned char            bLength;
    unsigned char            bDscType;
    unsigned int             wTotalLength;
    unsigned char            bNumIntf;
    unsigned char            bCfgValue;
    unsigned char            iCfg;
    unsigned char            bmAttributes;
    unsigned char            bMaxPower;
} USB_CFG_DSC;

/*
 * USB Interface Descriptor Structure
 */
typedef struct _USB_INTF_DSC
{
    unsigned char            bLength;
    unsigned char            bDscType;
    unsigned char            bIntfNum;
    unsigned char            bAltSetting;
    unsigned char            bNumEPs;
    unsigned char            bIntfCls;
    unsigned char            bIntfSubCls;
    unsigned char            bIntfProtocol;
    unsigned char            iIntf;
} USB_INTF_DSC;

/*
 * USB Endpoint Descriptor Structure
 */
typedef struct _USB_EP_DSC
{
    unsigned char            bLength;
    unsigned char            bDscType;
    unsigned char            bEPAdr;
    unsigned char            bmAttributes;
    unsigned int             wMaxPktSize;
    unsigned char            bInterval;
} USB_EP_DSC;


typedef struct _USB_CONFIGURATION
{   USB_CFG_DSC             cd01;
    USB_INTF_DSC            i00a00;
    USB_EP_DSC              ep01o_i00a00;
    USB_EP_DSC              ep01i_i00a00;
} USB_CONFIGURATION;

extern const USB_DEV_DSC device_dsc;
extern const USB_CONFIGURATION cfg01;

typedef struct { unsigned char l; unsigned char t; unsigned int s[1]; } __sd000_t;
typedef struct { unsigned char l; unsigned char t; unsigned int s[6]; } __sd001_t;
typedef struct { unsigned char l; unsigned char t; unsigned int s[14]; } __sd002_t;
typedef struct { unsigned char l; unsigned char t; unsigned int s[5]; } __sd003_t;

extern const __sd000_t sd000;
extern const __sd001_t sd001;
extern const __sd002_t sd002;
extern const __sd003_t sd003;


#endif