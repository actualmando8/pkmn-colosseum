stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
bl fn_800E202C
mr r31, r3
clrlwi r0, r31, 16
cmplwi r0, 0x0
bne @800710D4
lis r3, lbl_80268708@ha
li r4, 0xde
addi r3, r3, lbl_80268708@l
li r5, lbl_8047C090@sda21
bl fn_80196E10
@800710D4
clrlwi r0, r31, 16
cmplwi r0, 0x0
beq @800710F0
mr r3, r31
bl fn_800E24B0
mr r3, r31
bl fn_800E209C
@800710F0
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
