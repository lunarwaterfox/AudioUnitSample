//
//  AudioUnitException.hpp
//  AudioUnitSample
//
//  Created by lagopus Alopex on 2020/5/20.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#ifndef AudioUnitException_hpp
#define AudioUnitException_hpp

#include <exception>

using namespace std;

class AudioUnitException: exception {
private:
    const char* _info;
    
public:
    AudioUnitException(const char* str);
    ~AudioUnitException();
    
    const char* what();
};


#endif /* AudioUnitException_hpp */
