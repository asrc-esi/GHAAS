#!/bin/bash

case "$(uname)" in
    (Linux)
        export GHAASprocessorNum=$(nproc)
    ;;
    (Darwin)
        export GHAASprocessorNum=$(sysctl -n hw.ncpu)
    ;;
    (*)
        export GHAASprocessorNum=1
    ;;
esac

export __RGISarchiveFormat="gzipped"

function RGISprocessorNum ()
{
    local processorNum="${0}"

    export GHAASprocessorNum="${processorNum}"
}
function RGISarchiveFormat ()
{
   local format="${1}"

   case "${format}" in
        (plain)
            export __RGISarchiveFormat="plain"
        ;;
        (gzipped)
            export __RGISarchiveFormat="gzipped"
        ;;
        (netcdf)
            export __RGISarchiveFormat="netcdf"
        ;;
        (*)
            echo "Unrecognised file format:${format} in: RGISarchiveFormat"
            export __RGISarchiveFormat="plain"
        ;;
   esac
}

function RGIScase ()
{
    local caseVal="${1}"
    local  string="${2}"

    case "${caseVal}" in
        (lower)
            echo "$(echo "${string}"  | tr "[A-Z]" "[a-z]")"
        ;;
        (upper)
            echo "$(echo "${string}"  | tr "[a-z]" "[A-Z]")"
        ;;
        (*)
            echo "${string}"
        ;;
    esac
}

function RGISlookupSubject ()
{
    local variable="${1}"

	case "$(echo "${variable}" | tr "[A-Z]" "[a-z]")" in
		(air_temperature)                            #  0
			echo "AirTemperature"
		;;
		(min_air_temperature)                         #  1
			echo "AirTemperature-Min"
		;;
		(max_air_temperature)                         #  2
			echo "AirTemperature-Max"
		;;
		(diurnal_air_temperature_range)               #  3
			echo "AirTemperature-Range"
		;;
		(c_litterfall)                                #  4
			echo "C-LitterFall"
		;;
		(cell_area)                                   #  5
			echo "CellArea"
		;;
		(cell_slope)                                  #  6
			echo "CellSlope"
		;;
		(cloud_cover)                                 #  7
			echo "CloudCover"
		;;
		(confluence)                                  #  8
			echo "Confluence"
		;;
		(continents)                                  #  9
			echo "Continents"
		;;
		(countries)                                   # 10
			echo "Countries"
		;;
		(states)                                      # 11
			echo "States"
		;;
		(counties)                                    # 12
			echo "Counties"
		;;
		(cropland_fraction)                           # 13
			echo "Cropland-Fraction"
		;;
		(cropping_intensity)                          # 14
			echo "Crops-Intensity"
		;;
		(daily_precipitation_fraction)                # 15
			echo "Precipitation-DailyFraction"
		;;
		(din_areal_loading)                           # 16
			echo "DIN-AreaLoading"
		;;
		(din_point_loading)                           # 17
			echo "DIN-PointLoading"
		;;
		(discharge)                                   # 18
			echo "Discharge"
		;;
		(distance_to_oceans)                          # 19
			echo "DistToOceans"
		;;
		(doc_areal_loading)                           # 20
			echo "DOC-ArealLoading"
		;;
		(doc_point_loading)                           # 21
			echo "DOC-PointLoading"
		;;
		(don_areal_loading)                           # 22
			echo "DON-ArealLoading"
		;;
		(don_point_loading)                           # 23
			echo "DON-PointLoading"
		;;
		(elevation)                                   # 24
			echo "Elevation"
		;;
		(min_elevation)                               # 25
			echo "Elevation-Min"
		;;
		(max_elevation)                               # 26
			echo "Elevation-Max"
		;;
		(field_capacity)                              # 27
			echo "Soil-FieldCapacity"
		;;
		(growing_season1)                             # 28
			echo "Crops-GrowingSeason1"
		;;
		(growing_season2)                             # 29
			echo "Crops-GrowingSeason2"
		;;
		(heatindex)                                   # 30
			echo "Heatindex"
		;;
		(hydro_unit)                                  # 31
			echo "Hydro-Unit"
		;;
		(max_heatindex)                               # 32
			echo "Heatindex-Max"
		;;
		(min_heatindex)                               # 33
			echo "Heatindex-Min"
		;;
		(dewpoint_temperature)                        # 34
			echo "Humidity-Dewpoint"
		;;
		(relative_humidity)                           # 35
			echo "Humidity-Relative"
		;;
		(specific_humidity)                           # 36
			echo "Humidity-Specific"
		;;
		(vapor_pressure)                              # 37
			echo "Humidity-VaporPressure"
		;;
		(irrigated_area_fraction)                     # 38
			echo "Irrigation-AreaFraction"
		;;
		(irrigation_efficiency)                       # 39
			echo "Irrigation-Efficiency"
		;;
		(irrigation_gross_demand)                     # 40
			echo "IrrGrossDemand"
		;;
		(landcover_barren_percentage)                 # 41
			echo "Landcover-PctBarren"
		;;
		(landcover_deciduous_broadleaf_percentage)    # 42
			echo "Landcover-PctBroadleafDeciduous"
		;;
		(landcover_evergreen_broadleaf_percentage)    # 43
			echo "Landcover-PctBroadleafEvergreen"
		;;
		(landcover_deciduous_conifer_percentage)      # 44
			echo "Landcover-PctConiferDeciduous"
		;;
		(landcover_evergreen_conifer_percentage)      # 45
			echo "Landcover-PctConiferEvergreen"
		;;
		(landcover_cropland_percentage)               # 46
			echo "Landcover-PctCropland"
		;;
		(landcover_crop-natural_mosaic_percentage)    # 47
			echo "Landcover-PctCropNaturalMosaic"
		;;
		(landcover_grassland_percentage)              # 48
			echo "Landcover-PctGrassland"
		;;
		(landcover_mixed_forest_percentage)           # 49
			echo "Landcover-PctMixedForest"
		;;
		(landcover_mixed_savannah_percentage)         # 50
			echo "Landcover-PctSavannah"
		;;
		(landcover_mixed_woody_savannah_percentage)   # 51
			echo "Landcover-PctSavannahWoody"
		;;
		(landcover_mixed_closed_shrubland_percentage) # 52
			echo "Landcover-PctShrublandClosed"
		;;
		(landcover_mixed_open_shrubland_percentage)   # 53
			echo "Landcover-PctShrublandOpen"
		;;
		(landcover_mixed_snow_ice_percentage)         # 54
			echo "Landcover-PctSnowIce"
		;;
		(landcover_urban_built-up_percentage)         # 55
			echo "Landcover-PctUrbanBuiltUp"
		;;
		(landcover_vegetation_percentage)             # 56
			echo "Landcover-PctVegetation"
		;;
		(landcover_water_percentage)                  # 57
			echo "Landcover-PctWater"
		;;
		(landcover_wetland_percentage)                # 58
			echo "Landcover-PctWetland"
		;;
		(network)                                     # 59
			echo "Network"
		;;
		(n_litterfall)                                # 60
			echo "N-LitterFall"
		;;
		(nh4_areal_loading)                           # 61
			echo "NH4-ArealLoading"
		;;
		(nh4_point_loading)                           # 62
			echo "Nh4-PointLoading"
		;;
		(no3_areal_loading)                           # 63
			echo "NO3-ArealLoading"
		;;
		(no3_point_loading)                           # 64
			echo "NO3-PointLoading"
		;;
		(ocean_basins)                                # 65
			echo "OceanBasins"
		;;
		(other_crop_fraction)                         # 66
			echo "CropFraction-Other"
		;;
		(perennial_crop_fraction)                     # 67
			echo "CropFraction-Perennial"
		;;
		(population)                                  # 68
			echo "Population"
		;;
		(population_density)	                      # 69
			echo "Population-Density"
		;;
		(population-rural)                            # 70
			echo "Population-Rural"
		;;
		(population-urban)                            # 71
			echo "Population-Urban"
		;;
		(precipitation)                               # 72
			echo "Precipitation"
		;;
		(shortwave_downwelling_radiation)             # 73
			echo "Radiation-ShortWave-Downwelling"
		;;
		(shortwave_upwelling_radiation)               # 74
			echo "Radiation-ShortWave-Upwelling"
		;;
		(shortwave_clear_sky_radiation)               # 75
			echo "Radiation-ShortWave-ClearSky"
		;;
		(longwave_downwelling_radiation)              # 76
			echo "Radiation-LongWave-Downwelling"
		;;
		(longwave_upwelling_radiation)                # 77
			echo "Radiation-LongWave-Upwelling"
		;;
		(longwave_clear_sky_radiation)                # 78
			echo "Radiation-LongWave-ClearSky"
		;;
		(rain_fall)                                   # 79
			echo "RainFall"
		;;
		(rain_pet)                                    # 80
			echo "RainPET"
		;;
		(reservoir_capacity)                          # 81
			echo "Reservoir-Capacity"
		;;
		(rice_crop_fraction)                          # 82
			echo "CropFraction-Rice"
		;;
		(rice_percolation_rate)                       # 83
			echo "Crops-RicePercolationRate"
		;;
		(rooting_depth)                               # 84
			echo "Soil-RootingDepth"
		;;
		(runoff)                                      # 85
			echo "Runoff"
		;;
		(small_reservoir_coefficient)                 # 86
			echo "SmallReservoir-Coefficient"
		;;
		(snow_fall)                                   # 87
			echo "SnowFall"
		;;
		(soil_moisture)                               # 88
			echo "SoilMoisture"
		;;
		(soil_moisture_change)                        # 89
			echo "SoilMoistureChange"
		;;
		(streamline)                                  # 90
			echo "Streamline"
		;;
		(subbasin)                                    # 91
			echo "Subbasin"
		;;
		(surface_air_pressure)                        # 92
			echo "AirPressure"
		;;
		(upstream_area)                               # 93
			echo "UpstreamArea"
		;;
		(vegetables_crop_fraction)                    # 94
			echo "CropFraction-Vegetables"
		;;
		(wilting_point)                               # 95
			echo "Soil-WiltingPoint"
		;;
		(wind_speed)                                  # 96
			echo "WindSpeed"
		;;
		(u_wind_speed)                                # 97
			echo "WindSpeed-U10m"
		;;
		(v_wind_speed)                                # 98
			echo "WindSpeed-V10m"
		;;
		(*)
			echo "${variable}"
		;;
	esac
}

function _RGISlookupFullName ()
{
    local variable="${1}"

	case "$(echo "${variable}" | tr "[A-Z]" "[a-z]")" in
		(air_temperature)                             #  0
			echo "Air Temperature"
		;;
		(max_air_temperature)                         #  1
			echo "Maximum Air Temperature"
		;;
		(min_air_temperature)                         #  2
			echo "Minimum Air Temperature"
		;;
		(diurnal_air_temperature_range)               #  3
			echo "Diurnal AirTemperature Range"
		;;
		(c_litterfall)                                #  4
			echo "C LitterFall"
		;;
		(cell_area)                                   #  5
			echo "Cell Area"
		;;
		(cell_slope)                                  #  6
			echo "Cell Slope"
		;;
		(cloud_cover)                                 #  7
			echo "Cloud Cover"
		;;
		(confluence)                                  #  8
			echo "Confluence"
		;;
		(continents)                                  #  9
			echo "Continents"
		;;
		(countries)                                   # 10
			echo "Countries"
		;;
		(states)                                      # 11
			echo "States"
		;;
		(counties)                                    # 12
			echo "Counties"
		;;
		(cropland_fraction)                           # 13
			echo "Cropland Fraction"
		;;
		(cropping_intensity)                          # 14
			echo "Cropping Intensity"
		;;
		(daily_precipitation_fraction)                # 15
			echo "Daily Precipitation Fraction"
		;;
		(din_areal_loading)                           # 16
			echo "DIN Areal Loading"
		;;
		(din_point_loading)                           # 17
			echo "DIN Point Loading"
		;;
		(discharge)                                   # 18
			echo "Discharge"
		;;
		(distance_to_oceans)                          # 19
			echo "Distance to Oceans"
		;;
		(doc_areal_loading)                           # 20
			echo "DOC Areal Loading"
		;;
		(doc_point_loading)                           # 21
			echo "DOC Point Loading"
		;;
		(don_areal_loading)                           # 22
			echo "DON Areal Loading"
		;;
		(don_point_loading)                           # 23
			echo "DON Point Loading"
		;;
		(elevation)                                   # 24
			echo "Elevation"
		;;
		(max_elevation)                               # 25
			echo "Maximum Elevation"
		;;
		(min_elevation)                               # 26
			echo "Minimum Elevation"
		;;
		(field_capacity)                              # 27
			echo "Field Capacity"
		;;
		(growing_season1)                             # 28
			echo "Growing Season 1"
		;;
		(growing_season2)                             # 29
			echo "Growing Season 2"
		;;
		(heatindex)                                   # 30
			echo "Heatindex"
		;;
		(hydro_unit)                                  # 31
			echo "Hydrological Unit"
		;;
		(max_heatindex)                               # 32
			echo "Maximum Heatindex"
		;;
		(min_heatindex)                               # 33
			echo "Minimum Heatindex"
		;;
		(dewpoint_temperature)                        # 34
			echo "Dewpoint Temperature"
		;;
		(relative_humidity)                           # 35
			echo "Relative Humidity"
		;;
		(specific_humidity)                           # 36
			echo "Specific Humidity"
		;;
		(vapor_pressure)                              # 37
			echo "Vapor Pressure"
		;;
		(irrigated_area_fraction)                     # 38
			echo "Irrigated Area Fraction"
		;;
		(irrigation_efficiency)                       # 39
			echo "Irrigation Efficiency"
		;;
		(irrigation_gross_demand)                     # 40
			echo "Irrigation Gross Demand"
		;;
		(landcover_barren_percentage)                 # 41
			echo "Barren Percentage"
		;;
		(landcover_deciduous_broadleaf_percentage)    # 42
			echo "Deciduous Broadleaf Percentage"
		;;
		(landcover_evergreen_broadleaf_percentage)    # 43
			echo "Evergreen Broadleaf Percentage"
		;;
		(landcover_deciduous_conifer_percentage)      # 44
			echo "Deciduous Conifer Percentage"
		;;
		(landcover_evergreen_conifer_percentage)      # 45
			echo "Evergreen Conifer Percentage"
		;;
		(landcover_cropland_percentage)               # 46
			echo "Cropland Percentage"
		;;
		(landcover_crop-natural_mosaic_percentage)    # 47
			echo "Cropland-Natural Mosaic Percentage"
		;;
		(landcover_grassland_percentage)              # 48
			echo "Grassland Percentage"
		;;
		(landcover_mixed_forest_percentage)           # 49
			echo "Mixed Forest Percentage"
		;;
		(landcover_mixed_savannah_percentage)         # 50
			echo "Savannah Percentage"
		;;
		(landcover_mixed_woody_savannah_percentage)   # 51
			echo "Woody Savannah Percentage"
		;;
		(landcover_mixed_closed_shrubland_percentage) # 52
			echo "Closed Shrubland Percentage"
		;;
		(landcover_mixed_open_shrubland_percentage)   # 53
			echo "Open Shrubland Percentage"
		;;
		(landcover_mixed_snow_ice_percentage)         # 54
			echo "Snow-Ice Percentage"
		;;
		(landcover_urban_built-up_percentage)         # 55
			echo "Urban Built-Up Percentage"
		;;
		(landcover_vegetation_percentage)             # 56
			echo "Vegetation Fraction"
		;;
		(landcover_water_percentage)                  # 57
			echo "Water Percentage"
		;;
		(landcover_wetland_percentage)                # 58
			echo "Wetland Percentage"
		;;
		(network)                                     # 59
			echo "STNetwork"
		;;
		(n_litterfall)                                # 60
			echo "N LitterFall"
		;;
		(nh4_areal_loading)                           # 61
			echo "NH4 Areal Loading"
		;;
		(nh4_point_loading)                           # 62
			echo "NH4 Point Loading"
		;;
		(no3_areal_loading)                           # 63
			echo "NO3 Areal Loading"
		;;
		(no3_point_loading)                           # 64
			echo "NO3 Point Loading"
		;;
		(ocean_basins)                                # 65
			echo "Ocean Basins"
		;;
		(other_crop_fraction)                         # 66
			echo "Other Crop Fraction"
		;;
		(perennial_crop_fraction)                     # 67
			echo "Perennial Crop Fraction"
		;;
		(population)                                  # 68
			echo "Population"
		;;
		(population_density)                          # 69
			echo "Population Density"
		;;
		(population-rural)                            # 70
			echo "Rural Population"
		;;
		(population-urban)                            # 71
			echo "Urban Population"
		;;
		(precipitation)                               # 72
			echo "Precipitation"
		;;
		(shortwave_downwelling_radiation)             # 73
 			echo "Downwelling Short Wave Radiation"
		;;
		(shortwave_upwelling_radiation)               # 74
			echo "Upwelling Short Wave Radiation"
		;;
		(shortwave_clear_sky_radiation)               # 75
			echo "Clear Sky Short Wave Radiation"
		;;
		(longwave_downwelling_radiation)              # 76
			echo "Downwelling Long Wave Radiation"
		;;
		(longwave_upwelling_radiation)                # 77
			echo "Upwelling Long Wave Radiation"
		;;
		(longwave_clear_sky_radiation)                # 78
			echo "Clear Sky Long Wave Radiation"
		;;
		(rain_fall)                                   # 79
			echo "Rain Fall"
		;;
		(rain_pet)                                    # 80
			echo "Rain Potential ET"
		;;
		(reservoir_capacity)                          # 81
			echo "Reservoir Capacity"
		;;
		(rice_crop_fraction)                          # 82
			echo "Rice Crop Fraction"
		;;
		(rice_percolation_rate)                       # 83
			echo "Rice Percolation Rate"
		;;
		(rooting_depth)                               # 84
			echo "Rooting Depth"
		;;
		(runoff)                                      # 85
			echo "Runoff"
		;;
		(small_reservoir_coefficient)                 # 86
			echo "Small-reservoir Coefficient"
		;;
		(snow_fall)                                   # 87
			echo "Snow Fall"
		;;
		(soil_moisture)                               # 88
			echo "Soil Moisture"
		;;
		(soil_moisture_change)                        # 89
			echo "Soil Moisture Change"
		;;
		(streamline)                                  # 90
			echo "Streamline"
		;;
		(subbasin)                                    # 91
			echo "Subbasin"
 		;;
		(surface_air_pressure)                        # 92
			echo "Air Pressure"
		;;
		(upstream_area)                               # 93
			echo "Upstream Area"
		;;
		(vegetables_crop_fraction)                    # 94
			echo "Vegetables Crop Fraction"
		;;
		(wilting_point)                               # 95
			echo "Wilting Point"
		;;
		(wind_speed)                                  # 96
			echo "Wind Speed"
		;;
		(u_wind_speed)                                # 97
			echo "Wind Speed U-direction"
		;;
		(v_wind_speed)                                # 98
			echo "Wind Speed V-direction"
		;;
		(*)
			echo "${variable}"
		;;
	esac
}

function RGISlookupShadeset ()
{
    local variable="${1}"

	case "$(echo "${variable}" | tr "[A-Z]" "[a-z]")" in
		(air_temperature|max_air_temperature|min_air_temperature) # 0 1 2
			echo "blue-to-red"
		;;
		(diurnal_air_temperature_range)               #  3
			echo "grey"
		;;
		(c_litterfall)                                #  4
			echo "grey"
		;;
		(cell_area)                                   #  5
			echo "grey"
		;;
		(cell_slope)                                  #  6
			echo "grey"
		;;
		(cloud_cover)                                 #  7
			echo "grey"
		;;
		(confluence)                                  #  8
			echo "grey"
		;;
		(continents)                                  #  9
			echo "grey"
		;;
		(countries)                                   # 10
			echo "grey"
		;;
		(states)                                      # 11
			echo "grey"
		;;
		(counties)                                    # 12
			echo "grey"
		;;
		(cropland_fraction)                           # 13
			echo "grey"
		;;
		(cropping_intensity)                          # 14
			echo "grey"
		;;
		(daily_precipitation_fraction)                # 15
			echo "grey"
		;;
		(din_areal_loading)                           # 16
			echo "grey"
		;;
		(din_point_loading)                           # 17
			echo "grey"
		;;
		(discharge)                                   # 18
			echo "blue"
		;;
		(distance_to_oceans)                          # 19
			echo "grey"
		;;
		(don_areal_loading)                           # 20
			echo "grey"
		;;
		(don_point_loading)                           # 21
			echo "grey"
		;;
		(doc_areal_loading)                           # 22
			echo "grey"
		;;
		(doc_point_loading)                           # 23
			echo "grey"
		;;
		(elevation|max_elevation|min_elevation)       # 24 25 26
			echo "elevation"
		;;
		(field_capacity)                              # 27
			echo "grey"
		;;
		(growing_season1)                             # 28
			echo "grey"
		;;
		(growing_season2)                             # 29
			echo "grey"
		;;
		(heatindex)                                   # 30
			echo "blue-to-red"
		;;
		(hydro_unit)                                  # 31
			echo "grey"
		;;
		(max_heatindex)                               # 32
			echo "blue-to-red"
		;;
		(min_heatindex)                               # 33
			echo "blue-to-red"
		;;
		(dewpoint_temperature)                        # 34
			echo "blue"
		;;
		(relative_humidity)                           # 35
			echo "blue"
		;;
		(specific_humidity)                           # 36
			echo "blue"
		;;
		(vapor_pressure)                              # 37
			echo "blue"
		;;
		(irrigated_area_fraction)                     # 38
			echo "grey"
		;;
		(irrigation_efficiency)                       # 39
			echo "grey"
		;;
		(irrigation_gross_demand)                     # 40
			echo "grey"
		;;
		(landcover_barren_percentage)                 # 41
			echo "grey"
		;;
		(landcover_deciduous_broadleaf_percentage)    # 42
			echo "grey"
		;;
		(landcover_evergreen_broadleaf_percentage)    # 43
			echo "grey"
		;;
		(landcover_deciduous_conifer_percentage)      # 44
			echo "grey"
		;;
		(landcover_evergreen_conifer_percentage)      # 45
			echo "grey"
		;;
		(landcover_cropland_percentage)               # 46
			echo "grey"
		;;
		(landcover_crop-natural_mosaic_percentage)    # 47
			echo "grey"
		;;
		(landcover_grassland_percentage)              # 48
			echo "grey"
		;;
		(landcover_mixed_forest_percentage)           # 49
			echo "grey"
		;;
		(landcover_mixed_savannah_percentage)         # 50
			echo "grey"
		;;
		(landcover_mixed_woody_savannah_percentage)   # 51
			echo "grey"
		;;
		(landcover_mixed_closed_shrubland_percentage) # 52
			echo "grey"
		;;
		(landcover_mixed_open_shrubland_percentage)   # 53
			echo "grey"
		;;
		(landcover_mixed_snow_ice_percentage)         # 54
			echo "grey"
		;;
		(landcover_urban_built-up_percentage)         # 55
			echo "grey"
		;;
		(landcover_vegetation_percentage)             # 56
			echo "grey"
		;;
		(landcover_water_percentage)                  # 57
			echo "grey"
		;;
		(landcover_wetland_percentage)                # 58
			echo "grey"
		;;
		(network)                                     # 59
			echo "grey"
		;;
		(n_litterfall)                                # 60
			echo "grey"
		;;
		(nh4_areal_loading)                           # 61
			echo "grey"
		;;
		(nh4_point_loading)                           # 62
			echo "grey"
		;;
		(no3_areal_loading)                           # 63
			echo "grey"
		;;
		(no3_point_loading)                           # 64
			echo "grey"
		;;
		(ocean_basins)                                # 65
			echo "grey"
		;;
		(other_crop_fraction)                         # 66
			echo "grey"
		;;
		(perennial_crop_fraction)                     # 67
			echo "grey"
		;;
		(population)                                  # 68
			echo "grey"
		;;
		(population_density)                          # 69
			echo "grey"
		;;
		(population-rural)                            # 70
			echo "grey"
		;;
		(population-urban)                            # 71
			echo "grey"
		;;
		(precipitation)                               # 72
			echo "blue"
		;;
		(shortwave_downwelling_radiation)             # 73
			echo "grey"
		;;
		(shortwave_upwelling_radiation)               # 74
			echo "grey"
		;;
		(shortwave_clear_sky_radiation)               # 75
			echo "grey"
		;;
		(longwave_downwelling_radiation)              # 76
			echo "grey"
		;;
		(longwave_upwelling_radiation)                # 77
			echo "grey"
		;;
		(longwave_clear_sky_radiation)                # 78
			echo "grey"
		;;
		(rain_fall)                                   # 79
			echo "blue"
		;;
		(rain_pet)                                    # 80
			echo "grey"
		;;
		(reservoir_capacity)                          # 81
			echo "grey"
		;;
		(rice_crop_fraction)                          # 82
			echo "grey"
		;;
		(rice_percolation_rate)                       # 83
			echo "grey"
		;;
		(rooting_depth)                               # 84
			echo "grey"
		;;
		(runoff)                                      # 85
			echo "blue"
		;;
		(small_reservoir_coefficient)                 # 86
			echo "grey"
		;;
		(snow_fall)                                   # 87
			echo "grey"
		;;
		(soil_moisture)                               # 88
			echo "grey"
		;;
		(soil_moisture_change)                        # 89
			echo "blue-to-red"
		;;
		(surface_air_pressure)                        # 90
			echo "grey"
		;;
		(streamline)                                  # 91
			echo "grey"
		;;
		(subbasin)                                    # 92
			echo "grey"
		;;
		(upstream_area)                               # 93
			echo "grey"
		;;
		(vegetables_crop_fraction)                    # 94
			echo "grey"
		;;
		(wilting_point)                               # 95
			echo "grey"
		;;
		(wind_speed)                                  # 96
			echo "grey"
		;;
		(u_wind_speed)                                # 97
			echo "grey"
		;;
		(v_wind_speed)                                # 98
			echo "grey"
		;;
		(*)
			echo "grey"
		;;
	esac
}

function RGISlookupAggrMethod ()
{
    local variable="${1}"

	case "$(echo "${variable}" | tr "[A-Z]" "[a-z]")" in
		(air_temperature|max_air_temperature|min_air_temperature)  # 0 1 2
			echo "avg"
		;;
		(diurnal_air_temperature_range)               #  3
			echo "avg"
		;;
		(c_litterfall)                                #  4
			echo "sum"
		;;
		(cell_area)                                   #  5
			echo "avg"
		;;
		(cell_slope)                                  #  6
			echo "grey"
		;;
		(cloud_cover)                                 #  7
			echo "avg"
		;;
		(confluence)                                  #  8
			echo "N/A"
		;;
		(continents)                                  #  9
			echo "N/A"
		;;
		(countries)                                   # 10
			echo "N/A"
		;;
		(states)                                      # 11
			echo "N/A"
		;;
		(counties)                                    # 12
			echo "N/A"
		;;
		(cropland_fraction)                           # 13
			echo "avg"
		;;
		(cropping_intensity)                          # 14
			echo "avg"
		;;
		(daily_precipitation_fraction)                # 15
			echo "avg"
		;;
		(din_areal_loading)                           # 16
			echo "sum"
		;;
		(din_point_loading)                           # 17
			echo "sum"
		;;
		(discharge)                                   # 18
			echo "avg"
		;;
		(distance_to_oceans)                          # 19
			echo "max"
		;;
		(don_areal_loading)                           # 20
			echo "sum"
		;;
		(don_point_loading)                           # 21
			echo "sum"
		;;
		(doc_areal_loading)                           # 22
			echo "sum"
		;;
		(doc_point_loading)                           # 23
			echo "sum"
		;;
		(elevation|max_elevation|min_elevation)       # 24 25 26
			echo "avg"
		;;
		(field_capacity)                              # 27
			echo "avg"
		;;
		(growing_season1)                             # 28
			echo "avg"
		;;
		(growing_season2)                             # 29
			echo "avg"
		;;
		(heatindex)                                   # 30
			echo "avg"
		;;
		(hydro_unit)                                  # 31
			echo "grey"
		;;
		(max_heatindex)                               # 32
			echo "avg"
		;;
		(min_heatindex)                               # 33
			echo "avg"
		;;
		(dewpoint_temperature)                        # 34
			echo "avg"
		;;
		(relative_humidity)                           # 35
			echo "avg"
		;;
		(specific_humidity)                           # 36
			echo "avg"
		;;
		(vapor_pressure)                              # 37
			echo "avg"
		;;
		(irrigated_area_fraction)                     # 38
			echo "avg"
		;;
		(irrigation_efficiency)                       # 39
			echo "avg"
		;;
		(irrigation_gross_demand)                     # 40
			echo "sum"
		;;
		(landcover_barren_percentage)                 # 41
			echo "avg"
		;;
		(landcover_deciduous_broadleaf_percentage)    # 42
			echo "avg"
		;;
		(landcover_evergreen_broadleaf_percentage)    # 43
			echo "avg"
		;;
		(landcover_deciduous_conifer_percentage)      # 44
			echo "avg"
		;;
		(landcover_evergreen_conifer_percentage)      # 45
			echo "avg"
		;;
		(landcover_cropland_percentage)               # 46
			echo "avg"
		;;
		(landcover_crop-natural_mosaic_percentage)    # 47
			echo "avg"
		;;
		(landcover_grassland_percentage)              # 48
			echo "avg"
		;;
		(landcover_mixed_forest_percentage)           # 49
			echo "avg"
		;;
		(landcover_mixed_savannah_percentage)         # 50
			echo "avg"
		;;
		(landcover_mixed_woody_savannah_percentage)   # 51
			echo "avg"
		;;
		(landcover_mixed_closed_shrubland_percentage) # 52
			echo "avg"
		;;
		(landcover_mixed_open_shrubland_percentage)   # 53
			echo "avg"
		;;
		(landcover_mixed_snow_ice_percentage)         # 54
			echo "avg"
		;;
		(landcover_urban_built-up_percentage)         # 55
			echo "avg"
		;;
		(landcover_vegetation_percentage)             # 56
			echo "avg"
		;;
		(landcover_water_percentage)                  # 57
			echo "avg"
		;;
		(landcover_wetland_percentage)                # 58
			echo "avg"
		;;
		(network)                                     # 59
			echo "avg"
		;;
		(n_litterfall)                                # 60
			echo "sum"
		;;
		(nh4_areal_loading)                           # 61
			echo "sum"
		;;
		(nh4_point_loading)                           # 62
			echo "sum"
		;;
		(no3_areal_loading)                           # 63
			echo "sum"
		;;
		(no3_point_loading)                           # 64
			echo "sum"
		;;
		(ocean_basins)                                # 65
			echo "N/A"
		;;
		(other_crop_fraction)                         # 66
			echo "avg"
		;;
		(perennial_crop_fraction)                     # 67
			echo "avg"
		;;
		(population)                                  # 68
			echo "sum"
		;;
		(population_density)                          # 69
			echo "avg"
		;;
		(population-rural)                            # 70
			echo "avg"
		;;
		(population-urban)                            # 71
			echo "avg"
		;;
		(precipitation)                               # 72
			echo "sum"
		;;
		(shortwave_downwelling_radiation)             # 73
			echo "avg"
		;;
		(shortwave_upwelling_radiation)               # 74
			echo "avg"
		;;
		(shortwave_clear_sky_radiation)               # 75
			echo "avg"
		;;
		(longwave_downwelling_radiation)              # 76
			echo "avg"
		;;
		(longwave_upwelling_radiation)                # 77
			echo "avg"
		;;
		(longwave_clear_sky_radiation)                # 78
			echo "avg"
		;;
		(rain_fall)                                   # 79
			echo "sum"
		;;
		(rain_pet)                                    # 80
			echo "sum"
		;;
		(reservoir_capacity)                          # 81
			echo "avg"
		;;
		(rice_crop_fraction)                          # 82
			echo "avg"
		;;
		(rice_percolation_rate)                       # 83
			echo "avg"
		;;
		(rooting_depth)                               # 84
			echo "avg"
		;;
		(runoff)                                      # 85
			echo "sum"
		;;
		(small_reservoir_coefficient)                 # 86
			echo "avg"
		;;
		(snow_fall)                                   # 87
			echo "avg"
		;;
		(soil_moisture)                               # 88
			echo "avg"
		;;
		(soil_moisture_change)                        # 89
			echo "avg"
		;;
		(surface_air_pressure)                        # 90
			echo "avg"
		;;
		(streamline)                                  # 91
			echo "avg"
		;;
		(subbasin)                                    # 92
			echo "avg"
		;;
		(upstream_area)                               # 93
			echo "avg"
		;;
		(vegetables_crop_fraction)                    # 94
			echo "avg"
		;;
		(wilting_point)                               # 95
			echo "avg"
		;;
		(wind_speed)                                  # 96
			echo "avg"
		;;
		(u_wind_speed)                                # 97
			echo "avg"
		;;
		(v_wind_speed)                                # 98
			echo "avg"
		;;
		(*)
			echo "avg"
			echo "Defaulting aggregation method: ${variable}" > /dev/stderr
		;;
	esac
}

function RGISlookupTimeStep ()
{
	local timeStep=$(echo "${1}" | tr "[A-Z]" "[a-z]")

	case "${timeStep}" in
		(daily)
			echo "day"
		;;
		(monthly)
			echo "month"
		;;
		(annual)
			echo "year"
		;;
	esac
}

function RGIStimeNumberOfDays ()
{
	local    year="${1}"
	local   month=$((${2} - 1))
	local century="${year%??}"

	local monthLength[0]=31
	local monthLength[1]=28
	local monthLength[2]=31
	local monthLength[3]=30
	local monthLength[4]=31
	local monthLength[5]=30
	local monthLength[6]=31
	local monthLength[7]=31
	local monthLength[8]=30
	local monthLength[9]=31
	local monthLength[10]=30
	local monthLength[11]=31

	if (( month == 1))
	then
		if [[ "${year}" == "${century}00" ]]
		then
			local leapYear=$(( (${century} & 0xffffe) == ${century} ? 1 : 0))
		else
			local leapYear=$(( (${year}    & 0xffffe) == ${year}    ? 1 : 0))
		fi
	else
		local leapYear=0
	fi
	echo $((${monthLength[${month}]} + ${leapYear}))
}

function _RGISvariableDir ()
{
	local      archive="${1}"
	local       domain="${2}"
	local     variable="${3}"

	local       varDir=$(RGISlookupSubject "${variable}")

	if [ -e "${archive}/${domain%+}/${varDir}" ]
	then
		echo "${domain%+}/${varDir}"
		return 0
	else
		if [ "${domain%+}" == "${domain}" ] # Exact domain
		then
			echo "Missing ${archive}/${domain}/${varDir}" > /dev/stderr
			echo ""
			return 1
		else # Dynamic domain
			local parent="${archive}/${domain%+}/parent"
			if [ -e "${parent}" ]
			then
				_RGISvariableDir "${archive}" "$(cat ${archive}/${domain%+}/parent)+" "${variable}" || return 1
			else
				echo "Missing ${parent}" > /dev/stderr
				echo ""
				return 1
			fi
		fi
	fi
	return 0
}

function RGISgeoResolutionInSecond ()
{
    local resolution="${1}"

   	case "${resolution}" in
	("15sec")
		echo "15"
	;;
	("30sec")
		echo "30"
	;;
	("45sec")
		echo "45"
	;;
	("01min")
		echo "60"
	;;
	("1m30s")
		echo "90"
	;;
	("2m30s")
		echo "150"
	;;
	("03min")
		echo "180"
	;;
	("3m45s")
		echo "225"
	;;
	("05min")
		echo "300"
	;;
	("06min")
		echo "360"
	;;
	("10min")
		echo "600"
	;;
	("15min")
		echo "900"
	;;
	("30min")
		echo "1800"
	;;
	("60min")
		echo "3600"
	;;
	("02deg")
		echo "7200"
	;;
	(*)
		echo "Invalid resolution" > /dev/stderr
		return 1
	;;
	esac
    return 0
}

RGISgeoResolutionMultiplier () # Destination resolution is devided by source resolution. Decimals ar chapped off.
{
    local srcRes="$(RGISgeoResolutionInSecond "${1}")"
    local dstRes="$(RGISgeoResolutionInSecond "${2}")"

    if [[ "${srcRes}" == "" || "${dstRes}" == "" ]]
    then
        echo "Invalid resolutions: ${srcRes}, ${dstRes}" > /dev/stderr
        echo "Usage: Destination resolution is devided by source resolution. Decimals ar chapped off." > /dev/stderr
        return 1
    else
        echo "${dstRes} / ${srcRes}" | bc -l | sed "s:\([0-9]*\).*:\1:"
    fi
    return 0

}
function _RGISresolutionDir ()
{
	local      archive="${1}"
	local       domain="${2}"
	local     variable="${3}"
	local      product="${4}"
	local   resolution="${5}"

	local varDir=$(_RGISvariableDir "${archive}" "${domain}" "${variable}")
	if [ "${varDir}" == "" ]
	then
		return
	else
		if [ -e "${archive}/${varDir}/${product}/${resolution%+}" ]
		then
			echo "${varDir}/${product}/${resolution%+}"
		else
			if [ "${resolution%+}" == "${resolution}" ] # Exact resolution
			then
				echo "Missing ${archive}/${varDir}/${product}/${resolution}" > /dev/stderr
				echo ""
				return 1
			else # Dynamic resolution
				if [ "${resolution%km+}" == "${resolution}" ] # Geographic
				then
					local geogNum=0
					local geogRes[${geogNum}]="15sec"; (( ++geogNum ))
					local geogRes[${geogNum}]="30sec"; (( ++geogNum ))
					local geogRes[${geogNum}]="45sec"; (( ++geogNum ))
					local geogRes[${geogNum}]="01min"; (( ++geogNum ))
					local geogRes[${geogNum}]="1m30s"; (( ++geogNum ))
					local geogRes[${geogNum}]="2m30s"; (( ++geogNum ))
					local geogRes[${geogNum}]="03min"; (( ++geogNum ))
					local geogRes[${geogNum}]="3m45s"; (( ++geogNum ))
					local geogRes[${geogNum}]="05min"; (( ++geogNum ))
					local geogRes[${geogNum}]="06min"; (( ++geogNum ))
					local geogRes[${geogNum}]="7m30s"; (( ++geogNum ))
					local geogRes[${geogNum}]="10min"; (( ++geogNum ))
					local geogRes[${geogNum}]="15min"; (( ++geogNum ))
					local geogRes[${geogNum}]="30min"; (( ++geogNum ))
					local geogRes[${geogNum}]="60min"; (( ++geogNum ))
					local geogRes[${geogNum}]="1d30m"; (( ++geogNum ))
					local geogRes[${geogNum}]="02deg"; (( ++geogNum ))
					local geogRes[${geogNum}]="2d30m"; (( ++geogNum ))

					for (( num = 0; num < ${geogNum}; ++num ))
					do
						if [ "${geogRes[${num}]}" == "${resolution%+}" ]; then break; fi
					done
					if (( num < geogNum))
					then
						_RGISresolutionDir "${archive}" "${domain}" "${variable}"  "${product}" "${geogRes[((${num} + 1))]}+" || return 1
					else
						echo "No coarser resolution ${domain%+} ${variable} ${product} ${resolution%+}" > /dev/stderr
						return 1
					fi
				else # Cartesian
					local cartNum=0
					local cartRes[${cartNum}]="1km";    (( ++cartNum ))
					local cartRes[${cartNum}]="2km";    (( ++cartNum ))
					local cartRes[${cartNum}]="5km";    (( ++cartNum ))
					local cartRes[${cartNum}]="10km";   (( ++cartNum ))
					local cartRes[${cartNum}]="25km";   (( ++cartNum ))
					local cartRes[${cartNum}]="50km";   (( ++cartNum ))

					for (( num = 0; num < ${cartNum}; ++num ))
					do
						if [ "${cartRes[${num}]}" == "${resolution%+}" ]; then break; fi
					done
					if (( num < cartNum))
					then
						_RGISresolutionDir "${archive}" "${domain}" "${variable}"  "${product}" "${cartRes[((${num} + 1))]}+" || return 1
					else
						echo "No coarser resolution ${domain%+} ${variable} ${product} ${resolution%+}" > /dev/stderr
						return 1
					fi
				fi
			fi
		fi
	fi
	return 0
}
function _RGIStStepDir ()
{
	local tStepType="${1}"
	local     tStep="${2}"

	case "${tStepType}" in
		(TS)
			case "${tStep}" in
				(hourly)
					echo "Hourly"
				;;
				(3hourly)
					echo "3Hourly"
				;;
				(6hourly)
					echo "6Hourly"
				;;
				(daily)
					echo "Daily"
				;;
				(monthly)
					echo "Monthly"
				;;
				(annual)
					echo "Annual"
				;;
				(*)
					echo "${tStep}"
				;;
			esac
		;;
		(LT|LTmin|LTmax|LTslope|LTrange|LTstdDev|Stats|static)
			echo "Static"
		;;
	esac
	return 0
}

function RGISdirectoryPath ()
{
	local    archive="${1}"
	local     domain="${2}"
	local   variable="${3}"
	local    product="${4}"
	local resolution="${5}"
	local  tStepType="${6}"
	local      tStep=$(echo "${7}" | tr "[A-Z]" "[a-z]")

	local        dir=$(_RGIStStepDir ${tStepType} ${tStep})
	local     varDir=$(RGISlookupSubject "${variable}")

	echo "${archive}/${domain}/${varDir}/${product}/${resolution}/${dir}"
}

function RGISdirectory ()
{
	local    archive="${1}"
	local     domain="${2}"
	local   variable="${3}"
	local    product="${4}"
	local resolution="${5}"
	local  tStepType="${6}"
	local      tStep=$(echo "${7}" | tr "[A-Z]" "[a-z]")

	local dir=$(_RGIStStepDir ${tStepType} ${tStep})

	local resDir=$(_RGISresolutionDir "${archive}" "${domain}" "${variable}" "${product}" "${resolution}")
	if [ "${resDir}" == "" ]
	then
		return 1
	else
		echo "${archive}/${resDir}/${dir}"
	fi
	return 0
}

function RGISfileExtension ()
{
	local variable="${1}"
	case "$(echo "${variable}" | tr "[A-Z]" "[a-z]")" in
		(network)
			local extension="gdbn"
		;;
		(confluence|crossing-*)
			local extension="gdbp"
		;;
		(continents|countries|states|counties|ocean_basins|subbasin|hydro_unit)
			local extension="gdbd"
		;;
		(streamline)
			local extension="gdbl"
		;;
		(*)
			local extension="gdbc"
		;;
    esac
    echo "${extension}"
}

function RGISfilePath ()
{
	local      archive="${1}"
	local       domain="${2}"
	local     variable="${3}"
	local      product="${4}"
	local   resolution="${5}"
	local    tStepType="${6}"
	local        tStep=$(echo "${7}" | tr "[A-Z]" "[a-z]")
	local    timeRange="${8}"

	case "${tStep}" in
		(hourly)
			local tStepStr="h"
		;;
		(3hourly)
			local tStepStr="3h"
		;;
		(6hourly)
			local tStepStr="6h"
		;;
		(daily)
			local tStepStr="d"
		;;
		(monthly)
			local tStepStr="m"
		;;
		(annual)
			local tStepStr="a"
		;;
		("")
			local tStepStr=""
		;;
		(*)
			echo "Unknown time step ${tStep}" > /dev/stderr
			return 1
		;;
	esac

    local extension=$(RGISfileExtension "${variable}")

	local rgisDirectory=$(RGISdirectoryPath  "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "${tStepType}" "${tStep}")
	local      fileName=$(RGISdirectoryPath            "" "${domain}" "${variable}" "${product}" "${resolution}" "${tStepType}" "${tStep}")
	local      fileName=$(echo ${fileName} | sed "s:/::" | sed "s:/:_:g" )
	local      fileName=${fileName%_*}
	if [ "${rgisDirectory}" == "" ]
	then
		echo ""
		return 1
	fi
	if [[ "${tStepType}" == "static" ]]
	then
		local tStepType="Static"
		local tStepStr=""
	fi

    case "${__RGISarchiveFormat}" in
        (gzipped)
            echo "${rgisDirectory}/${fileName}_${tStepStr}${tStepType}${timeRange}.${extension}.gz"
        ;;
        (netcdf)
            echo "${rgisDirectory}/${fileName}_${tStepStr}${tStepType}${timeRange}.nc"
        ;;
        (*)
        	echo "${rgisDirectory}/${fileName}_${tStepStr}${tStepType}${timeRange}.${extension}"
        ;;
    esac
}

function RGISfile ()
{
	local      archive="${1}"
	local       domain="${2}"
	local     variable="${3}"
	local      product="${4}"
	local   resolution="${5}"
	local    tStepType="${6}"
	local        tStep=$(echo "${7}" | tr "[A-Z]" "[a-z]")
	local    timeRange="${8}"

	case "${tStep}" in
		(hourly)
			local tStepStr="h"
		;;
		(3hourly)
			local tStepStr="3h"
		;;
		(6hourly)
			local tStepStr="6h"
		;;
		(daily)
			local tStepStr="d"
		;;
		(monthly)
			local tStepStr="m"
		;;
		(annual)
			local tStepStr="a"
		;;
		("")
			local tStepStr=""
		;;
		(*)
			echo "Unknown time step ${tStep}" > /dev/stderr
			return 1
		;;
	esac

    local extension=$(RGISfileExtension "${variable}")

	local rgisDirectory=$(RGISdirectory      "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "${tStepType}" "${tStep}")
	local      fileName=$(_RGISresolutionDir "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" | sed "s:/:_:g" )
	if [ "${rgisDirectory}" == "" ]
	then
		echo ""
		return 1
	fi
	if [[ "${tStepType}" == "static" ]]
	then
		local tStepType="Static"
		local tStepStr=""
	fi

	if [[ "${timeRange}" == "xxxx" ]]
	then
		 ncPattern="${rgisDirectory}/${fileName}_${tStepStr}${tStepType}????.nc"
		gdbPattern="${rgisDirectory}/${fileName}_${tStepStr}${tStepType}????.${extension}"
		 gzPattern="${rgisDirectory}/${fileName}_${tStepStr}${tStepType}????.${extension}.gz"

		[ "$(ls -1  ${ncPattern} 2> /dev/null)" != "" ] && { echo  "${rgisDirectory}/${fileName}_${tStepStr}${tStepType}xxxx.nc";              return 0; }
		[ "$(ls -1 ${gdbPattern} 2> /dev/null)" != "" ] && { echo  "${rgisDirectory}/${fileName}_${tStepStr}${tStepType}xxxx.${extension}";    return 0; }
		[ "$(ls -1  ${gzPattern} 2> /dev/null)" != "" ] && { echo  "${rgisDirectory}/${fileName}_${tStepStr}${tStepType}xxxx.${extension}.gz"; return 0; }
	else
   	[ -e "${rgisDirectory}/${fileName}_${tStepStr}${tStepType}${timeRange}.nc" ]              && { echo    "${rgisDirectory}/${fileName}_${tStepStr}${tStepType}${timeRange}.nc";              return 0;}
   	[ -e "${rgisDirectory}/${fileName}_${tStepStr}${tStepType}${timeRange}.${extension}.gz" ] && { echo    "${rgisDirectory}/${fileName}_${tStepStr}${tStepType}${timeRange}.${extension}.gz"; return 0;}
   	[ -e "${rgisDirectory}/${fileName}_${tStepStr}${tStepType}${timeRange}.${extension}" ]    && { echo    "${rgisDirectory}/${fileName}_${tStepStr}${tStepType}${timeRange}.${extension}";    return 0;}
   fi
   echo ""
	return 1
}

function RGIStitle ()
{
	local     domain="${1}"
	local   variable="${2}"
	local    product="${3}"
	local resolution="${4}"
	local  tStepType="${5}"
	local      tStep=$(echo "${6}" | tr "[A-Z]" "[a-z]")
	local  timeRange="${7}"
	local    version="${8}"


	local variableFullName=$(_RGISlookupFullName "${variable}")

	if [[ "${tStepType}" == "static" ]]
	then
		local tStepString=""
	else
		case "${tStep}" in
			(hourly)
				local tStepStr="Hourly"
			;;
			(3hourly)
				local tStepStr="3Hourly"
			;;
			(6hourly)
				local tStepStr="6Hourly"
			;;
			(daily)
				local tStepStr="Daily"
			;;
			(monthly)
				local tStepStr="Monthly"
			;;
			(annual)
				local tStepStr="Annual"
			;;
			(*)
				echo "Unknown time step ${tStep}"  > /dev/stderr
				return 1
			;;
		esac
		if [[ "${timeRange}" == "" ]]
		then
			local tStepString=", ${tStepStr}${tStepType}"
		else
			local tStepString=", ${tStepStr}${tStepType}, ${timeRange}"
		fi
	fi
	echo "${domain}, ${product} ${variableFullName} (${resolution}${tStepString}) V${version}"
	return 0
}

function RGISAppend ()
{
    local    archive="${1}"
    local     domain="${2}"
    local   variable="${3}"
    local    product="${4}"
    local resolution="${5}"
    local    version="${6}"
    local  startyear="${7}"
	local    endyear="${8}"
	local      tStep="${9}"

 	local      files=""
	local  separator=" "

	for (( year = ${startyear}; year <= ${endyear}; ++year ))
	do
		local  filename=$(RGISfile "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "TS" "${tStep}" "${year}")
		local     files="${files}${separator}${filename}"
		local separator=" "
	done

	local filename=$(RGISfile  "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "TS" "${tStep}" "${startyear}-${endyear}")
	local    title=$(RGIStitle              "${domain}" "${variable}" "${product}" "${resolution}" "TS" "${tStep}" "${startyear}-${endyear}" "${version}")
	local  subject=$(RGISlookupSubject  "${variable}")
    local shadeset=$(RGISlookupShadeset "${variable}")

	grdAppendLayers -t "${title}" -d "${domain}" -u "${subject}" -v "${version}" -o "${filename}" ${files}
}

function RGISsetHeader ()
{
	local      archive="${1}"; shift
	local       domain="${1}"; shift
	local      subject="${1}"; shift
	local      product="${1}"; shift
	local   resolution="${1}"; shift
	local    tStepType="${1}"; shift
	local        tStep="${1}"; shift
	local    timeRange="${1}"; shift
	local      version="${1}"; shift
	local        title="${1}"; shift
	local      comment="${1}"; shift
    local     citation="${1}"; shift
	local  institution="${1}"; shift
	local   sourceInst="${1}"; shift
	local sourcePerson="${1}"; shift

    if [[ "${tStepType}" == "Static" ]]
    then
	    local rgisFile="$(RGISfile "${archive}" "${domain}" "${subject}" "${product}" "${resolution}" "${tStepType}")"
	elif [[ "${tStep}" == "" ]]
	then
	    local rgisFile="$(RGISfile "${archive}" "${domain}" "${subject}" "${product}" "${resolution}" "${tStepType}" "Annual")"
	elif [[ "${timeRange}" == "" ]]
	then
	    local rgisFile="$(RGISfile "${archive}" "${domain}" "${subject}" "${product}" "${resolution}" "${tStepType}" "${tStep}")"
	else
	    local rgisFile="$(RGISfile "${archive}" "${domain}" "${subject}" "${product}" "${resolution}" "${tStepType}" "${tStep}" "${timeRange}")"
	fi

	if [[ "${rgisFile}" == "" ]]
	then
		echo "Missing ${rgisFile} in RGISsetHeader"
		return 0
	fi
	if [[ "$(which finger)" == "" ]]
	then
		local person="Annonymous"
	else
		local person=$(finger $(env | grep "LOGNAME" | sed "s:LOGNAME=::") | grep Name | sed -e "s|.*Name: ||")
	fi

    local subString=""
	if [[ "${tStepType}"    == "" ]]; then local tStepType="static";  fi
	if [[ "${tStep}"        == "" ]]; then local     tStep="";        else local subString="${subString}, ${tStep}";     fi
	if [[ "${timeRange}"    == "" ]]; then local timeRange="";        else local subString="${subString}, ${timeRange}"; fi
	if [[ "${version}"      == "" ]]; then local   version="pre0.01"; fi
	local subString="${resolution}, ${subString}"

	if [[ "${title}"        == "" ]]; then local        title="$(RGIStitle "${domain}" "${subject}" "${product}" \("${subString}"\) "${version}")"; fi
	if [[ "${comment}"      == "" ]]; then local      comment="${domain} $(_RGISlookupFullName "${subject}") from ${product} at ${resolution}"; fi
	if [[ "${citation}"     == "" ]]; then local     citation="Pirated ${subject} from ${product}"; fi
	if [[ "${institution}"  == "" ]]; then local  institution="Advanced Science Research Center at the Graduate Center, CUNY"; fi
	if [[ "${sourceInst}"   == "" ]]; then local   sourceInst="City College of New York"; fi
	if [[ "${sourcePerson}" == "" ]]; then local sourcePerson="${person}"; fi
	setHeader  -t "${title}" -d "${domain}" -u "${subject}" -y "on" -c "${comment}" -i "${citation}" -n "${institution}" -o "${sourceInst}" -p "${sourcePerson}" -v "${version}" "${rgisFile}" "${rgisFile}"
}

function RGISAggregateTS ()
{
	local    archive="${1}"
	local     domain="${2}"
	local   variable="${3}"
	local    product="${4}"
	local resolution="${5}"
	local    version="${6}"
	local  startyear="${7}"
	local    endyear="${8}"
	local  fromtStep="${9}"
	local    totStep="${10}"

   local     files=""
   local separator=""

   for ((year = ${startyear}; year <= ${endyear} ; ++year))
   do
      local fromFile=$(RGISfile "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "TS" "${fromtStep}" "${year}")
      local   toFile=$(RGISfile "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "TS" "${totStep}"   "${year}")
      local    title=$(RGIStitle "${domain}" "${variable}" "${product}" "${resolution}" "TS" "${totStep}" "${year}" "${version}")
      local  subject=$(RGISlookupSubject    "${variable}")
      local shadeset=$(RGISlookupShadeset   "${variable}")
	  local   method=$(RGISlookupAggrMethod "${variable}")

      grdTSAggr -a "${method}" -e "$(RGISlookupTimeStep ${totStep})" -t "${title}" -d "${domain}" -u "${subject}" -s "${shadeset}" "${fromFile}" "${toFile}" || return 1
   done
}

function RGISClimatology ()
{
	local    archive="${1}"
	local     domain="${2}"
	local   variable="${3}"
	local    product="${4}"
	local resolution="${5}"
	local    version="${6}"
 	local  startyear="${7}"
	local    endyear="${8}"

	local     ltDir=$(RGISdirectory "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "LT" "monthly")
	local    tsFile=$(RGISfile      "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "TS" "monthly" "${startyear}-${endyear}")
	local    ltFile=$(RGISfile      "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "LT" "monthly" "${startyear}-${endyear}")
	local     title=$(RGIStitle                  "${domain}" "${variable}" "${product}" "${resolution}" "LT" "monthly" "${startyear}-${endyear}")
	local   subject=$(RGISlookupSubject  "${variable}")
	local  shadeset=$(RGISlookupShadeset "${variable}")

	[ -e "${ltDir}" ] || mkdir -p "${ltDir}" || return 1
	grdCycleMean	-t "${title}" -d "${domain}" -u "${subject}" -v "${version}" -s "${shadeset}" -n 12 "${tsFile}" - |\
	grdDateLayers   -e "month" - "${ltFile}" || return 1
	return 0
}

function RGISCellStats ()
{
	local    archive="${1}"
	local     domain="${2}"
	local   variable="${3}"
	local    product="${4}"
	local resolution="${5}"
	local    version="${6}"
 	local  startyear="${7}"
	local    endyear="${8}"

	local  statsDir=$(RGISdirectory  "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "Stats" "annual")
	local    tsFile=$(RGISfile       "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "TS"    "annual" "${startyear}-${endyear}")
	local statsFile=$(RGISfile       "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "Stats" "annual" "${startyear}-${endyear}")
	local     title=$(RGIStitle                   "${domain}" "${variable}" "${product}" "${resolution}" "Stats" "annual" "${startyear}-${endyear}" "${version}")
	local   subject=$(RGISlookupSubject "${variable}")

	[ -e "${statsDir}" ] || mkdir -p "${statsDir}" || return 1
	grdCellStats    -t "${title}" -u "${variable}" -d "${domain}" -v "${version}" "${tsFile}" "${statsFile}" || return 1

	local  annualLTfile=$(RGISfile "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "LT"      "annual"  "${startyear}-${endyear}")
	local         title=$(RGIStitle             "${domain}" "${variable}" "${product}" "${resolution}" "LT"      "annual"  "${startyear}-${endyear}" "${version}")
	grdExtractLayers -t "${title}" -f "avg" -l "avg" -d "${domain}" -u "$(RGISlookupSubject ${variable})" "${statsFile}" |\
	grdDateLayers -e "year" - "${annualLTfile}" || return 1

	local     maxLTfile=$(RGISfile "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "LTmax"    "annual"  "${startyear}-${endyear}")
	local         title=$(RGIStitle             "${domain}" "${variable}" "${product}" "${resolution}" "LTmax"    "annual"  "${startyear}-${endyear}" "${version}")
	grdExtractLayers -t "${title}" -f "Maximum" -l "Maximum" -d "${domain}" -u "$(RGISlookupSubject ${variable})" "${statsFile}" |\
	grdDateLayers -e "year" - "${maxLTfile}" || return 1

	local     minLTfile=$(RGISfile "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "LTmin"    "annual"  "${startyear}-${endyear}")
	local         title=$(RGIStitle             "${domain}" "${variable}" "${product}" "${resolution}" "LTmin"    "annual"  "${startyear}-${endyear}" "${version}")
	grdExtractLayers -t "${title}" -f "Minimum" -l "Minimum" -d "${domain}" -u "$(RGISlookupSubject ${variable})" "${statsFile}" |\
	grdDateLayers -e "year" - "${minLTfile}" || return 1

	local   rangeLTfile=$(RGISfile "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "LTrange"  "annual" "${startyear}-${endyear}")
	local         title=$(RGIStitle             "${domain}" "${variable}" "${product}" "${resolution}" "LTrange"  "annual" "${startyear}-${endyear}" "${version}")
	grdExtractLayers -t "${title}" -f "Range"   -l "Range"   -d "${domain}" -u "$(RGISlookupSubject ${variable})" "${statsFile}" |\
	grdDateLayers -e "year" - "${rangeLTfile}" || return 1

	local   slopeLTfile=$(RGISfile "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "LTslope"  "annual" "${startyear}-${endyear}")
	local         title=$(RGIStitle             "${domain}" "${variable}" "${product}" "${resolution}" "LTslope"  "annual" "${startyear}-${endyear}" "${version}")
	grdExtractLayers -t "${title}" -f "SigSlopeB1" -l "SigSlopeB1" -d "${domain}" -u "$(RGISlookupSubject ${variable})" "${statsFile}" |\
	grdDateLayers -e "year" - "${slopeLTfile}" || return 1

	local  stdDevLTfile=$(RGISfile "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "LTstdDev" "annual" "${startyear}-${endyear}")
	local         title=$(RGIStitle             "${domain}" "${variable}" "${product}" "${resolution}" "LTstdDev" "annual" "${startyear}-${endyear}" "${version}")
	grdExtractLayers -t "${title}" -f "StdDev" -l "StdDev" -d "${domain}" -u "$(RGISlookupSubject ${variable})" "${statsFile}" |\
	grdDateLayers -e "year" - "${stdDevLTfile}" || return 1

#	rm "${statsFile}"
	return 0
}

function RGISStatistics ()
{
	local    archive="${1}"
	local     domain="${2}"
	local   variable="${3}"
	local    product="${4}"
	local resolution="${5}"
	local    version="${6}"
	local  startyear="${7}"
	local    endyear="${8}"

	RGISAppend "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "${version}" "${startyear}" "${endyear}" "monthly" || return 1
	RGISAppend "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "${version}" "${startyear}" "${endyear}" "annual"  || return 1

	RGISClimatology "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "${version}" "${startyear}" "${endyear}"      || return 1
	RGISCellStats   "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "${version}" "${startyear}" "${endyear}"      || return 1
	local  annualTSfile=$(RGISfile "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "TS"    "annual"  "${startyear}-${endyear}")
	local monthlyTSfile=$(RGISfile "${archive}" "${domain}" "${variable}" "${product}" "${resolution}" "TS"    "monthly" "${startyear}-${endyear}")
	[ -e  "${annualTSfile}" ] && rm  "${annualTSfile}"
	[ -e "${monthlyTSfile}" ] && rm "${monthlyTSfile}"
	return 0
}

if [[ $0 == "${BASH_SOURCE}" ]]
then
    if (( $# > 1))
    then
        FUNCTION="$1"; shift
        ARGUMENTS="$@"
        ${FUNCTION} ${ARGUMENTS}
    fi
fi