# Simple Pixel-Noisy Bitmap generator

It's just a pixelated noise texture generator in Bitmap image (.bmp) format. Developed using C++, this project has taught me about basic method of storing pixels in byte array (Hexadecimals) to a file. Since the standard header library `<fstream>` is capable of creating, reading and writing operations for files and is particularly used in file handling, I decided to use this to create a very simple project that could generate a colored plane image, grayscale
noise or colored noisy image. 

> [!NOTE]
> This project is not entirely well packaged and well implemented for people who don't know much about programming in C++ (or atleast altering codebase and compiling), so there might be a few things that could've been better...

## Structure of a Bitmap file
![Basic bmp structure](https://miro.medium.com/1*ORbpVH3QpjPAhIejfUMCyg.png)------![Sophisticated bmp structure](https://binaryworld.net/Main/Articles/Lang1/Cat277/Code3685/image3685_1.jpg)

> [!TIP]
> Use a Hex editor for better understanding and also make sure to use file's input output stream as Binary and output.

The console program manually writes header starting with ASCII characters `BM` and then writes other metrics such as File size (4 bytes set to 0x00 for unknown value), 4 byte Reserved, Data offset, etc. 
>File size is set to `0x00` (UNKNOWN value) because as we are writing, the size is automatically adjusted based on the info written in file. Modern image viewers are lenient with this, so this may not cause an issue.

After this part, the codebase contains 2 integer variables defined for Width and Height of the image (set to 1px by default). These variables are in global scope and are mutable, so we can edit these values in main function just before generate/write is called to write the desired image resolution. The integer values are converted to 4 byte hex array using `reinterpret_cast<char*>` before writing.

Again, the manual writing process of metadata consisting DPI (~72), color planes (1), palette colors (0 x 4 bytes), etc.

## Actual Pixel array **[PROCEDURAL]**
The best thing about `unsigned char` is that it can store values between 0 to 255, which is exactly what we need for R, G and B. I've used 24-Bit Pixels (Standard RGB) to be able to produce approx. 16.7+ million colors.

An odd thing to notice in the hex editor, Bitmap stores pixel data in BGR sequence. It also starts from the bottom row first. 
To switch rows, a formula `(4 - (width * 3) % 4) % 4` is used for calculating padding bytes required to align each row of a 24-bit BMP image. 

**To write a single colored plane image:** A nested loop that writes B, G and R until the width resolution is reached, then a row shifting loop inside the height loop that writes `0x00` until the value produced by the row shifting formula is reached.
That's all for rendering a plane image.

## Effects **[PROCEDURAL]**
This is the interesting part of this simple project, adding effects to the boring plane image we just generated. For this though, each R, G and B value must be changed for each iteration according to the method before writing. 

I've used 2 very basic effects like gradient and pixel noise.

**Gradient:** Requires floating point data type and best used when separating horizontal and vertical factors for more control (a mathematical formula). 

**Pixel Noise:** Requires a library that can produce random numbers within a provided range of 0 to 255. This random number is assigned to each RGB channel (if all channels have the same random value, then the image would be in grayscale. Assigning a different random number for each channel would produce a decent looking colored image).
_________________________________________________________________________________________________________________________________________________
> ### Conlusion
> This is a fraction of a drop in the ocean of Graphics programming. Understanding arrays, vectors and systems design is crucial to get into this field. Projects like these atleast get you started... Thankyou for checking this out :D
