# SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
#
# SPDX-License-Identifier: MPL-2.0

from weather_brick import WeatherForecast
from arduino.app_utils import *

forecaster = WeatherForecast()

last_temp = 0
last_precip: float = 0

def get_weather_forecast(city: str) -> str:
    global last_temp, last_precip
    forecast = forecaster.get_forecast_by_city(city)
    print(f"Weather forecast for {city}: {forecast.description}: {forecast.cur_temp}: {forecast.daily_precip}")
    #print(f"Weather forecast for {city}: {forecast.description}")
    last_temp = forecast.cur_temp
    last_precip = forecast.daily_precip
    return forecast.category

def get_weather_temp() -> int:
    global last_temp
    #print(f"last_temp: {last_temp}")
    return int(last_temp)

def get_weather_precip() -> int:
    global last_precip
    precip100ths = int(round(last_precip * 100))
    #print(f"last_precip: {last_precip} {precip100ths}")
    return precip100ths



Bridge.provide("get_weather_forecast", get_weather_forecast)

Bridge.provide("get_weather_temp", get_weather_temp)

Bridge.provide("get_weather_precip", get_weather_precip)


App.run()
