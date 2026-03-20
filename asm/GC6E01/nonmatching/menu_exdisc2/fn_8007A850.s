stwu r1, -0x70(r1)
mflr r0
stw r0, 0x74(r1)
stfd f31, 0x60(r1)
psq_st f31, 0x68(r1), 0, 0
stfd f30, 0x50(r1)
psq_st f30, 0x58(r1), 0, 0
stfd f29, 0x40(r1)
psq_st f29, 0x48(r1), 0, 0
stfd f28, 0x30(r1)
psq_st f28, 0x38(r1), 0, 0
stfd f27, 0x20(r1)
psq_st f27, 0x28(r1), 0, 0
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
bl fn_801D036C
mr r0, r3
li r3, 0x1
mr r30, r0
bl fn_801C40F0
b @8007AA0C
@8007A8A4
cmpwi r0, 0x3
beq @8007A9FC
bge @8007A8C0
cmpwi r0, 0x1
beq @8007A8CC
bge @8007A980
b @8007AA0C
@8007A8C0
cmpwi r0, 0x5
bge @8007AA0C
b @8007AA08
@8007A8CC
li r3, 0x43cf
li r4, 0x0
li r5, 0x1
bl fn_801067E8
li r3, 0xe1
li r4, 0x1
bl fn_8010264C
mr r31, r3
li r3, 0x1
bl fn_801069FC
cmpwi r31, 0x1
beq @8007A92C
bge @8007A910
cmpwi r31, -0x1
beq @8007A958
bge @8007A920
b @8007A96C
@8007A910
cmpwi r31, 0x3
beq @8007A944
bge @8007A96C
b @8007A938
@8007A920
li r0, 0x3
stw r0, lbl_8047A638@sda21(r0)
b @8007AA0C
@8007A92C
li r0, 0x4
stw r0, lbl_8047A638@sda21(r0)
b @8007AA0C
@8007A938
li r0, 0x2
stw r0, lbl_8047A638@sda21(r0)
b @8007AA0C
@8007A944
li r3, 0xe1
bl fn_80102510
li r0, 0x0
stw r0, lbl_8047A638@sda21(r0)
b @8007AA0C
@8007A958
li r3, 0xe1
bl fn_80102510
li r0, 0x0
stw r0, lbl_8047A638@sda21(r0)
b @8007AA0C
@8007A96C
li r3, 0xe1
bl fn_80102510
li r0, 0x0
stw r0, lbl_8047A638@sda21(r0)
b @8007AA0C
@8007A980
li r3, 0x43a5
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
lfs f27, lbl_8047C114@sda21(r0)
lfd f28, lbl_8047C118@sda21(r0)
lis r31, 0x4330
lfd f30, lbl_8047C120@sda21(r0)
lfs f31, lbl_8047C108@sda21(r0)
b @8007A9E8
@8007A9B0
bl fn_800F0308
bl fn_800D37CC
xoris r0, r3, 0x8000
stw r31, 0x8(r1)
stw r0, 0xc(r1)
lfd f0, 0x8(r1)
fsubs f29, f0, f28
bl fn_800D3088
stw r3, 0x14(r1)
stw r31, 0x10(r1)
lfd f0, 0x10(r1)
fsubs f0, f0, f30
fdivs f0, f0, f29
fadds f27, f27, f0
@8007A9E8
fcmpo cr0, f27, f31
blt @8007A9B0
li r0, 0x1
stw r0, lbl_8047A638@sda21(r0)
b @8007AA0C
@8007A9FC
mr r3, r30
bl fn_800798E8
b @8007AA0C
@8007AA08
bl fn_800792D8
@8007AA0C
lwz r0, lbl_8047A638@sda21(r0)
cmpwi r0, 0x0
bgt @8007A8A4
mr r3, r30
bl fn_801D0314
li r3, 0x321
li r4, 0x0
bl fn_80113828
psq_l f31, 0x68(r1), 0, 0
lfd f31, 0x60(r1)
psq_l f30, 0x58(r1), 0, 0
lfd f30, 0x50(r1)
psq_l f29, 0x48(r1), 0, 0
lfd f29, 0x40(r1)
psq_l f28, 0x38(r1), 0, 0
lfd f28, 0x30(r1)
psq_l f27, 0x28(r1), 0, 0
lfd f27, 0x20(r1)
lwz r31, 0x1c(r1)
lwz r0, 0x74(r1)
lwz r30, 0x18(r1)
mtlr r0
addi r1, r1, 0x70
blr
