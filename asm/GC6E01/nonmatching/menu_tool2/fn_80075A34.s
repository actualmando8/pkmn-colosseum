stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
bl fn_80113F48
lis r4, 0x1080
mr r31, r3
addi r3, r4, 0x1000
bl fn_801CBA0C
mr r4, r3
mr r3, r31
stw r4, lbl_8047A5D0@sda21(r0)
bl fn_800F9318
lis r4, 0x1082
li r3, 0x5e0
addi r4, r4, 0x1800
li r5, 0x0
li r6, 0x1
bl fn_80176E0C
li r3, 0x4
bl fn_80177A44
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
