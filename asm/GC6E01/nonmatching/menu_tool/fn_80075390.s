stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
bl fn_80075638
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800753B4
bl fn_8007565C
b @800753BC
@800753B4
li r3, 0x3
bl fn_800756C8
@800753BC
li r3, 0x0
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
