#include <Performance_Calibration_Bird_sound_classifier_inferencing.h>
#include "sample_features.h"

float features[32000];
int count = 0;
ei_impulse_result_t result = { 0 };

int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (count%2==0){
    for (int i=0; i<32000; i++){
        features[i] = features_1[i];
    }
  }else{
    for (int i=0; i<32000; i++){
        features[i] = features_2[i];
    }
  }
  count++;

  signal_t features_signal;
  features_signal.total_length = sizeof(features) / sizeof(features[0]);
  features_signal.get_data = &raw_feature_get_data;

  run_classifier(&features_signal, &result, false);

  Serial.print("Predictions (DSP: ");
  Serial.print(result.timing.dsp);
  Serial.print(" ms., Classification: ");
  Serial.print(result.timing.classification);
  Serial.print(" ms., Anomaly: ");
  Serial.print(result.timing.anomaly);
  Serial.print(" ms.): \n");
  
  Serial.print("[");
  for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
            Serial.print(result.classification[ix].value, 5);
            Serial.print(", ");
  }
  Serial.print("]\n");
  
  delay(5000);
}
