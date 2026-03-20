stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
mr r29, r3
mr r30, r4
addi r31, r29, 0x1
li r4, 0x2
mr r3, r31
bl fn_8008ABE4
mr r3, r29
mr r4, r30
bl fn_80073700
mr r0, r3
mr r3, r31
mr r31, r0
li r4, 0x1
bl fn_8008ABE4
mr r3, r31
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
