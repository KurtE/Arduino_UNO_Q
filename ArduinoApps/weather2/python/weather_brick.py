# SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
#
# SPDX-License-Identifier: MPL-2.0

import requests
import json
from dataclasses import dataclass
import importlib.resources
import os
import pathlib

#from arduino.app_utils import brick

city_api_url = "https://geocoding-api.open-meteo.com/v1/search"
forecast_api_url = "https://api.open-meteo.com/v1/forecast"


@dataclass(frozen=True)
class WeatherData:
    """Weather forecast data with standardized codes and categories.

    Attributes:
        code (int): WMO weather code representing specific weather conditions.
        description (str): Human-readable weather description (e.g., "Partly cloudy", "Heavy rain").
        category (str): Simplified weather category: "sunny", "cloudy", "rainy", "snowy", or "foggy".
    """

    code: int
    description: str
    category: str
    cur_temp: float
    daily_precip: float


# The weather codes have been taken from here: https://www.nodc.noaa.gov/archive/arc0021/0002199/1.1/data/0-data/HTML/WMO-CODE/WMO4677.HTM
#with importlib.resources.open_text(os.path.abspath(os.getcwd()), "weather_data.json") as file:
with open("/app/python/weather_data.json") as file:
   weather_data = json.load(file)
 

#@brick
class WeatherForecast:
    """Weather forecast service using the open-meteo.com API.

    Provides weather forecasts by city name or geographic coordinates with no API key required.
    Returns structured weather data with WMO codes, descriptions, and simplified categories.
    """

    def get_forecast_by_city(self, city: str, timezone: str = "GMT", forecast_days: int = 1) -> WeatherData:
        """Get weather forecast for a specified city.

        Args:
            city (str): City name (e.g., "London", "New York").
            timezone (str): Timezone identifier. Defaults to "GMT".
            forecast_days (int): Number of days to forecast. Defaults to 1.

        Returns:
            WeatherData: Weather forecast with code, description, and category.

        Raises:
            RuntimeError: If city lookup or weather data retrieval fails.
        """
        try:
            response = requests.get(city_api_url, params={"name": city})
        except:
            raise RuntimeError("Failed to look city up")

        data = response.json()
        results = data.get("results", [])
        if results:
            result = results[0]
        else:
            raise RuntimeError("City not found")

        return self.get_forecast_by_coords(result["latitude"], result["longitude"], timezone=timezone, forecast_days=forecast_days)

    def get_forecast_by_coords(self, latitude: str, longitude: str, timezone: str = "GMT", forecast_days: int = 1) -> WeatherData:
        """Get weather forecast for specific coordinates.

        Args:
            latitude (str): Latitude coordinate (e.g., "45.0703").
            longitude (str): Longitude coordinate (e.g., "7.6869").
            timezone (str): Timezone identifier. Defaults to "GMT".
            forecast_days (int): Number of days to forecast. Defaults to 1.

            Returns:"current": ["precipitation", "temperature_2m", "wind_direction_10m", "wind_speed_10m"]

      WeatherData: Weather forecast with code, description, and category.

        Raises:
            RuntimeError: If weather data retrieval fails.
        """

        params = {
            "latitude": latitude,
            "longitude": longitude,
            "timezone": timezone,
            "daily": ["weather_code","precipitation_sum"],
            "forecast_days": f"{forecast_days}",
	        "timezone": "America/Los_Angeles",
            "current": "temperature_2m",
	        "wind_speed_unit": "kn",
	        "temperature_unit": "fahrenheit",
	        "precipitation_unit": "inch",
            "format": "json",
        }
        try:
            response = requests.get(forecast_api_url, params=params)
        except:
            raise RuntimeError("Failed to get weather data")

      
        data = response.json()
        #print(f"{data}")
        if response.status_code != 200:
            raise RuntimeError(f"Failed to get weather data: {data.get('reason', 'Unknown error')}")

        if "daily" not in data or "weather_code" not in data["daily"]:
            raise RuntimeError("Invalid response format")

        if "current" not in data or "temperature_2m" not in data["current"]:
            raise RuntimeError("Invalid response format temperature_2m")

        if "daily" not in data or "precipitation_sum" not in data["daily"]:
            raise RuntimeError("Invalid response format precipitation_sum")


        # This is the exact format of the response:
         # {'latitude': 48.0, 
         #  'longitude': -122.0, 
         #  'generationtime_ms': 0.11718273162841797, 
         #  'utc_offset_seconds': -28800, 
         #  'timezone': 'America/Los_Angeles', 
         #  'timezone_abbreviation': 'GMT-8', 
         #  'elevation': 7.0, 
         #  'current_units': {'time': 'iso8601', 'interval': 'seconds', 'temperature_2m': '°F'},
         #  'current': {'time': '2025-11-17T10:30', 'interval': 900, 'temperature_2m': 48.5},
         #  'daily_units': {'time': 'iso8601', 'weather_code': 'wmo code', 'precipitation_sum': 'inch'},
         #  'daily': {'time': ['2025-11-17'], 'weather_code': [51], 'precipitation_sum': [0.035]}
         # }

        weather_code = data["daily"]["weather_code"][forecast_days - 1]
        temp_2m = data["current"]["temperature_2m"]
        precip = data["daily"]["precipitation_sum"]
      
        return WeatherData(
            code=weather_code,
            description=weather_data[weather_code]["description"],
            category=weather_data[weather_code]["category"],
            cur_temp=temp_2m,
            daily_precip=precip[0],
        )

    def process(self, item):
        """Process dictionary input to get weather forecast.

        This method checks if the item is a dictionary with latitude and longitude or city name.
        If it is a dictionary with latitude and longitude, it retrieves the weather forecast by coordinates.
        If it is a dictionary with city name, it retrieves the weather forecast by city.

        Args:
            item (dict): Dictionary with either "city" key or "latitude"/"longitude" keys.

        Returns:
            WeatherData | dict: WeatherData object if valid input provided, empty dict if input format is invalid.

        Raises:
            CityLookupError: If the city is not found.
            WeatherForecastLookupError: If the weather forecast cannot be retrieved.
        """
        output = {}
        if isinstance(item, dict):
            if "latitude" in item and "longitude" in item:
                return self.get_forecast_by_coords(item["latitude"], item["longitude"])
            elif "city" in item:
                return self.get_forecast_by_city(item["city"])

        return output


class CityLookupError(Exception):
    """Exception raised when the city lookup (geocoding) fails."""

    pass


class WeatherForecastLookupError(Exception):
    """Exception raised when the weather forecast lookup fails."""

    pass