package com.mylopar6.contact;

import org.junit.Test;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class ContactServiceTest {
  
  @Test
  public void ShouldMakeNewContact()
  {
    ContactService contacts = new ContactService();
    Contact originalContact = new Contact("john", "smith", "1234567890", "123 address");
    String id = contacts.addContact("john", "smith", "1234567890", "123 address" );
    Contact newContact = contacts.getContactById(id);
    assert(originalContact.getFirstName() == newContact.getFirstName());
    assert(originalContact.getLastName() == newContact.getLastName());
    assert(originalContact.getAddress() == newContact.getAddress());
    assert(originalContact.getId() != newContact.getId());
  }

  @Test
  public void shouldDeleteContact()
  {
    ContactService contacts = new ContactService();
    String id = contacts.addContact("john", "smith", "1234567890", "123 address" );
    contacts.deleteContactById(id);
    RuntimeException exception = assertThrows(RuntimeException.class, () -> { 
      contacts.getContactById(id);
    });
    String message = exception.getMessage();
    assert( message == "Contact not found");
  }

  @Test
  public void shouldUpdateContactById() 
  {
    ContactService contacts = new ContactService();
    String id = contacts.addContact("john", "smith", "1234567890", "123 address" );
    
    contacts.updateContactById(id, "firstName", "jimmy");
    Contact contact = contacts.getContactById(id);
    assert(contact.getFirstName() == "jimmy");

    contacts.updateContactById(id, "lastName", "Slim-jim");
    contact = contacts.getContactById(id);
    assert(contact.getLastName() == "Slim-jim");

    contacts.updateContactById(id, "phone", "0987654321");
    contact = contacts.getContactById(id);
    assert(contact.getPhone() == "0987654321");

    contacts.updateContactById(id, "address", "1987 Skeet Street");
    contact = contacts.getContactById(id);
    assert(contact.getAddress() == "1987 Skeet Street");
  }

}
