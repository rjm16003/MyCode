.data

    Message:        .asciiz "New number generating..."
    UserMessage:    .asciiz "\nEnter guess: "
    bullMessage:    .asciiz "Bulls: "
    cowMessage:	    .asciiz "\nCows : "
    failMessage:    .asciiz "\nGame Over!"
    tryMessage:	    .asciiz "\nWould you like to try again? Enter 1 for yes, 0 for no: "
    hisMessage:	    .asciiz "\nWould you like to view the history of your guesses? Enter 1 for yes, 0 for no: "
    difMessage:	    .asciiz "Enter the desired difficulty. Enter 0 for easy, 1 for medium, and 2 for hard: "
    
    bullScoreMess: .asciiz "Bulls: "
    cowScoreMess: .asciiz ", Cows: "
    errMessage: .asciiz "Invalid Guess(4 digits, can't repeat)"
    winMessage: .asciiz "\nYOU WIN!!!\n"
    numMessage: .asciiz "\nThe secret number was "
    newLine: .asciiz "\n"
    guess: .asciiz "guess"
    semicollon: .asciiz ": "
    score: .asciiz "Result: "

    Bullarr:   .word 0, 0, 0, 0
    Cowarr:	.word 0,0,0,0
    Tryarr:     .space 80
    ScoreBullarr:   .space 80
    ScoreCowarr: .space 80
    prompt1: .asciiz "\n\nWould you like to try again? Enter: \n0 for no \n1 for yes \n2 to start a new game \nEntry: "
    invEntry: .asciiz "\nInvalid Entry... type 0, 1, or 2"
  

    Menu:	.asciiz "Welcome to the Bulls&Cows Game!\nEnter 1) To begin the game\nEnter 2) To read the rules\n0) To Quit\nEnter number here: "
    Rules:	.asciiz "Rules for Bulls & Cows:\nA 4 digit number will randomly be generated, the first digit will not equal 0\nGuess what the 4 digit number is!\nEach Digit only appears once, meaning numbers will not repeat\nIf you correctly guess the digit in the right spot, thats a bull\nIf you guess a digit that is in the number but not in the right spot, thats a cow\nOnce you get 4 bulls, you win the game.\n"      


.text
start:
    li $v0, 4
    la $a0, Menu
    syscall
    
    li $v0, 5
    syscall
    
    addi $t0, $0, 1
    addi $t1, $t0, 1
    addi $s5, $0, 0
    beq $v0, $0, Exit
    beq $v0, $t0, dif
   
    li $v0, 4
    la $a0, Rules
    syscall
    j start
    
 dif:li $v0, 4
    la $a0, difMessage
    syscall
    
    li   $v0, 5
    syscall
    addi $t9, $v0, 0
	
    beq $t9, $0, dife
    beq $t9, 1, difm
    j difh

dife:addi $t8, $0, 15
    j A0 
    
difm:addi $t8, $0, 10
    j A0        
difh:addi $t8, $0, 7
    j A0               

# 15 is easy, 7 is hard, 10 is medium

ARetry:
	li $v0, 4
	la $a0, Message
        syscall
A0:
	
# Random Number Generator
	li $v0, 42
	li $a0, 3
	li $a1, 9999
	syscall
# Make sure random number does not begin with 0	
	addi $t0, $a0, 0
	blt  $t0, 1000, A0
# Add random number to register t4 for verification
	addi $t4, $a0, 0

# Compare each number in Random Number to make sure digits do not repeat.
A1:
# $t4 mod 10 put result in $t0	
	la $t7, Bullarr
	div  $t4, $t4, 10
	mfhi $t0
	sw $t0, 0($t7)
# $t4 mod 10 put result in $t1		
	div  $t4, $t4, 10
	mfhi $t1
	sw $t1, 4($t7)
# $t4 mod 10 put result in $t2	
	div  $t4, $t4, 10
	mfhi $t2
	sw $t2, 8($t7)
# $t4 mod 10 put result in $t3	
	div  $t4, $t4, 10
	mfhi $t3
	sw $t3, 12($t7)
# If any digits match jump to newNum to generate a new Random Number	
	beq  $t0, $t1, A0
	beq  $t0, $t2, A0
	beq  $t0, $t3, A0
	beq  $t1, $t2, A0
	beq  $t1, $t3, A0
	beq  $t2, $t3, A0

	j    B0

BCont:
	beq $t8, $0, Fail
        #Display prompt1
	li $v0, 4
	la $a0, prompt1
	syscall
	
	#Recieve input from user
	li $v0, 5
	syscall
	#jal makeUpperCase #Makes sure User's input is set to UpperCase value
	move $t1, $v0
	li   $t2, 0
	li   $t3, 1
	li   $t4, 2
	
	beq  $t1, $t2, Exit
	beq  $t1, $t3, B0   
	beq  $t1, $t4, ARetry   		 		
        
        li $v0, 4
        la $a0, invEntry
        syscall
        
        j  BCont

B0:
	beq $t8, $0, Fail
		
	addi $t0, $a0, 0
	li   $v0, 4
	la   $a0, UserMessage
	syscall
	
	addi $a0, $t0, 0
	
	li   $v0, 5
	syscall
	
	addi $t4, $v0, 0
	
	sw $t4, Tryarr($s5)
	
	la $t6, Cowarr
	div  $t4, $t4, 10
	mfhi $t0
	sw $t0, 0($t6)
# $t4 mod 10 put result in $t1		
	div  $t4, $t4, 10
	mfhi $t1
	sw $t1, 4($t6)
# $t4 mod 10 put result in $t2	
	div  $t4, $t4, 10
	mfhi $t2
	sw $t2, 8($t6)
# $t4 mod 10 put result in $t3	
	div  $t4, $t4, 10
	mfhi $t3
	sw $t3, 12($t6)
	
	# If any digits match jump to inv to generate an invalid input prompt	
	beq  $t0, $t1, inv
	beq  $t0, $t2, inv
	beq  $t0, $t3, inv
	beq  $t1, $t2, inv
	beq  $t1, $t3, inv
	beq  $t2, $t3, inv
	beq $t3, $0, inv
	bgt $t4, 0, inv#if our guess is more than 4 digits
	
	add $t4, $0, $0 #Bull variable counter
	add $t5, $0, $0 #Cow variable counter
	
	addi $t8, $t8, -1		
	
B1:
	lw $s0, 0($t7)#load the actual number in the same way we stored the guesses
	lw $s1, 4($t7)
	lw $s2, 8($t7)
	lw $s3, 12($t7)
	
	bne $t0, $s0, B2#If digit 0 of our guess is not equal to digit 0 of the actual, jump
	jal bullp#else increment bull counter
	
B2:	bne $t1, $s1, B3#if digit 1 of our guess is not equal to digit 1 of the actual, jump
	jal bullp#else increment bull counter
	
B3:	bne $t2, $s2, B4
	jal bullp

B4:	bne $t3, $s3, C1
	jal bullp

C1:	beq  $t0, $s1, C11#if the guessed digit is present in the actual number, but in wrong spot
	beq  $t0, $s2, C11#jump to C11, where we increment the cow counter
	beq  $t0, $s3, C11
	j C2#else, jump to the next digit
C11:	jal cowp

C2:	beq  $t1, $s2, C21
	beq  $t1, $s3, C21
	beq  $t1, $s0, C21
	j C3
C21:	jal cowp

C3:	beq  $t2, $s3, C31
	beq $t2, $s1, C31
	beq $t2, $s0, C31
	j C4
C31:	jal cowp

C4:	beq $t3, $s0, C41
	beq $t3, $s1, C41
	beq $t3, $s2, C41
	j D1
C41:	jal cowp

D1: li $v0, 4#Print number of bulls guessed correctly
    la $a0, bullMessage
    syscall
    
    addi $a0, $t4, 0#t4 = bulls counter
     
    li $v0, 1
    syscall
    
    sw $t4, ScoreBullarr($s5)
    sw $t5, ScoreCowarr($s5)
    addi $s5, $s5, 4
    
D2: 
    beq $t4, 4, Win #if we have 4 bulls, we finished
    beq $t4,0,D21
    jal bullsound
#    bge $t4,0,bullsound  #$t4 bulls counter
    addi $t4,$t4,-1
    j D2
    
    
    D21:
    li $v0, 4#Print number of cows guessed correctly
    la $a0, cowMessage
    syscall
    
    addi $a0, $t5, 0 #t5 = cows counter
    
    li $v0, 1
    syscall
    
D3: 
    beq $t5,0,D31
    jal cowsound
 #   bge $t5,0,cowsound  #$t5 cows counter
    addi $t5,$t5,-1
    j D3
    
    D31: 
    beq $t4, 4, Win #if we have 4 bulls, we finished
    j BCont#else, make another guess
    
        
 inv:   li $v0, 4
    la $a0, errMessage
    syscall
    
    j B0
bullp:  addi $t4, $t4, 1
    jr $ra
cowp:   addi $t5, $t5, 1
    jr $ra
cowsound:    
    li $v0,31
    li $a0,60
    li $a1,1000
    li $a2,121
    li $a3,88
    syscall
    
    li $v0,31
    li $a0,60
    li $a1,500
    li $a2,0
    li $a3,0
    syscall
    
    jr $ra
    
bullsound:
       
    li $v0,31
    li $a0,60
    li $a1,500
    li $a2,127
    li $a3,88
    syscall
    
    li $v0,31
    li $a0,60
    li $a1,1000
    li $a2,0
    li $a3,0
    syscall
    
    jr $ra
  		
makeUpperCase:
	addi $t0, $0, 95
	slt  $t1, $t0, $v0
	beq  $t1, $0, goBack 
	sub $v0, $v0, 32
	jr  $ra
goBack:
	jr  $ra	
	  	  		  	  		  	
Win:	
	li $v0, 4
	la $a0, winMessage
	syscall
	
winsound: #Ode an die Freude music
li $v0,31
li $a0,64
li $a1,800
li $a2,1
li $a3,88
syscall
li $v0,31
li $a0,64
li $a1,800
li $a2,1
li $a3,88
syscall
li $v0,31
li $a0,65
li $a1,800
li $a2,1
li $a3,88
syscall
li $v0,31
li $a0,67
li $a1,800
li $a2,1
li $a3,88
syscall
li $v0,31
li $a0,67
li $a1,800
li $a2,1
li $a3,88
syscall
li $v0,31
li $a0,65
li $a1,800
li $a2,1
li $a3,88
syscall
li $v0,31
li $a0,64
li $a1,800
li $a2,1
li $a3,88
syscall
li $v0,31
li $a0,62
li $a1,800
li $a2,1
li $a3,88
syscall
li $v0,31
li $a0,60
li $a1,800
li $a2,1
li $a3,88
syscall
li $v0,31
li $a0,60
li $a1,800
li $a2,1
li $a3,88
syscall
li $v0,31
li $a0,62
li $a1,800
li $a2,1
li $a3,88
syscall
li $v0,31
li $a0,64
li $a1,800
li $a2,1
li $a3,88
syscall
li $v0,31
li $a0,62
li $a1,800
li $a2,1
li $a3,88
syscall

li $v0,31
li $a0,60
li $a1,800
li $a2,1
li $a3,88
syscall
li $v0,31
li $a0,60
li $a1,800
li $a2,1
li $a3,88
syscall
	
	j Done
	
Fail:	li $v0, 4
	la $a0, failMessage
	syscall
	
failsound: #swan lake music
li $v0,31
li $a0,76
li $a1,1600
li $a2,72
li $a3,88
syscall
li $v0,31
li $a0,69
li $a1,400
li $a2,72
li $a3,88
syscall
li $v0,31
li $a0,71
li $a1,400
li $a2,72
li $a3,88
syscall
li $v0,31
li $a0,72
li $a1,400
li $a2,72
li $a3,88
syscall
li $v0,31
li $a0,74
li $a1,400
li $a2,72
li $a3,88
syscall
li $v0,31
li $a0,76
li $a1,1200
li $a2,72
li $a3,88
syscall
li $v0,31
li $a0,72
li $a1,400
li $a2,72
li $a3,88
syscall
li $v0,31
li $a0,76
li $a1,400
li $a2,72
li $a3,88
syscall
li $v0,31
li $a0,72
li $a1,400
li $a2,72
li $a3,88
syscall
li $v0,31
li $a0,76
li $a1,1200
li $a2,72
li $a3,88
syscall
li $v0,31
li $a0,69
li $a1,400
li $a2,72
li $a3,88
syscall
li $v0,31
li $a0,72
li $a1,400
li $a2,72
li $a3,88
syscall
li $v0,31
li $a0,69
li $a1,400
li $a2,72
li $a3,88
syscall
li $v0,31
li $a0,65
li $a1,400
li $a2,72
li $a3,88
syscall
li $v0,31
li $a0,72
li $a1,400
li $a2,72
li $a3,88
syscall
li $v0,31
li $a0,69
li $a1,3200
li $a2,72
li $a3,88
syscall
		
Done:	li $v0, 4
	la $a0, hisMessage
	syscall
	
	li   $v0, 5
	syscall
	addi $t9, $v0, 0
	
	beq $t9, $0, TryAgain
	
	lw $s0, 0($t7)#load the actual number in the same way we stored the guesses
	lw $s1, 4($t7)
	lw $s2, 8($t7)
	lw $s3, 12($t7)
	
	li $v0, 4
	la $a0, numMessage
	syscall
	
	addi $a0, $s3, 0#t4 = bulls counter
	 
	li $v0, 1
	syscall
	
	addi $a0, $s2, 0#t4 = bulls counter
	 
	li $v0, 1
	syscall
	
	addi $a0, $s1, 0#t4 = bulls counter
	 
	li $v0, 1
	syscall
	
	addi $a0, $s0, 0#t4 = bulls counter
	 
	li $v0, 1
	syscall
	
	li $v0, 4
	la $a0, newLine
	syscall
	
	addi $t0, $0, 0
	addi $t1, $0, 0
	
	bne $s5, 0, scoreboard
	
	li $v0, 10
	syscall	
	
scoreboard:
        li $v0, 4
        la $a0, guess
        syscall
        
        addi $a0, $s5, 0
        sub $s5, $s5, 4
	
	addi $t0, $t0, 1
	addi $a0, $t0, 0
	
	li $v0, 1
	syscall
	
	li $v0, 4
        la $a0, semicollon
        syscall
        
        lw $t2, Tryarr($t1)
        addi $a0, $t2, 0
        
        li $v0, 1
	syscall
	
	li $v0, 4
        la $a0, newLine
        syscall
        
        li $v0, 4
        la $a0, score
        syscall
        
        li $v0, 4
        la $a0, bullScoreMess
        syscall
        
        lw $t2, ScoreBullarr($t1)
        addi $a0, $t2, 0
        
        li $v0, 1
	syscall
	
	li $v0, 4
        la $a0, cowScoreMess
        syscall
        
        lw $t2, ScoreCowarr($t1)
        addi $a0, $t2, 0
        
        li $v0, 1
	syscall
        
        
        li $v0, 4
        la $a0, newLine
        syscall
        
        addi $t1, $t1, 4
        
        
	beq $s5, 0, TryAgain
	j scoreboard

TryAgain:

	li $v0, 4
	la $a0, tryMessage
	syscall
	
	li   $v0, 5
	syscall
	addi $t9, $v0, 0
	
	bne $t9, $0, start

Exit:
       	li $v0, 10
	syscall
