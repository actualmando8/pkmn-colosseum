stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stmw r26, 0x8(r1)
mr r29, r3
mr r30, r4
mr r31, r5
cmpwi r29, 0x0
blt @8009331C
cmpwi r29, 0x3
ble @80093324
@8009331C
li r0, 0x0
b @800933F4
@80093324
lis r3, lbl_803FB328@ha
slwi r26, r29, 2
addi r27, r3, lbl_803FB328@l
lwzx r0, r27, r26
cmplwi r0, 0x0
beq @80093344
li r0, 0x1
b @800933F4
@80093344
li r3, 0x44a0
li r4, 0x20
bl fn_800E2C04
mr r28, r3
clrlwi r0, r28, 16
cmplwi r0, 0x0
bne @80093374
lis r3, lbl_8026F5A8@ha
li r4, 0x1dd
addi r3, r3, lbl_8026F5A8@l
li r5, lbl_8047C1E8@sda21
bl fn_80196E10
@80093374
mr r3, r28
bl fn_800E27B0
mr r28, r3
li r4, 0x0
li r5, 0x4490
bl memset
stwx r28, r27, r26
lis r3, fn_80093B04@ha
addi r5, r3, fn_80093B04@l
li r0, 0x0
lwzx r26, r27, r26
mr r3, r29
stw r0, 0x4340(r26)
addi r4, r26, 0x20
stw r29, 0x4338(r26)
bl fn_800716C8
mr r3, r26
bl fn_8009F77C
addi r3, r26, 0x18
bl fn_8009F9C8
lis r3, fn_800937F4@ha
mr r5, r26
addi r4, r3, fn_800937F4@l
addi r6, r26, 0x4338
addi r3, r26, 0x20
li r7, 0x4000
li r8, 0x8
li r9, 0x0
bl fn_800A19CC
addi r3, r26, 0x20
bl fn_800A1F94
li r0, 0x1
@800933F4
cmpwi r0, 0x0
bne @80093404
li r3, 0x0
b @800934D0
@80093404
lis r3, lbl_803FB328@ha
slwi r0, r29, 2
addi r4, r3, lbl_803FB328@l
mr r3, r30
lwzx r27, r4, r0
li r26, 0x0
bl strlen
cmplwi r31, 0x0
mr r29, r3
beq @80093438
mr r3, r31
bl strlen
b @8009343C
@80093438
li r3, 0x0
@8009343C
cmplwi r29, 0x7f
bge @8009344C
cmplwi r3, 0x7f
blt @80093454
@8009344C
li r26, 0x0
b @800934CC
@80093454
mr r3, r27
bl fn_8009F7B4
lwz r0, 0x4340(r27)
cmpwi r0, 0x0
bne @800934A8
li r26, 0x1
lis r3, 0x3
stw r26, 0x4340(r27)
addi r0, r3, 0x1
mr r4, r30
addi r3, r27, 0x4344
stw r0, 0x433c(r27)
bl fn_800CA968
cmplwi r31, 0x0
beq @800934A0
mr r4, r31
addi r3, r27, 0x43c4
bl fn_800CA968
b @800934A8
@800934A0
li r0, 0x0
stb r0, 0x43c4(r27)
@800934A8
mr r3, r27
bl fn_8009F890
addi r3, r27, 0x20
li r4, 0x8
bl fn_800A257C
cmpwi r26, 0x0
beq @800934CC
addi r3, r27, 0x18
bl fn_8009FABC
@800934CC
mr r3, r26
@800934D0
lmw r26, 0x8(r1)
lwz r0, 0x24(r1)
mtlr r0
addi r1, r1, 0x20
blr
