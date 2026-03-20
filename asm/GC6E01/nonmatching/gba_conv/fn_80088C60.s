stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
stw r28, 0x10(r1)
bl fn_8006A76C
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80088C94
li r3, 0x0
b @80088D64
@80088C94
lis r3, 0x2
subi r3, r3, 0x2030
bl fn_80071104
mr r28, r3
bl fn_80128E24
lis r5, 0x2
mr r4, r3
mr r3, r28
subi r5, r5, 0x2030
bl memcpy
li r30, 0x0
mr r29, r30
@80088CC4
clrlwi r31, r30, 16
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r0, r29, 0x59aa
li r4, 0xe
sthx r31, r3, r0
li r3, 0x0
bl fn_80129280
addi r0, r29, 0x26
addi r29, r29, 0x1660
sthx r31, r3, r0
addi r30, r30, 0x1
cmplwi r30, 0x4
blt @80088CC4
li r31, 0x0
li r3, 0x0
li r4, 0xe
bl fn_80129280
stb r31, 0x1c(r3)
li r3, 0x8ae
li r4, 0x0
bl fn_8019075C
li r3, 0x6
li r4, 0x2
li r5, 0x0
bl fn_801D0748
mr r31, r3
bl fn_80128E24
lis r5, 0x2
mr r4, r28
subi r5, r5, 0x2030
bl memcpy
mr r3, r28
bl fn_8007109C
cmpwi r31, 0x4
bne @80088D60
li r3, 0x0
b @80088D64
@80088D60
li r3, -0x1
@80088D64
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
lwz r28, 0x10(r1)
mtlr r0
addi r1, r1, 0x20
blr
