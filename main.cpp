#include <iostream>
#include <fstream>
#include <random>

/*———————————————————————————————————————————————————————————————————————————*/

const unsigned char BMP[18] = {
    0x42, 0x4D,             // "BM"
    0x00, 0x00, 0x00, 0x00, // file size = UNKNOWN bytes
    0x00, 0x00,             // reserved
    0x00, 0x00,             // reserved
    0x36, 0x00, 0x00, 0x00, // pixel data offset = 54
    0x28, 0x00, 0x00, 0x00  // DIB header size = 40
};

int width  = 1;             // width of image in pixels
int height = 1;             // height of image in pixels

const unsigned char metadata[28] = {
    0x01, 0x00,             // color planes = 1

    0x18, 0x00,             // 24 bits per pixel
    0x00, 0x00, 0x00, 0x00, // no compression
    0x04, 0x00, 0x00, 0x00, // image size = 4 bytes

    0x13, 0x0B, 0x00, 0x00, // horizontal resolution
    0x13, 0x0B, 0x00, 0x00, // vertical resolution

    0x00, 0x00, 0x00, 0x00, // colors in palette
    0x00, 0x00, 0x00, 0x00  // important colors
};

/*———————————————————————————————————————————————————————————————————————————*/
struct Pixel {
    const unsigned char RGB_MAX = 255;
    const unsigned char RGB_MIN = 0;

    unsigned char R = RGB_MAX;  // default white
    unsigned char G = RGB_MAX;  // default white
    unsigned char B = RGB_MAX;  // default white
};
/*———————————————————————————————————————————————————————————————————————————*/

void effect(Pixel &color, bool useNoise, bool Color, int &x, int &y) {
    float horizontal = x / float(width );   // Gradient factor width
    float vertical   = y / float(height);   // Gradient factor height

    if (!useNoise) {
        color.R = (color.RGB_MAX * horizontal * (1 - vertical));
        color.G = (color.RGB_MAX * horizontal * vertical);
        color.B = (color.RGB_MAX * (1 - horizontal) * vertical);

        return;
    }

    static std::random_device rd;
    static std::mt19937 rgb(rd());

    static std::uniform_int_distribution<> 
                range(color.RGB_MIN, color.RGB_MAX);

    if (Color) {
        color.R = range(rgb);
        color.G = range(rgb);
        color.B = range(rgb);
    } else {
        color.R = color.G = color.B = range(rgb);
    }
}

void generateImage(bool &isNoise, bool &isColorNoise) {
    Pixel px;

    std::fstream image(
        "Generated_Image.bmp",
        std::ios::binary | std::ios::out
    );
    if (image.is_open()) {
        for (int i = 0; i < 18; i++) {
            image.put(BMP[i]);
        }

        image.write(reinterpret_cast<char*>(&width), sizeof(width));
        image.write(reinterpret_cast<char*>(&height), sizeof(height));

        for (int i = 0; i < 28; i++) {
            image.put(metadata[i]);
        }

        for (int y = 1; y <= height; y++) {
            for (int x = 1; x <= width; x++) {
                effect(px, isNoise, isColorNoise, x, y);
                
                image.put(px.B);
                image.put(px.G);
                image.put(px.R);
            }

            int progress = (y * 100) / height;
            std::cout << "Bitmap Generating... " << progress
                << "%" << "\r" << std::flush;
            if (progress == 100) std::cout << "\n" << std::endl;

            const int shiftRow = (4 - (width * 3) % 4) % 4;     
            for (int k = 1; k <= shiftRow; k++) {
                image.put(0);
            }
        }

        if (isNoise) {
            std::cout << "[NOISY]";
            if (isColorNoise) {
                std::cout << "[COLORED]";
            }
            std::cout << ": Generated_Image.bmp" << std::endl;
        } else {
            std::cout << "[GRADIENT]: Generated_Image.bmp" << std::endl;
        }
        std::cout << "Saved generated image!" << std::endl;

        image.close();
    } else {
        std::cerr << "[ERROR]: Failed to create/generate image"
            << std::endl;
    }
}

/*———————————————————————————————————————————————————————————————————————————*/
int main() {
    std::cout << "\033[2J\033[1;1H" << std::flush;
    std::cout << "\033[?25l" << std::flush;

    /*———————————————————————————————————————————————————————————————————————*/
    bool noise = true;
    bool colored = true;

    width  = 16;
    height = 16;
    /*———————————————————————————————————————————————————————————————————————*/
    generateImage(noise, colored);

    return 0;
}