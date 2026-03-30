    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    li r28, 0x0
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    li r29, 0x0
    mr r31, r3
    lfs f31, -21900(r2)
    b @L_801CAF9C
@L_801CAF50:
    mr r3, r31
    mr r4, r29
    bl fn_8012AC08
    mr r30, r3
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CAF98
    mr r3, r30
    bl fn_8011ED68
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CAF98
    mr r3, r30
    bl fn_8011FC14
    fcmpu cr0, f31, f1
    bne @L_801CAF98
    addi r28, r28, 0x1
@L_801CAF98:
    addi r29, r29, 0x1
@L_801CAF9C:
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_801CAF50
    rlwinm r3, r28, 0, 16, 31
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
