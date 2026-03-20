stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
mr r29, r3
lis r4, 0x8000
lis r3, 0x1062
lwz r0, 0xf8(r4)
addi r3, r3, 0x4dd3
srwi r0, r0, 2
mulhwu r0, r3, r0
srwi r30, r0, 6
bl OSGetTick
mr r31, r3
@800739D0
bl OSGetTick
subf r0, r31, r3
cmplw r0, r30
blt @800739D0
mr r3, r29
bl fn_800D0F44
subis r0, r3, 0x4
cmplwi r0, 0x0
beq @800739FC
li r3, 0x1
b @80073A28
@800739FC
li r0, 0x11
mr r3, r29
stw r0, 0xc(r1)
addi r4, r1, 0xc
addi r5, r1, 0x8
bl fn_8025F648
cmpwi r3, 0x0
beq @80073A24
li r3, 0x2
b @80073A28
@80073A24
li r3, 0x0
@80073A28
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
