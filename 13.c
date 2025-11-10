// nano address_book.sh

// Ctrl + O   → Save
// Enter      → Confirm filename
// Ctrl + X   → Exit nano

// chmod +x address_book.sh
// ./address_book.sh


#!/bin/bash


ADDRESS_BOOK="address_colon.txt"


if [ ! -f "$ADDRESS_BOOK" ]; then
    touch "$ADDRESS_BOOK"
fi




show_menu() {
    echo "=================================="
    echo "        ADDRESS BOOK MENU"
    echo "=================================="
    echo "1. Search Address Book"
    echo "2. Add New Entry"
    echo "3. Remove or Edit Entry"
    echo "4. Display All Entries"
    echo "5. Quit"
    echo "=================================="
    echo -n "Enter your choice [1-5]: "
}


add_entry() {
    echo "---- Add New Entry ----"
    read -p "Enter ID: " id


    if grep -q "^$id:" "$ADDRESS_BOOK"; then
        echo "Error: ID already exists!"
        return
    fi

    read -p "Enter Name: " name
    read -p "Enter Address: " address
    read -p "Enter Phone: " phone

    echo "${id}:${name}:${address}:${phone}" >> "$ADDRESS_BOOK"
    echo "Entry added successfully!"
}


search_entry() {
    echo "---- Search Address Book ----"
    read -p "Enter ID, Name, or Phone to search: " key
    echo "----------------------------------"
    grep -i "$key" "$ADDRESS_BOOK" | awk -F ":" '{printf "ID: %s | Name: %s | Address: %s | Phone: %s\n", $1, $2, $3, $4}'
    if [ ${PIPESTATUS[0]} -ne 0 ]; then
        echo "No matching records found."
    fi
    echo "----------------------------------"
}


display_entries() {
    echo "---- Displaying All Entries ----"
    if [ ! -s "$ADDRESS_BOOK" ]; then
        echo "No records to display."
        return
    fi
    awk -F ":" '{printf "ID: %s | Name: %s | Address: %s | Phone: %s\n", $1, $2, $3, $4}' "$ADDRESS_BOOK"
}


remove_or_edit_entry() {
    echo "---- Remove or Edit Entry ----"
    read -p "Enter ID of entry to modify: " id

    if ! grep -q "^$id:" "$ADDRESS_BOOK"; then
        echo "Error: No entry found with ID $id."
        return
    fi

    echo "1. Remove entry"
    echo "2. Edit entry"
    read -p "Enter your choice [1-2]: " choice

    case $choice in
        1)
            grep -v "^$id:" "$ADDRESS_BOOK" > temp.txt && mv temp.txt "$ADDRESS_BOOK"
            echo "Entry with ID $id removed successfully!"
            ;;
        2)
            grep -v "^$id:" "$ADDRESS_BOOK" > temp.txt && mv temp.txt "$ADDRESS_BOOK"
            echo "Enter new details for ID $id:"
            read -p "Enter Name: " name
            read -p "Enter Address: " address
            read -p "Enter Phone: " phone
            echo "${id}:${name}:${address}:${phone}" >> "$ADDRESS_BOOK"
            echo "Entry updated successfully!"
            ;;
        *)
            echo "Invalid choice."
            ;;
    esac
}



while true; do
    show_menu
    read choice
    case $choice in
        1) search_entry ;;
        2) add_entry ;;
        3) remove_or_edit_entry ;;
        4) display_entries ;;
        5) echo "Exiting Address Book. Goodbye!"; exit 0 ;;
        *) echo "Invalid choice. Please select between 1-5." ;;
    esac
done
