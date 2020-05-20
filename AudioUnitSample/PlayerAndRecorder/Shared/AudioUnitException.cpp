//
//  AudioUnitException.cpp
//  AudioUnitSample
//
//  Created by lagopus Alopex on 2020/5/20.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#include "AudioUnitException.hpp"

AudioUnitException::AudioUnitException(const char* str): _info(str) {
    
}

AudioUnitException::~AudioUnitException() {
    
}


const char* AudioUnitException::what() {
    return _info;
}
