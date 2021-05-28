/**
      ___           ___           ___           ___           ___     
     /\  \         /\  \         /\__\         /\  \         /\  \    
    |::\  \       /::\  \       /:/  /        /::\  \       /::\  \   
    |:|:\  \     /:/\:\  \     /:/  /        /:/\:\__\     /:/\:\  \  
  __|:|\:\  \   /:/ /::\  \   /:/  /  ___   /:/ /:/  /    /:/  \:\  \ 
 /::::|_\:\__\ /:/_/:/\:\__\ /:/__/  /\__\ /:/_/:/__/___ /:/__/ \:\__\
 \:\~~\  \/__/ \:\/:/  \/__/ \:\  \ /:/  / \:\/:::::/  / \:\  \ /:/  /
  \:\  \        \::/__/       \:\  /:/  /   \::/~~/~~~~   \:\  /:/  / 
   \:\  \        \:\  \        \:\/:/  /     \:\~~\        \:\/:/  /  
    \:\__\        \:\__\        \::/  /       \:\__\        \::/  /   
     \/__/         \/__/         \/__/         \/__/         \/__/    

 */

#ifndef SAMPLER_H
#define SAMPLER_H

#include <vector>
#include <memory>
#include <string>

#include <sample.h>


class Sampler {
public:
  Sampler();

  void load(std::string filename);
  void advance();
  void advance();
  float read(int sample_idx, int channel);

  int size();

  void fill_buffers();
private:
  std::vector<std::unique_ptr<Sample> > samples;

  const int fill_task_priority = 90;
  AuxiliaryTask fill_buffers_task;
  
  void init_fill_buffers_task();
}

#endif
