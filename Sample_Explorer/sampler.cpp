/**
 *  sample.cpp
 */
#include <sampler.h>

Sampler::Sampler() {

  init_fill_buffers_task();
}

// NOTE: this should happen not in the audio thread.
void Sampler::load(std::string filename) {
  samplers.push_back( std::unique_ptr<Sample>(new Sample(filename)) );
}

void fill_buffers() {
  for (auto const& s : samples) {
    s->fill_buffer();
  }
}

void Sample::init_fill_buffers_task() {
  fill_buffer_task =
    Bela_createAuxiliaryTask([] (void *user_data) {
                               Sampler *this_sampler = static_cast<Sampler *>(user_data);
                               this_sampler->fill_buffers();
                             },
      fill_task_priority,
      "fill_buffers",
      this);
    
  if (fill_buffers_task == 0)
    throw "Unable to schedule auxiliary fill buffer task!";
}
