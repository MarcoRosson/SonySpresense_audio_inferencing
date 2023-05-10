# Sony Spresense Audio Inferencing

Welcome to this GitHub repository for audio inference on Sony Spresense boards. \
The purpose of this project is to provide a codebase that allows users to perform audio inference on recordings made with the microphone of the Spresense board, with the aim of bird sound recognition. The work makes use of *Edge Impulse*, a platform that enables Artificial Intelligence on edge devices, with a simple and user friendly interface. In particular, the project is based on [Performance Calibration: Bird sound classifier](https://studio.edgeimpulse.com/public/16060/latest), a simple a simple neural network model that allows the recognition of 2 bird species. The code is deployed in Arduino.

## Requirements
- **Sony Spresense** board
- Spresense extension board
- Analog microphone (e.g. electret microphone)

## To run the code
In order to make the code run on your the board it is needed to: \
1. Download the repository from **GitHub** with:
```console
git clone https://github.com/MarcoRosson/SonySpresense_audio_inferencing.git
```
2. Download **Arduino IDE**
3. Follow the guide to [Install the Spresense Arduino Library](https://developer.sony.com/develop/spresense/docs/arduino_set_up_en.html#_install_spresense_arduino_library) 
4. Include the *ei-performance-calibration_-bird-sound-classifier-arduino-1.0.14.zip* library in **Sketch > Include Library > Add . ZIP Library**
5. Select the board in **Tools > Board**
6. Change the setting in **Tools > Memory** to 896 KB
7. Insert the SD card in the extension board
8. Connect the board to the PC and flash the code '*SonySpresense_audio_inferencing.ino*' with the Arduino command. This may take some time, since the Spresense SDK needs to be loaded
9. Plug the microphone [following the guide](https://developer.sony.com/develop/spresense/tutorials-sample-projects/spresense-tutorials/using-multiple-microphone-inputs-with-spresense) (Just the left microphone is needed)

The audio inference should now be running. In order to check the results, unplug the device from power source, extract the SD and read it, the results should be stored in *"./predictions/predictions.txt"*

## To customize the project
The proposed Neural Network s just an example of what the system can do. In order to run the inference with your specifications, you can use the platform of [**Edge Impulse**](https://www.edgeimpulse.com/) to modify an existing project, create a new one or use a custom pretrained network, with the new function BYOM. \
Once the project is ready, go to the **Deployment** tab and export it as *Arduino Library*. Now you can import it in the project, in place of the existing one, without any modifications (only if you changed the paramteter '*Impulse Design>Create impulse>Time series data>Window size*', you may need to adapt the constant **MAX_SIZE** accordingly).

## Demo Code
### test_audio
This code can be used to check the microphone output:
 *  Flash the code on the board and plug the analog microphone,
 *  Open the serial monitor and copy the output in 'audio_test.py'
 *  The code will plot the sampled audio and create a wav file out of it

### test_inference
This code can be used to test the inference on the board: once the code is flahed on the board, inference will be performed alternatively on two pre-recorded audio samples and the result will be printed on terminal