//
//  AudioUnitPlayer.m
//  AudioUnitSample
//
//  Created by lagopus Alopex on 2020/5/19.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#import "AudioUnitPlayer.h"
#import <AudioToolbox/AudioToolbox.h>
#import "AUPCore.hpp"

@implementation AudioUnitPlayer {
    AUPCore *_core;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        _core = new AUPCore();
        _core->initialize();
    }
    return self;
}

- (void)loadPCMFIleWithPath:(NSURL *)path {
    _core->loadPCMFile((__bridge CFURLRef)path);
}

- (void)play {
    _core->play();
}

- (void)pause {
    _core->pause();
}

- (void)stop {
    _core->stop();
}

- (void)dealloc {
    delete _core;
}


@end
