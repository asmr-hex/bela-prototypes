/**
 *  sample.cpp
 */
#include <sampler.h>

Sampler::Sampler() {
  init_fill_buffers_task();
}

// NOTE: this should happen not in the audio thread.
void Sampler::load(std::string filename) {
  samples.push_back( std::unique_ptr<Sample>(new Sample(filename)) );
}

void Sampler::process_next_samples() {
  for (auto const& s : samples) {
    s->advance();
  }
}

float Sampler::read(int sample_idx, int channel) {
  return samples[sample_idx]->read(channel);
}

int Sampler::size() {
  return samples.size();
}

void Sampler::schedule_fill_buffers() {
  Bela_scheduleAuxiliaryTask(fill_buffers_task);
}

void Sampler::fill_buffers() {
  for (auto const& s : samples) {
    if (s->buffer_needs_filling())
      s->fill_buffer();
  }
}

void Sampler::init_fill_buffers_task() {
  fill_buffers_task =
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
