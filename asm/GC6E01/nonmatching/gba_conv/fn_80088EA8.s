stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stw r31, 0x2c(r1)
mr r31, r3
addi r3, r1, 0x14
bl fn_8012D32C
addi r3, r1, 0x8
bl fn_8012D2BC
lfs f0, 0x14(r1)
stfs f0, 0xc(r31)
lfs f0, 0x18(r1)
stfs f0, 0x10(r31)
lfs f0, 0x1c(r1)
stfs f0, 0x14(r31)
lfs f0, 0x8(r1)
stfs f0, 0x18(r31)
lfs f0, 0xc(r1)
stfs f0, 0x1c(r31)
lfs f0, 0x10(r1)
stfs f0, 0x20(r31)
bl fn_800FF56C
stw r3, 0x4(r31)
bl fn_8011394C
stw r3, 0x8(r31)
li r0, 0x1
li r3, 0xafc
stb r0, 0x0(r31)
bl fn_801906A0
stw r3, 0x24(r31)
li r3, 0xafd
bl fn_801906A0
stw r3, 0x28(r31)
li r3, 0xb11
bl fn_801906A0
stw r3, 0x2c(r31)
li r3, 0xde1
bl fn_801906A0
stw r3, 0x30(r31)
lwz r0, 0x34(r1)
lwz r31, 0x2c(r1)
mtlr r0
addi r1, r1, 0x30
blr
