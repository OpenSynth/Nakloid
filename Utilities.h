#ifndef Utilities_h
#define Utilities_h

#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "./parser/WavFormat.h"

namespace nak {
  // General
  extern enum ScoreMode{score_mode_nak, score_mode_ust, score_mode_smf} score_mode;
  extern enum PitchesMode{pitches_mode_pit, pitches_mode_lf0, pitches_mode_none} pitches_mode;
  extern std::string path_pitches;
  extern std::string path_singer;
  extern std::string path_song;
  extern unsigned long margin;
  extern unsigned char pitch_frame_length;

  // General nak
  extern std::string path_nak;

  // General ust
  extern std::string path_ust;

  // General smf
  extern short track;
  extern std::string path_smf;
  extern std::string path_lyrics;

  // Nakloid
  extern bool cache;
  extern std::string path_log;
  extern bool vowel_combining;
  extern double vowel_combining_volume;
  extern std::string path_output_nak;
  extern std::string path_output_pit;
  extern std::string path_prefix_map;

  // PitchMarker
  extern unsigned short pitch_margin;

  // BaseWavsMaker
  extern unsigned char base_wavs_lobe;
  extern bool is_normalize;

  // BaseWavsOverlapper
  extern bool compressor;
  extern double threshold_x;
  extern double threshold_y;
  extern double max_volume;

  // NoteArranger
  extern bool vowel_combining;
  extern unsigned short ms_front_edge;
  extern unsigned short ms_back_edge;
  extern bool sharpen_front;
  extern bool sharpen_back;

  // PitchArranger
  extern unsigned short ms_overshoot;
  extern double pitch_overshoot;
  extern unsigned short ms_preparation;
  extern double pitch_preparation;
  extern unsigned short ms_vibrato_offset;
  extern unsigned short ms_vibrato_width;
  extern double pitch_vibrato;
  extern double finefluctuation_deviation;
  extern bool vibrato;
  extern bool overshoot;
  extern bool preparation;
  extern bool interpolation;
  extern bool finefluctuation;

  // Nakloid.ini parser
  extern bool parse(std::string path_ini);

  // tools
  extern unsigned long ms2pos(unsigned long ms, WavFormat format);
  extern unsigned long pos2ms(unsigned long pos, WavFormat format);
  extern unsigned long tick2ms(unsigned long tick, unsigned short timebase, unsigned long tempo);

  extern std::vector<short> normalize(std::vector<short>wav, double target_rms);
  extern std::vector<short> normalize(std::vector<short>wav, double target_mean, double target_var);
  extern std::vector<short> normalize(std::vector<short>wav, short target_max, short target_min);
  extern double getRMS(std::vector<short> wav);
  extern double getMean(std::vector<short> wav);
  extern double getVar(std::vector<short> wav, double mean);

  extern std::vector<double> getHann(long len);
  extern std::vector<double> getTri(long len);
  extern std::vector<double> getLanczos(long len, unsigned short lobe);
  extern double sinc(double x);

  extern bool isVowel(std::string pron);
}

#endif

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif
