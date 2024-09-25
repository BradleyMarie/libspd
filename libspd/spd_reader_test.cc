#include "libspd/spd_reader.h"

#include <fstream>

#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "tools/cpp/runfiles/runfiles.h"

namespace libspd {
namespace {

using ::bazel::tools::cpp::runfiles::Runfiles;
using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

class MockSpdReader final : public SpdReader {
 public:
  MOCK_METHOD((std::expected<void, std::string>), HandleComment,
              (std::string_view), (override));
  MOCK_METHOD((std::expected<void, std::string>), HandleSample,
              (long double, long double), (override));
};

std::ifstream OpenRunfile(const std::string& filename) {
  std::unique_ptr<Runfiles> runfiles(Runfiles::CreateForTest());
  std::string path = "__main__/libspd/test_data/" + filename;
  return std::ifstream(runfiles->Rlocation(path),
                       std::ios::in | std::ios::binary);
}

TEST(SpdReader, BadStream) {
  std::ifstream input = OpenRunfile("notarealfile.spd");

  MockSpdReader spd_reader;
  EXPECT_EQ("Bad input stream passed", spd_reader.ReadFrom(input).error());
}

TEST(SpdReader, MismatchedLineEndings) {
  std::ifstream input = OpenRunfile("mismatched_line_endings.spd");

  MockSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(" Line Ending in CR"))
      .WillOnce(Return(std::expected<void, std::string>()));
  EXPECT_CALL(spd_reader, HandleSample(_, _)).Times(0);
  EXPECT_EQ("The input contained mismatched line endings",
            spd_reader.ReadFrom(input).error());
}

TEST(SpdReader, UnparsableToken) {
  std::ifstream input = OpenRunfile("unparsable_token.spd");

  MockSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(_)).Times(0);
  EXPECT_CALL(spd_reader, HandleSample(_, _)).Times(0);
  EXPECT_EQ("The input contained an unparsable token",
            spd_reader.ReadFrom(input).error());
}

TEST(SpdReader, OddNumberOfTokens) {
  std::ifstream input = OpenRunfile("odd_number_of_tokens.spd");

  MockSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(_)).Times(0);
  EXPECT_CALL(spd_reader, HandleSample(_, _)).Times(0);
  EXPECT_EQ("The input contained an odd number of tokens",
            spd_reader.ReadFrom(input).error());
}

TEST(SpdReader, ReturnsCommentError) {
  std::ifstream input = OpenRunfile("comment_only.spd");

  MockSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(_))
      .WillOnce(Return(std::unexpected("error")));
  EXPECT_CALL(spd_reader, HandleSample(_, _)).Times(0);
  EXPECT_EQ("error", spd_reader.ReadFrom(input).error());
}

TEST(SpdReader, ReturnsSampleError) {
  std::ifstream input = OpenRunfile("valid_unix.spd");

  MockSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(_)).Times(0);
  EXPECT_CALL(spd_reader, HandleSample(_, _))
      .WillOnce(Return(std::unexpected("error")));
  EXPECT_EQ("error", spd_reader.ReadFrom(input).error());
}

TEST(SpdReader, Empty) {
  std::ifstream input = OpenRunfile("empty.spd");

  MockSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(_)).Times(0);
  EXPECT_CALL(spd_reader, HandleSample(_, _)).Times(0);
  EXPECT_TRUE(spd_reader.ReadFrom(input));
}

TEST(SpdReader, SingleLine) {
  std::ifstream input = OpenRunfile("single_line.spd");

  MockSpdReader spd_reader;

  {
    InSequence sequence;
    EXPECT_CALL(spd_reader, HandleSample(0.125, 0.25))
        .WillOnce(Return(std::expected<void, std::string>()));
    EXPECT_CALL(spd_reader, HandleSample(0.375, 0.5))
        .WillOnce(Return(std::expected<void, std::string>()));
    EXPECT_CALL(spd_reader, HandleComment("Comment"))
        .WillOnce(Return(std::expected<void, std::string>()));
  }

  EXPECT_TRUE(spd_reader.ReadFrom(input));
}

TEST(SpdReader, CommentOnly) {
  std::ifstream input = OpenRunfile("comment_only.spd");

  MockSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment("Comment"))
      .WillOnce(Return(std::expected<void, std::string>()));
  EXPECT_CALL(spd_reader, HandleSample(_, _)).Times(0);

  EXPECT_TRUE(spd_reader.ReadFrom(input));
}

TEST(SpdReader, RawWithComments) {
  std::ifstream input = OpenRunfile("raw_with_comments.spd");

  MockSpdReader spd_reader;

  {
    InSequence sequence;
    EXPECT_CALL(spd_reader, HandleComment(""))
        .WillOnce(Return(std::expected<void, std::string>()));
    EXPECT_CALL(spd_reader, HandleComment("Comment"))
        .WillOnce(Return(std::expected<void, std::string>()));
    EXPECT_CALL(spd_reader, HandleSample(0.125, 2.0))
        .WillOnce(Return(std::expected<void, std::string>()));
    EXPECT_CALL(spd_reader, HandleComment("2.0"))
        .WillOnce(Return(std::expected<void, std::string>()));
    EXPECT_CALL(spd_reader, HandleSample(-1.0, 2.0))
        .WillOnce(Return(std::expected<void, std::string>()));
    EXPECT_CALL(spd_reader, HandleSample(0.125, 3.0))
        .WillOnce(Return(std::expected<void, std::string>()));
    EXPECT_CALL(spd_reader, HandleComment("3.0 #"))
        .WillOnce(Return(std::expected<void, std::string>()));
  }

  EXPECT_TRUE(spd_reader.ReadFrom(input));
}

TEST(SpdReader, ValidMac) {
  std::ifstream input = OpenRunfile("valid_mac.spd");

  MockSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(_)).Times(0);

  {
    InSequence sequence;
    EXPECT_CALL(spd_reader, HandleSample(1.0, 2.0))
        .WillOnce(Return(std::expected<void, std::string>()));
    EXPECT_CALL(spd_reader, HandleSample(3.0, 4.0))
        .WillOnce(Return(std::expected<void, std::string>()));
  }

  EXPECT_TRUE(spd_reader.ReadFrom(input));
}

TEST(SpdReader, ValidUnix) {
  std::ifstream input = OpenRunfile("valid_unix.spd");

  MockSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(_)).Times(0);

  {
    InSequence sequence;
    EXPECT_CALL(spd_reader, HandleSample(1.0, 2.0))
        .WillOnce(Return(std::expected<void, std::string>()));
    EXPECT_CALL(spd_reader, HandleSample(3.0, 4.0))
        .WillOnce(Return(std::expected<void, std::string>()));
  }

  EXPECT_TRUE(spd_reader.ReadFrom(input));
}

TEST(SpdReader, ValidWindows) {
  std::ifstream input = OpenRunfile("valid_windows.spd");

  MockSpdReader spd_reader;
  EXPECT_CALL(spd_reader, HandleComment(_)).Times(0);

  {
    InSequence sequence;
    EXPECT_CALL(spd_reader, HandleSample(1.0, 2.0))
        .WillOnce(Return(std::expected<void, std::string>()));
    EXPECT_CALL(spd_reader, HandleSample(3.0, 4.0))
        .WillOnce(Return(std::expected<void, std::string>()));
  }

  EXPECT_TRUE(spd_reader.ReadFrom(input));
}

}  // namespace
}  // namespace libspd