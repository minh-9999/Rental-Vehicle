#include "../third_party/googletest-src/googletest/include/gtest/gtest.h"
// #include "gtest/gtest.h"
#include "../src/Vehicles.h"
#include "../src/RentalTransaction.cc"
#include "../third_party/fmt-src/include/fmt/core.h"

TEST(IntegrationTest, RentAndSaveToJson)
{
    unordered_map<string, Vehicles> ds;
    vector<RentalTransaction> transactions;

    // ✅ Create vehicle
    ds["42H-12345"] = Vehicles("42H-12345", "Toyota", 2023, "SUV", "AVAILABLE", 500.0, 0, "");

    // ✅ Rent vehicle (no user input)
    bool ok = tryRentVehicle(ds, transactions, "khach123", "42H-12345");
    ASSERT_TRUE(ok);

    // ✅ Check in-memory status
    EXPECT_TRUE(ds["42H-12345"].isRented());
    EXPECT_EQ(ds["42H-12345"].renterName, "khach123");

    // saveList(ds);
    // ✅ Save to global jsondata string directly (no file I/O)
    saveToJson(ds, jsondata);

    // 👇 Optionally print for debug
    fmt::print("jsondata = {}\n", jsondata);

    // ✅ Load from jsondata directly
    unordered_map<string, Vehicles> ds_loaded;
    loadFromJson(ds_loaded, jsondata);
    // loadList(ds_loaded);

    // ✅ Check again after reloading
    ASSERT_TRUE(ds_loaded.contains("42H-12345"));
    EXPECT_TRUE(ds_loaded["42H-12345"].isRented());
    EXPECT_EQ(ds_loaded["42H-12345"].renterName, "khach123");
}

TEST(IntegrationTest, TryRentVehicleSuccess)
{
    unordered_map<string, Vehicles> ds;
    vector<RentalTransaction> txs;

    ds["42H-12345"] = Vehicles("42H-12345", "Toyota", 2023, "SUV", "AVAILABLE", 500, 0, "");

    bool result = tryRentVehicle(ds, txs, "khach123", "42H-12345");

    EXPECT_TRUE(result);
    EXPECT_TRUE(ds["42H-12345"].isRented());
    EXPECT_EQ(ds["42H-12345"].renterName, "khach123");
    EXPECT_FALSE(txs.empty());
}
