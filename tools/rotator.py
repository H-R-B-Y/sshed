"need pillow"
from PIL import Image

def rotate_image_90(input_path, output_path):
	"""Rotate an image by 90 degrees clockwise and save it."""
	with Image.open(input_path) as img:
		rotated_img = img.rotate(-90, expand=True)
		rotated_img.save(output_path)

if __name__ == "__main__":
	"Ok we want to loop through all the png images in a directory and rotate them,"
	"saving them to the same folder with _rot appended to the filename"
	import os
	input_directory = "images/Medium"
	for filename in os.listdir(input_directory):
		if filename.endswith(".png"):
			input_path = os.path.join(input_directory, filename)
			output_filename = filename[:-4] + "_rot.png"
			output_path = os.path.join(input_directory, output_filename)
			rotate_image_90(input_path, output_path)
			print(f"Rotated {input_path} and saved to {output_path}")