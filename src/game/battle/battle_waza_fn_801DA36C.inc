    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    beq @L_801DA3BC
    rlwinm r0, r4, 0, 24, 31
    cmpwi r0, 0x2
    beq @L_801DA3B0
    bge @L_801DA39C
    cmpwi r0, 0x1
    bge @L_801DA3A8
    b @L_801DA3BC
@L_801DA39C:
    cmpwi r0, 0x4
    bge @L_801DA3BC
    b @L_801DA3B8
@L_801DA3A8:
    bl fn_801DD028
    b @L_801DA3BC
@L_801DA3B0:
    bl fn_801DCF84
    b @L_801DA3BC
@L_801DA3B8:
    bl fn_801DCEA8
@L_801DA3BC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
