//
//  AUPViewController.m
//  AudioUnitSample
//
//  Created by lagopus Alopex on 2020/5/18.
//  Copyright © 2020 lagopus Alopex. All rights reserved.
//

#import "AUPViewController.h"
#import "AudioUnitPlayer.h"

@interface AUPViewController ()
@property (nonatomic, strong) AudioUnitPlayer *player;
@end

@implementation AUPViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    _player = [[AudioUnitPlayer alloc] init];
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
    NSURL *path = [[NSBundle mainBundle] URLForResource:@"Butterfly" withExtension:@"wav"];
    [_player loadPCMFIleWithPath:path];
}

- (IBAction)start:(id)sender {
    [_player play];
}

- (IBAction)pause:(id)sender {
     [_player pause];
}

- (IBAction)stop:(id)sender {
    [_player stop];
}

@end
