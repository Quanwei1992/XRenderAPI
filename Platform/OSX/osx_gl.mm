#import <Cocoa/Cocoa.h>
#import "AppDelegate.h"

int main(int agrc,char** argv)
{
    @autoreleasepool {
        NSApplication *application = [NSApplication sharedApplication];
        MyApplicationDelegate *appDelegate = [[MyApplicationDelegate alloc] init];
        id window = [[[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 200, 200)
        styleMask:NSTitledWindowMask backing:NSBackingStoreBuffered defer:NO]
            autorelease];
        [window cascadeTopLeftFromPoint:NSMakePoint(20,20)];
        [window setTitle:@"XRenderAPI"];
        [window makeKeyAndOrderFront:nil];
        [application setDelegate:appDelegate];
        [application run];  
    }
    return EXIT_SUCCESS;
}