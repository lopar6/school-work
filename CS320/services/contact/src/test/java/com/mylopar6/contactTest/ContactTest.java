package com.mylopar6.contact;

import org.junit.Test;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.lang.reflect.Executable;

public class ContactTest {

    public ContactTest(){}

    @Test
    public void shouldHaveUniqueId()
    {
        Contact Contact1 = new Contact("john", "smith", "1234567890", "123 address" );
        Contact Contact2 = new Contact("jimmy", "johns", "1234567890", "125 nono, 3" );
        assert(Contact1.getId() != Contact2.getId());

        boolean isMatch = false;
        String [] ContactIdArray = new String [100];
        Contact TempContact = new Contact("john", "smith", "1234567890", "123 address" );
        ContactIdArray[0] = TempContact.getId();
        for (int i = 0; i < 100; i++) {
            TempContact = new Contact("john", "smith", "1234567890", "123 address" );
            for ( int j = 0; j < ContactIdArray.length; j++ ) {
                if (TempContact.getId() == ContactIdArray[j]) {
                    isMatch = true;
                }
            } 
            ContactIdArray[i] = TempContact.getId();;
        }
        assert(!isMatch);
    }

    @Test
    public void shouldHave10CharFirstName()
    {
        RuntimeException exception = assertThrows(RuntimeException.class, () -> { 
            Contact contact1 = new Contact("12345678910", "smith", "1234567890", "123 address" );
        });
        String message = exception.getMessage();
        assert( message == "Names must be 10 characters or less");

        RuntimeException exception2 = assertThrows(RuntimeException.class, () -> { 
            Contact contact2 = new Contact("1234567891", "smith", "1234567890", "123 address" );
            contact2.setFirstName("12345678910");
        });
        message = exception2.getMessage();
        assert( message == "Names must be 10 characters or less");

        RuntimeException exception3 = assertThrows(RuntimeException.class, () -> { 
            Contact contact3 = new Contact(null , "smith", "1234567890", "123 address" );
        });
        message = exception3.getMessage();
        assert( message == "Names must be 10 characters or less");

        RuntimeException exception4 = assertThrows(RuntimeException.class, () -> { 
            Contact contact4 = new Contact("1234567891", "smith", "1234567890", "123 address" );
            contact4.setFirstName(null);
        });
        message = exception4.getMessage();
        assert( message == "Names must be 10 characters or less");
    }

    @Test
    public void shouldHave10CharLastName()
    {
        RuntimeException exception = assertThrows(RuntimeException.class, () -> { 
            Contact contact1 = new Contact("john", "12345678910", "1234567890", "123 address" );
        });
        String message = exception.getMessage();
        assert( message == "Names must be 10 characters or less");

        RuntimeException exception2 = assertThrows(RuntimeException.class, () -> { 
            Contact contact2 = new Contact("john", "smith", "1234567890", "123 address" );
            contact2.setLastName("12345678910");
        });
        message = exception2.getMessage();
        assert( message == "Names must be 10 characters or less");

        RuntimeException exception3 = assertThrows(RuntimeException.class, () -> { 
            Contact contact3 = new Contact("john" , null , "1234567890", "123 address" );
        });
        message = exception3.getMessage();
        assert( message == "Names must be 10 characters or less");

        RuntimeException exception4 = assertThrows(RuntimeException.class, () -> { 
            Contact contact4 = new Contact("john", "smith", "1234567890", "123 address" );
            contact4.setLastName(null);
        });
        message = exception4.getMessage();
        assert( message == "Names must be 10 characters or less");
    }

    @Test
    public void shouldHave10DigitPhone ()
    {
        RuntimeException exception = assertThrows(RuntimeException.class, () -> { 
            Contact contact1 = new Contact("john", "smith", "12345678901", "123 address" );
        });
        String message = exception.getMessage();
        assert( message == "Phone number must be exactly 10 digits");

        
        RuntimeException exception2 = assertThrows(RuntimeException.class, () -> { 
            Contact contact2 = new Contact("john", "smith", "1234567890", "123 address" );
            contact2.setPhone("12345678910");
        });
        message = exception2.getMessage();
        assert( message == "Phone number must be exactly 10 digits");
        
        RuntimeException exception3 = assertThrows(RuntimeException.class, () -> { 
            Contact contact3 = new Contact("john", "smith", "123456789", "123 address" );
        });
        message = exception3.getMessage();
        assert( message == "Phone number must be exactly 10 digits");

        RuntimeException exception4 = assertThrows(RuntimeException.class, () -> { 
            Contact contact3 = new Contact("john", "smith", null, "123 address" );
        });
        message = exception4.getMessage();
        assert( message == "Phone number must be exactly 10 digits");

        RuntimeException exception5 = assertThrows(RuntimeException.class, () -> { 
            Contact contact5 = new Contact("john", "smith", "letters", "123 address" );
        });
        message = exception5.getMessage();
        assert( message == "Phone number must be exactly 10 digits");
    
    }

    @Test
    public void shouldHave30CharAddress ()
    {
        RuntimeException exception = assertThrows(RuntimeException.class, () -> { 
            Contact contact = new Contact("john", "smith", "1234567890", "1234567890123456789012345678910" ); // 31 chars
        });
        String message = exception.getMessage();
        assert( message  == "Must have address with less than 30 characters");

        RuntimeException exception2 = assertThrows(RuntimeException.class, () -> { 
            Contact contact2 = new Contact("john", "smith", "1234567890", null );
        });
        String message2 = exception2.getMessage();
        assert( message2 == "Must have address with less than 30 characters");

        RuntimeException exception3 = assertThrows(RuntimeException.class, () -> { 
            Contact contact3 = new Contact("john", "smith", "1234567890", "123456789012345678901234567891" ); 
            contact3.setAddress("1234567890123456789012345678910"); //31 chars
        });
        String message3 = exception3.getMessage();
        assert( message3 == "Must have address with less than 30 characters");

        RuntimeException exception4 = assertThrows(RuntimeException.class, () -> { 
            Contact contact4 = new Contact("john", "smith", "1234567890", "123456789012345678901234567891" ); 
            contact4.setAddress(null);
        });
        String message4 = exception4.getMessage();
        assert( message4 == "Must have address with less than 30 characters");
    }    
}
