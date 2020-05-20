//
//  AUPCore.hpp
//  AudioUnitSample
//
//  Created by lagopus Alopex on 2020/5/19.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#ifndef AUPCore_hpp
#define AUPCore_hpp

#include <AudioToolbox/AudioToolbox.h>
#include <fstream>

class AGPCore {
private:
    AUGraph _aGraph;
    AudioUnit _ioUnit;
    AUNode _ioNode;
    std::ifstream _stream;
    
    static OSStatus renderCallback(void* inRefCon,
                                   AudioUnitRenderActionFlags* ioActionFlags,
                                   const AudioTimeStamp* inTimeStamp,
                                   UInt32 inBusNumber,
                                   UInt32 inNumberFrames,
                                   AudioBufferList* ioData);
    
public:
    ~AGPCore();
    void initialize();
    void loadPCMFile(CFStringRef url);
    void play() const;
    void stop();
    void pause() const;
};

#endif /* AUPCore_hpp */
