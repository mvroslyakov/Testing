//
// Created by akhtyamovpavel on 5/1/20.
//

#include "LeapTestCase.h"

#include <Functions.h>

TEST(LeapTest, Leap) {
  EXPECT_TRUE(IsLeap(2020));
  EXPECT_FALSE(IsLeap(2021));
  ASSERT_ANY_THROW(IsLeap(0));
  EXPECT_FALSE(IsLeap(100));
  EXPECT_TRUE(IsLeap(400));
  ASSERT_ANY_THROW(IsLeap(-1));
}


TEST(LeapTest, GetMonthDays) {
  EXPECT_EQ(31, GetMonthDays(2021, 5));
  EXPECT_EQ(30, GetMonthDays(2021, 4));
  EXPECT_EQ(28, GetMonthDays(2021, 2));
  EXPECT_EQ(29, GetMonthDays(2020, 2));
  ASSERT_ANY_THROW(GetMonthDays(2020, -1));
  ASSERT_ANY_THROW(GetMonthDays(2020, 13));
  EXPECT_EQ(30, GetMonthDays(2021, 11));
  EXPECT_EQ(31, GetMonthDays(2021, 12));
  EXPECT_EQ(31, GetMonthDays(2021, 1));
  EXPECT_EQ(30, GetMonthDays(2021, 6));
  EXPECT_EQ(31, GetMonthDays(2021, 3));
  EXPECT_EQ(31, GetMonthDays(2021, 7));
  EXPECT_EQ(31, GetMonthDays(2021, 8));
  EXPECT_EQ(30, GetMonthDays(2021, 9));
  EXPECT_EQ(31, GetMonthDays(2021, 10));
  ASSERT_NO_THROW(GetMonthDays(2020, 1));
}
