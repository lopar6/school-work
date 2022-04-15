package com.example.logans_inventory;

import java.util.jar.Attributes;

public class Item {
    Long id;
    Integer quantity;
    String name;

    Item(Long id, Integer quantity, String name){
        this.id = id;
        this.quantity = quantity;
        this.name = name;
    }
}
