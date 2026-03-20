stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
lis r3, lbl_803B6D88@ha
addi r31, r3, lbl_803B6D88@l
lwz r0, 0x40(r31)
slwi r0, r0, 3
lwzx r30, r31, r0
bl fn_801046B8
cmpw r3, r30
bne @8007151C
lwz r0, 0x40(r31)
lis r3, lbl_803B6D88@ha
addi r3, r3, lbl_803B6D88@l
li r4, 0x0
slwi r0, r0, 3
li r5, 0x0
lwzx r3, r3, r0
bl fn_80102568
@8007151C
li r3, 0xbe
bl fn_80104704
cmplwi r3, 0x0
beq @8007153C
li r3, 0xbe
li r4, 0x0
li r5, 0x1
bl fn_80102568
@8007153C
lwz r0, 0x40(r31)
lis r3, lbl_803B6D88@ha
addi r3, r3, lbl_803B6D88@l
li r4, 0x0
slwi r0, r0, 3
add r3, r3, r0
stw r4, 0x4(r3)
lwz r0, 0x40(r31)
cmpwi r0, 0x0
bne @8007156C
li r3, -0x1
b @800715A4
@8007156C
bgt @80071588
lis r3, lbl_80268708@ha
lis r5, lbl_80268718@ha
addi r3, r3, lbl_80268708@l
li r4, 0x5c
addi r5, r5, lbl_80268718@l
bl fn_80196E10
@80071588
lwz r4, 0x40(r31)
lis r3, lbl_803B6D88@ha
addi r3, r3, lbl_803B6D88@l
subi r0, r4, 0x1
stw r0, 0x40(r31)
slwi r0, r0, 3
lwzx r3, r3, r0
@800715A4
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
