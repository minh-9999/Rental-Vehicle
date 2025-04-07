#include "Vehicles.h"
#include "dataJson.h"
#include "RentalTransaction.h"

int main()
{
    unordered_map<string, Vehicles> vehicleList;
    vector<RentalTransaction> rentalTransactions;

    // loadList(vehicleList); // Load data on startup
    loadFromJson(vehicleList, jsondata);

    unsigned choice;

    do
    {
        fmt::print("\n \t\t ---- Functions ---- \n\n");
        fmt::print(" 🚗  1. Add a vehicle \n");
        fmt::print(" 📜  2. Display vehicle list \n");
        fmt::print(" 🔍  3. Search for a vehicle \n");
        fmt::print(" 🗑️   4. Delete a vehicle \n");
        fmt::print(" 🚙  5. Rent a vehicle \n");
        fmt::print(" 🔄  6. Return a vehicle \n");
        fmt::print(" 📄  7. Show rented vehicles \n");
        fmt::print(" 📝  8. Check transactions \n");
        fmt::print(" 💾  9. Save and exit \n\n\t");

        cin >> choice;

        switch (choice)
        {
        case 1:
            addVehicle(vehicleList);
            break;

        case 2:
            displayListVehicleInfo(vehicleList);
            break;

        case 3:
            findVehicle(vehicleList);
            break;

        case 4:
            deleteVehicle(vehicleList);
            break;

        case 5:
            checkRentVehicle(vehicleList, rentalTransactions);
            break;

        case 6:
            checkReturnVehicle(vehicleList, rentalTransactions);
            break;

        case 7:
            showRentedVehicles(jsondata);
            break;

        case 8:
            checkTransaction(rentalTransactions, vehicleList);
            break;

        case 9:
            // saveList(vehicleList);
            saveToJson(vehicleList, jsondata); // Save the list to JSON before exiting
            fmt::print(" Exiting the program \n");
            break;

        default:
            fmt::print(" Invalid choice, please try again! \n");
        }
    } while (choice != 9);
}

/*

clang++20 xe.cc -I"C:/Program Files (x86)/FMT/include" -L"C:/Program Files (x86)/FMT/lib" -lfmt

clang++20 xe.cc -I"C:/Program Files (x86)/FMT/include" -L"C:/Program Files (x86)/FMT/lib" -lfmt -DNOMINMAX

*/
