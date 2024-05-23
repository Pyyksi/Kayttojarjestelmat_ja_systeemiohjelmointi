#! /bin/bash
#Viikko 8. Tehtävä 6.
#Searches and prints out the 10 most frequently used words of the input

#Test for the arguments
if [ $# -eq 0 ]
    then
echo "usage: $0 filename"
exit 1

fi

#Assign the first argument given to file_name
file_name=$1

#Search for the most frequently used words in a file

tr -cs '[:alnum:]' '[\n*]' < $file_name | sort | uniq -ci | sort -nr | head -10


