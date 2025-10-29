//student database.sh
#!/bin/bash

DATABASE="students.txt"

# Function to add a new student
add_student() {
    echo "Enter student name:"
    read name
    echo "Enter student ID:"
    read id
    echo "Enter student grade:"
    read grade
    echo "$name,$id,$grade" >> "$DATABASE"
    echo "Student added successfully!"
}

# Function to view all students
view_students() {
    if [ ! -s "$DATABASE" ]; then
        echo "No students in the database."
        return
    fi
    echo "--- Student List ---"
    while IFS=',' read -r name id grade; do
        echo "Name: $name | ID: $id | Grade: $grade"
    done < "$DATABASE"
    echo "--------------------"
}

# Function to search for a student by ID
search_student() {
    echo "Enter student ID to search:"
    read search_id
    result=$(grep "^.*,$search_id,.*$" "$DATABASE")
    if [ -n "$result" ]; then
        IFS=',' read -r name id grade <<< "$result"
        echo "Student Found!"
        echo "Name: $name | ID: $id | Grade: $grade"
    else
        echo "Student with ID '$search_id' not found."
    fi
}

# Function to delete a student by ID
delete_student() {
    echo "Enter student ID to delete:"
    read del_id
    if grep -q "^.*,$del_id,.*$" "$DATABASE"; then
        grep -v "^.*,$del_id,.*$" "$DATABASE" > "$DATABASE.tmp"
        mv "$DATABASE.tmp" "$DATABASE"
        echo "Student with ID '$del_id' deleted successfully!"
    else
        echo "Student with ID '$del_id' not found."
    fi
}

# Main menu loop
while true; do
    echo "
--- Student Database Menu ---
1. Add a new student
2. View all students
3. Search for a student
4. Delete a student
5. Exit
"
    echo -n "Enter your choice: "
    read choice

    case $choice in
        1) add_student ;;
        2) view_students ;;
        3) search_student ;;
        4) delete_student ;;
        5)
            echo "Exiting... Goodbye!"
            exit 0
            ;;
        *)
            echo "Invalid choice. Please try again."
            ;;
    esac
done


// ---------------------------------------------------------------------------------------

//fibonaaci and palindrome
#!/bin/bash

// # Fibonacci function
fibonacci_series() {
    echo "Enter the number of elements to be printed: "
    read number

    # Check if input is a valid number
    if ! [[ "$number" =~ ^[0-9]+$ ]]; then
        echo "Not a valid number"
        return
    fi

    a=0  # first number in Fibonacci
    b=1  # second number in Fibonacci
    c=0

    echo "Fibonacci series:"
    echo "$a"
    echo "$b"

    for ((i=2; i<number; i++)); do
        c=$((a + b))  # Fibonacci logic
        a=$b
        b=$c
        echo "$c"
    done
}


// # Palindrome function
check_palindrome() {
    echo "Enter the string to check palindrome: "
    read original
    reversed=$(echo "$original" | rev)  # Reverse the string

    if [[ "$original" == "$reversed" ]]; then
        echo "Palindrome"
    else
        echo "Not Palindrome"
    fi
}

// # Asking the user for the operation
echo
echo "----------------------------------------------------------"
echo "Choose an operation:"
echo "a) Fibonacci"
echo "b) Palindrome"
echo "----------------------------------------------------------"
echo -n "Enter choice: "
read choice

case $choice in
    a|A) fibonacci_series ;;
    b|B) check_palindrome ;;
    *) echo "Invalid choice" ;;
esac



// ---------------------------------------------------------------------------------------
//AdressBook
#!/bin/bash

# Address Book File
ADDRESS_BOOK="address_book.txt"

# Create Address Book
create_address_book() {
    touch "$ADDRESS_BOOK"
    echo "Address book created!"
}

# View Address Book
view_address_book() {
    if [ -s "$ADDRESS_BOOK" ]; then
        echo -e "\nAddress Book:"
        cat "$ADDRESS_BOOK"
    else
        echo "Address book is empty."
    fi
}

# Insert a Record
insert_record() {
    echo "Enter name:"
    read name
    echo "Enter phone number:"
    read phone
    echo "Enter email:"
    read email
    echo "$name: $phone, $email" >> "$ADDRESS_BOOK"
    echo "Record for $name added."
}

# Delete a Record
delete_record() {
    echo "Enter name to delete:"
    read name
    sed -i "/^$name:/d" "$ADDRESS_BOOK"
    if [ $? -eq 0 ]; then
        echo "Record for $name deleted."
    else
        echo "No record found for $name."
    fi
}

# Modify a Record
modify_record() {
    echo "Enter name to modify:"
    read name
    if grep -i "^$name:" "$ADDRESS_BOOK"; then
        echo "Enter new phone number:"
        read phone
        echo "Enter new email:"
        read email
        sed -i "/^$name:/d" "$ADDRESS_BOOK"
        echo "$name: $phone, $email" >> "$ADDRESS_BOOK"
        echo "Record for $name modified."
    else
        echo "No record found for $name."
    fi
}

# Main Menu
while true; do
    echo -e "\nOptions:"
    echo "a) Create address book"
    echo "b) View address book"
    echo "c) Insert a record"
    echo "d) Delete a record"
    echo "e) Modify a record"
    echo "f) Exit"
    
    read -p "Select an option (a/b/c/d/e/f): " option

    case $option in
        a) create_address_book ;;
        b) view_address_book ;;
        c) insert_record ;;
        d) delete_record ;;
        e) modify_record ;;
        f) echo "Exiting the program."; exit 0 ;;
        *) echo "Invalid option. Please try again." ;;
    esac
done

// ---------------------------------------------------------------------------------------


//Employee.sh
#!/bin/bash

file="Employee_Record.txt"

# Check if file exists
if [ ! -e "$file" ]; then
    echo "File does not exist. Creating file: $file"
    touch "$file"
else
    echo "File already exists."
fi

# Main program loop
while true; do
    echo -e "\nEmployee Record Management\n"
    echo "1. View File"
    echo "2. Insert Record"
    echo "3. Delete Record"
    echo "4. Calculate Gross Pay"
    echo "5. Exit"

    read -p "Enter Choice: " choice

    case $choice in
        1)
            echo "Contents of $file:"
            cat "$file"
            ;;
        2)
            read -p "Enter Employee ID: " id
            read -p "Enter Name: " name
            read -p "Enter Designation: " designation
            read -p "Enter Basic Pay: " bpay

            echo "$id | $name | $designation | $bpay" >> "$file"
            echo "Record inserted successfully."
            ;;
        3)
            read -p "Enter Employee ID to delete: " id

            if grep -i "$id" "$file"; then
                sed -i "/$id/d" "$file"
                echo "Record deleted successfully."
            else
                echo "Record not found."
            fi
            ;;
        4)
            read -p "Enter Employee ID: " id

            if grep -i "$id" "$file"; then
                bpay=$(grep -i "$id" "$file" | awk -F ' \| ' '{print $4}')
                gross_pay=$(echo "scale=2; $bpay * 1.2" | bc)
                echo "Gross Pay for Employee ID $id: $gross_pay"
            else
                echo "Record not found."
            fi
            ;;
        5)
            echo "Exiting program."
            exit
            ;;
        *)
            echo "Invalid Choice"
            ;;
    esac
done