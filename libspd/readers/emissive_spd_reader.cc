
#include "libspd/readers/emissive_spd_reader.h"

#include "libspd/readers/validating_spd_reader.h"

namespace libspd {
namespace {

template <std::floating_point Type>
class EmisiveSpdReader final : public ValidatingSpdReader<Type> {
 protected:
  virtual std::expected<void, std::string> HandleComment(
      std::string_view comment) override {
    return std::expected<void, std::string>();
  }

  virtual std::expected<void, std::string> HandleSample(
      std::pair<const Type, Type>& sample) override {
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

  return reader.Reset();
}

std::expected<std::map<double, double>, std::string>
ReadEmisiveSpdAsDoublesFrom(std::istream& input) {
  EmisiveSpdReader<double> reader;

  std::expected<void, std::string> result = reader.ReadFrom(input);
  if (!result) {
    return std::unexpected(std::move(result.error()));
  }

  return reader.Reset();
}

std::expected<std::map<float, float>, std::string> ReadEmisiveSpdAsFloatsFrom(
    std::istream& input) {
  EmisiveSpdReader<float> reader;

  std::expected<void, std::string> result = reader.ReadFrom(input);
  if (!result) {
    return std::unexpected(std::move(result.error()));
  }

  return reader.Reset();
}

}  // namespace libspd