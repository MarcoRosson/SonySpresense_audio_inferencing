/* This code can be used to check the microphone output
 *  Flash the code on the board and plug the analog microphone,
 *  open the serial monitor and copy the output in 'audio_test.py'
 *  The code will plot the sampled audio and create a wav file out of it
 */


#include <Audio.h>
#include <LowPower.h>

AudioClass *theAudio; 

static const int32_t buffer_size = 200;
const int32_t max_size = 64000;

//char features[max_size];
int transformed[max_size/2];

void pcm_to_int(char* pcm_bytes, int* pcm_samples, int num_samples) {
    for (int i = 0; i < num_samples; i++) {
        int16_t pcm_sample = (pcm_bytes[i*2 + 1] << 8) | pcm_bytes[i*2]; // convert from little-endian bytes to signed int
        pcm_samples[i] = (int)pcm_sample; // store as int
    }
}

void get_audio(int samples[]){

    char s_buffer[buffer_size];
    int transformed_buffer[buffer_size/2];

    Serial.print("Initialization Audio Library\n");
    theAudio->setRecorderMode(AS_SETRECDR_STS_INPUTDEVICE_MIC,100); # Gain = 100 (10dB) [range: 0-210]
    theAudio->initRecorder(AS_CODECTYPE_PCM, "/mnt/sd0/BIN", AS_SAMPLINGRATE_16000, AS_CHANNEL_MONO);

    Serial.print("Start Recording!\n");

    theAudio->startRecorder();

    uint32_t read_size=0;
    uint32_t total_size=0;
    
    while(total_size<max_size){ 
//      Serial.printf("Total: %u \n", total_size);
//      Serial.printf("Read: %u \n", read_size);
      theAudio->readFrames(s_buffer, buffer_size, &read_size);
      usleep(10);
      for (int i=0; i<read_size/2; i++){
        pcm_to_int(s_buffer, transformed_buffer, buffer_size/2);
        samples[i+total_size/2] = transformed_buffer[i];
      }
      total_size += read_size;
    }
    theAudio->stopRecorder();

    theAudio->setReadyMode();
  
    Serial.printf("End Recording\n");
}



void setup() {
  Serial.begin(115200);
  
  theAudio = AudioClass::getInstance();
  theAudio->begin();
  LowPower.clockMode(CLOCK_MODE_32MHz); 
}

void loop() {
  get_audio(transformed);
  for (int i = 0; i<max_size/2; i++){
    Serial.printf("%d, ", transformed[i]); 
  }
  Serial.printf("*END*");
  Serial.flush();
 
  delay(10000);
}
