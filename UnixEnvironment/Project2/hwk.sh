#!/bin/bash

SIZE=0

unset MY_DOMINOS

NUMDRAW=6

FILE=.

if [ $# -eq 1 ]; then
   #draw the number of dominoes entered. 
   #If negative or greater than 28, error exit
   
   if [ $1 -lt 0 -o $1 -gt 28 ]; then
      echo "The number of dominoes entered is invalid"
   
   NUMDRAW=$1
   else
   
   while [ $SIZE -lt $NUMDRAW ]
   do
      TEST=T
      NUM1=$(($RANDOM%6))
      NUM2=$(($RANDOM%6))
      if [ $NUM1 -lt $NUM2 ]; then
         NDOMINO="$NUM1|$NUM2"
         for  ((count=0; count<$SIZE; count+=1 ))#domino in $MY_DOMINOS
         do
	    if [ $NDOMINO = ${MY_DOMINOS[$count]} ]; then
	       TEST=F
	       break
	    fi
         done
         if [ $TEST =  "T" ]; then
            MY_DOMINOS[$SIZE]=$NDOMINO
	    SIZE=$((SIZE+1))
	    echo $NDOMINO
         fi
      fi

   done
   fi
fi

#if [ $# -eq 1 ]; then
   #draw the number of dominoes entered. 
   #If negative or greater than 28, error exit
   
#   if [ $1 -lt 0 -o $1 -gt 28 ];
#      echo "The number of dominoes entered is invalid"
#   fi
#fi

if [ $# -eq 2 ]; then
   #first is dominoes, second is name of file to be overwritten/created
   #dominoes outputted to this file as they are drawn in addition to printing
   #in order drawn
   echo "FAIL"
fi




