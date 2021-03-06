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

#include <sampler.h>
#include <gui.h>


Sampler* sampler;
SamplerGui* gui;


bool setup(BelaContext *context, void *userData) {
  sampler = new Sampler();
  sampler->load("samples/tape.wav");
  sampler->load("samples/glass-harp.wav");
  
  // Set up the GUI
  gui = new SamplerGui(context->projectName, sampler);

  return true;
}

void render(BelaContext *context, void *userData) {
  // handle gui updates
  gui->update(sampler);

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
