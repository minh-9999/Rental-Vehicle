
#include "RentalTransaction.h"
#include "config.h"
#include "dataJson.h"
#include "safe_localtime.h"

#ifdef _WIN32
#include <windows.h>
#endif

// string jsondata = "vehicle.json";
string LOG = "rental_log.txt";

string getTimeZoneName()
{
#ifdef _WIN32
    TIME_ZONE_INFORMATION tzInfo;
    GetTimeZoneInformation(&tzInfo);

    char name[64];
    size_t converted;
    wcstombs_s(&converted, name, sizeof(name), tzInfo.StandardName, _TRUNCATE);
    return std::string(name);
#else
    time_t now = time(nullptr);
    struct tm *localTime = localtime(&now);
    return (localTime && localTime->tm_zone) ? std::string(localTime->tm_zone) : "Unknown";
#endif
}

string getUTCOffset()
{
#ifdef _WIN32
    TIME_ZONE_INFORMATION tzInfo;
    GetTimeZoneInformation(&tzInfo);
    int bias = -tzInfo.Bias / 60; // Độ lệch so với UTC (phút -> giờ)

    return string("UTC") + (bias >= 0 ? "+" : "") + to_string(bias);

#else
    time_t now = time(nullptr);
    struct tm local_tm;
    localtime_r(&now, &local_tm);

    // offset in seconds
    int offset = local_tm.tm_gmtoff;
    int hours = offset / 3600;
    int minutes = abs((offset % 3600) / 60);

    ostringstream oss;
    oss << "UTC" << (hours >= 0 ? "+" : "-")
        << setw(2) << setfill('0') << abs(hours)
        << ":" << setw(2) << setfill('0') << minutes;
    return oss.str();
#endif
}

void writeLog(const RentalTransaction &tx)
{
    ofstream file(LOG, ios::app);
    if (!file)
    {
        fmt::print("❌ Error: Unable to open file {} \n", LOG);
        return;
    }

    // tm *tm_info = localtime(&tx.timestamp);
    tm tm_info;
    char time_str[40];

    /*
    if (strftime(time_str, sizeof(time_str), "%d-%m-%Y %H:%M:%S", tm_info) == 0)
        strcpy(time_str, "INVALID_TIME");
    */

    if (SAFE_LOCALTIME(&tx.timestamp, &tm_info) ||
        strftime(time_str, sizeof(time_str), "%d-%m-%Y %H:%M:%S", &tm_info) == 0)
    {
        SAFE_STRCPY(time_str, sizeof(time_str), "INVALID_TIME");
    }

    // string tz_name = getTimeZoneName();
    string utc_offset = getUTCOffset();

    file << tx.transaction_id << ", " << tx.customer_id << ", " << tx.vehicle_id << ", "
         << time_str << " " << utc_offset << ", " << tx.status << endl;

    file.flush();
    file.close();
}

// ---------------------------------------------------------------------

void checkTransaction(const vector<RentalTransaction> &transactions, unordered_map<string, Vehicles> &ds)
{
    loadFromJson(ds, jsondata); // 🔥 Load vehicle data from JSON

    for (const auto &tx : transactions)
    {
        auto found = ds.find(tx.vehicle_id);

        if (found == ds.end())
        {
            fmt::print("\n❌ Error: Vehicle {} does not exist in the system! \n", tx.vehicle_id);
            continue;
        }

        Vehicles &vehicle = found->second;

        if (tx.status == "RENTED")
        {
            // if (vehicle.rentalStatus == "RENTED" && vehicle.renterName == tx.customer_id)
            if (vehicle.isRented() && vehicle.renterName == tx.customer_id)
                fmt::print("\n✅ Confirmation: Vehicle {} has been rented by {}.\n", tx.vehicle_id, tx.customer_id);
            else
                fmt::print("\n⚠️ Transaction Error: Vehicle {} rental status does not match the system!\n", tx.vehicle_id);
        }

        else if (tx.status == "RETURNED")
        {
            // if (vehicle.rentalStatus == "RETURNED" && vehicle.renterName.empty())
            if (!vehicle.isRented() && vehicle.renterName.empty()) // 🔥 Adjusted validation condition
                fmt::print("\n✅ Confirmation: Vehicle {} has been returned successfully!\n", tx.vehicle_id);
            else
                fmt::print("\n⚠️ Transaction Error: Vehicle {} returned, but status is invalid!\n", tx.vehicle_id);
        }
    }
}

//  for test
bool tryRentVehicle(unordered_map<string, Vehicles> &ds,
                    vector<RentalTransaction> &transactions,
                    const string &customer_id,
                    const string &vehicle_id)
{
    if (customer_id.empty() || vehicle_id.empty())
        return false;

    auto found = ds.find(vehicle_id);
    if (found == ds.end())
        return false;

    Vehicles &vehicle = found->second;
    if (vehicle.rentalStatus == "RENTED")
        return false;

    vehicle.rentalStatus = "RENTED";
    vehicle.renterName = customer_id;
    vehicle.rentalTimestamp = time(nullptr);

    RentalTransaction tx = {
        "TX" + to_string(vehicle.rentalTimestamp),
        customer_id,
        vehicle_id,
        vehicle.rentalTimestamp,
        "RENTED"};

    transactions.push_back(tx);
    return true;
}
// end

void checkRentVehicle(unordered_map<string, Vehicles> &ds, vector<RentalTransaction> &transactions)
{
    string customer_id, vehicle_id;

    fmt::print("\n Enter customer ID for rental: ");
    cin >> customer_id;
    fmt::print("\n Enter vehicle license plate to rent: ");
    cin >> vehicle_id;

    /*
        if (customer_id.empty() || vehicle_id.empty())
        {
            fmt::print("\n ❌ Customer ID or vehicle license plate cannot be empty! \n");
            return;
        }

        auto found = ds.find(vehicle_id);
        if (found == ds.end())
        {
            fmt::print("\n ❌ Vehicle with license plate {} not found! \n", vehicle_id);
            return;
        }

        Vehicles &vehicle = found->second; // Retrieve vehicle from the list

        if (vehicle.rentalStatus == "RENTED")
        {
            fmt::print("\n ❌ Vehicle {} is already rented and cannot be rented again! \n", vehicle_id);
            return;
        }

        // Update rental status
        vehicle.rentalStatus = "RENTED";
        vehicle.renterName = customer_id;
        vehicle.rentalTimestamp = time(nullptr); // Store rental timestamp

        // Log rental transaction
        RentalTransaction tx = {
            "TX" + to_string(vehicle.rentalTimestamp), // Transaction ID
            vehicle.renterName,
            vehicle_id,
            vehicle.rentalTimestamp,
            "RENTED"};

        transactions.push_back(tx);

        writeLog(tx);

        // Save data to JSON, check for errors
        // if (!saveToJson(ds, jsondata))
        // {
        //     fmt::print("\n ⚠️ Error saving data to JSON file! \n");
        //     return;
        // }

        saveToJson(ds, jsondata);
        fmt::print("\n ✅ Vehicle {} has been rented by {}. \n", vehicle_id, customer_id);
    */

    if (tryRentVehicle(ds, transactions, customer_id, vehicle_id))
    {
        writeLog(transactions.back());
        saveToJson(ds, jsondata);
        fmt::print("\n ✅ Vehicle {} has been rented by {}. \n", vehicle_id, customer_id);
    }
    else
    {
        fmt::print("\n ❌ Failed to rent vehicle. Check if it's already rented or not found.\n");
    }
}

void checkReturnVehicle(unordered_map<string, Vehicles> &ds, vector<RentalTransaction> &transactions)
{
    string customer_id, vehicle_id;

    fmt::print("\n Enter customer ID for return: ");
    cin >> customer_id;
    fmt::print("\n Enter vehicle license plate to return: ");
    cin >> vehicle_id;

    auto found = ds.find(vehicle_id);

    if (found == ds.end())
    {
        fmt::print("\n ❌ Vehicle with license plate {} not found! \n", vehicle_id);
        return;
    }

    Vehicles &vehicle = found->second; // Retrieve vehicle from the list

    // if (vehicle.rentalStatus == "RETURNED")
    if (!vehicle.isRented()) // 🔥 Use `isRented()` function instead of direct comparison
    {
        fmt::print("\n ❌ Vehicle {} is not currently rented, cannot be returned! \n", vehicle_id);
        return;
    }

    if (vehicle.renterName != customer_id)
    {
        fmt::print("\n ❌ Customer {} is not the renter of vehicle {}, cannot return it!\n", customer_id, vehicle_id);
        return;
    }

    // Update vehicle status after return
    // vehicle.rentalStatus = "RETURNED";
    vehicle.returnVehicle(); // 🔥 Call returnVehicle() to reset status to "AVAILABLE"
    vehicle.renterName = "";
    vehicle.rentalTimestamp = 0; // Reset rental timestamp

    // Get current timestamp
    time_t t_stamp = time(nullptr);

    // Log return transaction
    RentalTransaction tx = {
        "RX" + to_string(t_stamp), // Transaction ID
        customer_id,
        vehicle_id,
        t_stamp,
        "RETURNED"};

    transactions.push_back(tx);
    writeLog(tx);
    saveToJson(ds, jsondata);

    fmt::print("\n ✅ Vehicle {} has been returned by {}! \n", vehicle_id, customer_id);
}

void showRentedVehicles(const string &jsonFile)
{
    unordered_map<string, Vehicles> ds;
    loadFromJson(ds, jsonFile); // 🔥 Load from JSON file
    fmt::print("\n\t --- List of currently rented vehicles --- \n");
    bool hasRentedVehicle = false;

    for (const auto &[licensePlate, vehicle] : ds)
    {
        // if (vehicle.rentalStatus == "RENTED")
        if (vehicle.isRented()) // 🔥 Use `isRented()` function
        {
            displayVehicleInfo(vehicle); // 🔥 Reuse display function
            hasRentedVehicle = true;
        }
    }

    if (!hasRentedVehicle)
        fmt::print("\n No vehicles are currently rented.\n");
}