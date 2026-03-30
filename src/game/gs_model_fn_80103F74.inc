    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r5
    cmplwi r3, 0x0
    beq @L_80103FC8
    lwz r30, 0x1c(r3)
    rlwinm r31, r4, 0, 16, 31
    b @L_80103FC0
@L_80103FA4:
    lha r0, 0x6(r30)
    cmpw r0, r31
    bne @L_80103FBC
    mr r3, r30
    mr r4, r29
    bl fn_80109220
@L_80103FBC:
    lwz r30, 0x0(r30)
@L_80103FC0:
    cmplwi r30, 0x0
    bne @L_80103FA4
@L_80103FC8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
