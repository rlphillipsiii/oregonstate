#ifndef THERMO_DRIVER
#define THERMO_DRIVER

struct temperature {
	uint16_t raw;
	uint8_t fahrenheit;
	uint8_t celsius;
	
	uint8_t positive;
};

void init_thermo_sensors();
void prepare_local_read(uint8_t wait);
void local_thermo_read(uint8_t wait);
void get_local_temp(struct temperature * temp);

#ifdef MEGA128
void decode_temp(struct temperature *temp);
#endif

#endif
