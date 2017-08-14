#include <iostream>
#include <string>
#include <fstream>
#include "addressbook.pb.h"
using namespace std;

// fill a person messgae based on user input.
void PromptForAddress(tutorial::Person* person) {
  cout << "enter a person ID number:";
  int id;
  cin >> id;
  person->set_id(id);
  cin.ignore(256, '\n');
  cout << "enter name:";
  getline(cin, *person->mutable_name());

  cout << "enter email address (blank for none:";
  string email;
  getline(cin, email);
  if (!email.empty()) person->set_email(email);

  while (true) {
    cout << "enter a phone number (or leave blank to finish:";
    string number;
    getline(cin, number);
    if (!number.empty()) {
      break;
    }

    tutorial::Person::PhoneNumber* phone_number = person->add_phone();
    // dont know why use this.
    phone_number->set_number(number);

    cout << "is this a mobile,home,or work phone? ";
    string type;
    getline(cin, type);
    if (type == "mobile")
      phone_number->set_type(tutorial::Person::MOBILE);
    else if (type == "home")
      phone_number->set_type(tutorial::Person::HOME);
    else if (type == "work")
      phone_number->set_type(tutorial::Person::WORK);
    else
      cout << "Unknown phone type.Using default ." << endl;
  }
}

// main function:read a person based on user input;read a entire
// file(addressbook);write back to the same file.
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << "address_book_file" << endl;
    return -1;
  }
  tutorial::AddressBook address_book;
  {
    // read the existing address book
    fstream input(argv[1], ios::in | ios::binary);
    if (!input) {
      cout << argv[1] << ": File not found.  Creating a new file." << endl;
    } else if (!address_book.ParseFromIstream(&input)) {
      cerr << "Failed to parse address book." << endl;
      return -1;
    }
  }

  // add an address.
  PromptForAddress(address_book.add_person());

  {
    // write // Write the new address book back to disk.
    fstream output(argv[1], ios::out | ios::binary);
    if (!address_book.SerializeToOstream(&output)) {
      cerr << "Failed to write address book." << endl;
      return -1;
    }
  }
  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}