stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
lis r4, lbl_80267840@ha
li r3, 0x1e
addi r31, r4, lbl_80267840@l
li r4, 0x0
li r5, 0xff
bl fn_80165A20
li r3, 0x8ae
li r4, 0x0
bl fn_8019075C
lwz r0, lbl_8047A5A0@sda21(r0)
cmplwi r0, 0x0
beq @8005CDDC
addi r3, r31, 0x98
addi r5, r31, 0x21c
li r4, 0x20f
bl fn_80196E10
@8005CDDC
lis r3, 0x1
li r4, 0x20
addi r3, r3, 0xf60
bl fn_800E2C04
mr r30, r3
clrlwi r0, r30, 16
cmplwi r0, 0x0
bne @8005CE0C
addi r3, r31, 0x98
li r4, 0x212
li r5, lbl_8047BF28@sda21
bl fn_80196E10
@8005CE0C
mr r3, r30
bl fn_800E27B0
cmplwi r3, 0x0
stw r3, lbl_8047A5A0@sda21(r0)
bne @8005CE30
addi r3, r31, 0x98
addi r5, r31, 0x22c
li r4, 0x213
bl fn_80196E10
@8005CE30
bl fn_80113F48
lis r4, 0xffe
mr r30, r3
addi r3, r4, 0x1000
bl fn_801CBA0C
lwz r4, lbl_8047A5A0@sda21(r0)
stw r3, 0x4314(r4)
mr r3, r30
lwz r4, lbl_8047A5A0@sda21(r0)
lwz r4, 0x4314(r4)
bl fn_800F9318
lis r4, 0xfff
li r3, 0x531
addi r4, r4, 0x1800
li r5, 0x0
li r6, 0x1
bl fn_80176E0C
li r3, 0x4
bl fn_80177A44
bl fn_800FF548
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005CEA0
bl fn_8025CDB8
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8006B5D0
@8005CEA0
bl fn_8006B8E8
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005CEC4
li r31, 0x0
li r3, 0x0
li r4, 0xe
bl fn_80129280
stb r31, 0x1c(r3)
@8005CEC4
li r3, 0xd3
li r4, 0x0
bl fn_8010264C
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
