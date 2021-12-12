package com.mylopar6.task;


import org.junit.Test;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class TaskServiceTest {

  @Test
  public void shouldMakeNewTask() {
    TaskService tasks = new TaskService();
    Task originalTask = new Task("dishes", "Scrub, rinse, then dry");
    String id = tasks.addTask("dishes", "Scrub, rinse, then dry");
    Task newTask = tasks.getTaskById(id);
    assert(originalTask.getID() != newTask.getID());
    assert(originalTask.getName() == newTask.getName());
    assert(originalTask.getDescription() == newTask.getDescription());
  }

  @Test
  public void shouldOnlyRetrieveCorrectTask() {
    TaskService tasks = new TaskService();
    Task originalTask = new Task("dishes", "Scrub, rinse, then dry");
    String falseId = "123456789";
    RuntimeException exception = assertThrows(RuntimeException.class, () -> { 
      tasks.getTaskById(falseId);
    });
    String message = exception.getMessage();
    assert( message == "Task not found");
  }

  @Test
  public void shouldDeleteTask()
  {
    TaskService tasks = new TaskService();
    String id = tasks.addTask("dishes", "Scrub, rinse, then dry");
    tasks.deleteTaskById(id);
    RuntimeException exception = assertThrows(RuntimeException.class, () -> { 
      tasks.getTaskById(id);
    });
    String message = exception.getMessage();
    assert( message == "Task not found");
  }

  @Test
  public void shouldUpdateTaskById() 
  {
    TaskService tasks = new TaskService();
    String id = tasks.addTask("dishes", "Scrub, rinse, then dry");
    
    tasks.updateTaskById(id, "name", "jimmy");
    Task task = tasks.getTaskById(id);
    assert(task.getName() == "jimmy");

    tasks.updateTaskById(id, "description", "Dont even bother");
    task = tasks.getTaskById(id);
    assert(task.getDescription() == "Dont even bother");
    
    String falseId = "1234567890";
    RuntimeException exception = assertThrows(RuntimeException.class, () -> { 
      tasks.getTaskById(falseId);
    });
    String message = exception.getMessage();
    assert( message == "Task not found");
  }
}
