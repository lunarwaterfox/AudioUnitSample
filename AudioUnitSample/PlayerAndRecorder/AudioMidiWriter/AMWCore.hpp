//
//  AMWCore.hpp
//  AudioUnitSample
//
//  Created by lcl on 2020/6/1.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#ifndef AMWCore_hpp
#define AMWCore_hpp

#include <AudioToolbox/AudioToolbox.h>

class AMWCore {
    AUGraph _aGraph;
    
    AudioUnit _ioUnit;
    AUNode    _ioNode;
    
    AudioUnit _samplerUnit;
    AUNode    _samplerNode;
    
public:
    ~AMWCore();
    void initialize();
    void loadPresetFile(CFURLRef url);
    void write();
    void close();
    void play();
    void stop();
};

#endif /* AMWCore_hpp */
