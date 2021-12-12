package com.mylopar6.task;

import java.util.Random;

public class Task {
  private String ID;
  private String name;
  private String description;

  public Task(String name, String description) {
    
    this.ID = generateID();
    setName(name);
    setDescription(description);
  }

  public String getID() {
    return this.ID;
  }

  public String getName() {
    return this.name;
  }

  public void setName(String name) {
    validateString(name, "name", 10);
    this.name = name;
  }

  public String getDescription() {
    return this.description;
  }

  public void setDescription(String description) {
    validateString(description, "description", 50);
    this.description = description;
  }

  private String generateID(){
    Random random = new Random();
    String Id = String.valueOf(random.nextInt(11));
    return Id;
  }

  private void validateString(String input, String type, int maxLength){
    String errorMessage;
    if (type == "name") {
      errorMessage = "Name must be 10 characters or less";
    } else if (type == "description") {
      errorMessage = "Description must be 50 charaters or less";
    } else {
      errorMessage = "type not found";
      throw new RuntimeException(errorMessage);
    }

    if (input == null || input.length() > maxLength){
      throw new RuntimeException(errorMessage);
    }
  }
}
