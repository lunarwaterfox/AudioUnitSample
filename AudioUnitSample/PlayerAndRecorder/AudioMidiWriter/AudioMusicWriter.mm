//
//  AudioMusicWriter.m
//  AudioUnitSample
//
//  Created by lcl on 2020/6/1.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#import "AudioMusicWriter.h"
#import "AudioUnitException.hpp"
#import "AMWCore.hpp"

@implementation AudioMusicWriter {
    AMWCore *_core;
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
    _core = new AMWCore();
    
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

- (void)write {
    try {
        _core->write();
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

- (void)play {
    try {
        _core->play();
    } catch (AudioUnitException e) {
        NSLog(@"Audio Unit Error: %s", e.what());
    }
}

- (void)close {
    try {
        _core->close();
    } catch (AudioUnitException e) {
        NSLog(@"Audio Unit Error: %s", e.what());
    }
}



@end
