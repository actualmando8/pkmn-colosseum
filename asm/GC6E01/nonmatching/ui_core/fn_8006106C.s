stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r24, 0x10(r1)
mr r25, r3
mr r26, r4
mr r27, r5
mr r28, r6
mr r29, r7
bl fn_80061D34
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800611F0
mulli r6, r27, 0xb4
lis r5, lbl_803A9A60@ha
lha r4, 0x6(r26)
lis r3, lbl_802EF0A8@ha
addi r0, r5, lbl_803A9A60@l
add r5, r0, r6
addi r31, r5, 0x58
slwi r30, r28, 2
add r5, r31, r30
addi r0, r3, lbl_802EF0A8@l
lfs f0, 0x3c(r5)
mulli r3, r4, 0x1c
fctiwz f0, f0
add r3, r0, r3
lha r3, 0x2(r3)
stfd f0, 0x8(r1)
lwz r0, 0xc(r1)
add r0, r3, r0
extsh r0, r0
sth r0, 0x50(r26)
lha r5, 0x84(r25)
lha r3, 0x50(r26)
lha r4, 0x86(r25)
lha r0, 0x52(r26)
add r3, r5, r3
extsh r3, r3
add r0, r4, r0
extsh r4, r0
bl fn_800FE6D0
bl fn_800FE4D4
mr r3, r25
mr r4, r26
mr r5, r27
mr r6, r28
bl fn_80069A08
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800611C0
slwi r24, r28, 1
lhax r0, r31, r24
cmpwi r0, 0x0
beq @800611F0
slwi r0, r0, 1
li r3, lbl_80478910@sda21
lhzx r6, r3, r0
mr r5, r25
li r3, 0x0
li r4, 0x0
li r7, 0x0
bl fn_801040F0
lis r3, lbl_803A9A60@ha
addi r3, r3, lbl_803A9A60@l
lwz r0, 0x38(r3)
cmpwi r0, 0x3
bne @800611F0
add r4, r31, r30
lfs f1, 0x3c(r3)
addi r4, r4, 0xc
lfs f0, lbl_8047BFA4@sda21(r0)
lfs f2, 0x0(r4)
fadds f1, f2, f1
stfs f1, 0x0(r4)
lfs f1, 0x0(r4)
fcmpo cr0, f1, f0
cror eq, gt, eq
bne @800611F0
lfs f0, lbl_8047BF60@sda21(r0)
stfs f0, 0x0(r4)
lhax r3, r31, r24
subi r0, r3, 0x1
sthx r0, r31, r24
b @800611F0
@800611C0
slwi r0, r28, 1
lhax r0, r31, r0
cmpwi r0, 0x0
beq @800611F0
slwi r0, r0, 1
li r3, lbl_80478910@sda21
lhzx r6, r3, r0
mr r5, r25
li r3, 0x0
li r4, 0x0
li r7, 0x0
bl fn_801040F0
@800611F0
mr r3, r25
mr r4, r26
mr r5, r27
mr r6, r28
mr r7, r29
bl fn_80061D34
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8006122C
mr r5, r25
li r3, -0x8
li r4, -0x8
li r6, 0x40
li r7, 0x0
bl fn_801040F0
@8006122C
lmw r24, 0x10(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
