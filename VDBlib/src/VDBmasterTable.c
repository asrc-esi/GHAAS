/******************************************************************************

GHAAS Command Line Library V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2023, UNH - CCNY

VDBvariableDB.c

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <string.h>
#include <vdb.h>

#define VDBcontinuous    "continuous"
#define VDBdiscrete      "discrete"
#define VDBline          "line"
#define VDBnetwork       "network"
#define VDBpoint         "point"

#define VDBblue          "blue"
#define VDBblue2red      "blue-to-red"
#define VDBelevation     "elevation"
#define VDBgrey          "grey"
#define VDBavg           "avg"
#define VDBmax           "max"
#define VDBsum           "sum"
#define VDBnotApplicable "N/A"

#define VDBsamplingPoint "point"
#define VDBsamplingZone  "zone"

static VDBmasterTable_t _VDBmasterTable2 [] = {
        {"biochemistry",  "c_litterfall",                                 "C-LitterFall",                                 "C LitterFall",                          VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "din_areal_loading",                            "DIN-AreaLoading",                              "DIN Areal Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "din_point_loading",                            "DIN-PointLoading",                             "DIN Point Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "doc_areal_loading",                            "DOC-ArealLoading",                             "DOC Areal Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "doc_point_loading",                            "DOC-PointLoading",                             "DOC Point Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "don_areal_loading",                            "DON-ArealLoading",                             "DON Areal Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "don_point_loading",                            "DON-PointLoading",                             "DON Point Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "n_litterfall",                                 "N-LitterFall",                                 "N LitterFall",                          VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "nh4_areal_loading",                            "NH4-ArealLoading",                             "NH4 Areal Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "nh4_point_loading",                            "Nh4-PointLoading",                             "NH4 Point Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "no3_areal_loading",                            "NO3-ArealLoading",                             "NO3 Areal Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "no3_point_loading",                            "NO3-PointLoading",                             "NO3 Point Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"crop",          "growing_season1",                              "Crops-GrowingSeason1",                         "Growing Season 1",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "growing_season2",                              "Crops-GrowingSeason2",                         "Growing Season 2",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "irrigated_area_fraction",                      "Irrigation-AreaFraction",                      "Irrigated Area Fraction",               VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "irrigation_efficiency",                        "Irrigation-Efficiency",                        "Irrigation Efficiency",                 VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "irrigation_gross_demand",                      "IrrGrossDemand",                               "Irrigation Gross Demand",               VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"crop",          "cropland_fraction",                            "Cropland-Fraction",                            "Cropland Fraction",                     VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "cropping_intensity",                           "Crops-Intensity",                              "Cropping Intensity",                    VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "other_crop_fraction",                          "CropFraction-Other",                           "Other Crop Fraction",                   VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "perennial_crop_fraction",                      "CropFraction-Perennial",                       "Perennial Crop Fraction",               VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "rice_crop_fraction",                           "CropFraction-Rice",                            "Rice Crop Fraction",                    VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "rice_percolation_rate",                        "Crops-RicePercolationRate",                    "Rice Percolation Rate",                 VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "vegetables_crop_fraction",                     "CropFraction-Vegetables",                      "Vegetables Crop Fraction",              VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_barley",                         "CropFraction-Barley",                          "Crop Fraction - Barley",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_cassava",                        "CropFraction-Cassava",                         "Crop Fraction - Cassava",               VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_cotton",                         "CropFraction-Cotton",                          "Crop Fraction - Cotton",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_groundnut",                      "CropFraction-Groundnut",                       "Crop Fraction - Groundnut",             VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_maize",                          "CropFraction-Maize",                           "Crop Fraction - Maize",                 VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_millet",                         "CropFraction-Millet",                          "Crop Fraction - Millet",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_oilpalm",                        "CropFraction-Oilpalm",                         "Crop Fraction - Oilpalm",               VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_other",                          "CropFraction-Other",                           "Crop Fraction - Other",                 VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_potato",                         "CropFraction-Potato",                          "Crop Fraction - Potato",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_pulse",                          "CropFraction-Pulse",                           "Crop Fraction - Pulse",                 VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_rapeseed",                       "CropFraction-Rapeseed",                        "Crop Fraction - Rapeseed",              VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_rice",                           "CropFraction-Rice",                            "Crop Fraction - Rice",                  VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_rye",                            "CropFraction-Rye",                             "Crop Fraction - Rye",                   VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_sorghum",                        "CropFraction-Sorghum",                         "Crop Fraction - Sorghum",               VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_soybean",                        "CropFraction-Soybean",                         "Crop Fraction - Soybean",               VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_sugarbeet",                      "CropFraction-Sugarbeet",                       "Crop Fraction - Sugarbeet",             VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_sugarcane",                      "CropFraction-Sugarcane",                       "Crop Fraction - Sugarcane",             VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_sunflower",                      "CropFraction-Sunflower",                       "Crop Fraction - Sunflower",             VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_fraction_wheat",                          "CropFraction-Wheat",                           "Crop Fraction - Wheat",                 VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"energy",        "hydropower_plant",                             "HydroPower-Plant",                             "Hydro Power Plant",                     VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"energy",        "hydropower_subbasin",                          "HydroPower-Subbasin",                          "Hydro Power Subbasin",                  VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"energy",        "thermalpower_plant",                           "ThermalPower-Plant",                           "Thermal Power Plant",                   VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"energy",        "thermalpower_subbasin",                        "ThermalPower-Subbasin",                        "Thermal Power Subbasin",                VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"energy",        "electricity_region",                           "Electricity-Region",                           "Electricity Balancing Region",          VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"energy",        "electricity_region_crossing",                  "Electricity-RegionCrossing",                   "Electricity Balancing Region Crossing", VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geopgraphy",    "continent",                                    "Continent",                                    "Continent",                             VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geopgraphy",    "continent_mask",                               "ContinentMask",                                "Continent Mask",                        VDBcontinuous, VDBgrey,          VDBavg,           VDBnotApplicable},
        {"geopgraphy",    "county",                                       "County",                                       "County",                                VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geopgraphy",    "county_crossing",                              "CountyCrossing",                               "County Crossing",                       VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geopgraphy",    "country",                                      "Country",                                      "Country",                               VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geopgraphy",    "country_crossing",                             "CountryCrossing",                              "Country Crossing",                      VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geopgraphy",    "state",                                        "State",                                        "State",                                 VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geopgraphy",    "state_crossing",                               "StateCrossing",                                "State Crossing",                        VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geopgraphy",    "hydro_unit",                                   "Hydro-Unit",                                   "Hydrologicalal Unit",                   VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geography",     "marine_region",                                "MarineRegion",                                 "Marine Region",                         VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geography",     "ocean",                                        "Ocean",                                        "Ocean",                                 VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"hydrography",   "daily_precipitation_fraction",                 "Precipitation-DailyFraction",                  "Daily Precipitation Fraction",          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"hydrography",   "discharge",                                    "RiverDischarge",                               "River Discharge",                       VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingPoint},
        {"hydrography",   "discharge_gauge",                              "RiverDischargeGauge",                          "River Discharge Gauge",                 VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"hydrography",   "evapotranspiration",                           "Evapotranspiration",                           "Evapotranspiration",                    VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"hydrography",   "potential_evapotranspiration",                 "PotEvapotranspiration",                        "Potential Evapotranspiration",          VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"hydrography",   "precipitation",                                "Precipitation",                                "Precipitation",                         VDBcontinuous, VDBblue,          VDBsum,           VDBsamplingZone},
        {"hydrography",   "rainfall",                                     "Rainfall",                                     "Rainfall",                              VDBcontinuous, VDBblue,          VDBsum,           VDBsamplingZone},
        {"hydrography",   "rain_pet",                                     "RainPET",                                      "Rain Potential ET",                     VDBcontinuous, VDBblue,          VDBsum,           VDBsamplingZone},
        {"hydrography",   "runoff",                                       "Runoff",                                       "Runoff",                                VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"hydrography",   "snowfall",                                     "SnowFall",                                     "Snowfall",                              VDBcontinuous, VDBblue,          VDBsum,           VDBsamplingZone},
        {"hydrography",   "soil_moisture",                                "SoilMoisture",                                 "Soil Moisture",                         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"hydrography",   "soil_moisture_change",                         "SoilMoistureChange",                           "Soil Moisture Change",                  VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "barren_percentage",                            "Landcover-PctBarren",                          "Barren Percentage",                     VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "deciduous_broadleaf_percentage",               "Landcover-PctBroadleafDeciduous",              "Deciduous Broadleaf Percentage",        VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "evergreen_broadleaf_percentage",               "Landcover-PctBroadleafEvergreen",              "Evergreen Broadleaf Percentage",        VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "deciduous_conifer_percentage",                 "Landcover-PctConiferDeciduous",                "Deciduous Conifer Percentage",          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "evergreen_conifer_percentage",                 "Landcover-PctConiferEvergreen",                "Evergreen Conifer Percentage",          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "cropland_percentage",                          "Landcover-PctCropland",                        "Cropland Percentage",                   VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "crop-natural_mosaic_percentage",               "Landcover-PctCropNaturalMosaic",               "Cropland-Natural Mosaic Percentage",    VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "grassland_percentage",                         "Landcover-PctGrassland",                       "Grassland Percentage",                  VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "mixed_forest_percentage",                      "Landcover-PctMixedForest",                     "Mixed Forest Percentage",               VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "mixed_savannah_percentage",                    "Landcover-PctSavannah",                        "Savannah Percentage",                   VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "mixed_woody_savannah_percentage",              "Landcover-PctSavannahWoody",                   "Woody Savannah Percentage",             VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "mixed_closed_shrubland_percentage",            "Landcover-PctShrublandClosed",                 "Closed Shrubland Percentage",           VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "mixed_open_shrubland_percentage",              "Landcover-PctShrublandOpen",                   "Open Shrubland Percentage",             VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "mixed_snow_ice_percentage",                    "Landcover-PctSnowIce",                         "Snow-Ice Percentage",                   VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "urban_built-up_percentage",                    "Landcover-PctUrbanBuiltUp",                    "Urban Built-Up Percentage",             VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "vegetation_percentage",                        "Landcover-PctVegetation",                      "Vegetation Fraction",                   VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "water_percentage",                             "Landcover-PctWater",                           "Water Percentage",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_cover",    "wetland_percentage",                           "Landcover-PctWetland",                         "Wetland Percentage",                    VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"meteorology",   "air_temperature",                              "AirTemperature",                               "Air Temperature",                       VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingZone},
        {"meteorology",   "min_air_temperature",                          "AirTemperature-Min",                           "Minimum Air Temperature",               VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingZone},
        {"meteorology",   "max_air_temperature",                          "AirTemperature-Max",                           "Maximum Air Temperature",               VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingZone},
        {"meteorology",   "diurnal_air_temperature_range",                "AirTemperature-Range",                         "Diurnal AirTemperature Range",          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"meteorology",   "cloud_cover",                                  "CloudCover",                                   "Cloud Cover",                           VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"meteorology",   "heatindex",                                    "Heatindex",                                    "Heatindex",                             VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingZone},
        {"meteorology",   "max_heatindex",                                "Heatindex-Max",                                "Maximum Heatindex",                     VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingZone},
        {"meteorology",   "min_heatindex",                                "Heatindex-Min",                                "Minimum Heatindex",                     VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingZone},
        {"meteorology",   "dewpoint_temperature",                         "Humidity-Dewpoint",                            "Dewpoint Temperature",                  VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingZone},
        {"meteorology",   "relative_humidity",                            "Humidity-Relative",                            "Relative Humidity",                     VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"meteorology",   "specific_humidity",                            "Humidity-Specific",                            "Specific Humidity",                     VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"meteorology",   "vapor_pressure",                               "Humidity-VaporPressure",                       "Vapor Pressure",                        VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"meteorology",   "surface_air_pressure",                         "AirPressure",                                  "Air Pressure",                          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"meteorology",   "v_wind_speed",                                 "WindSpeed-V10m",                               "Wind Speed V-direction",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"meteorology",   "wind_speed",                                   "WindSpeed",                                    "Wind Speed",                            VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"meteorology",   "u_wind_speed",                                 "WindSpeed-U10m",                               "Wind Speed U-direction",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"network",       "basin",                                        "Basin",                                        "Basin",                                 VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"network",       "mouth",                                        "Mouth",                                        "Mouth",                                 VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"network",       "cell_area",                                    "CellArea",                                     "Cell Area",                             VDBcontinuous, VDBgrey,          VDBmax,           VDBsamplingZone},
        {"network",       "cell_slope",                                   "CellSlope",                                    "Cell Slope",                            VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"network",       "confluence",                                   "Confluence",                                   "Confluence",                            VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"network",       "distance_to_ocean",                            "DistToOcean",                                  "Distance to Ocean",                     VDBcontinuous, VDBgrey,          VDBmax,           VDBsamplingPoint},
        {"network",       "network",                                      "Network",                                      "STNetwork",                             VDBnetwork,    VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"network",       "ocean_basin",                                  "OceanBasin",                                   "Ocean Basin",                           VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"network",       "streamline",                                   "Streamline",                                   "Streamline",                            VDBline,       VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"network",       "subbasin",                                     "Subbasin",                                     "Subbasin",                              VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"network",       "travel_time",                                  "TravelTime",                                   "Travel Time",                           VDBcontinuous, VDBgrey,          VDBmax,           VDBsamplingZone},
        {"network",       "upstream_area",                                "UpstreamArea",                                 "Upstream Area",                         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingPoint},
        {"radiation",     "longwave_clear_sky_radiation",                 "Radiation-Longwave-ClearSky",                  "Clear Sky Longwave Radiation",          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"radiation",     "longwave_downwelling_radiation",               "Radiation-Longwave-Downwelling",               "Downwelling Longwave Radiation",        VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"radiation",     "longwave_upwelling_radiation",                 "Radiation-Longwave-Upwelling",                 "Upwelling Longwave Radiation",          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"radiation",     "shortwave_clear_sky_radiation",                "Radiation-Shortwave-ClearSky",                 "Clear Sky Shortwave Radiation",         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"radiation",     "shortwave_downwelling_radiation",              "Radiation-Shortwave-Downwelling",              "Downwelling Shortwave Radiation",       VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"radiation",     "shortwave_upwelling_radiation",                "Radiation-Shortwave-Upwelling",                "Upwelling Shortwave Radiation",         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"reservoir",     "reservoir_dam",                                "Reservoir-Dam",                                "Reservoir Dam",                         VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"reservoir",     "reservoir_area",                               "Reservoir-Area",                               "Reservoir Area",                        VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingPoint},
        {"reservoir",     "reservoir_capacity",                           "Reservoir-Capacity",                           "Reservoir Capacity",                    VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingPoint},
        {"reservoir",     "reservoir_height",                             "Reservoir-Height",                             "Reservoir Height",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingPoint},
        {"reservoir",     "reservoir_irrigation_demand",                  "Reservoir-IrrigationDemand",                   "Reservoir Irrigation Water Demand",     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"reservoir",     "reservoir_nonirrigation_demand",               "Reservoir-NonIrrigationDemand",                "Reservoir Non-Irrigation Water Demand", VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"reservoir",     "reservoir_subbasin",                           "Reservoir-Subbasin",                           "Reservoir Subbasin",                    VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"reservoir",     "reservoir_volume",                             "Reservoir-Volume",                             "Reservoir Volume",                      VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingPoint},
        {"reservoir",     "small_reservoir_coefficient",                  "SmallReservoir-Coefficient",                   "Small-reservoir Coefficient",           VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"river",         "river_slope",                                  "Slope",                                        "River Slope",                           VDBcontinuous, VDBelevation,     VDBavg,           VDBsamplingPoint},
        {"river",         "river_slope-min",                              "Slope-Min",                                    "Minimum River Slope",                   VDBcontinuous, VDBelevation,     VDBavg,           VDBsamplingPoint},
        {"river",         "river_slope-max",                              "Slope-Max",                                    "Maximum River Slope",                   VDBcontinuous, VDBelevation,     VDBavg,           VDBsamplingPoint},
        {"river",         "river_temperature",                            "River-Temperature",                            "River Temperature",                     VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingPoint},
        {"river",         "river_temperature_gauge",                      "River-Temperature-Gauge",                      "River Temperature Gauge",               VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"river",         "river_temperature_subbasin",                   "River-Temperature-Subbasin",                   "River Temperature Subbasin",            VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"socio_economy", "population",                                   "Population",                                   "Population",                            VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"socio_economy", "population_density",	                          "Population-Density",                           "Population Density",                    VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"socio_economy", "population-rural",                             "Population-Rural",                             "Rural Population",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"socio_economy", "population-urban",                             "Population-Urban",                             "Urban Population",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "available_water_capacity",                     "Soil-AvailableWaterCapacity",                  "Avaible Water Capacity",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "bulk_density",                                 "Soil-BulkDensity",                             "Bulk Density",                          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "depth_to_horizon",                             "Soil-DepthToRHorizon",                         "Depth to R Horizon",                    VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "depth_to_bedrock",                             "Soil-DepthToBedrock",                          "Depth to Bedrock",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "field_capacity",                               "Soil-FieldCapacity",                           "Field Capacity",                        VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "clay_percent_by_volume",                       "Soil-ClayPercentByVolume",                     "Percent Clay by Volume",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "clay_percent_by_mass",                         "Soil-ClayPercentByMass",                       "Percent Clay by Mass",                  VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "coarse_percent_by_volume",                     "Soil-CoarsePercentByVolume",                   "Percent Coarse by Volume",              VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "coarse_percent_by_mass",                       "Soil-CoarsePercentByMass",                     "Percent Coarse by Mass",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "particle_density",                             "Soil-ParticleDensity",                         "Particle Density",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "porosity",                                     "Soil-Porosity",                                "Porosity",                              VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "probability_of_horizon",                       "Soil-ProbabilityOfRHorizon",                   "Probability of R Horizon",              VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "rooting_depth",                                "Soil-RootingDepth",                            "Rooting Depth",                         VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"soil",          "sand_percent_by_volume",                       "Soil-SandPercentByVolume",                     "Percent Sand by Volume",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "sand_percent_by_mass",                         "Soil-SandPercentByMass",                       "Percent Sand by Mass",                  VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "silt_percent_by_volume",                       "Soil-SiltPercentByVolume",                     "Percent Silt by Volume",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "silt_percent_by_mass",                         "Soil-SiltPercentByMass",                       "Percent Silt by Mass",                  VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "wilting_point",                                "Soil-WiltingPoint",                            "Wilting Point",                         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"topography",    "bathymetry",                                   "Bathymetry",                                   "Bathymetry",                            VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"topography",    "bathymetry_min",                               "Bathymetry-Min",                               "Minimum Depth",                         VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"topography",    "bathymetry_max",                               "Bathymetry-Max",                               "Maximum Depth",                         VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"topography",    "elevation",                                    "Elevation",                                    "Elevation",                             VDBcontinuous, VDBelevation,     VDBavg,           VDBsamplingZone},
        {"topography",    "elevation_max",                                "Elevation-Max",                                "Maximum Elevation",                     VDBcontinuous, VDBelevation,     VDBavg,           VDBsamplingZone},
        {"topography",    "elevation_min",                                "Elevation-Min",                                "Minimum Elevation",                     VDBcontinuous, VDBelevation,     VDBavg,           VDBsamplingZone}};

static VDBmasterTable_t _VDBmasterTable3 [] = {
        {"biochemistry",  "biochem_c_litterfall",                         "Biochem-C-LitterFall",                         "C LitterFall",                          VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "biochem_din_areal_loading",                    "Biochem-DIN-AreaLoading",                      "DIN Areal Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "biochem_din_point_loading",                    "Biochem-DIN-PointLoading",                     "DIN Point Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "biochem_doc_areal_loading",                    "Biochem-DOC-ArealLoading",                     "DOC Areal Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "biochem_doc_point_loading",                    "Biochem-DOC-PointLoading",                     "DOC Point Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "biochem_don_areal_loading",                    "Biochem-DON-ArealLoading",                     "DON Areal Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "biochem_don_point_loading",                    "Biochem-DON-PointLoading",                     "DON Point Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "biochem_n_litterfall",                         "Biochem-N-LitterFall",                         "N LitterFall",                          VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "biochem_nh4_areal_loading",                    "Biochem-NH4-ArealLoading",                     "NH4 Areal Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "biochem_nh4_point_loading",                    "Biochem-Nh4-PointLoading",                     "NH4 Point Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "biochem_no3_areal_loading",                    "Biochem-NO3-ArealLoading",                     "NO3 Areal Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"biochemistry",  "biochem_no3_point_loading",                    "Biochem-NO3-PointLoading",                     "NO3 Point Loading",                     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"crop",          "crop_growing_season1",                         "Crop-GrowingSeason1",                          "Growing Season 1",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_growing_season2",                         "Crop-GrowingSeason2",                          "Growing Season 2",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_irrigation_intensity",                    "Crop-Irrigation-Intensity",                    "Irrigated Cropping Intensity",          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_irrigation_efficiency",                   "Crop-Irrigation-Efficiency",                   "Irrigation Efficiency",                 VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"crop",          "crop_irrigation_demand_gross",                 "Crop-Irrigation-DemandGross",                  "Irrigation Gross Water Demand",         VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"crop",          "crop_irrigation_demand_net",                   "Crop-Irrigation-DemandNet",                    "Irrigation Net Water Demand",           VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"crop",          "crop_irrigation_return_flow",                  "Crop-Irrigation-ReturnFlow",                   "Irrigation Return Flow",                VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"crop",          "crop_irrigation_rice_percolation",             "Crop-Irrigation-RicePercolationRate",          "Rice Percolation Rate",                 VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"energy",        "energy_hydropower_plant",                      "Energy-HydroPower-Plant",                      "Hydropower Plant",                      VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"energy",        "energy_hydropower_subbasin",                   "Energy-HydroPower-Subbasin",                   "Hydropower Subbasin",                   VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"energy",        "energy_thermalpower_plant",                    "Energy-ThermalPower-Plant",                    "Thermal Power Plant",                   VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"energy",        "energy_thermalpower_subbasin",                 "Energy-ThermalPower-Subbasin",                 "Thermal Power Subbasin",                VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"energy",        "energy_electricity_region",                    "Energy-Electricity-Region",                    "Electricity Balancing Region",          VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"energy",        "energy_electricity_region_crossing",           "Energy-Electricity-RegionCrossing",            "Electricity Balancing Region Crossing", VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geography",     "geography_continent",                          "Geography-Continent",                          "Continent",                             VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geography",     "geography_county",                             "Geography-County",                             "County",                                VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geography",     "geography_county_crossing",                    "Geography-CountyCrossing",                     "County Crossing",                       VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geography",     "geography_country",                            "Geography-Country",                            "Country",                               VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geography",     "geography_country_crossing",                   "Geography-CountryCrossing",                    "Country Crossing",                      VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geography",     "geography_state",                              "Geography-State",                              "State",                                 VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geography",     "geography_state_crossing",                     "Geography-StateCrossing",                      "State Crossing",                        VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geography",     "geography_landmask",                           "Geography-Landmask",                           "Landmask",                              VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"geography",     "geography_hydrological_unit",                  "Geography-Hydrologicalal-Unit",                "Hydrologicalal Unit",                   VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geography",     "geography_hydrological_unit_crossing",         "Geography-Hydrologicalal-UnitCrossing",        "Hydrologicalal Unit Crossing",          VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geography",     "geography_marine_region",                      "Geography-MarineRegion",                       "Marine Region",                         VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"geography",     "geography_ocean",                              "Geography-Ocean",                              "Ocean",                                 VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"hydrography",   "hydrography_discharge",                        "Hydrography-Discharge",                        "Discharge",                             VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingPoint},
        {"hydrography",   "hydrography_discharge_gauge",                  "Hydrography-Discharge-Gauge",                  "Discharge Gauge",                       VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"hydrography",   "hydrography_discharge_subbasin",               "Hydrography-Discharge-Subbasin",               "Discharge Subbasin",                       VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"hydrography",   "hydrography_drought_index_palmer",             "Hydrography-DroughtIndex-Palmer",              "Palmer Drought Index",                  VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"hydrography",   "hydrography_evapotranspiration",               "Hydrography-Evapotranspiration",               "Evapotranspiration",                    VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"hydrography",   "hydrography_evapotranspiration_deficit",       "Hydrography-Evapotranspiration-Deficit",       "Water Deficit",                         VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"hydrography",   "hydrography_evapotranspiration_potential",     "Hydrography-Evapotranspiration-Potential",     "Potential Evapotranspiration",          VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"hydrography",   "hydrography_evapotranspiration_potential_land","Hydrography-Evapotranspiration-PotentialLand", "Potenital Evapotranspiration - Land",   VDBcontinuous, VDBblue,          VDBsum,           VDBsamplingZone},
        {"hydrography",   "hydrography_evapotranspiration_potential_crop","Hydrography-Evapotranspiration-PotentialCrop", "Potential Evapotranspiration - Crop",   VDBcontinuous, VDBblue,          VDBsum,           VDBsamplingZone},
        {"hydrography",   "hydrography_precipitation",                    "Hydrography-Precipitation",                    "Precipitation",                         VDBcontinuous, VDBblue,          VDBsum,           VDBsamplingZone},
        {"hydrography",   "hydrography_precipitation_daily_fraction",     "Hydrography-Precipitation-DailyFraction",      "Daily Precipitation Fraction",          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"hydrography",   "hydrography_precipitation_rainfall",           "Hydrography-Precipitation-Rainfall",           "Precipitation",                         VDBcontinuous, VDBblue,          VDBsum,           VDBsamplingZone},
        {"hydrography",   "hydrography_precipitation_snowfall",           "Hydrography-Precipitation-Snowfall",           "Snowfall",                              VDBcontinuous, VDBblue,          VDBsum,           VDBsamplingZone},
        {"hydrography",   "hydrography_runoff",                           "Hydrography-Runoff",                           "Runoff",                                VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"hydrography",   "hydrography_runoff_surface",                   "Hydrography-Runoff-Surface",                   "Surface Runoff",                        VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"hydrography",   "hydrography_runoff_subsurface",                "Hydrography-Runoff-Subsurface",                "Subsurface Runoff",                     VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"hydrography",   "hydrography_snow_water_equivalent",            "Hydrography-Snow-WaterEquivalent",             "Snow Water Equivalent",                 VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"hydrography",   "hydrography_soil_moisture",                    "Hydrography-SoilMoisture",                     "Soil Moisture",                         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"hydrography",   "hydrography_soil_moisture_change",             "Hydrography-SoilMoisture-Change",              "Soil Moisture Change",                  VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_barren",                                  "LandFraction-Barren",                          "Barren Area Fraction",                  VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop",                                    "LandFraction-Crop",                            "Crop Area Fraction",                    VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated",                          "LandFraction-CropIrrigated",                   "Irrigated Crops",                       VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_banana",                   "LandFraction-CropIrrigated-Banana",            "Irrigated Banana",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_barley",                   "LandFraction-CropIrrigated-Barley",            "Irrigated Barley",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_cassava",                  "LandFraction-CropIrrigated-Cassava",           "Irrigated Cassava",                     VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_cereals",                  "LandFraction-CropIrrigated-Cereals",           "Irrigated Cereals",                     VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_cotton",                   "LandFraction-CropIrrigated-Cotton",            "Irrigated Cotton",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_fodder",                   "LandFraction-CropIrrigated-Fodder",            "Irrigated Fodder",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_groundnut",                "LandFraction-CropIrrigated-Groundnut",         "Irrigated Groundnut",                   VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_maize",                    "LandFraction-CropIrrigated-Maize",             "Irrigated Maize",                       VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_millet",                   "LandFraction-CropIrrigated-Millet",            "Irrigated Millet",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_nes",                      "LandFraction-CropIrrigated-NES",               "Irrigated NES",                         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_oilpalm",                  "LandFraction-CropIrrigated-Oilpalm",           "Irrigated Oilpalm",                     VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_potato",                   "LandFraction-CropIrrigated-Potato",            "Irrigated Potato",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_pulse",                    "LandFraction-CropIrrigated-Pulse",             "Irrigated Pulse",                       VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_rapeseed",                 "LandFraction-CropIrrigated-Rapeseed",          "Irrigated Rapeseed",                    VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_rice",                     "LandFraction-CropIrrigated-Rice",              "Irrigated Rice",                        VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_rye",                      "LandFraction-CropIrrigated-Rye",               "Irrigated Rye",                         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_sorghum",                  "LandFraction-CropIrrigated-Sorghum",           "Irrigated Sorghum",                     VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_soybean",                  "LandFraction-CropIrrigated-Soybean",           "Irrigated Soybean",                     VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_stimulant",                "LandFraction-CropIrrigated-Stimulant",         "Irrigated Stimulant",                   VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_sugarbeet",                "LandFraction-CropIrrigated-Sugarbeet",         "Irrigated Sugarbeet",                   VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_sugarcane",                "LandFraction-CropIrrigated-Sugarcane",         "Irrigated Sugarcane",                   VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_sunflower",                "LandFraction-CropIrrigated-Sunflower",         "Irrigated Sunflower",                   VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_tobacco",                  "LandFraction-CropIrrigated-Tobacco",           "Irrigated Tobacco",                     VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_vegetables",               "LandFraction-CropIrrigated-Vegetables",        "Irrigated Vegetables",                  VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_wheat",                    "LandFraction-CropIrrigated-Wheat",             "Irrigated Wheat",                       VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_yamsan",                   "LandFraction-CropIrrigated-Yasman",            "Irrigated Yasman and other roots",      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_other",                    "LandFraction-CropIrrigated-Other",             "Irrigated Other",                       VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_irrigated_perennial",                "LandFraction-CropIrrigated-Perennial",         "Irrigated Perennial",                   VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed",                            "LandFraction-CropRainfed",                     "Rainfed Crops",                         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_intensity",                  "LandFraction-CropRainfed-Intensity",           "Rainfed Cropping Intensity",            VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_banana",                     "LandFraction-CropRainfed-Banana",              "Rainfed Banana",                        VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfef_barley",                     "LandFraction-CropRainfed-Barley",              "Rainfed Barley",                        VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_cassava",                    "LandFraction-CropRainfed-Cassava",             "Rainfed Cassava",                       VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_cereals",                    "LandFraction-CropRainfed-Cereals",             "Rainfed Cereals",                       VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_cotton",                     "LandFraction-CropRainfed-Cotton",              "Rainfed Cotton",                        VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_fodder",                     "LandFraction-CropRainfed-Fodder",              "Rainfed Fodder",                        VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_groundnut",                  "LandFraction-CropRainfed-Groundnut",           "Rainfed Groundnut",                     VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_maize",                      "LandFraction-CropRainfed-Maize",               "Rainfed Maize",                         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_millet",                     "LandFraction-CropRainfed-Millet",              "Rainfed Millet",                        VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_nes",                        "LandFraction-CropRainfed-NES",                 "Rainfed NES",                           VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_oilpalm",                    "LandFraction-CropRainfed-Oilpalm",             "Rainfed Oilpalm",                       VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_potato",                     "LandFraction-CropRainfed-Potato",              "Rainfed Potato",                        VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_pulse",                      "LandFraction-CropRainfed-Pulse",               "Rainfed Pulse",                         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_rapeseed",                   "LandFraction-CropRainfed-Rapeseed",            "Rainfed Rapeseed",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_rice",                       "LandFraction-CropRainfed-Rice",                "Rainfed Rice",                          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_rye",                        "LandFraction-CropRainfed-Rye",                 "Rainfed Rye",                           VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_sorghum",                    "LandFraction-CropRainfed-Sorghum",             "Rainfed Sorghum",                       VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_soybean",                    "LandFraction-CropRainfed-Soybean",             "Rainfed Soybean",                       VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_stimulant",                  "LandFraction-CropRainfed-Stimulant",           "Rainfed Stimulant",                     VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_sugarbeet",                  "LandFraction-CropRainfed-Sugarbeet",           "Rainfed Sugarbeet",                     VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_sugarcane",                  "LandFraction-CropRainfed-Sugarcane",           "Rainfed Sugarcane",                     VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_sunflower",                  "LandFraction-CropRainfed-Sunflower",           "Rainfed Sunflower",                     VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_tobacco",                    "LandFraction-CropRainfed-Tobacco",             "Rainfed Tobacco",                       VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_vegetables",                 "LandFraction-CropRainfed-Vegetables",          "Rainfed Vegetables",                    VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_wheat",                      "LandFraction-CropRainfed-Wheat",               "Rainfed Wheat",                         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_crop_rainfed_yamsan",                     "LandFraction-CropRainfed-Yasman",              "Rainfed Yasman and other roots",        VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_vegetation",                              "LandFraction-Vegetation",                      "Vegetation Area Area Fraction",         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_vegetation_deciduous_broadleaf",          "LandFraction-Vegetation-BroadleafDeciduous",   "Deciduous Broadleaf Area Fraction",     VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_vegetation_evergreen_broadleaf",          "LandFraction-Vegetation-BroadleafEvergreen",   "Evergreen Broadleaf Area Fraction",     VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_vegetation_deciduous_conifer",            "LandFraction-Vegetation-ConiferDeciduous",     "Deciduous Conifer Area Fraction",       VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_vegetation_evergreen_conifer",            "LandFraction-Vegetation-ConiferEvergreen",     "Evergreen Conifer Area Fraction",       VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_vegetation_grassland",                    "LandFraction-Vegetation-Grassland",            "Grassland Area Fraction",               VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_vegetation_mixed_forest",                 "LandFraction-Vegetation-MixedForest",          "Mixed Forest Area Fraction",            VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_vegetation_mixed_savannah",               "LandFraction-Vegetation-Savannah",             "Savannah Area Fraction",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_vegetation_mixed_woody_savannah",         "LandFraction-Vegetation-SavannahWoody",        "Woody Savannah Area Fraction",          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_vegetation_mixed_closed_shrubland",       "LandFraction-Vegetation-ShrublandClosed",      "Closed Shrubland Area Fraction",        VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_vegetation_mixed_open_shrubland",         "LandFraction-Vegetation-ShrublandOpen",        "Open Shrubland Area Fraction",          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_snow_ice",                                "LandFraction-SnowIce",                         "Snow-Ice Area Fraction",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_urban_built-up",                          "LandFraction-UrbanBuiltUp",                    "Urban Built-Up Area Fraction",          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_water",                                   "LandFraction-Water",                           "Water Area Fraction",                   VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"land_fraction", "land_wetland",                                 "LandFraction-Wetland",                         "Wetland Area Fraction",                 VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"radiation",     "radiation_longwave_clear_sky",                 "Radiation-Longwave-ClearSky",                  "Clear Sky Longwave Radiation",          VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"radiation",     "radiation_longwave_downwelling",               "Radiation-Longwave-Downwelling",               "Downwelling Longwave Radiation",        VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"radiation",     "radiation_longwave_upwelling",                 "Radiation-Longwave-Upwelling",                 "Upwelling Longwave Radiation",          VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"radiation",     "radiation_shortwave_clear_sky",                "Radiation-Shortwave-ClearSky",                 "Clear Sky Shortwave Radiation",         VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"radiation",     "radiation_shortwave_downwelling",              "Radiation-Shortwave-Downwelling",              "Downwelling Shortwave Radiation",       VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"radiation",     "radiation_shortwave_upwelling",                "Radiation-Shortwave-Upwelling",                "Upwelling Shortwave Radiation",         VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"reservoir",     "reservoir_area",                               "Reservoir-Area",                               "Reservoir Area",                        VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingPoint},
        {"reservoir",     "reservoir_capacity",                           "Reservoir-Capacity",                           "Reservoir Capacity",                    VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingPoint},
        {"reservoir",     "reservoir_dam",                                "Reservoir-Dam",                                "Reservoir Dam",                         VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"reservoir",     "reservoir_height",                             "Reservoir-Height",                             "Reservoir Height",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingPoint},
        {"reservoir",     "reservoir_irrigation_demand",                  "Reservoir-IrrigationDemand",                   "Reservoir Irrigation Water Demand",     VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"reservoir",     "reservoir_nonirrigation_demand",               "Reservoir-NonIrrigationDemand",                "Reservoir Non-Irrigation Water Demand", VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"reservoir",     "reservoir_subbasin",                           "Reservoir-Subbasin",                           "Reservoir Subbasin",                    VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"reservoir",     "reservoir_volume",                             "Reservoir-Volume",                             "Reservoir Volume",                      VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"reservoir",     "reservoir_small_coefficient",                  "Reservoir-Small-Coefficient",                  "Small-reservoir Coefficient",           VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"river",         "river_basin",                                  "River-Basin",                                  "River Basin",                           VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"river",         "river_mouth",                                  "River-Mouth",                                  "River Mouth",                           VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"river",         "river_cell_area",                              "River-CellArea",                               "River Cell Area",                       VDBcontinuous, VDBgrey,          VDBmax,           VDBsamplingPoint},
        {"river",         "river_cell_slope",                             "River-CellSlope",                              "River Cell Slope",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingPoint},
        {"river",         "river_confluence",                             "River-Confluence",                             "River Confluence",                      VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"river",         "river_corridor",                               "River-Corridor",                               "River Corridor",                        VDBline,       VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"river",         "river_distance_to_ocean",                      "River-DistToOcean",                            "Distance to Ocean",                     VDBcontinuous, VDBgrey,          VDBmax,           VDBsamplingPoint},
        {"river",         "river_network",                                "River-Network",                                "STNetwork",                             VDBnetwork,    VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"river",         "river_ocean_basin",                            "River-OceanBasin",                             "Ocean Basin",                           VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"river",         "river_streamline",                             "River-Streamline",                             "Streamline",                            VDBline,       VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"river",         "river_subbasin",                               "River-Subbasin",                               "Subbasin",                              VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"river",         "river_slope"         ,                         "River-Slope",                                  "River Slope",                           VDBcontinuous, VDBelevation,     VDBavg,           VDBsamplingPoint},
        {"river",         "river_slope-min",                              "River-Slope-Min",                              "Minimum River Slope",                   VDBcontinuous, VDBelevation,     VDBavg,           VDBsamplingPoint},
        {"river",         "river_slope-max",                              "River-Slope-Max",                              "Maximum River Slope",                   VDBcontinuous, VDBelevation,     VDBavg,           VDBsamplingPoint},
        {"river",         "river_temperature",                            "River-Temperature",                            "River Temperature",                     VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingPoint},
        {"river",         "river_temperature_gauge",                      "River-Temperature-Gauge",                      "River Temperature Gauge",               VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"river",         "river_temperature_subbasin",                   "River-Temperature-Subbasin",                   "River Temperature Subbasin",            VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"river",         "river_travel_time",                            "River-TravelTime",                             "Travel Time",                           VDBcontinuous, VDBgrey,          VDBmax,           VDBsamplingPoint},
        {"river",         "river_upstream_area",                          "River-UpstreamArea",                           "Upstream Area",                         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingPoint},
        {"socioeconomy",  "socioeconomy_energy_regions",                  "Socioeconomy-Energy-Regions",                  "Energy Regions",                        VDBdiscrete,   VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"socioeconomy",  "socioeconomy_population",                      "Socioeconomy-Population",                      "Population",                            VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"socioeconomy",  "socioeconomy_population_density",	          "Socioeconomy-Population-Density",              "Population Density",                    VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"socioeconomy",  "socioeconomy_population-rural",                "Socioeconomy-Population-Rural",                "Rural Population",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"socioeconomy",  "socioeconomy_population-urban",                "Socioeconomy-Population-Urban",                "Urban Population",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_available_water_capacity",                "Soil-AvailableWaterCapacity",                  "Avaible Water Capacity",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_bulk_density",                            "Soil-BulkDensity",                             "Bulk Density",                          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_depth_to_horizon",                        "Soil-DepthToRHorizon",                         "Depth to R Horizon",                    VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_depth_to_bedrock",                        "Soil-DepthToBedrock",                          "Depth to Bedrock",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_field_capacity",                          "Soil-FieldCapacity",                           "Field Capacity",                        VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_clay_percent_by_volume",                  "Soil-ClayPercentByVolume",                     "Percent Clay by Volume",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_clay_percent_by_mass",                    "Soil-ClayPercentByMass",                       "Percent Clay by Mass",                  VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_coarse_percent_by_volume",                "Soil-CoarsePercentByVolume",                   "Percent Coarse by Volume",              VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_coarse_percent_by_mass",                  "Soil-CoarsePercentByMass",                     "Percent Coarse by Mass",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_particle_density",                        "Soil-ParticleDensity",                         "Particle Density",                      VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_porosity",                                "Soil-Porosity",                                "Porosity",                              VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_probability_of_horizon",                  "Soil-ProbabilityOfRHorizon",                   "Probability of R Horizon",              VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_rooting_depth",                           "Soil-RootingDepth",                            "Rooting Depth",                         VDBcontinuous, VDBgrey,          VDBsum,           VDBsamplingZone},
        {"soil",          "soil_sand_percent_by_volume",                  "Soil-SandPercentByVolume",                     "Percent Sand by Volume",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_sand_percent_by_mass",                    "Soil-SandPercentByMass",                       "Percent Sand by Mass",                  VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_silt_percent_by_volume",                  "Soil-SiltPercentByVolume",                     "Percent Silt by Volume",                VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_silt_percent_by_mass",                    "Soil-SiltPercentByMass",                       "Percent Silt by Mass",                  VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"soil",          "soil_wilting_point",                           "Soil-WiltingPoint",                            "Wilting Point",                         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"station",       "station_precipitation",                        "Station-Precipitation",                        "Precipitation Gauge",                   VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"station",       "station_power_plant",                          "Station-PowerPlant",                           "Power Plant Catchment",                 VDBpoint,      VDBnotApplicable, VDBnotApplicable, VDBnotApplicable},
        {"topography",    "topography_bathymetry",                        "Topography-Bathymetry",                        "Bathymetry",                            VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"topography",    "topography_bathymetry-max",                    "Topography-Bathymetry-Max",                    "Maximum Depth",                         VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"topography",    "topography_bathymetry-min",                    "Topography-Bathymetry-Min",                    "Minimum Depth",                         VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"topography",    "topography_elevation",                         "Topography-Elevation",                         "Elevation",                             VDBcontinuous, VDBelevation,     VDBavg,           VDBsamplingZone},
        {"topography",    "topography_elevation-max",                     "Topography-Elevation-Max",                     "Maximum Elevation",                     VDBcontinuous, VDBelevation,     VDBavg,           VDBsamplingZone},
        {"topography",    "topography_elevation-min",                     "Topography-Elevation-Min",                     "Minimum Elevation",                     VDBcontinuous, VDBelevation,     VDBavg,           VDBsamplingZone},
        {"weather",       "weather_air_temperature",                      "Weather-AirTemperature",                       "Air Temperature",                       VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingZone},
        {"weather",       "weather_air_temperature-max",                  "Weather-AirTemperature-Max",                   "Maximum Air Temperature",               VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingZone},
        {"weather",       "weather_air_temperature-min",                  "Weather-AirTemperature-Min",                   "Minimum Air Temperature",               VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingZone},
        {"weather",       "weather_air_temperature_diurnal_range",        "Weather-AirTemperature-Range",                 "Diurnal Air Temperature Range",         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"weather",       "weather_air_pressure",                         "Weather-AirPressure",                          "Air Pressure",                          VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"weather",       "weather_cloud_cover",                          "Weather-CloudCover",                           "Cloud Cover",                           VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"weather",       "weather_heatindex",                            "Weather-Heatindex",                            "Heatindex",                             VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingZone},
        {"weather",       "weather_heatindex-max",                        "Weather-Heatindex-Max",                        "Maximum Heatindex",                     VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingZone},
        {"weather",       "weather_heatindex-min",                        "Weather-Heatindex-Min",                        "Minimum Heatindex",                     VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingZone},
        {"weather",       "weather_humidity_dewpoint",                    "Weather-Humidity-Dewpoint",                    "Dewpoint Temperature",                  VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingZone},
        {"weather",       "weather_humidity_relative",                    "Weather-Humidity-Relative",                    "Relative Humidity",                     VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"weather",       "weather_humidity_specific",                    "Weather-Humidity-Specific",                    "Specific Humidity",                     VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"weather",       "weather_humidity_vapor_pressure",              "Weather-Humidity-VaporPressure",               "Vapor Pressure",                        VDBcontinuous, VDBblue,          VDBavg,           VDBsamplingZone},
        {"weather",       "weather_humidity_vapor_pressure_deficit",      "Weather-Humidity-VaporPressure-Deficit",       "Vapor Pressure Deficit",                VDBcontinuous, VDBblue2red,      VDBavg,           VDBsamplingZone},
        {"weather",       "weather_wind_speed",                           "Weather-WindSpeed",                            "Wind Speed",                            VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"weather",       "weather_wind_speed_u10m",                      "Weather-WindSpeed-U10m",                       "Wind Speed U-direction at 10m",         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone},
        {"weather",       "weather_wind_speed_v10m",                      "Weather-WindSpeed-V10m",                       "Wind Speed V-direction at 10m",         VDBcontinuous, VDBgrey,          VDBavg,           VDBsamplingZone}};

#define VDBrecordNum2 sizeof( _VDBmasterTable2)/sizeof(VDBmasterTable_t)
#define VDBrecordNum3 sizeof( _VDBmasterTable3)/sizeof(VDBmasterTable_t)

const char *VDBrgName (VDBversion version, const char *cfName) {
    int i;

    switch (version) {
        case VDBversion2:
            for (i = 0; i < VDBrecordNum2; ++i) {
                if (strcmp (cfName, _VDBmasterTable2[i].CFname) == 0) return (_VDBmasterTable2[i].GHAASname);
            }
            break;
        case VDBversion3:
            for (i = 0; i < VDBrecordNum3; ++i) {
                if (strcmp (cfName, _VDBmasterTable3[i].CFname) == 0) return (_VDBmasterTable3[i].GHAASname);
            }
            break;
    }
    return ((const  char *) NULL);
}

const char *VDBlongName (VDBversion version, const char *cfName) {
    int i;

    switch (version) {
        case VDBversion2:
            for (i = 0; i < VDBrecordNum2; ++i) {
                if (strcmp (cfName, _VDBmasterTable2[i].CFname) == 0) return (_VDBmasterTable2[i].LongName);
            }
            break;
        case VDBversion3:
            for (i = 0; i < VDBrecordNum3; ++i) {
                if (strcmp (cfName, _VDBmasterTable3[i].CFname) == 0) return (_VDBmasterTable3[i].LongName);
            }
            break;
    }
    return ((const  char *) NULL);
}

const char *VDBcfName (VDBversion version, const char *rgName) {
    int i;

    switch (version) {
        case VDBversion2:
            for (i = 0; i < VDBrecordNum2; ++i) {
                if (strcmp (rgName, _VDBmasterTable2[i].GHAASname) == 0) return (_VDBmasterTable2[i].CFname);
            }
            break;
        case VDBversion3:
            for (i = 0; i < VDBrecordNum3; ++i) {
                if (strcmp (rgName, _VDBmasterTable3[i].GHAASname) == 0) return (_VDBmasterTable3[i].CFname);
            }
            break;
    }
    return ((const  char *) NULL);
}

const char *VDBdataType (VDBversion version, const char *cfName) {
    int i;

    switch (version) {
        case VDBversion2:
            for (i = 0; i < VDBrecordNum2; ++i) {
                if (strcmp (cfName, _VDBmasterTable2[i].CFname) == 0) return (_VDBmasterTable2[i].DataType);
            }
            break;
        case VDBversion3:
            for (i = 0; i < VDBrecordNum3; ++i) {
                if (strcmp (cfName, _VDBmasterTable3[i].CFname) == 0) return (_VDBmasterTable3[i].DataType);
            }
            break;
    }
    return ((const  char *) NULL);
}

const char *VDBshadset (VDBversion version, const char *cfName) {
    int i;

    switch (version) {
        case VDBversion2:
            for (i = 0; i < VDBrecordNum2; ++i) {
                if (strcmp (cfName, _VDBmasterTable2[i].CFname) == 0) return (_VDBmasterTable2[i].Shadeset);
            }
            break;
        case VDBversion3:
            for (i = 0; i < VDBrecordNum3; ++i) {
                if (strcmp (cfName, _VDBmasterTable3[i].CFname) == 0) return (_VDBmasterTable3[i].Shadeset);
            }
            break;
    }
    return ((const  char *) NULL);
}

const char *VDBsampling (VDBversion version, const char *cfName) {
    int i;

    switch (version) {
        case VDBversion2:
            for (i = 0; i < VDBrecordNum2; ++i) {
                if (strcmp (cfName, _VDBmasterTable2[i].CFname) == 0) return (_VDBmasterTable2[i].Sampling);
            }
            break;
        case VDBversion3:
            for (i = 0; i < VDBrecordNum3; ++i) {
                if (strcmp (cfName, _VDBmasterTable3[i].CFname) == 0) return (_VDBmasterTable3[i].Sampling);
            }
            break;
    }
    return ((const  char *) NULL);
}

const char *VDBaggregation (VDBversion version, const char *cfName) {
    int i;

    switch (version) {
        case VDBversion2:
            for (i = 0; i < VDBrecordNum2; ++i) {
                if (strcmp(cfName, _VDBmasterTable2[i].CFname) == 0) return (_VDBmasterTable2[i].Aggregation);
            }
            break;
        case VDBversion3:
            for (i = 0; i < VDBrecordNum3; ++i) {
                if (strcmp(cfName, _VDBmasterTable3[i].CFname) == 0) return (_VDBmasterTable3[i].Aggregation);
            }
            break;
    }
    return ((const  char *) NULL);
}
