#!/usr/bin/env python3
"""
Simple Image to ASCII Converter for Heart
"""

from PIL import Image
import sys

def image_to_ascii(image_path, width=50, height=30):
    """Convert image to ASCII art optimized for heart shape"""
    
    # ASCII characters from darkest to lightest
    ascii_chars = "@%#*+=-:. "
    
    try:
        # Open and convert image
        with Image.open(image_path) as img:
            # Convert to grayscale
            img = img.convert('L')
            
            # Resize image to fit heart
            img = img.resize((width, height))
            
            # Convert to ASCII
            ascii_art = []
            for y in range(height):
                line = ""
                for x in range(width):
                    # Get pixel brightness (0-255)
                    pixel = img.getpixel((x, y))
                    # Map to ASCII character
                    ascii_index = int(pixel / 255 * (len(ascii_chars) - 1))
                    line += ascii_chars[ascii_index]
                ascii_art.append(line)
            
            return ascii_art
            
    except Exception as e:
        return [f"Error: {e}"]

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 heart_ascii.py <image_path> [output_file]")
        sys.exit(1)
    
    image_path = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) > 2 else "heart_image.txt"
    
    print(f"Converting {image_path} for heart...")
    
    ascii_result = image_to_ascii(image_path, width=60, height=40)
    
    # Save to file
    with open(output_file, 'w') as f:
        for line in ascii_result:
            f.write(line + '\n')
    
    print(f"ASCII image saved in: {output_file}")
    print(f"Proportions: {len(ascii_result[0]) if ascii_result else 0}x{len(ascii_result)}")

if __name__ == "__main__":
    main()