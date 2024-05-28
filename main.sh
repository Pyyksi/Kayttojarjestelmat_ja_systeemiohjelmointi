#!/bin/bash

# Create a temporary file to hold the modified content

# Define arrays for different values
content_array=("#define SYS_getreadcount 22"
 "int getreadcount(void);"
 "extern int sys_getreadcount(void);"
 "[SYS_getreadcount]   sys_getreadcount,"
 "  int readcount;"
 "  p->readcount = 0;"
 "  num = curproc->tf->eax;"
 "  if(num == SYS_read) {"
 "  curproc-> readcount++;}"
 "SYSCALL(getreadcount)"
 "int readcount = 0;"
 "  readcount++;"
 "extern int readcount;"
 "int"
 "sys_getreadcount(void)"
 "{"
 "  return readcount;"
 "}"
 )
insert_after_line_array=(22 25 105 129 51 90 137 138 139 31 67 78 91 92 93 94 95 96)
num_new_lines_array=(1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1)
line_number_array=(23 26 106 130 52 91 138 139 140 32 68 79 92 93 94 95 96 97)
file_path_array=("syscall.h" "user.h" "syscall.c" "syscall.c" "proc.h" "proc.c" "syscall.c" "syscall.c" "syscall.c" "usys.S" "sysfile.c" "sysfile.c" "sysproc.c" "sysproc.c" "sysproc.c" "sysproc.c" "sysproc.c" "sysproc.c")

# Iterate over the arrays
for ((i=0; i<${#content_array[@]}; i++)); do

    # Create a temporary file to hold the modified content
    temp_file="temp_file.txt"

    # Assign values from arrays for current iteration
    content="${content_array[i]}"
    line_number="${line_number_array[i]}"
    insert_after_line="${insert_after_line_array[i]}"
    num_new_lines="${num_new_lines_array[i]}"
    file_path="${file_path_array[i]}"

    # Copy the lines before the insertion point to the temporary file
    head -n "$insert_after_line" "$file_path" > "$temp_file"

    # Add the specified number of empty lines
    for ((j=0; j<num_new_lines; j++)); do
        echo >> "$temp_file"
    done

    # Copy the remaining lines from the insertion point to the end of the file to the temporary file
    tail -n +$((insert_after_line + 1)) "$file_path" >> "$temp_file"

    # Overwrite the original file with the modified content
    mv "$temp_file" "$file_path"

    # Write the content to the specified line
    sed -i "${line_number}s/.*/$content/" "$file_path"

    echo "Content has been written to line $line_number of $file_path"
done
