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

#include <sampler.h>


Sampler* sampler;

Gui gui;
GuiController controller;

int clientBuffer0Id;

unsigned int gSensitivitySliderIdx;
unsigned int gStartSliderIdx;
int gStartValue = 0;

bool setup(BelaContext *context, void *userData) {
  sampler = new Sampler();
  sampler->load("samples/tape.wav");
  sampler->load("samples/glass-harp.wav");
  
  // Set up the GUI
  gui.setup(context->projectName);

  // set buffer to receive from GUI
  clientBuffer0Id = gui.setBuffer('d', 2);
  
  // initialize gui data
  // gui.sendBuffer(0, sampler->size());
  
  // and attach to it
  // controller.setup(&gui, "Controls");
  
  // Arguments: name, default value, minimum, maximum, increment
  // gSensitivitySliderIdx = controller.addSlider("Scrub Sensitivity", 4410, 0, sampler->samples[0]->get_buffer_len(), 1); 
  // gStartSliderIdx = controller.addSlider("Scrub", 0, 0, sampler->samples[0]->size(), 4410);
	
  return true;
}

void render(BelaContext *context, void *userData) {
  //We store the DataBuffer in 'buffer'
  DataBuffer& buffer = gui.getDataBuffer(clientBuffer0Id);
  // Retrieve contents of the buffer as ints
  int* data = buffer.getAsInt();
  int triggeredSample = data[0];
  int isTriggered = data[1];

  if (isTriggered) {
    sampler->samples[triggeredSample]->trigger();
    rt_printf("%i sample is triggered", triggeredSample);
  }
  
  // Access the sliders specifying the index we obtained when creating then
  // int sensitivity = static_cast<int>(controller.getSliderValue(gSensitivitySliderIdx));
	
  // update the sensitivity of the scrubber
  // controller.getSlider(gStartSliderIdx).setStep(sensitivity);
	
  // get scrubber position
  // sampler->samples[0]->seek(static_cast<int>(controller.getSliderValue(gStartSliderIdx)));

  // fill buffers if necessary
  sampler->schedule_fill_buffers();
  
  for(unsigned int n = 0; n < context->audioFrames; n++) {
    sampler->process_next_samples();
    
    for(unsigned int channel = 0; channel < context->audioOutChannels; channel++) {
      float out = 0.0;
      for (unsigned int i = 0; i < sampler->size(); i++) {
        out += sampler->read(i, n);
      }
      
      audioWrite(context, n, channel, out);
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
