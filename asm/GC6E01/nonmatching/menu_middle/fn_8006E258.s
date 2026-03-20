stwu r1, -0x50(r1)
mflr r0
stw r0, 0x54(r1)
stmw r23, 0x2c(r1)
mr r31, r3
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r3, 0x59cc(r3)
bl fn_8006B154
lis r4, lbl_80268560@ha
mr r29, r3
addi r26, r4, lbl_80268560@l
li r25, 0x0
@8006E290
subf r0, r25, r29
mr r28, r26
cntlzw r0, r0
addi r27, r1, 0x8
srwi r0, r0, 5
li r23, 0x0
clrlwi r24, r0, 24
@8006E2AC
lhz r4, 0x0(r28)
mr r3, r31
bl fn_801046C8
mr r30, r3
mr r4, r24
bl fn_80109220
stw r30, 0x0(r27)
addi r28, r28, 0x2
addi r27, r27, 0x4
addi r23, r23, 0x1
cmplwi r23, 0x5
blt @8006E2AC
cmplwi r24, 0x0
beq @8006E2EC
li r0, 0x424b
b @8006E2F0
@8006E2EC
li r0, 0x0
@8006E2F0
lwz r3, 0x18(r1)
cmplwi r24, 0x0
stw r0, 0x4c(r3)
beq @8006E308
li r0, 0x3f40
b @8006E30C
@8006E308
li r0, 0x0
@8006E30C
lwz r3, 0x14(r1)
addi r26, r26, 0xa
addi r25, r25, 0x1
stw r0, 0x4c(r3)
cmplwi r25, 0x2
blt @8006E290
lmw r23, 0x2c(r1)
lwz r0, 0x54(r1)
mtlr r0
addi r1, r1, 0x50
blr
