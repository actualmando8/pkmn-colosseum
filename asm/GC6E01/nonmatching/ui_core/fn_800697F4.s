stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stmw r26, 0x8(r1)
mr r26, r3
li r29, 0x1
bl fn_8025DA3C
bl fn_8025DA88
cmplwi r26, 0x0
beq @80069824
li r0, 0x1
stb r0, 0x0(r26)
@80069824
lis r3, lbl_803A9F08@ha
addi r31, r3, lbl_803A9F08@l
addis r30, r31, 0x1
@80069830
lwz r4, 0x2c(r31)
cmpwi r4, 0x18
bne @80069850
li r0, 0x1
li r29, 0x0
stb r0, -0x327c(r30)
li r27, 0x0
b @80069924
@80069850
lis r3, 0x2aab
subi r0, r3, 0x5555
mulhw r3, r0, r4
srwi r0, r3, 31
add r28, r3, r0
mulli r0, r28, 0x6
subf r26, r0, r4
bl fn_80061018
cmpwi r3, 0x0
bne @8006988C
mr r3, r28
mr r4, r26
bl fn_8025D970
mr r27, r3
b @800698B8
@8006988C
mr r3, r28
bl fn_8025D808
clrlwi r0, r3, 16
cmpw r0, r26
bgt @800698A8
li r27, 0x0
b @800698B8
@800698A8
mr r3, r28
mr r4, r26
bl fn_8025D938
mr r27, r3
@800698B8
mulli r5, r28, 0x48
lis r4, lbl_803A9F08@ha
mr r3, r27
addi r4, r4, lbl_803A9F08@l
mulli r0, r26, 0xc
add r4, r4, r5
add r28, r4, r0
addi r28, r28, 0x30
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80069910
mr r3, r27
bl fn_8010BBB8
li r0, 0x1
li r29, 0x0
stb r0, 0x0(r28)
sth r3, 0x2(r28)
lwz r3, 0x2c(r31)
addi r0, r3, 0x1
stw r0, 0x2c(r31)
b @80069924
@80069910
li r0, 0x0
stb r0, 0x0(r28)
lwz r3, 0x2c(r31)
addi r0, r3, 0x1
stw r0, 0x2c(r31)
@80069924
cmpwi r29, 0x0
bne @80069830
mr r3, r27
lmw r26, 0x8(r1)
lwz r0, 0x24(r1)
mtlr r0
addi r1, r1, 0x20
blr
