from PIL import Image

def jpg_to_bmp(input_path, output_path):
    try:
        # Open the JPG image
        img = Image.open(input_path)
        
        # Convert the image to BMP format
        img_bmp = img.convert("RGB")
        
        # Save the BMP image
        img_bmp.save(output_path)
        print("Conversion successful.")
        
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    # Provide the input and output paths
    input_path = "input.jpg"  # Change this to your input JPG file path
    output_path = "output.bmp"  # Change this to your desired output BMP file path
    
    # Perform the conversion
    jpg_to_bmp(input_path, output_path)