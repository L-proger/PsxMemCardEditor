#pragma once

#include <cstdint>
#include <type_traits>
#include <cstring>
#include <fstream>

namespace LFramework::Image::Bmp {

    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    struct SafeInteger{
        std::uint8_t data[sizeof(T)];
        T load() const {
            T result;
            memcpy(&result, data, sizeof(T));
            return result;
        }
        void store(T value){
            memcpy(data, &value, sizeof(T));
        }
        SafeInteger& operator = (T value){
            store(value);
            return *this;
        }
        operator T() const {
            return load();
        }
    };

    #pragma pack(push,2)
    struct FileHeader {
        SafeInteger<std::uint16_t> bfType;
        SafeInteger<std::uint32_t> bfSize;
        SafeInteger<std::uint16_t> bfReserved1;
        SafeInteger<std::uint16_t> bfReserved2;
        SafeInteger<std::uint32_t> bfOffBits;
    };

    static_assert (sizeof(FileHeader) == 14, "Invalid FileHeader size");

    struct Info {
        SafeInteger<std::uint32_t> biSize;
        SafeInteger<std::int32_t>  biWidth;
        SafeInteger<std::int32_t>  biHeight;
        SafeInteger<std::uint16_t> biPlanes;
        SafeInteger<std::uint16_t> biBitCount;
        SafeInteger<std::uint32_t> biCompression;
        SafeInteger<std::uint32_t> biSizeImage;
        SafeInteger<std::int32_t>  biXPelsPerMeter;
        SafeInteger<std::int32_t>  biYPelsPerMeter;
        SafeInteger<std::uint32_t> biClrUsed;
        SafeInteger<std::uint32_t> biClrImportant;
    };
    #pragma pack(pop)

    inline void save24bitImage(int width, int height, const void* data, std::ostream& stream) {
        auto dataSize = width * height * 3;
        FileHeader fileHeader {};
        fileHeader.bfType = 0x4d42;
        fileHeader.bfSize = sizeof(FileHeader) + sizeof(Info) + dataSize;
        fileHeader.bfOffBits = fileHeader.bfSize - dataSize;

        Info info {};
        info.biSize = sizeof(Info);
        info.biWidth = width;
        info.biHeight = height;
        info.biPlanes = 1;
        info.biBitCount = 3 * 8;
        info.biSizeImage = dataSize;

        stream.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
        stream.write(reinterpret_cast<char*>(&info), sizeof(info));
        stream.write(reinterpret_cast<const char*>(data), dataSize);
    }

}
