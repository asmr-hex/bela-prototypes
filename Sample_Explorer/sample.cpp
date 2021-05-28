/***** sample.cpp *****/
#include <sample.h>
#include <libraries/AudioFile/AudioFile.h>


Sample::Sample(std::string filename, int n_output_channels) : channel_outputs(n_output_channels, 0.0) {
  init_fill_buffer_task();
  load(filename);
}

void Sample::load(std::string filename) {
  this->filename = filename;
  
  total_frames = AudioFileUtilities::getNumFrames(filename);
  
  if (total_frames <= 0)
    throw "Empty file!";

  if (total_frames <= buffer_len)
    throw "Sample must be larger than buffer!";

  // always initialize start and end
  start = 0;
  end = total_frames;
  
  // initialize buffers
  buffer[0] = AudioFileUtilities::load(filename, buffer_len, 0);
  buffer[1] = buffer[0];
  buffer[2] = buffer[0];
}

void Sample::trigger() {
	
}

std::vector<float> Sample::next() {
  // increment the read pointer
  if (++buffer_read_ptr >= buffer_len) {

    // shucks.
    if (!done_loading_next_buffer)
      rt_printf("Couldn't load next buffer in time :( -- try increasing buffer size!)");

    done_loading_next_buffer = 0;
    buffer_read_ptr = 0;
    active_buffer = !active_buffer;

    // schedule next buffer fill task
    Bela_scheduleAuxiliaryTask(fill_buffer_task);
  }

  // copy next samples into output buffer.
  for (unsigned int channel = 0; channel < channel_outputs.size(); channel++) {
    // wrap channel index in case there are more output channels than the file has.
    channel_outputs[channel] = buffer[active_buffer][channel%buffer[0].size()][buffer_read_ptr];
  }
  
  return channel_outputs;
}

void Sample::seek(int frame) {
  start = frame;

  // TODO reset read head and buffers.
}

int Sample::get_buffer_len() {
  return buffer_len;
}

int Sample::size() {
  return total_frames;
}

void Sample::init_fill_buffer_task() {
  fill_buffer_task =
    Bela_createAuxiliaryTask([] (void *user_data) {
                               Sample *this_sample = static_cast<Sample *>(user_data);
                               this_sample->fill_buffer();
                             },
                             fill_task_priority,
                             "fill_buffer",
                             this);
    
  if (fill_buffer_task == 0)
    throw "Unable to schedule auxiliary fill buffer task!";
}

void Sample::fill_buffer() {
  static int prev_start = 0;

  if (start != prev_start) {
    // seek location has changed (REFACTOR THIS)
    prev_start = start;
    file_read_ptr = start; // set file read buffer to new location
  } else {
    file_read_ptr+=buffer_len; // otherwise just increment by the buffer length
  }

  // wrap file read head around
  if (file_read_ptr >= total_frames)
    file_read_ptr = 0;

  int last_frame = file_read_ptr + buffer_len;
  int zero_pad = 0;

  // if reaching the end of the file take note of the last frame index
  // so we can zero-pad the rest later
  if ( (file_read_ptr + buffer_len) >= (total_frames - 1) ) {
    last_frame = total_frames - 1;
    zero_pad = 1;
  }

  for (int ch = 0; ch < buffer[0].size(); ch++) {
    // fill nonactive buffer
    AudioFileUtilities::getSamples(filename,
                                   buffer[!active_buffer][ch].data(),
                                   ch,
                                   file_read_ptr,
                                   last_frame);

    // zero-pad if necessary
    if (zero_pad) {
      int n_frames_to_pad = buffer_len - (last_frame - file_read_ptr);
      for (int n = 0; n < n_frames_to_pad; n++) {
        buffer[!active_buffer][ch][n+(buffer_len-n_frames_to_pad)] = 0;
      }
    }
  }
  
  done_loading_next_buffer = 1;
}
