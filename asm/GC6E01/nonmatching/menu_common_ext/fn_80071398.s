stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
mr r30, r3
lis r3, lbl_803B6D88@ha
addi r31, r3, lbl_803B6D88@l
lwz r0, 0x40(r31)
slwi r0, r0, 3
lwzx r29, r31, r0
bl fn_801046B8
cmpw r3, r29
bne @800713F4
lwz r0, 0x40(r31)
lis r3, lbl_803B6D88@ha
addi r3, r3, lbl_803B6D88@l
li r4, 0x0
slwi r0, r0, 3
li r5, 0x0
lwzx r3, r3, r0
bl fn_80102568
@800713F4
li r3, 0xbe
bl fn_80104704
cmplwi r3, 0x0
beq @80071414
li r3, 0xbe
li r4, 0x0
li r5, 0x1
bl fn_80102568
@80071414
lwz r0, 0x40(r31)
lis r3, lbl_803B6D88@ha
addi r3, r3, lbl_803B6D88@l
li r4, 0x0
slwi r0, r0, 3
add r3, r3, r0
stw r4, 0x4(r3)
lwz r0, 0x40(r31)
cmpwi r0, 0x0
beq @80071464
bgt @80071458
lis r3, lbl_80268708@ha
lis r5, lbl_80268718@ha
addi r3, r3, lbl_80268708@l
li r4, 0x5c
addi r5, r5, lbl_80268718@l
bl fn_80196E10
@80071458
lwz r3, 0x40(r31)
subi r0, r3, 0x1
stw r0, 0x40(r31)
@80071464
lis r3, lbl_803B6D88@ha
addi r3, r3, lbl_803B6D88@l
b @80071484
@80071470
cmpwi r4, 0x0
beq @80071498
lwz r4, 0x40(r31)
subi r0, r4, 0x1
stw r0, 0x40(r31)
@80071484
lwz r4, 0x40(r31)
slwi r0, r4, 3
lwzx r0, r3, r0
cmpw r30, r0
bne @80071470
@80071498
lwz r0, 0x40(r31)
lis r3, lbl_803B6D88@ha
addi r3, r3, lbl_803B6D88@l
slwi r0, r0, 3
lwzx r3, r3, r0
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
