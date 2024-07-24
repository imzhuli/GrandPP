#pragma once
#include "./base.hpp"

struct xRegionId {
	xCountryId CountryId;
	xCityId    CityId;
};

struct xRegionInfo {
	xRegionId RegionId;

	string CountryName;
	string CityName;
	string ShortCityName;
};
