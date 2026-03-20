stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
li r4, 0xe
bl fn_80129280
addis r3, r3, 0x1
lbz r0, -0x3678(r3)
cmplwi r0, 0x0
beq @8006A744
subi r3, r3, 0x4cd8
b @8006A748
@8006A744
li r3, 0x0
@8006A748
cmplwi r3, 0x0
beq @8006A758
lhz r3, 0x0(r3)
b @8006A75C
@8006A758
li r3, 0x0
@8006A75C
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
