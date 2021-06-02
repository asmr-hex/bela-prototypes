#include <gui.h>
#include <libraries/Gui/Gui.h>
#include <Bela.h>

#include <sampler.h>


SamplerGui::SamplerGui(std::string name) : _name(name) {
  _gui = new Gui();
  _gui->setup(_name);

  // initialize receive buffers
  infoRequestChannel = _gui->setBuffer('d', 1);
}

void SamplerGui::sendSamplerSize(int n) {
  // _gui->sendBuffer(size_buffer, n);
}

void SamplerGui::sendSampleName(const char *name) {
  _gui->sendBuffer(name_output, name);
}

void SamplerGui::check(Sampler *sampler) {
  DataBuffer& buffer = _gui->getDataBuffer(infoRequestChannel);
  int* data = buffer.getAsInt();
  int isTriggered = data[0];

  if (isTriggered) {
    rt_printf("YAY\n");
    data[0] = 0;
    //sendSampleName(sampler->samples[0]->filename.c_str());
    _gui->sendBuffer(name_output, sampler->samples[0]->filename.c_str(), sampler->samples[0]->filename.length());
  }
}
