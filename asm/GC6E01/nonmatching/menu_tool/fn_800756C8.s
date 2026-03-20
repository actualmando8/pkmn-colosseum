stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
mr r30, r3
bl fn_800FF56C
cmplwi r3, 0x43
bne @800757D8
li r3, 0x1a0
li r4, 0x20
bl fn_800E2C04
clrlwi r0, r3, 16
cmplwi r0, 0x0
beq @8007570C
bl fn_800E27B0
b @80075710
@8007570C
li r3, 0x0
@80075710
stw r3, lbl_8047A610@sda21(r0)
addi r31, r3, 0xc
li r3, 0x0
li r4, 0x1
bl fn_80135938
mr r6, r3
mr r3, r31
mr r4, r30
li r5, 0xa
bl fn_801240C4
lfs f0, lbl_8047C0A8@sda21(r0)
lwz r3, lbl_8047A610@sda21(r0)
stfs f0, 0x4(r3)
lwz r3, lbl_8047A610@sda21(r0)
stfs f0, 0x8(r3)
lwz r3, lbl_8047A610@sda21(r0)
lfs f1, 0x4(r3)
bl fn_800CE148
frsp f2, f1
lfs f1, lbl_8047C09C@sda21(r0)
lfs f0, lbl_8047C098@sda21(r0)
lwz r3, lbl_8047A610@sda21(r0)
fmadds f1, f1, f2, f0
lfs f0, lbl_8047C0A0@sda21(r0)
stfs f1, 0x18c(r3)
lwz r3, lbl_8047A610@sda21(r0)
lfs f1, 0x18c(r3)
fcmpo cr0, f1, f0
ble @80075788
stfs f0, 0x18c(r3)
@80075788
lis r4, lbl_802EF0A8@ha
lwz r3, lbl_8047A610@sda21(r0)
addi r4, r4, lbl_802EF0A8@l
addis r5, r4, 0x1
addi r3, r3, 0x144
lha r4, 0x7296(r5)
lha r5, 0x7298(r5)
bl fn_8010A5BC
lwz r4, lbl_8047A610@sda21(r0)
addi r3, r4, 0x144
addi r4, r4, 0xc
bl fn_80109C88
li r3, 0xd8
li r4, 0x0
li r5, 0x0
li r6, 0x0
li r7, 0x0
li r8, 0x0
crclr 6
bl fn_801026A4
@800757D8
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
