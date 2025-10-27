#include "version.h"

#include <gtest/gtest.h>

TEST(VersionTest, VersionConstants) {
    EXPECT_GE(BorASM::Version::MAJOR, 0);
    EXPECT_GE(BorASM::Version::MINOR, 0);
    EXPECT_GE(BorASM::Version::PATCH, 0);
    EXPECT_GT(BorASM::Version::BUILD, 0);
}

TEST(VersionTest, VersionStrings) {
    std::string version = BorASM::Version::GetVersionString();
    EXPECT_FALSE(version.empty());
    EXPECT_NE(version.find('.'), std::string::npos);
    std::string full_version = BorASM::Version::GetFullVersionString();
    EXPECT_FALSE(full_version.empty());
    EXPECT_GT(full_version.length(), version.length());
}

TEST(VersionTest, VersionFormat) {
    std::string version = BorASM::Version::GetVersionString();
    size_t dot_count = std::count(version.begin(), version.end(), '.');
    EXPECT_EQ(dot_count, 2);
    std::string expected_version =
        std::to_string(BorASM::Version::MAJOR) + "." + std::to_string(BorASM::Version::MINOR) + "." + std::to_string(BorASM::Version::PATCH);
    EXPECT_EQ(version, expected_version);
}

TEST(VersionTest, BuildInformation) {
    std::string build_info = BorASM::Version::GetBuildInfo();
    EXPECT_FALSE(build_info.empty());
    EXPECT_NE(build_info.find(' '), std::string::npos);
    EXPECT_STRNE(BorASM::Version::BUILD_DATE, "");
    EXPECT_STRNE(BorASM::Version::BUILD_TIME, "");
    EXPECT_STRNE(BorASM::Version::BUILD_YEAR, "");
}

TEST(VersionTest, GitInformation) {
    std::string git_info = BorASM::Version::GetGitInfo();
    EXPECT_FALSE(git_info.empty());
    EXPECT_NE(git_info.find('@'), std::string::npos);
    EXPECT_STRNE(BorASM::Version::GIT_BRANCH, "");
    EXPECT_STRNE(BorASM::Version::GIT_COMMIT_HASH, "");
}

TEST(VersionTest, VersionComparison) {
    EXPECT_TRUE(BorASM::Version::IsVersionAtLeast(0, 0, 0));
    EXPECT_TRUE(BorASM::Version::IsExactVersion(BorASM::Version::MAJOR, BorASM::Version::MINOR, BorASM::Version::PATCH));
    EXPECT_FALSE(BorASM::Version::IsExactVersion(999, 999, 999));
}

TEST(VersionTest, CompleteVersionInfo) {
    std::string complete_info = BorASM::Version::GetCompleteVersionInfo();
    EXPECT_FALSE(complete_info.empty());

    std::string version = BorASM::Version::GetVersionString();
    EXPECT_NE(complete_info.find(version), std::string::npos);
    EXPECT_NE(complete_info.find("built"), std::string::npos);
}

TEST(VersionTest, BuildType) {
    EXPECT_STRNE(BorASM::Version::BUILD_TYPE, "");

    std::string build_type(BorASM::Version::BUILD_TYPE);
    EXPECT_TRUE(build_type == "Debug" || build_type == "Release");

#ifdef NDEBUG
    EXPECT_TRUE(BorASM::Version::IS_RELEASE_BUILD);
    EXPECT_STREQ(BorASM::Version::BUILD_TYPE, "Release");
#else
    EXPECT_FALSE(BorASM::Version::IS_RELEASE_BUILD);
    EXPECT_STREQ(BorASM::Version::BUILD_TYPE, "Debug");
#endif
}

TEST(VersionTest, VersionConstantsNotNull) {
    EXPECT_STRNE(BorASM::Version::VERSION_STRING, "");
    EXPECT_STRNE(BorASM::Version::FULL_VERSION_STRING, "");
}

TEST(VersionTest, ReasonableVersionComponents) {
    EXPECT_LE(BorASM::Version::MAJOR, 1000);
    EXPECT_LE(BorASM::Version::MINOR, 1000);
    EXPECT_LE(BorASM::Version::PATCH, 1000);
    EXPECT_LE(BorASM::Version::BUILD, 400);
}