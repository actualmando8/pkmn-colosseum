    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lwz r5, 0x60(r3)
    lbz r0, 0x1d(r5)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_8002ACF0
    li r0, 0x0
    li r3, 0x0
    stb r0, 0x67(r31)
    b @L_8002AE30
    @L_8002ACF0:
    lbz r0, 0x1c(r5)
    cmplwi r0, 0x0
    beq @L_8002AD04
    cmplwi r0, 0x1
    bne @L_8002AD4C
    @L_8002AD04:
    li r3, 0x0
    li r4, 0xc
    li r5, 0x0
    bl fn_8012A5B0
    mr r4, r3
    li r3, 0x50
    bl fn_80132A38
    li r3, 0x151
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r31)
    extsh r3, r3
    li r4, 0x0
    subf r3, r3, r0
    li r5, -0x1
    li r6, 0x151
    bl fn_800FB680
    b @L_8002AE2C
    @L_8002AD4C:
    cmpwi r0, 0x3
    beq @L_8002AD78
    bge @L_8002ADE4
    cmpwi r0, 0x2
    bge @L_8002AD64
    b @L_8002ADE4
    @L_8002AD64:
    li r3, 0x0
    li r4, 0xd
    li r5, 0x0
    bl fn_8012A5B0
    b @L_8002ADF4
    @L_8002AD78:
    addi r0, r5, 0x20
    cmplwi r0, 0x0
    beq @L_8002ADDC
    lwz r0, lbl_8047A660(r13)
    cmpwi r0, 0x0
    ble @L_8002ADB8
    lwz r4, 0x778(r5)
    li r0, 0x0
    lwz r3, lbl_8047A660(r13)
    add r3, r4, r3
    stw r3, 0x778(r5)
    lwz r4, 0x77c(r5)
    lwz r3, lbl_8047A660(r13)
    add r3, r4, r3
    stw r3, 0x77c(r5)
    stw r0, lbl_8047A660(r13)
    @L_8002ADB8:
    lwz r0, lbl_8047A664(r13)
    cmpwi r0, 0x0
    ble @L_8002ADD4
    li r0, 0x0
    stw r0, 0x778(r5)
    stw r0, 0x77c(r5)
    stw r0, lbl_8047A664(r13)
    @L_8002ADD4:
    lwz r3, 0x778(r5)
    b @L_8002ADF4
    @L_8002ADDC:
    li r3, 0x0
    b @L_8002ADF4
    @L_8002ADE4:
    li r3, 0x0
    li r4, 0xd
    li r5, 0x0
    bl fn_8012A5B0
    @L_8002ADF4:
    mr r4, r3
    li r3, 0x50
    bl fn_80132A38
    li r3, 0x153
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r31)
    extsh r3, r3
    li r4, 0x0
    subf r3, r3, r0
    li r5, -0x1
    addi r3, r3, 0x6
    li r6, 0x153
    bl fn_800FB680
    @L_8002AE2C:
    li r3, 0x0
    @L_8002AE30:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
