stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
li r3, 0x0
li r4, 0xe
bl fn_80129280
addis r31, r3, 0x1
li r3, 0x3e8
subi r31, r31, 0x3658
bl fn_801657D0
mr r3, r31
bl fn_80088EA8
li r3, 0xb59
bl fn_801906A0
mr r31, r3
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r31, 0x14(r3)
li r31, 0x6
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r31, 0xc(r3)
li r31, 0x1
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r31, 0x0(r3)
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_80069C0C
li r3, 0x397
bl fn_800FF730
bl fn_800F0308
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x20(r3)
clrlwi r3, r0, 16
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
