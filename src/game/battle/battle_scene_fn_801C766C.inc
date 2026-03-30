    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    li r3, 0x1
    bl fn_8012F11C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C7698
    li r0, 0x1
    b @L_801C769C
@L_801C7698:
    li r0, 0x0
@L_801C769C:
    rlwinm r5, r0, 0, 24, 31
    li r3, 0x0
    li r4, 0x18
    bl fn_8012A450
    li r3, 0x0
    li r4, 0x1
    bl fn_80129280
    bl fn_80135CD0
    mr r0, r3
    addi r3, r1, 0x10
    mr r31, r0
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_8011418C
    lwz r4, 0x10(r1)
    mr r3, r31
    bl fn_80135B6C
    lwz r4, 0xc(r1)
    mr r3, r31
    bl fn_80135B4C
    lbz r4, 0x8(r1)
    mr r3, r31
    bl fn_80135B3C
    li r3, 0x4
    li r4, 0x2
    li r5, 0x0
    bl fn_801D0748
    cmpwi r3, 0x4
    bne @L_801C7718
    li r3, 0x1
    b @L_801C771C
@L_801C7718:
    li r3, 0x0
@L_801C771C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
