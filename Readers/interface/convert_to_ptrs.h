#ifndef TallinnNtupleProducer_Readers_convert_to_ptrs_h
#define TallinnNtupleProducer_Readers_convert_to_ptrs_h

/**
 * @brief Auxiliary function to convert std::vector<Particle> to std::vector<const Particle*>, 
 * @return std::vector of const pointers to particles in collection given as function argument
 */
template <typename T> 
std::vector<const T*>
convert_to_ptrs(const std::vector<T> & particles)
{
  std::vector<const T *> particle_ptrs;
  for(const T & particle: particles)
  {
    particle_ptrs.push_back(&particle);
  }
  return particle_ptrs;
}

#endif // TallinnNtupleProducer_Readers_convert_to_ptrs_h
