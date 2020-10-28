#pragma once
#include "Main.hpp"

using namespace WeatherApp;

int main() {
	Main->Start();
}

void main::Start() {
	string city;
	string country;
	std::cout << "\nWelcome to WeatherApp\n";
	std::cout << "Please input a city: ";
	std::cin >> city;
	std::cout << "Please input a country: ";
	std::cin >> country;

	GetWeather(city, country);
}

float coords[2];
void main::GetWeather(string city, string country) {
	GetCoords(city, country);

	Document reader;
	int coord1 = (int)coords[0];
	int coord2 = (int)coords[1];
	string lat = std::to_string(coord1);
	string lon = std::to_string(coord2);
	string fields = "temp%2Cfeels_like%2Chumidity%2Cwind_speed%2Cwind_direction%2Cvisibility%2Cweather_code";
	string url = "https://api.climacell.co/v3/weather/realtime?lat="+lat+"&lon="+lon+"&fields="+fields+"&apikey=hQz4BLHmuM8xceqw4UNaGy6jMcsa7x82";
	cpr::Response r = cpr::Get(cpr::Url{ url });

	if (reader.Parse(r.text.c_str()).HasParseError()) {
		std::cout << "Error parsing response\n";
		Start();
		return;
	}

	string weather;
	// Temp info
	double temp;
	double tempfeel;
	double humidity;
	// Wind info
	double speed;
	double degree;
	//
	int visibility;

	weather = reader["weather_code"]["value"].GetString();

	temp = reader["temp"]["value"].GetDouble();
	tempfeel = reader["feels_like"]["value"].GetDouble();
	humidity = reader["humidity"]["value"].GetDouble();

	speed = reader["wind_speed"]["value"].GetDouble();
	degree = reader["wind_direction"]["value"].GetDouble();

	visibility = reader["visibility"]["value"].GetInt();

		double tempinfo[] = {
			temp,
			tempfeel,
		};

		SetInfo(weather, tempinfo, humidity, speed, degree, visibility);
}

void main::GetCoords(string city, string country) {
	Document reader;
	std::unique_ptr<Geocoder> g(new Geocoder("819e6368ea4048018a264b18473a15d4"));

	if (reader.Parse(g->geocode(city + " " + country).c_str()).HasParseError()) {
		std::cout << "Error parsing response coords\n";
		Start();
		return;
	}
	else {
		coords[0] = reader["results"][0]["geometry"]["lat"].GetFloat();
		coords[1] = reader["results"][0]["geometry"]["lng"].GetFloat();
	}
}

void main::SetInfo(string weather, double tempinfo[], double humidity, double windspeed, double winddegree, int visibility) {
	const char* WindDirection;
	if (winddegree >= 180) {
		if (winddegree >= 270) {
			if (winddegree <= 330 && winddegree >= 300) {
				WindDirection = "North West";
			}
			else {
				WindDirection = "North";
			}
		}
		else {
			if (winddegree <= 240 && winddegree >= 210) {
				WindDirection = "South West";
			}
			else {
				WindDirection = "South";
			}
		}
	}
	else {
		if (winddegree >= 90) {
			if (winddegree <= 150 && winddegree >= 120) {
				WindDirection = "South East";
			}
			else {
				WindDirection = "South";
			}
		}
		else {
			if (winddegree <= 60 && winddegree >= 30) {
				WindDirection = "North East";
			}
			else {
				WindDirection = "North";
			}
		}
	}

	std::cout << "\nCurrent weather: \n";
	std::cout << weather << " Temp: " << tempinfo[0] << "\n";
	std::cout << "\nDetailed weather: \n";
	std::cout << "Temp: " << tempinfo[0] << " Temp feels like: " << tempinfo[1] << " Humidity: " << humidity << "\n";
	std::cout << "Wind speed: " << windspeed << " Wind angle: " << WindDirection << " Visibility: " << visibility << "\n";

	Start();
}