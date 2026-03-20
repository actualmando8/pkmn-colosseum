stwu r1, -0x220(r1)
mflr r0
stw r0, 0x224(r1)
stfd f31, 0x210(r1)
psq_st f31, 0x218(r1), 0, 0
stfd f30, 0x200(r1)
psq_st f30, 0x208(r1), 0, 0
stfd f29, 0x1f0(r1)
psq_st f29, 0x1f8(r1), 0, 0
stmw r22, 0x1c8(r1)
lis r4, lbl_80267840@ha
li r3, 0x0
addi r31, r4, lbl_80267840@l
li r4, 0xe
bl fn_80129280
li r27, 0x0
li r28, 0x0
bl fn_8007162C
li r29, 0x0
mr r22, r3
bl fn_800FF540
cmplwi r3, 0x0
beq @80059C48
addi r3, r31, 0x98
addi r5, r31, 0xb0
li r4, 0x267
bl fn_80196E10
@80059C48
li r3, 0x8ae
bl fn_801906A0
cmplwi r3, 0x0
beq @8005CBD8
addi r3, r31, 0x98
addi r5, r31, 0xd0
li r4, 0x268
bl fn_80196E10
b @8005CBD8
@80059C6C
bl fn_8007162C
mr r30, r3
bl fn_8007162C
mr r26, r3
mr r9, r30
li r3, 0xbe
li r4, 0x0
li r5, 0x0
li r6, 0x10
li r7, 0x0
li r8, 0x1
crclr 6
bl fn_801026A4
bl fn_8007162C
subi r0, r3, 0xa8
cmplwi r0, 0x5d
bgt @8005CAC0
lis r3, jumptable_802E62B0@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802E62B0@l
lwzx r0, r3, r0
mtctr r0
bctr
bl fn_80071344
cmpwi r3, 0x0
bge @80059CDC
li r26, -0x1
b @8005CAC0
@80059CDC
cmpwi r3, 0x1
slwi r0, r3, 2
addi r3, r31, 0x0
lwzx r26, r3, r0
beq @80059CF4
b @8005CAC0
@80059CF4
li r29, 0x1
b @8005CAC0
bl fn_80071344
cmpwi r3, 0x1
beq @80059D7C
bge @80059D18
cmpwi r3, 0x0
bge @80059D20
b @80059DC0
@80059D18
cmpwi r3, 0x3
b @80059DC0
@80059D20
li r26, 0xac
li r24, 0x0
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x4(r3)
li r24, 0x0
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x10(r3)
li r24, 0x0
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x0(r3)
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r3, r3, 0x59a8
li r4, 0x0
bl fn_8006A7E0
b @8005CAC0
@80059D7C
li r26, 0xb3
li r24, 0x2
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x4(r3)
li r24, 0x4
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x10(r3)
li r24, 0x2
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x0(r3)
b @8005CAC0
@80059DC0
li r26, -0x1
b @8005CAC0
li r3, 0x0
bl fn_8006B4AC
bl fn_80071344
cmpwi r3, 0x0
bge @80059DE4
li r26, -0x1
b @8005CAC0
@80059DE4
slwi r0, r3, 2
addi r3, r31, 0x18
lwzx r22, r3, r0
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006AFC4
cmplwi r3, 0x0
bne @80059E38
cmpwi r22, 0xae
beq @80059E18
cmpwi r22, 0xaf
bne @80059E38
@80059E18
li r3, 0x2
li r4, 0x3bfe
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
b @8005CAC0
@80059E38
cmpwi r22, 0xae
beq @80059E5C
bge @80059E50
cmpwi r22, 0xad
bge @80059EBC
b @80059F18
@80059E50
cmpwi r22, 0xb0
bge @80059F18
b @80059E78
@80059E5C
li r24, 0x0
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x0(r3)
mr r26, r22
b @8005CAC0
@80059E78
li r24, 0x1
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x0(r3)
li r24, 0x6
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0xc(r3)
li r24, 0x2
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x8(r3)
mr r26, r22
b @8005CAC0
@80059EBC
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A7BC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80059EF0
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006AFC4
cmplwi r3, 0x0
bne @80059F10
@80059EF0
li r3, 0x2
li r4, 0x4415
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
b @8005CAC0
@80059F10
mr r26, r22
b @8005CAC0
@80059F18
mr r26, r22
b @8005CAC0
li r24, 0x0
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A7BC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80059F5C
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006AFC4
cmplwi r3, 0x0
beq @80059F5C
li r24, 0x1
@80059F5C
cmpwi r24, 0x0
bne @80059F74
addi r3, r31, 0x98
addi r5, r31, 0x10c
li r4, 0x30f
bl fn_80196E10
@80059F74
li r3, 0x0
li r4, 0xe
bl fn_80129280
addis r26, r3, 0x1
li r3, 0x0
subi r26, r26, 0x3674
li r4, 0xe
bl fn_80129280
bl fn_8006AFC4
mr r25, r3
li r3, 0xc8
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80059FD4
li r3, 0xc8
bl fn_80102510
b @80059FC0
@80059FBC
bl fn_800F0308
@80059FC0
li r3, 0xc8
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80059FBC
@80059FD4
mr r3, r25
bl fn_8006A7E8
mr r24, r3
mr r3, r25
bl fn_8006A7C8
li r0, 0x0
mr r9, r3
stw r0, 0x8(r1)
mr r10, r24
li r3, 0xc8
li r4, 0x0
stw r26, 0xc(r1)
li r5, 0x0
li r6, 0x10
li r7, 0x0
li r8, 0x4
crclr 6
bl fn_801026A4
li r3, 0xd6
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005A054
li r3, 0xd6
bl fn_80102510
b @8005A040
@8005A03C
bl fn_800F0308
@8005A040
li r3, 0xd6
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005A03C
@8005A054
li r0, 0x1
li r4, 0x3d89
stw r0, 0x24(r1)
li r0, 0x0
addi r5, r1, 0x24
li r3, 0xd6
stw r4, 0x8(r1)
li r4, 0x0
li r6, 0x10
li r7, 0x1
stw r0, 0xc(r1)
li r8, 0x4
li r9, 0x3db0
li r10, 0x3db1
crclr 6
bl fn_801026A4
mr r24, r3
li r3, 0xd6
bl fn_80102510
cmpwi r24, 0x0
beq @8005A0C4
li r3, 0xc8
li r4, 0x0
li r5, 0x0
bl fn_80102568
bl fn_8006E0CC
li r26, -0x1
b @8005CAC0
@8005A0C4
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006AFC4
mr r24, r3
li r3, 0x1
bl fn_8006AFE4
mr r4, r24
bl fn_8006A7F0
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A76C
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005A118
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A79C
b @8005A19C
@8005A118
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A7BC
mr r24, r3
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A79C
li r3, 0x2
li r4, 0x44d9
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
bl fn_80088C60
cmpwi r3, 0x0
bge @8005A19C
clrlwi r0, r24, 24
cmplwi r0, 0x0
beq @8005A180
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A7AC
@8005A180
li r3, 0xc8
li r4, 0x0
li r5, 0x0
bl fn_80102568
bl fn_8006E0CC
li r26, -0x1
b @8005CAC0
@8005A19C
li r3, 0x0
li r4, 0xe
bl fn_80129280
addis r3, r3, 0x1
lwz r0, -0x3674(r3)
cmpwi r0, 0x1
beq @8005A2C8
li r3, 0x0
li r4, 0xe
bl fn_80129280
addis r25, r3, 0x1
li r3, 0x0
lwz r24, -0x3674(r25)
li r4, 0xe
bl fn_80129280
stw r24, 0x0(r3)
li r3, 0x0
li r4, 0xe
lwz r24, -0x3670(r25)
bl fn_80129280
stw r24, 0x4(r3)
li r3, 0x0
li r4, 0xe
lwz r24, -0x366c(r25)
bl fn_80129280
stw r24, 0x8(r3)
li r3, 0x0
li r4, 0xe
lwz r24, -0x3668(r25)
bl fn_80129280
stw r24, 0xc(r3)
li r3, 0x0
li r4, 0xe
lwz r24, -0x3664(r25)
bl fn_80129280
stw r24, 0x10(r3)
li r3, 0x0
li r4, 0xe
lwz r24, -0x3660(r25)
bl fn_80129280
stw r24, 0x14(r3)
li r3, 0x0
li r4, 0xe
lwz r24, -0x365c(r25)
bl fn_80129280
stw r24, 0x18(r3)
lwz r0, -0x3660(r25)
cmplwi r0, 0x0
bne @8005A268
li r3, 0x0
bl fn_8006ADB4
@8005A268
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x0(r3)
cmpwi r0, 0x0
beq @8005A290
addi r3, r31, 0x98
addi r5, r31, 0x158
li r4, 0xab
bl fn_80196E10
@8005A290
lwz r0, -0x3670(r25)
li r3, 0x8ae
cmpwi r0, 0x0
bne @8005A2A8
li r4, 0x1
b @8005A2AC
@8005A2A8
li r4, 0x2
@8005A2AC
bl fn_8019075C
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_80069C0C
li r26, 0xd1
b @8005A4C0
@8005A2C8
li r3, 0x0
li r4, 0xe
bl fn_80129280
addis r25, r3, 0x1
li r3, 0x0
lwz r24, -0x3674(r25)
li r4, 0xe
bl fn_80129280
stw r24, 0x0(r3)
li r3, 0x0
li r4, 0xe
lwz r24, -0x3670(r25)
bl fn_80129280
stw r24, 0x4(r3)
li r3, 0x0
li r4, 0xe
lwz r24, -0x366c(r25)
bl fn_80129280
stw r24, 0x8(r3)
li r3, 0x0
li r4, 0xe
lwz r24, -0x3668(r25)
bl fn_80129280
stw r24, 0xc(r3)
li r3, 0x0
li r4, 0xe
lwz r24, -0x3664(r25)
bl fn_80129280
stw r24, 0x10(r3)
li r3, 0x0
li r4, 0xe
lwz r24, -0x3660(r25)
bl fn_80129280
stw r24, 0x14(r3)
li r3, 0x0
li r4, 0xe
lwz r24, -0x365c(r25)
bl fn_80129280
stw r24, 0x18(r3)
lwz r0, -0x3660(r25)
cmplwi r0, 0x0
bne @8005A378
li r3, 0x0
bl fn_8006ADB4
@8005A378
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0xc(r3)
cmpwi r0, 0x6
beq @8005A3A0
addi r3, r31, 0x98
addi r5, r31, 0x184
li r4, 0x81
bl fn_80196E10
@8005A3A0
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x0(r3)
cmpwi r0, 0x1
beq @8005A3C8
addi r3, r31, 0x98
addi r5, r31, 0x1b0
li r4, 0x82
bl fn_80196E10
@8005A3C8
lwz r0, -0x3670(r25)
li r3, 0x8ae
cmpwi r0, 0x0
bne @8005A3E0
li r4, 0x1
b @8005A3E4
@8005A3E0
li r4, 0x2
@8005A3E4
bl fn_8019075C
li r3, 0x0
li r4, 0xe
bl fn_80129280
mr r4, r3
li r3, 0xb59
lwz r4, 0x14(r4)
bl fn_8019075C
li r3, 0xafc
li r4, 0x0
bl fn_8019075C
li r3, 0xb11
li r4, 0x0
bl fn_8019075C
li r3, 0xde1
li r4, 0x0
bl fn_8019075C
bl fn_80130054
lwz r4, -0x3634(r25)
li r3, 0xafc
bl fn_8019075C
lwz r4, -0x362c(r25)
li r3, 0xb11
bl fn_8019075C
lwz r4, -0x3628(r25)
li r3, 0xde1
bl fn_8019075C
lwz r29, -0x3654(r25)
li r3, 0x0
li r4, 0xe
bl fn_80129280
addis r4, r3, 0x1
li r3, 0x0
lfs f31, -0x364c(r4)
li r4, 0xe
bl fn_80129280
addis r4, r3, 0x1
li r3, 0x0
lfs f30, -0x3648(r4)
li r4, 0xe
bl fn_80129280
addis r4, r3, 0x1
li r3, 0x0
lfs f29, -0x3644(r4)
li r4, 0xe
bl fn_80129280
addis r3, r3, 0x1
lfs f0, lbl_8047BF18@sda21(r0)
lfs f1, -0x363c(r3)
li r27, 0x1
li r26, 0x105
fmuls f0, f0, f1
fctiwz f0, f0
stfd f0, 0x1c0(r1)
lwz r28, 0x1c4(r1)
@8005A4C0
li r3, 0xc8
li r4, 0x0
li r5, 0x0
bl fn_80102568
bl fn_8006E0CC
b @8005CAC0
li r24, 0x2
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x8(r3)
li r24, 0x0
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x0(r3)
bl fn_80071344
mr r24, r3
cmpwi r24, 0x6
beq @8005A548
bge @8005A550
cmpwi r24, 0x0
bge @8005A520
b @8005A550
@8005A520
li r3, 0x0
li r4, 0xe
bl fn_80129280
slwi r0, r24, 2
addi r4, r31, 0x30
stw r24, 0xc(r3)
lwzx r3, r4, r0
bl fn_8006B4AC
li r26, 0xaf
b @8005CAC0
@8005A548
li r26, 0xc0
b @8005CAC0
@8005A550
li r26, -0x1
b @8005CAC0
bl fn_80071344
mr r24, r3
cmpwi r24, 0x0
bge @8005A570
li r26, -0x1
b @8005CAC0
@8005A570
cmpwi r24, 0x2
bne @8005A580
li r26, -0x1
b @8005CAC0
@8005A580
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x4(r3)
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x0(r3)
cmpwi r0, 0x3
bne @8005A690
subfic r0, r24, 0x1
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r22, r0, 24
bl fn_8007162C
clrlwi r3, r3, 16
bl fn_80104704
cmplwi r3, 0x0
beq @8005A5D4
lwz r25, 0x20(r3)
b @8005A5D8
@8005A5D4
li r25, 0x0
@8005A5D8
addi r3, r25, 0xc
li r4, 0x1ce
bl fn_80108518
mr r9, r22
li r3, 0xbc
li r4, 0x0
li r5, 0x0
li r6, 0x10
li r7, 0x1
li r8, 0x1
crclr 6
bl fn_801026A4
mr r24, r3
cmpwi r24, 0x4
bge @8005A670
cmpwi r24, 0x0
bge @8005A620
b @8005A670
@8005A620
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x10(r3)
li r24, 0x0
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x8(r3)
li r24, 0x0
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0xc(r3)
li r26, 0xcc
li r3, 0xbc
li r4, 0x0
li r5, 0x0
bl fn_80102568
b @8005CAC0
@8005A670
li r3, 0xbc
li r4, 0x0
li r5, 0x1
bl fn_80102568
addi r3, r25, 0xc
li r4, 0x1ca
bl fn_80108518
b @8005CAC0
@8005A690
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x10(r3)
cmpwi r0, 0x4
bne @8005A6B0
li r26, 0xbf
b @8005CAC0
@8005A6B0
li r26, 0xb1
b @8005CAC0
addi r3, r31, 0x98
li r4, 0x3c4
li r5, lbl_8047BF1C@sda21
bl fn_80196E10
b @8005CAC0
bl fn_80071344
cmpwi r3, 0x1
beq @8005A770
bge @8005A778
cmpwi r3, 0x0
bge @8005A6E8
b @8005A778
@8005A6E8
bl fn_801D04E8
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005A718
li r3, 0x2
li r4, 0x44eb
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
b @8005CAC0
@8005A718
li r3, 0x0
li r4, 0x4
bl fn_80135168
cmplwi r3, 0x0
bne @8005A74C
li r3, 0x2
li r4, 0x444d
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
b @8005CAC0
@8005A74C
li r3, 0x0
li r4, 0x2
bl fn_80129280
lwz r5, lbl_8047A5A0@sda21(r0)
mr r4, r3
addi r3, r5, 0x1660
bl fn_8012AC64
li r26, 0xee
b @8005CAC0
@8005A770
li r26, 0xed
b @8005CAC0
@8005A778
li r26, -0x1
b @8005CAC0
lwz r4, lbl_8047A5A0@sda21(r0)
li r3, 0x2
addi r22, r4, 0x1660
bl fn_8006B4AC
mr r3, r22
bl fn_800776E4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005A8C0
li r3, 0xbe
li r4, 0x0
li r5, 0x0
li r6, 0x10
li r7, 0x0
li r8, 0x1
li r9, 0xf5
crclr 6
bl fn_801026A4
bl fn_8006B420
stw r3, 0x8(r1)
li r0, 0x0
mr r9, r22
li r3, 0xda
stw r0, 0xc(r1)
li r4, 0x0
li r5, 0x0
li r6, 0x10
li r7, 0x0
li r8, 0x4
li r10, 0x0
crclr 6
bl fn_801026A4
li r3, 0xda
li r4, 0x0
li r5, -0x28
bl fn_80102868
bl fn_8006B420
mr r4, r3
mr r3, r22
bl fn_80076054
mr r24, r3
clrlwi r0, r24, 16
cmplwi r0, 0x0
bne @8005A840
addi r3, r31, 0x98
addi r5, r31, 0x1e0
li r4, 0x1bb
bl fn_80196E10
@8005A840
li r3, 0x26
bl fn_80166A28
clrlwi r4, r24, 16
li r3, 0x7
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x7
li r4, 0x440a
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0xda
li r4, 0x0
li r5, 0x0
bl fn_80102568
li r3, 0xbe
li r4, 0x0
li r5, 0x1
bl fn_80102568
li r3, 0xda
li r4, 0x0
li r5, 0x1
bl fn_80102568
bl fn_8006E0CC
li r3, 0x0
bl fn_8006B4AC
li r3, 0x1
bl fn_801069FC
bl fn_800714C8
li r26, -0x1
b @8005CAC0
@8005A8C0
li r3, 0x0
bl fn_8006B4AC
mr r3, r22
li r4, 0x0
bl fn_8012A774
lwz r3, lbl_8047A5A0@sda21(r0)
li r4, 0x0
bl fn_8006AC28
lwz r3, lbl_8047A5A0@sda21(r0)
mr r4, r22
bl fn_8006A824
bl fn_800714C8
li r26, 0xb2
b @8005CAC0
li r24, 0x0
mr r22, r24
@8005A900
lwz r0, lbl_8047A5A0@sda21(r0)
addi r3, r22, 0x1660
li r4, 0x0
li r5, 0xb18
add r3, r0, r3
bl memset
lwz r0, lbl_8047A5A0@sda21(r0)
addi r3, r22, 0x1660
add r3, r0, r3
bl fn_8012A248
addi r22, r22, 0xb18
addi r24, r24, 0x1
cmplwi r24, 0x4
blt @8005A900
lwz r4, lbl_8047A5A0@sda21(r0)
li r5, 0x0
stw r5, 0x4c(r1)
li r3, 0x2
addi r0, r4, 0x1660
stw r0, 0x50(r1)
stw r5, 0x54(r1)
stw r5, 0x58(r1)
bl fn_8006B4AC
addi r5, r1, 0x4c
li r3, 0x0
li r4, 0x2
li r6, 0x0
bl fn_800849B4
mr r24, r3
li r3, 0x0
bl fn_8006B4AC
cmpwi r24, 0x0
bge @8005A990
bl fn_800714C8
li r26, -0x1
b @8005CAC0
@8005A990
lwz r3, lbl_8047A5A0@sda21(r0)
li r4, 0x0
bl fn_8006AC28
lwz r3, lbl_8047A5A0@sda21(r0)
addi r4, r3, 0x1660
bl fn_8006A824
bl fn_800714C8
li r26, 0xb2
b @8005CAC0
lwz r25, lbl_8047A5A0@sda21(r0)
li r3, 0xda
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005A9F0
li r3, 0xda
bl fn_80102510
b @8005A9DC
@8005A9D8
bl fn_800F0308
@8005A9DC
li r3, 0xda
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005A9D8
@8005A9F0
mr r3, r25
bl fn_8006A7E8
mr r24, r3
mr r3, r25
bl fn_8006A7C8
li r0, 0x0
mr r9, r3
stw r0, 0x8(r1)
mr r10, r24
li r3, 0xda
li r4, 0x0
stw r0, 0xc(r1)
li r5, 0x0
li r6, 0x10
li r7, 0x0
li r8, 0x4
crclr 6
bl fn_801026A4
li r3, 0xd6
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005AA70
li r3, 0xd6
bl fn_80102510
b @8005AA5C
@8005AA58
bl fn_800F0308
@8005AA5C
li r3, 0xd6
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005AA58
@8005AA70
li r6, 0x0
li r0, -0x2a
stw r6, 0x20(r1)
addi r5, r1, 0x20
li r3, 0xd6
li r4, 0x0
stw r6, 0x8(r1)
li r6, 0x10
li r7, 0x1
li r8, 0x4
stw r0, 0xc(r1)
li r9, 0x3d47
li r10, 0x3d49
crclr 6
bl fn_801026A4
mr r24, r3
li r3, 0xd6
bl fn_80102510
cmpwi r24, 0x0
bne @8005AEA8
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006AFC4
cmplwi r3, 0x0
beq @8005AD64
li r3, 0xda
li r4, 0x0
li r5, 0x0
bl fn_80102568
bl fn_8006E0CC
li r3, 0xbe
li r4, 0x0
li r5, 0x1
bl fn_80102568
li r3, 0xbe
li r4, 0x0
li r5, 0x0
li r6, 0x10
li r7, 0x0
li r8, 0x1
li r9, 0xeb
crclr 6
bl fn_801026A4
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006AFC4
mr r25, r3
li r3, 0xda
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005AB6C
li r3, 0xda
bl fn_80102510
b @8005AB58
@8005AB54
bl fn_800F0308
@8005AB58
li r3, 0xda
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005AB54
@8005AB6C
mr r3, r25
bl fn_8006A7E8
mr r24, r3
mr r3, r25
bl fn_8006A7C8
li r0, 0x0
mr r9, r3
stw r0, 0x8(r1)
mr r10, r24
li r3, 0xda
li r4, 0x0
stw r0, 0xc(r1)
li r5, 0x0
li r6, 0x10
li r7, 0x0
li r8, 0x4
crclr 6
bl fn_801026A4
li r3, 0xd6
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005ABEC
li r3, 0xd6
bl fn_80102510
b @8005ABD8
@8005ABD4
bl fn_800F0308
@8005ABD8
li r3, 0xd6
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005ABD4
@8005ABEC
li r0, 0x1
li r4, 0x3c54
stw r0, 0x1c(r1)
li r0, -0x28
addi r5, r1, 0x1c
li r3, 0xd6
stw r4, 0x8(r1)
li r4, 0x0
li r6, 0x10
li r7, 0x1
stw r0, 0xc(r1)
li r8, 0x4
li r9, 0x3d47
li r10, 0x3d49
crclr 6
bl fn_801026A4
mr r24, r3
li r3, 0xd6
bl fn_80102510
cmpwi r24, 0x0
bne @8005AEA8
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A7BC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005AC98
li r3, 0x2
li r4, 0x44c2
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x0
li r4, 0x3c
li r5, 0x9e
li r6, 0x1
bl fn_8001E074
extsb r22, r3
li r3, 0x1
bl fn_801069FC
cmpwi r22, 0x0
bne @8005AEA8
@8005AC98
li r3, 0xda
li r4, 0x0
li r5, 0x0
bl fn_80102568
bl fn_8006E0CC
li r3, 0xbe
li r4, 0x0
li r5, 0x1
bl fn_80102568
mr r9, r30
li r3, 0xbe
li r4, 0x0
li r5, 0x0
li r6, 0x10
li r7, 0x0
li r8, 0x1
crclr 6
bl fn_801026A4
lwz r25, lbl_8047A5A0@sda21(r0)
li r3, 0xda
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005AD1C
li r3, 0xda
bl fn_80102510
b @8005AD08
@8005AD04
bl fn_800F0308
@8005AD08
li r3, 0xda
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005AD04
@8005AD1C
mr r3, r25
bl fn_8006A7E8
mr r24, r3
mr r3, r25
bl fn_8006A7C8
li r0, 0x0
mr r9, r3
stw r0, 0x8(r1)
mr r10, r24
li r3, 0xda
li r4, 0x0
stw r0, 0xc(r1)
li r5, 0x0
li r6, 0x10
li r7, 0x0
li r8, 0x4
crclr 6
bl fn_801026A4
@8005AD64
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r6, lbl_8047A5A0@sda21(r0)
lis r5, 0x1
mr r4, r3
addi r3, r6, 0x4318
subi r5, r5, 0x33d4
bl memcpy
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r4, lbl_8047A5A0@sda21(r0)
bl fn_8006AF44
bl fn_801D04E8
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005ADF0
li r3, 0x26
bl fn_80166A28
li r3, 0x2
li r4, 0x3c60
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
lwz r3, lbl_8047A5A0@sda21(r0)
bl fn_8006A7E8
cmpwi r3, 0x0
beq @8005AE80
li r3, 0x2
li r4, 0x3d55
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005AE80
@8005ADF0
bl fn_800889A4
cmpwi r3, 0x0
bge @8005AE44
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r6, lbl_8047A5A0@sda21(r0)
lis r4, 0x1
subi r5, r4, 0x33d4
addi r4, r6, 0x4318
bl memcpy
lwz r3, lbl_8047A5A0@sda21(r0)
bl fn_8006A7E8
cmpwi r3, 0x0
beq @8005AE80
li r3, 0x2
li r4, 0x3d55
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005AE80
@8005AE44
lwz r3, lbl_8047A5A0@sda21(r0)
bl fn_8006A7E8
cmpwi r3, 0x0
bne @8005AE6C
li r3, 0x2
li r4, 0x3c5e
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005AE80
@8005AE6C
li r3, 0x2
li r4, 0x3d44
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@8005AE80
li r3, 0xda
li r4, 0x0
li r5, 0x0
bl fn_80102568
bl fn_8006E0CC
bl fn_800714C8
li r3, 0x1
bl fn_801069FC
li r26, -0x1
b @8005CAC0
@8005AEA8
lwz r3, lbl_8047A5A0@sda21(r0)
bl fn_8006A7E8
cmpwi r3, 0x0
beq @8005AEE8
li r3, 0x2
li r4, 0x3d55
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0xda
li r4, 0x0
li r5, 0x0
bl fn_80102568
bl fn_8006E0CC
li r3, 0x1
bl fn_801069FC
@8005AEE8
li r26, -0x1
b @8005CAC0
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_80069C0C
li r3, 0x0
bl fn_8006B09C
mr r25, r3
li r3, 0xda
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005AF44
li r3, 0xda
bl fn_80102510
b @8005AF30
@8005AF2C
bl fn_800F0308
@8005AF30
li r3, 0xda
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005AF2C
@8005AF44
mr r3, r25
bl fn_8006A7E8
mr r24, r3
mr r3, r25
bl fn_8006A7C8
li r0, 0x0
mr r9, r3
stw r0, 0x8(r1)
mr r10, r24
li r3, 0xda
li r4, 0x0
stw r0, 0xc(r1)
li r5, 0x0
li r6, 0x10
li r7, 0x0
li r8, 0x4
crclr 6
bl fn_801026A4
li r3, 0xd6
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005AFC4
li r3, 0xd6
bl fn_80102510
b @8005AFB0
@8005AFAC
bl fn_800F0308
@8005AFB0
li r3, 0xd6
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005AFAC
@8005AFC4
li r6, 0x0
li r0, -0x2a
stw r6, 0x18(r1)
addi r5, r1, 0x18
li r3, 0xd6
li r4, 0x0
stw r6, 0x8(r1)
li r6, 0x10
li r7, 0x1
li r8, 0x4
stw r0, 0xc(r1)
li r9, 0x3d47
li r10, 0x3d49
crclr 6
bl fn_801026A4
mr r24, r3
li r3, 0xd6
bl fn_80102510
li r3, 0xda
li r4, 0x0
li r5, 0x0
bl fn_80102568
bl fn_8006E0CC
cmpwi r24, 0x0
beq @8005B030
li r26, -0x1
b @8005CAC0
@8005B030
li r26, 0xd1
b @8005CAC0
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006AFC4
mr r0, r3
li r3, 0x0
mr r26, r0
li r4, 0xe
bl fn_80129280
lwz r0, 0x4(r3)
cmpwi r0, 0x2
bne @8005B078
addi r3, r31, 0x98
addi r5, r31, 0x1f4
li r4, 0x4b9
bl fn_80196E10
@8005B078
cmplwi r26, 0x0
bne @8005B090
addi r3, r31, 0x98
li r4, 0x4ba
li r5, lbl_8047BF20@sda21
bl fn_80196E10
@8005B090
bl fn_8006B420
mr r25, r3
li r3, 0xda
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005B0D0
li r3, 0xda
bl fn_80102510
b @8005B0BC
@8005B0B8
bl fn_800F0308
@8005B0BC
li r3, 0xda
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005B0B8
@8005B0D0
mr r3, r26
bl fn_8006A7E8
mr r24, r3
mr r3, r26
bl fn_8006A7C8
stw r25, 0x8(r1)
li r0, 0x0
mr r9, r3
mr r10, r24
stw r0, 0xc(r1)
li r3, 0xda
li r4, 0x0
li r5, 0x0
li r6, 0x10
li r7, 0x0
li r8, 0x4
crclr 6
bl fn_801026A4
bl fn_8006B420
mr r4, r3
addi r3, r26, 0xb44
bl fn_80076054
clrlwi r4, r3, 16
cmplwi r4, 0x0
beq @8005B16C
li r3, 0x1
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
li r3, 0xda
li r4, 0x0
li r5, 0x0
bl fn_80102568
bl fn_8006E0CC
bl fn_800714C8
li r26, -0x1
b @8005CAC0
@8005B16C
li r3, 0xd6
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005B1A4
li r3, 0xd6
bl fn_80102510
b @8005B190
@8005B18C
bl fn_800F0308
@8005B190
li r3, 0xd6
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005B18C
@8005B1A4
li r6, 0x0
li r0, -0x2a
stw r6, 0x14(r1)
addi r5, r1, 0x14
li r3, 0xd6
li r4, 0x0
stw r6, 0x8(r1)
li r6, 0x10
li r7, 0x1
li r8, 0x4
stw r0, 0xc(r1)
li r9, 0x3d47
li r10, 0x3d49
crclr 6
bl fn_801026A4
mr r24, r3
li r3, 0xd6
bl fn_80102510
cmpwi r24, 0x0
beq @8005B210
li r3, 0xda
li r4, 0x0
li r5, 0x0
bl fn_80102568
bl fn_8006E0CC
li r26, -0x1
b @8005CAC0
@8005B210
lwz r5, lbl_8047A5A0@sda21(r0)
li r3, 0x0
li r4, 0xe
addi r25, r5, 0x4318
bl fn_80129280
li r0, 0x1985
subi r5, r25, 0x4
subi r4, r3, 0x4
mtctr r0
@8005B234
lwz r3, 0x4(r4)
lwzu r0, 0x8(r4)
stw r3, 0x4(r5)
stwu r0, 0x8(r5)
bdnz @8005B234
lwz r0, 0x4(r4)
mr r3, r25
stw r0, 0x4(r5)
bl fn_8006AFC4
mr r0, r3
mr r3, r25
mr r22, r0
bl fn_8006A7BC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005B4CC
cmplwi r22, 0x0
beq @8005B4CC
li r3, 0xda
li r4, 0x0
li r5, 0x0
bl fn_80102568
bl fn_8006E0CC
li r3, 0xbe
li r4, 0x0
li r5, 0x1
bl fn_80102568
li r3, 0xbe
li r4, 0x0
li r5, 0x0
li r6, 0x10
li r7, 0x0
li r8, 0x1
li r9, 0xd7
crclr 6
bl fn_801026A4
li r3, 0xc8
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005B2FC
li r3, 0xc8
bl fn_80102510
b @8005B2E8
@8005B2E4
bl fn_800F0308
@8005B2E8
li r3, 0xc8
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005B2E4
@8005B2FC
mr r3, r22
bl fn_8006A7E8
mr r24, r3
mr r3, r22
bl fn_8006A7C8
li r0, 0x0
mr r9, r3
addis r4, r25, 0x1
stw r0, 0x8(r1)
subi r0, r4, 0x3674
mr r10, r24
stw r0, 0xc(r1)
li r3, 0xc8
li r4, 0x0
li r5, 0x0
li r6, 0x10
li r7, 0x0
li r8, 0x4
crclr 6
bl fn_801026A4
li r3, 0xd6
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005B384
li r3, 0xd6
bl fn_80102510
b @8005B370
@8005B36C
bl fn_800F0308
@8005B370
li r3, 0xd6
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005B36C
@8005B384
li r0, 0x1
li r4, 0x44c8
stw r0, 0x10(r1)
li r0, 0x0
addi r5, r1, 0x10
li r3, 0xd6
stw r4, 0x8(r1)
li r4, 0x0
li r6, 0x10
li r7, 0x1
stw r0, 0xc(r1)
li r8, 0x4
li r9, 0x3d47
li r10, 0x3d49
crclr 6
bl fn_801026A4
mr r24, r3
li r3, 0xd6
bl fn_80102510
li r3, 0xc8
li r4, 0x0
li r5, 0x0
bl fn_80102568
bl fn_8006E0CC
li r3, 0xbe
li r4, 0x0
li r5, 0x1
bl fn_80102568
cmpwi r24, 0x0
beq @8005B404
li r26, -0x1
b @8005CAC0
@8005B404
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A76C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005B4CC
mr r9, r30
li r3, 0xbe
li r4, 0x0
li r5, 0x0
li r6, 0x10
li r7, 0x0
li r8, 0x1
crclr 6
bl fn_801026A4
bl fn_8006B420
mr r25, r3
li r3, 0xda
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005B484
li r3, 0xda
bl fn_80102510
b @8005B470
@8005B46C
bl fn_800F0308
@8005B470
li r3, 0xda
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005B46C
@8005B484
mr r3, r26
bl fn_8006A7E8
mr r24, r3
mr r3, r26
bl fn_8006A7C8
stw r25, 0x8(r1)
li r0, 0x0
mr r9, r3
mr r10, r24
stw r0, 0xc(r1)
li r3, 0xda
li r4, 0x0
li r5, 0x0
li r6, 0x10
li r7, 0x0
li r8, 0x4
crclr 6
bl fn_801026A4
@8005B4CC
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A76C
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005B4FC
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A79C
b @8005B564
@8005B4FC
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A7BC
mr r24, r3
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A79C
bl fn_80088C60
cmpwi r3, 0x0
bge @8005B564
clrlwi r0, r24, 24
cmplwi r0, 0x0
beq @8005B548
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A7AC
@8005B548
li r3, 0xda
li r4, 0x0
li r5, 0x0
bl fn_80102568
bl fn_8006E0CC
li r26, -0x1
b @8005CAC0
@8005B564
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r22, 0x10(r3)
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r23, 0xc(r3)
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r26, 0x8(r3)
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r25, 0x4(r3)
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r24, 0x0(r3)
addi r3, r1, 0x98
li r4, 0x0
li r5, 0x50
bl memset
li r5, 0x0
li r0, 0x5
stw r24, 0x98(r1)
li r3, 0x0
li r4, 0xe
stw r25, 0x9c(r1)
stw r26, 0xa0(r1)
stw r23, 0xa4(r1)
stw r22, 0xa8(r1)
stw r5, 0xac(r1)
stw r0, 0xb0(r1)
bl fn_80129280
lwz r0, 0x0(r3)
cmpwi r0, 0x1
bne @8005B760
lwz r24, 0x98(r1)
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x0(r3)
li r3, 0x0
li r4, 0xe
lwz r24, 0x9c(r1)
bl fn_80129280
stw r24, 0x4(r3)
li r3, 0x0
li r4, 0xe
lwz r24, 0xa0(r1)
bl fn_80129280
stw r24, 0x8(r3)
li r3, 0x0
li r4, 0xe
lwz r24, 0xa4(r1)
bl fn_80129280
stw r24, 0xc(r3)
li r3, 0x0
li r4, 0xe
lwz r24, 0xa8(r1)
bl fn_80129280
stw r24, 0x10(r3)
li r3, 0x0
li r4, 0xe
lwz r24, 0xac(r1)
bl fn_80129280
stw r24, 0x14(r3)
li r3, 0x0
li r4, 0xe
lwz r24, 0xb0(r1)
bl fn_80129280
stw r24, 0x18(r3)
lwz r0, 0xac(r1)
cmplwi r0, 0x0
bne @8005B6A0
li r3, 0x0
bl fn_8006ADB4
@8005B6A0
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0xc(r3)
cmpwi r0, 0x6
beq @8005B6C8
addi r3, r31, 0x98
addi r5, r31, 0x184
li r4, 0x81
bl fn_80196E10
@8005B6C8
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x0(r3)
cmpwi r0, 0x1
beq @8005B6F0
addi r3, r31, 0x98
addi r5, r31, 0x1b0
li r4, 0x82
bl fn_80196E10
@8005B6F0
lwz r0, 0x9c(r1)
li r3, 0x8ae
cmpwi r0, 0x0
bne @8005B708
li r4, 0x1
b @8005B70C
@8005B708
li r4, 0x2
@8005B70C
bl fn_8019075C
li r3, 0x0
li r4, 0xe
bl fn_80129280
mr r4, r3
li r3, 0xb59
lwz r4, 0x14(r4)
bl fn_8019075C
li r3, 0xafc
li r4, 0x0
bl fn_8019075C
li r3, 0xb11
li r4, 0x0
bl fn_8019075C
li r3, 0xde1
li r4, 0x0
bl fn_8019075C
bl fn_80130054
li r29, 0x4c
li r26, 0x105
b @8005B85C
@8005B760
lwz r24, 0x98(r1)
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x0(r3)
li r3, 0x0
li r4, 0xe
lwz r24, 0x9c(r1)
bl fn_80129280
stw r24, 0x4(r3)
li r3, 0x0
li r4, 0xe
lwz r24, 0xa0(r1)
bl fn_80129280
stw r24, 0x8(r3)
li r3, 0x0
li r4, 0xe
lwz r24, 0xa4(r1)
bl fn_80129280
stw r24, 0xc(r3)
li r3, 0x0
li r4, 0xe
lwz r24, 0xa8(r1)
bl fn_80129280
stw r24, 0x10(r3)
li r3, 0x0
li r4, 0xe
lwz r24, 0xac(r1)
bl fn_80129280
stw r24, 0x14(r3)
li r3, 0x0
li r4, 0xe
lwz r24, 0xb0(r1)
bl fn_80129280
stw r24, 0x18(r3)
lwz r0, 0xac(r1)
cmplwi r0, 0x0
bne @8005B800
li r3, 0x0
bl fn_8006ADB4
@8005B800
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x0(r3)
cmpwi r0, 0x0
beq @8005B828
addi r3, r31, 0x98
addi r5, r31, 0x158
li r4, 0xab
bl fn_80196E10
@8005B828
lwz r0, 0x9c(r1)
li r3, 0x8ae
cmpwi r0, 0x0
bne @8005B840
li r4, 0x1
b @8005B844
@8005B840
li r4, 0x2
@8005B844
bl fn_8019075C
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_80069C0C
li r26, 0xd1
@8005B85C
li r3, 0xda
li r4, 0x0
li r5, 0x0
bl fn_80102568
bl fn_8006E0CC
b @8005CAC0
bl fn_80071344
cmpwi r3, 0x0
bge @8005B888
li r26, -0x1
b @8005CAC0
@8005B888
cmpwi r3, 0x4
bge @8005B9F0
slwi r0, r3, 4
addi r22, r31, 0x4c
add r22, r22, r0
addi r23, r22, 0x8
lwz r0, 0x0(r23)
cmpwi r0, 0x0
bne @8005B910
bl fn_801D04E8
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005B8DC
li r3, 0x2
li r4, 0x44ea
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
b @8005CAC0
@8005B8DC
li r3, 0x0
li r4, 0x4
bl fn_80135168
cmplwi r3, 0x0
bne @8005B910
li r3, 0x2
li r4, 0x44db
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
b @8005CAC0
@8005B910
lwz r24, 0x0(r22)
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x4(r3)
li r3, 0x0
lwz r24, 0x4(r22)
li r4, 0xe
bl fn_80129280
stw r24, 0xc(r3)
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r4, 0x0(r23)
addi r3, r3, 0x59a8
bl fn_8006A7E0
li r3, 0x0
li r4, 0xe
bl fn_80129280
lbz r4, 0xc(r22)
addi r3, r3, 0x59a8
extsb r4, r4
bl fn_8006A81C
addi r23, r22, 0x1
li r24, 0x1
li r22, 0x1660
@8005B978
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r0, r22, 0x59a8
li r4, 0x1
add r3, r3, r0
bl fn_8006A7E0
li r3, 0x0
li r4, 0xe
bl fn_80129280
lbz r4, 0xc(r23)
addi r0, r22, 0x59a8
add r3, r3, r0
extsb r4, r4
bl fn_8006A81C
addi r22, r22, 0x1660
addi r23, r23, 0x1
addi r24, r24, 0x1
cmplwi r24, 0x4
blt @8005B978
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x4(r3)
cmpwi r0, 0x2
bne @8005B9E8
li r26, 0xbf
b @8005CAC0
@8005B9E8
li r26, 0xaf
b @8005CAC0
@8005B9F0
li r26, -0x1
b @8005CAC0
li r0, 0x0
cmpwi r22, 0xc1
stw r0, 0x28(r1)
bne @8005BA14
li r0, 0x7
stw r0, 0x28(r1)
b @8005BA28
@8005BA14
bl fn_8006B420
lwz r5, lbl_8047A5A0@sda21(r0)
mr r4, r3
addi r3, r5, 0x42c0
bl fn_80077E80
@8005BA28
bl fn_8007162C
lwz r7, lbl_8047A5A0@sda21(r0)
addi r5, r1, 0x28
li r4, 0x0
li r6, 0x10
addi r9, r7, 0x42c0
li r7, 0x1
li r8, 0x1
crclr 6
bl fn_801026A4
cmpwi r3, 0x0
bge @8005BA60
li r26, -0x1
b @8005CAC0
@8005BA60
cmpwi r3, 0x8
beq @8005BAF0
bge @8005CAC0
cmpwi r3, 0x3
beq @8005BA78
b @8005CAC0
@8005BA78
lwz r4, lbl_8047A5A0@sda21(r0)
lwz r0, 0x42c8(r4)
cmpwi r0, 0x2
bne @8005CAC0
addi r3, r1, 0x5c
addi r4, r4, 0x42d8
li r5, 0x3c
bl memcpy
addi r9, r1, 0x5c
li r3, 0xb4
li r4, 0x0
li r5, 0x0
li r6, 0x10
li r7, 0x1
li r8, 0x1
crclr 6
bl fn_801026A4
mr r24, r3
li r3, 0xb4
bl fn_80102510
cmpwi r24, 0x0
blt @8005BAE4
lwz r3, lbl_8047A5A0@sda21(r0)
addi r4, r1, 0x5c
li r5, 0x3c
addi r3, r3, 0x42d8
bl memcpy
@8005BAE4
li r0, 0x3
stw r0, 0x28(r1)
b @8005BA28
@8005BAF0
bl fn_800714C8
li r26, 0xc1
b @8005CAC0
bl fn_8007162C
lwz r7, lbl_8047A5A0@sda21(r0)
li r4, 0x0
li r5, 0x0
li r6, 0x10
addi r9, r7, 0x42c0
li r7, 0x1
li r8, 0x1
crclr 6
bl fn_801026A4
cmpwi r3, 0x0
bge @8005BB34
li r26, -0x1
b @8005CAC0
@8005BB34
cmpwi r3, 0x6
beq @8005BB64
bge @8005BB4C
cmpwi r3, 0x5
bge @8005BB58
b @8005CAC0
@8005BB4C
cmpwi r3, 0x8
bge @8005CAC0
b @8005BB9C
@8005BB58
bl fn_800714C8
li r26, 0xc0
b @8005CAC0
@8005BB64
bl fn_8007162C
bl fn_801022B8
cmpwi r3, 0x9fc
bne @8005BB9C
li r3, 0x0
bl fn_8006B51C
lwz r5, lbl_8047A5A0@sda21(r0)
mr r4, r3
addi r3, r5, 0x42c0
bl fn_80077E80
lwz r3, lbl_8047A5A0@sda21(r0)
li r0, 0x6
sth r0, 0x42c6(r3)
b @8005CAC0
@8005BB9C
bl fn_8006B420
lwz r5, lbl_8047A5A0@sda21(r0)
mr r4, r3
addi r3, r5, 0x42c0
bl fn_80077EA4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005BC4C
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x8(r3)
li r3, 0x0
li r4, 0xe
mulli r24, r0, 0x54
bl fn_80129280
addis r5, r24, 0x1
lwz r4, lbl_8047A5A0@sda21(r0)
subi r5, r5, 0x3624
add r3, r3, r5
addi r4, r4, 0x42c0
bl fn_80077E80
@8005BBF4
bl fn_801D04E8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005BC44
li r3, 0x2
li r4, 0x44b1
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x0
li r4, 0x3c
li r5, 0x9e
li r6, 0x0
bl fn_8001E074
extsb r0, r3
cmpwi r0, 0x0
bne @8005BC44
bl fn_80088964
cmpwi r3, 0x0
blt @8005BBF4
@8005BC44
li r3, 0x1
bl fn_801069FC
@8005BC4C
li r26, -0x1
b @8005CAC0
addi r3, r31, 0x98
li r4, 0x5f5
li r5, lbl_8047BF1C@sda21
bl fn_80196E10
b @8005CAC0
bl fn_80071344
mr r24, r3
cmpwi r24, 0x6
bge @8005BCB8
cmpwi r24, 0x0
bge @8005BC84
b @8005BCB8
@8005BC84
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r24, 0x8(r3)
bl fn_80105624
lhz r0, 0x4(r3)
rlwinm r0, r0, 0, 21, 21
cmpwi r0, 0x0
beq @8005BCB0
li r26, 0xc0
b @8005CAC0
@8005BCB0
li r26, 0xc2
b @8005CAC0
@8005BCB8
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x4(r3)
cmpwi r0, 0x2
bne @8005BCE0
li r3, 0xb3
bl fn_80071398
mr r26, r3
b @8005CAC0
@8005BCE0
li r3, 0xaf
bl fn_80071398
mr r26, r3
b @8005CAC0
bl fn_80071344
cmpwi r3, 0x1
beq @8005BD50
bge @8005BD50
cmpwi r3, 0x0
bge @8005BD0C
b @8005BD50
@8005BD0C
bl fn_800714C8
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x10(r3)
cmpwi r0, 0x4
beq @8005BD30
li r26, 0xb1
b @8005CAC0
@8005BD30
bl fn_80089028
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005BD48
li r26, 0xe4
b @8005CAC0
@8005BD48
li r26, 0xb6
b @8005CAC0
@8005BD50
li r3, 0xb3
bl fn_80071398
mr r26, r3
b @8005CAC0
li r22, 0x0
mr r23, r22
@8005BD68
lwz r0, lbl_8047A5A0@sda21(r0)
addi r3, r23, 0x1660
li r4, 0x0
li r5, 0xb18
add r3, r0, r3
bl memset
lwz r0, lbl_8047A5A0@sda21(r0)
addi r3, r23, 0x1660
add r3, r0, r3
bl fn_8012A248
addi r23, r23, 0xb18
addi r22, r22, 0x1
cmplwi r22, 0x4
blt @8005BD68
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x4(r3)
cmpwi r0, 0x2
beq @8005BE30
bge @8005BE30
cmpwi r0, 0x0
bge @8005BDC8
b @8005BE30
@8005BDC8
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x59ac(r3)
cmpwi r0, 0x0
bne @8005BE08
lwz r4, lbl_8047A5A0@sda21(r0)
li r0, 0x0
stw r0, 0x44(r1)
li r3, 0x0
addi r5, r4, 0x1660
addi r4, r4, 0x2178
stw r5, 0x3c(r1)
stw r4, 0x40(r1)
stw r0, 0x48(r1)
b @8005BE78
@8005BE08
lwz r5, lbl_8047A5A0@sda21(r0)
li r6, 0x0
stw r6, 0x3c(r1)
li r3, 0x1
addi r4, r5, 0x1660
addi r0, r5, 0x2178
stw r4, 0x40(r1)
stw r0, 0x44(r1)
stw r6, 0x48(r1)
b @8005BE78
@8005BE30
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x59ac(r3)
cmpwi r0, 0x0
bne @8005BE50
li r3, 0x2
b @8005BE54
@8005BE50
li r3, 0x3
@8005BE54
lwz r7, lbl_8047A5A0@sda21(r0)
addi r6, r7, 0x1660
addi r5, r7, 0x2178
addi r4, r7, 0x2c90
addi r0, r7, 0x37a8
stw r6, 0x3c(r1)
stw r5, 0x40(r1)
stw r4, 0x44(r1)
stw r0, 0x48(r1)
@8005BE78
addi r5, r1, 0x3c
li r4, 0x1a
li r6, 0x0
bl fn_800849B4
cmpwi r3, 0x0
bge @8005BEA0
li r3, 0xb3
bl fn_80071398
mr r26, r3
b @8005CAC0
@8005BEA0
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x59ac(r3)
cmpwi r0, 0x0
bne @8005BEC8
lwz r3, lbl_8047A5A0@sda21(r0)
li r4, 0x0
addi r3, r3, 0x1660
bl fn_8012A774
@8005BEC8
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x4(r3)
cmpwi r0, 0x2
beq @8005BFE0
bge @8005BFE0
cmpwi r0, 0x0
bge @8005BEF0
b @8005BFE0
@8005BEF0
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r4, lbl_8047A5A0@sda21(r0)
addi r3, r3, 0x59a8
addi r4, r4, 0x1660
bl fn_8006A824
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r4, lbl_8047A5A0@sda21(r0)
addi r3, r3, 0x7008
addi r4, r4, 0x2178
bl fn_8006A824
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x59ac(r3)
cmpwi r0, 0x0
bne @8005BF74
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r3, r3, 0x59a8
li r4, 0x1
bl fn_8006A81C
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r3, r3, 0x7008
li r4, 0x2
bl fn_8006A81C
b @8005BFA4
@8005BF74
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r3, r3, 0x59a8
li r4, 0x2
bl fn_8006A81C
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r3, r3, 0x7008
li r4, 0x3
bl fn_8006A81C
@8005BFA4
li r3, 0x0
li r4, 0xe
bl fn_80129280
addis r3, r3, 0x1
li r4, 0x0
subi r3, r3, 0x7998
bl fn_8006A81C
li r3, 0x0
li r4, 0xe
bl fn_80129280
addis r3, r3, 0x1
li r4, 0x0
subi r3, r3, 0x6338
bl fn_8006A81C
b @8005C0C0
@8005BFE0
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r4, lbl_8047A5A0@sda21(r0)
addi r3, r3, 0x59a8
addi r4, r4, 0x1660
bl fn_8006A824
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r4, lbl_8047A5A0@sda21(r0)
addi r3, r3, 0x7008
addi r4, r4, 0x2178
bl fn_8006A824
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r4, lbl_8047A5A0@sda21(r0)
addis r3, r3, 0x1
subi r3, r3, 0x7998
addi r4, r4, 0x2c90
bl fn_8006A824
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r4, lbl_8047A5A0@sda21(r0)
addis r3, r3, 0x1
subi r3, r3, 0x6338
addi r4, r4, 0x37a8
bl fn_8006A824
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r3, r3, 0x59a8
li r4, 0x1
bl fn_8006A81C
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r3, r3, 0x7008
li r4, 0x2
bl fn_8006A81C
li r3, 0x0
li r4, 0xe
bl fn_80129280
addis r3, r3, 0x1
li r4, 0x3
subi r3, r3, 0x7998
bl fn_8006A81C
li r3, 0x0
li r4, 0xe
bl fn_80129280
addis r3, r3, 0x1
li r4, 0x4
subi r3, r3, 0x6338
bl fn_8006A81C
@8005C0C0
li r26, 0xb6
b @8005CAC0
li r23, 0x0
li r22, 0x0
mr r24, r22
@8005C0D4
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r0, r22, 0x7005
addi r22, r22, 0x1660
stbx r24, r3, r0
addi r23, r23, 0x1
cmplwi r23, 0x4
blt @8005C0D4
bl fn_80071344
cmpwi r3, 0x0
bge @8005C254
bl fn_80071160
cmpwi r3, 0x1
beq @8005C180
bge @8005C1B4
cmpwi r3, 0x0
bge @8005C120
b @8005C1B4
@8005C120
li r3, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C168
li r3, 0x2
li r4, 0x4445
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005C150
@8005C14C
bl fn_800F0308
@8005C150
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C14C
b @8005C1D4
@8005C168
li r3, 0x2
li r4, 0x3d55
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005C1D4
@8005C180
li r3, 0x2
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005C19C
@8005C198
bl fn_800F0308
@8005C19C
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C198
b @8005C1D4
@8005C1B4
mr r4, r3
li r3, 0x2f
bl fn_80132A38
li r3, 0x2
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@8005C1D4
li r3, 0x1
bl fn_801069FC
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005C244
li r3, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C20C
li r4, 0x4445
b @8005C210
@8005C20C
li r4, 0x3c4f
@8005C210
li r3, 0x2
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005C228
@8005C224
bl fn_800F0308
@8005C228
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C224
li r3, 0x1
bl fn_801069FC
@8005C244
li r3, 0xb3
bl fn_80071398
mr r26, r3
b @8005CAC0
@8005C254
li r3, 0xd0
li r4, 0x1
bl fn_8010264C
mr r24, r3
li r3, 0xd0
bl fn_80102510
cmpwi r24, 0x0
bge @8005C3C4
bl fn_80071160
cmpwi r3, 0x1
beq @8005C2F0
bge @8005C324
cmpwi r3, 0x0
bge @8005C290
b @8005C324
@8005C290
li r3, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C2D8
li r3, 0x2
li r4, 0x4445
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005C2C0
@8005C2BC
bl fn_800F0308
@8005C2C0
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C2BC
b @8005C344
@8005C2D8
li r3, 0x2
li r4, 0x3d55
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005C344
@8005C2F0
li r3, 0x2
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005C30C
@8005C308
bl fn_800F0308
@8005C30C
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C308
b @8005C344
@8005C324
mr r4, r3
li r3, 0x2f
bl fn_80132A38
li r3, 0x2
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@8005C344
li r3, 0x1
bl fn_801069FC
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005C3B4
li r3, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C37C
li r4, 0x4445
b @8005C380
@8005C37C
li r4, 0x3c4f
@8005C380
li r3, 0x2
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005C398
@8005C394
bl fn_800F0308
@8005C398
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C394
li r3, 0x1
bl fn_801069FC
@8005C3B4
li r3, 0xb3
bl fn_80071398
mr r26, r3
b @8005CAC0
@8005C3C4
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x4(r3)
cmpwi r0, 0x2
beq @8005C3F4
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_80069C0C
li r26, 0xd1
b @8005CAC0
@8005C3F4
li r26, 0xb5
b @8005CAC0
bl fn_80071344
cmpwi r3, 0x3
li r26, 0xd1
beq @8005C4A8
bge @8005C4A8
cmpwi r3, -0x1
beq @8005C4A8
bge @8005C420
b @8005C4A8
@8005C420
slwi r0, r3, 2
addi r22, r31, 0x8c
add r22, r22, r0
li r3, 0x0
lbz r0, 0x0(r22)
li r4, 0xe
extsb r24, r0
bl fn_80129280
lbz r0, 0x1(r22)
li r4, 0xe
stw r24, 0x59d0(r3)
li r3, 0x0
extsb r24, r0
bl fn_80129280
lbz r0, 0x2(r22)
li r4, 0xe
stw r24, 0x7030(r3)
li r3, 0x0
extsb r24, r0
bl fn_80129280
addis r3, r3, 0x1
lbz r0, 0x3(r22)
stw r24, -0x7970(r3)
li r3, 0x0
extsb r24, r0
li r4, 0xe
bl fn_80129280
addis r4, r3, 0x1
li r3, 0x0
stw r24, -0x6310(r4)
li r4, 0xe
bl fn_80129280
bl fn_80069C0C
b @8005CAC0
@8005C4A8
bl fn_80071160
cmpwi r3, 0x1
beq @8005C524
bge @8005C558
cmpwi r3, 0x0
bge @8005C4C4
b @8005C558
@8005C4C4
li r3, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C50C
li r3, 0x2
li r4, 0x4445
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005C4F4
@8005C4F0
bl fn_800F0308
@8005C4F4
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C4F0
b @8005C578
@8005C50C
li r3, 0x2
li r4, 0x3d55
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005C578
@8005C524
li r3, 0x2
li r4, 0x44c0
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005C540
@8005C53C
bl fn_800F0308
@8005C540
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C53C
b @8005C578
@8005C558
mr r4, r3
li r3, 0x2f
bl fn_80132A38
li r3, 0x2
li r4, 0x44b8
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
@8005C578
li r3, 0x1
bl fn_801069FC
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005C5E8
li r3, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C5B0
li r4, 0x4445
b @8005C5B4
@8005C5B0
li r4, 0x3c4f
@8005C5B4
li r3, 0x2
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005C5CC
@8005C5C8
bl fn_800F0308
@8005C5CC
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C5C8
li r3, 0x1
bl fn_801069FC
@8005C5E8
li r3, 0xb3
bl fn_80071398
mr r26, r3
b @8005CAC0
bl fn_8006B8FC
li r29, 0x397
b @8005CAC0
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_80062948
bl fn_80071398
mr r26, r3
b @8005CAC0
li r3, 0xb9
li r4, 0x1
bl fn_8010264C
mr r24, r3
li r3, 0xb9
li r4, 0x0
li r5, 0x0
bl fn_80102568
li r3, 0xbe
li r4, 0x0
li r5, 0x1
bl fn_80102568
cmpwi r24, 0x0
beq @8005C670
bge @8005C664
cmpwi r24, -0x1
b @8005C738
@8005C664
cmpwi r24, 0x2
bge @8005C738
b @8005C6E0
@8005C670
bl fn_801D04E8
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005C6A0
li r3, 0x2
li r4, 0x44ea
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
b @8005CAC0
@8005C6A0
li r3, 0x0
li r4, 0x4
bl fn_80135168
cmplwi r3, 0x0
bne @8005C6D4
li r3, 0x2
li r4, 0x44db
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
b @8005CAC0
@8005C6D4
li r3, 0xb
bl fn_8002D91C
b @8005CAC0
@8005C6E0
li r3, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C72C
li r3, 0x2
li r4, 0x4445
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005C710
@8005C70C
bl fn_800F0308
@8005C710
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005C70C
li r3, 0x1
bl fn_801069FC
@8005C72C
li r3, 0xc
bl fn_8002D91C
b @8005CAC0
@8005C738
li r26, -0x1
b @8005CAC0
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A7BC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005CA7C
bl fn_8006ADEC
mr r0, r3
li r3, 0x0
mr r25, r0
li r4, 0xe
bl fn_80129280
bl fn_8006AFC4
mr r26, r3
cmplwi r26, 0x0
bne @8005C794
addi r3, r31, 0x98
li r4, 0x72c
li r5, lbl_8047BF24@sda21
bl fn_80196E10
@8005C794
cmplwi r25, 0x0
beq @8005CA74
li r3, 0x0
bl fn_8006ADB4
mr r3, r26
bl fn_8006A7E8
cmpwi r3, 0x0
bne @8005C868
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A76C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005CA74
mr r22, r25
mr r4, r25
li r3, 0x0
bl fn_801293FC
@8005C7E0
li r3, 0x2
li r4, 0x3c03
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x0
li r4, 0x3c
li r5, 0x9e
li r6, 0x0
bl fn_8001E074
extsb r0, r3
cmpwi r0, 0x0
bne @8005C824
bl fn_80088D84
cmpwi r3, 0x0
bge @8005CA74
b @8005C7E0
@8005C824
li r3, 0x2
li r4, 0x3d54
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x0
li r4, 0x3c
li r5, 0x9e
li r6, 0x1
bl fn_8001E074
extsb r0, r3
cmpwi r0, 0x0
bne @8005C7E0
mr r4, r22
li r3, 0x0
bl fn_80129384
b @8005CA74
@8005C868
li r3, 0x7
li r4, 0x3c23
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x0
li r4, 0x3c
li r5, 0x9e
li r6, 0x0
bl fn_8001E074
extsb r0, r3
cmpwi r0, 0x0
bne @8005C9E4
lwz r3, lbl_8047A5A0@sda21(r0)
li r7, 0x0
stw r7, 0x2c(r1)
addi r5, r1, 0x2c
addi r0, r3, 0x1660
addi r6, r1, 0xe8
stw r0, 0x30(r1)
li r3, 0x0
li r4, 0x40
stw r7, 0x34(r1)
stw r7, 0x38(r1)
bl fn_800849B4
cmpwi r3, 0x0
blt @8005C868
addi r3, r26, 0xb44
bl fn_8012AC3C
lwz r4, lbl_8047A5A0@sda21(r0)
mr r24, r3
addi r3, r4, 0x1660
bl fn_8012AC3C
cmplw r3, r24
bne @8005C9CC
addi r3, r26, 0xb44
bl fn_8012AC54
lwz r4, lbl_8047A5A0@sda21(r0)
mr r24, r3
addi r3, r4, 0x1660
bl fn_8012AC54
mr r4, r24
bl fn_800F9EE4
cmpwi r3, 0x0
bne @8005C9CC
li r3, 0x7
li r4, 0x3d51
li r5, 0x0
li r6, 0x1
bl fn_80106D3C
lwz r5, 0xe8(r1)
lis r3, 0x99
lwz r4, 0xec(r1)
subi r0, r3, 0x6981
add r5, r5, r25
add r3, r4, r25
stw r5, 0xe8(r1)
cmplw r5, r0
stw r3, 0xec(r1)
ble @8005C95C
stw r0, 0xe8(r1)
@8005C95C
lis r3, 0x99
lwz r4, 0xec(r1)
subi r0, r3, 0x6981
cmplw r4, r0
ble @8005C974
stw r0, 0xec(r1)
@8005C974
li r3, 0x1
bl fn_80093574
addi r4, r1, 0xe8
li r3, 0x1
li r5, 0x0
bl fn_80092C90
li r3, 0x1
bl fn_80093574
cmpwi r3, 0xc
bne @8005C9B4
li r3, 0x7
li r4, 0x3d52
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005CA18
@8005C9B4
li r3, 0x7
li r4, 0x3d53
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005C868
@8005C9CC
li r3, 0x7
li r4, 0x44da
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005C868
@8005C9E4
li r3, 0x7
li r4, 0x3d54
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x0
li r4, 0x3c
li r5, 0x9e
li r6, 0x1
bl fn_8001E074
extsb r0, r3
cmpwi r0, 0x0
bne @8005C868
@8005CA18
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006A76C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005CA74
@8005CA34
li r3, 0x2
li r4, 0x44ec
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x0
li r4, 0x3c
li r5, 0x9e
li r6, 0x0
bl fn_8001E074
extsb r0, r3
cmpwi r0, 0x0
bne @8005CA74
bl fn_80088C60
cmpwi r3, 0x0
blt @8005CA34
@8005CA74
li r3, 0x1
bl fn_801069FC
@8005CA7C
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x0(r3)
cmpwi r0, 0x1
beq @8005CAB4
bge @8005CAB4
cmpwi r0, 0x0
bge @8005CAA4
b @8005CAB4
@8005CAA4
li r3, 0xae
bl fn_80071398
mr r26, r3
b @8005CAC0
@8005CAB4
li r3, 0xac
bl fn_80071398
mr r26, r3
@8005CAC0
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005CB20
li r3, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005CB20
li r3, 0x2
li r4, 0x3c4f
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005CB04
@8005CB00
bl fn_800F0308
@8005CB04
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005CB00
li r3, 0x1
bl fn_801069FC
@8005CB20
cmpwi r26, 0x0
mr r22, r30
bge @8005CB50
bl fn_800714C8
mr r25, r3
li r3, 0xbe
li r4, 0x0
li r5, 0x1
bl fn_80102568
cmpwi r25, 0x0
blt @8005CBE4
b @8005CB98
@8005CB50
bl fn_8007162C
cmpw r26, r3
beq @8005CB98
bl fn_8007162C
mr r25, r3
bl fn_801046B8
cmpw r3, r25
bne @8005CB80
bl fn_8007162C
li r4, 0x0
li r5, 0x0
bl fn_80102568
@8005CB80
li r3, 0xbe
li r4, 0x0
li r5, 0x1
bl fn_80102568
mr r3, r26
bl fn_800715BC
@8005CB98
cmplwi r29, 0x0
beq @8005CBD8
bl fn_8007162C
mr r25, r3
bl fn_801046B8
cmpw r3, r25
bne @8005CBE4
bl fn_8007162C
li r4, 0x0
li r5, 0x0
bl fn_80102568
li r3, 0xbe
li r4, 0x0
li r5, 0x1
bl fn_80102568
b @8005CBE4
@8005CBD8
bl fn_8007162C
cmpwi r3, 0x0
bgt @80059C6C
@8005CBE4
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @8005CC10
bl fn_8006B8FC
fmr f1, f31
mr r3, r29
fmr f2, f30
mr r4, r28
fmr f3, f29
bl fn_80113778
b @8005CCA4
@8005CC10
cmplwi r29, 0x0
bne @8005CC1C
li r29, 0x3a1
@8005CC1C
cmplwi r29, 0x3a1
bne @8005CC94
bl fn_8006B8F0
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005CC98
li r3, 0x1
bl fn_8008ABA0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005CC58
li r4, 0x4445
b @8005CC5C
@8005CC58
li r4, 0x3c4f
@8005CC5C
li r3, 0x2
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
b @8005CC74
@8005CC70
bl fn_800F0308
@8005CC74
li r3, 0x1
bl fn_800F7EF8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005CC70
li r3, 0x1
bl fn_801069FC
b @8005CC98
@8005CC94
bl fn_8006B8FC
@8005CC98
mr r3, r29
li r4, 0x0
bl fn_80113828
@8005CCA4
psq_l f31, 0x218(r1), 0, 0
lfd f31, 0x210(r1)
psq_l f30, 0x208(r1), 0, 0
lfd f30, 0x200(r1)
psq_l f29, 0x1f8(r1), 0, 0
lfd f29, 0x1f0(r1)
lmw r22, 0x1c8(r1)
lwz r0, 0x224(r1)
mtlr r0
addi r1, r1, 0x220
blr
