    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    lis r3, lbl_80279B84@ha
    stw r0, 0x74(r1)
    addi r3, r3, lbl_80279B84@l
    li r0, 0xa
    addi r5, r1, 0x4
    stmw r29, 0x64(r1)
    subi r4, r3, 0x4
    mtctr r0
@L_801EF2FC:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801EF2FC
    lwz r0, 0x4(r4)
    addi r31, r1, 0x8
    li r29, 0x0
    stw r0, 0x4(r5)
    b @L_801EF354
@L_801EF324:
    clrlslwi r0, r29, 24, 2
    lwzx r30, r31, r0
    mr r3, r30
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801EF350
    mr r3, r30
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
@L_801EF350:
    addi r29, r29, 0x1
@L_801EF354:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x15
    blt @L_801EF324
    lmw r29, 0x64(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
