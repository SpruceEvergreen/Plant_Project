#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <string>
#include <cctype>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "Plant.h"

// Function declatarions
// transforming string functions
std::string replace_spaces(std::string& s);
std::string replace_spaces_back(std::string& s);
std::string transform_string(std::string& s);
std::string transform_string_back(std::string& s);
std::string transform_string_to_lower(std::string& s);
// helping functions
void create_plants(std::vector<Plant>& plants);
void display_menu();
// main menu functions
void display_plants(std::vector<Plant> const& plants);
bool add_plant(std::vector<Plant>& plants);
bool remove_plant_from_file(int plant_num, std::vector<Plant>& plants);
void remove_plant(std::vector<Plant>& plants);
bool change_plant_in_file(int plant_num, std::vector<Plant>& plants);
void change_plant(std::vector<Plant>& plants);
void search_plant(std::vector<Plant>& plants);

// replaces spaces in the string with _
std::string replace_spaces(std::string& s) {
    std::replace(s.begin(), s.end(), ' ', '_');
    return s;
}

// places spaces back instead of _
std::string replace_spaces_back(std::string& s) {
    std::replace(s.begin(), s.end(), '_', ' ');
    return s;
}

// replaces spaces in the string with _ and makes all characters of the string uppercase 
std::string transform_string(std::string& s) {
    std::replace(s.begin(), s.end(), ' ', '_');
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
    return s;
}

// replaces _ in the string with spaces and makes all characters of the string lowercase
std::string transform_string_back(std::string& s) {
    std::replace(s.begin(), s.end(), '_', ' ');
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

// makes all characters of the string lowercase
std::string transform_string_to_lower(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}
// creating a vector of Plant objects from the file "file_plants.txt"
void create_plants(std::vector<Plant>& plants) {
    std::fstream in_file;
    in_file.open("file_plants.txt");
    std::string info;

    if (!plants.empty()) {
        plants.clear();
    }

    while (getline(in_file, info)) {
        std::istringstream iss{ info };
        std::string name{};
        std::string date{};
        std::string date2{};
        std::string date3{};

        iss >> name >> date >> date2 >> date3;

        replace_spaces_back(name);
        replace_spaces_back(date);
        replace_spaces_back(date2);
        replace_spaces_back(date3);

        Plant* new_plant = new Plant{ name, date, date2, date3 };
        plants.push_back(*new_plant);
        delete new_plant;
    }

    in_file.close();

}
// displaying the vector of Plants objects
void display_plants(std::vector<Plant> const& plants) {
    if (plants.size() == 1) {
        std::cout << "\nThe database has " << plants.size() << " plant" << std::endl;
    }
    else {
        std::cout << "\nThe database has " << plants.size() << " plants" << std::endl;
    }

    int number{};
    for (auto const& v : plants) {
        ++number;
        std::cout << "******* Plant number #: " << number << " *******" << std::endl;
        std::cout << std::setw(25) << std::left << "Plant name: "
            << std::setw(50) << std::left << v.get_name() << std::endl;
        std::cout << std::setw(25) << std::left << "Date of purchase: "
            << std::setw(50) << std::left << v.get_date_of_purchase() << std::endl;
        std::cout << std::setw(25) << std::left << "Last date of watering: "
            << std::setw(50) << std::left << v.get_date_of_last_watering() << std::endl;
        std::cout << std::setw(25) << std::left << "Added on: "
            << std::setw(50) << std::left << v.get_added_date() << std::endl;
        std::cout << "\n" << std::endl;
    }
}
// adding plant object to the "file_plants.txt" file and creating vector of Plant objects with updated infromation from the "file_plants.txt" file
bool add_plant(std::vector<Plant>& plants) {
    std::ofstream out_file{ "file_plants.txt", std::ios::app };
    if (!out_file) {
        std::cerr << "Error opening output file" << std::endl;
        return 1;
    }

    std::vector<std::string> temp{};
    for (auto p : plants) {
        temp.push_back(p.get_name());
    }

    // Making all the names uppercase and replacing spaces with '_'
    for (auto& t : temp) {
        transform_string(t);
    }

    std::string name, name_buffer, date_of_purchase, date_of_last_watering;
    std::cout << "Adding a new plant" << std::endl;
    bool done = false;

    do {
        std::cout << "Enter plant name (Q for quiting)";
        std::cout << "\nPlease, make sure that the name doesn't contain \"_\": ";
        getline(std::cin, name);
        name_buffer = name;
        transform_string(name_buffer);
        int name_counter{};
        if (name_buffer == "Q") {
            std::cout << "Quiting adding the plant" << std::endl;
            return true;
        }

        for (const auto& t : temp)
            if (t == name_buffer) {
                name_counter++;
            }

        if (name_counter > 0) {
            std::cout << "The plant with this name already exists" << std::endl;
        }
        if (name_counter == 0) {
            done = true;
        }
    } while (name_buffer != "Q" && !done);


    std::cout << "Enter plant date of purchase: ";
    getline(std::cin, date_of_purchase);
    std::cout << "Enter date of last watering: ";
    getline(std::cin, date_of_last_watering);
    replace_spaces(name);
    replace_spaces(date_of_purchase);
    replace_spaces(date_of_last_watering);

    // Getting local date to put into the added_date variable 
    std::time_t t = std::time(nullptr);
    char mbstr[11];
    std::strftime(mbstr, sizeof(mbstr), "%d/%m/%Y", std::localtime(&t));
    std::string added_date{ mbstr };

    out_file << name << " " << date_of_purchase << " " << date_of_last_watering
        << " " << added_date << std::endl;

    create_plants(plants);
    out_file.close();
    return true;
}

// removing plant object from the "file_plants.txt" file and creating vector of Plant objects with updated infromation from the "file_plants.txt" file
bool remove_plant_from_file(int plant_num, std::vector<Plant>& plants) {
    std::ofstream remove_plant;
    remove_plant.open("file_plants.txt");

    if (!remove_plant) {
        std::cerr << "Error opening output file" << std::endl;
        return 1;
    }

    std::cout << "\nPlant *" << plants.at(plant_num).get_name() << "* was deleted" << std::endl;

    for (size_t i = 0; i < plants.size(); i++)
        if (i != static_cast<size_t>(plant_num)) {

            // Modifing the contents of std::vector<Plant> plants {} so the attributes 
            // have the _ instead of any white spaces in the attributes (so they would be
            // the same style as content of the file)
            std::string temp_name = plants.at(i).get_name();
            replace_spaces(temp_name);
            plants.at(i).change_name(temp_name);

            std::string temp_date1 = plants.at(i).get_date_of_purchase();
            replace_spaces(temp_date1);
            plants.at(i).change_date_of_purchase(temp_date1);

            std::string temp_date2 = plants.at(i).get_date_of_last_watering();
            replace_spaces(temp_date2);
            plants.at(i).change_date_of_last_watering(temp_date2);

            // writing modified attributes into the file
            remove_plant << plants.at(i).get_name() << " "
                << plants.at(i).get_date_of_purchase() << " "
                << plants.at(i).get_date_of_last_watering() << " "
                << plants.at(i).get_added_date() << std::endl;
        }

    remove_plant.close();
    create_plants(plants);
    return true;
}

// Validating the id of the plant that the user wants to delete
void remove_plant(std::vector<Plant>& plants) {
    std::vector<std::string> temp{};
    for (auto p : plants) {
        temp.push_back(p.get_name());
    }

    // Making all the names uppercase and replacing spaces with '_'
    for (auto& t : temp) {
        transform_string(t);
    }

    // Displaying the contents of the temp vector for control
    std::cout << "Temp vector" << std::endl;
    for (auto t : temp) {
        std::cout << t << std::endl;
    }

    std::string entry{};
    size_t num_value{};
    size_t plant_num{};
    int loop_counter{};
    bool done = false;

    do {
        // if looping more than once
        if (loop_counter > 0) {
            std::cout << "Sorry, can't find plant" << std::endl;
        }

        // promting the user
        std::cout << "The plants are: " << std::endl;
        for (size_t i{}; i < plants.size(); ++i) {
            std::cout << std::setw(15) << std::left << "Plant number: "
                << std::setw(5) << std::left << i + 1
                << std::setw(15) << std::left << "Plant name: "
                << std::setw(50) << std::left << plants.at(i).get_name() << std::endl;
        }
        std::cout << "\nWhat plant do you want to delete?" << std::endl;
        std::cout << "Enter plant number or name (Q for quiting): ";
        getline(std::cin, entry);
        std::istringstream validator{ entry };

        // checking if entry is an integer or not
        if (validator >> num_value) {
            // if entry is an integer
            if (num_value > plants.size()) {
                std::cout << "\nPlant number " << num_value;
                std::cout << " is not in the file." << std::endl;
                std::cout << "File has " << plants.size();
                std::cout << " plants." << std::endl;
            }
            else {
                num_value--;
                plant_num = num_value;
                remove_plant_from_file(plant_num, plants);
                done = true;
            }

        }
        else {
            // if entry is not an integer
            int counter{};
            transform_string(entry);

            // Checking if the entry is Q for quiting
            if (entry == "Q") {
                std::cout << "Not removing plants" << std::endl;
            }
            else {
                // Looking for plant name in the vector of names
                for (size_t i{}; i < temp.size(); i++) {
                    if (temp.at(i) == entry) {
                        counter = i;
                        plant_num = counter;
                        remove_plant_from_file(plant_num, plants);
                        done = true;
                    }
                }
            }
        }
        loop_counter++;
    } while (entry != "Q" && !done);
}
//changing plant object from the "file_plants.txt" file and creating vector of Plant objects with updated infromation from the "file_plants.txt" file
bool change_plant_in_file(int plant_num, std::vector<Plant>& plants) {

    // Information message
    std::cout << "\nPlant No." << plant_num + 1
        << " *" << plants.at(plant_num).get_name() << "* will be changed" << std::endl;

    // getting the type of value to change
    bool validator_num = false;
    int value_to_change{};
    std::string temp_holder{};
    do {
        std::cout << "\nEnter 1 to change name" << std::endl;
        std::cout << "Enter 2 to change date of purchase" << std::endl;
        std::cout << "Enter 3 to change date of last watering" << std::endl;
        std::cout << "Type your entry here: ";
        getline(std::cin, temp_holder);
        std::istringstream validator_num_ss{ temp_holder };

        if (validator_num_ss >> value_to_change) {
            validator_num = true;
        }
        else {
            std::cout << "Sorry, not found. Try again" << std::endl;
        }

    } while (!validator_num);

    // getting the value to change and changing it in the vector plants
    if (value_to_change == 1) {
        std::string new_name{};
        std::cout << "Type in new name: ";
        std::getline(std::cin, new_name);
        std::cout << "Name of the plant *" << plants.at(plant_num).get_name()
            << "* was changed for *"
            << new_name << "*" << std::endl;
        plants.at(plant_num).change_name(new_name);
    }
    else if (value_to_change == 2) {
        std::string new_date_of_purchase{};
        std::cout << "Type in new date of purchase: ";
        std::getline(std::cin, new_date_of_purchase);
        plants.at(plant_num).change_date_of_purchase(new_date_of_purchase);
        std::cout << "Date of purchase of the plant *" << plants.at(plant_num).get_name()
            << "* was changed for *"
            << new_date_of_purchase << "*" << std::endl;
    }
    else if (value_to_change == 3) {
        std::string new_date_of_last_watering{};
        std::cout << "Type in new date of last watering: ";
        std::getline(std::cin, new_date_of_last_watering);
        plants.at(plant_num).change_date_of_last_watering(new_date_of_last_watering);
        std::cout << "Date of last watering of the plant *" << plants.at(plant_num).get_name()
            << "* was changed for " << "*"
            << new_date_of_last_watering << "*" << std::endl;
    }
    else {
        std::cout << "Not found" << std::endl;
    }

    // opening file to change the value 
    std::ofstream change_plant;
    change_plant.open("file_plants.txt");

    if (!change_plant) {
        std::cerr << "Error opening output file" << std::endl;
        return 1;
    }

    // writing to the file from the vector plants with the changed value
    for (size_t i = 0; i < plants.size(); i++) {
        std::string temp_name = plants.at(i).get_name();
        replace_spaces(temp_name);
        plants.at(i).change_name(temp_name);

        std::string temp_date1 = plants.at(i).get_date_of_purchase();
        replace_spaces(temp_date1);
        plants.at(i).change_date_of_purchase(temp_date1);

        std::string temp_date2 = plants.at(i).get_date_of_last_watering();
        replace_spaces(temp_date2);
        plants.at(i).change_date_of_last_watering(temp_date2);

        change_plant << plants.at(i).get_name() << " "
            << plants.at(i).get_date_of_purchase() << " "
            << plants.at(i).get_date_of_last_watering() << " "
            << plants.at(i).get_added_date() << std::endl;
    }
    change_plant.close();
    create_plants(plants);

    return true;
}
// Validating the id of the plant that the user wants to change
void change_plant(std::vector<Plant>& plants) {
    std::vector<std::string> temp{};
    for (auto p : plants) {
        temp.push_back(p.get_name());
    }

    // Making all the names uppercase and replacing spaces with '_'
    for (auto& t : temp) {
        transform_string(t);
    }

    // getting the number of the plant to change
    std::string entry{};
    size_t num_value{};
    size_t plant_num{};
    int loop_counter{};
    bool done = false;

    do {
        // if looping more than once
        if (loop_counter > 0) {
            std::cout << "Sorry, can't find plant" << std::endl;
        }

        // promting the user
        std::cout << "The plants are: " << std::endl;
        for (size_t i{}; i < plants.size(); ++i) {
            std::cout << std::setw(15) << std::left << "Plant number: "
                << std::setw(5) << std::left << i + 1
                << std::setw(15) << std::left << "Plant name: "
                << std::setw(50) << std::left << plants.at(i).get_name() << std::endl;
        }
        std::cout << "\nWhat plant do you want to change?";
        std::cout << "\nEnter plant number or name (Q for quiting): ";
        getline(std::cin, entry);
        std::istringstream validator{ entry };

        // checking if entry is an integer
        if (validator >> num_value) {
            // if entry is an integer
            if (num_value > plants.size()) {
                std::cout << "\nPlant number " << num_value;
                std::cout << " is not in the file." << std::endl;
                std::cout << "File has " << plants.size();
                std::cout << " plants." << std::endl;
            }
            else {
                num_value--;
                plant_num = num_value;
                change_plant_in_file(plant_num, plants);
                done = true;
            }

        }
        else {
            // if entry is not an integer
            int counter{};
            transform_string(entry);

            // Checking if the entry is Q for quiting
            if (entry == "Q") {
                std::cout << "Not changing plant information" << std::endl;
            }
            else {
                // Looking for plant name in the vector of names
                for (size_t i{}; i < temp.size(); i++) {
                    if (temp.at(i) == entry) {
                        counter = i;
                        plant_num = counter;
                        change_plant_in_file(plant_num, plants);
                        done = true;
                    }
                }
            }
        }
        loop_counter++;
    } while (entry != "Q" && !done);
}
// searching the Plant vector for the plant via name (partial match)
void search_plant(std::vector<Plant>& plants) {
    std::cout << "Type what you want to find: ";
    std::string search_key{};      // var to store the input
    std::getline(std::cin, search_key); // reading input into the var
    transform_string(search_key);       // transforming contents of the var to the needed format
    std::vector<int> plant_num{};
    std::vector<Plant> temp{};
    create_plants(temp);
    for (auto& t : temp) {
        std::string buff = t.get_name();
        transform_string(buff);
        t.change_name(buff);
    }

    int match_count{};     // var to store amount of matching cases
    // looping the the temp vector to find partial matches and storing them into the match map
    for (size_t i = 0; i < temp.size(); i++) {
        if (temp.at(i).get_name().find(search_key) != std::string::npos) {
            match_count++;
            plant_num.push_back(i);
        }
    }
    // displaying the amount of matches
    if (match_count == 1) {
        std::cout << "Found " << match_count << " match" << std::endl;
    }
    else {
        std::cout << "Found " << match_count << " matches" << std::endl;
    }

    for (const auto& n : plant_num) {
        std::cout << std::setw(15) << std::left << "Plant number: "
            << std::setw(5) << std::left << n + 1
            << std::setw(15) << std::left << "Plant name: "
            << std::setw(20) << std::left << plants.at(n).get_name()
            << std::setw(10) << std::left << "Added on: "
            << std::setw(15) << std::left << plants.at(n).get_added_date() << std::endl;
    }

}
// main menu driver function 
void display_menu() {
    std::cout << "\nD - Display all plants" << std::endl;
    std::cout << "A - Add a plant" << std::endl;
    std::cout << "R - Remove a plant" << std::endl;
    std::cout << "C - Change plant information" << std::endl;
    std::cout << "S - Search for a plant by name" << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << "Enter a selection (Q to quit): ";
}

int main() {
    std::vector<Plant> plants{};
    create_plants(plants);
    std::string selection{};
    do {
        display_menu();
        getline(std::cin, selection);
        std::transform(selection.begin(), selection.end(), selection.begin(),
            [](unsigned char c) { return std::toupper(c); });
        if (selection == "D") {
            std::cout << "\nDisplaying plants" << std::endl;
            display_plants(plants);
        }
        else if (selection == "A") {
            add_plant(plants);
        }
        else if (selection == "R") {
            remove_plant(plants);
        }
        else if (selection == "C") {
            change_plant(plants);
        }
        else if (selection == "S") {
            search_plant(plants);
        }
        else if (selection == "Q")
            std::cout << "\nQuitting" << std::endl;
        else
            std::cout << "\nNot found, try again...";
    } while (selection != "Q");


    return 0;
}

