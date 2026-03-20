stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stmw r27, 0xc(r1)
mr r27, r3
mr r28, r4
mr r29, r5
mr r30, r6
b @80097E80
@80097E7C
bl fn_800F0308
@80097E80
bl fn_8010B560
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80097E7C
lis r3, lbl_803FB380@ha
li r4, 0x0
addi r3, r3, lbl_803FB380@l
li r5, 0x44
bl memset
lis r3, lbl_803FB380@ha
li r4, 0xac
addi r31, r3, lbl_803FB380@l
li r9, 0x0
li r0, -0x1
stb r4, 0x0(r31)
mr r4, r27
mr r5, r28
stw r27, 0x8(r31)
mr r7, r29
mr r8, r30
li r3, 0xac
stw r28, 0xc(r31)
li r6, 0x0
sth r9, 0x18(r31)
stw r29, 0x10(r31)
stw r30, 0x14(r31)
stw r0, 0x4(r31)
bl fn_8009769C
lwz r3, 0x4(r31)
lmw r27, 0xc(r1)
lwz r0, 0x24(r1)
mtlr r0
addi r1, r1, 0x20
blr
