    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stmw r27, 0x1c(r1)
    lhz r0, 0x70(r3)
    mr r31, r5
    li r8, 0x0
    cmplwi r0, 0x13a
    bne @L_801D4DD8
    li r0, 0x4
    stw r0, 0x0(r31)
    b @L_801D4F9C
@L_801D4DD8:
    cmplwi r4, 0x0
    beq @L_801D4E38
    lwz r3, 0x8(r4)
    rlwinm. r0, r3, 0, 31, 31
    beq @L_801D4DF8
    li r0, 0x5
    stw r0, 0x0(r31)
    b @L_801D4F9C
@L_801D4DF8:
    rlwinm. r0, r3, 29, 31, 31
    mr r4, r0
    beq @L_801D4E08
    li r8, 0x1
@L_801D4E08:
    rlwinm. r0, r3, 28, 31, 31
    mr r5, r0
    beq @L_801D4E18
    addi r8, r8, 0x1
@L_801D4E18:
    rlwinm. r0, r3, 27, 31, 31
    mr r6, r0
    beq @L_801D4E28
    addi r8, r8, 0x1
@L_801D4E28:
    rlwinm. r0, r3, 26, 31, 31
    mr r7, r0
    beq @L_801D4E38
    addi r8, r8, 0x1
@L_801D4E38:
    cmpwi r8, 0x0
    bne @L_801D4E58
    li r8, 0x4
    li r4, 0x1
    li r5, 0x1
    li r6, 0x1
    li r7, 0x1
    b @L_801D4EAC
@L_801D4E58:
    cmpwi r8, 0x1
    bne @L_801D4EAC
    rlwinm. r0, r4, 0, 24, 31
    beq @L_801D4E70
    li r0, 0x3
    stw r0, 0x0(r31)
@L_801D4E70:
    rlwinm. r0, r5, 0, 24, 31
    beq @L_801D4E80
    li r0, 0x0
    stw r0, 0x0(r31)
@L_801D4E80:
    rlwinm. r0, r6, 0, 24, 31
    beq @L_801D4E90
    li r0, 0x1
    stw r0, 0x0(r31)
@L_801D4E90:
    rlwinm. r0, r7, 0, 24, 31
    beq @L_801D4EA0
    li r0, 0x2
    stw r0, 0x0(r31)
@L_801D4EA0:
    lwz r0, 0x0(r31)
    stw r0, -31560(r13)
    b @L_801D4F9C
@L_801D4EAC:
    xoris r3, r8, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    rlwinm r27, r4, 0, 24, 31
    lfd f1, -21576(r2)
    rlwinm r28, r5, 0, 24, 31
    stw r0, 0x8(r1)
    rlwinm r29, r6, 0, 24, 31
    lfs f2, -21676(r2)
    rlwinm r30, r7, 0, 24, 31
    lfd f0, 0x8(r1)
    fsubs f0, f0, f1
    fdivs f31, f2, f0
@L_801D4EE0:
    bl fn_800E0BE4
    fmr f0, f31
    cmplwi r27, 0x0
    beq @L_801D4F14
    fcmpo cr0, f31, f1
    ble @L_801D4F10
    li r0, 0x3
    stw r0, 0x0(r31)
    lwz r3, -31560(r13)
    lwz r0, 0x0(r31)
    cmpw r3, r0
    bne @L_801D4F94
@L_801D4F10:
    fadds f0, f0, f31
@L_801D4F14:
    cmplwi r28, 0x0
    beq @L_801D4F40
    fcmpo cr0, f0, f1
    ble @L_801D4F3C
    li r0, 0x0
    stw r0, 0x0(r31)
    lwz r3, -31560(r13)
    lwz r0, 0x0(r31)
    cmpw r3, r0
    bne @L_801D4F94
@L_801D4F3C:
    fadds f0, f0, f31
@L_801D4F40:
    cmplwi r29, 0x0
    beq @L_801D4F6C
    fcmpo cr0, f0, f1
    ble @L_801D4F68
    li r0, 0x1
    stw r0, 0x0(r31)
    lwz r3, -31560(r13)
    lwz r0, 0x0(r31)
    cmpw r3, r0
    bne @L_801D4F94
@L_801D4F68:
    fadds f0, f0, f31
@L_801D4F6C:
    cmplwi r30, 0x0
    beq @L_801D4EE0
    fcmpo cr0, f0, f1
    ble @L_801D4EE0
    li r0, 0x2
    stw r0, 0x0(r31)
    lwz r3, -31560(r13)
    lwz r0, 0x0(r31)
    cmpw r3, r0
    beq @L_801D4EE0
@L_801D4F94:
    lwz r0, 0x0(r31)
    stw r0, -31560(r13)
@L_801D4F9C:
    psq_l f31, 0x38(r1), 0, 0
    lfd f31, 0x30(r1)
    lmw r27, 0x1c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
