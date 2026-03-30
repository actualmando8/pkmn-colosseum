    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r3, 0x4(r30)
    bl fn_8005DA18
    lha r3, 0x4(r3)
    bl fn_8005D934
    li r31, 0x0
@L_80104344:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    beq @L_8010436C
    lbz r0, 0x95(r30)
    extsb r0, r0
    cmpw r0, r31
    bne @L_80104368
    b @L_8010438C
@L_80104368:
    addi r31, r31, 0x1
@L_8010436C:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_80104388
    lha r3, 0x18(r3)
    bl fn_8005D934
    b @L_80104344
@L_80104388:
    li r3, 0x0
@L_8010438C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
