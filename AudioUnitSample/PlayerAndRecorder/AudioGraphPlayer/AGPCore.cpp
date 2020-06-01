//
//  AUPCore.cpp
//  AudioUnitSample
//
//  Created by lagopus Alopex on 2020/5/19.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#include "AGPCore.hpp"
#include <AudioToolbox/AudioToolbox.h>
#include "AudioUnitException.hpp"

typedef struct _RIFF_t {
    char chunkID[4];
    uint32_t chunkSize;
    char format[4];
} RIFF_t;

typedef struct _FMT_t {
    char subchunk1ID[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
} FMT_t;

typedef struct _Data_t {
    char subchunk2ID[4];
    uint32_t subchunk2Size;
} Data_t;

typedef struct _WAVHeader {
    RIFF_t riff;
    FMT_t fmt;
    Data_t data;
} WAVHeader;


using namespace std;

OSStatus AGPCore::renderCallback(void* inRefCon,
                                       AudioUnitRenderActionFlags* ioActionFlags,
                                       const AudioTimeStamp* inTimeStamp,
                                       UInt32 inBusNumber,
                                       UInt32 inNumberFrames,
                                       AudioBufferList* ioData) {
    AGPCore *core = (AGPCore *)inRefCon;
    
    core->_stream.read((char *)ioData->mBuffers[0].mData, ioData->mBuffers[0].mDataByteSize);
    
    int length = (int)core->_stream.gcount();
    ioData->mBuffers[0].mDataByteSize = length;

    if (length == 0) {
        core->stop();
    }
    
    return noErr;
}

void AGPCore::initialize() {
    // 创建 AUGrpph 实例
    OSStatus status = NewAUGraph(&_aGraph);
    if (status != noErr) {
        throw AudioUnitException("Audio Graph create failed.");
    }
    
    AudioComponentDescription desc = {0};
    desc.componentType = kAudioUnitType_Output;
    desc.componentSubType = kAudioUnitSubType_RemoteIO;
    desc.componentManufacturer = kAudioUnitManufacturer_Apple;
    desc.componentFlags = 0;
    desc.componentFlagsMask = 0;
    
    status = AUGraphAddNode(_aGraph, &desc, &_ioNode);
    if (status != noErr) {
        throw AudioUnitException("Audio Graph add node failed.");
    }
    
    status = AUGraphOpen(_aGraph);
    if (status != noErr) {
        throw AudioUnitException("Audio Graph open failed.");
    }
    
    status = AUGraphNodeInfo(_aGraph, _ioNode, NULL, &_ioUnit);
    if (status != noErr) {
        throw AudioUnitException("Audio Graph get audio unit info failed.");
    }
    
    // 开启扬声器
    uint32_t flag = 1;
    status = AudioUnitSetProperty(_ioUnit, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Output, 0, &flag, sizeof(flag));
    if (status != noErr) {
        throw AudioUnitException("Audio Unit open loudspeaker failed.");
    }
    
    // 设置 callback
    AURenderCallbackStruct input;
    input.inputProc = AGPCore::renderCallback;
    input.inputProcRefCon = this;
    
    status = AUGraphSetNodeInputCallback(_aGraph, _ioNode, 0, &input);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit set callback failed.");
    }
    
    // 初始化
    status = AUGraphInitialize(_aGraph);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit Initialize failed.");
    }
}

void AGPCore::loadPCMFile(CFStringRef url) {
    // 读取 Wav header
    WAVHeader header;
    
    const char *str = CFStringGetCStringPtr(url, kCFStringEncodingUTF8);
    if (_stream.is_open()) {
        _stream.close();
    }
    
    _stream.open(str);
    _stream.read((char *)&header, sizeof(header));

    // 设置 stream format
    AudioStreamBasicDescription audioFormat = {0};
    audioFormat.mSampleRate = header.fmt.sampleRate;
    audioFormat.mFormatID = kAudioFormatLinearPCM;
    audioFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger;
    audioFormat.mFramesPerPacket = 1;
    audioFormat.mChannelsPerFrame = header.fmt.numChannels;
    audioFormat.mBitsPerChannel = header.fmt.bitsPerSample;
    audioFormat.mBytesPerFrame = audioFormat.mChannelsPerFrame * audioFormat.mBitsPerChannel / 8;
    audioFormat.mBytesPerPacket = audioFormat.mBytesPerFrame * audioFormat.mFramesPerPacket;

    OSStatus status = AudioUnitSetProperty(_ioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0, &audioFormat, sizeof(audioFormat));
    if (status != noErr) {
        throw AudioUnitException("Audio Unit audio format failed.");
    }
}

void AGPCore::play() const {
    OSStatus status = AUGraphStart(_aGraph);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit play failed.");
    }
}

void AGPCore::stop() {
    OSStatus status = AUGraphStop(_aGraph);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit stop failed.");
    }

    _stream.clear();
    _stream.seekg(sizeof(WAVHeader));
}

void AGPCore::pause() const {
    OSStatus status = AUGraphStop(_aGraph);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit pause failed.");
    }
}


AGPCore::~AGPCore() {
    AUGraphStop(_aGraph);
    AUGraphUninitialize(_aGraph);
    AUGraphClose(_aGraph);
    AUGraphRemoveNode(_aGraph, _ioNode);
    DisposeAUGraph(_aGraph);
    
    _stream.close();
}
