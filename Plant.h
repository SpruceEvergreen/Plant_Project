#ifndef PLANT_H
#define PLANT_H

#include <string>
#include <iomanip>

class Plant {
    friend std::ostream& operator<<(std::ostream& os, const Plant& p);
    std::string name;
    std::string date_of_purchase;
    std::string date_of_last_watering;
    std::string added_date;

public:

    Plant() = default;

    Plant(std::string name, std::string date_of_purchase, std::string date_of_last_watering,
        std::string added_date);

    std::string get_name() const {
        return name;
    }

    std::string change_name(std::string name) {
        this->name = name;
        return name;
    }

    std::string get_date_of_purchase() const {
        return date_of_purchase;
    }

    std::string change_date_of_purchase(std::string date_of_purchase) {
        this->date_of_purchase = date_of_purchase;
        return date_of_purchase;
    }

    std::string get_date_of_last_watering() const {
        return date_of_last_watering;
    }

    std::string change_date_of_last_watering(std::string date_of_last_watering) {
        this->date_of_last_watering = date_of_last_watering;
        return date_of_last_watering;
    }

    bool operator<(const Plant& rhs) const {
        return this->name < rhs.name;
    }

    bool operator==(const Plant& rhs) const {
        return this->name == rhs.name;
    }

    std::string get_added_date() const {
        return added_date;
    }
};

#endif // PLANT_H
