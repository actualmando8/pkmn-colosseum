stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stfd f31, 0x20(r1)
psq_st f31, 0x28(r1), 0, 0
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
mr r30, r3
bl fn_800D37CC
xoris r3, r3, 0x8000
lis r0, 0x4330
stw r3, 0xc(r1)
lfd f1, lbl_8047BF80@sda21(r0)
stw r0, 0x8(r1)
lfd f0, 0x8(r1)
fsubs f31, f0, f1
bl fn_800D3088
lis r5, 0x4330
lis r4, lbl_803A9A60@ha
stw r3, 0x14(r1)
addi r31, r4, lbl_803A9A60@l
lwz r0, 0x38(r31)
stw r5, 0x10(r1)
lfd f1, lbl_8047BF88@sda21(r0)
cmpwi r0, 0x7
lfd f0, 0x10(r1)
fsubs f0, f0, f1
fdivs f2, f0, f31
stfs f2, 0x3c(r31)
beq @8005E620
bge @8005E078
cmpwi r0, 0x3
beq @8005E398
bge @8005E068
cmpwi r0, 0x1
beq @8005E188
bge @8005E1D0
cmpwi r0, 0x0
bge @8005E0A0
b @8005E670
@8005E068
cmpwi r0, 0x5
beq @8005E4F8
bge @8005E520
b @8005E3DC
@8005E078
cmpwi r0, 0xb
beq @8005E654
bge @8005E094
cmpwi r0, 0x9
beq @8005E670
bge @8005E3C0
b @8005E648
@8005E094
cmpwi r0, 0x64
beq @8005E670
b @8005E670
@8005E0A0
bl fn_80069048
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005E670
lis r3, lbl_803A9A60@ha
addi r30, r3, lbl_803A9A60@l
lbz r0, 0x34(r30)
cmplwi r0, 0x0
bne @8005E17C
li r0, 0x0
stb r0, 0x34(r30)
stw r0, 0x2c(r30)
bl fn_8025DA88
cmpwi r3, 0x2
bne @8005E0F0
lis r3, lbl_803A9A60@ha
li r0, 0x4
addi r3, r3, lbl_803A9A60@l
stw r0, 0x30(r3)
b @8005E100
@8005E0F0
lis r3, lbl_803A9A60@ha
li r0, 0x2
addi r3, r3, lbl_803A9A60@l
stw r0, 0x30(r3)
@8005E100
li r0, 0x0
stw r0, 0x10(r30)
stw r0, 0xc(r30)
stw r0, 0x18(r30)
stw r0, 0x14(r30)
stw r0, 0x20(r30)
stw r0, 0x1c(r30)
stw r0, 0x28(r30)
stw r0, 0x24(r30)
bl fn_8025DA88
cmpwi r3, 0x2
bne @8005E138
li r3, 0x0
b @8005E13C
@8005E138
li r3, 0x1
@8005E13C
li r0, 0x1
cmpwi r3, 0x0
stb r0, 0x34(r30)
bne @8005E154
li r3, 0x5c4
b @8005E158
@8005E154
li r3, 0x5c3
@8005E158
lis r5, fn_800626CC@ha
li r4, 0x0
addi r5, r5, fn_800626CC@l
li r6, 0x0
li r7, 0x0
bl fn_8017B000
li r0, 0x1
stw r0, 0x38(r31)
b @8005E670
@8005E17C
li r0, 0x2
stw r0, 0x38(r31)
b @8005E670
@8005E188
bl fn_80069048
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005E1B4
lis r3, lbl_803A9A60@ha
addi r3, r3, lbl_803A9A60@l
lbz r0, 0x34(r3)
cmplwi r0, 0x0
beq @8005E1B4
li r0, 0x1
b @8005E1B8
@8005E1B4
li r0, 0x0
@8005E1B8
clrlwi r0, r0, 24
cmplwi r0, 0x0
beq @8005E670
li r0, 0x2
stw r0, 0x38(r31)
b @8005E670
@8005E1D0
bl fn_80060A28
lis r3, lbl_803A9A60@ha
addi r3, r3, lbl_803A9A60@l
mr r5, r3
li r0, 0x4
mtctr r0
@8005E1E8
addi r4, r5, 0x58
lfs f1, 0x3c(r4)
lfs f0, 0x54(r4)
fcmpu cr0, f1, f0
beq @8005E204
li r0, 0x0
b @8005E358
@8005E204
addi r6, r4, 0x4
lfs f1, 0x3c(r6)
lfs f0, 0x54(r6)
fcmpu cr0, f1, f0
beq @8005E220
li r0, 0x0
b @8005E358
@8005E220
addi r6, r4, 0x8
lfs f1, 0x3c(r6)
lfs f0, 0x54(r6)
fcmpu cr0, f1, f0
beq @8005E23C
li r0, 0x0
b @8005E358
@8005E23C
addi r6, r4, 0xc
lfs f1, 0x3c(r6)
lfs f0, 0x54(r6)
fcmpu cr0, f1, f0
beq @8005E258
li r0, 0x0
b @8005E358
@8005E258
addi r6, r4, 0x10
lfs f1, 0x3c(r6)
lfs f0, 0x54(r6)
fcmpu cr0, f1, f0
beq @8005E274
li r0, 0x0
b @8005E358
@8005E274
addi r6, r4, 0x14
lfs f1, 0x3c(r6)
lfs f0, 0x54(r6)
fcmpu cr0, f1, f0
beq @8005E290
li r0, 0x0
b @8005E358
@8005E290
addi r5, r5, 0xb4
bdnz @8005E1E8
addi r4, r3, 0x328
lfs f1, 0x4(r4)
lfs f0, 0x8(r4)
fcmpu cr0, f1, f0
beq @8005E2B4
li r0, 0x0
b @8005E358
@8005E2B4
addi r5, r3, 0xc
addi r4, r5, 0x328
lfs f1, 0x4(r4)
lfs f0, 0x8(r4)
fcmpu cr0, f1, f0
beq @8005E2D4
li r0, 0x0
b @8005E358
@8005E2D4
addi r5, r5, 0xc
addi r4, r5, 0x328
lfs f1, 0x4(r4)
lfs f0, 0x8(r4)
fcmpu cr0, f1, f0
beq @8005E2F4
li r0, 0x0
b @8005E358
@8005E2F4
addi r5, r5, 0xc
addi r4, r5, 0x328
lfs f1, 0x4(r4)
lfs f0, 0x8(r4)
fcmpu cr0, f1, f0
beq @8005E314
li r0, 0x0
b @8005E358
@8005E314
lfs f1, 0x48(r3)
lfs f0, 0x50(r3)
fcmpu cr0, f1, f0
beq @8005E32C
li r0, 0x0
b @8005E358
@8005E32C
addi r3, r3, 0x4
lfs f1, 0x48(r3)
lfs f0, 0x50(r3)
fcmpu cr0, f1, f0
beq @8005E348
li r0, 0x0
b @8005E358
@8005E348
lis r3, lbl_803A9A60@ha
li r0, 0x1
addi r3, r3, lbl_803A9A60@l
stb r0, 0x368(r3)
@8005E358
clrlwi r0, r0, 24
cmplwi r0, 0x0
beq @8005E670
lis r3, lbl_803A9A60@ha
addi r3, r3, lbl_803A9A60@l
lwz r0, 0x4(r3)
cmpwi r0, 0x0
bne @8005E38C
lfs f0, lbl_8047BF60@sda21(r0)
li r0, 0x3
stw r0, 0x38(r31)
stfs f0, 0x3b8(r3)
b @8005E670
@8005E38C
li r0, 0x4
stw r0, 0x38(r31)
b @8005E670
@8005E398
lfs f1, 0x3b8(r31)
lfs f0, lbl_8047BF64@sda21(r0)
fadds f1, f1, f2
fcmpo cr0, f1, f0
stfs f1, 0x3b8(r31)
cror eq, gt, eq
bne @8005E670
li r0, 0xa
stw r0, 0x38(r31)
b @8005E670
@8005E3C0
lwz r3, 0x3bc(r31)
bl fn_801666BC
cmpwi r3, 0x0
bne @8005E670
li r0, 0xb
stw r0, 0x38(r31)
b @8005E670
@8005E3DC
mr r7, r31
li r3, 0x1
li r5, 0x0
lfs f1, lbl_8047BF68@sda21(r0)
@8005E3EC
addi r6, r7, 0x58
li r4, 0x0
li r0, 0x2
mtctr r0
@8005E3FC
lfs f0, 0x84(r6)
lfs f3, 0x6c(r6)
fcmpu cr0, f0, f3
beq @8005E43C
lfs f2, 0x9c(r6)
lfs f0, 0x3c(r31)
fmuls f0, f2, f0
fmuls f0, f0, f1
fsubs f0, f3, f0
stfs f0, 0x6c(r6)
lfs f0, 0x6c(r6)
lfs f2, 0x84(r6)
fcmpo cr0, f0, f2
bge @8005E438
stfs f2, 0x6c(r6)
@8005E438
li r3, 0x0
@8005E43C
addi r6, r6, 0x4
lfs f0, 0x84(r6)
lfs f3, 0x6c(r6)
fcmpu cr0, f0, f3
beq @8005E480
lfs f2, 0x9c(r6)
lfs f0, 0x3c(r31)
fmuls f0, f2, f0
fmuls f0, f0, f1
fsubs f0, f3, f0
stfs f0, 0x6c(r6)
lfs f0, 0x6c(r6)
lfs f2, 0x84(r6)
fcmpo cr0, f0, f2
bge @8005E47C
stfs f2, 0x6c(r6)
@8005E47C
li r3, 0x0
@8005E480
addi r6, r6, 0x4
lfs f0, 0x84(r6)
lfs f3, 0x6c(r6)
fcmpu cr0, f0, f3
beq @8005E4C4
lfs f2, 0x9c(r6)
lfs f0, 0x3c(r31)
fmuls f0, f2, f0
fmuls f0, f0, f1
fsubs f0, f3, f0
stfs f0, 0x6c(r6)
lfs f0, 0x6c(r6)
lfs f2, 0x84(r6)
fcmpo cr0, f0, f2
bge @8005E4C0
stfs f2, 0x6c(r6)
@8005E4C0
li r3, 0x0
@8005E4C4
addi r6, r6, 0x4
addi r4, r4, 0x2
bdnz @8005E3FC
addi r7, r7, 0xb4
addi r5, r5, 0x1
cmpwi r5, 0x4
blt @8005E3EC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005E670
li r0, 0x5
stw r0, 0x38(r31)
b @8005E670
@8005E4F8
lfs f1, 0x3b4(r31)
lfs f0, lbl_8047BF6C@sda21(r0)
fadds f1, f1, f2
fcmpo cr0, f1, f0
stfs f1, 0x3b4(r31)
cror eq, gt, eq
bne @8005E670
li r0, 0x6
stw r0, 0x38(r31)
b @8005E670
@8005E520
mr r3, r31
li r0, 0x2
mtctr r0
@8005E52C
addi r4, r3, 0x358
lfs f0, 0x0(r4)
lfs f1, 0x4(r4)
fcmpu cr0, f0, f1
beq @8005E5C4
fsubs f1, f1, f0
lfs f2, lbl_8047BF70@sda21(r0)
lfs f0, 0x3c(r31)
fmuls f1, f2, f1
fmuls f3, f1, f0
fcmpo cr0, f3, f2
ble @8005E560
fmr f3, f2
@8005E560
lfs f0, lbl_8047BF74@sda21(r0)
fcmpo cr0, f3, f0
cror eq, lt, eq
bne @8005E574
fmr f3, f0
@8005E574
lfs f1, 0x0(r4)
lfs f0, lbl_8047BF60@sda21(r0)
fadds f1, f1, f3
fcmpo cr0, f3, f0
stfs f1, 0x0(r4)
lfs f2, 0x4(r4)
lfs f0, 0x0(r4)
fsubs f1, f2, f0
ble @8005E59C
b @8005E5A0
@8005E59C
fneg f3, f3
@8005E5A0
lfs f0, lbl_8047BF60@sda21(r0)
fcmpo cr0, f1, f0
ble @8005E5B0
b @8005E5B4
@8005E5B0
fneg f1, f1
@8005E5B4
fcmpo cr0, f1, f3
cror eq, lt, eq
bne @8005E5C4
stfs f2, 0x0(r4)
@8005E5C4
addi r3, r3, 0x8
bdnz @8005E52C
lis r3, lbl_803A9A60@ha
lfs f0, lbl_8047BF60@sda21(r0)
addi r3, r3, lbl_803A9A60@l
lfs f2, 0x358(r3)
lfs f1, 0x35c(r3)
fsubs f1, f2, f1
fcmpo cr0, f1, f0
ble @8005E5F0
b @8005E5F4
@8005E5F0
fneg f1, f1
@8005E5F4
lfs f0, lbl_8047BF78@sda21(r0)
fcmpo cr0, f1, f0
cror eq, lt, eq
bne @8005E670
lis r3, lbl_803A9A60@ha
lfs f0, lbl_8047BF60@sda21(r0)
addi r3, r3, lbl_803A9A60@l
li r0, 0x7
stw r0, 0x38(r31)
stfs f0, 0x3b8(r3)
b @8005E670
@8005E620
lfs f1, 0x3b8(r31)
lfs f0, lbl_8047BF7C@sda21(r0)
fadds f1, f1, f2
fcmpo cr0, f1, f0
stfs f1, 0x3b8(r31)
cror eq, gt, eq
bne @8005E670
li r0, 0x8
stw r0, 0x38(r31)
b @8005E670
@8005E648
li r0, 0x9
stw r0, 0x38(r31)
b @8005E670
@8005E654
li r0, 0x64
li r4, 0x1c6
stw r0, 0x38(r31)
lwz r3, 0x4(r30)
bl fn_801080CC
li r0, 0x1
stb r0, 0x2(r30)
@8005E670
psq_l f31, 0x28(r1), 0, 0
lwz r0, 0x34(r1)
lfd f31, 0x20(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
mtlr r0
addi r1, r1, 0x30
blr
