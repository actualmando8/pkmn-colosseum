stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
mr r31, r3
cmpwi r31, 0x0
blt @8006B3EC
cmpwi r31, 0x7
blt @8006B3F4
@8006B3EC
li r3, 0x0
b @8006B40C
@8006B3F4
li r3, 0x0
li r4, 0xe
bl fn_80129280
addis r0, r31, 0x1
add r3, r0, r3
lbz r3, -0x342c(r3)
@8006B40C
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
