stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
mr r29, r3
mr r30, r4
b @80097B2C
@80097B28
bl fn_800F0308
@80097B2C
bl fn_8010B560
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80097B28
lis r3, lbl_803FB380@ha
li r4, 0x0
addi r3, r3, lbl_803FB380@l
li r5, 0x44
bl memset
lis r3, lbl_803FB380@ha
li r4, 0x58
addi r31, r3, lbl_803FB380@l
li r9, 0x0
li r0, -0x1
stb r4, 0x0(r31)
mr r5, r29
mr r6, r30
stw r9, 0x8(r31)
li r3, 0x58
li r4, 0x0
li r7, 0x0
stw r29, 0xc(r31)
li r8, 0x0
sth r30, 0x18(r31)
stw r9, 0x10(r31)
stw r9, 0x14(r31)
stw r0, 0x4(r31)
bl fn_8009769C
lwz r3, 0x4(r31)
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
