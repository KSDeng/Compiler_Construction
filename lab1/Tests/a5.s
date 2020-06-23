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


compare:
  move $t0, $a0
  move $t1, $a1
  bgt $t0, $t1, label0
  j label1
label0:
  move $v0, $t0
  jr $ra
label1:
  blt $t0, $t1, label2
  j label3
label2:
  move $v0, $t1
  jr $ra
label3:
  move $v0, $0
  jr $ra

add2:
  move $t0, $a0
  move $t1, $a1
  add $t2, $t0, $t1
  move $v0, $t2
  jr $ra

main:
  li $t0, 0
label4:
  li $t1, 10
  blt $t0, $t1, label5
  j label6
label5:
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
  j label4
label6:
  li $t0, 0
  li $s2, 0
label7:
  li $t1, 10
  blt $t0, $t1, label8
  j label9
label8:
  move $s3, $t0
  li $t5, 4
  mul $s4, $s3, $t5
  lw $s5, 0($t7)
  add $s6, $s5, $s4
  lw $s7, 0($s6)
  addi $t8, $t0, 1
  li $t5, 4
  mul $t9, $t8, $t5
  lw $t3, 0($t7)
  add $t6, $t3, $t9
  lw $t4, 0($t6)
  addi $sp, $sp, -12
  sw $ra, 0($sp)
  sw $a0, 4($sp)
  sw $a1, 8($sp)
  move $a0, $s7
  move $a1, $t4
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -4
  sw $t1, 0($sp)
  addi $sp, $sp, -4
  sw $t2, 0($sp)
  addi $sp, $sp, -4
  sw $t3, 0($sp)
  addi $sp, $sp, -4
  sw $t4, 0($sp)
  addi $sp, $sp, -4
  sw $t5, 0($sp)
  addi $sp, $sp, -4
  sw $t6, 0($sp)
  addi $sp, $sp, -4
  sw $t7, 0($sp)
  addi $sp, $sp, -4
  sw $s0, 0($sp)
  addi $sp, $sp, -4
  sw $s1, 0($sp)
  addi $sp, $sp, -4
  sw $s2, 0($sp)
  addi $sp, $sp, -4
  sw $s3, 0($sp)
  addi $sp, $sp, -4
  sw $s4, 0($sp)
  addi $sp, $sp, -4
  sw $s5, 0($sp)
  addi $sp, $sp, -4
  sw $s6, 0($sp)
  addi $sp, $sp, -4
  sw $s7, 0($sp)
  addi $sp, $sp, -4
  sw $t8, 0($sp)
  addi $sp, $sp, -4
  sw $t9, 0($sp)
  jal compare
  lw $t9, 0($sp)
  addi $sp, $sp, 4
  lw $t8, 0($sp)
  addi $sp, $sp, 4
  lw $s7, 0($sp)
  addi $sp, $sp, 4
  lw $s6, 0($sp)
  addi $sp, $sp, 4
  lw $s5, 0($sp)
  addi $sp, $sp, 4
  lw $s4, 0($sp)
  addi $sp, $sp, 4
  lw $s3, 0($sp)
  addi $sp, $sp, 4
  lw $s2, 0($sp)
  addi $sp, $sp, 4
  lw $s1, 0($sp)
  addi $sp, $sp, 4
  lw $s0, 0($sp)
  addi $sp, $sp, 4
  lw $t7, 0($sp)
  addi $sp, $sp, 4
  lw $t6, 0($sp)
  addi $sp, $sp, 4
  lw $t5, 0($sp)
  addi $sp, $sp, 4
  lw $t4, 0($sp)
  addi $sp, $sp, 4
  lw $t3, 0($sp)
  addi $sp, $sp, 4
  lw $t2, 0($sp)
  addi $sp, $sp, 4
  lw $t1, 0($sp)
  addi $sp, $sp, 4
  lw $t0, 0($sp)
  addi $sp, $sp, 4
  lw $a1, 8($sp)
  lw $a0, 4($sp)
  lw $ra, 0($sp)
  addi $sp, $sp, 12
  move $t0, $v0
  move $t0, $s2
  li $t5, 4
  mul $t0, $t1, $t5
  lw $t2, 0($t3)
  add $t4, $t2, $t0
  sw $t4, 0($t6)
  addi $s0, $t7, 2
  move $t7, $s0
  addi $s1, $s2, 1
  move $s2, $s1
  j label7
label9:
  li $t7, 0
label10:
  li $s3, 5
  blt $t7, $s3, label11
  j label12
label11:
  move $s4, $t7
  li $t5, 4
  mul $s5, $s4, $t5
  lw $s6, 0($t3)
  add $s7, $s6, $s5
  lw $t8, 0($s7)
  move $t9, $t7
  li $t5, 4
  mul $t1, $t9, $t5
  lw $t2, 0($t0)
  add $t4, $t2, $t1
  lw $t6, 0($t4)
  addi $sp, $sp, -12
  sw $ra, 0($sp)
  sw $a0, 4($sp)
  sw $a1, 8($sp)
  move $a0, $t8
  move $a1, $t6
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -4
  sw $t1, 0($sp)
  addi $sp, $sp, -4
  sw $t2, 0($sp)
  addi $sp, $sp, -4
  sw $t3, 0($sp)
  addi $sp, $sp, -4
  sw $t4, 0($sp)
  addi $sp, $sp, -4
  sw $t5, 0($sp)
  addi $sp, $sp, -4
  sw $t6, 0($sp)
  addi $sp, $sp, -4
  sw $t7, 0($sp)
  addi $sp, $sp, -4
  sw $s0, 0($sp)
  addi $sp, $sp, -4
  sw $s1, 0($sp)
  addi $sp, $sp, -4
  sw $s2, 0($sp)
  addi $sp, $sp, -4
  sw $s3, 0($sp)
  addi $sp, $sp, -4
  sw $s4, 0($sp)
  addi $sp, $sp, -4
  sw $s5, 0($sp)
  addi $sp, $sp, -4
  sw $s6, 0($sp)
  addi $sp, $sp, -4
  sw $s7, 0($sp)
  addi $sp, $sp, -4
  sw $t8, 0($sp)
  addi $sp, $sp, -4
  sw $t9, 0($sp)
  jal add2
  lw $t9, 0($sp)
  addi $sp, $sp, 4
  lw $t8, 0($sp)
  addi $sp, $sp, 4
  lw $s7, 0($sp)
  addi $sp, $sp, 4
  lw $s6, 0($sp)
  addi $sp, $sp, 4
  lw $s5, 0($sp)
  addi $sp, $sp, 4
  lw $s4, 0($sp)
  addi $sp, $sp, 4
  lw $s3, 0($sp)
  addi $sp, $sp, 4
  lw $s2, 0($sp)
  addi $sp, $sp, 4
  lw $s1, 0($sp)
  addi $sp, $sp, 4
  lw $s0, 0($sp)
  addi $sp, $sp, 4
  lw $t7, 0($sp)
  addi $sp, $sp, 4
  lw $t6, 0($sp)
  addi $sp, $sp, 4
  lw $t5, 0($sp)
  addi $sp, $sp, 4
  lw $t4, 0($sp)
  addi $sp, $sp, 4
  lw $t3, 0($sp)
  addi $sp, $sp, 4
  lw $t2, 0($sp)
  addi $sp, $sp, 4
  lw $t1, 0($sp)
  addi $sp, $sp, 4
  lw $t0, 0($sp)
  addi $sp, $sp, 4
  lw $a1, 8($sp)
  lw $a0, 4($sp)
  lw $ra, 0($sp)
  addi $sp, $sp, 12
  move $t0, $v0
  move $t0, $t7
  li $t5, 4
  mul $t0, $t1, $t5
  lw $t2, 0($t3)
  add $t4, $t2, $t0
  sw $t4, 0($t6)
  move $s0, $t7
  li $t5, 4
  mul $s1, $s0, $t5
  lw $s2, 0($t3)
  add $s3, $s2, $s1
  lw $s4, 0($s3)
  move $a0, $s4
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $s5, $t7, 1
  move $t7, $s5
  j label10
label12:
  move $v0, $0
  jr $ra
