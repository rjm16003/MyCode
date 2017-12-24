#Created by: Robert McNeil rjm160030
#Created on: 7/13/2016
#Last modified: 7/13/2016
#Class: CS 3340.0U1
#Function: Celsius and Fahrenheit Converter
############################
	.data
#Stored Strings
ctf:	.asciiz "1) Celsius -> Fahrenheit\n"
ftc:	.asciiz "2) Fahrenheit -> Celsius\nEnter a number:\n"
celtemp:.asciiz "Enter a temperature in Celsuis: "
fahtemp:.asciiz "Enter a temperature in Fahrenheit: " 

five:	.float 5.00#predefined float constants
thrtwo:	.float 32.00
nine:	.float 9.00

#
	.text
main:

			
	la $a0, ctf  # load the address of the string
	li $v0, 4    # 4: print a null-terminated string
	syscall
	
	la $a0, ftc  # load the address of the string
	li $v0, 4    # 4: print a null-terminated string
	syscall
	
	li $v0, 5 #5: Read an Integer
	syscall
	
	beq $v0, 1, ctfc #Branch to Celsius to Fahrenheit Converter if entered number is 1
	beq $v0, 2, ftcc #Branch to Fahrenheit to Celsius Converter if entered number is 2
	j main #If wrong number entered, repeat the jump

ctfc:	la $a0, celtemp#Celsius to Fahrenheit Converter
	li $v0, 4
	syscall
	
	li $v0, 6 # 6: Read in a float
	syscall
	
	l.s $f3, nine#multiply by 9
	mul.s $f1, $f0, $f3
	
	l.s $f4, five #divide by 5
	div.s $f2, $f1, $f4
	
	l.s $f5 thrtwo # add 32
	add.s $f12, $f2, $f5
	 
	li $v0, 2 # print a result, a float
	syscall
	
	
	j end #jump to the end of the program

ftcc:	la $a0, fahtemp #Fahrenheit to Celsius Converter
	li $v0, 4
	syscall
	
	li $v0, 6 # 6: Read in a float
	syscall
	

	l.s $f5, thrtwo #subtract 32
	sub.s $f1, $f0, $f5
	
	l.s $f3, five # multiply by 32
	mul.s $f2, $f1, $f3
	 
	l.s $f6, nine # divide by 9
	div.s $f12, $f2, $f6
	 
	li $v0, 2 #Print the result, a float
	syscall
	
end:			
	
