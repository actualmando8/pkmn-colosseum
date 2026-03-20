stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
lis r4, lbl_803B6D88@ha
addi r5, r4, lbl_803B6D88@l
lwz r4, 0x40(r5)
cmplwi r4, 0x8
blt @800715F8
lis r3, lbl_80268708@ha
lis r5, lbl_80268750@ha
addi r3, r3, lbl_80268708@l
li r4, 0x41
addi r5, r5, lbl_80268750@l
bl fn_80196E10
b @8007161C
@800715F8
addi r0, r4, 0x1
li r4, 0x0
stw r0, 0x40(r5)
slwi r0, r0, 3
stwx r3, r5, r0
lwz r0, 0x40(r5)
slwi r0, r0, 3
add r3, r5, r0
stw r4, 0x4(r3)
@8007161C
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
