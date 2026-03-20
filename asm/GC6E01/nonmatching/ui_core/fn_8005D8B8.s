lwz r0, lbl_80478968@sda21(r0)
cmplw r3, r0
blt @8005D8CC
li r3, 0x0
b @8005D8DC
@8005D8CC
mulli r4, r3, 0x1c
lis r3, lbl_802EF0A8@ha
addi r0, r3, lbl_802EF0A8@l
add r3, r0, r4
@8005D8DC
cmplwi r3, 0x0
beq @8005D8F0
lbz r0, 0x0(r3)
extrwi r3, r0, 1, 24
blr
@8005D8F0
li r3, 0x0
blr
