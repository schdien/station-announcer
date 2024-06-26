#ifndef __JQ8900_H__
#define __JQ8900_H__

void JQ8900_commandData(uint8_t command, uint8_t data[], uint8_t dataLen);
void JQ8900_inReport(uint8_t stationNow);
void JQ8900_outReport(uint8_t stationNext);
#endif
