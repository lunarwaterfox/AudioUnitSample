//
//  AudioMusicWriter.h
//  AudioUnitSample
//
//  Created by lcl on 2020/6/1.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface AudioMusicWriter : NSObject

-(void)loadPresetFile:(NSURL *)path;
-(void)write;
-(void)play;
-(void)stop;
-(void)close;

@end

NS_ASSUME_NONNULL_END
