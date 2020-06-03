//
//  AMWCore.cpp
//  AudioUnitSample
//
//  Created by lcl on 2020/6/1.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#include "AMWCore.hpp"
#include "AudioUnitException.hpp"


void AMWCore::initialize() {
    // 启动 AUGraph
    OSStatus status = NewAUGraph(&_aGraph);
    if (status != noErr) {
        throw AudioUnitException("Audio Graph create failed.");
    }
    
    AudioComponentDescription isDesc = {0};
    isDesc.componentType = kAudioUnitType_Output;
    isDesc.componentSubType = kAudioUnitSubType_GenericOutput;
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
    
    // 设置 stream format
//    AudioStreamBasicDescription audioFormat = {0};
//    audioFormat.mSampleRate = 44100;
//    audioFormat.mFormatID = kAudioFormatLinearPCM;
//    audioFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger;
//    audioFormat.mFramesPerPacket = 1;
//    audioFormat.mChannelsPerFrame = 2;
//    audioFormat.mBitsPerChannel = 16;
//    audioFormat.mBytesPerFrame = audioFormat.mChannelsPerFrame * audioFormat.mBitsPerChannel / 8;
//    audioFormat.mBytesPerPacket = audioFormat.mBytesPerFrame * audioFormat.mFramesPerPacket;
//
//    status = AudioUnitSetProperty(_ioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0, &audioFormat, sizeof(audioFormat));
//    if (status != noErr) {
//        throw AudioUnitException("Audio Unit audio format failed.");
//    }
//    
//    status = AudioUnitSetProperty(_ioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 0, &audioFormat, sizeof(audioFormat));
//    if (status != noErr) {
//        throw AudioUnitException("Audio Unit audio format failed.");
//    }
}



void AMWCore::loadPresetFile(CFURLRef url) {
    CFReadStreamRef stream = CFReadStreamCreateWithFile(kCFAllocatorDefault, url);
    CFReadStreamOpen(stream);
    
       
    CFPropertyListFormat dataFormat;
    CFErrorRef errorRef = 0;
    CFPropertyListRef presetPropertyList = CFPropertyListCreateWithStream(kCFAllocatorDefault, stream, 0, kCFPropertyListImmutable, &dataFormat, &errorRef);

    if (presetPropertyList != 0) {
        OSStatus status = AudioUnitSetProperty(_samplerUnit, kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global, 0, &presetPropertyList, sizeof(CFPropertyListRef));
        if (status != noErr) {
            throw AudioUnitException("Audio Unit load preset failed.");
        }
        
        CFRelease(presetPropertyList);
    }

    if (errorRef) {
        CFRelease(errorRef);
    }
    
    CFReadStreamClose(stream);
    CFRelease(stream);
}

void AMWCore::play() {
    UInt32 noteNum = 48;
    UInt32 onVelocity = 127;
    UInt32 noteCommand = 9 << 4 | 0;
        
    OSStatus status = MusicDeviceMIDIEvent(_samplerUnit, noteCommand, noteNum, onVelocity, 0);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit load play failed.");
    }
}

void AMWCore::stop() {
    UInt32 noteNum = 48;
    UInt32 noteCommand = 8 << 4 | 0;
        
    OSStatus status = MusicDeviceMIDIEvent(_samplerUnit, noteCommand, noteNum, 0, 0);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit load stop failed.");
    }
}

void AMWCore::write() {
    
}

void AMWCore::close() {
    
}



AMWCore::~AMWCore() {
    AUGraphStop(_aGraph);
    AUGraphUninitialize(_aGraph);
    AUGraphClose(_aGraph);
    AUGraphRemoveNode(_aGraph, _ioNode);
    AUGraphRemoveNode(_aGraph, _samplerNode);
    DisposeAUGraph(_aGraph);
}
