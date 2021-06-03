/***** sample.cpp *****/
#include <sample.h>
#include <libraries/AudioFile/AudioFile.h>


Sample::Sample(std::string filename) {
  load(filename);
}

void Sample::load(std::string filename) {
  is_loading = true;
  
  this->filename = filename;
  
  total_frames = AudioFileUtilities::getNumFrames(filename);
  
  if (total_frames <= 0)
    throw "Empty file!";

  if (total_frames <= buffer_len)
    throw "Sample must be larger than buffer!";

  // always initialize start and end
  prev_start = 0;
  start = 0;
  end = total_frames;
  
  // initialize buffers
  buffer[0] = AudioFileUtilities::load(filename, buffer_len, 0);
  buffer[1] = buffer[0];
  buffer[2] = buffer[0];

  must_fill_next_buffer = false;
  is_loading = false;
}

void Sample::trigger() {
  status = Playing;
  
  forceable_seek(0, true);
}

void Sample::advance() {
  // do not advance if we are stopped!
  if (status == Stopped) return;

  // increment the buffer read pointer
  if (++buffer_read_ptr >= buffer_len) {

    // shucks.
    if (must_fill_next_buffer)
      rt_printf("Couldn't load next buffer in time :( -- try increasing buffer size!)\n");

    buffer_read_ptr = 0;
    active_buffer = !active_buffer;
    must_fill_next_buffer = true;
  }
}

float Sample::read(int channel) {
  if (status == Stopped)
    return 0.0;
  
  return buffer[active_buffer][channel%buffer[0].size()][buffer_read_ptr];
}

void Sample::seek(int frame) {
  forceable_seek(frame, false);
}

void Sample::forceable_seek(int frame, bool force) {
  start = frame;

  if (prev_start != start || force) {
    restart = true;
    must_fill_next_buffer_asap = true;
    must_fill_next_buffer = true; 
  }
}

int Sample::get_buffer_len() {
  return buffer_len;
}

int Sample::size() {
  return total_frames;
}

bool Sample::buffer_needs_filling() {
  return (status == Playing) && (must_fill_next_buffer || must_fill_next_buffer_asap);
}

void Sample::fill_buffer() {
  if (is_loading) return;

  if (start != prev_start || restart) {
    // seek location has changed (REFACTOR THIS)
    restart = false;
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

    // TODO impl loop mode....but for now, if we reach the end of file, set
    // status to Stopped
    status = Stopped;
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

  if (must_fill_next_buffer_asap) {
    buffer_read_ptr = 0;
    active_buffer = !active_buffer;
    must_fill_next_buffer_asap = false;
  } else {
    must_fill_next_buffer = false;  
  }
}
