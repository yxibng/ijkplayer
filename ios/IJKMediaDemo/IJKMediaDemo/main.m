//
//  main.m
//  IJKMediaDemo
//
//  Created by yxibng on 2024/4/10.
//

#import <UIKit/UIKit.h>
#import "IJKAppDelegate.h"

int main(int argc, char * argv[]) {
    NSString * appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([IJKAppDelegate class]);
    }
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
