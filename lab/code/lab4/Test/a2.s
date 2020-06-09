.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text

read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra

write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra


main:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t0, $v0
  move $t1, $t0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t2, $v0
  move $t3, $t2
  bgt $t1, $t3, label0
  j label1
label0:
  li $t5, 2
  mul $t4, $t3, $t5
  bgt $t1, $t4, label3
  j label4
label3:
  move $t6, $t1
  move $a0, $t6
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label14
label4:
  li $t5, 2
  mul $t7, $t3, $t5
  move $a0, $t7
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label14
label1:
  beq $t1, $t3, label6
  j label7
label6:
  li $s0, 100
  bgt $t3, $s0, label9
  j label10
label9:
  li $s1, 100
  move $a0, $s1
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label14
label10:
  move $s2, $t3
  move $a0, $s2
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label14
label7:
  li $t5, 2
  mul $s3, $t1, $t5
  blt $s3, $t3, label12
  j label13
label12:
  move $s4, $t3
  move $a0, $s4
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label14
label13:
  li $t5, 2
  mul $s5, $t1, $t5
  move $a0, $s5
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
label14:
  move $v0, $0
  jr $ra
