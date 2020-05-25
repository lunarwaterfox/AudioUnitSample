//
//  AMDViewController.m
//  AudioUnitSample
//
//  Created by lagopus Alopex on 2020/5/25.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#import "AMDViewController.h"
#import "AudioMusicDevice.h"
#import <AVFoundation/AVFoundation.h>

@interface AMDViewController ()

@property (nonatomic, strong) AudioMusicDevice *player;

@end

@implementation AMDViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    AVAudioSession* audioSession = [AVAudioSession sharedInstance];
    [audioSession setCategory:AVAudioSessionCategoryPlayback error:nil];
    [audioSession setPreferredSampleRate: 44100 error:nil];
    [audioSession setPreferredIOBufferDuration: 0.02 error:nil];
    [audioSession setActive:YES error:nil];
    
    _player = [[AudioMusicDevice alloc] init];
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
    [_player loadPresetFile:url];
}

- (IBAction)start:(id)sender {
    [_player play];
}

- (IBAction)stop:(id)sender {
    [_player stop];
}

@end
