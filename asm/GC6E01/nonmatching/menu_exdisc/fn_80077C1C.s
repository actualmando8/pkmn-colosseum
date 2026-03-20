stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
clrlwi r0, r3, 16
cmpwi r0, 0xaf
beq @80077C4C
bge @80077C54
cmpwi r0, 0x0
beq @80077C44
b @80077C54
@80077C44
li r3, 0x1
b @80077C58
@80077C4C
li r3, 0x0
b @80077C58
@80077C54
bl fn_80142984
@80077C58
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
