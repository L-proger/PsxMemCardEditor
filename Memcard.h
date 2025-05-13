#pragma once

#include <PsxMemCard/PsxMemCard.h>
#include <ImageBmp.h>
#include <array>
#include <cstring>
#include <vector>

class MemcardIo {
public:
    using Frame = std::array<std::uint8_t, 128>;
    using Block = std::array<Frame, 64>;
    virtual bool readFrame(Frame& frame, std::size_t frameIndex) = 0;
};

class VirtualMemcardIo : public MemcardIo {
public:
    using Image = std::array<std::uint8_t, 128 * 1024>;
    VirtualMemcardIo(const Image& memcardImage) : _image(memcardImage){

    }
    bool readFrame(Frame& frame, std::size_t frameIndex) override {
        if(frameIndex > 0x3ff){
            return false;
        }
        memcpy(frame.data(), _image.data() + (frameIndex * sizeof(Frame)), sizeof(Frame));
        return true;
    }
private:
    Image _image;
};


class Memcard{
public:
    using RawIcon = std::array<std::uint8_t, (16 * 16) / 2>; //4 bit indices for color palette
    using Icon = std::array<std::uint16_t, 16 * 16>; //Colors from color palette

    struct FileRecord {
        std::uint8_t blockId;
        Psx::MemCard::BlockAllocationState allocationState = Psx::MemCard::BlockAllocationState::Formatted;
        //std::uint8_t allocationState = 0;
        std::uint16_t fileSize = 0; //in bytes
        FileRecord* nextBlock = nullptr;
        std::uint8_t name[21] = {};
    };

    struct FileHeader {
        using ColorPalette = std::array<std::uint16_t, 16>;
        std::uint8_t iconFramesCount;
        std::uint8_t unknownValue;
        std::array<std::uint8_t, 64> title;
        ColorPalette colorPalette;
    };

    Memcard(std::shared_ptr<MemcardIo> memcardInterface) : _memcard(memcardInterface){
        scanFileTable();
        scanBrokenSectorsList();
    }

    static void unpackRawIcon(const FileHeader::ColorPalette& palette, const RawIcon& icon, Icon& result) {
        for(int i = 0; i < icon.size(); ++i){
            auto rawData = icon[i];
            int ip0 = rawData & 0x0f;
            int ip1 = (rawData >> 4) & 0x0f;

            auto cp0 = palette[ip0];
            auto cp1 = palette[ip1];

            result[i * 2 + 0] = cp0;
            result[i * 2 + 1] = cp1;
        }
    }

    const FileRecord& getFile(int id) const {
        return _records[id];
    }

    void readFileIcon(const FileRecord* file, int iconId, RawIcon& icon) {
        _memcard->readFrame(*reinterpret_cast<MemcardIo::Frame*>(&icon), (((file->blockId + 1) * sizeof(MemcardIo::Block)) / sizeof(MemcardIo::Frame)) + 1 + iconId);
    }

    void readFileHeader(const FileRecord* file, FileHeader& header) const {
        MemcardIo::Frame block;
        _memcard->readFrame(block, ((file->blockId + 1) * (1024 * 8)) / 128);
        header.iconFramesCount = block[2] & 0xf;
        header.unknownValue = block[3];
        memcpy(header.title.data(), &block[4], sizeof(header.title));
        memcpy(header.colorPalette.data(), &block[0x60], sizeof(header.colorPalette));
    }

    void readReplacementData(MemcardIo::Frame& block, std::size_t replacementDataBlockId) {
        _memcard->readFrame(block, replacementDataBlockId + 36);
    }
private:

    void scanBrokenSectorsList() {
        MemcardIo::Frame block;
        for(int i = 16; i <= 35; ++i){
            _memcard->readFrame(block, i + 1);
            std::uint32_t id = block[0] | (block[1] << 8) | (block[2] << 16) | (block[3] << 24) ;
            _brokenSectorId[i - 16] = id;
        }
    }
    void scanFileTable()  {
        MemcardIo::Frame block;

        for(int i = 0; i < 15; ++i){
            _memcard->readFrame(block, i + 1);
            auto frame = reinterpret_cast<const Psx::MemCard::DirectoryFrame*>(block.data());
            auto& record = _records[i];
            record.blockId = i;

            record.fileSize = frame->fileSize[0] | (frame->fileSize[1] << 8) | (frame->fileSize[2] << 16) | (frame->fileSize[3] << 24);
            record.allocationState = static_cast<Psx::MemCard::BlockAllocationState>(frame->allocationState[0]);

            std::uint16_t nextBlockId = frame->nextBlockId[0] | (frame->nextBlockId[1] << 8);
            if(nextBlockId != 0xffff){
                record.nextBlock = &_records[nextBlockId];
            }
            memcpy(record.name, frame->name, sizeof(frame->name));
        }
    }
    std::array<FileRecord, 15> _records;
    std::array<std::uint32_t, 20> _brokenSectorId;
    std::shared_ptr<MemcardIo> _memcard;
};



inline std::vector<std::uint8_t> generateImage24BitBGR(int width, int height, std::uint8_t r, std::uint8_t g, std::uint8_t b) {
    std::vector<std::uint8_t> result;
    result.resize(width * height * 3);

    for(int i = 0; i < (width * height); ++i){
        result[i * 3 + 0] = b;
        result[i * 3 + 1] = g;
        result[i * 3 + 2] = r;
    }

    return result;
}

using IconBgr24 = std::array<std::uint8_t, 16 * 16 *3>;

inline void iconToBgr24(const Memcard::Icon& icon, IconBgr24& result){
    for(int i = 0; i < icon.size(); ++i){
        auto rawPixel = icon[i];
        bool stp = (rawPixel & 0x8000) != 0;
        std::uint8_t r = rawPixel & 0x1f;
        std::uint8_t g = (rawPixel >> 5) & 0x1f;
        std::uint8_t b = (rawPixel >> 10) & 0x1f;


        result[i * 3 + 0] = b << 3;
        result[i * 3 + 1] = g << 3;
        result[i * 3 + 2] = r << 3;
    }
}

inline void iconToRgb24(const Memcard::Icon& icon, IconBgr24& result){
    for(int i = 0; i < icon.size(); ++i){
        auto rawPixel = icon[i];
        bool stp = (rawPixel & 0x8000) != 0;
        std::uint8_t r = rawPixel & 0x1f;
        std::uint8_t g = (rawPixel >> 5) & 0x1f;
        std::uint8_t b = (rawPixel >> 10) & 0x1f;


        result[i * 3 + 0] = r << 3;
        result[i * 3 + 1] = g << 3;
        result[i * 3 + 2] = b << 3;
    }
}
