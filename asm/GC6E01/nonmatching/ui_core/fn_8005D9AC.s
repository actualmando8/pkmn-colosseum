cmpwi r3, 0x0
bge @8005D9B8
li r3, 0x0
@8005D9B8
lwz r0, lbl_80478848@sda21(r0)
cmplw r3, r0
blt @8005D9C8
li r3, 0x1
@8005D9C8
mulli r6, r3, 0x1c
lis r3, lbl_802E2DB8@ha
addi r0, r3, lbl_802E2DB8@l
add r3, r0, r6
sth r4, 0x6(r3)
sth r5, 0x8(r3)
blr
