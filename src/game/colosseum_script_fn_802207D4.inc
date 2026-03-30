    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0xfb
    stw r0, 0x14(r1)
    li r6, 0x0
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_8012640C
    rlwinm. r4, r3, 0, 16, 31
    beq @L_80220850
    mr r3, r31
    li r5, 0x1
    li r6, 0x0
    bl fn_8020147C
    mr r3, r31
    li r4, 0x0
    li r5, 0xfb
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80220850
    mr r3, r31
    li r4, 0x82
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_80220850:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
