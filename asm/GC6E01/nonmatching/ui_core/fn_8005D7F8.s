lwz r5, lbl_80478E94@sda21(r0)
cmplwi r5, 0x0
bne @8005D80C
li r3, 0x0
blr
@8005D80C
lwz r4, lbl_80478E90@sda21(r0)
lwz r0, 0x0(r4)
cmplw r3, r0
blt @8005D824
li r3, 0x0
blr
@8005D824
mulli r0, r3, 0xa
add r3, r5, r0
blr
