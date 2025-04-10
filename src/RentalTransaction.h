#pragma once

#include "Vehicles.h"

// Logging and transaction functions
void writeLog(const RentalTransaction &);

//  for test
bool tryRentVehicle(unordered_map<string, Vehicles> &,
                    vector<RentalTransaction> &,
                    const string &,
                    const string &);
// end

void checkTransaction(const vector<RentalTransaction> &, unordered_map<string, Vehicles> &);
void checkRentVehicle(unordered_map<string, Vehicles> &, vector<RentalTransaction> &);
void checkReturnVehicle(unordered_map<string, Vehicles> &, vector<RentalTransaction> &);

// Display rented vehicles
void showRentedVehicles(const string &);