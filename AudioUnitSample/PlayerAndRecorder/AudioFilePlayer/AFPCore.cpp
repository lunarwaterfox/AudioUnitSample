//
//  AudioUnitCore.cpp
//  sound
//
//  Created by lagopus Alopex on 2020/5/12.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#include "AFPCore.hpp"
#include "AudioUnitException.hpp"
#include <AudioToolbox/AudioToolbox.h>
#include <fstream>

using namespace std;

void AFPCore::initialize() {
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
    fileDesc.componentType = kAudioUnitType_Generator;
    fileDesc.componentSubType = kAudioUnitSubType_AudioFilePlayer;
    fileDesc.componentManufacturer = kAudioUnitManufacturer_Apple;
    fileDesc.componentFlags = 0;
    fileDesc.componentFlagsMask = 0;
    
    status = AUGraphAddNode(_aGraph, &fileDesc, &_fileNode);
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
    
    status = AUGraphNodeInfo(_aGraph, _fileNode, NULL, &_fileUnit);
    if (status != noErr) {
        throw AudioUnitException("Audio Graph get file unit info failed.");
    }
    
    // 开启扬声器
    uint32_t flag = 1;
    status = AudioUnitSetProperty(_ioUnit, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Output, 0, &flag, sizeof(flag));
    if (status != noErr) {
        throw AudioUnitException("Audio Unit open loudspeaker failed.");
    }
    
    status = AUGraphConnectNodeInput(_aGraph, _fileNode, 0, _ioNode, 0);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit connect failed.");
    }
    
    status = AUGraphInitialize(_aGraph);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit Initialize failed.");
    }
}

void AFPCore::loadPCMFile(CFURLRef url) {
    // 打开 audio file
    OSStatus status = AudioFileOpenURL(url, kAudioFileReadPermission, 0, &_audioFile);
    if (status != noErr) {
        throw AudioUnitException("Audio file open failed.");
    }
    
    AudioStreamBasicDescription fileDataFormat;
    UInt32 size = sizeof(fileDataFormat);
    status = AudioFileGetProperty(_audioFile, kAudioFilePropertyDataFormat, &size, &fileDataFormat);
    if (status != noErr) {
        throw AudioUnitException("Audio file get property failed.");
    }
    
    // 设置 audio unit
    status = AudioUnitSetProperty(_fileUnit, kAudioUnitProperty_ScheduledFileIDs, kAudioUnitScope_Global, 0, &_audioFile, sizeof(_audioFile));
    if (status != noErr) {
        throw AudioUnitException("Audio unit setup failed.");
    }
    
    setupPlayParams();
}

void AFPCore::setupPlayParams() {
    ScheduledAudioFileRegion playRegion;
    playRegion.mTimeStamp.mFlags = kAudioTimeStampSampleTimeValid;
    playRegion.mTimeStamp.mSampleTime = 0;
    playRegion.mCompletionProc = NULL;
    playRegion.mCompletionProcUserData = NULL;
    playRegion.mAudioFile = _audioFile;
    playRegion.mLoopCount = 0;
    playRegion.mStartFrame = 0;
    playRegion.mFramesToPlay = UInt32(-1);
     
    OSStatus status = AudioUnitSetProperty(_fileUnit, kAudioUnitProperty_ScheduledFileRegion, kAudioUnitScope_Global, 0, &playRegion, sizeof(playRegion));
    if (status != noErr) {
        throw AudioUnitException("Audio unit setup failed.");
    }
    
    UInt32 primeFrames = 0;
    status = AudioUnitSetProperty(_fileUnit, kAudioUnitProperty_ScheduledFilePrime, kAudioUnitScope_Global, 0, &primeFrames, sizeof(primeFrames));
    if (status != noErr) {
        throw AudioUnitException("Audio unit setup failed.");
    }
    
    AudioTimeStamp startTime;
    startTime.mFlags = kAudioTimeStampSampleTimeValid;
    startTime.mSampleTime = -1;
    status = AudioUnitSetProperty(_fileUnit, kAudioUnitProperty_ScheduleStartTimeStamp, kAudioUnitScope_Global, 0, &startTime, sizeof(startTime));
    if (status != noErr) {
        throw AudioUnitException("Audio unit setup failed.");
    }
}

void AFPCore::play() const {
    OSStatus status = AUGraphStart(_aGraph);
    if (status != noErr) {
        throw AudioUnitException("Audio unit play failed.");
    }
}

void AFPCore::stop() {
    OSStatus status = AUGraphStop(_aGraph);
    if (status != noErr) {
        throw AudioUnitException("Audio unit stop failed.");
    }
    
    AudioUnitReset(_fileUnit, kAudioUnitScope_Global, 0);
    setupPlayParams();
}

void AFPCore::pause() const {
    OSStatus status = AUGraphStop(_aGraph);
    if (status != noErr) {
        throw AudioUnitException("Audio unit pause failed.");
    }
}


AFPCore::~AFPCore() {
    AUGraphStop(_aGraph);
    AUGraphUninitialize(_aGraph);
    AUGraphClose(_aGraph);
    AUGraphRemoveNode(_aGraph, _ioNode);
    AUGraphRemoveNode(_aGraph, _fileNode);
    DisposeAUGraph(_aGraph);
}
