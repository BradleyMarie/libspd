#ifndef _LIBSPD_READERS_REFLECTIVE_SPD_READER_
#define _LIBSPD_READERS_REFLECTIVE_SPD_READER_

#include <concepts>
#include <expected>
#include <istream>
#include <map>
#include <string>
#include <type_traits>

namespace libspd {

// NOTE: Behavior is undefined if input is not a binary stream
std::expected<std::map<long double, long double>, std::string>
ReadReflectiveSpdAsLongDoublesFrom(std::istream& input);

// NOTE: Behavior is undefined if input is not a binary stream
std::expected<std::map<double, double>, std::string>
ReadReflectiveSpdAsDoublesFrom(std::istream& input);

// NOTE: Behavior is undefined if input is not a binary stream
std::expected<std::map<float, float>, std::string>
ReadReflectiveSpdAsFloatsFrom(std::istream& input);

// NOTE: Behavior is undefined if input is not a binary stream
template <std::floating_point Type>
std::expected<std::map<Type, Type>, std::string> ReadReflectiveSpdFrom(
    std::istream& input) {
  if constexpr (std::is_same<long double, Type>()) {
    return ReadReflectiveSpdAsLongDoublesFrom(input);
  } else if constexpr (std::is_same<double, Type>()) {
    return ReadReflectiveSpdAsDoublesFrom(input);
  } else {
    return ReadReflectiveSpdAsFloatsFrom(input);
  }
}

}  // namespace libspd

#endif  // _LIBSPD_READERS_REFLECTIVE_SPD_READER_