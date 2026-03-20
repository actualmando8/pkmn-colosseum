stwu r1, -0x90(r1)
mflr r0
stw r0, 0x94(r1)
stfd f31, 0x80(r1)
psq_st f31, 0x88(r1), 0, 0
stfd f30, 0x70(r1)
psq_st f30, 0x78(r1), 0, 0
stfd f29, 0x60(r1)
psq_st f29, 0x68(r1), 0, 0
stfd f28, 0x50(r1)
psq_st f28, 0x58(r1), 0, 0
stfd f27, 0x40(r1)
psq_st f27, 0x48(r1), 0, 0
stw r31, 0x3c(r1)
stw r30, 0x38(r1)
stw r29, 0x34(r1)
stw r28, 0x30(r1)
bl fn_801D036C
mr r31, r3
b @80079204
@80078DAC
cmplwi r0, 0xb
bgt @80079204
lis r3, jumptable_802EE4D8@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EE4D8@l
lwzx r0, r3, r0
mtctr r0
bctr
bl fn_80113F48
lis r4, 0x1095
li r5, 0x0
addi r4, r4, 0x1800
li r6, 0x0
bl fn_80176E0C
lis r3, 0x104f
li r4, 0x6
addi r3, r3, 0x1000
li r5, 0x0
li r6, 0x1
bl fn_801CB834
bl fn_80075C44
clrlwi r0, r3, 24
cmplwi r0, 0x1
bne @80078E18
li r0, 0x2
stw r0, lbl_8047A620@sda21(r0)
b @80079204
@80078E18
li r0, 0x3
stw r0, lbl_8047A620@sda21(r0)
b @80079204
lfs f1, lbl_8047C100@sda21(r0)
li r3, 0x3
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
lis r3, 0x104f
li r4, 0x0
addi r3, r3, 0x1000
li r5, 0x0
li r6, 0x1
bl fn_801CB834
lfs f1, lbl_8047C100@sda21(r0)
li r3, 0x2
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
li r3, 0x1
bl fn_801C40F0
li r3, 0x43c3
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
bl fn_80075C20
clrlwi r0, r3, 24
cmplwi r0, 0x1
bne @80078EA0
li r0, 0x5
stw r0, lbl_8047A620@sda21(r0)
b @80079204
@80078EA0
li r0, 0x4
stw r0, lbl_8047A620@sda21(r0)
b @80079204
lfs f1, lbl_8047C100@sda21(r0)
li r3, 0x3
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
lis r3, 0x104f
li r4, 0x1
addi r3, r3, 0x1000
li r5, 0x0
li r6, 0x1
bl fn_801CB834
lfs f1, lbl_8047C100@sda21(r0)
li r3, 0x2
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
li r3, 0x1
bl fn_801C40F0
li r3, 0x43c0
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x3f9
bl fn_80166A28
lfs f27, lbl_8047C0E0@sda21(r0)
lfd f28, lbl_8047C0F0@sda21(r0)
lis r30, 0x4330
lfd f30, lbl_8047C0F8@sda21(r0)
lfs f31, lbl_8047C0E4@sda21(r0)
b @80078F5C
@80078F24
bl fn_800F0308
bl fn_800D37CC
xoris r0, r3, 0x8000
stw r30, 0x18(r1)
stw r0, 0x1c(r1)
lfd f0, 0x18(r1)
fsubs f29, f0, f28
bl fn_800D3088
stw r3, 0x24(r1)
stw r30, 0x20(r1)
lfd f0, 0x20(r1)
fsubs f0, f0, f30
fdivs f0, f0, f29
fadds f27, f27, f0
@80078F5C
fcmpo cr0, f27, f31
blt @80078F24
li r0, 0x0
stw r0, lbl_8047A620@sda21(r0)
b @80079204
bl fn_801EE398
clrlwi r0, r3, 24
cmplwi r0, 0x1
bne @80078F8C
li r0, 0x7
stw r0, lbl_8047A620@sda21(r0)
b @80079204
@80078F8C
li r0, 0x6
stw r0, lbl_8047A620@sda21(r0)
b @80079204
li r3, 0x43af
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
bl fn_80075BC4
cmplwi r3, 0x1
bge @80078FE0
li r3, 0x43b2
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
li r0, 0x0
stw r0, lbl_8047A620@sda21(r0)
b @80079204
@80078FE0
mr r4, r3
li r3, 0x2f
bl fn_80132A38
li r3, 0x43bb
li r4, 0x1
li r5, 0x0
bl fn_801067E8
bl fn_8001E184
mr r30, r3
li r3, 0x1
bl fn_801069FC
extsb r0, r30
cmpwi r0, 0x0
beq @8007904C
bge @80079028
cmpwi r0, -0x1
bge @80079030
b @8007904C
@80079028
cmpwi r0, 0x2
bge @8007904C
@80079030
li r3, 0x43c1
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r0, 0x0
stw r0, lbl_8047A620@sda21(r0)
b @80079204
@8007904C
li r3, 0x43c4
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
li r0, 0x8
stw r0, lbl_8047A620@sda21(r0)
b @80079204
li r3, 0x43c6
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
li r0, 0x0
stw r0, lbl_8047A620@sda21(r0)
b @80079204
mr r3, r31
bl fn_800788BC
b @80079204
lis r3, lbl_80268AA8@ha
addi r6, r3, lbl_80268AA8@l
lwz r5, 0x0(r6)
lwz r4, 0x4(r6)
lwz r3, 0x8(r6)
lwz r0, 0xc(r6)
stw r5, 0x8(r1)
stw r4, 0xc(r1)
stw r3, 0x10(r1)
stw r0, 0x14(r1)
bl fn_80113F48
lfs f1, lbl_8047C104@sda21(r0)
mr r28, r3
li r3, 0x3
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
lis r4, 0x104f
mr r3, r28
addi r4, r4, 0x1000
bl fn_800F9318
mr r30, r3
cmplwi r30, 0x0
beq @80079108
li r4, 0x0
bl fn_800E4014
@80079108
lis r3, 0xffe
addi r3, r3, 0x1000
bl fn_801CBA0C
mr r29, r3
mr r3, r28
mr r4, r29
bl fn_800F9318
lis r4, 0xfff
li r3, 0x5d5
addi r4, r4, 0x1800
li r5, 0x0
li r6, 0x1
bl fn_80176E0C
li r3, 0x4
bl fn_80177A44
lfs f1, lbl_8047C104@sda21(r0)
li r3, 0x2
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
lis r4, lbl_803F6F18@ha
lis r3, lbl_803F6E40@ha
addi r0, r4, lbl_803F6F18@l
addi r5, r1, 0x8
stw r0, 0xc(r1)
addi r6, r3, lbl_803F6E40@l
li r3, 0x0
li r4, 0x20
bl fn_800849B4
cmpwi r3, 0x0
bge @80079190
li r0, 0x0
stw r0, lbl_8047A620@sda21(r0)
b @80079204
@80079190
lfs f1, lbl_8047C104@sda21(r0)
li r3, 0x3
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
mr r3, r29
bl fn_801CB9D8
cmplwi r30, 0x0
beq @800791D8
mr r3, r30
li r4, 0x1
bl fn_800E4014
lis r4, 0x1095
mr r3, r28
addi r4, r4, 0x1800
li r5, 0x0
li r6, 0x0
bl fn_80176E0C
@800791D8
lfs f1, lbl_8047C104@sda21(r0)
li r3, 0x2
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
li r0, 0xa
stw r0, lbl_8047A620@sda21(r0)
b @80079204
bl fn_80078390
b @80079204
bl fn_80077ED4
@80079204
lwz r0, lbl_8047A620@sda21(r0)
cmpwi r0, 0x0
bgt @80078DAC
mr r3, r31
bl fn_801D0314
li r3, 0x321
li r4, 0x0
bl fn_80113828
psq_l f31, 0x88(r1), 0, 0
lfd f31, 0x80(r1)
psq_l f30, 0x78(r1), 0, 0
lfd f30, 0x70(r1)
psq_l f29, 0x68(r1), 0, 0
lfd f29, 0x60(r1)
psq_l f28, 0x58(r1), 0, 0
lfd f28, 0x50(r1)
psq_l f27, 0x48(r1), 0, 0
lfd f27, 0x40(r1)
lwz r31, 0x3c(r1)
lwz r30, 0x38(r1)
lwz r29, 0x34(r1)
lwz r0, 0x94(r1)
lwz r28, 0x30(r1)
mtlr r0
addi r1, r1, 0x90
blr
