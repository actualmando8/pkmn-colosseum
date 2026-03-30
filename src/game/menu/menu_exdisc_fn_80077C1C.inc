    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    rlwinm r0, r3, 0, 16, 31
    cmpwi r0, 0xaf
    beq @L_80077C4C
    bge @L_80077C54
    cmpwi r0, 0x0
    beq @L_80077C44
    b @L_80077C54
@L_80077C44:
    li r3, 0x1
    b @L_80077C58
@L_80077C4C:
    li r3, 0x0
    b @L_80077C58
@L_80077C54:
    bl fn_80142984
@L_80077C58:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
