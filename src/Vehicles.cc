
// #define _CRT_SECURE_NO_WARNINGS

#include "Vehicles.h"
#include "dataJson.h"
#include "safe_localtime.h"
#include "config.h"
#include "../third_party/fmt-src/include/fmt/core.h"

// #define LOG "rental_log.txt"

string jsondata = "vehicle.json";

// 🔥 Added constructor to match initialization in ds[key]
Vehicles::Vehicles(const string &plate, const string &brand, unsigned int year,
                   const string &type, const string &status, double price,
                   time_t rentalTime, const string &renter)
    : licensePlate(plate), manufacturer(brand), yearOfManufacture(year),
      vehicleType(type), rentalStatus(status), rentalPrice(price),
      rentalTimestamp(rentalTime), renterName(renter) {}

// Getter methods
string Vehicles::getMake() const { return manufacturer; }

string Vehicles::getModel() const { return vehicleType; }

unsigned Vehicles::getYear() const { return yearOfManufacture; }

string Vehicles::getLicensePlate() const { return licensePlate; }

double Vehicles::getRentalPrice() const { return rentalPrice; }

string Vehicles::getVehicleType() const
{
    return vehicleType;
}

string Vehicles::getRentalStatus() const
{
    return rentalStatus;
}

string Vehicles::getRenterName() const
{
    return renterName;
}

// Rental status methods
bool Vehicles::isRented() const { return rentalStatus == "RENTED"; }

void Vehicles::returnVehicle()
{
    rentalStatus = "AVAILABLE";
    renterName = "";
} // Reset renter info when returned

// Rent vehicle
void Vehicles::rentVehicle(const string &renter)
{
    rentalStatus = "RENTED"; // Set status to "RENTED"
    renterName = renter;     // Set the renter's name
}

void displayVehicle(const Vehicles &x)
{
    cout << "\n  License Plate: " << x.licensePlate << "\n  Manufacturer: " << x.manufacturer
         << "\n  Year: " << x.yearOfManufacture << "\n  Rental Price: " << formatCurrency(x.rentalPrice)
         << " VNĐ \n  Type: " << x.vehicleType;

    string rentalStatus = (x.rentalStatus == "RENTED") ? "  Rented" : "  Available";
    cout << "\n  Status: " << rentalStatus;

    if (x.rentalStatus == "RENTED" && !x.renterName.empty())
    {
        char time_str[40];
        tm lctm;
        SAFE_LOCALTIME(&x.rentalTimestamp, &lctm);
        strftime(time_str, sizeof(time_str), "%d-%m-%Y %H:%M:%S", &lctm);

        cout << "  Renter: " << x.renterName << " | Rental Time: " << time_str << "\n";
    }
    else
        cout << "  No renter.\n";

    cout << "\n   " << string(100, '-') << " \n";
}

void displayVehicleInfo(const Vehicles &x)
{
    fmt::print("\n 🏷  License Plate: {}\n 🚗 Manufacturer: {}\n 📅 Year: {}\n 💰 Rental Price: {} VNĐ \n 🚙 Type: {}",
               x.licensePlate, x.manufacturer, x.yearOfManufacture, formatCurrency(x.rentalPrice), x.vehicleType);

    string rentalStatus = (x.rentalStatus == "RENTED") ? "🚩 Rented" : "✅ Available";
    fmt::print("\n 📌 Status: {}", rentalStatus);

    if (x.rentalStatus == "RENTED" && !x.renterName.empty())
    {
        char time_str[40];
        /*
          tm *lctm = localtime(&x.rentalTimestamp);
          strftime(time_str, sizeof(time_str), "%d-%m-%Y %H:%M:%S", lctm);
        */

        tm lctm;
        SAFE_LOCALTIME(&x.rentalTimestamp, &lctm);
        strftime(time_str, sizeof(time_str), "%d-%m-%Y %H:%M:%S", &lctm);

        fmt::print(" 👤 Renter: {} | Rental Time: {}\n", x.renterName, time_str);
    }

    else
        fmt::print(" 👤 No renter.\n");

    cout << "\n\n\t\t\t  [DEBUG] inside displayVehicleInfo \n\n";

    fmt::print("\n   {} \n", string(100, '-'));
}

void displayListVehicleInfo(const unordered_map<string, Vehicles> &list)
{
    if (list.empty())
    {
        fmt::print("\n\t 📭 No vehicles available! \n");
        return;
    }

    fmt::print("\n\t 📭 Available vehicles: \n");

    for (const auto &[_, x] : list)
        displayVehicleInfo(x);
}

void addVehicle(unordered_map<string, Vehicles> &list)
{
    string licensePlate, manufacturer, vehicleType;
    unsigned yearOfManufacture;
    double rentalPrice;

    cin.ignore(10000, '\n'); // Clear buffer before input
    fmt::print("\n Enter license plate: ");
    getline(cin, licensePlate);

    fmt::print("\n Enter manufacturer: ");
    getline(cin, manufacturer);

    while (true)
    {
        fmt::print("\n Year of manufacture: ");
        if (cin >> yearOfManufacture && yearOfManufacture > 1960 && yearOfManufacture < 2024)
        {
            cin.ignore(10000, '\n');
            break;
        }
        fmt::print("❌ Error: Invalid year! Please re-enter.\n");
        cin.clear(); // Clear invalid input flag
        cin.ignore(10000, '\n');
    }

    fmt::print("\n Enter vehicle type (e.g., Sedan, SUV, Hatchback...): ");
    getline(cin, vehicleType);

    while (true)
    {
        fmt::print("\n Enter rental price (VNĐ/day): ");
        if (cin >> rentalPrice && rentalPrice > 0)
        {
            cin.ignore(10000, '\n');
            break;
        }
        fmt::print("❌ Error: Rental price must be positive! Please re-enter.\n");
        cin.clear();
        cin.ignore(10000, '\n');
    }

    Vehicles m(licensePlate, manufacturer, yearOfManufacture, vehicleType, "AVAILABLE", rentalPrice, 0, "");
    m.rentalStatus = "";
    m.renterName = "";
    m.rentalTimestamp = 0;

    list[m.licensePlate] = m;
    saveToJson(list, jsondata);

    fmt::print("\n   Vehicle added successfully! \n");
}

void findVehicle(const unordered_map<string, Vehicles> &list)
{
    string plate;
    cin.ignore();
    fmt::print("\n Enter license plate to search: ");
    getline(cin, plate);

    auto result = list.find(plate);

    if (result != list.end())
    {
        fmt::print(" Vehicle found: \n");
        displayVehicleInfo(result->second);
    }

    else
        fmt::print(" No information found for license plate: {}! \n", plate);
}

void deleteVehicle(unordered_map<string, Vehicles> &list)
{
    string plate;
    cin.ignore();
    fmt::print("\n Enter license plate to delete: ");
    getline(cin, plate);

    if (list.find(plate) != list.end())
    {
        list.erase(plate);
        fmt::print("  Vehicle with license plate {} deleted. \n", plate);

        // Update JSON
        saveToJson(list, jsondata);
    }

    else
        fmt::print(" Vehicle not found! \n");
}

void saveList(const unordered_map<string, Vehicles> &ds)
{
    ofstream file("data.txt");

    if (!file)
    {
        fmt::print("❌ Error: Unable to open file for saving\n");
        return;
    }

    for (const auto &[bS, vehicle] : ds)
        file << vehicle.licensePlate << ", " << vehicle.manufacturer << ", " << vehicle.yearOfManufacture << ", "
             << vehicle.vehicleType << ", " << vehicle.rentalStatus << ", " << vehicle.rentalPrice << ", "
             << vehicle.rentalTimestamp << ", " << vehicle.renterName << "\n";

    if (file.fail())
        fmt::print("❌ Error: Failed to write data to file\n");

    file.close();
    saveToJson(ds, jsondata);
}

void loadList(unordered_map<string, Vehicles> &vehiclesMap)
{
    ifstream file("data.txt");

    if (!file)
    {
        fmt::print("⚠️ Warning: File not found, starting with a new list\n");
        return;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string licensePlate, manufacturer, vehicleType, rentalStatus, renterName;
        unsigned yearOfManufacture;
        double rentalPrice;
        time_t rentalTimestamp;

        // Parse each field from the line
        getline(ss, licensePlate, ',');
        getline(ss >> ws, manufacturer, ',');

        string yearStr;
        getline(ss >> ws, yearStr, ',');
        yearOfManufacture = stoi(yearStr);

        getline(ss >> ws, vehicleType, ',');
        getline(ss >> ws, rentalStatus, ',');

        string priceStr;
        getline(ss >> ws, priceStr, ',');
        rentalPrice = stod(priceStr);

        string timestampStr;
        getline(ss >> ws, timestampStr, ',');
        rentalTimestamp = stol(timestampStr);

        getline(ss >> ws, renterName);

        // Validate and store
        if (!licensePlate.empty() && !manufacturer.empty() && yearOfManufacture > 1900 &&
            !vehicleType.empty() && !rentalStatus.empty() && rentalPrice > 0)
        {
            vehiclesMap[licensePlate] = Vehicles(
                licensePlate, manufacturer, yearOfManufacture,
                vehicleType, rentalStatus, rentalPrice,
                rentalTimestamp, renterName);
        }
        else
        {
            fmt::print("⚠️ Invalid line:\n"
                       "  plate='{}'\n  brand='{}'\n  year={}\n  type='{}'\n  status='{}'\n  price={}\n  ts={}\n  renter='{}'\n",
                       licensePlate, manufacturer, yearOfManufacture, vehicleType, rentalStatus,
                       rentalPrice, rentalTimestamp, renterName);
            fmt::print("⚠️ Skipping line: '{}'\n", line);
        }
    }

    file.close();

    // Load data from JSON (append or override)
    loadFromJson(vehiclesMap, jsondata);
}

// ---------------------------------------------------------------------
