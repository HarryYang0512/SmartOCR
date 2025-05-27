#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <algorithm>

// Struct must match Python ctypes structure
struct Rect {
    int x;
    int y;
    int width;
    int height;
};

// Exported C function for DLL
extern "C" __declspec(dllexport)
int detectTextRegions(const char* imagePath, Rect* outputRects, int maxRects) {
    std::vector<Rect> results;

    // Load the image
    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        return 0;
    }

    // === Preprocessing ===
    cv::Mat gray, filtered, binary, morph;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::bilateralFilter(gray, filtered, 9, 75, 75);
    cv::threshold(filtered, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

    // Dilation to merge characters and handle spacing
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 5)); // broader horizontally
    cv::dilate(binary, morph, kernel);

    // === Contour Detection ===
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(morph, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        cv::Rect r = cv::boundingRect(contour);
        double aspect = static_cast<double>(r.width) / r.height;

        // Loosened filtering for general OCR
        if (r.area() > 300 && r.area() < 50000 &&
            r.width > 10 && r.height > 10 &&
            aspect > 0.3 && aspect < 20.0) {

            // Add padding
            int pad_x = 10, pad_y = 5;
            int x = std::max(r.x - pad_x, 0);
            int y = std::max(r.y - pad_y, 0);
            int w = std::min(r.width + 2 * pad_x, image.cols - x);
            int h = std::min(r.height + 2 * pad_y, image.rows - y);
            results.push_back({x, y, w, h});
        }
    }

    // === Debug: Save boxed output ===
    cv::Mat debug = image.clone();
    for (const auto& r : results) {
        cv::rectangle(debug, cv::Rect(r.x, r.y, r.width, r.height), cv::Scalar(0, 255, 0), 2);
    }
    cv::imwrite("../output/boxed_output.jpg", debug);
    cv::imwrite("../output/morph_debug.jpg", morph);

    // === Fill results into C-style array ===
    int count = std::min(static_cast<int>(results.size()), maxRects);
    for (int i = 0; i < count; ++i) {
        outputRects[i] = results[i];
    }

    return count;
}