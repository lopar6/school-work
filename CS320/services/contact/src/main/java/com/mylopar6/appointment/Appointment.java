package com.mylopar6.appointment;

import java.util.Random;
import java.util.Date;

public class Appointment {

  private String ID;
  private Date date;
  private String description;

  public Appointment(Date date, String description) throws RuntimeException {
    this.ID = generateId();
    setDate(date);
    setDescription(description);
  }

  public String getID() {
    return this.ID;
  }

  public Date getDate() {
    return this.date;
  }

  public void setDate(Date date) {
    validateDate(date);
    this.date = date;
  }

  public String getDescription() {
    return this.description;
  }

  public void setDescription(String description) {
    validateDescription(description);
    this.description = description;
  }
  
  private String generateId(){
    Random random = new Random();
    String Id = String.valueOf(random.nextInt(11));
    return Id;
  } 

  private void validateDescription(String inputDescription) {
    if (inputDescription == null || inputDescription.length() > 50) {
      throw new RuntimeException("Must have a description of less than 50 characters");
    }
  }

  private void validateDate(Date inputDate) {
    if (inputDate == null || inputDate.before(new Date())) {
      throw new RuntimeException("Must have a date in the future");
    }
  }
}
