/*
      ___                        ___           ___           ___     
     /  /\           ___        /  /\         /  /\         /  /\    
    /  /::|         /__/\      /  /::\       /  /::\       /  /::\   
   /  /:|:|         \__\:\    /  /:/\:\     /  /:/\:\     /  /:/\:\  
  /  /:/|:|__       /  /::\  /  /:/  \:\   /  /::\ \:\   /  /:/  \:\ 
 /__/:/_|::::\   __/  /:/\/ /__/:/ \  \:\ /__/:/\:\_\:\ /__/:/ \__\:\
 \__\/  /~~/:/  /__/\/:/~~  \  \:\  \__\/ \__\/~|::\/:/ \  \:\ /  /:/
       /  /:/   \  \::/      \  \:\          |  |:|::/   \  \:\  /:/ 
      /  /:/     \  \:\       \  \:\         |  |:|\/     \  \:\/:/  
     /__/:/       \__\/        \  \:\        |__|:|~       \  \::/   
     \__\/                      \__\/         \__\|         \__\/   

*/
#include <vector>

#include <Bela.h>
#include <libraries/AudioFile/AudioFile.h>
#include <libraries/Gui/Gui.h>
#include <libraries/GuiController/GuiController.h>

#include <sample.h>
#include <sampler.h>

Sample* sample;

Gui gui;
GuiController controller;

unsigned int gSensitivitySliderIdx;
unsigned int gStartSliderIdx;
int gStartValue = 0;

bool setup(BelaContext *context, void *userData) {
  sample = new Sample("samples/tape.wav", context->audioOutChannels);

  // Set up the GUI
  gui.setup(context->projectName);
  // and attach to it
  controller.setup(&gui, "Controls");
  
  // Arguments: name, default value, minimum, maximum, increment
  gSensitivitySliderIdx = controller.addSlider("Scrub Sensitivity", 4410, 0, sample->get_buffer_len(), 1); 
  gStartSliderIdx = controller.addSlider("Scrub", 0, 0, sample->size(), 4410);
	
  return true;
}

void render(BelaContext *context, void *userData) {
  // Access the sliders specifying the index we obtained when creating then
  int sensitivity = static_cast<int>(controller.getSliderValue(gSensitivitySliderIdx));
	
  // update the sensitivity of the scrubber
  controller.getSlider(gStartSliderIdx).setStep(sensitivity);
	
  // get scrubber position
  sample->seek(static_cast<int>(controller.getSliderValue(gStartSliderIdx)));
  
  for(unsigned int n = 0; n < context->audioFrames; n++) {
    sample->advance();
    
    for(unsigned int channel = 0; channel < context->audioOutChannels; channel++) {
      audioWrite(context, n, channel, sample->read(n));
    }
  }
}


void cleanup(BelaContext *context, void *userData)
{
}


/**
\example sample-streamer/render.cpp

Playback of large wav files
---------------------------

When dealing with large wav files it is usually not a good idea to load the
entire file into memory. This example shows how to use two buffers to continually
load chunks of the file, thus allowing playback of very large files. While one buffer
is being used for playback the other buffer is being filled with the next chunk
of samples.

In order to do this, an AuxiliaryTask is used to load the file into the inactive
buffer without interrupting the audio thread. We can set the global variable
`gDoneLoadingBuffer` to 1 each time the buffer has finished loading, allowing us
to detect cases were the buffers havn't been filled in time. These cases can usually
be mitigated by using a larger buffer size.

Try uploading a large wav file into the project and playing it back. You will need
specify the amount of channels (`#``define NUM_CHANNELS`) and the name of the file (`gFilename`).


*/
