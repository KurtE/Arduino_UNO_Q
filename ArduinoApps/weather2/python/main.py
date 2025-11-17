# SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
#
# SPDX-License-Identifier: MPL-2.0

from weather_brick import WeatherForecast
from arduino.app_utils import *

forecaster = WeatherForecast()

last_temp = 0

def get_weather_forecast(city: str) -> str:
    global last_temp
    forecast = forecaster.get_forecast_by_city(city)
    print(f"Weather forecast for {city}: {forecast.description}: {forecast.cur_temp}")
    #print(f"Weather forecast for {city}: {forecast.description}")
    last_temp = forecast.cur_temp
    return forecast.category

def get_weather_temp() -> int:
    global last_temp
    print(f"last_temp: {last_temp}")
    return int(last_temp)



Bridge.provide("get_weather_forecast", get_weather_forecast)

Bridge.provide("get_weather_temp", get_weather_temp)

App.run()
