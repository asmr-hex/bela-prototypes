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

void SamplerGui::sendSamplerSize(int n) {
  // _gui->sendBuffer(size_buffer, n);
}

void SamplerGui::sendSampleName(const char *name) {
  _gui->sendBuffer(name_output, name);
}

int* SamplerGui::check(Sampler *sampler) {
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
  }

  DataBuffer& scrubBuffer = _gui->getDataBuffer(scrubChannel);
  int* scrubData = scrubBuffer.getAsInt();
  int scrubbed = scrubData[0];
  
  // if (scrubbed) {
  //   rt_printf("scrubbing %i\n", scrubData[0]);
  // }
  
  return scrubData;
}
