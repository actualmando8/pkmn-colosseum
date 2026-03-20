stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
mr r29, r3
mr r30, r4
b @80097A60
@80097A5C
bl fn_800F0308
@80097A60
bl fn_8010B560
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80097A5C
lis r3, lbl_803FB380@ha
li r4, 0x0
addi r3, r3, lbl_803FB380@l
li r5, 0x44
bl memset
lis r3, lbl_803FB380@ha
li r5, 0x59
addi r31, r3, lbl_803FB380@l
li r4, 0x0
li r0, -0x1
stb r5, 0x0(r31)
li r3, 0x39d
stw r4, 0x8(r31)
stw r29, 0xc(r31)
sth r30, 0x18(r31)
stw r4, 0x10(r31)
stw r4, 0x14(r31)
stw r0, 0x4(r31)
bl fn_800FF730
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r0, 0x0(r3)
rlwinm r0, r0, 0, 28, 28
cmpwi r0, 0x0
beq @80097AE0
li r3, 0x0
li r4, 0x0
bl fn_8011288C
@80097AE0
bl fn_800F0308
lwz r3, 0x4(r31)
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
