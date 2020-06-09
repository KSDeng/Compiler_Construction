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


fact:
  move $t0, $a0
  li $t1, 1
  beq $t0, $t1, label0
  j label1
label0:
  move $v0, $t0
  jr $ra
label1:
  addi $t2, $t0, -1
  addi $sp, $sp, -8
  sw $ra, 0($sp)
  sw $a0, 4($sp)
  move $a0, $t2
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -4
  sw $t1, 0($sp)
  addi $sp, $sp, -4
  sw $t2, 0($sp)
  jal fact
  lw $t2, 0($sp)
  addi $sp, $sp, 4
  lw $t1, 0($sp)
  addi $sp, $sp, 4
  lw $t0, 0($sp)
  addi $sp, $sp, 4
  lw $a0, 4($sp)
  lw $ra, 0($sp)
  addi $sp, $sp, 8
  move $t3, $v0
  mul $t4, $t0, $t3
  move $v0, $t4
  jr $ra

main:
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t0, $v0
  move $t1, $t0
  move $t2, $t1
  addi $sp, $sp, -8
  sw $ra, 0($sp)
  sw $a0, 4($sp)
  move $a0, $t2
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -4
  sw $t1, 0($sp)
  addi $sp, $sp, -4
  sw $t2, 0($sp)
  jal fact
  lw $t2, 0($sp)
  addi $sp, $sp, 4
  lw $t1, 0($sp)
  addi $sp, $sp, 4
  lw $t0, 0($sp)
  addi $sp, $sp, 4
  lw $a0, 4($sp)
  lw $ra, 0($sp)
  addi $sp, $sp, 8
  move $t3, $v0
  move $t4, $t3
  move $t5, $t4
  move $a0, $t5
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $v0, $0
  jr $ra
