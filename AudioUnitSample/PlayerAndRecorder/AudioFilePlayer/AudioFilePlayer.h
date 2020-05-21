//
//  AudioUnitPlayer.h
//  sound
//
//  Created by lagopus Alopex on 2020/5/12.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface AudioFilePlayer : NSObject

-(void)loadPCMFileWithURL:(NSURL *)path;
-(void)play;
-(void)pause;
-(void)stop;

@end

NS_ASSUME_NONNULL_END
