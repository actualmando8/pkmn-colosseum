stwu r1, -0x70(r1)
mflr r0
stw r0, 0x74(r1)
stmw r26, 0x58(r1)
lis r6, lbl_8026F488@ha
lis r9, lbl_803FB2F8@ha
addi r8, r6, lbl_8026F488@l
li r7, 0x0
lwz r29, 0x0(r8)
addi r6, r9, lbl_803FB2F8@l
lwz r30, 0x4(r8)
lwz r31, 0x8(r8)
lwz r12, 0xc(r8)
lhz r0, 0x10(r8)
sth r3, 0x0(r6)
sth r4, 0x2(r6)
sth r5, 0x4(r6)
stw r29, 0x8(r1)
stw r30, 0xc(r1)
stw r31, 0x10(r1)
stw r12, 0x14(r1)
sth r0, 0x18(r1)
clrlwi r11, r0, 16
clrlwi r4, r11, 16
mr r10, r29
mr r9, r30
mr r8, r31
mr r5, r12
clrlwi r3, r4, 16
@80088530
stw r29, 0x1c(r1)
addi r26, r1, 0x44
lhz r27, 0x0(r6)
li r28, 0x0
stw r30, 0x20(r1)
stw r31, 0x24(r1)
stw r12, 0x28(r1)
sth r11, 0x2c(r1)
li r0, 0x3
mtctr r0
@80088558
stw r10, 0x30(r1)
li r0, 0x0
stw r9, 0x34(r1)
stw r8, 0x38(r1)
stw r5, 0x3c(r1)
sth r4, 0x40(r1)
cmpwi r0, 0x0
stw r10, 0x44(r1)
stw r9, 0x48(r1)
stw r8, 0x4c(r1)
stw r5, 0x50(r1)
sth r3, 0x54(r1)
blt @800885A4
cmpwi r0, 0x3
bge @800885A4
cmpwi r28, 0x0
blt @800885A4
cmpwi r28, 0x3
blt @800885AC
@800885A4
li r0, 0x0
b @800885B0
@800885AC
lhz r0, 0x0(r26)
@800885B0
clrlwi r0, r0, 16
cmplw r0, r27
bne @800885C4
li r0, 0x1
b @80088684
@800885C4
li r0, 0x1
cmpwi r0, 0x0
stw r10, 0x44(r1)
stw r9, 0x48(r1)
stw r8, 0x4c(r1)
stw r5, 0x50(r1)
sth r3, 0x54(r1)
blt @800885FC
cmpwi r0, 0x3
bge @800885FC
cmpwi r28, 0x0
blt @800885FC
cmpwi r28, 0x3
blt @80088604
@800885FC
li r0, 0x0
b @80088608
@80088604
lhz r0, 0x2(r26)
@80088608
clrlwi r0, r0, 16
cmplw r0, r27
bne @8008861C
li r0, 0x1
b @80088684
@8008861C
li r0, 0x2
cmpwi r0, 0x0
stw r10, 0x44(r1)
stw r9, 0x48(r1)
stw r8, 0x4c(r1)
stw r5, 0x50(r1)
sth r3, 0x54(r1)
blt @80088654
cmpwi r0, 0x3
bge @80088654
cmpwi r28, 0x0
blt @80088654
cmpwi r28, 0x3
blt @8008865C
@80088654
li r0, 0x0
b @80088660
@8008865C
lhz r0, 0x4(r26)
@80088660
clrlwi r0, r0, 16
cmplw r0, r27
bne @80088674
li r0, 0x1
b @80088684
@80088674
addi r26, r26, 0x6
addi r28, r28, 0x1
bdnz @80088558
li r0, 0x0
@80088684
cmpwi r0, 0x0
bne @80088694
li r3, 0x2
b @800886BC
@80088694
addi r6, r6, 0x2
addi r7, r7, 0x1
cmpwi r7, 0x3
blt @80088530
li r3, 0x387
bl fn_800FF730
bl fn_800F0308
lis r3, lbl_803FB2F8@ha
addi r3, r3, lbl_803FB2F8@l
lwz r3, 0x8(r3)
@800886BC
lmw r26, 0x58(r1)
lwz r0, 0x74(r1)
mtlr r0
addi r1, r1, 0x70
blr
