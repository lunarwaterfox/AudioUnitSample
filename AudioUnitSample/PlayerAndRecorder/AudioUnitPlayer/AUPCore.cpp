//
//  AUPCore.cpp
//  AudioUnitSample
//
//  Created by lagopus Alopex on 2020/5/19.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#include "AUPCore.hpp"
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

OSStatus AUPCore::renderCallback(void* inRefCon,
                                       AudioUnitRenderActionFlags* ioActionFlags,
                                       const AudioTimeStamp* inTimeStamp,
                                       UInt32 inBusNumber,
                                       UInt32 inNumberFrames,
                                       AudioBufferList* ioData) {
    AUPCore *core = (AUPCore *)inRefCon;
    
    core->_stream.read((char *)ioData->mBuffers[0].mData, ioData->mBuffers[0].mDataByteSize);
    
    int length = (int)core->_stream.gcount();
    ioData->mBuffers[0].mDataByteSize = length;

    if (length == 0) {
        core->stop();
    }
    
    return noErr;
}

void AUPCore::initialize() {
    // 创建 AudioUnit实例
    AudioComponentDescription desc = {0};
    desc.componentType = kAudioUnitType_Output;
    desc.componentSubType = kAudioUnitSubType_RemoteIO;
    desc.componentManufacturer = kAudioUnitManufacturer_Apple;
    desc.componentFlags = 0;
    desc.componentFlagsMask = 0;

    AudioComponent ioComponent = AudioComponentFindNext(NULL, &desc);
    if (!ioComponent) {
        throw AudioUnitException("Find Audio Component failed.");
    }

    OSStatus status = AudioComponentInstanceNew(ioComponent, &_ioUnit);
    if (status != noErr) {
        throw AudioUnitException("Audio Component create instance failed.");
    }
    
    // 开启扬声器
    uint32_t flag = 1;
    status = AudioUnitSetProperty(_ioUnit, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Output, 0, &flag, sizeof(flag));
    if (status != noErr) {
        throw AudioUnitException("Audio Unit open loudspeaker failed.");
    }
    
    // 设置 callback
    AURenderCallbackStruct input;
    input.inputProc = AUPCore::renderCallback;
    input.inputProcRefCon = this;

    status = AudioUnitSetProperty(_ioUnit, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Input, 0, &input, sizeof(input));
    if (status != noErr) {
        throw AudioUnitException("Audio Unit set callback failed.");
    }
    
    // 初始化
    status = AudioUnitInitialize(_ioUnit);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit initialize failed.");
    }
}

void AUPCore::loadPCMFile(CFStringRef url) {
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

void AUPCore::play() const {
    OSStatus status = AudioOutputUnitStart(_ioUnit);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit play failed.");
    }
}

void AUPCore::stop() {
    OSStatus status = AudioOutputUnitStop(_ioUnit);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit stop failed.");
    }

    _stream.clear();
    _stream.seekg(sizeof(WAVHeader));
}

void AUPCore::pause() const {
    OSStatus status = AudioOutputUnitStop(_ioUnit);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit pause failed.");
    }
}


AUPCore::~AUPCore() {
    AudioOutputUnitStop(_ioUnit);
    AudioUnitUninitialize(_ioUnit);
    
    _stream.close();
}
