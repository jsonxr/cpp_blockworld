/**
 * @file    SimplexNoise.h
 * @brief   A Perlin Simplex Noise C++ Implementation (1D, 2D, 3D).
 *
 * Copyright (c) 2014-2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef APP_VENDOR_SIMPLEXNOISE_H
#define APP_VENDOR_SIMPLEXNOISE_H

// NOLINTBEGIN

#include <array>
#include <cstddef>  // size_t
#include <cstdint>

/**
 * @brief A Perlin Simplex Noise C++ Implementation (1D, 2D, 3D, 4D).
 */
class SimplexNoise {
 public:
  // 1D Perlin simplex noise
  float noise(float x) const;
  // 2D Perlin simplex noise
  float noise(float x, float y) const;
  // 3D Perlin simplex noise
  float noise(float x, float y, float z) const;

  // Fractal/Fractional Brownian Motion (fBm) noise summation
  float fractal(size_t octaves, float x) const;
  float fractal(size_t octaves, float x, float y) const;
  float fractal(size_t octaves, float x, float y, float z) const;

  uint32_t set_seed(uint32_t seed);

  /**
   * Constructor of to initialize a fractal noise summation
   *
   * @param[in] frequency    Frequency ("width") of the first octave of noise
   * (default to 1.0)
   * @param[in] amplitude    Amplitude ("height") of the first octave of noise
   * (default to 1.0)
   * @param[in] lacunarity   Lacunarity specifies the frequency multiplier
   * between successive octaves (default to 2.0).
   * @param[in] persistence  Persistence is the loss of amplitude between
   * successive octaves (usually 1/lacunarity)
   */
  explicit SimplexNoise(int seed = 0, float frequency = 1.0f,
                        float amplitude = 1.0f, float lacunarity = 2.0f,
                        float persistence = 0.5f)
      : seed_(set_seed(seed)),
        mFrequency(frequency),
        mAmplitude(amplitude),
        mLacunarity(lacunarity),
        mPersistence(persistence) {}

 private:
  int seed_{};
  // Parameters of Fractional Brownian Motion (fBm) : sum of N "octaves" of
  // noise
  float mFrequency{1.0f};  ///< Frequency ("width") of the first octave of noise
                           ///< (default to 1.0)
  float mAmplitude{1.0f};  ///< Amplitude ("height") of the first octave of
                           ///< noise (default to 1.0)
  float mLacunarity{2.0f};   ///< Lacunarity specifies the frequency multiplier
                             ///< between successive octaves (default to 2.0).
  float mPersistence{0.5f};  ///< Persistence is the loss of amplitude between
                             ///< successive octaves (usually 1/lacunarity)

  inline uint8_t hash(int32_t i) const {
    return perm_[static_cast<uint8_t>(i)];
  }

  std::array<uint8_t, 256> perm_{
      151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,
      225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190,
      6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117,
      35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136,
      171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158,
      231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,
      245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209,
      76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,
      164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,
      202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,
      58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,
      154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253,
      19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
      228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,
      145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184,
      84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,
      222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156,
      180};
};

// NOLINTEND
#endif  // APP_VENDOR_SIMPLEXNOISE_H
