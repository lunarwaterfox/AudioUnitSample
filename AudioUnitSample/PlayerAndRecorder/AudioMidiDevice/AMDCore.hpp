//
//  AMDCore.hpp
//  AudioUnitSample
//
//  Created by lagopus Alopex on 2020/5/25.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#ifndef AMDCore_hpp
#define AMDCore_hpp

#include <AudioToolbox/AudioToolbox.h>

class AMDCore {
private:
    AUGraph _aGraph;
    
    AudioUnit _ioUnit;
    AUNode    _ioNode;
    
    AudioUnit _samplerUnit;
    AUNode    _samplerNode;
    
public:
    ~AMDCore();
    void initialize();
};

#endif /* AMDCore_hpp */
