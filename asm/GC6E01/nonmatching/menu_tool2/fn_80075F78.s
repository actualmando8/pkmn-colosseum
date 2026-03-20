stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
lbz r0, 0x95(r3)
extsb r0, r0
cmpwi r0, 0x1
beq @80075FB8
bge @80075FA4
cmpwi r0, 0x0
bge @80075FB0
b @80075FC8
@80075FA4
cmpwi r0, 0x3
bge @80075FC8
b @80075FC0
@80075FB0
li r3, 0x43bc
b @80075FCC
@80075FB8
li r3, 0x43ba
b @80075FCC
@80075FC0
li r3, 0x43be
b @80075FCC
@80075FC8
li r3, 0x1
@80075FCC
bl fn_800FA280
mr r4, r3
li r3, 0x37
bl fn_80132A38
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
