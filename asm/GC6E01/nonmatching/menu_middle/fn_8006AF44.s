stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
mr r31, r3
cmplwi r4, 0x0
beq @8006AF98
addis r3, r31, 0x1
li r5, 0x1660
subi r3, r3, 0x4cd8
bl memcpy
lis r3, lbl_80267DD8@ha
addis r4, r31, 0x1
addi r5, r3, lbl_80267DD8@l
li r3, 0x0
lwz r5, 0x0(r5)
li r0, 0x1
stw r5, -0x4cb4(r4)
sth r3, -0x4cd6(r4)
stb r0, -0x3678(r4)
b @8006AFA4
@8006AF98
addis r3, r31, 0x1
li r0, 0x0
stb r0, -0x3678(r3)
@8006AFA4
addis r3, r31, 0x1
li r0, 0x0
stb r0, -0x3675(r3)
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
