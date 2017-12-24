#Created by: Robert McNeil rjm160030
#Created on: 6/27/2016
#Last modified: 6/27/2016
#Class: CS 3340.0U1
#Function: Passing a course check, homework 3
############################


	.data
prompt1:	.asciiz "\n Are you attending most sessions of this course? (Y/N): "
prompt2:	.asciiz "\n Are you spending considerable effort for this course? (Y/N):  "
prompt3:	.asciiz "\n Are you curious about how does a computer work? (Y/N):  "
PASS:	.asciiz "\n You will PASS this course!"
DoWell:	.asciiz "\n You will DO WELL!"
FAIL:	.asciiz "\n You will FAIL this course!"
#
	.text
main:
	li $v0, 4 #print a string
	la $a0, prompt1 # Prompt for first question
	syscall
	
	li $v0, 12 #read in a character
	syscall

	move $t0, $v0 #move character to a temp value
	
	li $v0, 4 #print a string
	la $a0, prompt2 #prompt for second question
	syscall
	
	li $v0, 12 #read in a character
	syscall
	move $t1, $v0 #move character to t1
	
	li $v0, 4 #print a string
	la $a0, prompt2 #prompt for third question
	syscall
	
	li $v0, 12 #read in a character
	syscall
	move $t2, $v0 #move character to t2
	
	beq $t0, 89, Y1# If the answer to the first question is Y, branch
	beq $t1, 89, EXIT# If N,Y,Y or N,Y,N is an input combo, jump to exit
	beq $t2, 89, EXIT# If N,N,Y is an input combo, jump to exit
	li $v0, 4 #print a string
	la $a0, FAIL # Tell the user they will fail
	syscall
	j EXIT
	
Y1:	beq $t1, 89, Y2# If Y,Y,N or Y,Y,Y is an input combo, branch
	j EXIT #else jump to exit

Y2:	bne $t2, 89, Y3# If Y,Y,N is an input combo, branch, else we have Y,Y,Y
	li $v0, 4 #print a string
	la $a0, DoWell # Print Do Well message
	syscall
	j EXIT#jump to exit

Y3:	li $v0, 4 #print a string, our input combo is Y,Y,N
	la $a0, PASS # Print pass message
	syscall


EXIT: #exit the program
	


	
