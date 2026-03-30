    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80102494
@L_80102448:
    mr r3, r31
    bl fn_80104704
    cmplwi r3, 0x0
    bne @L_80102460
    li r3, 0x0
    b @L_801024AC
@L_80102460:
    bl fn_800F037C
    cmplwi r3, 0x0
    bne @L_8010248C
    lis r3, lbl_80271E10@ha
    lis r4, lbl_8035B060@ha
    addi r3, r3, lbl_80271E10@l
    mr r5, r31
    addi r4, r4, lbl_8035B060@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801024A8
@L_8010248C:
    bl fn_800F0308
    b @L_80102448
@L_80102494:
    bl fn_80104704
    neg r0, r3
    or r0, r0, r3
    rlwinm r3, r0, 1, 31, 31
    b @L_801024AC
@L_801024A8:
    li r3, 0x0
@L_801024AC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
