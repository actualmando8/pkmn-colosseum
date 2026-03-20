stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x8(r3)
cmpwi r0, 0x3
bge @80077C08
cmpwi r0, 0x0
bge @80077C00
b @80077C08
@80077C00
li r3, 0x1
b @80077C0C
@80077C08
li r3, 0x0
@80077C0C
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
