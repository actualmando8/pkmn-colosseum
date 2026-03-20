lwz r0, lbl_80478968@sda21(r0)
cmplw r3, r0
blt @8005D894
li r3, 0x0
b @8005D8A4
@8005D894
mulli r6, r3, 0x1c
lis r3, lbl_802EF0A8@ha
addi r0, r3, lbl_802EF0A8@l
add r3, r0, r6
@8005D8A4
cmplwi r3, 0x0
beqlr
sth r4, 0x2(r3)
sth r5, 0x4(r3)
blr
