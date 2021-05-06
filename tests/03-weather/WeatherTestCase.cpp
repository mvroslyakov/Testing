//
// Created by Pavel Akhtyamov on 02.05.2020.
//

#include "WeatherTestCase.h"
#include "WeatherMock.h"
#include <httpmockserver/mock_server.h>
#include <httpmockserver/test_environment.h>
#include <cmath>

using ::testing::Return;
using ::testing::_;

TEST(WeatherMockTest, Base) {
  Weather weather;
  ASSERT_ANY_THROW(weather.GetTemperature("fjdslkfjsl"));
  
  testing::NiceMock<WeatherMock> weather_mock;
  //WeatherMock weather_mock;
  const cpr::Url kBaseUrl = cpr::Url{"http://api.openweathermap.org/data/2.5/weather"};
  const cpr::Url kForecastUrl = cpr::Url{"http://api.openweathermap.org/data/2.5/forecast"};
  std::string key_api = "96a45e9fd38501c7e6e9d513b1398795";
  weather_mock.SetApiKey(key_api);
  cpr::Response london_resp = cpr::Get(
      kBaseUrl, cpr::Parameters{
          {"q", "London"},
          {"appid", key_api},
          {"units", "metric"}
      }
  );
  cpr::Response moscow_resp = cpr::Get(
      kBaseUrl, cpr::Parameters{
          {"q", "Moscow"},
          {"appid", key_api},
          {"units", "metric"}
      }
  );
  ON_CALL(
      weather_mock,
      Get("London", kBaseUrl)
  ).WillByDefault(Return(london_resp));
  ON_CALL(
      weather_mock,
      Get("Moscow", kBaseUrl)
  ).WillByDefault(Return(moscow_resp));
  auto moscow_result = json::parse(moscow_resp.text);
  auto london_result = json::parse(london_resp.text);
  float answer_moscow = moscow_result["main"]["temp"];
  float answer_london = london_result["main"]["temp"];
  
  
  float temp_london = weather_mock.GetTemperature("London");
  float temp_moscow = weather_mock.GetTemperature("Moscow");
  ASSERT_EQ(answer_moscow, temp_moscow);
  ASSERT_EQ(answer_london, temp_london);
  
  std::stringstream temp_ML;
  if (answer_moscow - answer_london < 0) {
    temp_ML << "Weather in " << "Moscow" << " ";
    temp_ML << "is colder than in " << "London" << " by " << int(answer_london - answer_moscow) << " degrees";
  } else {
    temp_ML << "Weather in " << "Moscow" << " ";
    temp_ML << "is warmer than in " << "London" << " by " << int(answer_moscow - answer_london) << " degrees";
  }
  auto res_ML = weather_mock.GetDifferenceString("Moscow", "London");
  ASSERT_EQ(temp_ML.str(), res_ML);
  
  std::stringstream temp_LM;
  if (answer_london - answer_moscow < 0) {
    temp_LM << "Weather in " << "London" << " ";
    temp_LM << "is colder than in " << "Moscow" << " by " << int(answer_moscow - answer_london) << " degrees";
  } else {
    temp_LM << "Weather in " << "London" << " ";
    temp_LM << "is warmer than in " << "Moscow" << " by " << int(answer_moscow - answer_london) << " degrees";
  }
  auto res_LM = weather_mock.GetDifferenceString("London", "Moscow");
  ASSERT_EQ(temp_LM.str(), res_LM);
}

TEST(WeatherMockTest, Forecast) {
  testing::NiceMock<WeatherMock> weather_mock;
  const cpr::Url kBaseUrl = cpr::Url{"http://api.openweathermap.org/data/2.5/weather"};
  const cpr::Url kForecastUrl = cpr::Url{"http://api.openweathermap.org/data/2.5/forecast"};
  std::string key_api = "96a45e9fd38501c7e6e9d513b1398795";
  weather_mock.SetApiKey(key_api);
  cpr::Response london_resp_base = cpr::Get(
      kBaseUrl, cpr::Parameters{
          {"q", "London"},
          {"appid", key_api},
          {"units", "metric"}
      }
  );
  cpr::Response london_resp_fore = cpr::Get(
      kForecastUrl, cpr::Parameters{
          {"q", "London"},
          {"appid", key_api},
          {"units", "metric"}
      }
  );
  ON_CALL(
      weather_mock,
      Get("London", kBaseUrl)
  ).WillByDefault(Return(london_resp_base));
  ON_CALL(
      weather_mock,
      Get("London", kForecastUrl)
  ).WillByDefault(Return(london_resp_fore));
  auto london_result_base = json::parse(london_resp_base.text);
  auto london_result_fore = json::parse(london_resp_fore.text);
  float answer_london_base = london_result_base["main"]["temp"];
  float answer_london_fore = london_result_fore["list"][7]["main"]["temp"];
  
  float temp_london_cur = weather_mock.GetTemperature("London");
  float temp_london_fut = weather_mock.GetTomorrowTemperature("London");
  ASSERT_EQ(temp_london_fut, answer_london_fore);
  
  std::stringstream output;

  output << "The weather in " << "London" << " tomorrow will be ";

  std::string response;
  float diff = answer_london_fore - answer_london_base;

  if (diff > 3) {
    response = "much warmer";
  } else if (diff > 0.5) {
    response = "warmer";
  } else if (diff < -3) {
    response = "much colder";
  } else if (diff < -0.5) {
    response = "colder";
  } else {
    response = "the same";
  }

  output << response << " than today.";
  auto res = weather_mock.GetTomorrowDiff("London");
  ASSERT_EQ(output.str(), res);
}

TEST(WeatherMockTest, AllBranchForecast) {
  testing::NiceMock<FutWeatherMock> weather_mock;
  ON_CALL(
      weather_mock,
      GetTomorrowTemperature("London")
  ).WillByDefault(Return(20));
  ON_CALL(
      weather_mock,
      GetTemperature("London")
  ).WillByDefault(Return(10));
  ASSERT_EQ("The weather in London tomorrow will be much warmer than today.", weather_mock.GetTomorrowDiff("London"));
  
  ON_CALL(
      weather_mock,
      GetTomorrowTemperature("London")
  ).WillByDefault(Return(20));
  ON_CALL(
      weather_mock,
      GetTemperature("London")
  ).WillByDefault(Return(19.4));
  ASSERT_EQ("The weather in London tomorrow will be warmer than today.", weather_mock.GetTomorrowDiff("London"));
  
  ON_CALL(
      weather_mock,
      GetTomorrowTemperature("London")
  ).WillByDefault(Return(19.4));
  ON_CALL(
      weather_mock,
      GetTemperature("London")
  ).WillByDefault(Return(20));
  ASSERT_EQ("The weather in London tomorrow will be colder than today.", weather_mock.GetTomorrowDiff("London"));
  
  ON_CALL(
      weather_mock,
      GetTomorrowTemperature("London")
  ).WillByDefault(Return(10));
  ON_CALL(
      weather_mock,
      GetTemperature("London")
  ).WillByDefault(Return(20));
  ASSERT_EQ("The weather in London tomorrow will be much colder than today.", weather_mock.GetTomorrowDiff("London"));
  
  ON_CALL(
      weather_mock,
      GetTomorrowTemperature("London")
  ).WillByDefault(Return(20));
  ON_CALL(
      weather_mock,
      GetTemperature("London")
  ).WillByDefault(Return(20));
  ASSERT_EQ("The weather in London tomorrow will be the same than today.", weather_mock.GetTomorrowDiff("London"));
}
