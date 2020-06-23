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
  li $t0, 0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t1, $v0
  move $t2, $t1
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t3, $v0
  move $t4, $t3
  bgt $t2, $t4, label0
  j label1
label0:
  move $t5, $t2
  j label2
label1:
  move $t5, $t4
label2:
  li $t6, 0
  beq $t0, $t6, label4
  j label5
label4:
  div $t5, $t2
  mflo $t7
  mul $s0, $t7, $t2
  beq $t5, $s0, label6
  j label7
label6:
  div $t5, $t4
  mflo $s1
  mul $s2, $s1, $t4
  beq $t5, $s2, label9
  j label10
label9:
  move $s3, $t5
  li $t0, 1
  j label2
label10:
  addi $s4, $t5, 1
  move $t5, $s4
  j label2
label7:
  addi $s5, $t5, 1
  move $t5, $s5
  j label2
label5:
  move $s6, $s3
  move $a0, $s6
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $v0, $0
  jr $ra
