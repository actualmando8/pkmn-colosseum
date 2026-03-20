stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
cmplwi r3, 0x0
beq @80083D14
b @80083D20
@80083D14
li r3, 0x0
li r4, 0xd
bl fn_80129280
@80083D20
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
