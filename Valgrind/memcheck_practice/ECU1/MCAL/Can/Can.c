#include "Can.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>


extern void CanIf_RxIndication(uint8 Hrh, uint32 CanId, uint8 CanDlc, const uint8 *CanSduPtr);


void Can_Init(const Can_ConfigType* Config)
{
    
}

void Can_DeInit(void)
{
    
}

Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition)
{
   return 1;
}

Std_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo)
{
   return 1;
}


void Can_MainFunction_Write(void)
{
    /* Not use in small demo */
}

void Can_MainFunction_Read(void)
{
    
}


void Can_MainFunction_BusOff(void)
{
    /* Not use in small demo */
}

void Can_MainFunction_Wakeup(void)
{
    /* Not supported by Linux Can Interface */
}

void Can_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
   
}
