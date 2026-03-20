lwz r5, 0x0(r3)
lwz r4, 0x0(r4)
lbz r3, 0x1c(r5)
lbz r0, 0x1c(r4)
extsb r3, r3
extsb r0, r0
cmpw r3, r0
bge @8007FD8C
li r3, 0x1
blr
@8007FD8C
ble @8007FD98
li r3, -0x1
blr
@8007FD98
lbz r3, 0x1a(r5)
lbz r0, 0x1a(r4)
cmplw r3, r0
bge @8007FDB0
li r3, -0x1
blr
@8007FDB0
subf r0, r3, r0
srwi r3, r0, 31
blr
