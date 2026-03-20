stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
mr r30, r3
mr r31, r4
bl fn_800D0F44
subis r0, r3, 0x4
cmplwi r0, 0x0
beq @80073068
li r3, 0x1
b @800730E0
@80073068
mr r3, r30
addi r4, r1, 0x8
bl fn_8025F3F4
cmpwi r3, 0x0
beq @80073084
li r3, 0x2
b @800730E0
@80073084
lbz r0, 0x8(r1)
rlwinm r0, r0, 0, 28, 28
cmpwi r0, 0x0
bne @800730B4
li r0, 0x11
mr r3, r30
stw r0, 0xc(r1)
addi r4, r1, 0xc
addi r5, r1, 0x8
bl fn_8025F648
li r3, -0x1
b @800730E0
@800730B4
mr r3, r30
addi r4, r1, 0x10
addi r5, r1, 0x8
bl fn_8025F584
cmpwi r3, 0x0
beq @800730D4
li r3, 0x3
b @800730E0
@800730D4
lwz r0, 0x10(r1)
li r3, 0x0
stw r0, 0x0(r31)
@800730E0
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
mtlr r0
addi r1, r1, 0x20
blr
