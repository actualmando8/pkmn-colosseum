    nofralloc
    rlwinm r0, r3, 0, 24, 31
    cmpwi r0, 0x90
    beq @L_8021C078
    bge @L_8021C05C
    cmpwi r0, 0x20
    beq @L_8021C070
    bge @L_8021C088
    cmpwi r0, 0x10
    beq @L_8021C068
    b @L_8021C088
@L_8021C05C:
    cmpwi r0, 0xa0
    beq @L_8021C080
    b @L_8021C088
@L_8021C068:
    li r3, 0x1
    blr
@L_8021C070:
    li r3, 0x2
    blr
@L_8021C078:
    li r3, -0x1
    blr
@L_8021C080:
    li r3, -0x2
    blr
@L_8021C088:
    li r3, 0x0
    blr
