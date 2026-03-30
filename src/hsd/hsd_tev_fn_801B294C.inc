    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    stw r0, 0xa4(r1)
    stw r31, 0x9c(r1)
    mr r31, r4
    stw r30, 0x98(r1)
    mr r30, r3
    bl fn_801B387C
    cmpwi r3, 0x0
    bne @L_801B29AC
    li r0, 0x0
    stw r0, 0xc(r1)
    bl fn_801B37A0
    li r6, 0xff
    li r5, 0xff
    li r4, 0x4
    li r0, 0x4
    stw r3, 0x10(r1)
    addi r3, r1, 0x8
    stw r6, 0x14(r1)
    stw r5, 0x18(r1)
    stw r4, 0x1c(r1)
    stw r0, 0x20(r1)
    bl fn_801B3638
@L_801B29AC:
    mr r3, r30
    mr r4, r31
    bl fn_801B29E4
    bl fn_801B3258
    bl fn_801B3770
    bl fn_801B3890
    mr r3, r30
    bl fn_801B2F1C
    lwz r0, 0xa4(r1)
    lwz r31, 0x9c(r1)
    lwz r30, 0x98(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
