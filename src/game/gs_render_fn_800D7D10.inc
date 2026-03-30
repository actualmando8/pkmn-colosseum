    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r5, lbl_8047AA80(r13)
    lwz r0, 0x0(r5)
    cmpwi r0, 0x1
    bne @L_800D7D48
    mr r6, r4
    clrlwi r5, r3, 24
    li r3, 0x44
    li r4, 0x2
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D7D80
    @L_800D7D48:
    clrlwi r0, r3, 24
    cmplwi r0, 0x9
    ble @L_800D7D68
    lis r3, lbl_80270440@ha
    addi r3, r3, lbl_80270440@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_800D7D80
    @L_800D7D68:
    mulli r6, r0, 0x30
    lis r5, lbl_80400948@ha
    mr r3, r4
    addi r0, r5, lbl_80400948@l
    add r4, r0, r6
    bl fn_800E0628
    @L_800D7D80:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
