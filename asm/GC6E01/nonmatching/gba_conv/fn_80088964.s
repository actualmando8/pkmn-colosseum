stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
li r3, 0xd
li r4, 0x2
li r5, 0x0
bl fn_801D0748
cmpwi r3, 0x4
bne @80088990
li r3, 0x0
b @80088994
@80088990
li r3, -0x1
@80088994
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
