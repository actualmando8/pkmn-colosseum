stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
mr r31, r4
lha r0, 0x6(r31)
cmpwi r0, 0xa28
bge @80070490
cmpwi r0, 0xa1d
bge @80070454
b @80070490
@80070454
lwz r3, 0x4c(r31)
cmplwi r3, 0x0
beq @80070490
bl fn_800FA280
mr r0, r3
li r3, 0x37
mr r4, r0
bl fn_80132A38
lwz r5, 0x64(r31)
li r3, 0x0
li r4, 0x0
li r6, 0xe7
bl fn_800FB680
li r0, 0x0
stw r0, 0x4c(r31)
@80070490
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
