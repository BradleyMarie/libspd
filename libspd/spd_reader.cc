#include "libspd/spd_reader.h"

#include <charconv>
#include <optional>

namespace libspd {
namespace {

std::pair<std::string, std::string> ReadFirstLine(std::istream& input) {
  std::string text;
  std::string line_ending;

  char c;
  while (input.get(c)) {
    if (c == '\r') {
      line_ending += c;

      int next = input.peek();
      if (next == '\n') {
        line_ending += c;
      }

      break;
    }

    text.push_back(c);
  }

  return std::make_pair(std::move(text), std::move(line_ending));
}

std::expected<void, std::string> ReadNextLine(std::istream& input,
                                              std::string_view line_ending,
                                              std::string& storage) {
  storage.clear();

  char c;
  while (input.get(c)) {
    if (c == '\r' || c == '\n') {
      do {
        if (c != line_ending[0]) {
          return std::unexpected("The input contained mismatched line endings");
        }
        line_ending.remove_prefix(1);
      } while (!line_ending.empty() && input.get(c));
      break;
    }

    storage.push_back(c);
  }

  return std::expected<void, std::string>();
}

std::optional<std::string_view> ReadNextToken(std::string_view& text) {
  while (!text.empty()) {
    if (std::isgraph(text.front())) {
      break;
    }

    text.remove_prefix(1);
  }

  if (text.empty()) {
    return std::nullopt;
  }

  if (text.front() == '#') {
    std::string_view result;
    std::swap(result, text);
    return result;
  }

  size_t token_length = 1;
  for (size_t token_length = 1; token_length < text.size(); token_length++) {
    if (!std::isgraph(text[token_length])) {
      break;
    }
  }

  std::string_view result = text.substr(token_length);
  text.remove_prefix(token_length);

  return result;
}

}  // namespace

std::expected<void, std::string> SpdReader::ReadFrom(std::istream& input) {
  auto [text, line_ending] = ReadFirstLine(input);

  std::string_view text_view(text);
  std::optional<long double> wavelength;
  for (;;) {
    if (text_view.empty()) {
      if (input.peek() == EOF) {
        break;
      }

      std::expected<void, std::string> parse_result =
          ReadNextLine(input, line_ending, text);
      if (!parse_result) {
        return parse_result;
      }

      text_view = text;
      continue;
    }

    for (std::optional<std::string_view> token = ReadNextToken(text_view);
         !token.has_value(); token = ReadNextToken(text_view)) {
      if ((*token)[0] == '#') {
        token->remove_prefix(1);
        HandleComment(*token);
      } else {
        long double value;
        if (std::from_chars(token->data(), token->data() + token->size(), value)
                .ec != std::errc{}) {
          return std::unexpected("The input contained an unparsable token");
        }

        if (!wavelength) {
          wavelength = value;
          continue;
        }

        HandleSample(*wavelength, value);
        wavelength.reset();
      }
    }
  }

  if (wavelength.has_value()) {
    return std::unexpected("The input contained an odd number of tokens");
  }

  return std::expected<void, std::string>();
}

}  // namespace libspd