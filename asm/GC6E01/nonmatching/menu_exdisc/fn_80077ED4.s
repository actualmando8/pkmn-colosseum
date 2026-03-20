stwu r1, -0x80(r1)
mflr r0
stw r0, 0x84(r1)
stfd f31, 0x70(r1)
psq_st f31, 0x78(r1), 0, 0
stfd f30, 0x60(r1)
psq_st f30, 0x68(r1), 0, 0
stfd f29, 0x50(r1)
psq_st f29, 0x58(r1), 0, 0
stfd f28, 0x40(r1)
psq_st f28, 0x48(r1), 0, 0
stfd f27, 0x30(r1)
psq_st f27, 0x38(r1), 0, 0
stmw r27, 0x1c(r1)
lis r3, lbl_803F6F18@ha
addi r3, r3, lbl_803F6F18@l
bl fn_8012AC54
mr r30, r3
bl fn_801D036C
mr r0, r3
li r3, 0x0
mr r29, r0
li r4, 0x0
bl fn_80129280
li r0, 0x3bfa
subi r5, r29, 0x4
subi r4, r3, 0x4
mtctr r0
@80077F44
lwz r3, 0x4(r4)
lwzu r0, 0x8(r4)
stw r3, 0x4(r5)
stwu r0, 0x8(r5)
bdnz @80077F44
bl fn_80075B74
li r3, 0x4
li r4, 0x2
li r5, 0x0
bl fn_801D0748
cmpwi r3, 0x4
beq @80078048
li r3, 0x4c7
bl fn_80166A28
lis r3, 0x104f
li r4, 0x4
addi r3, r3, 0x1000
li r5, 0x0
li r6, 0x0
bl fn_801CB834
lis r3, 0x104f
li r4, 0x1
addi r3, r3, 0x1000
bl fn_801CB708
lfs f27, lbl_8047C0E0@sda21(r0)
lfd f31, lbl_8047C0F0@sda21(r0)
lis r28, 0x4330
lfd f29, lbl_8047C0F8@sda21(r0)
lfs f28, lbl_8047C0E4@sda21(r0)
b @80077FF4
@80077FBC
bl fn_800F0308
bl fn_800D37CC
xoris r0, r3, 0x8000
stw r28, 0x8(r1)
stw r0, 0xc(r1)
lfd f0, 0x8(r1)
fsubs f30, f0, f31
bl fn_800D3088
stw r3, 0x14(r1)
stw r28, 0x10(r1)
lfd f0, 0x10(r1)
fsubs f0, f0, f29
fdivs f0, f0, f30
fadds f27, f27, f0
@80077FF4
fcmpo cr0, f27, f28
blt @80077FBC
mr r4, r30
li r3, 0x4d
bl fn_80132A38
li r3, 0x1
bl fn_80103CC0
li r3, 0x44b0
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_80103CC0
li r3, 0x2
li r4, 0x44cf
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
b @80078344
@80078048
li r3, 0x1
bl fn_80103CC0
li r3, 0x2
li r4, 0x3d83
li r5, 0x0
li r6, 0x1
bl fn_80106D3C
bl fn_800F0308
li r27, 0x0
lis r3, lbl_803F6F18@ha
addi r28, r3, lbl_803F6F18@l
b @8007808C
@80078078
mr r3, r28
mr r4, r27
bl fn_8012AC08
bl fn_80124A60
addi r27, r27, 0x1
@8007808C
clrlwi r0, r27, 16
cmplwi r0, 0x6
blt @80078078
bl fn_80115BD8
bl fn_801159F0
lis r5, lbl_803F6F18@ha
mr r4, r3
addi r3, r5, lbl_803F6F18@l
bl fn_80130660
li r27, 0x0
lis r3, lbl_803F6F18@ha
addi r28, r3, lbl_803F6F18@l
b @800780F8
@800780C0
mr r3, r28
mr r4, r27
bl fn_8012AC08
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800780F4
lis r3, lbl_803F6F18@ha
mr r4, r27
addi r3, r3, lbl_803F6F18@l
bl fn_8012AC08
mr r31, r3
b @80078104
@800780F4
addi r27, r27, 0x1
@800780F8
clrlwi r0, r27, 16
cmplwi r0, 0x6
blt @800780C0
@80078104
cmplwi r31, 0x0
bne @80078120
lis r3, lbl_80268AB8@ha
li r4, 0x42e
addi r3, r3, lbl_80268AB8@l
li r5, lbl_8047C0E8@sda21
bl fn_80196E10
@80078120
lis r4, lbl_803F6E40@ha
li r3, 0x1
addi r4, r4, lbl_803F6E40@l
lwz r0, 0x8(r4)
ori r0, r0, 0x8
stw r0, 0x8(r4)
bl fn_80093574
lis r3, lbl_803F6E40@ha
mr r5, r31
addi r4, r3, lbl_803F6E40@l
li r3, 0x1
bl fn_80092C90
li r3, 0x1
bl fn_80093574
li r3, 0x1
bl fn_80093610
cmpwi r3, 0xc
beq @800782B8
li r3, 0x1
bl fn_80093698
li r3, 0x1
bl fn_801069FC
li r3, 0x1
bl fn_80103CC0
li r3, 0x2
li r4, 0x3d85
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
li r3, 0x0
li r4, 0x0
bl fn_80129280
li r0, 0x3bfa
subi r5, r3, 0x4
subi r4, r29, 0x4
mtctr r0
@800781B8
lwz r3, 0x4(r4)
lwzu r0, 0x8(r4)
stw r3, 0x4(r5)
stwu r0, 0x8(r5)
bdnz @800781B8
li r3, 0x4
li r4, 0x2
li r5, 0x0
bl fn_801D0748
li r3, 0x4c7
bl fn_80166A28
lis r3, 0x104f
li r4, 0x4
addi r3, r3, 0x1000
li r5, 0x0
li r6, 0x0
bl fn_801CB834
lis r3, 0x104f
li r4, 0x1
addi r3, r3, 0x1000
bl fn_801CB708
lfs f27, lbl_8047C0E0@sda21(r0)
lfd f28, lbl_8047C0F0@sda21(r0)
lis r31, 0x4330
lfd f30, lbl_8047C0F8@sda21(r0)
lfs f31, lbl_8047C0E4@sda21(r0)
b @8007825C
@80078224
bl fn_800F0308
bl fn_800D37CC
xoris r0, r3, 0x8000
stw r31, 0x10(r1)
stw r0, 0x14(r1)
lfd f0, 0x10(r1)
fsubs f29, f0, f28
bl fn_800D3088
stw r3, 0xc(r1)
stw r31, 0x8(r1)
lfd f0, 0x8(r1)
fsubs f0, f0, f30
fdivs f0, f0, f29
fadds f27, f27, f0
@8007825C
fcmpo cr0, f27, f31
blt @80078224
mr r4, r30
li r3, 0x4d
bl fn_80132A38
li r3, 0x1
bl fn_80103CC0
li r3, 0x44b0
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
li r3, 0x1
bl fn_80103CC0
li r3, 0x2
li r4, 0x44cf
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
b @80078344
@800782B8
li r3, 0x1
bl fn_80093698
li r3, 0x1
bl fn_80103CC0
li r3, 0x2
li r4, 0x3d84
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
li r3, 0x3d2
li r4, 0x0
li r5, 0xff
bl fn_80165668
mr r4, r30
li r3, 0x4d
bl fn_80132A38
li r3, 0x1
bl fn_80103CC0
li r3, 0x4435
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
li r3, 0x1
bl fn_80103CC0
li r3, 0x2
li r4, 0x3d55
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
@80078344
mr r3, r29
bl fn_801D0314
li r0, 0x0
stw r0, lbl_8047A620@sda21(r0)
psq_l f31, 0x78(r1), 0, 0
lfd f31, 0x70(r1)
psq_l f30, 0x68(r1), 0, 0
lfd f30, 0x60(r1)
psq_l f29, 0x58(r1), 0, 0
lfd f29, 0x50(r1)
psq_l f28, 0x48(r1), 0, 0
lfd f28, 0x40(r1)
psq_l f27, 0x38(r1), 0, 0
lfd f27, 0x30(r1)
lmw r27, 0x1c(r1)
lwz r0, 0x84(r1)
mtlr r0
addi r1, r1, 0x80
blr
