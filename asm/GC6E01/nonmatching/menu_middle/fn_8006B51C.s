stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
mr r31, r3
cmpwi r31, 0x0
blt @8006B540
cmplwi r31, 0x6
blt @8006B548
@8006B540
li r3, 0x0
b @8006B568
@8006B548
li r3, 0x0
li r4, 0xe
bl fn_80129280
mulli r4, r31, 0x54
mr r0, r3
addis r3, r4, 0x1
subi r3, r3, 0x3624
add r3, r0, r3
@8006B568
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
