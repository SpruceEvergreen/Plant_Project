#include "Plant.h"

#include <iostream>

Plant::Plant(std::string name, std::string date_of_purchase, std::string date_of_last_watering, std::string added_date)
    : name{ name }, date_of_purchase{ date_of_purchase },
    date_of_last_watering{ date_of_last_watering }, added_date{ added_date } {}


std::ostream& operator<<(std::ostream& os, const Plant& p) {
    os << std::setw(30) << std::left << p.name
        << std::setw(20) << std::left << p.date_of_purchase
        << std::setw(20) << std::left << p.date_of_last_watering;
    return os;
}
