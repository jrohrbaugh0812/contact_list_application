#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<map>
#include<functional>

std::string stringToLowerCase(std::string& stringValue) {
	for (int i = 0; i < stringValue.length(); i++) {
		stringValue.at(i) = std::tolower(stringValue.at(i));
	}

	return stringValue;
}

bool fileExists(std::string& fileName) {
	std::ifstream file(fileName);
	return file.good();
}

std::string getLine(std::string& fileName, const std::string& searchString) {
	std::ifstream file(fileName);

	if (!file.is_open()) {
		std::cerr << "Error: unable to open file " << fileName << std::endl;
		return "";
	}

	std::string line;
	while (std::getline(file, line)) {
		if (line.find(searchString) != std::string::npos) {
			return line;
		}
	}

	// If the search string is not found in any line;
	return "";
}

void createContactList(std::string& fileName) {
	std::ofstream contactList{ fileName };

	for (int i = 0; i < 90; i++) {
		contactList << "-";
	}

	contactList << "\n";
	contactList << "|";
	for (int i = 1; i < 39; i++) {
		contactList << " ";
	}
	contactList << "Contact List";
	for (int i = 51; i < 89; i++) {
		contactList << " ";
	}
	contactList << "|";
	contactList << "\n";
	contactList << "| |Name                |Number              |Address             |Email               |  |";
	contactList << "\n";
	for (int i = 0; i < 90; i++) {
		contactList << "-";
	}
	
	contactList.close();
}

void displayContactList(std::string& fileName) {
	std::ifstream contactList{ fileName };

	if (!contactList.is_open()) {
		std::cerr << "Error: Unable to open file " << fileName << std::endl;
		return;
	}

	std::string line;
	while (std::getline(contactList, line)) {
		std::cout << line << std::endl;
	}

	contactList.close();
}

struct Contact {
	std::string name;
	std::string number;
	std::string address;
	std::string email;

	Contact()
		: name(""), number(""), address(""), email("") {}
	Contact(std::string& name, std::string& number, std::string& address, std::string& email)
		: name(name), number(number), address(address), email(email) {}
};

Contact convertStringToContact(std::string& line) {
	std::string contactName = line.substr(3, 20);
	std::string contactNumber = line.substr(24, 20);
	std::string contactAddress = line.substr(45, 20);
	std::string contactEmail = line.substr(66, 20);

	Contact contact(contactName, contactNumber, contactAddress, contactEmail);

	return contact;
}

std::string convertContactToString(Contact newContact, std::string& fileName) {
	for (int i = 0; i < 20; i++) {
		if (newContact.name.length() < 20) {
			newContact.name += " ";
		}
		if (newContact.number.length() < 20) {
			newContact.number += " ";
		}
		if (newContact.address.length() < 20) {
			newContact.address += " ";
		}
		if (newContact.email.length() < 20) {
			newContact.email += " ";
		}
	}
	return "|  " + newContact.name + " " + newContact.number + " " + newContact.address + " " + newContact.email + "   |";
}

void insertContact(Contact newContact, std::string& fileName);

bool checkContactString(std::string& string) {
	if (string.length() >= 20) {
		return false;
	}
	else if (string == "" || string.at(0) == ' ') {
		return false;
	}
	return true;
}

bool checkContactNumber(std::string& number) {
	
	if (number == "") {
		return false;
	}
	else if (number.length() != 12) {
		return false;
	}
	for (int i = 0; i <= 11; i++) {
		if (i <= 2 || (i >= 4 && i <= 6) || i >= 8) {
			if (!isdigit(number.at(i))) {
				return false;
			}
		}
		else {
			if (number.at(i) != '-') {
				return false;
			}
		}
	}
	return true;
}

bool checkContactEmail(std::string& email) {

	if (email == "" || email.at(0) == '@') {
		return false;
	}

	bool atSignPresent = false;
	bool atSignInBetween = false;

	for (int i = 0; i < email.length(); i++) {
		if (email.at(i) == '@') {
			atSignPresent = true;
		}
		if (i > 0) {
			if (email.at(i - 1) == '@') {
				atSignInBetween = true;
			}
		}
	}

	if (!atSignPresent || !atSignInBetween) {
		return false;
	}

	return checkContactString(email);
}

Contact getContactFromUser(std::string& fileName) {
	Contact contact;

	std::cin.ignore();
	while (!checkContactString(contact.name)) {
		std::cout << "Name (< 20 characters): " << std::endl;
		std::getline(std::cin, contact.name);
	}
	while (!checkContactNumber(contact.number)) {
		std::cout << "Phone Number (XXX-XXX-XXXX): " << std::endl;
		std::getline(std::cin, contact.number);
	}
	while (!checkContactString(contact.address)) {
		std::cout << "Address (< 20 characters): " << std::endl;
		std::getline(std::cin, contact.address);
	}
	while (!checkContactEmail(contact.email)) {
		std::cout << "Email (< 20 characters): " << std::endl;
		std::getline(std::cin, contact.email);
	}

	return contact;
}

void insertContact(Contact newContact, std::string& fileName) {
	std::string contactListColumnLine = getLine(fileName, "| |Name ");
	std::string newLine = convertContactToString(newContact, fileName);

	std::ifstream contactListInput{ fileName, std::ios_base::app };

	std::vector<std::string> lines;
	std::string line;

	while (std::getline(contactListInput, line)) {
		lines.push_back(line);
		if (line.find(contactListColumnLine) != std::string::npos) {
			lines.push_back(newLine);
		}
	}

	contactListInput.close();

	std::ofstream contactListOutput(fileName);

	for (const auto& l : lines) {
		contactListOutput << l << std::endl;
	}

	contactListOutput.close();
}

bool validFileName(std::string& fileName) {
	for (int i = 0; i < fileName.length(); i++) {
		if (fileName.at(i) == '.') {
			return false;
		}
	}

	return true;
}

void deleteContactFromContactList(Contact oldContact, std::string& fileName) {
	bool deleted = false;
	std::string deleteLine = convertContactToString(oldContact, fileName);
	std::string currentLine;

	std::ifstream contactList{ fileName };
	std::ofstream tempFile{ "temp.txt" };

	while (std::getline(contactList, currentLine)) {
		if (currentLine.find(deleteLine) == std::string::npos) {
			tempFile << currentLine << std::endl;
		}
		else {
			deleted = true;
		}
	}

	contactList.close();
	tempFile.close();

	if (!deleted) {
		std::cout << "Contact does not exist." << std::endl;
		if (remove("temp.txt") != 0) {
			std::cerr << "Error: Unable to delete temporary file " << fileName << std::endl;
		}
		return;
	}

	if (remove(fileName.c_str()) != 0) {
		std::cerr << "Error: Unable to delete original file " << fileName << std::endl;
		return;
	}
	if (rename("temp.txt", fileName.c_str()) != 0) {
		std::cerr << "Error: Unable to rename temporary file to " << fileName << std::endl;
		return;
	}
}

void sortContactList(std::string& sortBy, std::string& order, std::string& fileName) {
	std::string currentLine;
	std::string tempFileName = "temp.txt";
	Contact currentContact;

	std::ifstream contactList{ fileName };
	std::ofstream tempFile{ tempFileName };
	tempFile.close();
	createContactList(tempFileName);

	std::vector<Contact> contacts;

	int count = 0;
	while (std::getline(contactList, currentLine)) {
		count++;
		if (count >= 4 && currentLine.find("---") == std::string::npos) {
			currentContact = convertStringToContact(currentLine);
			contacts.push_back(currentContact);
		}
	}

	std::map<std::string, std::function<bool(const Contact&, const Contact&)>> comparators = {
		{"name", [](const Contact& a, const Contact& b) {return a.name < b.name; }},
		{"number", [](const Contact& a, const Contact& b) {return a.number < b.number; }},
		{"email", [](const Contact& a, const Contact& b) {return a.email < b.email; }},
		{"address", [](const Contact& a, const Contact& b) {return a.address < b.address; }}
	};

	std::sort(contacts.begin(), contacts.end(), [&](const Contact& a, const Contact b) {
		if (order == "ascending") {
			return comparators[sortBy](a, b);
		}
		else {
			return comparators[sortBy](b, a);
		}
	});

	for (int i = contacts.size() - 1; i >= 0; i--) {
		insertContact(contacts.at(i), tempFileName);
	}

	contactList.close();

	if (remove(fileName.c_str()) != 0) {
		std::cerr << "Error: Unable to delete original file " << fileName << std::endl;
		return;
	}
	if (rename("temp.txt", fileName.c_str()) != 0) {
		std::cerr << "Error: Unable to rename temporary file to " << fileName << std::endl;
		return;
	}
}

std::string initializeApp() {
	std::cout << "Welcome to Jacob's Contact List Application..." << std::endl;

	std::string alreadyHaveList;
	std::string fileName;

	bool acceptableFile = false;
	while (!acceptableFile) {
		std::cout << "Do you have a prexisting contact list (Yes or No)? ";
		std::cin >> alreadyHaveList;
		std::cin.ignore();
		if (stringToLowerCase(alreadyHaveList) == "yes") {
			std::cout << "What is the name of your contact list?";
			std::getline(std::cin, fileName);
			if (validFileName(fileName)) {
				fileName += ".txt";
				if (fileExists(fileName)) {
					std::cout << "Located file, opening..." << std::endl;
					acceptableFile = true;
				}
				else {
					std::cout << "File does not exist." << std::endl;
				}
			}
			else {
				std::cout << "File name not valid." << std::endl;
			}
		}
		else if (stringToLowerCase(alreadyHaveList) == "no") {
			std::cout << "What would you like to name your new contact list? ";
			std::getline(std::cin, fileName);
			if (validFileName(fileName)) {
				fileName += ".txt";
				createContactList(fileName);
				acceptableFile = true;
			}
			else {
				std::cout << "Invalid file name." << std::endl;
			}
		}
		else {
			std::cout << "Sorry, that is not a valid response.";
		}
	}

	std::cout << "Here is your contact list (" << fileName << "):" << std::endl;
	displayContactList(fileName);

	return fileName;
}

void handleAddContact(std::string& fileName) {
	std::string addContact;

	std::cout << "Would you like to add a new contact (Yes or No)? " << std::endl;
	std::cin >> addContact;

	if (stringToLowerCase(addContact) == "yes") {
		Contact newContact = getContactFromUser(fileName);
		insertContact(newContact, fileName);
		std::cout << "Here is your updated contact list (" << fileName << "):" << std::endl;
		displayContactList(fileName);
	}
}

void handleSortContact(std::string& fileName) {
	std::string sortContacts;

	std::cout << "Would you like to sort your contact list (Yes or No)? " << std::endl;
	std::cin >> sortContacts;
	if (stringToLowerCase(sortContacts) == "yes") {
		std::string sortBy;
		std::string order;
		while (sortBy != "name" && sortBy != "number" && sortBy != "address" && sortBy != "email") {
			std::cout << "How would you like to sort your contact list (name, number, address, or email)? " << std::endl;
			std::cin >> sortBy;
			sortBy = stringToLowerCase(sortBy);
		}
		while (order != "ascending" && order != "descending") {
			std::cout << "How would you like to order your contact list (ascending or descending)? " << std::endl;
			std::cin >> order;
			order = stringToLowerCase(order);
		}
		sortContactList(sortBy, order, fileName);
		std::cout << "Here is your sorted contact list (" << fileName << "):" << std::endl;
		displayContactList(fileName);
	}
}

void handleDeleteContact(std::string& fileName) {
	std::string deleteContact;

	std::cout << "Would you like to delete an old contact (Yes or No)? " << std::endl;
	std::cin >> deleteContact;
	if (stringToLowerCase(deleteContact) == "yes") {
		Contact oldContact = getContactFromUser(fileName);
		deleteContactFromContactList(oldContact, fileName);
		std::cout << "Here is your updated contact list (" << fileName << "):" << std::endl;
		displayContactList(fileName);
	}
}

int main() {
	std::string fileName = initializeApp();
	
	while (true) {
		handleAddContact(fileName);
		handleSortContact(fileName);
		handleDeleteContact(fileName);
		std::string continueApp;
		std::cout << "Is there more you would like to do (yes or no)? " << std::endl;
		std::cin >> continueApp;
		if (stringToLowerCase(continueApp) == "no") {
			break;
		}
	}

	std::cout << "Exiting application..." << std::endl;
	
	return 0;
}