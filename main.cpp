/**
 * `WavGen` is an application to generate a 1 channel PCM .wav file that can play a
 * generated sinusoidal signal with a defined sample rate, bit depth, and a given duration.
 *
 * http://soundfile.sapp.org/doc/WaveFormat/
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

#include "sinewav/include/sinewav.hpp"
#include "sinewav/include/wav.hpp"

constexpr unsigned int SAMPLERATE = 44100;
constexpr unsigned int BITDEPTH = 16;
constexpr unsigned int NUMCHANNELS = 1;

int main(int argc, char** argv)
{
    auto usage_str = R"usage(
  WavGen <outdir> [duration]
    )usage";

    std::string directory = "/Users/abtiwary/Desktop/";
    unsigned int duration = 2;

    if (argc == 3) {
        directory = std::string{argv[1]};
        try {
            duration = std::stoi(argv[2]);
        } catch (std::invalid_argument& e) {
            std::cout << usage_str << '\n';
        } catch (std::out_of_range& e) {
            std::cout << usage_str << '\n';
        }
    } else if (argc == 2) {
        directory = std::string{argv[1]};
    } else {
        std::cout << usage_str << '\n';
        exit(-1);
    }

    std::cout << "generating a wav file with duration = " << duration << '\n';

    int max_amplitude_for_bitdepth = (2 << (BITDEPTH - 1)) - 1;
    std::vector<uint16_t> samples(SAMPLERATE * duration, 0);

    SineWave::SinusoidalOscillator sinOsc{440, 0.5, 0, SAMPLERATE};

    for(size_t i=0; i < SAMPLERATE * duration; i++) {
        samples[i] = static_cast<uint16_t>(sinOsc.tick(i) * max_amplitude_for_bitdepth);
    }

    // write out a test file that can played using Audacity
    std::ofstream temp_output{directory + "samples.dat", std::ios::binary | std::ios::trunc};
    temp_output.write(reinterpret_cast<char*>(samples.data()), samples.size());
    temp_output.close();

    // write out the wav file
    WavFile wav_output{directory + "samples.wav"};

    WavHeader wav_header{};

    WavFmtChunk wav_fmt_chunk{};
    wav_fmt_chunk.sample_rate = SAMPLERATE;
    wav_fmt_chunk.average_bytes_per_second = SAMPLERATE * BITDEPTH / 8;
    wav_fmt_chunk.block_align = BITDEPTH / 8;
    wav_fmt_chunk.significant_bits_per_sample = BITDEPTH;
    wav_fmt_chunk.number_channels = static_cast<uint16_t>(NUMCHANNELS);

    WavDataChunk wav_data_chunk{};
    wav_data_chunk.chunk_size = samples.size() * BITDEPTH / 8;

    wav_header.chunk_data_size = static_cast<uint32_t>(sizeof(WavHeader) + sizeof(WavFmtChunk) + sizeof(WavDataChunk) + (samples.size() * (BITDEPTH / 8)) - 8);
    //std::cout << wav_header.chunk_data_size << std::endl;

    wav_output.write_content(
        wav_header,
        wav_fmt_chunk,
        wav_data_chunk,
        samples,
        BITDEPTH
    );

    // get the total file size
    std::cout << "generated a file of size: " << wav_output.get_file_size() << '\n';

    return 0;
}
