stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x0(r3)
cmpwi r0, 0x2
bne @80077DFC
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x8(r3)
cmpwi r0, 0x0
bne @80077DFC
li r3, 0x6
b @80077E40
@80077DFC
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x4(r3)
cmpwi r0, 0x1
beq @80077E34
bge @80077E24
cmpwi r0, 0x0
bge @80077E2C
b @80077E3C
@80077E24
cmpwi r0, 0x3
b @80077E3C
@80077E2C
li r3, 0x3
b @80077E40
@80077E34
li r3, 0x4
b @80077E40
@80077E3C
li r3, 0x2
@80077E40
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
