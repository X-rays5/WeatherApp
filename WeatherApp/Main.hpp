#pragma once
#include <cpr/cpr.h>
#include <rapidjson/document.h>
#include <iostream>
#include "geocoder/geocoder.h"

using std::string;
using namespace rapidjson;

namespace WeatherApp {
	class main {
	public:
		void Start();
		void GetWeather(string city, string countrycode);
		void GetCoords(string city, string country);
		void SetInfo(string weather, double tempinfo[], double humidity, double windspeed, double winddegree, int visibility);
	};

	inline std::unique_ptr<main> Main;
}