#ifndef _WAV_HPP_
#define _WAV_HPP_

#include <cstdint>
#include <string>
#include <fstream>

struct WavHeader {
    unsigned char chunk_id[4] = {'R', 'I', 'F', 'F'};
    uint32_t chunk_data_size = 0;
    unsigned char riff_type[4] = {'W', 'A', 'V', 'E'};
};

struct WavFmtChunk {
    unsigned char chunk_id[4] = {'f', 'm', 't', ' '};
    uint32_t chunk_data_size = 16;
    uint16_t compression_code = 1;
    uint16_t number_channels = 1;
    uint32_t sample_rate = 0;
    uint32_t average_bytes_per_second = 0;
    uint16_t block_align = 0;
    uint16_t significant_bits_per_sample = 0;
};

struct WavDataChunk {
    unsigned char chunk_id[4] = {'d', 'a', 't', 'a'};
    uint32_t chunk_size = 0;
};

class WavFile {
public:
    WavFile(
            std::string&& file_path
        ) : m_file_path(file_path) {
        m_wav_output.open(m_file_path, std::ios::binary);
    }

    ~WavFile() {
        m_wav_output.close();
    }

    void write_content(
        const WavHeader& wav_header,
        const WavFmtChunk& wav_fmt_chunk,
        const WavDataChunk& wav_data_chunk,
        const std::vector<uint16_t>& samples,
        unsigned int bit_depth
    );

    unsigned long get_file_size();

private:
    std::string m_file_path;
    std::ofstream m_wav_output;
    uint32_t m_file_size = 0;
};

inline void
WavFile::write_content(
    const WavHeader& wav_header,
    const WavFmtChunk& wav_fmt_chunk,
    const WavDataChunk& wav_data_chunk,
    const std::vector<uint16_t>& samples,
    unsigned int bit_depth) {
    m_wav_output.write(reinterpret_cast<const char*>(&wav_header), sizeof(WavHeader));
    m_wav_output.write(reinterpret_cast<const char*>(&wav_fmt_chunk), sizeof(WavFmtChunk));
    m_wav_output.write(reinterpret_cast<const char*>(&wav_data_chunk), sizeof(WavDataChunk));
    m_wav_output.write(reinterpret_cast<const char*>(&samples[0]), samples.size() * bit_depth / 8);
    m_file_size = m_wav_output.tellp();
}

inline unsigned long
WavFile::get_file_size() {
    return m_file_size;
}


#endif //_WAV_HPP_
