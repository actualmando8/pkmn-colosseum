stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
li r3, 0x1
bl fn_801C40F0
li r3, 0xe0
bl fn_80102510
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
