stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
stw r28, 0x10(r1)
mr r28, r3
mr r29, r4
mr r30, r5
cmplwi r30, 0x0
beq @80089094
mr r3, r30
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80089094
li r3, 0x0
b @80089360
@80089094
lwz r11, 0x0(r29)
cmplwi r30, 0x0
lwz r12, 0x4(r29)
lwz r31, 0x8(r29)
rlwinm r5, r11, 0, 16, 23
rlwinm r3, r12, 0, 16, 23
rlwinm r10, r11, 0, 8, 15
rlwinm r0, r31, 0, 16, 23
rlwinm r7, r12, 0, 8, 15
rlwinm r4, r31, 0, 8, 15
slwi r9, r11, 24
slwi r8, r5, 8
slwi r6, r12, 24
slwi r5, r3, 8
slwi r3, r31, 24
slwi r0, r0, 8
srwi r10, r10, 8
or r8, r9, r8
srwi r7, r7, 8
or r5, r6, r5
srwi r4, r4, 8
or r0, r3, r0
srwi r9, r11, 24
or r8, r10, r8
srwi r6, r12, 24
or r5, r7, r5
srwi r3, r31, 24
or r0, r4, r0
or r7, r9, r8
or r4, r6, r5
stw r7, 0x0(r28)
or r0, r3, r0
lhz r31, 0xe(r29)
stw r4, 0x4(r28)
stw r0, 0x8(r28)
beq @80089138
mr r3, r30
bl fn_8011F5C8
clrlwi r0, r3, 16
slwi r0, r0, 16
or r31, r31, r0
@80089138
rlwinm r0, r31, 0, 16, 23
rlwinm r4, r31, 0, 8, 15
slwi r3, r31, 24
lhz r5, 0xe(r29)
slwi r0, r0, 8
srwi r4, r4, 8
or r0, r3, r0
srwi r3, r31, 24
or r0, r4, r0
cmplwi r5, 0x32
or r0, r3, r0
addi r31, r28, 0x10
stw r0, 0xc(r28)
li r0, 0x0
beq @8008917C
cmplwi r5, 0x1e
bne @80089180
@8008917C
li r0, 0x1
@80089180
cmpwi r0, 0x0
bne @800891A0
lis r3, lbl_8026F568@ha
lis r5, lbl_8026F574@ha
addi r3, r3, lbl_8026F568@l
li r4, 0xb7
addi r5, r5, lbl_8026F574@l
bl fn_80196E10
@800891A0
mr r10, r29
lhz r3, 0xe(r29)
cmpwi r3, 0x0
ble @80089314
srwi r0, r3, 2
cmplwi r0, 0x0
mtctr r0
beq @800892CC
@800891C0
lhz r9, 0x12(r10)
lhz r11, 0x10(r10)
addi r10, r10, 0x4
slwi r0, r9, 16
lhz r9, 0x12(r10)
or r11, r11, r0
rlwinm r7, r11, 0, 8, 15
slwi r0, r9, 16
rlwinm r4, r11, 0, 16, 23
srwi r8, r11, 24
slwi r5, r11, 24
lhz r11, 0x10(r10)
slwi r4, r4, 8
addi r10, r10, 0x4
or r11, r11, r0
srwi r6, r7, 8
or r0, r5, r4
lhz r9, 0x12(r10)
or r0, r6, r0
rlwinm r4, r11, 0, 16, 23
or r0, r8, r0
rlwinm r7, r11, 0, 8, 15
stw r0, 0x0(r31)
srwi r8, r11, 24
slwi r5, r11, 24
slwi r4, r4, 8
lhz r11, 0x10(r10)
slwi r0, r9, 16
addi r10, r10, 0x4
srwi r6, r7, 8
or r11, r11, r0
or r0, r5, r4
or r0, r6, r0
lhz r9, 0x12(r10)
or r0, r8, r0
rlwinm r7, r11, 0, 8, 15
rlwinm r4, r11, 0, 16, 23
addi r31, r31, 0x4
stw r0, 0x0(r31)
srwi r8, r11, 24
slwi r5, r11, 24
lhz r11, 0x10(r10)
slwi r0, r9, 16
slwi r4, r4, 8
or r11, r11, r0
srwi r6, r7, 8
or r0, r5, r4
addi r31, r31, 0x4
rlwinm r4, r11, 0, 16, 23
rlwinm r7, r11, 0, 8, 15
or r0, r6, r0
slwi r5, r11, 24
or r0, r8, r0
slwi r4, r4, 8
stw r0, 0x0(r31)
srwi r6, r7, 8
or r0, r5, r4
srwi r8, r11, 24
or r0, r6, r0
addi r31, r31, 0x4
or r0, r8, r0
addi r10, r10, 0x4
stw r0, 0x0(r31)
addi r31, r31, 0x4
bdnz @800891C0
andi. r3, r3, 0x3
beq @80089314
@800892CC
mtctr r3
@800892D0
lhz r9, 0x12(r10)
lhz r11, 0x10(r10)
addi r10, r10, 0x4
slwi r0, r9, 16
or r11, r11, r0
rlwinm r4, r11, 0, 16, 23
rlwinm r7, r11, 0, 8, 15
slwi r5, r11, 24
slwi r4, r4, 8
srwi r8, r11, 24
srwi r6, r7, 8
or r0, r5, r4
or r0, r6, r0
or r0, r8, r0
stw r0, 0x0(r31)
addi r31, r31, 0x4
bdnz @800892D0
@80089314
cmplwi r30, 0x0
beq @8008935C
mr r3, r30
mr r4, r31
bl fn_8008AE18
addi r3, r31, 0x64
li r4, 0x0
li r5, 0xc
bl memset
addi r28, r31, 0x64
li r29, 0x0
@80089340
mr r3, r29
bl fn_80265F14
stb r3, 0x0(r28)
addi r28, r28, 0x1
addi r29, r29, 0x1
cmpwi r29, 0xb
blt @80089340
@8008935C
li r3, 0x1
@80089360
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
lwz r28, 0x10(r1)
mtlr r0
addi r1, r1, 0x20
blr
