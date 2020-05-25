//
//  AudioMusicDevice.h
//  AudioUnitSample
//
//  Created by lagopus Alopex on 2020/5/25.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface AudioMusicDevice : NSObject

-(void)loadPresetFile:(NSURL *)path;
-(void)play;
-(void)stop;

@end

NS_ASSUME_NONNULL_END
