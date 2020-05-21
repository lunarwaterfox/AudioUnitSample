//
//  AudioUnitCore.hpp
//  sound
//
//  Created by lagopus Alopex on 2020/5/12.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#ifndef AudioUnitCore_hpp
#define AudioUnitCore_hpp

#include <AudioToolbox/AudioToolbox.h>

class AFPCore {
private:
    AUGraph   _aGraph;
    
    AudioUnit _ioUnit;
    AUNode    _ioNode;
    
    AudioUnit _fileUnit;
    AUNode    _fileNode;
    
    AudioFileID _audioFile;
    
    static OSStatus renderCallback(void* inRefCon,
                                   AudioUnitRenderActionFlags* ioActionFlags,
                                   const AudioTimeStamp* inTimeStamp,
                                   UInt32 inBusNumber,
                                   UInt32 inNumberFrames,
                                   AudioBufferList* ioData);
    
    void setupPlayParams();
    
public:
    ~AFPCore();
    void initialize();
    void loadPCMFile(CFURLRef url);
    void play() const;
    void stop();
    void pause() const;
};

#endif /* AudioUnitCore_hpp */
