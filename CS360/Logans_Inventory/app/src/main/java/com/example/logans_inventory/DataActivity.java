package com.example.logans_inventory;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.Icon;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

enum row {
    QUANTITY,
    ITEM
}

public class DataActivity extends AppCompatActivity {

    InventoryDatabase db;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        db = new InventoryDatabase(getApplicationContext());
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_data);

        Button addItemButton = findViewById(R.id.buttonAddItem);
        addItemButton.setOnClickListener(addItemButtonListener);
        loadInventoryTable();
    }

    @SuppressLint({"SetTextI18n", "UseCompatLoadingForDrawables"})
    private void loadInventoryTable() {
        TableLayout inventoryTable = findViewById(R.id.inventoryTable);

        TableRow headerRow = findViewById(R.id.header_row);
        inventoryTable.removeAllViews();
        inventoryTable.addView(headerRow);

        Drawable border = getResources().getDrawable(R.drawable.value_cell_border, getTheme());

        Item[] items = db.getAllItems();

        for(Item item :items) {
            TableRow tableRow = new TableRow(this);
            tableRow.setLayoutParams(
                    new TableRow.LayoutParams(
                            TableRow.LayoutParams.MATCH_PARENT,
                            TableRow.LayoutParams.WRAP_CONTENT
                    )
            );
            tableRow.setBackground(border);

            EditText quantityView = new EditText(this);
            quantityView.setText(item.quantity.toString());
            quantityView.setTextAlignment(View.TEXT_ALIGNMENT_CENTER);
            quantityView.setBackground(border);
            quantityView.setPaddingRelative(20, 20, 20, 20);
            quantityView.addTextChangedListener(textChangedListener(item, row.QUANTITY));
            tableRow.addView(quantityView);

            EditText nameView = new EditText(this);
            nameView.setText(item.name);
            nameView.setBackground(border);
            nameView.setPaddingRelative(20, 20, 20, 20);
            nameView.addTextChangedListener(textChangedListener(item, row.ITEM));
            tableRow.addView(nameView);

            ImageButton trashButton = new ImageButton(this);
            Drawable trashIcon;
            trashIcon = getResources().getDrawable(R.drawable.ic_baseline_delete_24, getTheme());
            trashButton.setOnClickListener(deleteItemButtonListener(item.id));
            trashButton.setImageDrawable(trashIcon);
            tableRow.addView(trashButton);

            inventoryTable.addView(tableRow);
        }
    }

    private TextWatcher textChangedListener(Item item, row type) {
        TextWatcher textWatcher = new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {
            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
            }

            @Override
            public void afterTextChanged(Editable editable) {
                String text = editable.toString();
                char[] chars = text.toCharArray();
                if(type == row.QUANTITY) {
//                    text.replaceFirst("^0+(?!$)", "");
                    if(!text.equals("")) {
                        for(char textChar: chars) {
                            if (!Character.isDigit(textChar)){
                                editable.clear();
                                return;
                            }
                        }
                    } else {
                        text = "0";
                        editable.append('0');
                        return;
                    }
                    item.quantity = Integer.valueOf(text);
                }
                if(type == row.ITEM) {
                    item.name = text;
                }
                db.editItem(item);
            }
        };
        return textWatcher;
    };

    private final View.OnClickListener addItemButtonListener = new View.OnClickListener() {
        public void onClick(View v) {
            db.addItem("", 0);
            loadInventoryTable();
        }
    };

//  TODO fix bug col deleted
    private View.OnClickListener deleteItemButtonListener (long itemId) {
        return v -> {
            TableRow tableRow = (TableRow) v.getParent();
            TableLayout tableLayout = (TableLayout) tableRow.getParent();
            tableLayout.removeView(tableRow);
            db.deleteItem(itemId);
        };
    }

}