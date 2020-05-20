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
    
    CFIndex length = CFReadStreamRead(core->_stream, (UInt8 *)ioData->mBuffers[0].mData, ioData->mBuffers[0].mDataByteSize);
    ioData->mBuffers[0].mDataByteSize = (UInt32)length;

    if (length == 0) {
        core->stop();
    }
    
    return noErr;
}

void AUPCore::initialize() {
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
    
    status = AudioUnitInitialize(_ioUnit);
    if (status != noErr) {
        throw AudioUnitException("Audio Unit initialize failed.");
    }
}

void AUPCore::loadPCMFile(CFURLRef url) {
    WAVHeader header;
    
    _stream = CFReadStreamCreateWithFile(kCFAllocatorDefault, url);
    _url = url;
    
    CFReadStreamOpen(_stream);
    CFReadStreamRead(_stream, (UInt8 *)&header, sizeof(header));

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

void AUPCore::play() const noexcept {
    AudioOutputUnitStart(_ioUnit);
}

void AUPCore::stop() noexcept {
    AudioOutputUnitStop(_ioUnit);
    CFReadStreamClose(_stream);
    
    loadPCMFile(_url);
}

void AUPCore::pause() const noexcept {
    AudioOutputUnitStop(_ioUnit);
}


AUPCore::~AUPCore() {
    AudioOutputUnitStop(_ioUnit);
    AudioUnitUninitialize(_ioUnit);
    
    CFReadStreamClose(_stream);
}
