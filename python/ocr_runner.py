import ctypes
from ctypes import Structure, c_int, POINTER
import cv2
import pytesseract
pytesseract.pytesseract.tesseract_cmd = r"C:\Program Files\Tesseract-OCR\tesseract.exe"
import os
from nlp_postprocess import clean_ocr_text, spell_correct

# Define the same Rect struct used in C++
class Rect(Structure):
    _fields_ = [("x", c_int), ("y", c_int), ("width", c_int), ("height", c_int)]

# Load your compiled DLL
dll_path = os.path.abspath("../build/bin/Release/preprocess.dll")
ocr_lib = ctypes.CDLL(dll_path)

# Setup function signature
ocr_lib.detectTextRegions.argtypes = [ctypes.c_char_p, POINTER(Rect), c_int]
ocr_lib.detectTextRegions.restype = c_int

# Image to process
image_path = b"../test_images/sample.jpg"

# Allocate output buffer
max_boxes = 100
BoxArray = Rect * max_boxes
output_boxes = BoxArray()

# Call the C++ function
num_detected = ocr_lib.detectTextRegions(image_path, output_boxes, max_boxes)

# Load image and process each detected region
image = cv2.imread(image_path.decode())
results = []

for i in range(num_detected):
    rect = output_boxes[i]
    x, y, w, h = rect.x, rect.y, rect.width, rect.height
    roi = image[y:y+h, x:x+w]
    raw_text = pytesseract.image_to_string(roi, config="--psm 7")
    text = spell_correct(clean_ocr_text(raw_text))
    results.append((x, y, w, h, text.strip()))

# Output results
for i, (x, y, w, h, text) in enumerate(results):
    print(f"[{i+1}] Box({x},{y},{w},{h}) → '{text}'")