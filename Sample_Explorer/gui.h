/**
 * P5 GUI Prototype controls for Field Sampler.
 */

#ifndef SAMPLER_GUI
#define SAMPLER_GUI

#include <string>

#include <JSON.h>


// Forward declarations
class Gui;
class Sampler;

class SamplerGui {
public:
  SamplerGui(std::string name);
  // ~SamplerGui();

  void check(Sampler *sampler);
  
  void sendSamplerSize(int n);
  void sendSampleName(const char* name);
private:
  Gui *_gui;
  std::string _name;

  // send buffer IDs
  int name_output = 1;

  // receive buffer IDs
  int infoRequestChannel;
  
};

#endif
