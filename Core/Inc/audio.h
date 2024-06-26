#ifndef INC_AUDIO_H_
#define INC_AUDIO_H_

struct audio{
	struct {
		void (*WriteCommandData)(uint8_t command, uint8_t data[]);
	}Transmit;
	struct{
		void (*Stop)();
	}Setup;
	struct {
		void (*In)(uint8_t stationNow);
		void (*Out)(uint8_t stationNext);
		void (*Tips)();
	}Report;
}Audio;

#endif /* INC_AUDIO_H_ */
