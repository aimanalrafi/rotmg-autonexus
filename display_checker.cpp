#include <iostream>
#include <vector>
#include <CoreGraphics/CoreGraphics.h>

std::vector<CGPoint> regionPoints; // Stores the region coordinates

// Mouse event callback function
CGEventRef mouseCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon) {
    if (type == kCGEventLeftMouseDown || type == kCGEventLeftMouseUp || type == kCGEventLeftMouseDragged) {
        CGPoint location = CGEventGetLocation(event);
        if (type == kCGEventLeftMouseDown) {
            regionPoints.clear();
            regionPoints.push_back(location);
        } else if (!regionPoints.empty()) {
            regionPoints.push_back(location);
            if (type == kCGEventLeftMouseUp) {
                CGRect region = CGRectMake(std::min(regionPoints[0].x, regionPoints[1].x),
                                           std::min(regionPoints[0].y, regionPoints[1].y),
                                           std::abs(regionPoints[0].x - regionPoints[1].x),
                                           std::abs(regionPoints[0].y - regionPoints[1].y));
                std::cout << "Selected Region: (x=" << region.origin.x << ", y=" << region.origin.y
                          << ", width=" << region.size.width << ", height=" << region.size.height << ")" << std::endl;
                // Add your code to handle the selected region here
                
                regionPoints.clear();
            }
        }
    }
    return event;
}

int main() {
    CFMachPortRef eventTap;
    CGEventMask eventMask;
    CFRunLoopSourceRef runLoopSource;
    
    eventMask = CGEventMaskBit(kCGEventLeftMouseDown) |
                CGEventMaskBit(kCGEventLeftMouseUp) |
                CGEventMaskBit(kCGEventLeftMouseDragged);
    
    eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault, eventMask, mouseCallback, nullptr);
    if (!eventTap) {
        std::cerr << "Failed to create the event tap." << std::endl;
        return 1;
    }
    
    runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    
    std::cout << "Click the top-left and bottom-right corners of the region." << std::endl;
    
    CFRunLoopRun();
    
    CFRelease(runLoopSource);
    CFRelease(eventTap);
    
    return 0;
}
