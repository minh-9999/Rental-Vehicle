#include "../third_party/googletest-src/googletest/include/gtest/gtest.h"
// #include "gtest/gtest.h"
#include "../src/Vehicles.h"
#include "../src/config.h"
#include "../third_party/fmt-src/include/fmt/core.h"

struct TestVehicles
{
    static Vehicles availableCar()
    {
        return Vehicles("XYZ-123", "Toyota", 2020, "Camry", "AVAILABLE", 100.0, 0, "");
    }

    static Vehicles rentedCar()
    {
        return Vehicles("51T-63017", "GM", 2021, "SUV", "RENTED", 628194.85, time(nullptr), "John Doe");
    }

    static Vehicles carUnderMaintenance()
    {
        return Vehicles("AB-1111", "Ford", 2022, "Explorer", "UNDER MAINTENANCE", 150.0, 0, "");
    }

    static Vehicles carWithExpiredRental()
    {
        return Vehicles("XX-9999", "Honda", 2019, "Civic", "RENTED", 200.0, time(nullptr) - 86400, "Alice");
    }

    static Vehicles carWithCustomStatus(const string &status)
    {
        return Vehicles("CUSTOM-123", "BMW", 2023, "X5", status, 200.0, 0, "");
    }
};

TEST(VehicleTest, CreateVehicle)
{
    Vehicles car = TestVehicles::availableCar();

    EXPECT_EQ(car.getMake(), "Toyota");
    EXPECT_EQ(car.getModel(), "Camry");
    EXPECT_EQ(car.getYear(), 2020);
    EXPECT_EQ(car.getLicensePlate(), "XYZ-123");
    EXPECT_NEAR(car.getRentalPrice(), 100.0, 1e-6); // ✅ More robust floating-point comparison
    EXPECT_EQ(car.getVehicleType(), "Camry");       // ✅ Ensure vehicle type is set correctly
    EXPECT_EQ(car.getRentalStatus(), "AVAILABLE");  // ✅ Ensure default status is correct
    EXPECT_EQ(car.getRenterName(), "");             // ✅ Ensure no renter assigned initially
    EXPECT_FALSE(car.isRented());                   // ✅ Initially should not be rented
}

TEST(VehicleTest, RentAndReturn)
{
    // Vehicles car = TestVehicles::rentedCar();
    Vehicles car = TestVehicles::availableCar();
    // Check initial state
    EXPECT_FALSE(car.isRented());
    EXPECT_EQ(car.renterName, "");

    // ✅ Use a function instead of modifying rentalStatus directly
    car.rentVehicle("Thien Anh");
    EXPECT_TRUE(car.isRented());
    EXPECT_EQ(car.renterName, "Thien Anh"); // Verify renter name

    // Return vehicle
    car.returnVehicle();
    EXPECT_FALSE(car.isRented());
    EXPECT_EQ(car.rentalStatus, "AVAILABLE"); // ✅ Status should be reset
    EXPECT_EQ(car.renterName, "");            // ✅ Renter should be cleared
}

// TEST(DisplayTest, ShowVehicleInfo_1)
// {
//     Vehicles v("42H-12345", "Toyota", 2023, "SUV", "AVAILABLE", 500.0, 0, "");

//     /*
//     // Redirect stdout to capture printed output
//     stringstream buffer;
//     auto old = cout.rdbuf(buffer.rdbuf());

//     fmt::print("\n[DEBUG] Vehicle Data: license: {}, manufacturer: {}, year: {}, type: {}, status: {}\n",
//                     v.licensePlate, v.manufacturer, v.yearOfManufacture, v.vehicleType, v.rentalStatus);

//     // Call display function
//     displayVehicleInfo(v);

//     // Verify all necessary details
//     string output = buffer.str();

//     // Restore stdout
//     cout.rdbuf(old);

//     // ------------------------------- for debug
//     if (output.empty())
//         fmt::print("No output captured. \n");
//     else
//         fmt::print("Captured output: {} \n", output);
//     // ------------------------------- end for debug

//     */

//     // cout << "\n   Debug: Displaying vehicle info \n\n";

//     /*
//         string output = utils::capture_stdout([&]()
//                                               { displayVehicleInfo(v); });
//     */

//     /*
//         string output = utils::capture_stdout_with_fmt([&]()
//                                                        { displayVehicleInfo(v); });
//     */

//     string output = utils::capture_stdout([&]()
//                                           {
//                                             cout << "Debugging output for vehicle: " << v.licensePlate << endl;
//                                                 displayVehicle(v); });

//     // Remove unnecessary whitespace/newlines
//     // output = trim(output);

//     /*
//         if (output.empty())
//             fmt::print("\n\t No output captured. \n\n");
//         else
//             fmt::print("\n\t  [Captured]\n{} \n\n", output);
//     */

//     if (output.empty())
//         cout << "\n  No output captured!" << endl;
//     else
//         cout << "\n  Captured output:\n"
//              << output << endl;

//     EXPECT_FALSE(output.empty()); // Ensure some output was captured

//     // Verify Manufacturer (Toyota or TOYOTA)
//     EXPECT_TRUE(output.find("Toyota") != string::npos || output.find("TOYOTA") != string::npos);

//     // Verify License Plate (42H-12345)
//     EXPECT_NE(output.find("42H-12345"), string::npos);

//     // Verify Vehicle Type (SUV or suv)
//     EXPECT_TRUE(output.find("Suv") != string::npos || output.find("SUV") != string::npos);

//     // Verify Year of Manufacture (2023)
//     EXPECT_NE(output.find("2023"), string::npos);

//     // Verify Rental Price (500.0)
//     EXPECT_NE(output.find("500.0"), string::npos);

//     // Verify Rental Status (Available or AVAILABLE)
//     EXPECT_TRUE(output.find("Available") != string::npos || output.find("AVAILABLE") != string::npos);

//     // Ensure "No renter" message is displayed (since the vehicle is available)
//     EXPECT_EQ(output.find("No renter"), string::npos); // Ensure no renter is displayed
// }

// TEST(DisplayTest, ShowVehicleInfo_2)
// {
//     Vehicles v("42H-12345", "Toyota", 2023, "SUV", "AVAILABLE", 500.0, 0, "");

//     // Capture the output from the display function
//     CAPTURE_STDOUT(output, displayVehicleInfo(v));

//     EXPECT_FALSE(output.empty()); // Ensure some output was captured

//     // Verify Manufacturer (Toyota or TOYOTA)
//     EXPECT_TRUE(checkPattern_1(output, "Toyota"));

//     // Verify License Plate (42H-12345)
//     EXPECT_TRUE(checkPattern_1(output, "42H-12345"));

//     // Verify Vehicle Type (SUV or suv)
//     EXPECT_TRUE(checkPattern_1(output, "Suv"));

//     // Verify Year of Manufacture (2023)
//     EXPECT_TRUE(checkPattern_1(output, "2023"));

//     // Verify Rental Price (500.0)
//     EXPECT_TRUE(checkPattern_1(output, "500.0"));

//     // Verify Rental Status (Available or AVAILABLE)
//     EXPECT_TRUE(checkPattern_1(output, "Available"));

//     // Ensure "No renter" message is displayed (since the vehicle is available)
//     EXPECT_FALSE(checkPattern_1(output, "No renter"));
// }
