#!/bin/bash

#Name: Robert McNeil rjm160030
#Date: October 11, 2016
#File: hwk2.sh
#
#Summary: Drawing from a pile of 28 dominoes
#################

SIZE=0

unset MY_DOMINOS

NUMDRAW=6 #Number of dominoes to draw
EXIT="F" #Is the input correct or do we exit
FILE="F" #Are we writing to a file?


if [ $# -eq 2 ]; then #If we are writing to a file
   FILE="T" #Set FILE to true
fi

if [ $# -gt 0 ]; then 
   #draw the number of dominoes entered. 
   #If negative or greater than 28, error exit
   if [ $1 -lt 0 -o $1 -gt 28 ]; then
      echo "The number of dominoes entered is invalid"
      $EXIT="T"
   fi
   NUMDRAW=$1
fi

if [ $EXIT = "F" ]; then #If no error ocurred
   while [ $SIZE -lt $NUMDRAW ] #Continue drawing until the end
   do
      TEST=T #Dummy variable for whether our draw is valid
      NUM1=$(($RANDOM%7))
      NUM2=$(($RANDOM%7))
      if [ $NUM1 -le $NUM2 ]; then 
         NDOMINO="$NUM1|$NUM2"
         for  ((count=0; count<$SIZE; count+=1 )) #Go through our drawn list
         do
	    if [ $NDOMINO = ${MY_DOMINOS[$count]} ]; then #If the domino has been drawn
	       TEST=F #We cant use this domino, exit and restart
	       break
	    fi
         done
         if [ $TEST =  "T" ]; then #If our draw was valid
            MY_DOMINOS[$SIZE]=$NDOMINO #add our draw to the list
	    SIZE=$((SIZE+1)) #Increment the counter
	    if [ $FILE = "T" ]; then #If file, write to a file
	       echo $NDOMINO >> $2
	    fi
	    
	    echo $NDOMINO #Print to the screen
         fi
      fi

   done
fi



