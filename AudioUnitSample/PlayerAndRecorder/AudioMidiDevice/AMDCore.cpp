//
//  AMDCore.cpp
//  AudioUnitSample
//
//  Created by lagopus Alopex on 2020/5/25.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#include "AMDCore.hpp"
#include "AudioUnitException.hpp"

void AMDCore::initialize() {
    // 启动 AUGraph
    OSStatus status = NewAUGraph(&_aGraph);
    if (status != noErr) {
        throw AudioUnitException("Audio Graph create failed.");
    }
    
    AudioComponentDescription isDesc = {0};
    isDesc.componentType = kAudioUnitType_Output;
    isDesc.componentSubType = kAudioUnitSubType_RemoteIO;
    isDesc.componentManufacturer = kAudioUnitManufacturer_Apple;
    isDesc.componentFlags = 0;
    isDesc.componentFlagsMask = 0;
    
    status = AUGraphAddNode(_aGraph, &isDesc, &_ioNode);
    if (status != noErr) {
        throw AudioUnitException("Audio Graph add io node failed.");
    }
    
    AudioComponentDescription fileDesc = {0};
    fileDesc.componentType = kAudioUnitType_MusicDevice;
    fileDesc.componentSubType = kAudioUnitSubType_Sampler;
    fileDesc.componentManufacturer = kAudioUnitManufacturer_Apple;
    fileDesc.componentFlags = 0;
    fileDesc.componentFlagsMask = 0;
    
    status = AUGraphAddNode(_aGraph, &fileDesc, &_samplerNode);
    if (status != noErr) {
        throw AudioUnitException("Audio Graph add file node failed.");
    }
    
    status = AUGraphOpen(_aGraph);
    if (status != noErr) {
        throw AudioUnitException("Audio Graph open failed.");
    }
    
    // 设置 unit
    status = AUGraphNodeInfo(_aGraph, _ioNode, NULL, &_ioUnit);
    if (status != noErr) {
        throw AudioUnitException("Audio Graph get io unit info failed.");
    }
    
    status = AUGraphNodeInfo(_aGraph, _samplerNode, NULL, &_samplerUnit);
    if (status != noErr) {
        throw AudioUnitException("Audio Graph get file unit info failed.");
    }
    
    // 开启扬声器
    uint32_t flag = 1;
    status = AudioUnitSetProperty(_ioUnit, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Output, 0, &flag, sizeof(flag));
    if (status != noErr) {
        throw AudioUnitException("Audio Unit open loudspeaker failed.");
    }
    
    status = AUGraphConnectNodeInput(_aGraph, _samplerNode, 0, _ioNode, 0);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit connect failed.");
    }
    
    //
    status = AudioUnitInitialize(_ioUnit);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit Initialize failed.");
    }
    
    Float64 sampleRate = 44100;
    UInt32 sampleRateSize = sizeof(sampleRate);
    
    status = AudioUnitSetProperty(_ioUnit, kAudioUnitProperty_SampleRate, kAudioUnitScope_Output, 0, &sampleRate, sampleRateSize);
    if (status != noErr) {
       throw AudioUnitException("Audio Unit set sample rate failed.");
    }
    
    UInt32 framesPerSlice = 0;
    UInt32 framesPerSlicePropertySize = sizeof(framesPerSlice);
    
    status = AudioUnitGetProperty(_ioUnit, kAudioUnitProperty_MaximumFramesPerSlice, kAudioUnitScope_Global, 0, &framesPerSlice, &framesPerSlicePropertySize);
    if (status != noErr) {
       throw AudioUnitException("Audio Unit set slice frame failed.");
    }
    
    status = AudioUnitSetProperty(_samplerUnit, kAudioUnitProperty_SampleRate, kAudioUnitScope_Output, 0, &sampleRate, sampleRateSize);
    if (status != noErr) {
       throw AudioUnitException("Audio Unit set sample rate failed.");
    }
    
    status = AudioUnitSetProperty(_samplerUnit, kAudioUnitProperty_MaximumFramesPerSlice, kAudioUnitScope_Global, 0, &framesPerSlice, framesPerSlicePropertySize);
    if (status != noErr) {
       throw AudioUnitException("Audio Unit set slice frame failed.");
    }
    
    
    status = AUGraphInitialize(_aGraph);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit Initialize failed.");
    }
    
    status = AUGraphStart(_aGraph);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit Initialize failed.");
    }
}



void AMDCore::loadPresetFile(CFURLRef url) {
    CFDataRef propertyResourceData = 0;
    SInt32 errorCode = 0;
    OSStatus result = noErr;
       
    Boolean status = CFURLCreateDataAndPropertiesFromResource(kCFAllocatorDefault, url, &propertyResourceData, NULL, NULL, &errorCode);
    if (status == true && propertyResourceData != 0) {
        
    }
       
    CFPropertyListFormat dataFormat;
    CFErrorRef errorRef = 0;
    CFPropertyListRef presetPropertyList = CFPropertyListCreateWithData(kCFAllocatorDefault, propertyResourceData, kCFPropertyListImmutable, &dataFormat, &errorRef);

    if (presetPropertyList != 0) {
           result = AudioUnitSetProperty(_samplerUnit, kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global, 0, &presetPropertyList, sizeof(CFPropertyListRef));
           CFRelease(presetPropertyList);
    }

    if (errorRef) {
        CFRelease(errorRef);
    }
        
    CFRelease (propertyResourceData);
}

void AMDCore::play() {
    UInt32 noteNum = 48;
    UInt32 onVelocity = 127;
    UInt32 noteCommand = 9 << 4 | 0;
        
    OSStatus result = MusicDeviceMIDIEvent(_samplerUnit, noteCommand, noteNum, onVelocity, 0);
}

void AMDCore::stop() {
    UInt32 noteNum = 48;
    UInt32 noteCommand = 8 << 4 | 0;
        
    OSStatus result = MusicDeviceMIDIEvent(_samplerUnit, noteCommand, noteNum, 0, 0);
}



AMDCore::~AMDCore() {
    
}
