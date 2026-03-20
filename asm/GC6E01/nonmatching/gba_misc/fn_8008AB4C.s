stwu r1, -0x790(r1)
mflr r0
stw r0, 0x794(r1)
stw r31, 0x78c(r1)
mr r31, r3
mr r3, r4
addi r4, r1, 0x8
bl fn_80083D30
subi r3, r31, 0x1
addi r4, r1, 0x8
bl fn_800733D0
lwz r0, 0x794(r1)
lwz r31, 0x78c(r1)
mtlr r0
addi r1, r1, 0x790
blr
