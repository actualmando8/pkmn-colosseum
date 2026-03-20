stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stmw r26, 0x8(r1)
li r31, 0x1
bl fn_8025D9CC
mr r30, r3
bl fn_8025DA88
cmpwi r3, 0x2
beq @800690D0
bge @800690E8
cmpwi r3, 0x0
bge @80069094
b @800690E8
@80069094
cmpwi r30, 0x4
bne @800690B8
lis r3, lbl_803A9F08@ha
li r0, 0x4
addi r3, r3, lbl_803A9F08@l
li r31, 0x2
addis r3, r3, 0x1
stw r0, -0x3284(r3)
b @800690E8
@800690B8
lis r3, lbl_803A9F08@ha
li r31, 0x2
addi r3, r3, lbl_803A9F08@l
addis r3, r3, 0x1
stw r30, -0x3284(r3)
b @800690E8
@800690D0
lis r3, lbl_803A9F08@ha
li r0, 0x4
addi r3, r3, lbl_803A9F08@l
li r31, 0x4
addis r3, r3, 0x1
stw r0, -0x3284(r3)
@800690E8
lis r3, lbl_803A9F08@ha
addi r4, r3, lbl_803A9F08@l
addis r3, r4, 0x1
lwz r0, -0x3284(r3)
cmpwi r0, 0x4
beq @800691D8
addi r28, r4, 0x1
addi r27, r4, 0x30
li r26, 0x1
b @800691D0
@80069110
lbz r0, 0x4(r28)
cmplwi r0, 0x0
bne @800691C4
bl fn_8006B1D4
clrlwi r30, r3, 16
mr r3, r26
bl fn_8025D89C
clrlwi r29, r3, 16
cmplw r29, r30
bge @8006913C
b @80069140
@8006913C
mr r29, r30
@80069140
mr r3, r26
bl fn_8025D560
clrlwi r0, r29, 16
cmpw r3, r0
bne @800691C4
mr r3, r26
bl fn_8025D560
mr r30, r3
bl fn_8006B1D4
clrlwi r29, r3, 16
mr r3, r26
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r29
bge @80069180
b @80069184
@80069180
mr r0, r29
@80069184
clrlwi r0, r0, 16
cmpw r30, r0
bne @800691C4
subi r0, r30, 0x1
cmpwi r0, 0x0
bge @800691A0
li r0, 0x0
@800691A0
slwi r3, r0, 2
lfs f1, lbl_8047BFE8@sda21(r0)
addis r3, r3, 0x1
subi r3, r3, 0x3274
lfsx f0, r27, r3
fcmpu cr0, f1, f0
bne @800691C4
li r0, 0x1
stb r0, 0x4(r28)
@800691C4
addi r28, r28, 0x1
addi r27, r27, 0x30
addi r26, r26, 0x1
@800691D0
cmpw r26, r31
blt @80069110
@800691D8
lis r3, lbl_803A9F08@ha
addi r3, r3, lbl_803A9F08@l
mtctr r31
cmpwi r31, 0x0
ble @80069208
@800691EC
lbz r0, 0x4(r3)
cmplwi r0, 0x0
bne @80069200
li r3, 0x0
b @8006920C
@80069200
addi r3, r3, 0x1
bdnz @800691EC
@80069208
li r3, 0x1
@8006920C
lmw r26, 0x8(r1)
lwz r0, 0x24(r1)
mtlr r0
addi r1, r1, 0x20
blr
