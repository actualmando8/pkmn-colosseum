lis r4, lbl_80267DD8@ha
addi r4, r4, lbl_80267DD8@l
lwz r0, 0x0(r4)
cmpw r3, r0
bne @8006B170
li r3, 0x0
blr
@8006B170
addi r4, r4, 0x4
lwz r0, 0x0(r4)
cmpw r3, r0
bne @8006B188
li r3, 0x1
blr
@8006B188
addi r4, r4, 0x4
lwz r0, 0x0(r4)
cmpw r3, r0
bne @8006B1A0
li r3, 0x2
blr
@8006B1A0
addi r4, r4, 0x4
lwz r0, 0x0(r4)
cmpw r3, r0
bne @8006B1B8
li r3, 0x3
blr
@8006B1B8
li r3, -0x1
blr
