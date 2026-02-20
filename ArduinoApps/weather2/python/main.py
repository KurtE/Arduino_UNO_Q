# SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
#
# SPDX-License-Identifier: MPL-2.0

from weather_brick import WeatherForecast
from arduino.app_utils import *
try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import ConfigParser  # ver. < 3.0

config = ConfigParser()

forecaster = WeatherForecast()

last_data_list: list[float] = {}

def get_weather_city() ->str:
    try:
        city_name = config.get('location', 'city')
    except:
        city_name = ""
    return city_name
    

def get_weather_forecast(city: str) -> str:
    global last_data_list
    forecast = forecaster.get_forecast_by_city(city)
    print(f"Weather forecast for {city}: {forecast.description}: {forecast.cur_temp}: {forecast.daily_precip}: {forecast.cur_wind_speed}: {forecast.cur_wind_dir}")
    #print(f"Weather forecast for {city}: {forecast.description}")
    last_data_list = [forecast.cur_temp, forecast.daily_precip, forecast.cur_wind_speed, float(forecast.cur_wind_dir)]
    #print(last_data_list)
    return forecast.category

def get_weather_data() -> list:
    global last_data_list
    return last_data_list

try:
    config.read('weather.ini')
    city_name = config.get('location', 'city')
except:
    config.add_section('location')
    config.set('location', 'city', 'Los Angeles')
    # save to a file
    with open('weather.ini', 'w') as configfile:
        config.write(configfile)
    city_name = config.get('location', 'city')
print(city_name)



Bridge.provide("get_weather_forecast", get_weather_forecast)

Bridge.provide("get_weather_data", get_weather_data)

Bridge.provide("get_weather_city", get_weather_city)

App.run()
