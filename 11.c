// nano address_book.sh

// Ctrl + O   → Save
// Enter      → Confirm filename
// Ctrl + X   → Exit nano

// chmod +x address_book.sh

// ./address_book.sh

#!/bin/bash

ADDRESS_BOOK = "address.txt"

if [ ! -f "$ADDRESS_BOOK" ]; then
    touch "$ADDRESS_BOOK"
fi 

show_menu() {
    echo "=============================="
    echo "     ADDRESS BOOK SYSTEM"
    echo "=============================="
    echo "1. Search Address Book"
    echo "2. Add Address Book Entry"
    echo "3. Remove Address Book Entry"
    echo "4. Quit"
    echo "=============================="
    echo -n "Enter your choice [1-4]: "
}

add_entry() {
    read -p "Enter ID: " id 

    if grep -q "^$id;" "ADDRESS_BOOK;" then 
        echo "Error : ID already exists !"
        return
    fi 

    read -p "Enter Name: " name
    read -p "Enter Phone: " phone
    echo "$id;$name;$phone" >> "$ADDRESS_BOOK"
    echo "Entry added successfully!"
    
    
}

remove_entry() {
    read -p "Enter ID of entry to remove: " id
    if grep -q "^$id;" "$ADDRESS_BOOK"; then
        grep -v "^$id;" "$ADDRESS_BOOK" > temp.txt && mv temp.txt "$ADDRESS_BOOK"
        echo "Entry with ID $id removed successfully!"
    else
        echo "Error: No entry found with ID $id."
    fi
}

while true; do
    show_menu
    read choice
    case $choice in
        1) search_entry ;;
        2) add_entry ;;
        3) remove_entry ;;
        4) echo "Exiting Address Book. Goodbye!"; exit 0 ;;
        *) echo "Invalid choice. Please select 1-4." ;;
    esac
done