stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
stw r28, 0x10(r1)
mr r31, r3
lbz r0, 0x1(r31)
extsb r0, r0
cmpwi r0, 0x3
beq @800703B0
bge @80070358
cmpwi r0, 0x0
beq @80070360
b @80070408
@80070358
cmpwi r0, 0x6
b @80070408
@80070360
lbz r0, 0x2(r31)
extsb r0, r0
cmpwi r0, 0x0
bne @80070408
lis r3, lbl_80267F68@ha
li r28, 0x0
addi r29, r3, lbl_80267F68@l
lis r3, lbl_80267EA8@ha
addi r30, r3, lbl_80267EA8@l
@80070384
lwz r0, 0x4(r29)
mr r3, r31
lhz r4, 0x0(r29)
slwi r0, r0, 2
lhzx r5, r30, r0
bl fn_801081F8
addi r29, r29, 0x8
addi r28, r28, 0x1
cmplwi r28, 0x10
blt @80070384
b @80070408
@800703B0
lbz r0, 0x2(r31)
extsb r0, r0
cmpwi r0, 0x0
bne @80070408
lis r3, lbl_80267F68@ha
li r28, 0x0
addi r29, r3, lbl_80267F68@l
lis r3, lbl_80267EA8@ha
addi r30, r3, lbl_80267EA8@l
@800703D4
lwz r0, 0x4(r29)
mr r3, r31
lhz r4, 0x0(r29)
slwi r0, r0, 2
add r5, r30, r0
lhz r5, 0x2(r5)
bl fn_801081F8
addi r29, r29, 0x8
addi r28, r28, 0x1
cmplwi r28, 0x10
blt @800703D4
li r0, 0x1
stb r0, 0x2(r31)
@80070408
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
lwz r28, 0x10(r1)
mtlr r0
addi r1, r1, 0x20
blr
