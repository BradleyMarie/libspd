#ifndef _LIBSPD_READERS_EMISSIVE_SPD_READER_
#define _LIBSPD_READERS_EMISSIVE_SPD_READER_

#include <concepts>
#include <expected>
#include <istream>
#include <map>
#include <string>
#include <type_traits>

namespace libspd {

std::expected<std::map<long double, long double>, std::string>
ReadEmisiveSpdAsLongDoublesFrom(std::istream& input);

std::expected<std::map<double, double>, std::string>
ReadEmisiveSpdAsDoublesFrom(std::istream& input);

std::expected<std::map<float, float>, std::string> ReadEmisiveSpdAsFloatsFrom(
    std::istream& input);

template <std::floating_point Type>
std::expected<std::map<Type, Type>, std::string> ReadEmisiveSpdFrom(
    std::istream& input) {
  if constexpr (std::is_same<long double, Type>()) {
    return ReadEmisiveSpdAsLongDoublesFrom(input);
  } else if constexpr (std::is_same<double, Type>()) {
    return ReadEmisiveSpdAsDoublesFrom(input);
  } else {
    return ReadEmisiveSpdAsFloatsFrom(input);
  }
}

}  // namespace libspd

#endif  // _LIBSPD_READERS_EMISSIVE_SPD_READER_