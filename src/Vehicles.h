#pragma once

#include <ctime>
// #include <fmt/core.h>
#include "../third_party/fmt-src/include/fmt/core.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <locale.h> // Required for setlocale()

#include <utility>
#include "rental_export.h"

using namespace std;

extern string LOG;

struct Vehicles
{
    string licensePlate;
    string manufacturer;
    unsigned yearOfManufacture;

    string vehicleType;
    string rentalStatus; // Default status is "AVAILABLE"
    double rentalPrice;
    time_t rentalTimestamp = 0; // Timestamp of the most recent rental
    string renterName;

    // 🔥 Default constructor
    Vehicles() : licensePlate(""), manufacturer(""), yearOfManufacture(0), vehicleType(""),
                 rentalStatus("AVAILABLE"), rentalPrice(0.0), rentalTimestamp(0), renterName("") {}

    // 🔥 Parameterized constructor
    Vehicles(const string &_licensePlate, const string &_manufacturer, unsigned int _yearOfManufacture,
             const string &_vehicleType, const string &_rentalStatus, double _rentalPrice,
             time_t _rentalTimestamp, const string &_renterName);

    // ✅ Getter methods
    string getMake() const;
    string getModel() const;
    unsigned getYear() const;
    string getLicensePlate() const;
    double getRentalPrice() const;

    string getVehicleType() const;
    string getRentalStatus() const;
    string getRenterName() const;

    // ✅ Rental status management
    bool isRented() const;
    void returnVehicle();
    void rentVehicle(const string &renter);
};

// Display vehicle information
void displayVehicle(const Vehicles &);
void displayVehicleInfo(const Vehicles &);
void displayListVehicleInfo(const unordered_map<string, Vehicles> &);

// Vehicle management functions
void addVehicle(unordered_map<string, Vehicles> &);
void findVehicle(const unordered_map<string, Vehicles> &);
void deleteVehicle(unordered_map<string, Vehicles> &);
void saveList(const unordered_map<string, Vehicles> &);
void loadList(unordered_map<string, Vehicles> &);


// Rental transaction structure
struct RentalTransaction
{
    string transaction_id;
    string customer_id;
    string vehicle_id;
    time_t timestamp;
    string status; // "RENTED" or "RETURNED"
};
