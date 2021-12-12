package com.mylopar6.task;

import org.junit.Test;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.lang.reflect.Executable;

public class TaskTest {

  public TaskTest() {}

  // Task fields 'name' and description are required as there is only one contructor

  @Test 
  public void testTask() {
    Task task = new Task("dishes", "Scrub, rinse, then dry");
    assert(task.getID() != null);
    assert(task.getName() == "dishes");
    assert(task.getDescription() == "Scrub, rinse, then dry");
  }

  @Test
  public void shouldHaveUniqueId() {
      Task Task1 = new Task("dishes", "Scrub, rinse, then dry");
      Task Task2 = new Task("laundry", "load, turn on, ah forget it");
      assert(Task1.getID() != Task2.getID());

      boolean isMatch = false;
      String [] TaskIdArray = new String [100000];
      Task TempTask = new Task("dishes", "Scrub, rinse, then dry");
      TaskIdArray[0] = TempTask.getID();
      for (int i = 0; i < 100000; i++) {
          TempTask = new Task("laundry", "load, turn on, ah forget it");
          for ( int j = 0; j < TaskIdArray.length; j++ ) {
              if (TempTask.getID() == TaskIdArray[j]) {
                  isMatch = true;
              }
          } 
          TaskIdArray[i] = TempTask.getID();;
      }
      assert(!isMatch);
  }

  @Test 
  public void shouldHave10CharName() {

    RuntimeException exception = assertThrows(RuntimeException.class, () -> { 
      Task task = new Task("12345678910", "Scrub, rinse, then dry");
    });
    String message = exception.getMessage();
    assert( message == "Name must be 10 characters or less");

    exception = assertThrows(RuntimeException.class, () -> { 
      Task task = new Task("dishes", "Scrub, rinse, then dry");
      task.setName("12345678910");
    });
    message = exception.getMessage();
    assert( message == "Name must be 10 characters or less");

    exception = assertThrows(RuntimeException.class, () -> { 
      Task task = new Task(null, "Scrub, rinse, then dry");
    });
    message = exception.getMessage();
    assert( message == "Name must be 10 characters or less");

    exception = assertThrows(RuntimeException.class, () -> { 
      Task task = new Task("dishes", "Scrub, rinse, then dry");
      task.setName(null);
    });
    message = exception.getMessage();
    assert( message == "Name must be 10 characters or less");
  }

  @Test 
  public void shouldHave50CharDescription() {

    RuntimeException exception = assertThrows(RuntimeException.class, () -> { 
      Task task = new Task("dishes", "123456789012345678901234567890123456789012345678910"); //51
    });
    String message = exception.getMessage();
    assert( message == "Description must be 50 charaters or less");

    Task task = new Task("dishes", "Scrub, rinse, then dry");
    exception = assertThrows(RuntimeException.class, () -> { 
      task.setDescription("123456789012345678901234567890123456789012345678910");
    });
    message = exception.getMessage();
    assert( message == "Description must be 50 charaters or less");

    exception = assertThrows(RuntimeException.class, () -> { 
      Task task1 = new Task("dishes", null);
    });
    message = exception.getMessage();
    assert( message == "Description must be 50 charaters or less");

    exception = assertThrows(RuntimeException.class, () -> { 
      task.setDescription(null);
    });
    message = exception.getMessage();
    assert( message == "Description must be 50 charaters or less");
  }
}