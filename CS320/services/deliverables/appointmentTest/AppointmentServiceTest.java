package com.mylopar6.appointment;

import org.junit.Test;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.lang.reflect.Executable;
import java.util.Date;

public class AppointmentServiceTest {
  Date DATE_2031 = new Date(1938441140000L);
  Date DATE_2032 = new Date(1959441140000L); 
  Date DATE_2020 = new Date(1599441140790L); 
  String DESCRIPTION = "I am a resonable description";


  @Test
  public void ShouldMakeNewAppointment()
  {
    AppointmentService appointments = new AppointmentService();
    Appointment originalAppointment = new Appointment(DATE_2031, DESCRIPTION);
    String id = appointments.addAppointment(DATE_2031, DESCRIPTION);
    Appointment newAppointment = appointments.getAppointmentById(id);
    assert(originalAppointment.getID() != newAppointment.getID());
    assert(originalAppointment.getDate() == newAppointment.getDate());
    assert(originalAppointment.getDescription() == newAppointment.getDescription());
  }

  @Test
  public void shouldDeleteAppointment()
  {
    AppointmentService appointments = new AppointmentService();
    String id = appointments.addAppointment(DATE_2031, DESCRIPTION);
    appointments.deleteAppointmentById(id);
    RuntimeException exception = assertThrows(RuntimeException.class, () -> { 
      appointments.getAppointmentById(id);
    });
    String message = exception.getMessage();
    assert( message == "Appointment not found");
  }

  @Test
  public void shouldUpdateAppointmentById() 
  {
    AppointmentService appointments = new AppointmentService();
    String id = appointments.addAppointment(DATE_2031, DESCRIPTION);
    
    Appointment appointment = appointments.getAppointmentById(id);
    appointments.updateDateById(id, DATE_2032);
    appointment = appointments.getAppointmentById(id);
    assert(appointment.getDate() == DATE_2032);

    appointments.updateDescriptionById(id, "new description");
    appointment = appointments.getAppointmentById(id);
    assert(appointment.getDescription() == "new description");
    
    String falseId = "1234567890";
    RuntimeException exception = assertThrows(RuntimeException.class, () -> { 
      appointments.getAppointmentById(falseId);
    });
    String message = exception.getMessage();
    assert( message == "Appointment not found");
  }
}
