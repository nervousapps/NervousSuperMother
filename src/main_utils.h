#ifndef Main_Utils_h
#define Main_Utils_h

#define DEBUG true

#define MIDI_CHANNEL 16
const int chipSelect = BUILTIN_SDCARD;
boolean hasSD = false;

// SD card utils
boolean openSDCard()
{
  int crashCountdown = 0;
  if (!(SD.begin(chipSelect)))
  {
    while (!(SD.begin(chipSelect)))
    {
      delay(20);
      crashCountdown++;
      if (crashCountdown > 4)
      {
        return false;
      }
    }
  }
  return true;
}

void init_sd_card()
{
  if (DEBUG)
    Serial.println("Initializing SD card...");
  hasSD = openSDCard();
  if (!hasSD)
  {
    if (DEBUG)
      Serial.println("initialization failed!");
  }
}

// Debug utils
#if DEBUG
volatile uint32_t cpu_load = 0;
volatile bool has_result = false;

void cpuLoadSleep()
{
  uint32_t st;
  static uint32_t wt = 0;
  static uint32_t busy_time = 0;
  static uint32_t sleep_time = 0;

  busy_time += ARM_DWT_CYCCNT - wt;
  st = ARM_DWT_CYCCNT;
  __disable_irq();
  __asm volatile("wfi \n");
  sleep_time += ARM_DWT_CYCCNT - st;
  wt = ARM_DWT_CYCCNT;
  __enable_irq();
  if ((busy_time + sleep_time) > F_CPU)
  {
    cpu_load = busy_time / ((busy_time + sleep_time) / 1000);
    busy_time = 0;
    sleep_time = 0;
    has_result = true;
  }
}

void CPUMonitor()
{
  Serial.print("CPU:");
  Serial.print(AudioProcessorUsage());
  Serial.print("  MEM:");
  Serial.println(AudioMemoryUsage());
  Serial.print("CPU MAX:");
  Serial.print(AudioProcessorUsageMax());
  Serial.print("  MEM MAX:");
  Serial.println(AudioMemoryUsageMax());
}
#endif

#endif