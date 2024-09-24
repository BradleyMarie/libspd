
#include "libspd/readers/emissive_spd_reader.h"

#include <cmath>

#include "libspd/spd_reader.h"

namespace libspd {
namespace {

template <std::floating_point Type>
class EmisiveSpdReader final : public SpdReader {
 public:
  std::map<Type, Type> samples;

 protected:
  virtual std::expected<void, std::string> ParseComment(
      std::string_view comment) override {
    return std::expected<void, std::string>();
  }

  virtual std::expected<void, std::string> ParseSample(
      long double wavelength, long double value) override {
    if (wavelength < static_cast<long double>(0.0)) {
      return std::unexpected(
          "The input contained a sample with a negative wavelength");
    }

    if (value < static_cast<long double>(0.0)) {
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

    Type value_final_precision = static_cast<Type>(value);

    if (!std::isfinite(value_final_precision)) {
      return std::unexpected(
          "The input contained a sample with a non-finite spectral power");
    }

    if (!std::isfinite(value_final_precision)) {
      return std::unexpected(
          "The input contained a sample with a non-finite spectral power");
    }

    auto [_, inserted] =
        samples.try_emplace(wavelength_final_precision, value_final_precision);
    if (!inserted) {
      return std::unexpected(
          "The input contained multiple samples for the same wavelength");
    }

    return std::expected<void, std::string>();
  }
};

}  // namespace

std::expected<std::map<long double, long double>, std::string>
ReadEmisiveSpdAsLongDoublesFrom(std::istream& input) {
  EmisiveSpdReader<long double> reader;

  std::expected<void, std::string> result = reader.ReadFrom(input);
  if (!result) {
    return std::unexpected(std::move(result.error()));
  }

  return std::move(reader.samples);
}

std::expected<std::map<double, double>, std::string>
ReadEmisiveSpdAsDoublesFrom(std::istream& input) {
  EmisiveSpdReader<double> reader;

  std::expected<void, std::string> result = reader.ReadFrom(input);
  if (!result) {
    return std::unexpected(std::move(result.error()));
  }

  return std::move(reader.samples);
}

std::expected<std::map<float, float>, std::string> ReadEmisiveSpdAsFloatsFrom(
    std::istream& input) {
  EmisiveSpdReader<float> reader;

  std::expected<void, std::string> result = reader.ReadFrom(input);
  if (!result) {
    return std::unexpected(std::move(result.error()));
  }

  return std::move(reader.samples);
}

}  // namespace libspd