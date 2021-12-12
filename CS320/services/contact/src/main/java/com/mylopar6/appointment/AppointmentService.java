package com.mylopar6.appointment;

import java.util.HashMap;
import java.util.Date;

public class AppointmentService {
  
  public AppointmentService() {}
  
  private HashMap<String, Appointment> appointments = new HashMap<String, Appointment>();

  public String addAppointment (Date date, String description) {
    Appointment newAppointment = new Appointment(date, description);
    String id = newAppointment.getID();
    appointments.put(id, newAppointment);
    return id;
  }

  public void deleteAppointmentById(String id){
    appointments.remove(id);
  }

  public void updateDescriptionById(String id, String description){
    Appointment appointment = appointments.get(id);
    if (appointment == null) {
      throw new RuntimeException("Appointment not found");
    }
    appointment.setDescription(description);
    appointments.put(id, appointment);
  }

  public void updateDateById(String id, Date date){
    Appointment appointment = appointments.get(id);
    if (appointment == null) {
      throw new RuntimeException("Appointment not found");
    }
    appointment.setDate(date);
    appointments.put(id, appointment);
  }

  public Appointment getAppointmentById(String id) {
    Appointment appointment = appointments.get(id);
    if (appointment == null) {
      throw new RuntimeException("Appointment not found");
    }
    return appointments.get(id);
  }
}

