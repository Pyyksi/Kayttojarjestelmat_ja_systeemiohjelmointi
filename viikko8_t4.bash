#! /bin/bash
#Viikko 8. Tehtävä 4.
#Prints out startup parametres (environment variables, positional and special parameters)

#Prints out environment variables
echo "Environment variables: "
printenv

#Prints out positional parameters
echo
echo "Positional parameters: "
last=$#
for i in $(seq 1 $last); do
    echo "$i: ${!i}"
done

#Prints out special parameters
echo
echo "Special Parameters: "
echo $#
echo $0
echo $i
echo $*
echo $!
echo $?
echo $_
echo $$
echo $@
echo $-


