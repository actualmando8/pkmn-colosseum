    nofralloc
    clrlwi r0, r4, 24
    li r4, 0x0
    cmpwi r0, 0x3
    beq @L_8005D7DC
    bge @L_8005D7BC
    cmpwi r0, 0x1
    beq @L_8005D7CC
    bge @L_8005D7D4
    b @L_8005D7F0
@L_8005D7BC:
    cmpwi r0, 0x5
    beq @L_8005D7EC
    bge @L_8005D7F0
    b @L_8005D7E4
@L_8005D7CC:
    lhz r4, 0x4(r3)
    b @L_8005D7F0
@L_8005D7D4:
    lhz r4, 0x6(r3)
    b @L_8005D7F0
@L_8005D7DC:
    lhz r4, 0x8(r3)
    b @L_8005D7F0
@L_8005D7E4:
    lhz r4, 0x0(r3)
    b @L_8005D7F0
@L_8005D7EC:
    lhz r4, 0x2(r3)
@L_8005D7F0:
    mr r3, r4
    blr
