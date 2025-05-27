import re
from textblob import TextBlob

def clean_ocr_text(text):
    if not text.strip():
        return ""

    # 1. Remove brackets, pipes, weird OCR garbage
    text = re.sub(r"[\[\]{}|~`]", "", text)

    # 2. Fix duplicated or broken spacing (e.g., "Stuf f" → "Stuff")
    text = re.sub(r"\b(\w+)\s+(\w+)\b", r"\1\2", text)

    # 3. Normalize punctuation (basic)
    text = re.sub(r"[iI][\]\.]$", "i!", text)

    # 4. Fix multiple spaces and trim
    text = re.sub(r"\s{2,}", " ", text).strip()

    return text

def spell_correct(text):
    if not text.strip():
        return ""
    # Avoid numeric strings from being "corrected"
    if text.isnumeric():
        return text
    return str(TextBlob(text).correct())