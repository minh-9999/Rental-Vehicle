#include "../third_party/googletest-src/googletest/include/gtest/gtest.h"
// #include "gtest/gtest.h"

#include "../src/dataJson.h"
// #include "../third_party/json-src/single_include/nlohmann/json.hpp"
#include "../src/safe_localtime.h"
#include "../src/config.h"
#include "../third_party/fmt-src/include/fmt/core.h"

using json = nlohmann::json;

// Mock JSON file for testing
const string TEST_JSON_FILE = "test_vehicles.json";

// Test loading data from JSON
TEST(DataJsonTest, LoadFromJsonWithAvailable)
{
    unordered_map<string, Vehicles> ds;

    json mockData = {
        {"42h-94787",
         {{"licensePlate", "42h-94787"},
          {"rentalPrice", 475894.345},
          {"manufacturer", "toyota"},
          {"vehicleType", "suv"},
          {"yearOfManufacture", 2023},
          {"renterName", ""},
          {"rentalTime", "01-01-2024 12:00:00"},
          {"rentalStatus", "AVAILABLE"}}}};

    ofstream outFile(TEST_JSON_FILE);
    outFile << mockData.dump(4);
    outFile.close();

    loadFromJson(ds, TEST_JSON_FILE);

    ASSERT_EQ(ds.size(), 1);
    EXPECT_EQ(ds["42h-94787"].licensePlate, "42h-94787");
    EXPECT_DOUBLE_EQ(ds["42h-94787"].rentalPrice, 475894.345);
    EXPECT_EQ(ds["42h-94787"].manufacturer, "toyota");
    EXPECT_EQ(ds["42h-94787"].vehicleType, "suv");
    EXPECT_EQ(ds["42h-94787"].yearOfManufacture, 2023);
    EXPECT_EQ(ds["42h-94787"].renterName, "");

    // ✅ Use stringToTime() instead of strptime()
    // time_t expectedTime = stringToTime("01-01-2024 12:00:00");
    time_t expectedTime = convertStringToTime("01-01-2024 12:00:00");
    cout << "\n\t Expected: " << expectedTime << ", Actual: " << ds["42h-94787"].rentalTimestamp << endl;
    EXPECT_EQ(ds["42h-94787"].rentalTimestamp, expectedTime);

    EXPECT_EQ(ds["42h-94787"].rentalStatus, "AVAILABLE");
}

// Test saving data to JSON
TEST(DataJsonTest, SaveToJsonWithAvailable)
{
    unordered_map<string, Vehicles> ds;

    // time_t rentalTimestamp = stringToTime("01-01-2024 12:00:00");
    time_t rentalTimestamp = convertStringToTime("01-01-2024 12:00:00");
    fmt::print("\n --- Converted time: {}\n", rentalTimestamp);

    ds["42h-94787"] = Vehicles("42h-94787", "toyota", 2023, "suv", "AVAILABLE", 475894.345, rentalTimestamp, "");

    saveToJson(ds, TEST_JSON_FILE);

    ifstream inFile(TEST_JSON_FILE);
    json result;
    inFile >> result;
    fmt::print("\n  --- Loaded JSON: {}\n", result.dump(4)); // print data loaded from JSON
    inFile.close();

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result["42h-94787"]["licensePlate"], "42h-94787");
    EXPECT_DOUBLE_EQ(result["42h-94787"]["rentalPrice"], 475894.345);
    EXPECT_EQ(result["42h-94787"]["manufacturer"], "toyota");
    EXPECT_EQ(result["42h-94787"]["vehicleType"], "suv");
    EXPECT_EQ(result["42h-94787"]["yearOfManufacture"], 2023);
    EXPECT_EQ(result["42h-94787"]["renterName"], "");

    // Check timestamp format
    char buffer[20];
    tm lctm;
    SAFE_LOCALTIME(&rentalTimestamp, &lctm);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &lctm);

    // EXPECT_EQ(result["42h-94787"]["rentalTimestamp"], buffer);
    EXPECT_EQ(result["42h-94787"]["rentalTime"], buffer);
    EXPECT_EQ(result["42h-94787"]["rentalStatus"], "AVAILABLE");
}

// Test loading JSON with missing fields
TEST(DataJsonTest, LoadFromJson_MissingFields)
{
    unordered_map<string, Vehicles> ds;

    json mockData = {
        {"90f-31502", {{"licensePlate", "90f-31502"}, {"rentalPrice", 629412.865}, {"manufacturer", "innova"}, {"vehicleType", "suv"}}}}; // Missing yearOfManufacture, renterName, rentalTimestamp , rentalStatus

    ofstream outFile(TEST_JSON_FILE);
    outFile << mockData.dump(4);
    outFile.close();

    loadFromJson(ds, TEST_JSON_FILE);

    ASSERT_EQ(ds.size(), 1);
    EXPECT_EQ(ds["90f-31502"].licensePlate, "90f-31502");
    EXPECT_DOUBLE_EQ(ds["90f-31502"].rentalPrice, 629412.865);
    EXPECT_EQ(ds["90f-31502"].manufacturer, "innova");
    EXPECT_EQ(ds["90f-31502"].vehicleType, "suv");

    // Default values for missing fields
    EXPECT_EQ(ds["90f-31502"].yearOfManufacture, 0);
    EXPECT_EQ(ds["90f-31502"].renterName, "");
    EXPECT_EQ(ds["90f-31502"].rentalTimestamp, 0);
    EXPECT_EQ(ds["90f-31502"].rentalStatus, "AVAILABLE"); // Assuming default is AVAILABLE if missing
}

// Cleanup after tests
class CleanupTest : public ::testing::Test
{
protected:
    void TearDown() override
    {
        if (filesystem::exists(TEST_JSON_FILE))
        {
            filesystem::remove(TEST_JSON_FILE);
        }
    }
};
