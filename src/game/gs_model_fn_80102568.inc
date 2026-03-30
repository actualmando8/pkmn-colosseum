    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    bl fn_80104704
    cmplwi r3, 0x0
    bne @L_801025A0
    li r3, 0x1
    b @L_80102604
@L_801025A0:
    mr r4, r30
    bl fn_80104828
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801025F8
@L_801025B4:
    mr r3, r29
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_80102600
    bl fn_800F037C
    cmplwi r3, 0x0
    bne @L_801025F0
    lis r3, lbl_80271E10@ha
    lis r4, lbl_8035B060@ha
    addi r3, r3, lbl_80271E10@l
    mr r5, r29
    addi r4, r4, lbl_8035B060@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_80102600
@L_801025F0:
    bl fn_800F0308
    b @L_801025B4
@L_801025F8:
    mr r3, r29
    bl fn_80104704
@L_80102600:
    li r3, 0x0
@L_80102604:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
