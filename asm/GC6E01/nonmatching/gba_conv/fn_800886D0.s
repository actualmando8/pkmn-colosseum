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
beq @80088704
li r3, 0x0
b @80088944
@80088704
bl fn_8006AE18
cmpwi r3, 0x0
beq @80088718
li r3, 0x0
b @80088944
@80088718
bl fn_801EE398
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80088730
li r3, 0x0
b @80088944
@80088730
li r3, 0x478
bl fn_801902E0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8008874C
li r3, 0x0
b @80088944
@8008874C
li r3, 0xafd
bl fn_801902E0
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80088768
li r3, 0x0
b @80088944
@80088768
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
li r3, 0x4
bl fn_801CADA8
extsh r0, r3
cmpwi r0, -0x1
beq @800887B4
bge @800887C4
cmpwi r0, -0x2
bge @800887BC
b @800887C4
@800887B4
li r29, 0x1
b @800887C8
@800887BC
li r29, 0x2
b @800887C8
@800887C4
li r29, 0x0
@800887C8
mr r3, r29
bl fn_80266320
cmpwi r29, 0x2
bne @800887FC
bl fn_80128E24
lis r5, 0x2
mr r4, r28
subi r5, r5, 0x2030
bl memcpy
mr r3, r28
bl fn_8007109C
li r3, 0x0
b @80088944
@800887FC
bl fn_8006ADEC
mr r0, r3
li r3, 0x0
mr r29, r0
bl fn_8006ADB4
mr r4, r29
li r3, 0x0
bl fn_801293FC
li r3, 0xafd
bl fn_80190528
@80088824
li r3, 0x2
li r4, 0x3c37
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x0
li r4, -0x1
li r5, -0x1
li r6, 0x0
bl fn_8001E074
extsb r0, r3
cmpwi r0, 0x0
bne @800888D8
li r30, 0x0
mr r29, r30
@80088860
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
blt @80088860
li r31, 0x0
li r3, 0x0
li r4, 0xe
bl fn_80129280
stb r31, 0x1c(r3)
li r3, 0x8ae
li r4, 0x0
bl fn_8019075C
li r3, 0x4
li r4, 0x2
li r5, 0x0
bl fn_801D0748
subi r0, r3, 0x4
cmplwi r0, 0x1
ble @80088938
@800888D8
li r3, 0x2
li r4, 0x3c0f
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x0
li r4, -0x1
li r5, -0x1
li r6, 0x1
bl fn_8001E074
extsb r0, r3
cmpwi r0, 0x0
bne @80088824
bl fn_80128E24
lis r5, 0x2
mr r4, r28
subi r5, r5, 0x2030
bl memcpy
li r3, 0x0
bl fn_8006ADB4
mr r3, r28
bl fn_8007109C
li r3, 0x0
b @80088944
@80088938
mr r3, r28
bl fn_8007109C
li r3, 0x1
@80088944
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
lwz r28, 0x10(r1)
mtlr r0
addi r1, r1, 0x20
blr
