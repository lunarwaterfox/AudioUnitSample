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
#import "AudioUnitException.hpp"

@implementation AudioUnitPlayer {
    AUPCore *_core;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        [self initAUPCore];
    }
    return self;
}


- (void)initAUPCore {
    _core = new AUPCore();
    
    try {
        _core->initialize();
    } catch (AudioUnitException e) {
       NSLog(@"Audio Unit Error: %s", e.what());
    }
    
}

- (void)loadPCMFIleWithPath:(NSString *)path {
    try {
        _core->loadPCMFile((__bridge CFStringRef)path);
    } catch (AudioUnitException e) {
       NSLog(@"Audio Unit Error: %s", e.what());
    }
}

- (void)play {
    try {
         _core->play();
    } catch (AudioUnitException e) {
       NSLog(@"Audio Unit Error: %s", e.what());
    }
}

- (void)pause {
    try {
        _core->pause();
    } catch (AudioUnitException e) {
       NSLog(@"Audio Unit Error: %s", e.what());
    }
}

- (void)stop {
    try {
        _core->stop();
    } catch (AudioUnitException e) {
       NSLog(@"Audio Unit Error: %s", e.what());
    }
}

- (void)dealloc {
    delete _core;
}


@end
