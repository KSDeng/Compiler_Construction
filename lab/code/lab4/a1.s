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
  mul $t8, $t7, $t1
  add $t9, $t4, $t8
  move $t0, $t9
  move $t3, $t0
  move $a0, $t3
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  add $t2, $t4, $t7
  mul $t5, $t0, $t2
  li $t6, 25
  div $t5, $t6
  mflo $t1
  move $t0, $t1
  add $t8, $t7, $t0
  div $t4, $t0
  mflo $t9
  sub $t3, $t8, $t9
  li $t6, 12
  mul $t2, $t5, $t6
  add $t1, $t3, $t2
  move $t7, $t1
  move $t4, $t0
  move $a0, $t4
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t8, $t7
  move $a0, $t8
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $v0, $0
  jr $ra
