stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
lis r3, lbl_803FB380@ha
addi r8, r3, lbl_803FB380@l
lbz r3, 0x0(r8)
lwz r4, 0x8(r8)
lwz r5, 0xc(r8)
lhz r6, 0x18(r8)
lwz r7, 0x10(r8)
lwz r8, 0x14(r8)
bl fn_8009769C
lis r4, lbl_803FB380@ha
addi r4, r4, lbl_803FB380@l
stw r3, 0x4(r4)
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
