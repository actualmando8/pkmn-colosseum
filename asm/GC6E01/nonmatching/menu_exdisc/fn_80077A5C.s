stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
cmplwi r3, 0x0
li r31, 0x0
beq @80077A90
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80077A94
@80077A90
li r31, 0x1
@80077A94
clrlwi r3, r31, 24
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
