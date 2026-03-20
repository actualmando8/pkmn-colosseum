stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
li r3, 0x0
bl fn_80083BF8
neg r0, r3
andc r0, r0, r3
srwi r3, r0, 31
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
