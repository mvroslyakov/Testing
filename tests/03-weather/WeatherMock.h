//
// Created by Pavel Akhtyamov on 02.05.2020.
//

#pragma once

#include <gmock/gmock.h>
#include <Weather.h>

class WeatherMock : public Weather {
 public:
  MOCK_METHOD(float, GetTemperature, (const std::string&), (override));
  //MOCK_METHOD(json, GetResponseForCity, (const std::string &city, const cpr::Url& url), (const));
  //MOCK_METHOD(float, GetTomorrowTemperature, (const std::string&), (override));
  //MOCK_METHOD(void, SetApiKey, (const std::string&), (override));
  //MOCK_METHOD(std::string, GetDifferenceString, (const std::string&, const std::string& ), (override));
  //MOCK_METHOD(std::string, GetTomorrowDiff, (const std::string&), (override));
};




