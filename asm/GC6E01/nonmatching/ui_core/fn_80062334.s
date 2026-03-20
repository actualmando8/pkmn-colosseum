stwu r1, -0x70(r1)
mflr r0
stw r0, 0x74(r1)
stmw r26, 0x58(r1)
bl fn_8025DA88
lis r4, lbl_803A9E40@ha
lfs f5, lbl_8047BFA4@sda21(r0)
addi r7, r4, lbl_803A9E40@l
lfs f4, lbl_8047BFA8@sda21(r0)
lfs f3, lbl_8047BFB0@sda21(r0)
addi r6, r7, 0x18
lfs f2, lbl_8047BFB4@sda21(r0)
addi r5, r7, 0x30
lfs f1, lbl_8047BF68@sda21(r0)
addi r0, r7, 0x48
lfs f0, lbl_8047BFB8@sda21(r0)
lis r4, lbl_803A9A60@ha
addi r11, r4, lbl_803A9A60@l
stw r7, 0x8(r1)
addi r8, r1, 0x8
addi r9, r1, 0x34
stw r6, 0xc(r1)
mr r7, r11
addi r10, r1, 0x18
li r31, 0x0
stw r5, 0x10(r1)
stw r0, 0x14(r1)
stfs f5, 0x34(r1)
stfs f4, 0x38(r1)
stfs f3, 0x3c(r1)
stfs f2, 0x40(r1)
stfs f1, 0x44(r1)
stfs f0, 0x48(r1)
stfs f0, 0x18(r1)
stfs f1, 0x1c(r1)
stfs f2, 0x20(r1)
stfs f3, 0x24(r1)
stfs f4, 0x28(r1)
stfs f5, 0x2c(r1)
@800623D0
cmpwi r3, 0x2
addi r30, r7, 0x58
bne @80062488
mr r4, r30
mr r5, r30
mr r6, r9
mr r12, r10
li r0, 0x6
mtctr r0
@800623F4
lwz r0, 0x4(r11)
cmpwi r0, 0x1
beq @80062424
bge @80062434
cmpwi r0, 0x0
bge @80062410
b @80062434
@80062410
li r0, 0x3
lfs f0, lbl_8047BF60@sda21(r0)
sth r0, 0x0(r4)
stfs f0, 0xc(r5)
b @80062434
@80062424
li r0, 0x0
lfs f0, lbl_8047BF60@sda21(r0)
sth r0, 0x0(r4)
stfs f0, 0xc(r5)
@80062434
cmpwi r31, 0x2
bge @80062458
lfs f0, lbl_8047BFBC@sda21(r0)
lfs f1, lbl_8047BF60@sda21(r0)
stfs f0, 0x3c(r5)
lfs f0, 0x0(r6)
stfs f1, 0x54(r5)
stfs f0, 0x24(r5)
b @80062470
@80062458
lfs f0, lbl_8047BFC0@sda21(r0)
lfs f1, lbl_8047BF60@sda21(r0)
stfs f0, 0x3c(r5)
lfs f0, 0x0(r12)
stfs f1, 0x54(r5)
stfs f0, 0x24(r5)
@80062470
addi r4, r4, 0x2
addi r5, r5, 0x4
addi r6, r6, 0x4
addi r12, r12, 0x4
bdnz @800623F4
b @800625A4
@80062488
srwi r26, r31, 31
clrlwi r0, r31, 31
xor r0, r0, r26
li r12, 0x0
mr r4, r12
mr r5, r30
mr r6, r30
subf r0, r26, r0
li r29, 0x6
mtctr r29
@800624B0
lwz r26, 0x4(r11)
lwz r27, 0x0(r8)
cmpwi r26, 0x1
lwzx r28, r27, r4
beq @800624E8
bge @800624F8
cmpwi r26, 0x0
bge @800624D4
b @800624F8
@800624D4
li r26, 0x3
lfs f0, lbl_8047BF60@sda21(r0)
sth r26, 0x0(r5)
stfs f0, 0xc(r6)
b @800624F8
@800624E8
li r26, 0x0
lfs f0, lbl_8047BF60@sda21(r0)
sth r26, 0x0(r5)
stfs f0, 0xc(r6)
@800624F8
cmpwi r0, 0x0
beq @8006254C
lis r26, 0x5555
lfs f0, lbl_8047BFC0@sda21(r0)
addi r27, r26, 0x5556
slwi r26, r28, 2
mulhw r29, r27, r12
stfs f0, 0x3c(r6)
lfs f0, lbl_8047BF60@sda21(r0)
addi r28, r1, 0x18
addi r26, r26, 0x24
stfs f0, 0x54(r6)
srwi r27, r29, 31
add r27, r29, r27
mulli r27, r27, 0x3
subf r27, r27, r12
slwi r27, r27, 2
addi r27, r27, 0xc
lfsx f0, r28, r27
stfsx f0, r30, r26
b @80062590
@8006254C
lis r27, 0x5555
lfs f0, lbl_8047BFBC@sda21(r0)
addi r27, r27, 0x5556
slwi r29, r28, 2
mulhw r26, r27, r12
stfs f0, 0x3c(r6)
lfs f0, lbl_8047BF60@sda21(r0)
addi r28, r1, 0x34
addi r29, r29, 0x24
stfs f0, 0x54(r6)
srwi r27, r26, 31
add r27, r26, r27
mulli r27, r27, 0x3
subf r27, r27, r12
slwi r27, r27, 2
lfsx f0, r28, r27
stfsx f0, r30, r29
@80062590
addi r4, r4, 0x4
addi r5, r5, 0x2
addi r6, r6, 0x4
addi r12, r12, 0x1
bdnz @800624B0
@800625A4
addi r7, r7, 0xb4
addi r8, r8, 0x4
addi r31, r31, 0x1
cmpwi r31, 0x4
blt @800623D0
mr r5, r11
li r6, 0x0
li r0, 0x4
mtctr r0
@800625C8
cmpwi r3, 0x2
addi r7, r5, 0x328
bne @80062614
cmpwi r6, 0x2
bge @800625F8
lfs f0, lbl_8047BFBC@sda21(r0)
lfs f1, lbl_8047BF60@sda21(r0)
stfs f0, 0x4(r7)
lfs f0, lbl_8047BFC4@sda21(r0)
stfs f1, 0x8(r7)
stfs f0, 0x0(r7)
b @80062660
@800625F8
lfs f0, lbl_8047BFC0@sda21(r0)
lfs f1, lbl_8047BF60@sda21(r0)
stfs f0, 0x4(r7)
lfs f0, lbl_8047BFC4@sda21(r0)
stfs f1, 0x8(r7)
stfs f0, 0x0(r7)
b @80062660
@80062614
srwi r4, r6, 31
clrlwi r0, r6, 31
xor r0, r0, r4
subf r0, r4, r0
cmpwi r0, 0x0
beq @80062648
lfs f0, lbl_8047BFC0@sda21(r0)
lfs f1, lbl_8047BF60@sda21(r0)
stfs f0, 0x4(r7)
lfs f0, lbl_8047BFC4@sda21(r0)
stfs f1, 0x8(r7)
stfs f0, 0x0(r7)
b @80062660
@80062648
lfs f0, lbl_8047BFBC@sda21(r0)
lfs f1, lbl_8047BF60@sda21(r0)
stfs f0, 0x4(r7)
lfs f0, lbl_8047BFC4@sda21(r0)
stfs f1, 0x8(r7)
stfs f0, 0x0(r7)
@80062660
addi r5, r5, 0xc
addi r6, r6, 0x1
bdnz @800625C8
lfs f4, lbl_8047BF70@sda21(r0)
lis r3, lbl_803A9A60@ha
lfs f3, lbl_8047BF90@sda21(r0)
addi r3, r3, lbl_803A9A60@l
lfs f2, lbl_8047BF60@sda21(r0)
li r0, 0x0
lfs f1, lbl_8047BFBC@sda21(r0)
lfs f0, lbl_8047BFC0@sda21(r0)
stb r0, 0x368(r3)
stfs f4, 0x358(r11)
stfs f3, 0x35c(r11)
stfs f4, 0x360(r3)
stfs f3, 0x364(r3)
stfs f2, 0x54(r3)
stfs f1, 0x4c(r3)
stfs f2, 0x44(r3)
stfs f2, 0x50(r3)
stfs f0, 0x48(r3)
stfs f2, 0x40(r3)
lmw r26, 0x58(r1)
lwz r0, 0x74(r1)
mtlr r0
addi r1, r1, 0x70
blr
