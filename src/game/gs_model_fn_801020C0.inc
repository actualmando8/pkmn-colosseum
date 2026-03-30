    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r31, 0x0
    bl fn_8005DA18
    cmplwi r3, 0x0
    bne @L_801020E8
    li r3, 0x0
    b @L_80102124
@L_801020E8:
    lha r0, 0x4(r3)
@L_801020EC:
    extsh r3, r0
    bl fn_8005D934
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    beq @L_80102108
    addi r31, r31, 0x1
@L_80102108:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_80102120
    lha r0, 0x18(r3)
    b @L_801020EC
@L_80102120:
    mr r3, r31
@L_80102124:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
