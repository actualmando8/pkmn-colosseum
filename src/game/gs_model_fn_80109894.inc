    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_801098B8
    li r3, 0x0
    b @L_80109920
@L_801098B8:
    lbz r0, 0x1(r31)
    cmplwi r0, 0x0
    beq @L_801098D8
    lbz r0, 0x4(r31)
    cmplwi r0, 0x0
    bne @L_8010991C
    stw r4, 0xc(r31)
    b @L_8010991C
@L_801098D8:
    lbz r0, 0x14(r31)
    cmplwi r0, 0x0
    bne @L_8010991C
    stw r4, 0x1c(r31)
    lwz r3, 0x24(r31)
    bl fn_800EC1BC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8010991C
    lwz r3, 0x24(r31)
    lwz r4, 0x1c(r31)
    bl fn_800ECCA8
    lwz r3, 0x24(r31)
    lfs f1, lbl_8047CE70(r13)
    bl fn_800EC9DC
    lwz r3, 0x24(r31)
    bl fn_800EC990
@L_8010991C:
    li r3, 0x1
@L_80109920:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
