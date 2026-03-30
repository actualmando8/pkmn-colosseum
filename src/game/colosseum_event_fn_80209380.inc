    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    li r5, 0xee
    stw r0, 0x24(r1)
    li r6, 0x0
    stmw r29, 0x14(r1)
    mr r29, r3
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_802093B4
    li r3, 0x0
    b @L_802093B8
@L_802093B4:
    bl fn_801DAC3C
@L_802093B8:
    cmplwi r3, 0x0
    mr r31, r3
    beq @L_80209470
    bl fn_800E3D08
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80209470
    li r30, 0x0
    b @L_80209440
@L_802093D8:
    mr r3, r29
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_802093FC
    li r4, 0x1
    bl fn_801DA4E8
@L_802093FC:
    mr r3, r31
    li r4, 0x1
    bl fn_800E4014
    li r3, 0x3
    bl fn_801F000C
    mr r3, r29
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_80209434
    li r4, 0x0
    bl fn_801DA4E8
@L_80209434:
    li r3, 0x2
    bl fn_801F000C
    addi r30, r30, 0x1
@L_80209440:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x8
    blt @L_802093D8
    mr r3, r29
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_80209470
    li r4, 0x1
    bl fn_801DA4E8
@L_80209470:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
