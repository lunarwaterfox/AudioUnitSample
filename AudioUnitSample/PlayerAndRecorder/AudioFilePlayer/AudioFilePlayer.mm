//
//  AudioUnitPlayer.m
//  sound
//
//  Created by lagopus Alopex on 2020/5/12.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#import "AudioFilePlayer.h"
#import "AFPCore.hpp"
#import "AudioUnitException.hpp"

@implementation AudioFilePlayer {
    AFPCore *_core;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        [self prepareCore];
    }
    return self;
}

- (void)prepareCore {
    _core = new AFPCore();
    
    try {
        _core->initialize();
    } catch (AudioUnitException e) {
        NSLog(@"Audio Unit Error: %s", e.what());
    }
}

- (void)dealloc {
    if (_core) {
        delete _core;
    }
}

-(void)loadPCMFileWithURL:(NSURL *)path {
    try {
        _core->loadPCMFile((__bridge CFURLRef)path);
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




@end
