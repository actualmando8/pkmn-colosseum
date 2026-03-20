stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
addi r0, r3, 0x1f
li r4, 0x20
clrrwi r3, r0, 5
bl fn_800E2C04
clrlwi r0, r3, 16
cmplwi r0, 0x0
beq @80071134
bl fn_800E27B0
b @80071150
@80071134
bne @8007114C
lis r3, lbl_80268708@ha
li r4, 0xd5
addi r3, r3, lbl_80268708@l
li r5, lbl_8047C090@sda21
bl fn_80196E10
@8007114C
li r3, 0x0
@80071150
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
