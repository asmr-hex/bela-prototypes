#include <gui.h>
#include <libraries/Gui/Gui.h>
#include <Bela.h>

#include <sampler.h>


SamplerGui::SamplerGui(std::string name, Sampler *sampler) : _name(name) {
  _gui = new Gui();
  _gui->setup(_name);

  info = {0, sampler->samples[0]->size(), sampler->size()};
  
  // initialize receive buffers
  infoRequestChannel = _gui->setBuffer('d', 1);
  scrubChannel = _gui->setBuffer('d', 2);
}

void SamplerGui::update(Sampler *sampler) {
  // Check whether a new sample has been selected
  DataBuffer& buffer = _gui->getDataBuffer(infoRequestChannel);
  int* data = buffer.getAsInt();
  int selected = data[0];
  if (selected) {
    data[0] = 0;
    _gui->sendBuffer(name_output, sampler->samples[selected-1]->filename.c_str(), sampler->samples[selected-1]->filename.length());
    info[0] = selected-1;
    info[1] = sampler->samples[selected-1]->size();
    info[2] = sampler->size();
    _gui->sendBuffer(info_output, info);

    // currently, selecting a sample from the gui is the same as triggering it
    sampler->samples[selected-1]->trigger();
  }

  // check if scrubbing has occured to the current sample
  DataBuffer& scrubBuffer = _gui->getDataBuffer(scrubChannel);
  int* scrubData = scrubBuffer.getAsInt();
  int scrubbed = scrubData[0];
  if (scrubbed) {
    scrubData[0] = 0;
    sampler->samples[scrubbed-1]->seek(scrubData[1]);
  }

}
