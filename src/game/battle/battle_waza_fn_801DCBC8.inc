    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r30, r5
    mr r29, r4
    mr r31, r30
    bl fn_801DB060
    mr r28, r3
    bl fn_801DB060
    li r0, 0x4e20
    addi r4, r30, 0x43
    stw r0, 0x78(r29)
    li r0, 0x0
    rlwinm r30, r4, 0, 0, 26
    mr r27, r3
    stw r0, 0x80(r29)
    mr r3, r30
    mr r6, r28
    li r5, 0x4e20
    stw r0, 0x7c(r29)
    stw r0, 0xa4(r29)
    lwz r4, 0x1c(r31)
    bl fn_8010147C
    mr r4, r28
    li r3, 0x4e20
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801DCC84
    stw r28, 0x80(r29)
    mr r4, r28
    li r3, 0x4e20
    bl fn_800F9318
    mr r6, r27
    li r4, 0x4e20
    li r5, 0x0
    bl fn_801013A0
    mr r4, r27
    li r3, 0x4e20
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801DCC84
    stw r27, 0x7c(r29)
    mr r4, r27
    li r3, 0x4e20
    bl fn_800F9318
    stw r3, 0xa4(r29)
@L_801DCC84:
    lwz r3, 0x1c(r31)
    lwz r0, 0x0(r31)
    addi r3, r3, 0x1f
    stw r0, 0x88(r29)
    rlwinm r0, r3, 0, 0, 26
    add r30, r30, r0
    lwz r0, 0x4(r31)
    cmpwi r0, 0x1
    beq @L_801DCCC4
    bge @L_801DCCCC
    cmpwi r0, 0x0
    bge @L_801DCCB8
    b @L_801DCCCC
@L_801DCCB8:
    li r0, 0x0
    stw r0, 0x84(r29)
    b @L_801DCCCC
@L_801DCCC4:
    li r0, 0x1
    stw r0, 0x84(r29)
@L_801DCCCC:
    lwz r0, 0x8(r31)
    stw r0, 0x90(r29)
    lwz r0, 0xc(r31)
    cmpwi r0, 0x1
    beq @L_801DCCFC
    bge @L_801DCD04
    cmpwi r0, 0x0
    bge @L_801DCCF0
    b @L_801DCD04
@L_801DCCF0:
    li r0, 0x0
    stw r0, 0x8c(r29)
    b @L_801DCD04
@L_801DCCFC:
    li r0, 0x1
    stw r0, 0x8c(r29)
@L_801DCD04:
    lwz r0, 0x10(r31)
    stw r0, 0x94(r29)
    lwz r0, 0x14(r31)
    stw r0, 0x98(r29)
    lwz r0, 0x18(r31)
    stw r0, 0x9c(r29)
    lwz r0, 0x14(r29)
    rlwinm r0, r0, 2, 0, 29
    add r3, r29, r0
    lwz r0, 0x28(r3)
    cmpwi r0, 0x0
    bne @L_801DCD90
    lfs f0, -21232(r2)
    stfs f0, 0xc(r1)
    stfs f0, 0x8(r1)
    lwz r3, 0xa4(r29)
    cmplwi r3, 0x0
    beq @L_801DCD58
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_800EC4D0
@L_801DCD58:
    lfs f0, 0xc(r1)
    lfs f1, 0x8(r1)
    fcmpo cr0, f0, f1
    cror eq, gt, eq
    bne @L_801DCD70
    b @L_801DCD74
@L_801DCD70:
    fmr f0, f1
@L_801DCD74:
    fctiwz f0, f0
    lwz r0, 0x14(r29)
    rlwinm r0, r0, 2, 0, 29
    stfd f0, 0x10(r1)
    add r3, r29, r0
    lwz r0, 0x14(r1)
    stw r0, 0x28(r3)
@L_801DCD90:
    mr r3, r30
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
