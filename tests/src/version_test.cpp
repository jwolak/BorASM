/*
 * Version system test file for BorASM project
 * Tests version extraction and utility functions
 */

#include "version.h"

#include <gtest/gtest.h>

// Test version constants
TEST(VersionTest, VersionConstants) {
    EXPECT_GE(BorASM::Version::MAJOR, 0);
    EXPECT_GE(BorASM::Version::MINOR, 0);
    EXPECT_GE(BorASM::Version::PATCH, 0);
    EXPECT_GT(BorASM::Version::BUILD, 0);
}

// Test version strings
TEST(VersionTest, VersionStrings) {
    std::string version = BorASM::Version::GetVersionString();
    EXPECT_FALSE(version.empty());
    EXPECT_NE(version.find('.'), std::string::npos);  // Should contain dots

    std::string full_version = BorASM::Version::GetFullVersionString();
    EXPECT_FALSE(full_version.empty());
    EXPECT_GT(full_version.length(), version.length());  // Should be longer than basic version
}

// Test version format
TEST(VersionTest, VersionFormat) {
    // Test that version string follows semantic versioning pattern
    std::string version = BorASM::Version::GetVersionString();

    // Count dots - should have exactly 2 for major.minor.patch
    size_t dot_count = std::count(version.begin(), version.end(), '.');
    EXPECT_EQ(dot_count, 2);

    // Verify version components match string representation
    std::string expected_version =
        std::to_string(BorASM::Version::MAJOR) + "." + std::to_string(BorASM::Version::MINOR) + "." + std::to_string(BorASM::Version::PATCH);
    EXPECT_EQ(version, expected_version);
}

// Test build information
TEST(VersionTest, BuildInformation) {
    std::string build_info = BorASM::Version::GetBuildInfo();
    EXPECT_FALSE(build_info.empty());
    EXPECT_NE(build_info.find(' '), std::string::npos);  // Should contain date and time

    // Test individual build components
    EXPECT_STRNE(BorASM::Version::BUILD_DATE, "");
    EXPECT_STRNE(BorASM::Version::BUILD_TIME, "");
    EXPECT_STRNE(BorASM::Version::BUILD_YEAR, "");
}

// Test Git information
TEST(VersionTest, GitInformation) {
    std::string git_info = BorASM::Version::GetGitInfo();
    EXPECT_FALSE(git_info.empty());

    // Git info should contain @ symbol (branch@hash format)
    EXPECT_NE(git_info.find('@'), std::string::npos);

    // Individual components should not be empty (even if "unknown")
    EXPECT_STRNE(BorASM::Version::GIT_BRANCH, "");
    EXPECT_STRNE(BorASM::Version::GIT_COMMIT_HASH, "");
}

// Test version comparison functions
TEST(VersionTest, VersionComparison) {
    // Test version at least
    EXPECT_TRUE(BorASM::Version::IsVersionAtLeast(0, 0, 0));

    // Test exact version match
    EXPECT_TRUE(BorASM::Version::IsExactVersion(BorASM::Version::MAJOR, BorASM::Version::MINOR, BorASM::Version::PATCH));

    // Test version not matching
    EXPECT_FALSE(BorASM::Version::IsExactVersion(999, 999, 999));
}

// Test complete version info
TEST(VersionTest, CompleteVersionInfo) {
    std::string complete_info = BorASM::Version::GetCompleteVersionInfo();
    EXPECT_FALSE(complete_info.empty());

    // Should contain version
    std::string version = BorASM::Version::GetVersionString();
    EXPECT_NE(complete_info.find(version), std::string::npos);

    // Should contain build info indicators
    EXPECT_NE(complete_info.find("built"), std::string::npos);
}

// Test build type detection
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

// Test version string constants are not null
TEST(VersionTest, VersionConstantsNotNull) {
    EXPECT_STRNE(BorASM::Version::VERSION_STRING, "");
    EXPECT_STRNE(BorASM::Version::FULL_VERSION_STRING, "");
}

// Test that all version components are reasonable
TEST(VersionTest, ReasonableVersionComponents) {
    EXPECT_LE(BorASM::Version::MAJOR, 1000);  // Reasonable upper bound
    EXPECT_LE(BorASM::Version::MINOR, 1000);
    EXPECT_LE(BorASM::Version::PATCH, 1000);
    EXPECT_LE(BorASM::Version::BUILD, 400);  // Days in year upper bound
}