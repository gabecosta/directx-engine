#ifndef _PROGJOGOS_AUDIO_H_
#define _PROGJOGOS_AUDIO_H_

#ifdef ENGINE_EXPORTS
#define DLL __declspec( dllexport ) 
#else
#define DLL __declspec( dllimport )
#endif

#include "Sound.h"                                          
#include "Types.h"                                         
#include <XAudio2.h>                                       
#include <unordered_map>                                    
#include <string>                                           
using std::unordered_map;
using std::string;

class DLL Audio
{
private:
    IXAudio2* audioEngine;                                  
    IXAudio2MasteringVoice* masterVoice;                   
    unordered_map<uint, Sound*> soundTable;                

public:
    Audio();                                                
    ~Audio();                                              

    void Add(uint id, string filename, uint nVoices = 1);   
    void Play(uint id, bool repeat = false);                
    void Stop(uint id);                                     
    void Volume(uint id, float level);                      
    void Frequency(uint id, float level);                   
}; 

#endif