package com.mylopar6.task;

import java.util.HashMap;

public class TaskService {
  
  public TaskService() {}
  
  private HashMap<String, Task> tasks = new HashMap<String, Task>();
  private String [] TASK_TYPES = {
    "name",
    "description"
  };
  private int TASK_TYPES_LENGTH = 2;
  

  public String addTask (String name, String description) {
    Task newTask = new Task(name, description);
    String id = newTask.getID();
    tasks.put(id, newTask);
    return id;
  }

  public void deleteTaskById(String id){
    tasks.remove(id);
  }

  public void updateTaskById(String id, String field, String value){
    Task task = tasks.get(id);
    if (task == null) {
      throw new RuntimeException("Task not found");
    }
    boolean isValidField = false;
    for (int i=0; i < TASK_TYPES_LENGTH; i++) {
      if (field == TASK_TYPES[i]){
        isValidField = true;
      }
    }
    if (!isValidField) {
      throw new RuntimeException("Invalid field");
    }
    switch (field) {
      case "name": task.setName(value);
        break;
      case "description": task.setDescription(value);
        break;
    }
    tasks.put(id, task);
  }

  public Task getTaskById(String id) {
    Task task = tasks.get(id);
    if (task == null) {
      throw new RuntimeException("Task not found");
    }
    return tasks.get(id);
  }
}
