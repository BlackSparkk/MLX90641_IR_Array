#include <MLX90641_API.h>
#include <MLX90641_I2C_Driver.h>

#define slave_addr 0x33
#define TA_SHIFT 5

float emissivity = 0.95;
float tr;

static uint16_t eeprom[832];
static uint16_t frame[242];
paramsMLX90641 params;
static float temperatures[192];

void setup() {
  SerialUSB.begin(115200);
  MLX90641_I2CInit();
  MLX90641_I2CFreqSet(1000);
}

void loop() {
  delay(2000);
  get_pixels();
  print_pixels();  
}

void get_pixels()
{
  MLX90641_DumpEE(slave_addr,eeprom);
  MLX90641_ExtractParameters(eeprom, &params);
  MLX90641_GetFrameData(slave_addr, frame);
  tr = MLX90641_GetTa(frame,&params) - TA_SHIFT;
  MLX90641_CalculateTo(frame,&params,emissivity,tr,temperatures);
}

void print_pixels()
{
  for(int i = 0 ; i < 12 ; i++){
    for(int j = 0 ; j < 16 ; j++)
    {
      SerialUSB.print(temperatures[j+16*i]);
      SerialUSB.print(" : ");
    }
    SerialUSB.print("\n\n");  
  }
  SerialUSB.println("---------------------------------------------------------------------------------------------------------------------------------");   
}
