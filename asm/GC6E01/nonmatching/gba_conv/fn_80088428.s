stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
li r3, 0x27
bl fn_80166A28
lis r4, 0x107c
li r3, 0x62d
addi r4, r4, 0x1800
li r5, 0x0
li r6, 0x0
bl fn_80176E0C
li r3, 0x1
bl fn_80176B48
lis r3, lbl_803FB2F8@ha
addi r3, r3, lbl_803FB2F8@l
bl fn_80087C64
mr r31, r3
li r3, 0x28
bl fn_80166A28
lis r4, 0x107d
li r3, 0x62d
addi r4, r4, 0x1800
li r5, 0x0
li r6, 0x0
bl fn_80176E0C
li r3, 0x1
bl fn_80176B48
bl fn_800FF660
lis r3, lbl_803FB2F8@ha
addi r3, r3, lbl_803FB2F8@l
stw r31, 0x8(r3)
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
