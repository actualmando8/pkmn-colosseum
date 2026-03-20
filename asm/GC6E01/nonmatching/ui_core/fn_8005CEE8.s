stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
mr r31, r3
bl fn_8006B8F0
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r31, 0x0(r3)
li r3, 0x395
bl fn_800FF58C
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
