#ifndef INC_SYSTEM_ASSET_H_
#define INC_SYSTEM_ASSET_H_
//warning: This head file can only be included in ONE source file.

struct color {
	uint8_t Black[3];
	uint8_t Blue[3];
	uint8_t Red[3];
	uint8_t Magenta[3];
	uint8_t Green[3];
	uint8_t Cyan[3];
	uint8_t Yellow[3];
	uint8_t White[3];
}const Color = {
		{ 0x00,0x00,0x00 },
		{ 0x00,0x00,0x80 },
		{ 0xfc,0x00,0x00 },
		{ 0xfc,0x00,0xfc },
		{ 0x00,0x80,0x00 },
		{ 0x00,0xfc,0xfc },
		{ 0xfc,0xfc,0x00 },
		{ 0xfc,0xfc,0xfc },
};




struct route {
	uint8_t StationNum;
	char* Title;
	char* Station[16];//similar to 2D array
}const Route[]={
		{7,{"302路"},{"地铁红高路站","电子科大医院站","电子科大西校门站","天润路水杉路口站","西源大道天润路口站","电子科大南校门站","电子科大南二门站"}},//Route[0]
		{16,{"205路"},{"尚丰路站","地铁尚锦路站","西区大道尚华路口站","西区大道尚雅路口站","模具工业园站","地铁红高路站","天润路站","电子科大医院站","电子科大西校门站","合作路天润路口站","天骄路中站","天骄路南站","西源大道天润路口站","电子科大南校门站","电子科大南二门站","顺清街站"}}//Route[1]
};



#endif /* INC_SYSTEM_ASSET_H_ */
