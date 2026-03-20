stwu r1, -0x50(r1)
mflr r0
stw r0, 0x54(r1)
stmw r20, 0x20(r1)
mr r28, r3
mr r29, r4
bl fn_80073C38
cmpwi r3, 0x0
beq @80071ECC
b @8007228C
@80071ECC
li r0, 0x33
mr r3, r28
stw r0, 0x18(r1)
addi r4, r1, 0x18
addi r5, r1, 0xd
bl fn_8025F648
cmpwi r3, 0x0
beq @80071EF4
li r3, 0xb
b @80071FE8
@80071EF4
lis r4, 0x8000
lis r3, 0x1062
lwz r0, 0xf8(r4)
addi r3, r3, 0x4dd3
srwi r0, r0, 2
mulhwu r0, r3, r0
srwi r0, r0, 6
mulli r24, r0, 0x64
bl OSGetTick
lis r5, lbl_803B6E18@ha
lis r4, lbl_803B6E08@ha
slwi r6, r28, 3
mr r23, r3
addi r0, r5, lbl_803B6E18@l
slwi r22, r28, 2
add r25, r0, r6
addi r21, r4, lbl_803B6E08@l
addi r20, r25, 0x4
@80071F3C
bl OSGetTick
subf r0, r23, r3
cmplw r0, r24
ble @80071F54
li r3, 0x1
b @80071FD4
@80071F54
mr r3, r28
addi r4, r1, 0xa
bl fn_8025F3F4
cmpwi r3, 0x0
beq @80071F70
li r3, 0x2
b @80071FD4
@80071F70
lbz r0, 0xa(r1)
andi. r0, r0, 0xa
cmpwi r0, 0x8
beq @80071FB0
lwz r12, 0x0(r25)
cmplwi r12, 0x0
beq @80071F9C
mr r3, r28
lwz r4, 0x0(r20)
mtctr r12
bctrl
@80071F9C
lwzx r0, r21, r22
cmpwi r0, 0x0
beq @80071F3C
li r3, 0x3e8
b @80071FD4
@80071FB0
mr r3, r28
addi r4, r1, 0x1c
addi r5, r1, 0xd
bl fn_8025F584
cmpwi r3, 0x0
beq @80071FD0
li r3, 0x3
b @80071FD4
@80071FD0
li r3, 0x0
@80071FD4
cmpwi r3, 0x0
beq @80071FE4
addi r3, r3, 0xb
b @80071FE8
@80071FE4
li r3, 0x0
@80071FE8
cmpwi r3, 0x0
beq @80071FF4
b @8007228C
@80071FF4
lwz r0, 0x1c(r1)
srwi r0, r0, 24
cmplwi r0, 0x33
beq @8007200C
li r3, 0xf
b @8007228C
@8007200C
lis r4, lbl_803B6E18@ha
lis r3, lbl_803B6E08@ha
slwi r5, r28, 3
slwi r27, r28, 2
addi r0, r4, lbl_803B6E18@l
addi r26, r3, lbl_803B6E08@l
add r31, r0, r5
li r23, 0x0
addi r30, r31, 0x4
lis r3, 0x1062
mr r22, r29
addi r25, r3, 0x4dd3
lis r24, 0x8000
@80072040
lwz r0, 0xf8(r24)
srwi r0, r0, 2
mulhwu r0, r25, r0
srwi r0, r0, 6
mulli r20, r0, 0x64
bl OSGetTick
mr r21, r3
@8007205C
bl OSGetTick
subf r0, r21, r3
cmplw r0, r20
ble @80072074
li r3, 0x1
b @800720F4
@80072074
mr r3, r28
addi r4, r1, 0x9
bl fn_8025F3F4
cmpwi r3, 0x0
beq @80072090
li r3, 0x2
b @800720F4
@80072090
lbz r0, 0x9(r1)
andi. r0, r0, 0xa
cmpwi r0, 0x8
beq @800720D0
lwz r12, 0x0(r31)
cmplwi r12, 0x0
beq @800720BC
mr r3, r28
lwz r4, 0x0(r30)
mtctr r12
bctrl
@800720BC
lwzx r0, r26, r27
cmpwi r0, 0x0
beq @8007205C
li r3, 0x3e8
b @800720F4
@800720D0
mr r3, r28
addi r4, r1, 0x14
addi r5, r1, 0xc
bl fn_8025F584
cmpwi r3, 0x0
beq @800720F0
li r3, 0x3
b @800720F4
@800720F0
li r3, 0x0
@800720F4
cmpwi r3, 0x0
beq @80072100
b @80072130
@80072100
lwz r0, 0x14(r1)
stw r0, 0x0(r22)
lwzx r0, r26, r27
cmpwi r0, 0x0
beq @8007211C
li r3, 0x3e8
b @80072130
@8007211C
addi r23, r23, 0x4
addi r22, r22, 0x4
cmpwi r23, 0x10
blt @80072040
li r3, 0x0
@80072130
cmpwi r3, 0x0
beq @80072140
addi r3, r3, 0xf
b @8007228C
@80072140
lwz r5, 0xc(r29)
lis r6, lbl_803B6E08@ha
slwi r24, r28, 2
li r23, 0x0
srwi r3, r5, 24
rlwinm r0, r5, 24, 16, 23
rlwinm r4, r5, 8, 8, 15
slwi r5, r5, 24
or r0, r3, r0
addi r25, r6, lbl_803B6E08@l
or r0, r4, r0
or r0, r5, r0
clrlwi r0, r0, 16
slwi r22, r0, 2
lis r3, 0x1062
lis r27, 0x8000
addi r26, r3, 0x4dd3
b @8007226C
@80072188
lwz r0, 0xf8(r27)
srwi r0, r0, 2
mulhwu r0, r26, r0
srwi r0, r0, 6
mulli r20, r0, 0x64
bl OSGetTick
mr r21, r3
@800721A4
bl OSGetTick
subf r0, r21, r3
cmplw r0, r20
ble @800721BC
li r3, 0x1
b @8007223C
@800721BC
mr r3, r28
addi r4, r1, 0x8
bl fn_8025F3F4
cmpwi r3, 0x0
beq @800721D8
li r3, 0x2
b @8007223C
@800721D8
lbz r0, 0x8(r1)
andi. r0, r0, 0xa
cmpwi r0, 0x8
beq @80072218
lwz r12, 0x0(r31)
cmplwi r12, 0x0
beq @80072204
mr r3, r28
lwz r4, 0x0(r30)
mtctr r12
bctrl
@80072204
lwzx r0, r25, r24
cmpwi r0, 0x0
beq @800721A4
li r3, 0x3e8
b @8007223C
@80072218
mr r3, r28
addi r4, r1, 0x10
addi r5, r1, 0xb
bl fn_8025F584
cmpwi r3, 0x0
beq @80072238
li r3, 0x3
b @8007223C
@80072238
li r3, 0x0
@8007223C
cmpwi r3, 0x0
beq @80072248
b @80072278
@80072248
lwz r0, 0x10(r1)
add r3, r29, r23
stw r0, 0x10(r3)
lwzx r0, r25, r24
cmpwi r0, 0x0
beq @80072268
li r3, 0x3e8
b @80072278
@80072268
addi r23, r23, 0x4
@8007226C
cmpw r23, r22
blt @80072188
li r3, 0x0
@80072278
cmpwi r3, 0x0
beq @80072288
addi r3, r3, 0x12
b @8007228C
@80072288
li r3, 0x0
@8007228C
lmw r20, 0x20(r1)
lwz r0, 0x54(r1)
mtlr r0
addi r1, r1, 0x50
blr
