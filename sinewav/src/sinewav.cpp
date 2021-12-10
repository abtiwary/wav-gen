#include "sinewav.hpp"

namespace SineWave {

auto
SinusoidalOscillator::tick(size_t increment) -> double {
    return m_amplitude * sin(m_angle * increment) + m_phase;
}

}
