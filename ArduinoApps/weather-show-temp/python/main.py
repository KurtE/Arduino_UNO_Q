# SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
#
# SPDX-License-Identifier: MPL-2.0

from weather_brick import WeatherForecast
from arduino.app_utils import *

forecaster = WeatherForecast()

int last_temp

def get_weather_forecast(city: str) -> str:
    forecast = forecaster.get_forecast_by_city(city)
    print(f"Weather forecast for {city}: {forecast.description}: {forecast.cur_temp}")
    #print(f"Weather forecast for {city}: {forecast.description}")
    last_temp = forcast.cur_temp
    return forecast.category

def get_weather_temp() -> int:
    forecast = forecaster.get_forecast_by_city(city)
    #print(f"Weather forecast for {city}: {forecast.description}")
    return last_temp



Bridge.provide("get_weather_forecast", get_weather_forecast)
Bridge.provide("get_weather_temp", get_weather_temp)

App.run()
