#ifndef _SINEWAV_HPP_
#define _SINEWAV_HPP_

#include <cstdint>
#include <cmath>

namespace SineWave {

class SinusoidalOscillator {
public:
    SinusoidalOscillator(
        double frequency, double amplitude, double phase, unsigned int sample_rate
        ) : m_frequency(frequency),
              m_amplitude(amplitude),
              m_phase(phase),
              m_sample_rate(sample_rate)
    {
        m_angle = m_tau * m_frequency / m_sample_rate;
    };

    double tick(size_t increment);

private:
    double m_frequency = 0.0;
    double m_amplitude = 0.0;
    double m_angle = 0.0;
    double m_phase = 0.0;
    double m_tau = 2.0 * M_PI;
    unsigned int m_sample_rate = 44100;
};

}

#endif //_SINEWAV_HPP_
