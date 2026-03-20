stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stfd f31, 0x20(r1)
psq_st f31, 0x28(r1), 0, 0
bl fn_800D37CC
xoris r3, r3, 0x8000
lis r0, 0x4330
stw r3, 0xc(r1)
lfd f1, lbl_8047BFF0@sda21(r0)
stw r0, 0x8(r1)
lfd f0, 0x8(r1)
fsubs f31, f0, f1
bl fn_800D3088
lis r0, 0x4330
stw r3, 0x14(r1)
lis r4, lbl_803A9F08@ha
lfd f1, lbl_8047C020@sda21(r0)
stw r0, 0x10(r1)
addi r5, r4, lbl_803A9F08@l
addis r6, r5, 0x1
li r3, 0x0
lfd f0, 0x10(r1)
fsubs f0, f0, f1
fdivs f0, f0, f31
stfs f0, -0x3278(r6)
@8006940C
addis r4, r5, 0x1
subi r4, r4, 0x3274
li r0, 0x6
mtctr r0
@8006941C
lfs f0, 0x0(r4)
lfs f1, 0x18(r4)
fcmpu cr0, f0, f1
beq @800694D4
fsubs f1, f1, f0
lfs f2, lbl_8047C010@sda21(r0)
lfs f0, -0x3278(r6)
fmuls f1, f2, f1
fmuls f3, f1, f0
fcmpo cr0, f3, f2
ble @8006944C
fmr f3, f2
@8006944C
lfs f0, lbl_8047C014@sda21(r0)
fcmpo cr0, f3, f0
cror eq, lt, eq
bne @80069460
fmr f3, f0
@80069460
lfs f1, 0x0(r4)
lfs f0, lbl_8047BFE8@sda21(r0)
fadds f1, f1, f3
fcmpo cr0, f3, f0
stfs f1, 0x0(r4)
lfs f2, 0x18(r4)
lfs f0, 0x0(r4)
fsubs f1, f2, f0
ble @80069488
b @8006948C
@80069488
fneg f3, f3
@8006948C
lfs f0, lbl_8047BFE8@sda21(r0)
fcmpo cr0, f1, f0
ble @800694A0
fmr f0, f1
b @800694A4
@800694A0
fneg f0, f1
@800694A4
fcmpo cr0, f0, f3
cror eq, lt, eq
beq @800694D0
lfs f0, lbl_8047BFE8@sda21(r0)
fcmpo cr0, f1, f0
ble @800694C0
b @800694C4
@800694C0
fneg f1, f1
@800694C4
lfs f0, lbl_8047C018@sda21(r0)
fcmpo cr0, f1, f0
bge @800694D4
@800694D0
stfs f2, 0x0(r4)
@800694D4
addi r4, r4, 0x4
bdnz @8006941C
addi r5, r5, 0x30
addi r3, r3, 0x1
cmpwi r3, 0x4
blt @8006940C
psq_l f31, 0x28(r1), 0, 0
lwz r0, 0x34(r1)
lfd f31, 0x20(r1)
mtlr r0
addi r1, r1, 0x30
blr
