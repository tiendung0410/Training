#include "Dio.h"
#include <string.h>

/* ================== Private state ================== */




/* ==== Helpers ==== */

/* ================== API ================== */
void Dio_Init(const Dio_ConfigType* ConfigPtr) {
  
}

Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId) {
  return 0;
}

void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level) {
 
}

Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId) {
  return 0;
}

/* === Port operations  === */
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId) {
  return 0;
}

void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level) {
  
}

void Dio_MaskedWritePort(Dio_PortType PortId, Dio_PortLevelType Level, Dio_PortLevelType Mask) {
  
}

/* === Channel group (mask/offset on 1 port) === */
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType* grp) {
  return 0;
}

void Dio_WriteChannelGroup(const Dio_ChannelGroupType* grp, Dio_PortLevelType Level) {
  
}

/* === Version info === */
void Dio_GetVersionInfo(uint16* major, uint16* minor, uint16* patch) {
 
}
