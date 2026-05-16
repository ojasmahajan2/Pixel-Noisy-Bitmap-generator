# Simple Pixel-Noisy Bitmap generator

It's just a pixelated noise texture generator in Bitmap image (.bmp) format. Developed using C++, this project has taught me about basic method of storing pixels in byte array (Hexadecimals) to a file. Since the standard header library `<fstream>` is capable of creating, reading and writing operations for files and is particularly used in file handling, I decided to use this to create a very simple project that could generate a colored plane image, grayscale
noise or colored noisy image. 

> [!NOTE]
> This project is not entirely well packaged and well implemented for people who don't know much about programming in C++ (or atleast altering codebase and compiling), so there might be a few things that could've been better...

## Structure of a Bitmap file
