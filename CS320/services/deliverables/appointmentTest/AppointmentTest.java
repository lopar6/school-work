package com.mylopar6.appointment;

import org.junit.Test;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.lang.reflect.Executable;
import java.util.Date;

public class AppointmentTest {
  Date DATE_2031 = new Date(1938441140000L);
  Date DATE_2032 = new Date(1959441140000L); 
  Date DATE_2020 = new Date(1599441140790L); 

  public AppointmentTest() {
    Appointment appointment = new Appointment(DATE_2031, "Meeting with a real person");
    assert(appointment.getID() != null);
    assert(appointment.getDate() == DATE_2031);
    assert(appointment.getDescription() == "Meeting with a real person");
  }

  @Test
  public void shouldHaveUniqueId() {
    Appointment appointment1 = new Appointment(DATE_2031, "Meeting with a real person");
    Appointment appointment2 = new Appointment(DATE_2032, "Meeting with a FAKE person");
    assert(appointment1.getID() != appointment2.getID());

    boolean isMatch = false;
    String [] appointmentIDArray = new String [10000];
    Appointment tempAppointment = new Appointment(DATE_2031, "Meeting with a real person");
    appointmentIDArray[0] = tempAppointment.getID();
    for (int i = 0; i < 10000; i++) {
      tempAppointment = new Appointment(DATE_2031, "Meeting with a real person");
        for ( int j = 0; j < appointmentIDArray.length; j++ ) {
            if (tempAppointment.getID() == appointmentIDArray[j]) {
                isMatch = true;
            }
        } 
        appointmentIDArray[i] = tempAppointment.getID();;
    }
    assert(!isMatch);
  }

  @Test
  public void shouldHaveFutureDate() {

    String description = "I am a resonable description";

    RuntimeException exception = assertThrows(RuntimeException.class, () -> { 
      Appointment appointment = new Appointment(DATE_2020, description);
    });
    String message = exception.getMessage();
    assert( message  == "Must have a date in the future");

    exception = assertThrows(RuntimeException.class, () -> { 
      Appointment appointment = new Appointment(null, description);
    });
    message = exception.getMessage();
    assert( message  == "Must have a date in the future");

    Appointment appointment = new Appointment(DATE_2032, description);
    exception = assertThrows(RuntimeException.class, () -> { 
      appointment.setDate(DATE_2020);
    });
    
    message = exception.getMessage();
    assert( message  == "Must have a date in the future");


  }    

  @Test
  public void shouldHaveDescription () {

    String nullDescription = null;
    RuntimeException exception = assertThrows(RuntimeException.class, () -> { 
      Appointment appointment = new Appointment(DATE_2031, nullDescription);
    });
    String message = exception.getMessage();
    assert( message  == "Must have a description of less than 50 characters");

    String description51Char = "123456789012345678901234567890123456789012345678901"; // 51    
    exception = assertThrows(RuntimeException.class, () -> { 
      Appointment appointment = new Appointment(DATE_2031, description51Char);
    });
    message = exception.getMessage();
    assert( message  == "Must have a description of less than 50 characters");

    Appointment appointment = new Appointment(DATE_2031, "I am a resonable length");
    exception = assertThrows(RuntimeException.class, () -> { 
      appointment.setDescription(description51Char);
    });
    message = exception.getMessage();
    assert( message  == "Must have a description of less than 50 characters");
  }    
}
