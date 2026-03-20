stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
stw r28, 0x10(r1)
cmpwi r3, 0x0
blt @800936C4
cmpwi r3, 0x3
ble @800936CC
@800936C4
li r3, 0x0
b @800937D4
@800936CC
lis r4, lbl_803FB328@ha
slwi r30, r3, 2
addi r31, r4, lbl_803FB328@l
lwzx r28, r31, r30
cmplwi r28, 0x0
bne @800936EC
li r3, 0x1
b @800937D4
@800936EC
lwz r3, 0x4338(r28)
li r4, 0x1
bl fn_800716E8
@800936F8
mr r3, r28
bl fn_8009F7B4
lwz r29, 0x433c(r28)
mr r3, r28
bl fn_8009F890
addi r3, r28, 0x20
li r4, 0x8
bl fn_800A257C
srwi r0, r29, 16
cmpwi r0, 0x3
bne @8009372C
bl fn_800F0308
b @800936F8
@8009372C
mr r3, r28
bl fn_8009F7B4
li r0, 0xd
lis r3, 0x3
stw r0, 0x4340(r28)
addi r0, r3, 0xd
mr r3, r28
stw r0, 0x433c(r28)
bl fn_8009F890
addi r3, r28, 0x20
li r4, 0x8
bl fn_800A257C
addi r3, r28, 0x18
bl fn_8009FABC
addi r3, r28, 0x20
li r4, 0x0
bl fn_800A1E54
lwz r3, 0x4338(r28)
li r4, 0x0
li r5, 0x0
bl fn_800716C8
lwz r3, 0x4338(r28)
li r4, 0x0
bl fn_800716E8
lwzx r3, r31, r30
bl fn_800E202C
mr r29, r3
clrlwi r0, r29, 16
cmplwi r0, 0x0
bne @800937B8
lis r3, lbl_8026F5A8@ha
li r4, 0x1e6
addi r3, r3, lbl_8026F5A8@l
li r5, lbl_8047C1E8@sda21
bl fn_80196E10
@800937B8
mr r3, r29
bl fn_800E24B0
mr r3, r29
bl fn_800E209C
li r0, 0x0
li r3, 0x1
stwx r0, r31, r30
@800937D4
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
lwz r28, 0x10(r1)
mtlr r0
addi r1, r1, 0x20
blr
