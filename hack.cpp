#include <iostream>
#include <chrono>
#include <thread>
#include <ApplicationServices/ApplicationServices.h>

// Define the region of interest coordinates
const float regionX = 2956.21;  // Top-left x-coordinate
const float regionY = 130.609;  // Top-left y-coordinate
const float regionWidth = 3;  // Width of the region
const float regionHeight = 3;  // Height of the region

// Function to mimic user action by pressing the "R" key
void mimicUserAction() {
    // Create a keyboard event for the "R" key press
    CGEventRef event = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)15, true);
    // Post the keyboard event to mimic the key press
    CGEventPost(kCGHIDEventTap, event);
    // Release the event
    CFRelease(event);
}


// Function to capture and print the color of the region
void monitorRegion() {
    // Get the screen size
    CGRect screenFrame = CGDisplayBounds(CGMainDisplayID());
    float screenWidth = CGRectGetWidth(screenFrame);
    float screenHeight = CGRectGetHeight(screenFrame);

    // Calculate the actual coordinates of the region
    float actualRegionX = regionX * screenWidth / screenFrame.size.width;
    float actualRegionY = regionY * screenHeight / screenFrame.size.height;
    float actualRegionWidth = regionWidth * screenWidth / screenFrame.size.width;
    float actualRegionHeight = regionHeight * screenHeight / screenFrame.size.height;

    // Create a bitmap context to extract the color information
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(NULL, actualRegionWidth, actualRegionHeight, 8, actualRegionWidth * 4, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);

    // Extract the color information of the region
    CGImageRef screenshot = CGDisplayCreateImageForRect(CGMainDisplayID(), CGRectMake(actualRegionX, actualRegionY, actualRegionWidth, actualRegionHeight));
    CGContextDrawImage(context, CGRectMake(0, 0, actualRegionWidth, actualRegionHeight), screenshot);

    // Get the color of the top-left pixel in the region
    UInt8* pixels = (UInt8*)CGBitmapContextGetData(context);
    int pixelIndex = 0;
    UInt8 red = pixels[pixelIndex];
    UInt8 green = pixels[pixelIndex + 1];
    UInt8 blue = pixels[pixelIndex + 2];

    // Clean up resources
    CGContextRelease(context);
    CGImageRelease(screenshot);

    // // Print the region information
    // std::cout << "Region of Interest: X=" << actualRegionX << " Y=" << actualRegionY
    //           << " Width=" << actualRegionWidth << " Height=" << actualRegionHeight << std::endl;

    // Print the color of the region
    std::cout << "Color of the region: R=" << (int)red << " G=" << (int)green << " B=" << (int)blue << std::endl;

        // Check if the color matches the desired value (95, 95, 95) grey x=2971.64, y = 129.678
    // if (red == 95 && green == 95 && blue == 95) {
    //     // Mimic user action by pressing the "R" key
    //     mimicUserAction();
    // }

    if (((red >= 162) && (green <= 75) && (blue <= 62)) || (red == 231 && green == 231 && blue == 231) || (red == 95 && green == 95 && blue == 95)) {
        // Mimic user action by pressing the "R" key
        std::cout << "Danger zone! Nexus-ing!" << std::endl;
        mimicUserAction();
    }
    // (red == 205 && green == 57 && blue == 62) || (red == 162 && green == 43 && blue == 46)
}

int main() {
    // Monitor the region continuously
    while (true) {
        monitorRegion();
        std::this_thread::sleep_for(std::chrono::milliseconds(250)); // Add a 100ms delay
    }

    return 0;
}
