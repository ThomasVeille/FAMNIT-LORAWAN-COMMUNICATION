#include <driver/i2s.h>

#define I2S_WS   33
#define I2S_SD   34
#define I2S_SCK  32
#define I2S_PORT I2S_NUM_0

void setupI2SMic() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 256,
    .use_apll = false
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
}

float readDBFS() {
  const int samplesToRead = 512;
  int32_t samples[samplesToRead];
  size_t bytesRead = 0;

  i2s_read(I2S_PORT, samples, sizeof(samples), &bytesRead, portMAX_DELAY);
  int samplesRead = bytesRead / sizeof(int32_t);

  double sumSquares = 0;
  for (int i = 0; i < samplesRead; i++) {
    int32_t sample = samples[i] >> 8;
    sumSquares += (double)sample * (double)sample;
  }
  double rms = sqrt(sumSquares / samplesRead);

  if (rms < 1) rms = 1;

  const double FULL_SCALE = 8388608.0; 
  double normalized = rms / FULL_SCALE;

  return 20.0 * log10(normalized); 
}
//Approximative conversion with the technologie use by the ICS-43434 sensor
float dbfsToSPL(float dbfs) {
  const float REF_DBFS = -26.0;
  const float REF_SPL = 94.0;
  return dbfs - REF_DBFS + REF_SPL;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Test microphone I2S ICS-43434");
  setupI2SMic();
}

void loop() {
  float dbfs = readDBFS();
  float spl = dbfsToSPL(dbfs);

  //Serial.println("dBFS (relatif) : " + String(dbfs, 1) + "   |   dB SPL (approx.) : " + String(spl, 1));
  Serial.println("Microphone  ICS43434 : " + String(spl, 1) + " dB");
  delay(500);
}