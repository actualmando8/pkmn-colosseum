cmpwi r3, 0x0
bge @8005D9F0
li r3, 0x0
@8005D9F0
lwz r0, lbl_80478848@sda21(r0)
cmplw r3, r0
blt @8005DA00
li r3, 0x1
@8005DA00
mulli r4, r3, 0x1c
lis r3, lbl_802E2DB8@ha
addi r0, r3, lbl_802E2DB8@l
add r3, r0, r4
lbz r3, 0x2(r3)
blr
