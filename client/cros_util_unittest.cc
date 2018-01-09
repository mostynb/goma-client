// Copyright 2014 The Goma Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "cros_util.h"

#include<string>
#include<vector>

#include <gtest/gtest.h>

using std::string;

namespace devtools_goma {

TEST(CrosUtil, ParseBlacklistContents) {
  std::vector<string> expect;

  static const char* kEmpty = "";
  EXPECT_EQ(expect, ParseBlacklistContents(kEmpty));

  static const char* kCrLf = "\n\r ";
  EXPECT_EQ(expect, ParseBlacklistContents(kCrLf));

  static const char* kTmp = "/tmp";
  expect.push_back("/tmp");
  EXPECT_EQ(expect, ParseBlacklistContents(kTmp));
  expect.clear();

  static const char* kTmpWithWhiteSpaces = "\r\n /tmp\r\n ";
  expect.push_back("/tmp");
  EXPECT_EQ(expect, ParseBlacklistContents(kTmpWithWhiteSpaces));
  expect.clear();

  static const char* kTwoDirs = "\n/example\n/example2\n";
  expect.push_back("/example");
  expect.push_back("/example2");
  EXPECT_EQ(expect, ParseBlacklistContents(kTwoDirs));
  expect.clear();

  static const char* kTwoDirsWithSpaces =
      "\n/example \r\n \r\n \r\n /example2\n";
  expect.push_back("/example");
  expect.push_back("/example2");
  EXPECT_EQ(expect, ParseBlacklistContents(kTwoDirsWithSpaces));
  expect.clear();

  static const char* kDirnameWithSpace = "\n/dirname with space \r\n";
  expect.push_back("/dirname with space");
  EXPECT_EQ(expect, ParseBlacklistContents(kDirnameWithSpace));
  expect.clear();

  static const char* kTwoDirnamesWithSpace =
      "\n/dirname with  space \r\n /with space/part 2 \r\n";
  expect.push_back("/dirname with  space");
  expect.push_back("/with space/part 2");
  EXPECT_EQ(expect, ParseBlacklistContents(kTwoDirnamesWithSpace));
  expect.clear();
}

TEST(CrosUtil, IsBlacklisted) {
  std::vector<string> blacklist;
  blacklist.push_back("/tmp");
  EXPECT_TRUE(IsBlacklisted("/tmp", blacklist));
  blacklist.clear();

  blacklist.push_back("non-related");
  blacklist.push_back("/tmp");
  EXPECT_TRUE(IsBlacklisted("/tmp", blacklist));
  blacklist.clear();

  blacklist.push_back("/usr");
  blacklist.push_back("/tmp");
  EXPECT_TRUE(IsBlacklisted("/usr/local/etc", blacklist));
  blacklist.clear();

  blacklist.push_back("non-related");
  blacklist.push_back("/local");
  EXPECT_TRUE(IsBlacklisted("/usr/local/etc", blacklist));
  blacklist.clear();

  blacklist.push_back("non-related");
  blacklist.push_back("/etc");
  EXPECT_TRUE(IsBlacklisted("/usr/local/etc", blacklist));
  blacklist.clear();

  EXPECT_FALSE(IsBlacklisted("/tmp", blacklist));
  blacklist.clear();

  blacklist.push_back("non-related");
  EXPECT_FALSE(IsBlacklisted("/tmp", blacklist));
  blacklist.clear();

  blacklist.push_back("/opt");
  blacklist.push_back("/tmp");
  EXPECT_FALSE(IsBlacklisted("/usr/local/etc", blacklist));
  blacklist.clear();
}

TEST(CrosUtil, GetLoadAverage) {
  // Smoke test
  EXPECT_GE(GetLoadAverage(), 0.0);
}

TEST(CrosUtil, RandInt) {
  // Smoke test
  for (size_t i = 0; i < 100; ++i) {
    int r = RandInt(10, 20);
    EXPECT_LT(r, 21);
    EXPECT_GT(r, 9);
  }
  EXPECT_EQ(128, RandInt(128, 128));
}

}  // namespace devtools_goma
