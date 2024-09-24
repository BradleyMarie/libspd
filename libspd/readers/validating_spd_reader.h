#ifndef _LIBSPD_READERS_VALIDATING_SPD_READER_
#define _LIBSPD_READERS_VALIDATING_SPD_READER_

#include <cmath>
#include <concepts>

#include "libspd/spd_reader.h"

namespace libspd {

template <std::floating_point Type>
class ValidatingSpdReader : public SpdReader {
 private:
  std::map<Type, Type> samples_;

 public:
  std::map<Type, Type> Reset() {
    std::map<Type, Type> result = std::move(samples_);
    samples_.clear();
    return result;
  }

 protected:
  virtual std::expected<void, std::string> HandleSample(
      std::pair<const Type, Type>& sample) = 0;

  std::expected<void, std::string> HandleSample(
      long double wavelength, long double spectral_power) final override {
    if (wavelength < static_cast<long double>(0.0)) {
      return std::unexpected(
          "The input contained a sample with a negative wavelength");
    }

    if (spectral_power < static_cast<long double>(0.0)) {
      return std::unexpected(
          "The input contained a sample with a negative spectral power");
    }

    Type wavelength_final_precision = static_cast<Type>(wavelength);
    if (wavelength_final_precision == static_cast<Type>(0.0)) {
      return std::unexpected(
          "The input contained a sample for wavelength zero");
    }

    if (!std::isfinite(wavelength_final_precision)) {
      return std::unexpected(
          "The input contained a sample with for a non-finite wavelength");
    }

    Type spectral_power_final_precision = static_cast<Type>(spectral_power);

    if (!std::isfinite(spectral_power_final_precision)) {
      return std::unexpected(
          "The input contained a sample with a non-finite spectral power");
    }

    if (!std::isfinite(spectral_power_final_precision)) {
      return std::unexpected(
          "The input contained a sample with a non-finite spectral power");
    }

    auto [iter, inserted] = samples_.try_emplace(
        wavelength_final_precision, spectral_power_final_precision);
    if (!inserted) {
      return std::unexpected(
          "The input contained multiple samples for the same wavelength");
    }

    return HandleSample(*iter);
  }
};

}  // namespace libspd

#endif  // _LIBSPD_READERS_VALIDATING_SPD_READER_