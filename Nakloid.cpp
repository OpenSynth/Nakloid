#include "Nakloid.h"

using namespace std;

Nakloid::Nakloid() : voice_db(0), margin(0), score(0)
{
  setDefaultFormat();
}

Nakloid::Nakloid(string path_ust) : voice_db(0), margin(0), score(0)
{
  setDefaultFormat();
  setScore(path_ust);
}

Nakloid::Nakloid(string singer, string path_smf, short track, string path_lyric, string path_song) : voice_db(0), margin(0), score(0)
{
  setDefaultFormat();
  setScore(singer, path_smf, track, path_lyric, path_song);
}

Nakloid::~Nakloid()
{
  if (voice_db != 0) {
    delete voice_db;
    voice_db = 0;
  }
  if (score != 0) {
    delete score;
    score = 0;
  }
}

void Nakloid::setDefaultFormat()
{
  format.chunkSize = 16;
  format.wFormatTag = 1;
  format.wChannels = 1;
  format.dwSamplesPerSec = 44100;
  format.dwAvgBytesPerSec = format.dwSamplesPerSec*2;
  format.wBlockAlign = 2;
  format.wBitsPerSamples = 16;
}

bool Nakloid::setScore(string path_ust)
{
  if (score != 0) {
    delete score;
    score = 0;
  }

  score = new Score(path_ust);
  if (score == 0 || !score->isScoreLoaded()) {
    cerr << "score hasn't loaded" << endl;
    return false;
  }

  voice_db = new VoiceDB(score->getSinger());

  return true;
}

bool Nakloid::setScore(string singer, string path_smf, short track, string path_lyric, string path_song)
{
  if (score != 0) {
    delete score;
    score = 0;
  }

  score = new Score(singer, path_smf, track, path_lyric, path_song);
  if (score == 0 || !score->isScoreLoaded()) {
    cerr << "score hasn't loaded" << endl;
    return false;
  }

  voice_db = new VoiceDB(score->getSinger());

  return true;
}

bool Nakloid::vocalization()
{
  if (score == 0) {
    cerr << "score hasn't loaded" << endl;
    return false;
  }

  if (voice_db == 0) {
    cerr << "can't find voiceDB" << endl;
    return false;
  }

  cout << "----- start vocalization -----" << endl << endl;
  list<Note> notes;

  // set note params from voiceDB
  notes = score->getNotesList();
  cout << endl << "loading voiceDB..." << endl << endl;
  for (list<Note>::iterator it_notes=notes.begin(); it_notes!=notes.end(); ++it_notes) {
    if (it_notes == notes.begin()) {
      unsigned short ovrl = it_notes->isOvrl()?it_notes->getOvrl():voice_db->getVoice(it_notes->getPron()).ovrl;
      if (margin < max<unsigned long>(ovrl, it_notes->getStart()))
        margin = max<unsigned long>(ovrl, it_notes->getStart());
    }
    if (!it_notes->isOvrl())
      it_notes->setOvrl(voice_db->getVoice(it_notes->getPron()).ovrl);
    if (!it_notes->isPrec())
      it_notes->setPrec(voice_db->getVoice(it_notes->getPron()).prec);
  }
  score->setNotes(notes);
  cout << endl << "load finished" << endl << endl << endl;

  // arrange note params
  cout << endl << "arrange params..." << endl << endl;
  NoteArranger::arrange(score);
  PitchArranger::arrange(score);
  cout << endl << "arrange finished" << endl << endl << endl;

  // Singing Voice Synthesis
  notes = score->getNotesList();
  BaseWavsOverlapper *overlapper = new BaseWavsOverlapper(format, score->getPitches());
  for (list<Note>::iterator it_notes=notes.begin(); it_notes!=notes.end(); ++it_notes) {
    cout << "pron: " << it_notes->getPron() << ", "
      << "pron start: " << it_notes->getPronStart() << ", "
      << "pron end: " << it_notes->getPronEnd() << endl;
    overlapper->overlapping(it_notes->getPronStart(), it_notes->getPronEnd(), voice_db->getVoice(it_notes->getPron()).bwc, it_notes->getVelocities());
  }
  overlapper->outputWav(score->getSongPath());
  delete overlapper;

  cout << "----- vocalization finished -----" << endl << endl;
  return true;
}


/*
 * accessor
 */
WavFormat Nakloid::getFormat()
{
  return format;
}

void Nakloid::setFormat(WavFormat format)
{
  this->format = format;
}

Score* Nakloid::getScore()
{
  return score;
}

void Nakloid::setMargin(long margin)
{
  this->margin = margin;
}

long Nakloid::getMargin()
{
  return this->margin;
}
