clrlwi r0, r4, 24
li r4, 0x0
cmpwi r0, 0x3
beq @8005D7DC
bge @8005D7BC
cmpwi r0, 0x1
beq @8005D7CC
bge @8005D7D4
b @8005D7F0
@8005D7BC
cmpwi r0, 0x5
beq @8005D7EC
bge @8005D7F0
b @8005D7E4
@8005D7CC
lhz r4, 0x4(r3)
b @8005D7F0
@8005D7D4
lhz r4, 0x6(r3)
b @8005D7F0
@8005D7DC
lhz r4, 0x8(r3)
b @8005D7F0
@8005D7E4
lhz r4, 0x0(r3)
b @8005D7F0
@8005D7EC
lhz r4, 0x2(r3)
@8005D7F0
mr r3, r4
blr
