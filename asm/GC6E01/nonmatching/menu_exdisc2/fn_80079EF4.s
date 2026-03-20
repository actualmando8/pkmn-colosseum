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
lwz r5, lbl_804788F0@sda21(r0)
li r6, 0x0
lis r3, lbl_802E61D8@ha
stb r6, lbl_8047A630@sda21(r0)
subi r29, r5, 0x1
stb r6, lbl_8047A631@sda21(r0)
slwi r5, r29, 2
addi r0, r3, lbl_802E61D8@l
stb r6, lbl_8047A632@sda21(r0)
add r3, r0, r5
addi r0, r29, 0x1
mtctr r0
cmpwi r29, 0x0
blt @80079F88
@80079F70
lwz r0, 0x0(r3)
cmplw r0, r4
ble @80079F88
subi r3, r3, 0x4
subi r29, r29, 0x1
bdnz @80079F70
@80079F88
cmpwi r29, 0x0
bge @80079F94
li r29, 0x0
@80079F94
li r3, 0xe1
bl fn_80102510
lfs f27, lbl_8047C114@sda21(r0)
lfd f31, lbl_8047C118@sda21(r0)
lis r30, 0x4330
lfd f29, lbl_8047C120@sda21(r0)
lfs f28, lbl_8047C128@sda21(r0)
b @80079FEC
@80079FB4
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
@80079FEC
fcmpo cr0, f27, f28
blt @80079FB4
li r3, 0xef
li r4, 0x0
bl fn_8010264C
lfs f27, lbl_8047C114@sda21(r0)
lfd f31, lbl_8047C118@sda21(r0)
lis r30, 0x4330
lfd f29, lbl_8047C120@sda21(r0)
lfs f28, lbl_8047C108@sda21(r0)
b @8007A050
@8007A018
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
@8007A050
fcmpo cr0, f27, f28
blt @8007A018
cmpwi r29, 0x1
bge @8007A10C
li r3, 0x43a7
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
cmpwi r31, 0x0
bne @8007A09C
li r3, 0x2
li r4, 0x44cf
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
@8007A09C
li r3, 0xef
bl fn_80102510
lfs f27, lbl_8047C114@sda21(r0)
lfd f31, lbl_8047C118@sda21(r0)
lis r30, 0x4330
lfd f29, lbl_8047C120@sda21(r0)
lfs f28, lbl_8047C108@sda21(r0)
b @8007A0F4
@8007A0BC
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
@8007A0F4
fcmpo cr0, f27, f28
blt @8007A0BC
li r0, 0x1
li r3, 0x0
stw r0, lbl_8047A638@sda21(r0)
b @8007A5A4
@8007A10C
cmpwi r29, 0x2
beq @8007A204
bge @8007A124
cmpwi r29, 0x1
bge @8007A130
b @8007A450
@8007A124
cmpwi r29, 0x4
bge @8007A450
b @8007A30C
@8007A130
lbz r0, lbl_8047A635@sda21(r0)
cmplwi r0, 0x0
beq @8007A1E8
li r3, 0x43ae
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
cmpwi r31, 0x0
bne @8007A178
li r3, 0x2
li r4, 0x44cf
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
@8007A178
li r3, 0xef
bl fn_80102510
lfs f27, lbl_8047C114@sda21(r0)
lfd f31, lbl_8047C118@sda21(r0)
lis r30, 0x4330
lfd f29, lbl_8047C120@sda21(r0)
lfs f28, lbl_8047C108@sda21(r0)
b @8007A1D0
@8007A198
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
@8007A1D0
fcmpo cr0, f27, f28
blt @8007A198
li r0, 0x1
li r3, 0x0
stw r0, lbl_8047A638@sda21(r0)
b @8007A5A4
@8007A1E8
li r3, 0x43b4
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r0, 0x1
stb r0, lbl_8047A632@sda21(r0)
b @8007A4C8
@8007A204
lbz r3, lbl_8047A635@sda21(r0)
cmplwi r3, 0x0
beq @8007A2C8
lbz r0, lbl_8047A634@sda21(r0)
cmplwi r0, 0x0
beq @8007A2C8
li r3, 0x43ab
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
cmpwi r31, 0x0
bne @8007A258
li r3, 0x2
li r4, 0x44cf
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
@8007A258
li r3, 0xef
bl fn_80102510
lfs f27, lbl_8047C114@sda21(r0)
lfd f31, lbl_8047C118@sda21(r0)
lis r30, 0x4330
lfd f29, lbl_8047C120@sda21(r0)
lfs f28, lbl_8047C108@sda21(r0)
b @8007A2B0
@8007A278
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
@8007A2B0
fcmpo cr0, f27, f28
blt @8007A278
li r0, 0x1
li r3, 0x0
stw r0, lbl_8047A638@sda21(r0)
b @8007A5A4
@8007A2C8
cmplwi r3, 0x0
beq @8007A2EC
li r3, 0x43b3
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r0, 0x1
stb r0, lbl_8047A631@sda21(r0)
b @8007A4C8
@8007A2EC
li r3, 0x43b6
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r0, 0x1
stb r0, lbl_8047A632@sda21(r0)
stb r0, lbl_8047A631@sda21(r0)
b @8007A4C8
@8007A30C
lbz r3, lbl_8047A635@sda21(r0)
cmplwi r3, 0x0
beq @8007A3DC
lbz r0, lbl_8047A634@sda21(r0)
cmplwi r0, 0x0
beq @8007A3DC
lbz r0, lbl_8047A633@sda21(r0)
cmplwi r0, 0x0
beq @8007A3DC
li r3, 0x43a9
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
cmpwi r31, 0x0
bne @8007A36C
li r3, 0x2
li r4, 0x44cf
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
@8007A36C
li r3, 0xef
bl fn_80102510
lfs f27, lbl_8047C114@sda21(r0)
lfd f31, lbl_8047C118@sda21(r0)
lis r30, 0x4330
lfd f29, lbl_8047C120@sda21(r0)
lfs f28, lbl_8047C108@sda21(r0)
b @8007A3C4
@8007A38C
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
@8007A3C4
fcmpo cr0, f27, f28
blt @8007A38C
li r0, 0x1
li r3, 0x0
stw r0, lbl_8047A638@sda21(r0)
b @8007A5A4
@8007A3DC
lbz r0, lbl_8047A634@sda21(r0)
cmplwi r0, 0x0
beq @8007A404
li r3, 0x43b1
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r0, 0x1
stb r0, lbl_8047A630@sda21(r0)
b @8007A4C8
@8007A404
cmplwi r3, 0x0
beq @8007A42C
li r3, 0x43b5
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r0, 0x1
stb r0, lbl_8047A630@sda21(r0)
stb r0, lbl_8047A631@sda21(r0)
b @8007A4C8
@8007A42C
li r3, 0x43c2
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r0, 0x1
stb r0, lbl_8047A630@sda21(r0)
stb r0, lbl_8047A631@sda21(r0)
stb r0, lbl_8047A632@sda21(r0)
b @8007A4C8
@8007A450
li r3, 0xef
bl fn_80102510
li r3, 0x1
bl fn_801069FC
lfs f27, lbl_8047C114@sda21(r0)
lfd f31, lbl_8047C118@sda21(r0)
lis r30, 0x4330
lfd f29, lbl_8047C120@sda21(r0)
lfs f28, lbl_8047C108@sda21(r0)
b @8007A4B0
@8007A478
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
@8007A4B0
fcmpo cr0, f27, f28
blt @8007A478
li r0, 0x1
li r3, 0x0
stw r0, lbl_8047A638@sda21(r0)
b @8007A5A4
@8007A4C8
li r3, 0x43d1
li r4, 0x1
li r5, 0x0
bl fn_801067E8
bl fn_8001E184
mr r30, r3
li r3, 0x1
bl fn_801069FC
extsb r0, r30
cmpwi r0, 0x0
beq @8007A5A0
bge @8007A504
cmpwi r0, -0x1
bge @8007A50C
b @8007A5A0
@8007A504
cmpwi r0, 0x2
bge @8007A5A0
@8007A50C
cmpwi r31, 0x0
bne @8007A530
li r3, 0x2
li r4, 0x44cf
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
@8007A530
li r3, 0xef
bl fn_80102510
lfs f27, lbl_8047C114@sda21(r0)
lfd f28, lbl_8047C118@sda21(r0)
lis r31, 0x4330
lfd f30, lbl_8047C120@sda21(r0)
lfs f31, lbl_8047C108@sda21(r0)
b @8007A588
@8007A550
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
@8007A588
fcmpo cr0, f27, f31
blt @8007A550
li r0, 0x1
li r3, 0x0
stw r0, lbl_8047A638@sda21(r0)
b @8007A5A4
@8007A5A0
li r3, 0x1
@8007A5A4
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
