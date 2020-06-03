//
//  AMWViewController.m
//  AudioUnitSample
//
//  Created by lcl on 2020/6/1.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#import "AMWViewController.h"
#import "AudioMusicWriter.h"
#import <AVFoundation/AVFoundation.h>

@interface AMWViewController ()

@property (nonatomic, strong) AudioMusicWriter *writer;

@end

@implementation AMWViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    AVAudioSession* audioSession = [AVAudioSession sharedInstance];
    [audioSession setCategory:AVAudioSessionCategoryPlayback error:nil];
    [audioSession setPreferredSampleRate: 44100 error:nil];
    [audioSession setPreferredIOBufferDuration: 0.02 error:nil];
    [audioSession setActive:YES error:nil];
    
    _writer = [[AudioMusicWriter alloc] init];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (IBAction)ready:(id)sender {
    NSURL *url = [[NSBundle mainBundle] URLForResource:@"Vibraphone" withExtension:@"aupreset"];
    [_writer loadPresetFile:url];
}

- (IBAction)write:(id)sender {
    [_writer write];
}

- (IBAction)stop:(id)sender {
    [_writer stop];
}

- (IBAction)play:(id)sender {
    [_writer play];
}

- (IBAction)close:(id)sender {
    [_writer close];
}


@end
