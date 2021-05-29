/***** sample.h *****/
#ifndef SAMPLE_H
#define SAMPLE_H

#include <string>
#include <vector>

#include <Bela.h>


class Sample {
public:
  Sample(std::string filename);

  void load(std::string filename);
  void trigger();
  void advance();
  float read(int channel);
  void seek(int frame);
  
  std::string filename;

  int get_buffer_len();
  int size();

  void fill_buffer();
  bool buffer_needs_filling();
private:
  const int buffer_len = 22050;
  int total_frames;

  int prev_start;
  int start;
  int end;

  bool is_loading = false;
  
  // Three buffers for each channel:
  // one of them loads the next chunk of audio
  // one is used for playback
  // and one is pinned to the beginning of the sample (since triggers can't be predicted)
  std::vector<std::vector<float> > buffer[3];

  bool must_fill_next_buffer = false;
  
  int active_buffer = 0;
  
  // read pointer relative to active buffer
  int buffer_read_ptr = 0;
  
  // read pointer relative to file, incremented by buffer_len
  int file_read_ptr = 0;

  bool done_loading_next_buffer = true;

  // const int fill_task_priority = 90;
  // AuxiliaryTask fill_buffer_task;
  
  // void init_fill_buffer_task();
};

#endif
