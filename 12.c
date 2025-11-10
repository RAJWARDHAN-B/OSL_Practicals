// nano phonebook.sh

// Ctrl + O   → Save
// Enter      → Confirm filename
// Ctrl + X   → Exit nano

// chmod +x phonebook.sh

// ./phonebook.sh

#!/bin/bash

PHONEBOOK="phonebook.txt"


if [ ! -f "$PHONEBOOK" ]; then
    touch "$PHONEBOOK"
fi


show_menu() {
    echo "=============================="
    echo "       PHONE BOOK MENU"
    echo "=============================="
    echo "1. Add a new entry"
    echo "2. Search by name or phone number"
    echo "3. Sort by last name"
    echo "4. Delete an entry"
    echo "5. Quit"
    echo "=============================="
    echo -n "Enter your choice [1-5]: "
}


add_entry() {
    read -p "Enter First Name: " fname
    read -p "Enter Last Name: " lname
    read -p "Enter Phone Number: " phone
    echo -e "${fname}\t${lname}\t${phone}" >> "$PHONEBOOK"
    echo "Entry added successfully!"
}


search_entry() {
    read -p "Enter name or phone number to search: " key
    echo "--------------------------------"
    grep -i "$key" "$PHONEBOOK" | awk -F "\t" '{printf "First Name: %s | Last Name: %s | Phone: %s\n", $1, $2, $3}'
    if [ $? -ne 0 ]; then
        echo "No matching records found."
    fi
    echo "--------------------------------"
}


sort_phonebook() {
    sort -k2 "$PHONEBOOK" -o "$PHONEBOOK"
    echo "Phonebook sorted by last name successfully!"
}


delete_entry() {
    read -p "Enter phone number to delete: " phone
    if grep -q "$phone" "$PHONEBOOK"; then
        grep -v "$phone" "$PHONEBOOK" > temp.txt && mv temp.txt "$PHONEBOOK"
        echo "Entry deleted successfully!"
    else
        echo "No entry found with that phone number."
    fi
}


while true; do
    show_menu
    read choice
    case $choice in
        1) add_entry ;;
        2) search_entry ;;
        3) sort_phonebook ;;
        4) delete_entry ;;
        5) echo "Exiting Phonebook. Goodbye!"; exit 0 ;;
        *) echo "Invalid choice. Please select 1-5." ;;
    esac
done
