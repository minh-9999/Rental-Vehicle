#include "../third_party/googletest-src/googletest/include/gtest/gtest.h"
// #include "gtest/gtest.h"
#include "../src/Vehicles.h"
#include "../src/RentalTransaction.h"
// #include "../src/dataJson.h"

TEST(IntegrationTest, RentAndSaveToJson)
{
    unordered_map<string, Vehicles> ds;
    vector<RentalTransaction> transactions;

    // ✅ Create vehicle
    ds["42H-12345"] = Vehicles("42H-12345", "Toyota", 2023, "SUV", "AVAILABLE", 500.0, 0, "");

    // ✅ Rent vehicle
    checkRentVehicle(ds, transactions);

    // ✅ Check status
    EXPECT_TRUE(ds["42H-12345"].isRented());

    // ✅ Save to JSON
    saveList(ds);

    // ✅ Load from JSON
    unordered_map<string, Vehicles> ds_loaded;
    loadList(ds_loaded);

    // ✅ Verify data after saving
    EXPECT_TRUE(ds_loaded["42H-12345"].isRented());
}
