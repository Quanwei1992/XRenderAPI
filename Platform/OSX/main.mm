//
//  main.m
//  osx_gl
//
//  Created by 袁全伟 on 2017/11/9.
//  Copyright © 2017年 XRenderAPI. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "AppDelegate.h"

int main(int argc, const char * argv[]) {
    
    
    [NSApplication sharedApplication];
    
    // Menu
    id menubar = [[NSMenu alloc] initWithTitle:@"XRenderAPI"];
    id appMenuItem = [[NSMenuItem alloc] init];
    [menubar addItem:appMenuItem];
    [NSApp setMainMenu:menubar];
    
    id appMenu = [[NSMenu alloc ] init];
    id quitMenuItem = [[NSMenuItem alloc] initWithTitle:@"Quit" action:@selector(terminate:) keyEquivalent:@"q"];
    [appMenu addItem: quitMenuItem];
    [appMenuItem setSubmenu:appMenu];
    
    
    
    [NSApp setDelegate: [[AppDelegate alloc ] init ]];
    [NSApp activateIgnoringOtherApps:YES];
    [NSApp run];
    
    return 0;
}
