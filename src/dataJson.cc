#include "dataJson.h"
#include "safe_localtime.h"

// #include "../third_party/json-src/single_include/nlohmann/json.hpp"
#include "config.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

void saveToJson(const unordered_map<string, Vehicles> &ds, const string &filename)
{
    // json j;
    ordered_json j;
    for (const auto &[key, vehicle] : ds)
    {
        // json vehicleData = {
        ordered_json vehicleData = {
            {"licensePlate", vehicle.licensePlate},
            {"manufacturer", vehicle.manufacturer},
            {"yearOfManufacture", vehicle.yearOfManufacture},
            {"vehicleType", vehicle.vehicleType},
            {"rentalStatus", vehicle.rentalStatus},
            {"rentalPrice", vehicle.rentalPrice},
            {"renterName", vehicle.renterName}};

        if (vehicle.rentalTimestamp > 0)
        {
            char buf[20];
            tm lctm;
            SAFE_LOCALTIME(&vehicle.rentalTimestamp, &lctm);
            strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M:%S", &lctm);
            vehicleData["rentalTime"] = buf;
        }

        j[key] = vehicleData;
    }

    ofstream file(filename, ios::trunc);
    if (!file)
    {
        fmt::print("\n ❌ Error: Unable to open file {} for writing!\n", filename);
        // cout << "\n ❌ Error: Unable to open file " << filename << " for writing!\n";
        return;
    }
    file << j.dump(4);
}

void loadFromJson(unordered_map<string, Vehicles> &ds, const string &filename)
{
    ifstream file(filename);
    if (!file)
    {
        fmt::print("\n 🟡 Warning: File \"{}\" not found! Starting with an empty list.\n", filename);
        // cout << "\n 🟡 Warning: File \"" << filename << "\" not found! Starting with an empty list.\n";
        return;
    }

    // json j;
    ordered_json j; // ✅ use ordered_json instead of json

    try
    {
        if (file.peek() == ifstream::traits_type::eof())
        {
            fmt::print("\n 🟡 Notice: JSON file {} is empty, no vehicle data found!\n", filename);
            // cout << "\n 🟡 Notice: JSON file " << filename << " is empty, no vehicle data found!\n";
            return;
        }
        file >> j;
        if (!j.is_object())
        {
            throw runtime_error("JSON file is not a valid object!");
        }
    }
    catch (const exception &e)
    {
        fmt::print("\n ❌ Error reading JSON {}: {}\n", filename, e.what());
        // cout << "\n ❌ Error reading JSON " << filename << ": " << e.what() << "\n";
        return;
    }

    ds.clear();
    for (auto &[key, value] : j.items())
    {
        try
        {
            if (!value.is_object())
                throw runtime_error(fmt::format("Vehicle {} data is not an object!", key));
            // throw runtime_error("Vehicle " + key + " data is not an object!");

            // Handle rentalTime field (optional)
            std::optional<time_t> tt = nullopt;
            if (value.contains("rentalTime") && value["rentalTime"].is_string())
            {
                const string &timeStr = value["rentalTime"].get<string>();
                if (!timeStr.empty())
                {
                    tt = stringToTime(timeStr);
                    // tt = convertStringToTime(timeStr);
                }
            }

            // Handle rentalStatus with default "AVAILABLE"
            string status = value.value("status", "AVAILABLE");
            if (status != "RENTED" && status != "AVAILABLE")
            {
                fmt::print("\n ⚠️ Error: Vehicle {} has an invalid status! Defaulting to AVAILABLE.\n", key);
                // cout << "\n  Error: Vehicle" << key << " has an invalid status! Defaulting to AVAILABLE.\n";
                status = "AVAILABLE";
            }

            // Create the vehicle entry
            ds[key] = Vehicles{
                value.value("licensePlate", ""),
                value.value("manufacturer", ""),
                value.value("yearOfManufacture", 0U),
                value.value("vehicleType", ""),
                status,
                value.value("rentalPrice", 0.0),
                tt.value_or(0),
                value.value("renterName", "")};
        }
        catch (const exception &e)
        {
            fmt::print("\n ⚠️ Error: Vehicle {} has invalid data! Skipping... ({})\n", key, e.what());
            // cout << "\n ⚠️ Error: Vehicle " << key << " has invalid data! Skipping... \n";
        }
    }
}

/*
void loadFromJson(unordered_map<string, Vehicles> &ds, const string &filename)
{
    ifstream file(filename);
    if (!file)
    {
        std::cout << "\n 🟡 Warning: File \"" << filename << "\" not found! Starting with an empty list.\n";
        return;
    }

    ordered_json j;

    try
    {
        if (file.peek() == ifstream::traits_type::eof())
        {
            std::cout << "\n 🟡 Notice: JSON file " << filename << " is empty, no vehicle data found!\n";
            return;
        }
        file >> j;
        if (!j.is_object())
        {
            throw runtime_error("JSON file is not a valid object!");
        }
    }
    catch (const exception &e)
    {
        std::cout << "\n ❌ Error reading JSON " << filename << ": " << e.what() << "\n";
        return;
    }

    ds.clear();
    for (auto &[key, value] : j.items())
    {
        try
        {
            if (!value.is_object())
                throw runtime_error(fmt::format("Vehicle {} data is not an object!", key));

            // Handle rentalTimestamp field (expected epoch time)
            time_t tt = 0;
            if (value.contains("rentalTimestamp") && value["rentalTimestamp"].is_number())
            {
                tt = value["rentalTimestamp"].get<time_t>();
            }
            // If rentalTimestamp not found, try rentalTime (string to time conversion)
            else if (value.contains("rentalTime") && value["rentalTime"].is_string())
            {
                const string &timeStr = value["rentalTime"].get<string>();
                // tt = stringToTime(timeStr);
                tt = convertStringToTime(timeStr);
            }

            // Handle rentalStatus with default "AVAILABLE"
            string status = value.value("status", "AVAILABLE");
            if (status != "RENTED" && status != "AVAILABLE")
            {
                std::cout << "\n ⚠️ Error: Vehicle " << key << " has an invalid status! Defaulting to AVAILABLE.\n";
                status = "AVAILABLE";
            }

            // Create the vehicle entry
            ds[key] = Vehicles{
                value.value("licensePlate", ""),
                value.value("manufacturer", ""),
                value.value("yearOfManufacture", 0U),
                value.value("vehicleType", ""),
                status,
                value.value("rentalPrice", 0.0),
                tt,
                value.value("renterName", "")};
        }
        catch (const exception &e)
        {
            std::cout << "\n ⚠️ Error: Vehicle " << key << " has invalid data! Skipping...\n";
        }
    }
}
*/
