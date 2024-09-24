
#include "libspd/readers/reflective_spd_reader.h"

#include "libspd/readers/validating_spd_reader.h"

namespace libspd {
namespace {

template <std::floating_point Type>
class ReflectiveSpdReader final : public ValidatingSpdReader<Type> {
 protected:
  virtual std::expected<void, std::string> HandleComment(
      std::string_view comment) override {
    return std::expected<void, std::string>();
  }

  virtual std::expected<void, std::string> HandleSample(
      std::pair<const Type, Type>& sample) override {
    if (sample.second > static_cast<Type>(1.0)) {
      return std::unexpected(
          "The input contained a sample with a spectral power greater than "
          "one");
    }

    return std::expected<void, std::string>();
  }
};

}  // namespace

std::expected<std::map<long double, long double>, std::string>
ReadReflectiveSpdAsLongDoublesFrom(std::istream& input) {
  ReflectiveSpdReader<long double> reader;

  std::expected<void, std::string> result = reader.ReadFrom(input);
  if (!result) {
    return std::unexpected(std::move(result.error()));
  }

  return reader.Reset();
}

std::expected<std::map<double, double>, std::string>
ReadReflectiveSpdAsDoublesFrom(std::istream& input) {
  ReflectiveSpdReader<double> reader;

  std::expected<void, std::string> result = reader.ReadFrom(input);
  if (!result) {
    return std::unexpected(std::move(result.error()));
  }

  return reader.Reset();
}

std::expected<std::map<float, float>, std::string>
ReadReflectiveSpdAsFloatsFrom(std::istream& input) {
  ReflectiveSpdReader<float> reader;

  std::expected<void, std::string> result = reader.ReadFrom(input);
  if (!result) {
    return std::unexpected(std::move(result.error()));
  }

  return reader.Reset();
}

}  // namespace libspd