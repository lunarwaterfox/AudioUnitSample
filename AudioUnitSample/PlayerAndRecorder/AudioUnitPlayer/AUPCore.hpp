//
//  AUPCore.hpp
//  AudioUnitSample
//
//  Created by lagopus Alopex on 2020/5/19.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#ifndef AUPCore_hpp
#define AUPCore_hpp

#include <AudioUnit/AudioUnit.h>

class AUPCore {
private:
    AudioUnit _ioUnit;
    
    static OSStatus renderCallback(void* inRefCon,
                                   AudioUnitRenderActionFlags* ioActionFlags,
                                   const AudioTimeStamp* inTimeStamp,
                                   UInt32 inBusNumber,
                                   UInt32 inNumberFrames,
                                   AudioBufferList* ioData);
    
public:
    ~AUPCore();
    void initialize();
    void loadPCMFile(CFStringRef path);
    void play() const noexcept;
    void stop() const noexcept;
    void pause() const noexcept;
};

#endif /* AUPCore_hpp */
