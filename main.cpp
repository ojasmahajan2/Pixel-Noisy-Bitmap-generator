#include <fstream>
#include <stdexcept>
#include <iostream>
#include <random>

// ————————————————————————————————————————————————————————————————————————————
const uint32_t WIDTH  = 16;
const uint32_t HEIGHT = 16;

bool noise = true;
bool colored = true;
// ————————————————————————————————————————————————————————————————————————————

struct ImageRenderer {
public: 
    void run() {
        std::fstream image(
            "Generated_Image.bmp",
            std::ios::binary | std::ios::out
        );

        if (image.is_open()) {
            header(image);
            imageDimensions(image);
            metaData(image);

            pixelArray(image);

            std::cout << "[Completed]: ";
            std::cout << "Image saved!\n" << std::endl;

            image.close();
        } else {
            throw std::runtime_error("Failed to open the file!");
        }
    }

private:
    void writeBinary(std::fstream& img, const auto& value) {
        img.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }

// ============================================================================
    #pragma pack(push, 1)
    struct BitmapHeader {
        const uint32_t FileSize      
            = 14 + 40 + (WIDTH * 3 + (4 - (WIDTH * 3) % 4) % 4) * HEIGHT;

        const uint32_t Reserved      = 0;
        const uint32_t PxDataOffset  = 14 + 40;
        const uint32_t DIBHeaderSize = 40;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct DIBmetadata {
        const uint16_t ColorPlanes          = 1;
        const uint16_t BitsPerPixel         = 24;
        const uint32_t Compression          = 0;
        const uint32_t ImageSize            = 0;
        const uint32_t HorizontalResolution = 2835;
        const uint32_t VerticalResolution   = 2835;
        const uint32_t PaletteColors        = 0;
        const uint32_t ImportantColors      = 0;
    };
    #pragma pack(pop)
// ============================================================================

    void header(std::fstream& img) {
        const char* Signature        = "BM";
        img.write(Signature, 2);

        BitmapHeader bmpHeader;
        writeBinary(img, bmpHeader);
    }

    void imageDimensions(std::fstream& img) {
        writeBinary(img, WIDTH);
        writeBinary(img, HEIGHT);
    }

    void metaData(std::fstream& img) {
        DIBmetadata mtadta;
        writeBinary(img, mtadta);
    }

// ============================================================================
    struct PixelRGB {
        uint8_t B = 255;
        uint8_t G = 255;
        uint8_t R = 255;
    };
// ============================================================================
    
    void pixelArray(std::fstream& img) {
        PixelRGB pixel;
        const uint8_t ShiftRow = (4 - (WIDTH * 3) % 4) % 4;

        for (uint32_t column = 0; column < HEIGHT; column++) {
            for (uint32_t row = 0; row < WIDTH; row++) {
                effect(pixel, row, column);
                writeBinary(img, pixel);
            }

            const char padding[3] = {0, 0, 0};
            img.write(padding, ShiftRow);

            std::cout << "Rendering: " << ((column + 1) * 100) / HEIGHT << "%"
                        << "\r" << std::flush;
        }

        std::cout << "\n";
    }

    void effect(PixelRGB& color, const uint32_t& x, const uint32_t& y) {
        const uint8_t RGB_MAX = 255;
        const uint8_t RGB_MIN = 0;

        if (!noise && colored) {
            float horizontal = x / float(WIDTH);    // Gradient factor width
            float vertical   = y / float(HEIGHT);   // Gradient factor height

            color.R = (RGB_MAX * horizontal * (1 - vertical));
            color.G = (RGB_MAX * horizontal * vertical);
            color.B = (RGB_MAX * (1 - horizontal) * vertical);

            return;
        }
        else if (!noise && !colored) {
            return;
        }

        static std::random_device rd;
        static std::mt19937 rgb(rd());

        static std::uniform_int_distribution<> 
                    range(RGB_MIN, RGB_MAX);

        if (noise && colored) {
            color.R = range(rgb);
            color.G = range(rgb);
            color.B = range(rgb);
        } else {
            color.R = color.G = color.B = range(rgb);
        }
    }
};

int main() {
    std::cout << "\033[2J\033[1;1H" << std::flush;
    std::cout << "\033[?25l" << std::flush;

    try {
        ImageRenderer app;
        app.run();
    }
    catch (const std::exception& error) {
        std::cerr << error.what() << "\n";
        return EXIT_FAILURE;
    }

    std::cout << "\033[?25h" << std::flush;

    return EXIT_SUCCESS;
}
