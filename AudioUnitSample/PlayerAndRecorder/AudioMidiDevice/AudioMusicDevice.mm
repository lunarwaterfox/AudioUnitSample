//
//  AudioMusicDevice.m
//  AudioUnitSample
//
//  Created by lagopus Alopex on 2020/5/25.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#import "AudioMusicDevice.h"
#import "AMDCore.hpp"
#import "AudioUnitException.hpp"

@implementation AudioMusicDevice {
    AMDCore *_core;
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
    _core = new AMDCore();
    
    try {
        _core->initialize();
    } catch (AudioUnitException e) {
        NSLog(@"Audio Unit Error: %s", e.what());
    }
}

-(void)loadPresetFile:(NSURL *)path {
    try {
        _core->loadPresetFile((__bridge CFURLRef)path);
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

- (void)stop {
    try {
         _core->stop();
    } catch (AudioUnitException e) {
        NSLog(@"Audio Unit Error: %s", e.what());
    }
}

@end
