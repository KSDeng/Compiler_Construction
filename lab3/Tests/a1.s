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
  li $t0, 12
  li $t1, 4
  li $t2, 5
  mul $t3, $t0, $t1
  move $t4, $t3
  add $t5, $t4, $t2
  sub $t6, $t5, $t1
  move $t7, $t6
  mul $s0, $t7, $t1
  add $s1, $t4, $s0
  move $s2, $s1
  move $s3, $s2
  move $a0, $s3
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  add $s4, $t4, $t7
  mul $s5, $s2, $s4
  li $s6, 25
  div $s5, $s6
  mflo $s7
  move $s2, $s7
  add $t8, $t7, $s2
  div $t4, $s2
  mflo $t9
  sub $t0, $t8, $t9
  li $t2, 12
  mul $t3, $t1, $t2
  add $t5, $t0, $t3
  move $t7, $t5
  move $t6, $s2
  move $a0, $t6
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $s0, $t7
  move $a0, $s0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $v0, $0
  jr $ra
