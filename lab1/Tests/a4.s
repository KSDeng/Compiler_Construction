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
label0:
  li $t1, 5
  blt $t0, $t1, label1
  j label2
label1:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t2, $v0
  move $t3, $t0
  li $t5, 4
  mul $t4, $t3, $t5
  lw $t6, 0($t7)
  add $s0, $t6, $t4
  sw $s0, 0($t2)
  addi $s1, $t0, 1
  move $t0, $s1
  j label0
label2:
  li $s2, 0
label3:
  li $s3, 1
  beq $s2, $s3, label4
  j label5
label4:
  li $s2, 0
  li $t0, 0
label6:
  li $t1, 5
  blt $t0, $t1, label7
  j label3
label7:
  move $s4, $t0
label9:
  li $s5, 0
  bgt $s4, $s5, label12
  j label11
label12:
  move $s6, $s4
  li $t5, 4
  mul $s7, $s6, $t5
  lw $t8, 0($t7)
  add $t9, $t8, $s7
  lw $t3, 0($t9)
  addi $t6, $s4, -1
  li $t5, 4
  mul $t4, $t6, $t5
  lw $s0, 0($t7)
  add $t2, $s0, $t4
  lw $s1, 0($t2)
  blt $t3, $s1, label10
  j label11
label10:
  move $s3, $s4
  li $t5, 4
  mul $s2, $s3, $t5
  lw $t1, 0($t7)
  add $t0, $t1, $s2
  lw $s5, 0($t0)
  move $s6, $s5
  addi $t8, $s4, -1
  li $t5, 4
  mul $s7, $t8, $t5
  lw $t9, 0($t7)
  add $t6, $t9, $s7
  lw $s0, 0($t6)
  move $t4, $s4
  li $t5, 4
  mul $t2, $t4, $t5
  lw $t3, 0($t7)
  add $s1, $t3, $t2
  sw $s1, 0($s0)
  move $s3, $s6
  addi $t1, $s4, -1
  li $t5, 4
  mul $s2, $t1, $t5
  lw $t0, 0($t7)
  add $s5, $t0, $s2
  sw $s5, 0($s3)
  li $t8, 0
  addi $t9, $s4, -1
  move $s4, $t9
  j label9
label11:
  addi $t6, $s7, 1
  move $s7, $t6
  j label6
  j label3
label5:
  li $s7, 0
label13:
  li $t4, 5
  blt $s7, $t4, label14
  j label15
label14:
  move $t3, $s7
  li $t5, 4
  mul $t2, $t3, $t5
  lw $s1, 0($t7)
  add $s0, $s1, $t2
  lw $s6, 0($s0)
  move $a0, $s6
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $t1, $s7, 1
  move $s7, $t1
  j label13
label15:
  move $v0, $0
  jr $ra
