/*To deploy this code on Sony Spresense you need to:
 * Follow this guide to install the Spresense Arduino Library: https://developer.sony.com/develop/spresense/docs/arduino_set_up_en.html#_install_spresense_arduino_library
 * Include the ei-performance-calibration_-bird-sound-classifier-arduino-1.0.14.zip library in "Sketch > Include Library > Add . ZIP Library" 
 * Select the board in "Tools > Board"
 * Change the setting in "Tools > Memory" to 896 KB
*/

#include <Performance_Calibration_Bird_sound_classifier_inferencing.h> //This need also to be included in "Sketch > Include Library > Add . ZIP Library"
#include <Audio.h>
#include <SDHCI.h>
#include <File.h>
#include <LowPower.h>
#include <RTC.h>

// Declare an audio object
AudioClass *theAudio; 

// To print prediction on SD card
SDClass SD; 
File myFile;

#define BUFFER_SIZE 200 // Size of the buffer for theAudio->readFrames
#define MAX_SIZE 64000 // Size of the array containg the raw audio samples: 2s * 16000 Hz * 2 bytes (16bits) 

int features[MAX_SIZE/2]; // Array containg the recorded samples

ei_impulse_result_t result = { 0 }; // Contains the result of the predictions

// This function is passed to features_signal.get_data
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

// A function that converts the recorded samples from bytes (char) to integers
void pcm_to_int(char* pcm_bytes, int* pcm_samples, int num_samples) {
    for (int i = 0; i < num_samples; i++) {
        int16_t pcm_sample = (pcm_bytes[i*2 + 1] << 8) | pcm_bytes[i*2]; // convert from little-endian bytes to signed int
        pcm_samples[i] = (int)pcm_sample;
    }
}


void get_audio(int samples[]){

    // Buffers where the temporary data is stored
    char s_buffer[BUFFER_SIZE];
    int transformed_buffer[BUFFER_SIZE/2];

    // Set the audio object in recording mode from the ready state
    theAudio->setRecorderMode(AS_SETRECDR_STS_INPUTDEVICE_MIC,200); // 200=20dB gain -> can be changed to adapt to the environment
    // Initialize the recorder
    theAudio->initRecorder(AS_CODECTYPE_PCM, "/mnt/sd0/BIN", AS_SAMPLINGRATE_16000, AS_CHANNEL_MONO);
    // Start the recording
    theAudio->startRecorder();

    uint32_t read_size=0; // Size of the current frame
    uint32_t total_size=0; // Size of the total recorded samples
    
    while(total_size<MAX_SIZE){ 
      // printf("Total: %u \n", total_size);
      // printf("Read: %u \n", read_size);
      theAudio->readFrames(s_buffer, BUFFER_SIZE, &read_size); // reads from theAudio to the s_buffer and updates the read_size
      usleep(50); // to give time to theAudio to get data
      // convert samples from char to int and append to samples
      for (int i=0; i<read_size/2; i++){
        pcm_to_int(s_buffer, transformed_buffer, BUFFER_SIZE/2);
        samples[i+total_size/2] = transformed_buffer[i];
      }
      total_size += read_size; // update total size
    }
    // End recording
    theAudio->stopRecorder();
    // Get back to ready mode (for power saving)
    theAudio->setReadyMode();
}


void setup() {
  Serial.begin(115200);
  RTC.begin();
  theAudio = AudioClass::getInstance();
  theAudio->begin();
  while (!SD.begin()) {
    ; /* wait until SD card is mounted. */
  }
  SD.mkdir("predictions/");
  
}

void loop() {
  // Record the audio
  get_audio(features);

  // Declare and fill a features_signal
  signal_t features_signal;
  features_signal.total_length = sizeof(features) / sizeof(features[0]);
  features_signal.get_data = &raw_feature_get_data;

  // Perform the classification
  run_classifier(&features_signal, &result, false);

  // Elapsed time
  RtcTime rtc = RTC.getTime();
  int h = rtc.hour(); 
  int m = rtc.minute(); 
  int s = rtc.second();
  

  // printing the time in the format of hh:mm:ss
//  Serial.printf("%02d:%02d:%02d\n", h, m, s);

//  Serial.print("Predictions (DSP: ");
//  Serial.print(result.timing.dsp);
//  Serial.print(" ms., Classification: ");
//  Serial.print(result.timing.classification);
//  Serial.print(" ms., Anomaly: ");
//  Serial.print(result.timing.anomaly);
//  Serial.print(" ms.): \n");
//  

//  Serial.print("[");
//  for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
//            Serial.print(result.classification[ix].value, 5);
//            Serial.print(", ");
//  }
//  Serial.print("]\n");

  // To print the samples and plot them
//  for (int i = 0; i<10000; i++){
//    printf("%d, ", features[i]); 
//  }
//  printf("done \n");

  // Write the predictions on the SD card
  myFile = SD.open("predictions/predictions.txt", FILE_WRITE);
  if (myFile) {
    myFile.printf("%02d:%02d:%02d\n", h, m, s);
    myFile.print("[");
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
              myFile.print(result.classification[ix].value, 5);
              myFile.print(", ");
    }
    myFile.print("]\n");
    myFile.close();
  } 

  // Wait some time before a new recording 
  LowPower.deepSleep(5); // This function set a low power mode while the board is unused

}
