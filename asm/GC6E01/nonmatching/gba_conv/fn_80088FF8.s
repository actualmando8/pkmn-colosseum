stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
cntlzw r0, r4
srwi r0, r0, 5
clrlwi r3, r0, 24
bl fn_80089030
li r3, 0x0
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
