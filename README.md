# Sony Spresense Audio Inferencing

Welcome to this GitHub repository for audio inference on Sony Spresense boards. \
The purpose of this project is to provide a codebase that allows users to perform audio inference on recordings made with the microphone of the Spresense board, with the aim of bird sound recognition. The work makes use of *Edge Impulse*, a platform that enables Artificial Intelligence on edge devices, with a simple and user friendly interface. In particular, the project is based on [Performance Calibration: Bird sound classifier](https://studio.edgeimpulse.com/public/16060/latest), a simple NN that allows the recognition of 2 bird species. The code is deployed in Arduino.

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