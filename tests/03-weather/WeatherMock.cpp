//
// Created by Pavel Akhtyamov on 02.05.2020.
//

#include "WeatherMock.h"
using ::testing::Return;
using ::testing::_;

TEST(WeatherMockTest, All) {
  testing::NiceMock<WeatherMock> weather;
  std::string key_api = "96a45e9fd38501c7e6e9d513b1398795";
  weather.SetApiKey(key_api);
  ON_CALL(
      weather,
      GetTemperature("London")
  ).WillByDefault(Return(14));
  
//  EXPECT_CALL(weather, GetResponseForCity(_,_)).Times(1);


  auto temp = weather.GetTemperature("London");
  ASSERT_EQ(14, temp);
}

