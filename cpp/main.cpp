#include <iostream>
#include <opencv2/opencv.hpp>

// Match the struct used in preprocess.cpp
struct Rect {
    int x;
    int y;
    int width;
    int height;
};

// Declare the C-style DLL function
extern "C" __declspec(dllimport)
int detectTextRegions(const char* imagePath, Rect* outputRects, int maxRects);

int main() {
    const char* path = "D:/SmartOCR/test_images/test.png";
    const int maxRects = 100;
    Rect results[maxRects];

    int count = detectTextRegions(path, results, maxRects);

    std::cout << "Detected " << count << " regions:\n";
    for (int i = 0; i < count; ++i) {
        Rect r = results[i];
        std::cout << "[" << i+1 << "] x=" << r.x << ", y=" << r.y << ", w=" << r.width << ", h=" << r.height << "\n";
    }

    return 0;
}
