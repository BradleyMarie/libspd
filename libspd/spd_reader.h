#ifndef _LIBSPD_SPD_READER_
#define _LIBSPD_SPD_READER_

#include <expected>
#include <istream>
#include <string>
#include <string_view>

namespace libspd {

// The base class for reading SPD files which performs very minimal validation
// and operates at the maximum precision possible. It is recommended that most
// clients do not implement this class directly and instead use the readers
// under `libspd/readers` which perform more validation and can use reduced
// precision.
class SpdReader {
 public:
  // NOTE: Behavior is undefined if input is not a binary stream
  std::expected<void, std::string> ReadFrom(std::istream& input);

 protected:
  virtual std::expected<void, std::string> ParseComment(
      std::string_view comment) = 0;
  virtual std::expected<void, std::string> ParseSample(long double wavelength,
                                                       long double value) = 0;
};

}  // namespace libspd

#endif  // _LIBSPD_SPD_READER_