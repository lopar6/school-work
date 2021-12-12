package com.mylopar6.contact;

import java.util.HashMap;

public class ContactService {
  
  public ContactService() {}
  
  private HashMap<String, Contact> contacts = new HashMap<String, Contact>();
  private String [] CONTACT_TYPES = {
    "firstName",
    "lastName",
    "phone",
    "address"
  };
  private int CONTACT_TYPES_LENGTH = 4;
  

  public String addContact(String firstName, String lastName, String number, String address ) {
    Contact newContact = new Contact(firstName, lastName, number, address);
    String id = newContact.getId();
    contacts.put(id, newContact);
    return id;
  }

  public void deleteContactById(String id){
    contacts.remove(id);
  }

  public void updateContactById(String id, String field, String value){
    Contact contact = contacts.get(id);
    if (contact == null) {
      throw new RuntimeException("Contact not found");
    }
    boolean isValidField = false;
    for (int i=0; i < CONTACT_TYPES_LENGTH; i++) {
      if (field == CONTACT_TYPES[i]){
        isValidField = true;
      }
    }
    if (!isValidField) {
      throw new RuntimeException("Invalid field");
    }
    switch (field) {
      case "firstName": contact.setFirstName(value);
        break;
      case "lastName": contact.setLastName(value);
        break;
      case "phone": contact.setPhone(value);
        break; 
      case "address": contact.setAddress(value);
        break;
    }
    contacts.put(id, contact);
  }

  public Contact getContactById(String id) {
    Contact contact = contacts.get(id);
    if (contact == null) {
      throw new RuntimeException("Contact not found");
    }
    return contacts.get(id);
  }
}
