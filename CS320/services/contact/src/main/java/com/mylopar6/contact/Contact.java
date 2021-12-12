package com.mylopar6.contact;

import java.util.Random;

public class Contact 
{
    String id;
    String firstName;
    String lastName;
    String phone;
    String address;

    public Contact(String firstName, String lastName, String phone, String address) throws RuntimeException {
        this.id = generateId();
        setFirstName(firstName);
        setLastName(lastName);
        setPhone(phone);
        setAddress(address);
    }

    public String getId() {
        return this.id;
    }

    public String getFirstName() {
        return this.firstName;
    }

    public void setFirstName(String firstName) {
        validateInput(firstName, 10, false);
        this.firstName = firstName;
    }

    public String getLastName() {
        return this.lastName;
    }

    public void setLastName(String lastName) {
        validateInput(lastName, 10, false);
        this.lastName = lastName;
    }

    public String getPhone() {
        return this.phone;
    }

    public void setPhone(String phone) {
        validateInput(phone, 10, true);
        this.phone = phone;
    }

    public String getAddress() {
        return this.address;
    }

    public void setAddress(String address) {
        validateInput(address, 30, false);
        this.address = address;
    }

    private String generateId(){
        Random random = new Random();
        String Id = String.valueOf(random.nextInt(11));
        return Id;
    }

    private void validateInput(String input, int Length, Boolean exactly){
        String errorResponse; 
        boolean isPhone = false;
        if (exactly && Length == 10) {
            isPhone = true;
            errorResponse = "Phone number must be exactly 10 digits";
        } else if (Length == 10) {
            errorResponse = "Names must be 10 characters or less";
        } else {
            errorResponse = "Must have address with less than 30 characters";
        }
        
        if (input == null || input.length() > Length){
            throw new RuntimeException(errorResponse);
        }
        if (exactly && input.length() < Length) {
            throw new RuntimeException(errorResponse);
        } 
        if (isPhone){
            try {
                Integer.parseInt(input);
            } catch (Exception e) {
                throw new RuntimeException(errorResponse);
            }
        }
    }
}
