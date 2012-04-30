#include <htc.h>
#include "usb.h"
#include "descriptor.h"

BDT ep0Bo;         //Endpoint #0 BD Out
BDT ep0Bi;         //Endpoint #0 BD In


CTRL_TRF_SETUP   SetupPkt;

unsigned char ctrl_trf_data[EP0_BUFF_SIZE];

unsigned char ctrl_trf_state;            // Control Transfer State

unsigned int ctrl_trf_count;            // Data counter

unsigned char ctrl_trf_mem;

POINTER pSrc;                   // Data source pointer

POINTER pDst;                   // Data destination pointer

unsigned char usb_device_state;          // Device States: DETACHED, ATTACHED, ...

unsigned char usb_active_cfg;            // Value of current configuration

unsigned char usb_new_addr;              // Address sent by host

extern void USBUserInit(void);

void USBPrepareForNextSetupTrf(void)
{
    ctrl_trf_state = WAIT_SETUP;          // See usbctrltrf.h
    ep0Bo.Cnt  = EP0_BUFF_SIZE;           // ?????? ??????
    ep0Bo.ADR  = (unsigned char*)&SetupPkt;   // ????? SetupPkt
    ep0Bo.Stat = _USIE | _DAT0 | _DTSEN;  // EP0 buff dsc init, see usbmmap.h
    ep0Bi.Stat = _UCPU;                   // EP0 IN buffer initialization
}

void USBCtrlTrfTxService(void)
{
    unsigned char count;

    /*
     * First, have to figure out how many byte of data to send.
     */
    if(ctrl_trf_count < EP0_BUFF_SIZE)
        count = ctrl_trf_count;
    else
        count = EP0_BUFF_SIZE;

    ep0Bi.Cnt = count;

    /*
     * Subtract the number of bytes just about to be sent from the total.
     */
    ctrl_trf_count -= count;

    #asm
        LFSR 0
        _ctrl_trf_data
    #endasm
    if(ctrl_trf_mem == _ROM)
    {
        /* ?????????? ?? ROM */
        TBLPTRL = pSrc.bLow;
        TBLPTRH = pSrc.bHigh;
        TBLPTRU = pSrc.bUpper;
        while(count)
        {
            #asm 
                TBLRD*+
            #endasm
	    POSTINC0 = TABLAT;
            count--;
	}
	/* ??????????? ????????? */
        pSrc.bLow   = TBLPTRL;
        pSrc.bHigh  = TBLPTRH;
        pSrc.bUpper = TBLPTRU;
    }
    else
    {
        /* ?????????? ?? RAM */
        FSR2L = pSrc.bLow;
        FSR2H = pSrc.bHigh;
        while(count)
        {
            POSTINC0 = POSTINC2;
            count--;
	}
	/* ??????????? ????????? */
        pSrc.bLow  = FSR2L;
        pSrc.bHigh = FSR2H;
    }
}//end USBCtrlTrfTxService

void USBCtrlEPServiceComplete(void)
{
    if(SetupPkt.bmRequestType & 0x80) // DEV_TO_HOST
    {
        /* Control Read */
        ctrl_trf_state = CTRL_TRF_TX;

        if(SetupPkt.wLength < ctrl_trf_count)
            ctrl_trf_count = SetupPkt.wLength;

        /* ??????????? ?????? ?????? ????????????? ?????? ? ????? */
        USBCtrlTrfTxService();

        /* Data stage - ???????? ????????????? ?????? */
        ep0Bi.ADR = (unsigned char*)&ctrl_trf_data;

        /* Status stage - ??????? ?????? OUT(1) */
        ep0Bo.Cnt = EP0_BUFF_SIZE;
        ep0Bo.ADR = (unsigned char*)&SetupPkt;
        ep0Bo.Stat = _USIE;           // Note: DTSEN is 0!
    }
    else // HOST_TO_DEV
    {
        /* Control Write ??? No-data Control */
        ctrl_trf_state = CTRL_TRF_RX;

        /* Data stage - ??????? ?????? ???? ??????????? */
        ep0Bo.Cnt = EP0_BUFF_SIZE;
        ep0Bo.ADR = (unsigned char*)&ctrl_trf_data;
        ep0Bo.Stat = _USIE | _DAT1 | _DTSEN;

        /* Status stage - ???????? ?????? IN(1) */
        ep0Bi.Cnt = 0;
    }
}

void USBCtrlStall(void)
{
    /* ?????? STALL ? ???????????? ??????? ????????? SETUP */
    ep0Bo.Cnt = EP0_BUFF_SIZE;
    ep0Bo.ADR = (unsigned char*)&SetupPkt;
    ep0Bo.Stat = _USIE | _BSTALL;
    ep0Bi.Stat = _USIE | _BSTALL;
}

void USBCheckStdRequest(void)
{
    ctrl_trf_count = 0;

    if(SetupPkt.bmRequestType & 0x60) goto Stall;

    switch(SetupPkt.bRequest)
    {
        case SET_ADR:
            usb_new_addr     = (unsigned char)SetupPkt.wValue;   // ????? ????? ???????
            usb_device_state = ADR_PENDING_STATE;       // Update state only
	    break;
        case GET_DSC:
	    switch(SetupPkt.bDscType)
	    {
                case DSC_DEV:
                    pSrc.bRom = (unsigned char*)&device_dsc;
                    ctrl_trf_count = sizeof(device_dsc);
		    break;
                case DSC_CFG:
                    pSrc.bRom = (unsigned char*)&cfg01;
                    ctrl_trf_count = sizeof(cfg01);
		    break;
                case DSC_STR:
		    switch(SetupPkt.bDscIndex)
		    {
		        case 1:
	                    pSrc.bRom = (unsigned char*)&sd001;
	                    ctrl_trf_count = sizeof(sd001);
			    break;
		        case 2:
	                    pSrc.bRom = (unsigned char*)&sd002;
	                    ctrl_trf_count = sizeof(sd002);
			    break;
		        case 3:
	                    pSrc.bRom = (unsigned char*)&sd003;
	                    ctrl_trf_count = sizeof(sd003);
			    break;
		        default:
	                    pSrc.bRom = (unsigned char*)&sd000;
	                    ctrl_trf_count = sizeof(sd000);
			    break;
		    }
		    break;
                default: // ??? ?????? ???????????
		    goto Stall;
	    }
            ctrl_trf_mem = _ROM;               // Set memory type
            break;
        case SET_CFG:
            usb_active_cfg = (unsigned char)SetupPkt.wValue;

            if(usb_active_cfg == 0)
                usb_device_state = ADDRESS_STATE;
            else
            {
                usb_device_state = CONFIGURED_STATE;
		USBUserInit();
            }
            break;
        case GET_CFG:
            pSrc.bRam = (unsigned char*)&usb_active_cfg;    // Set Source
            ctrl_trf_count = 1;                         // Set data count
            ctrl_trf_mem = _RAM;                        // Set memory type
            break;
        default: // ??????????? ??????
	    goto Stall;
    }
    USBCtrlEPServiceComplete();
    return;
Stall:
    USBCtrlStall();
    return;
}

void USBCtrlEPService(void)
{
    if(USTAT == EP00_OUT)
    {
        if((ep0Bo.Stat & TOKEN_TYPE_MASK) == SETUP_TOKEN) // EP0 SETUP
	{
            USBCheckStdRequest();
            ep0Bi.Stat = _USIE | _DAT1 | _DTSEN;
            UCONbits.PKTDIS = 0;
	}
        else                                        // EP0 OUT
	{
	    if(ctrl_trf_state == CTRL_TRF_RX)
	    {
	        if(ep0Bo.Stat & _DTSMASK) // DTS == 1
	            ep0Bo.Stat = _USIE | _DAT0 | _DTSEN;
	        else
	            ep0Bo.Stat = _USIE | _DAT1 | _DTSEN;
	    }
	    else
	    {
	        USBPrepareForNextSetupTrf();
	    }
        }
    }
    else if(USTAT == EP00_IN)                       // EP0 IN
    {
	if(usb_device_state == ADR_PENDING_STATE)
        {
            UADDR = usb_new_addr;
            if(UADDR)
	    {
                usb_device_state = ADDRESS_STATE;
		PORTAbits.RA5 = 1;              /* usb is active */
	    }
	    else
                usb_device_state = DEFAULT_STATE;
        }

        if(ctrl_trf_state == CTRL_TRF_TX)
        {
            USBCtrlTrfTxService();

            if(ep0Bi.Stat & _DTSMASK) // DTS == 1
                ep0Bi.Stat = _USIE | _DAT0 | _DTSEN;
            else
                ep0Bi.Stat = _USIE | _DAT1 | _DTSEN;
        }
        else
	{
           USBPrepareForNextSetupTrf();
	}
    }
}//end USBCtrlEPService

void USBIntService(void)
{
    unsigned char flags = UIR & UIE;

    if(flags & _ACTIVIF)
    {
        UCONbits.SUSPND = 0;
        UIEbits.ACTVIE = 0;
        UIRbits.ACTVIF = 0;
        UIRbits.IDLEIF = 0;
        UIEbits.IDLEIE = 1;
    }

    if(flags & _URSTIF)
    {
        while(UIRbits.TRNIF)            // Flush any pending transactions
            UIRbits.TRNIF = 0;
        UIR   = 0;                      // Clears all USB interrupts
        UIE   = 0x7B;                   // Enable all interrupts except ACTVIE
        UADDR = 0;                      // Reset to default address
        UEP0  = EP_CTRL | HSHK_EN;      // Init EP0 as a Ctrl EP
        USBPrepareForNextSetupTrf();    // ?????????? BDT ??? ?????? SETUP
        UCONbits.PKTDIS = 0;            // Make sure packet processing is enabled

        usb_active_cfg   = 0;           // Clear active configuration
        usb_device_state = DEFAULT_STATE;
    }

    if(flags & _IDLEIF)
    {
        UIEbits.ACTVIE = 1;                    // Enable bus activity interrupt
        UIRbits.IDLEIF  = 0;
        UCONbits.SUSPND = 1;                    // Put USB module in power conserve
    }

    if(flags & _STALLIF)
    {
        if(UEP0bits.EPSTALL)
        {
            USBPrepareForNextSetupTrf();        // Firmware Work-Around
            UEP0bits.EPSTALL = 0;
        }
        UIRbits.STALLIF = 0;
    }

    if(usb_device_state < DEFAULT_STATE) return;

    if(flags & _TRNIF)
    {
        USBCtrlEPService();
        UIRbits.TRNIF = 0;
    }
}

void USBInit(void)
{
    ep0Bo.Stat = _UCPU;                   // EP0 OUT BDT
    ep0Bi.Stat = _UCPU;                   // EP0 IN  BDT
    UCFG = _PUEN | _TRINT | _FS | _PPBM0;
    UCON = 0x08;                          // Enable USBEN only
    while(UCONbits.SE0);                  // Blocking loop
    UIR = 0;                              // Clear all USB interrupts
    UIE = 0x11;                           // Unmask RESET & IDLE interrupts only

    usb_device_state = POWERED_STATE;
}