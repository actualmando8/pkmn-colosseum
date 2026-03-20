stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
slwi r0, r5, 24
subi r3, r3, 0x1
or r4, r0, r4
bl fn_800730F8
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
