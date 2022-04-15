package com.example.logans_inventory;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

public class InventoryDatabase extends SQLiteOpenHelper {

    private static final String DATABASE_NAME = "inventory.db";
    private static final int VERSION = 1;

    public InventoryDatabase(Context context) {
        super(context, DATABASE_NAME, null, VERSION);
    }

    private static final class UserTable {
        private static final String TABLE = "users";
        private static final String COL_ID = "_id";
        private static final String COL_USERNAME = "username";
        private static final String COL_PASSWORD = "password";
    }

    private static final class ItemTable {
        private static final String TABLE = "items";
        private static final String COL_ID = "_id";
        private static final String COL_NAME = "title";
        private static final String COL_QUANTITY = "quantity";
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL("create table " + ItemTable.TABLE + " (" +
                ItemTable.COL_ID + " integer primary key autoincrement, " +
                ItemTable.COL_NAME + " text, " +
                ItemTable.COL_QUANTITY + " integer)");

        db.execSQL("create table " + UserTable.TABLE + " (" +
                UserTable.COL_ID + " integer primary key autoincrement, " +
                UserTable.COL_USERNAME + " text, " +
                UserTable.COL_PASSWORD + " text)");
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion,
                          int newVersion) {
        db.execSQL("drop table if exists " + ItemTable.TABLE);
        onCreate(db);
    }

    public boolean login(String userName, String password) {
        SQLiteDatabase db = getWritableDatabase();
        String checkLogin = "select count(*) from " + UserTable.TABLE +
                " where " + UserTable.COL_USERNAME + "=\"" + userName + "\"" +
                " and " + UserTable.COL_PASSWORD + "=\"" + password + "\"";
        Cursor cursor = db.rawQuery(checkLogin, new String[] { });
        if (cursor.moveToFirst()) {
            do {
                if (Integer.parseInt(cursor.getString(0)) > 0){
                    return true;
                }
            } while (cursor.moveToNext());
        }
        return false;
    }

    public void signUp(String userName, String password) {
        SQLiteDatabase db = getWritableDatabase();

        ContentValues values = new ContentValues();
        values.put(UserTable.COL_USERNAME, userName);
        values.put(UserTable.COL_PASSWORD, password);

        db.insert(UserTable.TABLE, null, values);
    }


    public void addItem(String name, Integer quantity) {
        SQLiteDatabase db = getWritableDatabase();

        ContentValues values = new ContentValues();
        values.put(ItemTable.COL_NAME, name);
        values.put(ItemTable.COL_QUANTITY, quantity);

        db.insert(ItemTable.TABLE, null, values);
    }

    public void deleteItem(Long id) {
        String idString = id.toString();
        SQLiteDatabase db = getWritableDatabase();
        db.delete(ItemTable.TABLE, ItemTable.COL_ID + "=" + id, null);
    }

    public void editItem(Item item) {
        Log.d("loglog", "editItem is getting called");
        SQLiteDatabase db = getWritableDatabase();
        ContentValues values = new ContentValues();
        values.put(ItemTable.COL_ID, item.id);
        values.put(ItemTable.COL_NAME, item.name);
        values.put(ItemTable.COL_QUANTITY, item.quantity);
        db.update(ItemTable.TABLE, values, ItemTable.COL_ID + "=" + item.id, null);
    }

    public Item[] getAllItems() {
        SQLiteDatabase db = getWritableDatabase();

        String numItems = "0";
        String selectAllSql = "select count(*) from " + ItemTable.TABLE;
        Cursor NumItemsCursor = db.rawQuery(selectAllSql, new String[] { });

        if (NumItemsCursor.moveToFirst()) {
            do {
                numItems = NumItemsCursor.getString(0);
            } while (NumItemsCursor.moveToNext());
        }
        NumItemsCursor.close();
        Item[] items = new Item[Integer.parseInt(numItems)];


        String getAllSql = "select * from " + ItemTable.TABLE;
        Cursor cursor = db.rawQuery(getAllSql, new String[] { });
        int index = 0;
        if (cursor.moveToFirst()) {
            do {
                long id = cursor.getLong(0);
                String name = cursor.getString(1);
                String quantity = cursor.getString(2);
                Item item = new Item(id, Integer.parseInt(quantity), name);

                items[index] = item;

                index++;
            } while (cursor.moveToNext());
        }
        cursor.close();

        return items;
    }
}