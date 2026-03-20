stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
bl fn_80105624
lis r3, lbl_803A9EA0@ha
li r30, 0x0
addi r31, r3, lbl_803A9EA0@l
@8006875C
mr r3, r31
addi r4, r30, 0x1
bl fn_80068418
addi r31, r31, 0x1a
addi r30, r30, 0x1
cmpwi r30, 0x4
blt @8006875C
li r3, 0x0
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
