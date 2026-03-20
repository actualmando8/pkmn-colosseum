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
stw r31, 0x2c(r1)
stw r30, 0x28(r1)
stw r29, 0x24(r1)
mr r31, r3
li r3, 0x43a1
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
li r3, 0x43a2
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
lfs f1, lbl_8047C100@sda21(r0)
li r3, 0x3
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
lis r3, 0x104f
li r4, 0x2
addi r3, r3, 0x1000
li r5, 0x0
li r6, 0x0
bl fn_801CB834
li r3, 0x4c7
bl fn_80166A28
lfs f1, lbl_8047C100@sda21(r0)
li r3, 0x2
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
lis r3, 0x104f
li r4, 0x1
addi r3, r3, 0x1000
bl fn_801CB708
lis r3, 0x104f
li r4, 0x3
addi r3, r3, 0x1000
li r5, 0x0
li r6, 0x1
bl fn_801CB834
li r3, 0x43a3
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
li r3, 0x43a4
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
li r3, 0x0
li r4, 0x2
bl fn_80129280
li r30, 0x0
mr r29, r3
b @80078A14
@800789E8
mr r3, r29
clrlwi r5, r30, 24
li r4, 0x3
bl fn_8012A5B0
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x1
beq @80078A10
li r0, 0x1
b @80078A24
@80078A10
addi r30, r30, 0x1
@80078A14
clrlwi r0, r30, 24
cmplwi r0, 0x6
blt @800789E8
li r0, 0x0
@80078A24
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80078AEC
li r3, 0x43a6
li r4, 0x1
li r5, 0x0
bl fn_801067E8
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
lis r30, 0x4330
lfd f29, lbl_8047C0F8@sda21(r0)
lfs f28, lbl_8047C0E4@sda21(r0)
b @80078AC0
@80078A88
bl fn_800F0308
bl fn_800D37CC
xoris r0, r3, 0x8000
stw r30, 0x8(r1)
stw r0, 0xc(r1)
lfd f0, 0x8(r1)
fsubs f30, f0, f31
bl fn_800D3088
stw r3, 0x14(r1)
stw r30, 0x10(r1)
lfd f0, 0x10(r1)
fsubs f0, f0, f29
fdivs f0, f0, f30
fadds f27, f27, f0
@80078AC0
fcmpo cr0, f27, f28
blt @80078A88
li r3, 0x43ac
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
li r0, 0x0
stw r0, lbl_8047A620@sda21(r0)
b @80078CF4
@80078AEC
li r3, 0x4c7
bl fn_80166A28
lis r3, 0x104f
li r4, 0x5
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
lis r30, 0x4330
lfd f29, lbl_8047C0F8@sda21(r0)
lfs f28, lbl_8047C0E4@sda21(r0)
b @80078B6C
@80078B34
bl fn_800F0308
bl fn_800D37CC
xoris r0, r3, 0x8000
stw r30, 0x10(r1)
stw r0, 0x14(r1)
lfd f0, 0x10(r1)
fsubs f30, f0, f31
bl fn_800D3088
stw r3, 0xc(r1)
stw r30, 0x8(r1)
lfd f0, 0x8(r1)
fsubs f0, f0, f29
fdivs f0, f0, f30
fadds f27, f27, f0
@80078B6C
fcmpo cr0, f27, f28
blt @80078B34
li r3, 0x0
li r4, 0x0
bl fn_80129280
li r0, 0x3bfa
subi r5, r31, 0x4
subi r4, r3, 0x4
mtctr r0
@80078B90
lwz r3, 0x4(r4)
lwzu r0, 0x8(r4)
stw r3, 0x4(r5)
stwu r0, 0x8(r5)
bdnz @80078B90
bl fn_80075BFC
bl fn_80115BD8
bl fn_801159F0
mr r4, r3
mr r3, r29
bl fn_80130660
li r3, 0x4
li r4, 0x2
li r5, 0x0
bl fn_801D0748
extsb r0, r3
cmpwi r0, 0x4
beq @80078CAC
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
lis r30, 0x4330
lfd f30, lbl_8047C0F8@sda21(r0)
lfs f31, lbl_8047C0E4@sda21(r0)
b @80078C58
@80078C20
bl fn_800F0308
bl fn_800D37CC
xoris r0, r3, 0x8000
stw r30, 0x10(r1)
stw r0, 0x14(r1)
lfd f0, 0x10(r1)
fsubs f29, f0, f28
bl fn_800D3088
stw r3, 0xc(r1)
stw r30, 0x8(r1)
lfd f0, 0x8(r1)
fsubs f0, f0, f30
fdivs f0, f0, f29
fadds f27, f27, f0
@80078C58
fcmpo cr0, f27, f31
blt @80078C20
li r3, 0x0
li r4, 0x0
bl fn_80129280
li r0, 0x3bfa
subi r5, r3, 0x4
subi r4, r31, 0x4
mtctr r0
@80078C7C
lwz r3, 0x4(r4)
lwzu r0, 0x8(r4)
stw r3, 0x4(r5)
stwu r0, 0x8(r5)
bdnz @80078C7C
li r3, 0x43ac
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r0, 0x0
stw r0, lbl_8047A620@sda21(r0)
b @80078CF4
@80078CAC
li r3, 0x3d2
li r4, 0x0
li r5, 0xff
bl fn_80165668
li r3, 0x43a8
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
li r3, 0x43aa
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
li r0, 0x0
stw r0, lbl_8047A620@sda21(r0)
@80078CF4
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
lwz r31, 0x2c(r1)
lwz r30, 0x28(r1)
lwz r0, 0x84(r1)
lwz r29, 0x24(r1)
mtlr r0
addi r1, r1, 0x80
blr
