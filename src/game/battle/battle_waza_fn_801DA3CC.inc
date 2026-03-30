    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    beq @L_801DA41C
    rlwinm r0, r4, 0, 24, 31
    cmpwi r0, 0x2
    beq @L_801DA410
    bge @L_801DA3FC
    cmpwi r0, 0x1
    bge @L_801DA408
    b @L_801DA41C
@L_801DA3FC:
    cmpwi r0, 0x4
    bge @L_801DA41C
    b @L_801DA418
@L_801DA408:
    bl fn_801DD078
    b @L_801DA41C
@L_801DA410:
    bl fn_801DCFD8
    b @L_801DA41C
@L_801DA418:
    bl fn_801DCF00
@L_801DA41C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
