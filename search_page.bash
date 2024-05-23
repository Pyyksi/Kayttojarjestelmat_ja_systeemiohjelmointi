#! /bin/bash
#Viikko 8. Tehtävä 5.
#Searches for wanted man-pages found under /usr/share/man and prints out all the matching ones

#Test for the arguments
if [ $# -eq 0 ]
    then
echo "usage: search_page command"
exit 1

elif [ $# -eq 1 ]
    then
    
#Assign the first argument given to manual_name
manual_name=$1

#Search for and print the manual pages
find /usr/share/man -type f -name "${manual_name}.*.gz"

fi



