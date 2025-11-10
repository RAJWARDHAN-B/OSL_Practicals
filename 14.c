// nano file_management.sh

// Ctrl + O   → Save
// Enter      → Confirm filename
// Ctrl + X   → Exit nano

// chmod +x file_management.sh
// ./file_management.sh

#!/bin/bash

show_menu() {
    echo "=================================="
    echo "        FILE MANAGEMENT MENU"
    echo "=================================="
    echo "1. Test if file exists"
    echo "2. Read a file"
    echo "3. Delete a file"
    echo "4. Display list of files"
    echo "5. Quit"
    echo "=================================="
    echo -n "Enter your choice [1-5]: "
}


test_file() {
    read -p "Enter the filename to test: " filename
    if [ -f "$filename" ]; then
        echo "✅ File '$filename' exists."
    else
        echo "❌ File '$filename' does not exist."
    fi
}


read_file() {
    read -p "Enter the filename to read: " filename
    if [ -f "$filename" ]; then
        echo "------ File Contents: $filename ------"
        cat "$filename"
        echo "--------------------------------------"
    else
        echo "Error: File '$filename' not found!"
    fi
}


delete_file() {
    read -p "Enter the filename to delete: " filename
    if [ -f "$filename" ]; then
        read -p "Are you sure you want to delete '$filename'? (y/n): " confirm
        if [ "$confirm" == "y" ]; then
            rm "$filename"
            echo "File '$filename' deleted successfully!"
        else
            echo "Deletion cancelled."
        fi
    else
        echo "Error: File '$filename' not found!"
    fi
}


list_files() {
    echo "------ Files in Current Directory ------"
    ls -1
    echo "----------------------------------------"
}


while true; do
    show_menu
    read choice
    case $choice in
        1) test_file ;;
        2) read_file ;;
        3) delete_file ;;
        4) list_files ;;
        5) echo "Exiting File Management Program. Goodbye!"; exit 0 ;;
        *) echo "Invalid choice. Please select 1-5." ;;
    esac
done