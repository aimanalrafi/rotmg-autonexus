#include <iostream>
#include <chrono>
#include <thread>
#include <ApplicationServices/ApplicationServices.h>
#include <random>

// Define the region of interest coordinates
// mainly to monitor red: hp ~ 10-15%
const float regionX = 2956.21;  // Top-left x-coordinate
const float regionY = 130.609;  // Top-left y-coordinate
// mainly to monitor orange: hp ~ 30%
const float regionX1 = 3010.55;  // Top-left x-coordinate
const float regionY1 = 130.609;  // Top-left y-coordinate

// test gray area
// const float regionX1 = 3162.78;  // Top-left x-coordinate
// const float regionY1 = 86.5355;  // Top-left y-coordinate

const float regionWidth = 3;  // Width of the region
const float regionHeight = 3;  // Height of the region

// Function to generate a random integer in the range [min, max]
int randomInt(int min, int max) {
    // Create a random number engine
    std::random_device rd; // Get a random seed from the hardware
    std::mt19937 gen(rd()); // Use Mersenne Twister engine with random seed

    // Create a uniform distribution for the given range [min, max]
    std::uniform_int_distribution<int> distribution(min, max);

    // Generate and return a random integer
    return distribution(gen);
}


// http://web.archive.org/web/20100501161453/http://www.classicteck.com/rbarticles/mackeyboard.php
// Function to mimic user action
void mimicKeyPress(CGKeyCode key) {
    CGEventSourceRef eventSource = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
    CGEventRef keyDownEvent = CGEventCreateKeyboardEvent(eventSource, key, true);
    CGEventRef keyUpEvent = CGEventCreateKeyboardEvent(eventSource, key, false);

    // Post the key down event to mimic the key press
    CGEventPost(kCGHIDEventTap, keyDownEvent);

    // Sleep for the desired duration to mimic holding down the key
    // also randomised to lower probability detection
    std::this_thread::sleep_for(std::chrono::milliseconds(randomInt(200,500)));

    // Post the key up event to release the key
    CGEventPost(kCGHIDEventTap, keyUpEvent);

    // Release the events and event source
    CFRelease(keyDownEvent);
    CFRelease(keyUpEvent);
    CFRelease(eventSource);
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

    float actualRegionX1 = regionX1 * screenWidth / screenFrame.size.width;
    float actualRegionY1 = regionY1 * screenHeight / screenFrame.size.height;

    float actualRegionWidth = regionWidth * screenWidth / screenFrame.size.width;
    float actualRegionHeight = regionHeight * screenHeight / screenFrame.size.height;

    // Create a bitmap context to extract the color information
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(NULL, actualRegionWidth, actualRegionHeight, 8, actualRegionWidth * 4, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);

    // Extract the color information of the 10% region
    CGImageRef screenshot = CGDisplayCreateImageForRect(CGMainDisplayID(), CGRectMake(actualRegionX, actualRegionY, actualRegionWidth, actualRegionHeight));
    CGContextDrawImage(context, CGRectMake(0, 0, actualRegionWidth, actualRegionHeight), screenshot);

    // Extract the color information of the 30% region
    CGImageRef screenshot1 = CGDisplayCreateImageForRect(CGMainDisplayID(), CGRectMake(actualRegionX1, actualRegionY1, actualRegionWidth, actualRegionHeight));
    CGContextDrawImage(context, CGRectMake(0, 0, actualRegionWidth, actualRegionHeight), screenshot1);

    // Get the color of the top-left pixel in the region
    UInt8* pixels = (UInt8*)CGBitmapContextGetData(context);
    int pixelIndex = 0;
    UInt8 red = pixels[pixelIndex];
    UInt8 green = pixels[pixelIndex + 1];
    UInt8 blue = pixels[pixelIndex + 2];

    UInt8* pixels1 = (UInt8*)CGBitmapContextGetData(context);
    int pixelIndex1 = 0;
    UInt8 red1 = pixels1[pixelIndex1];
    UInt8 green1 = pixels1[pixelIndex1 + 1];
    UInt8 blue1 = pixels1[pixelIndex1 + 2];

    // Clean up resources
    CGContextRelease(context);
    CGImageRelease(screenshot);
    CGImageRelease(screenshot1);


    // // Print the region information
    // std::cout << "Region of Interest: X=" << actualRegionX << " Y=" << actualRegionY
    //           << " Width=" << actualRegionWidth << " Height=" << actualRegionHeight << std::endl;

    // Print the color of the region
    // std::cout << "Color of the 10% region: R=" << (int)red << " G=" << (int)green << " B=" << (int)blue << std::endl;

    // std::cout << "Color of the 30% region: R!=" << (int)red1 << " G!=" << (int)green1 << " B!=" << (int)blue1 << std::endl;


    // 10% region
    if (((red >= 162) && (green <= 75) && (blue <= 62)) || ((red == 95) && (green == 95) && (blue == 95))) {

        // Mimic user action by pressing the "R" key
        std::cout << "Triggered: Color of the 10% region: R=" << (int)red << " G=" << (int)green << " B=" << (int)blue << std::endl;

        std::cout << "Danger zone! Nexus-ing!" << std::endl;
        mimicKeyPress(15);
        
        
    }

    // 30% region
    if ((((red1 >= 213) && (green1 <= 126) && (blue1 <= 34)) && ((red1 <= 231) && (green1 >= 125) && (blue1 >= 10)))) {

        std::cout << "Triggered: Color of the 30% region: R=" << (int)red1 << " G=" << (int)green1 << " B=" << (int)blue1 << std::endl;

        std::cout << "Health low drinking HP pots!" << std::endl;
        mimicKeyPress(3);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        
    } else if(((red1 == 95) && (green1 == 95) && (blue1 == 95))) {

        std::cout << "Triggered: Color of the 30% region: R=" << (int)red1 << " G=" << (int)green1 << " B=" << (int)blue1 << std::endl;

        std::cout << "Danger zone! Nexus-ing!" << std::endl;
        mimicKeyPress(15);

    }

}
int main() {
    std::cout <<"tracking! Good luck!" << std::endl;    
    // Monitor the region continuously
    while (true) {
        monitorRegion();
        std::this_thread::sleep_for(std::chrono::milliseconds(250)); 
    }

    return 0;
}
