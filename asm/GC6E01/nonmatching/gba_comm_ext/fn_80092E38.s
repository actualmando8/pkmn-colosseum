stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stmw r27, 0xc(r1)
mr r30, r3
mr r31, r4
cmpwi r30, 0x0
blt @80092E60
cmpwi r30, 0x3
ble @80092E68
@80092E60
li r0, 0x0
b @80092F38
@80092E68
lis r3, lbl_803FB328@ha
slwi r27, r30, 2
addi r28, r3, lbl_803FB328@l
lwzx r0, r28, r27
cmplwi r0, 0x0
beq @80092E88
li r0, 0x1
b @80092F38
@80092E88
li r3, 0x44a0
li r4, 0x20
bl fn_800E2C04
mr r29, r3
clrlwi r0, r29, 16
cmplwi r0, 0x0
bne @80092EB8
lis r3, lbl_8026F5A8@ha
li r4, 0x1dd
addi r3, r3, lbl_8026F5A8@l
li r5, lbl_8047C1E8@sda21
bl fn_80196E10
@80092EB8
mr r3, r29
bl fn_800E27B0
mr r29, r3
li r4, 0x0
li r5, 0x4490
bl memset
stwx r29, r28, r27
lis r3, fn_80093B04@ha
addi r5, r3, fn_80093B04@l
li r0, 0x0
lwzx r27, r28, r27
mr r3, r30
stw r0, 0x4340(r27)
addi r4, r27, 0x20
stw r30, 0x4338(r27)
bl fn_800716C8
mr r3, r27
bl fn_8009F77C
addi r3, r27, 0x18
bl fn_8009F9C8
lis r3, fn_800937F4@ha
mr r5, r27
addi r4, r3, fn_800937F4@l
addi r6, r27, 0x4338
addi r3, r27, 0x20
li r7, 0x4000
li r8, 0x8
li r9, 0x0
bl fn_800A19CC
addi r3, r27, 0x20
bl fn_800A1F94
li r0, 0x1
@80092F38
cmpwi r0, 0x0
bne @80092F48
li r3, 0x0
b @80092FB4
@80092F48
lis r3, lbl_803FB328@ha
slwi r0, r30, 2
addi r3, r3, lbl_803FB328@l
li r27, 0x0
lwzx r28, r3, r0
mr r3, r28
bl fn_8009F7B4
lwz r0, 0x4340(r28)
cmpwi r0, 0x0
bne @80092F8C
li r0, 0xb
lis r3, 0x3
stw r0, 0x4340(r28)
addi r0, r3, 0xb
li r27, 0x1
stw r0, 0x433c(r28)
stw r31, 0x4344(r28)
@80092F8C
mr r3, r28
bl fn_8009F890
addi r3, r28, 0x20
li r4, 0x8
bl fn_800A257C
cmpwi r27, 0x0
beq @80092FB0
addi r3, r28, 0x18
bl fn_8009FABC
@80092FB0
mr r3, r27
@80092FB4
lmw r27, 0xc(r1)
lwz r0, 0x24(r1)
mtlr r0
addi r1, r1, 0x20
blr
